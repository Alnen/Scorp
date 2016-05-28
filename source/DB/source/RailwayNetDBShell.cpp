#include "Scorp/DB/RailwayNetDBShell.h"
#include "Scorp/Exceptions/ScorpDBException.h"

RailwayNetDBShell::RailwayNetDBShell()
    : ScorpDBShell()
{
}

RailwayNetDBShell::~RailwayNetDBShell()
{
}

bool RailwayNetDBShell::createDatabase(const std::string& path)
{
    m_databasePath = path;
    try
    {
    m_database = std::move(std::unique_ptr<SQLite::Database>(new SQLite::Database(path,
                                                             SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE)));
    (*m_database).exec(std::string("CREATE TABLE Route (Id INTEGER (1, 30) PRIMARY KEY UNIQUE NOT NULL,")
        + " Name STRING (1, 50) NOT NULL)");
    (*m_database).exec(std::string("CREATE TABLE RoutePart (Id INTEGER (1, 30) PRIMARY KEY NOT NULL UNIQUE,")
        + " Route INTEGER (1, 30) NOT NULL REFERENCES Route (Id) ON DELETE NO ACTION ON UPDATE NO ACTION"
        + " MATCH SIMPLE, Transition INTEGER (1, 30) NOT NULL REFERENCES Transition (Id) ON DELETE"
        + " NO ACTION ON UPDATE NO ACTION MATCH SIMPLE, TimeOffsetFrom TIME NOT NULL,"
        + " TimeOffsetTo TIME NOT NULL)");
    (*m_database).exec(std::string("CREATE TABLE Station (Id INTEGER (1, 30) UNIQUE PRIMARY KEY NOT NULL,")
        + " Name STRING (1, 50) NOT NULL UNIQUE, X INTEGER (1, 30) NOT NULL, Y INTEGER (1, 30) NOT NULL,"
        + " Capasity INTEGER (1, 30) NOT NULL)");
    (*m_database).exec(std::string("CREATE TABLE Train (Number INTEGER (1, 30) UNIQUE NOT NULL PRIMARY KEY,")
        + " Route INTEGER (1, 30) NOT NULL REFERENCES Route (Id) ON DELETE NO ACTION ON UPDATE NO ACTION"
        + " MATCH SIMPLE)");
    (*m_database).exec(std::string("CREATE TABLE Transition (Id INTEGER (1, 30) PRIMARY KEY UNIQUE NOT NULL,")
        + " StationFrom INTEGER (1, 30) NOT NULL REFERENCES Station (Id) ON DELETE NO ACTION ON UPDATE"
        + " NO ACTION MATCH SIMPLE, StationTo INTEGER (1, 30) NOT NULL REFERENCES Station (Id) ON DELETE"
        + " NO ACTION ON UPDATE NO ACTION MATCH SIMPLE)");
    }
    catch (SQLite::Exception)
    {
        throw ScorpDBException::OpenFailedException(path);
        return false;
    }
    return true;
}

bool RailwayNetDBShell::addTransition(RailwayNetDBObject::Transition transition)
{
    try
    {
        SQLite::Statement query(*m_database, "INSERT INTO Transition VALUES(:id, :stationFrom, :stationTo)");
        query.bind(":id", transition.id);
        query.bind(":stationFrom", transition.stationFrom);
        query.bind(":stationTo", transition.stationTo);
        query.exec();
    }
    catch (SQLite::Exception)
    {
        //throw ScorpDBException::BadMemoryLocationException(TableName::TRANSITION, std::to_string(transition.id));
        return false;
    }
    return true;
}

bool RailwayNetDBShell::addTrain(RailwayNetDBObject::Train train)
{
    try
    {
         SQLite::Statement query(*m_database, "INSERT INTO Train VALUES(:number, :route)");
         query.bind(":number", train.number);
         query.bind(":route", train.route);
         query.exec();
    }
    catch (SQLite::Exception)
    {
        //throw ScorpDBException::BadMemoryLocationException(TableName::TRAIN, std::to_string(train.number));
        return false;
    }
    return true;
}

