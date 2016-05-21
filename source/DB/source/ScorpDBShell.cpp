#include "Scorp/DB/ScorpDBShell.h"
#include "Scorp/exceptions/ScorpDBException.h"
#include <set>

ScorpDBShell::ScorpDBShell(void)
    : m_database(nullptr)
{
}

bool ScorpDBShell::createDatabase(const std::string& path)
{
    m_databasePath = path;
    try
    {
    m_database = std::move(std::unique_ptr<SQLite::Database>(new SQLite::Database(path, SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE)));
    (*m_database).exec(std::string("CREATE TABLE Route (Id INTEGER (1, 30) NOT NULL")
                       +" PRIMARY KEY UNIQUE, Name STRING (1, 50) NOT NULL)");
    (*m_database).exec(std::string("CREATE TABLE RoutePart (Route INTEGER (1, 30) REFERENCES")
                       + " Route (Id) ON DELETE CASCADE ON UPDATE CASCADE MATCH [FULL]"
                       + " NOT NULL, Transition INTEGER (1, 30) NOT NULL REFERENCES Transition"
                       + " (Id) ON DELETE NO ACTION ON UPDATE NO ACTION MATCH SIMPLE, "
                       + "TimeOffsetFrom DATE (1, 30) NOT NULL, TimeOffsetTo DATE (1, 30) "
                       + "NOT NULL, PRIMARY KEY (Route, Transition))");
    (*m_database).exec(std::string("CREATE TABLE Station (Id INTEGER (1, 30) PRIMARY KEY")
                       + " UNIQUE NOT NULL, Name STRING (1, 50) UNIQUE NOT NULL, X INTEGER"
                       + " (1, 30) NOT NULL, Y INTEGER (1, 30) NOT NULL, Capasity INTEGER "
                       + "(1, 30) NOT NULL)");
    (*m_database).exec(std::string("CREATE TABLE Train (Number INTEGER (1, 30) PRIMARY KEY ")
                       + "UNIQUE NOT NULL, [Route] INTEGER (1, 30) NOT NULL REFERENCES Route"
                       + " (Id) ON DELETE CASCADE ON UPDATE CASCADE MATCH [FULL])");
    (*m_database).exec(std::string("CREATE TABLE Transition (Id INTEGER (1, 30) PRIMARY KEY ")
                       + "NOT NULL UNIQUE, StationFrom INTEGER (1, 30) NOT NULL REFERENCES "
                       + "Station (Id) ON DELETE CASCADE ON UPDATE CASCADE MATCH [FULL], "
                       + "StationTo INTEGER (1, 30) NOT NULL REFERENCES Station (Id) ON DELETE "
                       + "CASCADE ON UPDATE CASCADE MATCH [FULL])");
    (*m_database).exec(std::string("CREATE TABLE User (Login STRING (1, 20) PRIMARY KEY NOT ")
                       + "NULL UNIQUE, Password STRING (1, 12), [Group]  STRING (1, 15) NOT "
                       + "NULL REFERENCES UserGroup (Name) ON DELETE CASCADE ON UPDATE CASCADE "
                       + "MATCH SIMPLE DEFAULT User)");
    (*m_database).exec(std::string("CREATE TABLE UserGroup (Name STRING (1, 15) PRIMARY KEY NOT ")
                       + "NULL UNIQUE, ViewMap BOOLEAN DEFAULT (0) NOT NULL, EditMap BOOLEAN "
                       + "DEFAULT (0) NOT NULL, Authorization BOOLEAN DEFAULT (0) NOT NULL, "
                       + "AccountManagement BOOLEAN DEFAULT (0) NOT NULL, [EditStationInfo] "
                       + "BOOLEAN DEFAULT (0) NOT NULL, EditSchedule BOOLEAN NOT NULL DEFAULT"
                       + " (0), EditTrainsList BOOLEAN NOT NULL DEFAULT (0), FindTrips "
                       + "BOOLEAN NOT NULL DEFAULT (0))");
    }
    catch (SQLite::Exception)
    {
        throw ScorpDBException::OpenFailedException(path);
        return false;
    }
    return true;
}

bool ScorpDBShell::connectToDatabase(const std::string& path)
{
    m_databasePath = path;
    try
    {
        m_database = std::move(std::unique_ptr<SQLite::Database>(new SQLite::Database(path, SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE)));
    }
    catch (SQLite::Exception)
    {
        throw ScorpDBException::OpenFailedException(path);
        return false;
    }
    return true;
}

std::string ScorpDBShell::getDatabasePath() const
{
    return m_databasePath;
}

bool ScorpDBShell::addUser(ScorpDBObject::User user)
{
    try
    {
        SQLite::Statement query(*m_database, "INSERT INTO User VALUES(:login, :password, :group)");
        query.bind(":login", user.login);
        query.bind(":password", user.password);
        query.bind(":group", user.group);
        query.exec();
    }
    catch(SQLite::Exception)
    {
        throw ScorpDBException::BadMemoryLocationException(TableName::USER, user.login);
        return false;
    }
    return true;
}