bool RailwayNetDBShell::addStation(RailwayNetDBObject::Station station)
{
    try
    {
        SQLite::Statement query(*m_database, "INSERT INTO Station VALUES(:id, :name, :x, :y, :capacity)");
        query.bind(":id", station.id);
        query.bind(":name", station.name);
        query.bind(":x", station.x);
        query.bind(":y", station.y);
        query.bind(":capacity", station.capasity);
        query.exec();
    }
    catch (SQLite::Exception)
    {
        //throw ScorpDBException::BadMemoryLocationException(TableName::STATION, std::to_string(station.id));
        return false;
    }
    return true;
}

bool RailwayNetDBShell::addRoutePart(RailwayNetDBObject::RoutePart route_part)
{
    try
    {
        SQLite::Statement query(*m_database, std::string("INSERT INTO TimeDelay VALUES(:id,")
            + " :route, :transition, :timeOffsetFrom, :timeOffsetTo)");
        query.bind(":id", route_part.id);
        query.bind(":route", route_part.route);
        query.bind(":transition", route_part.transition);
        std::string str_time_from = "01:01:01";
        std::string str_time_to = "01:01:01";
        query.bind(":timeOffsetFrom", str_time_from);
        query.bind(":timeOffsetTo", str_time_to);
        //query.bind(":timeOffsetFrom", route_part.timeOffsetFrom);
        //query.bind(":timeOffsetTo", route_part.timeOffsetTo);
        query.exec();
    }
    catch (SQLite::Exception)
    {
        //throw ScorpDBException::BadMemoryLocationException(TableName::ROUTE_PART, std::to_string(route_part.id));
        return false;
    }
    return true;
}

bool RailwayNetDBShell::addRoute(RailwayNetDBObject::Route route)
{
    try
    {
         SQLite::Statement query(*m_database, "INSERT INTO Route VALUES(:id, :name)");
         query.bind(":id", route.id);
         query.bind(":name", route.name);
         query.exec();
    }
    catch (SQLite::Exception)
    {
        //throw ScorpDBException::BadMemoryLocationException(TableName::ROUTE, std::to_string(route.id));
        return false;
    }
    return true;
}

bool RailwayNetDBShell::changeTransition(int id, int station_from, int station_to)
{
    try
    {
        SQLite::Statement query(*m_database, std::string("UPDATE Transition SET StationFrom = :stationFrom,")
            + " StationTo = :stationTo WHERE Id = :id");
        query.bind(":id", id);
        query.bind(":stationFrom", station_from);
        query.bind(":stationTo", station_to);
        query.exec();
    }
    catch (SQLite::Exception)
    {
        //throw ScorpDBException::ItemNotFoundException(key, table_name);
        //throw ScorpDBException::BadMemoryLocationException(TableName::USER_GROUPS, user_group.name);
        return false;
    }
    return true;
}

bool RailwayNetDBShell::changeTransitionStationFrom(int id, int value)
{
    try
    {
        SQLite::Statement query(*m_database, std::string("UPDATE Transition SET StationFrom = :stationFrom")
            + " WHERE Id = :id");
        query.bind(":id", id);
        query.bind(":stationFrom", value);
        query.exec();
    }
    catch (SQLite::Exception)
    {
        //throw ScorpDBException::ItemNotFoundException(key, table_name);
        //throw ScorpDBException::BadMemoryLocationException(TableName::USER_GROUPS, user_group.name);
        return false;
    }
    return true;
}

bool RailwayNetDBShell::changeTransitionStationTo(int id, int value)
{
    try
    {
        SQLite::Statement query(*m_database, std::string("UPDATE Transition SET StationTo = :stationTo")
            + " WHERE Id = :id");
        query.bind(":id", id);
        query.bind(":stationTo", value);
        query.exec();
    }
    catch (SQLite::Exception)
    {
        //throw ScorpDBException::ItemNotFoundException(key, table_name);
        //throw ScorpDBException::BadMemoryLocationException(TableName::USER_GROUPS, user_group.name);
        return false;
    }
    return true;
}

bool RailwayNetDBShell::changeTrainRoute(int number, int value)
{
    try
    {
        SQLite::Statement query(*m_database, std::string("UPDATE Train SET Route = :route")
            + " WHERE Number = :number");
        query.bind(":number", number);
        query.bind(":route", value);
        query.exec();
    }
    catch (SQLite::Exception)
    {
        //throw ScorpDBException::ItemNotFoundException(key, table_name);
        //throw ScorpDBException::BadMemoryLocationException(TableName::USER_GROUPS, user_group.name);
        return false;
    }
    return true;
}

bool RailwayNetDBShell::changeStation(int id, const std::string& name, int x, int y, int capasity)
{
    try
    {
        SQLite::Statement query(*m_database, std::string("UPDATE Station SET Name = :name,")
            + " X = :x, Y = :y, Capasity = :capasity WHERE Id = :id");
        query.bind(":id", id);
        query.bind(":name", name);
        query.bind(":x", x);
        query.bind(":y", y);
        query.bind(":capasity", capasity);
        query.exec();
    }
    catch (SQLite::Exception)
    {
        //throw ScorpDBException::ItemNotFoundException(key, table_name);
        //throw ScorpDBException::BadMemoryLocationException(TableName::USER_GROUPS, user_group.name);
        return false;
    }
    return true;
}

bool RailwayNetDBShell::changeStationName(int id, const std::string& name)
{
    try
    {
        SQLite::Statement query(*m_database, std::string("UPDATE Station SET Name = :name")
            + " WHERE Id = :id");
        query.bind(":id", id);
        query.bind(":name", name);
        query.exec();
    }
    catch (SQLite::Exception)
    {
        //throw ScorpDBException::ItemNotFoundException(key, table_name);
        //throw ScorpDBException::BadMemoryLocationException(TableName::USER_GROUPS, user_group.name);
        return false;
    }
    return true;
}

bool RailwayNetDBShell::changeStationCoord(int id, int x, int y)
{
    try
    {
        SQLite::Statement query(*m_database, std::string("UPDATE Station SET X = :x,")
            + " Y = :y WHERE Id = :id");
        query.bind(":id", id);
        query.bind(":x", x);
        query.bind(":y", y);
        query.exec();
    }
    catch (SQLite::Exception)
    {
        //throw ScorpDBException::ItemNotFoundException(key, table_name);
        //throw ScorpDBException::BadMemoryLocationException(TableName::USER_GROUPS, user_group.name);
        return false;
    }
    return true;
}

bool RailwayNetDBShell::changeStationCapasity(int id, int value)
{
    try
    {
        SQLite::Statement query(*m_database, std::string("UPDATE Station SET Capasity = :capasity")
            + " WHERE Id = :id");
        query.bind(":id", id);
        query.bind(":capasity", value);
        query.exec();
    }
    catch (SQLite::Exception)
    {
        //throw ScorpDBException::ItemNotFoundException(key, table_name);
        //throw ScorpDBException::BadMemoryLocationException(TableName::USER_GROUPS, user_group.name);
        return false;
    }
    return true;
}

bool RailwayNetDBShell::changeRoutePartTime(int id, const ScorpCore::Time& time_from, const ScorpCore::Time& time_to)
{
    try
    {
        SQLite::Statement query(*m_database, std::string("UPDATE RoutePart SET TimeOffsetFrom = :timeFrom,")
            + " TimeOffsetTo = :timeTo WHERE Id = :id");
        query.bind(":id", id);
        std::string str_time_from = "01:01:01";
        std::string str_time_to = "01:01:01";
        query.bind(":timeFrom", str_time_from);
        query.bind(":timeTo", str_time_to);
        //query.bind(":timeFrom", time_from);
        //query.bind(":timeTo", time_to);
        query.exec();
    }
    catch (SQLite::Exception)
    {
        //throw ScorpDBException::ItemNotFoundException(key, table_name);
        //throw ScorpDBException::BadMemoryLocationException(TableName::USER_GROUPS, user_group.name);
        return false;
    }
    return true;
}