bool ScorpDBShell::addUserGroups(ScorpDBObject::UserGroup user_group)
{
    try
    {
        SQLite::Statement query(*m_database, std::string("INSERT INTO UserGroup VALUES(:name, :viewMap, :editMap,")
                                + " :authorization, :accountManagement, :editStationInfo, :editSchedule,"
                                + " :editTrainsList, :findRoute)");
        query.bind(":name", user_group.name);
        query.bind(":viewMap", user_group.viewMap);
        query.bind(":editMap", user_group.editMap);
        query.bind(":authorization", user_group.autorization);
        query.bind(":accountManagement", user_group.accountManagment);
        query.bind(":editStationInfo", user_group.editStationInfo);
        query.bind(":editSchedule", user_group.editSchedule);
        query.bind(":editTrainsList", user_group.editTrainsList);
        query.bind(":findRoute", user_group.findTrips);
        query.exec();
    }
    catch(SQLite::Exception)
    {
        throw ScorpDBException::BadMemoryLocationException(TableName::USER_GROUPS, user_group.name);
        return false;
    }
    return true;
}

bool ScorpDBShell::addTransition(ScorpDBObject::Transition transition)
{
    try
    {
        SQLite::Statement query(*m_database, "INSERT INTO Transition VALUES(:id, :stationFrom, :stationTo)");
        query.bind(":id", transition.id);
        query.bind(":stationFrom", transition.stationFrom);
        query.bind(":stationTo", transition.stationTo);
        query.exec();
    }
    catch(SQLite::Exception)
    {
        throw ScorpDBException::BadMemoryLocationException(TableName::TRANSITION, std::to_string(transition.id));
        return false;
    }
    return true;
}

bool ScorpDBShell::addTrain(ScorpDBObject::Train train)
{
    try
    {
         SQLite::Statement query(*m_database, "INSERT INTO Train VALUES(:number, :route)");
         query.bind(":number", train.number);
         query.bind(":route", train.route);
         query.exec();
    }
    catch(SQLite::Exception)
    {
        throw ScorpDBException::BadMemoryLocationException(TableName::TRAIN, std::to_string(train.number));
        return false;
    }
    return true;
}

bool ScorpDBShell::addStation(ScorpDBObject::Station station)
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
    catch(SQLite::Exception)
    {
        throw ScorpDBException::BadMemoryLocationException(TableName::STATION, std::to_string(station.id));
        return false;
    }
    return true;
}

bool ScorpDBShell::addRoutePart(ScorpDBObject::RoutePart route_part)
{
    try
    {
        SQLite::Statement query(*m_database, "INSERT INTO TimeDelay VALUES(:route, :transition, :timeOffsetFrom, :timeOffsetTo)");
        query.bind(":route", route_part.route);
        query.bind(":transition", route_part.transistion);
        query.bind(":timeOffsetFrom", route_part.timeOffsetFrom);
        query.bind(":timeOffsetTo", route_part.timeOffsetTo);
        query.exec();
    }
    catch(SQLite::Exception)
    {
        throw ScorpDBException::BadMemoryLocationException(TableName::ROUTE_PART, std::to_string(route_part.route)
                                                           + " " + std::to_string(route_part.transistion));
        return false;
    }
    return true;
}

bool ScorpDBShell::addRoute(ScorpDBObject::Route route)
{
    try
    {
         SQLite::Statement query(*m_database, "INSERT INTO Route VALUES(:id, :name)");
         query.bind(":id", route.id);
         query.bind(":name", route.name);
         query.exec();
    }
    catch(SQLite::Exception)
    {
        throw ScorpDBException::BadMemoryLocationException(TableName::ROUTE, std::to_string(route.id));
        return false;
    }
    return true;
}