bool RailwayNetDBShell::changeRoutePartTimeFrom(int id, const ScorpCore::Time& time)
{
    try
    {
        SQLite::Statement query(*m_database, std::string("UPDATE RoutePart SET TimeOffsetFrom = :timeFrom")
            + " WHERE Id = :id");
        query.bind(":id", id);
        std::string str_time = "01:01:01";
        query.bind(":timeFrom", str_time);
        //query.bind(":timeFrom", time);
        query.exec();
    }
    catch (SQLite::Exception)
    {
        //throw ScorpDBException::ItemNotFoundException(key, table_name);
        //throw ScorpDBException::BadMemoryLocationException(TableName::USER_GROUPS, user_group.name);
        return false;
    }
    return true;
}

bool RailwayNetDBShell::changeRoutePartTimeTo(int id, const ScorpCore::Time& time)
{
    try
    {
        SQLite::Statement query(*m_database, std::string("UPDATE RoutePart SET TimeOffsetTo = :timeTo")
            + " WHERE Id = :id");
        query.bind(":id", id);
        std::string str_time = "01:01:01";
        query.bind(":timeFrom", str_time);
        //query.bind(":timeTo", time);
        query.exec();
    }
    catch (SQLite::Exception)
    {
        //throw ScorpDBException::ItemNotFoundException(key, table_name);
        //throw ScorpDBException::BadMemoryLocationException(TableName::USER_GROUPS, user_group.name);
        return false;
    }
    return true;
}

bool RailwayNetDBShell::changeRouteName(int id, const std::string& name)
{
    try
    {
        SQLite::Statement query(*m_database, std::string("UPDATE Route SET Name = :name WHERE Id = :id"));
        query.bind(":id", id);
        query.bind(":name", name);
        query.exec();
    }
    catch (SQLite::Exception)
    {
        //throw ScorpDBException::ItemNotFoundException(key, table_name);
        //throw ScorpDBException::BadMemoryLocationException(TableName::USER_GROUPS, user_group.name);
        return false;
    }
    return true;
}

bool RailwayNetDBShell::removeTransition(int id)
{
    try
    {
        SQLite::Statement query(*m_database, "DELETE FROM Transition WHERE Id = :id");
        query.bind(":id", id);
        query.exec();
    }
    catch (SQLite::Exception)
    {
        //throw ScorpDBException::ItemNotFoundException(key, table_name);
        //throw ScorpDBException::BadMemoryLocationException(TableName::USER_GROUPS, user_group.name);
        return false;
    }
    return true;
}

bool RailwayNetDBShell::removeTrain(int number)
{
    try
    {
        SQLite::Statement query(*m_database, "DELETE FROM Train WHERE Number = :number");
        query.bind(":number", number);
        query.exec();
    }
    catch (SQLite::Exception)
    {
        //throw ScorpDBException::ItemNotFoundException(key, table_name);
        //throw ScorpDBException::BadMemoryLocationException(TableName::USER_GROUPS, user_group.name);
        return false;
    }
    return true;
}

bool RailwayNetDBShell::removeStation(int id)
{
    try
    {
        SQLite::Statement query(*m_database, "DELETE FROM Station WHERE Id = :id");
        query.bind(":id", id);
        query.exec();
    }
    catch (SQLite::Exception)
    {
        //throw ScorpDBException::ItemNotFoundException(key, table_name);
        //throw ScorpDBException::BadMemoryLocationException(TableName::USER_GROUPS, user_group.name);
        return false;
    }
    return true;
}

bool RailwayNetDBShell::removeRoutePart(int id)
{
    try
    {
        SQLite::Statement query(*m_database, "DELETE FROM RoutePart WHERE Id = :id");
        query.bind(":id", id);
        query.exec();
    }
    catch (SQLite::Exception)
    {
        //throw ScorpDBException::ItemNotFoundException(key, table_name);
        //throw ScorpDBException::BadMemoryLocationException(TableName::USER_GROUPS, user_group.name);
        return false;
    }
    return true;
}

bool RailwayNetDBShell::removeRoute(int id)
{
    try
    {
        SQLite::Statement query(*m_database, "DELETE FROM Route WHERE Id = :id");
        query.bind(":id", id);
        query.exec();
    }
    catch (SQLite::Exception)
    {
        //throw ScorpDBException::ItemNotFoundException(key, table_name);
        //throw ScorpDBException::BadMemoryLocationException(TableName::USER_GROUPS, user_group.name);
        return false;
    }
    return true;
}

RailwayNetDBObject::Transition RailwayNetDBShell::getTransition(int id)
{
    RailwayNetDBObject::Transition transition;
    try
    {
        SQLite::Statement query(*m_database, "SELECT StationFrom, StationTo FROM Transition WHERE Id = :id");
        query.bind(":id", id);
        while (query.executeStep())
        {
            transition.id = id;
            transition.stationFrom = query.getColumn(0).getInt();
            transition.stationTo = query.getColumn(1).getInt();
            break;
        }
    }
    catch (SQLite::Exception)
    {
        //throw ScorpDBException::BadMemoryLocationException(table_name, key);
    }
    return transition;
}

RailwayNetDBObject::Train RailwayNetDBShell::getTrain(int number)
{
    RailwayNetDBObject::Train train;
    try
    {
        SQLite::Statement query(*m_database, "SELECT Route FROM Train WHERE Number = :number");
        query.bind(":number", number);
        while (query.executeStep())
        {
            train.number = number;
            train.route = query.getColumn(0).getInt();
            break;
        }
    }
    catch (SQLite::Exception)
    {
        //throw ScorpDBException::BadMemoryLocationException(table_name, key);
    }
    return train;
}

RailwayNetDBObject::Station RailwayNetDBShell::getStation(int id)
{
    RailwayNetDBObject::Station station;
    try
    {
        SQLite::Statement query(*m_database, "SELECT Name, X, Y, Capasity FROM Station WHERE Id = :id");
        query.bind(":id", id);
        while (query.executeStep())
        {
            station.id = id;
            station.name = query.getColumn(0).getText();
            station.x = query.getColumn(1).getInt();
            station.y = query.getColumn(2).getInt();
            station.capasity = query.getColumn(3).getInt();
            break;
        }
    }
    catch (SQLite::Exception)
    {
        //throw ScorpDBException::BadMemoryLocationException(table_name, key);
    }
    return station;
}

RailwayNetDBObject::RoutePart RailwayNetDBShell::getRoutePart(int id)
{
    RailwayNetDBObject::RoutePart route_part;
    try
    {
        SQLite::Statement query(*m_database, std::string("SELECT Route, Transition, TimeOffsetFrom,")
            + " TimeOffsetTo FROM RoutePart WHERE Id = :id");
        query.bind(":id", id);
        std::string str_time_from;
        std::string str_time_to;
        while (query.executeStep())
        {
            route_part.id = id;
            route_part.route = query.getColumn(0).getInt();
            route_part.transition = query.getColumn(1).getInt();
            str_time_from = query.getColumn(2).getText();
            str_time_to = query.getColumn(3).getText();
            //route_part.timeOffsetFrom = query.getColumn(2).getText();
            //route_part.timeOffsetTo = query.getColumn(3).getText();
            break;
        }
    }
    catch (SQLite::Exception)
    {
        //throw ScorpDBException::BadMemoryLocationException(table_name, key);
    }
    return route_part;
}

RailwayNetDBObject::Route RailwayNetDBShell::getRoute(int id)
{
    RailwayNetDBObject::Route route;
    try
    {
        SQLite::Statement query(*m_database, "SELECT Name FROM Route WHERE Id = :id");
        query.bind(":id", id);
        while (query.executeStep())
        {
            route.id = id;
            route.name = query.getColumn(0).getText();
            break;
        }
    }
    catch (SQLite::Exception)
    {
        //throw ScorpDBException::BadMemoryLocationException(table_name, key);
    }
    return route;
}

std::vector<RailwayNetDBObject::Transition> RailwayNetDBShell::getTransitions()
{
    std::vector<RailwayNetDBObject::Transition> transition_list;
    try
    {
        SQLite::Statement query(*m_database, "SELECT * FROM Transition");
        while (query.executeStep())
        {
            transition_list.push_back(RailwayNetDBObject::Transition(query.getColumn(0).getInt(),
                query.getColumn(1).getInt(), query.getColumn(2).getInt()));
        }
    }
    catch (SQLite::Exception)
    {
        //throw ScorpDBException::BadMemoryLocationException(table_name, key);
        return std::vector<RailwayNetDBObject::Transition>();
    }
    return transition_list;
}