bool ScorpDBShell::changeDataInColume(TableName table_name, const std::string& key, const std::string& colume_name, const std::string& data)
{
    std::string str_table_name;
    std::string key_column;
    if (table_name == TableName::USER)
    {
        str_table_name = "User";
        key_column = "Login";
    }
    else if (table_name == TableName::USER_GROUPS)
    {
        str_table_name = "UserGroup";
        key_column = "Name";
    }
    else if (table_name == TableName::ROUTE_PART)
    {
        str_table_name = "RoutePart";
        key_column = "Route";
    }
    else if (table_name == TableName::TRANSITION)
    {
        str_table_name = "Transition";
        key_column = "Id";
    }
    else if (table_name == TableName::TRAIN)
    {
        str_table_name = "Train";
        key_column = "Number";
    }
    else if (table_name == TableName::ROUTE)
    {
        str_table_name = "Route";
        key_column = "Id";
    }
    else if (table_name == TableName::STATION)
    {
        str_table_name = "Station";
        key_column = "Id";
    }
    std::string colume;
    std::vector<std::vector<std::string>> table_rows;
    try
    {
        SQLite::Statement query(*m_database, "pragma table_info("+str_table_name+");");
        int row_size = query.getColumnCount();
        std::vector<std::string> query_colums;
        std::string curr_column;
        while (query.executeStep())
        {
            for (int i = 0; i < row_size; ++i)
            {
                curr_column = query.getColumn(i).getText();
                query_colums.push_back(curr_column);
            }
            table_rows.push_back(query_colums);
            query_colums.clear();
        }
    }
    catch(SQLite::Exception)
    {
        throw ScorpDBException::BadMemoryLocationException(table_name, key);
        return false;
    }
    if (!table_rows.empty())
    {
        for(std::vector<std::string> column_info : table_rows)
        {
            if (column_info[1].compare(colume_name) == 0)
            {
                colume = colume_name;
            }
        }
        if ((!key_column.empty()) && (!str_table_name.empty()) && (!key.empty())
            && (!colume_name.empty()) && (!colume_name.empty()))
        {
            try
            {
                 SQLite::Statement query(*m_database, "UPDATE "+str_table_name+" SET "+colume+" = :Data WHERE "+key_column+"=:Key");
                 query.bind(":data", data);
                 query.bind(":key", key);
                 query.exec();
            }
            catch(SQLite::Exception)
            {
                throw ScorpDBException::BadMemoryLocationException(table_name, key);
                return false;
            }
        }
    }
    else
    {
        throw ScorpDBException::ItemNotFoundException(key, table_name);
        return false;
    }
    return true;
}

bool ScorpDBShell::deleteFromTable(TableName table_name, const std::string& key)
{
    std::string str_table_name;
    std::string key_column;
    if (table_name == TableName::USER)
    {
        str_table_name = "User";
        key_column = "Login";
    }
    else if (table_name == TableName::USER_GROUPS)
    {
        str_table_name = "UserGroup";
        key_column = "Name";
    }
    else if (table_name == TableName::ROUTE_PART)
    {
        str_table_name = "RoutePart";
        key_column = "Route";
    }
    else if (table_name == TableName::TRANSITION)
    {
        str_table_name = "Transition";
        key_column = "Id";
    }
    else if (table_name == TableName::TRAIN)
    {
        str_table_name = "Train";
        key_column = "Number";
    }
    else if (table_name == TableName::ROUTE)
    {
        str_table_name = "Route";
        key_column = "Id";
    }
    else if (table_name == TableName::STATION)
    {
        str_table_name = "Station";
        key_column = "Id";
    }
    try
    {
        if ((!key_column.empty()) && (!str_table_name.empty()))
        {
            SQLite::Statement query(*m_database, "DELETE FROM "+str_table_name+" WHERE "+key_column+"=:key");
            query.bind(":key", key);
            query.exec();
        }
    }
    catch(SQLite::Exception)
    {
        throw ScorpDBException::BadMemoryLocationException(table_name, key);
        return false;
    }
    return true;
}

std::vector<std::string> ScorpDBShell::getRowFromTable(TableName table_name, const std::string& key)
{
    std::string str_table_name;
    std::string key_column;
    if (table_name == TableName::USER)
    {
        str_table_name = "User";
        key_column = "Login";
    }
    else if (table_name == TableName::USER_GROUPS)
    {
        str_table_name = "UserGroup";
        key_column = "Name";
    }
    else if (table_name == TableName::ROUTE_PART)
    {
        str_table_name = "RoutePart";
        key_column = "Route";
    }
    else if (table_name == TableName::TRANSITION)
    {
        str_table_name = "Transition";
        key_column = "Id";
    }
    else if (table_name == TableName::TRAIN)
    {
        str_table_name = "Train";
        key_column = "Number";
    }
    else if (table_name == TableName::ROUTE)
    {
        str_table_name = "Route";
        key_column = "Id";
    }
    else if (table_name == TableName::STATION)
    {
        str_table_name = "Station";
        key_column = "Id";
    }
    std::vector<std::string> data_row;
    if ((!str_table_name.empty()) && (!key_column.empty()) && (!key.empty()))
    {
        try
        {
            SQLite::Statement query(*m_database, "SELECT * FROM "+str_table_name+" WHERE "+key_column+" LIKE :key");
            query.bind(":key", key);
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
            throw ScorpDBException::BadMemoryLocationException(table_name, key);
            return std::vector<std::string>();
        }
    }
    return data_row;
}