std::vector<RailwayNetDBObject::Train> RailwayNetDBShell::getTrains()
{
    std::vector<RailwayNetDBObject::Train> train_list;
    try
    {
        SQLite::Statement query(*m_database, "SELECT * FROM Train");
        while (query.executeStep())
        {
            train_list.push_back(RailwayNetDBObject::Train(query.getColumn(0).getInt(),
                query.getColumn(1).getInt()));
        }
    }
    catch (SQLite::Exception)
    {
        //throw ScorpDBException::BadMemoryLocationException(table_name, key);
        return std::vector<RailwayNetDBObject::Train>();
    }
    return train_list;
}

std::vector<RailwayNetDBObject::Station> RailwayNetDBShell::getStations()
{
    std::vector<RailwayNetDBObject::Station> station_list;
    try
    {
        SQLite::Statement query(*m_database, "SELECT * FROM Station");
        while (query.executeStep())
        {
            station_list.push_back(RailwayNetDBObject::Station(query.getColumn(0).getInt(),
                query.getColumn(1).getText(), query.getColumn(2).getInt(), query.getColumn(3).getInt(),
                query.getColumn(4).getInt()));
        }
    }
    catch (SQLite::Exception)
    {
        //throw ScorpDBException::BadMemoryLocationException(table_name, key);
        return std::vector<RailwayNetDBObject::Station>();
    }
    return station_list;
}

std::vector<RailwayNetDBObject::RoutePart> RailwayNetDBShell::getRouteParts()
{
    std::vector<RailwayNetDBObject::RoutePart> route_part_list;
    try
    {
        SQLite::Statement query(*m_database, "SELECT * FROM RoutePart");
        std::string time_from;
        std::string time_to;
        while (query.executeStep())
        {
            time_from = query.getColumn(3).getText();
            time_to = query.getColumn(4).getText();
            //route_part_list.push_back(RailwayNetDBObject::RoutePart(query.getColumn(0).getInt(),
            //    query.getColumn(1).getText(), query.getColumn(2).getInt(), query.getColumn(3).getInt(),
            //    query.getColumn(4).getInt()));
        }
    }
    catch (SQLite::Exception)
    {
        //throw ScorpDBException::BadMemoryLocationException(table_name, key);
        return std::vector<RailwayNetDBObject::RoutePart>();
    }
    return route_part_list;
}

std::vector<RailwayNetDBObject::Route> RailwayNetDBShell::getRoutes()
{
    std::vector<RailwayNetDBObject::Route> route_list;
    try
    {
        SQLite::Statement query(*m_database, "SELECT * FROM Route");
        while (query.executeStep())
        {
            route_list.push_back(RailwayNetDBObject::Route(query.getColumn(0).getInt(),
                query.getColumn(1).getText()));
        }
    }
    catch (SQLite::Exception)
    {
        //throw ScorpDBException::BadMemoryLocationException(table_name, key);
        return std::vector<RailwayNetDBObject::Route>();
    }
    return route_list;
}

std::vector<std::string> RailwayNetDBShell::getRoutsFromAtoB(std::string stA, std::string stB)
{
    std::vector<std::vector<std::string>> routs= getAllRowsFromTable(TableName::ROUTE);
    std::vector<std::vector<std::string>> routParts= getAllRowsFromTable(TableName::ROUTE_PART);
    std::vector<std::vector<std::string>> transitions= getAllRowsFromTable(TableName::TRANSITION);
    std::set<std::string> validTrasitionsA;
    std::set<std::string> validTrasitionsB;
    std::vector<std::string> resultRouts;
    for(std::vector<std::string> tr: transitions)
    {
        if(tr[1]==stA) validTrasitionsA.insert(tr[0]);
        if(tr[1]==stB) validTrasitionsB.insert(tr[0]);

    }
    bool aCheck, bCheck;
   for(std::vector<std::string> rout: routs)
    {
        aCheck=false;
        bCheck=false;
        for(std::vector<std::string> routPart: routParts)
        if((rout[0])==routPart[0])
        {
           if(validTrasitionsA.find(routPart[1])!=validTrasitionsA.end()) aCheck=true;
            if(validTrasitionsB.find(routPart[1])!=validTrasitionsB.end()) bCheck=true;
            if(aCheck&&bCheck)
            {
                resultRouts.push_back(rout[0]);
                aCheck=false;
                bCheck=false;
            }

        }
    }
    return resultRouts;

}

std::vector<std::string> RailwayNetDBShell::getAllStations()
{

        const char* table_name="Station";
        std::vector<std::string> table_rows;
        try
            {
                SQLite::Statement query(*m_database, "SELECT Name FROM Station");
                std::string curr_row;
                while (query.executeStep())
                {
                    curr_row = query.getColumn(0).getText();
                    table_rows.push_back(curr_row);
                }
            }
            catch(SQLite::Exception)
            {
                throw ScorpDBException::BadMemoryLocationException(TableName::STATION, "");
            }

        return table_rows;


}

bool RailwayNetDBShell::deleteAllStations()
{
    try
    {
            SQLite::Statement query(*m_database, "DELETE FROM Station");
            query.exec();

    }
    catch(SQLite::Exception)
    {
        throw ScorpDBException::BadMemoryLocationException(TableName::STATION, "");
        return false;
    }
    return true;
}

bool RailwayNetDBShell::deleteStation(std::string id)
{
    try
    {
        deleteFromTable(TableName::STATION, id);
    }
    catch(SQLite::Exception)
    {
        throw ScorpDBException::BadMemoryLocationException(TableName::STATION, "");
        return false;
    }
    return true;
}

bool RailwayNetDBShell::deleteAllTrains()
{
    try
    {
            SQLite::Statement query(*m_database, "DELETE FROM Train");
            query.exec();

    }
    catch(SQLite::Exception)
    {
        throw ScorpDBException::BadMemoryLocationException(TableName::TRAIN, "");
        return false;
    }
    return true;
}

bool RailwayNetDBShell::deleteTrain(std::string number)
{
    try
    {
        deleteFromTable(TableName::TRAIN, number);
    }
    catch(SQLite::Exception)
    {
        throw ScorpDBException::BadMemoryLocationException(TableName::TRAIN, "");
        return false;
    }
    return true;
}

RailwayNetDBObject::Route RailwayNetDBShell::getRoute(std::string name)
{

    std::vector<std::string> data_row;
    try
    {
        SQLite::Statement query(*m_database, "SELECT * FROM Route WHERE Name LIKE :key");
        query.bind(":key", name);
        int row_size = query.getColumnCount();
        query.executeStep();
        std::string col = "";
        for(int i = 0; i < row_size; ++i)
        {
            col = query.getColumn(i).getText();
            data_row.push_back(col);
        }
    }
    catch(SQLite::Exception)
    {
        throw ScorpDBException::BadMemoryLocationException(TableName::ROUTE, name);
        RailwayNetDBObject::Route route(0,name);
        return route;
    }
    RailwayNetDBObject::Route route(std::stoi(data_row[0]), data_row[1]);
    return route;
}

bool RailwayNetDBShell::deleteRout(std::string name)
{
    RailwayNetDBObject::Route rt=getRoute(name);
    try
    {
        deleteFromTable(TableName::ROUTE, std::to_string(rt.id));
    }
    catch(SQLite::Exception)
    {
        throw ScorpDBException::BadMemoryLocationException(TableName::ROUTE, "");
        return false;
    }
    return true;
}

bool RailwayNetDBShell::deleteAllRouts()
{
    try
    {
            SQLite::Statement query(*m_database, "DELETE FROM Route");
            query.exec();

    }
    catch(SQLite::Exception)
    {
        throw ScorpDBException::BadMemoryLocationException(TableName::ROUTE, "");
        return false;
    }
    return true;
}