std::vector<std::vector<std::string>>  ScorpDBShell::getAllRowsFromTable(TableName table_name)
{
    std::string str_table_name;
    if (table_name == TableName::USER)
    {
        str_table_name = "User";
    }
    else if (table_name == TableName::USER_GROUPS)
    {
        str_table_name = "UserGroup";
    }
    else if (table_name == TableName::ROUTE_PART)
    {
        str_table_name = "RoutePart";
    }
    else if (table_name == TableName::TRANSITION)
    {
        str_table_name = "Transition";
    }
    else if (table_name == TableName::TRAIN)
    {
        str_table_name = "Train";
    }
    else if (table_name == TableName::ROUTE)
    {
        str_table_name = "Route";
    }
    else if (table_name == TableName::STATION)
    {
        str_table_name = "Station";
    }
    std::vector<std::vector<std::string>> table_rows;
    if (!str_table_name.empty())
    {
        try
        {
            SQLite::Statement query(*m_database, "SELECT * FROM "+str_table_name);
            int row_size = query.getColumnCount();
            std::vector<std::string> query_colums;
            std::string curr_column;
            while (query.executeStep())
            {
                for (int i = 0; i < row_size; ++i)
                {
                    curr_column = query.getColumn(i).getText();
                    query_colums.push_back(curr_column);
                }
                table_rows.push_back(query_colums);
                query_colums.clear();
            }
        }
        catch(SQLite::Exception)
        {
            throw ScorpDBException::BadMemoryLocationException(table_name, "");
        }
    }
    return table_rows;
}

bool ScorpDBShell::isUserExist(const std::string& login)
{
    std::vector<std::vector<std::string>> table_rows;
    SQLite::Statement query(*m_database, "SELECT * FROM User WHERE Login=:login");
    query.bind(":login", login);
    int row_size = query.getColumnCount();
    std::vector<std::string> query_colums;
    std::string curr_column;
    while (query.executeStep())
    {
        for(int i = 0; i < row_size; ++i)
        {
            curr_column = query.getColumn(i).getText();
            query_colums.push_back(curr_column);
        }
        table_rows.push_back(query_colums);
        query_colums.clear();
    }
    if (table_rows.size() == 0)
        return false;
    else
        return true;
}

bool ScorpDBShell::authenticate(const std::string& login, const std::string& password)
{
    std::vector<std::vector<std::string>> table_rows;
    SQLite::Statement query(*m_database, "SELECT * FROM User WHERE Login=:login AND Password=:password");
    query.bind(":login", login);
    query.bind(":password", password);
    int row_size = query.getColumnCount();
    std::vector<std::string> query_colums;
    std::string curr_column;
    while (query.executeStep())
    {
        for(int i = 0; i < row_size; ++i)
        {
            curr_column = query.getColumn(i).getText();
            query_colums.push_back(curr_column);
        }
        table_rows.push_back(query_colums);
        query_colums.clear();
    }
    if (table_rows.size() == 0)
        return false;
    else
        return true;
}

std::array<bool, 8> ScorpDBShell::getUserRights(UserGroupName group)
{
    std::array<bool, 8> rights;
    std::string group_name = "";
    if (group == UserGroupName::USER)
    {
        group_name = "User";
    }
    else if (group == UserGroupName::ADMIN)
    {
        group_name = "Admin";
    }
    else if (group == UserGroupName::OPERATOR)
    {
        group_name = "Operator";
    }
    std::vector<std::string> str_rights;
    try
    {
        str_rights = getRowFromTable(TableName::USER_GROUPS, group_name);
    }
    catch(ScorpDBException::BadMemoryLocationException)
    {
        throw ScorpDBException::ItemNotFoundException(group_name, TableName::USER_GROUPS);
        for (size_t i = 0; i < 8; ++i)
        {
            rights[i] = false;
        }
        return rights;
    }
    for (size_t i = 0; i < 8; ++i)
    {
        rights[i] = (str_rights[i+1].compare("1") == 0);
    }
    return rights;
}

UserGroupName ScorpDBShell::getUserGroup(const std::string& login)
{
    std::string group;
    try
    {
        std::vector<std::string> query_colums;
        SQLite::Statement query(*m_database, "SELECT * FROM User where Login=:login");
        query.bind(":login", login);
        int row_size = query.getColumnCount();
        std::string curr_column;
        query.executeStep();
        for(int i = 0; i < row_size; ++i)
        {
            curr_column = query.getColumn(i).getText();
            query_colums.push_back(curr_column);
        }
        group = query_colums[2];
    }
    catch(SQLite::Exception)
    {
        throw ScorpDBException::ItemNotFoundException(login, TableName::USER);
        group = "";
    }
    if (group.compare("Admin") == 0)
    {
        return UserGroupName::ADMIN;
    }
    else if (group.compare("Operator") == 0)
    {
        return UserGroupName::OPERATOR;
    }
    else
    {
        return UserGroupName::USER;
    }
}

ScorpDBShell::~ScorpDBShell(void)
{
}
