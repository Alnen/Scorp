#include "ScorpDBSell.h"
#include "ScorpExceptions.h"

ScorpDBSell::ScorpDBSell(void)
    : db("ScorpDB.db", SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE)
{	
}

ScorpDBSell::ScorpDBSell(std::string db_path, bool is_new)
    : db(db_path, SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE)
{
    if(is_new)
	{
        db.exec(std::string("CREATE TABLE Route (Id INTEGER (1, 30) NOT NULL PRIMARY KEY UNIQUE, ")
                +"Name CHAR (1, 30) NOT NULL)");
        db.exec(std::string("CREATE TABLE RoutePart (Route INTEGER (1, 30) REFERENCES Route (Id) ")
                + "ON DELETE CASCADE ON UPDATE CASCADE MATCH FULL, Transition INTEGER (1, 30) "
                + "NOT NULL REFERENCES Transition (Transition) ON DELETE CASCADE ON UPDATE CASCADE "
                + "MATCH FULL, TimeOffsetFrom DATE (1, 30) NOT NULL, TimeOffsetTo DATE (1, 30) "
                + "NOT NULL, PRIMARY KEY (Route, Transition))");
        db.exec(std::string("CREATE TABLE Station (Id INTEGER (1, 30) PRIMARY KEY UNIQUE NOT NULL, ")
                + "Name CHAR UNIQUE NOT NULL, X INTEGER (1, 30) NOT NULL, Y INTEGER (1, 30) "
                + "NOT NULL, Capasity INTEGER (1, 30) NOT NULL)");
        db.exec(std::string("CREATE TABLE Train (Number INTEGER (1, 30) PRIMARY KEY UNIQUE NOT NULL, ")
                + "\"Route\" INTEGER (1, 30) NOT NULL REFERENCES Route (Id) ON DELETE CASCADE "
                + "ON UPDATE CASCADE MATCH FULL)");
        db.exec(std::string("CREATE TABLE Transition (Transition INTEGER (1, 30) PRIMARY KEY NOT NULL ")
                + "UNIQUE, StationFrom INTEGER (1, 30) NOT NULL REFERENCES Station (Id) "
                + "ON DELETE CASCADE ON UPDATE CASCADE MATCH FULL, StationTo INTEGER (1, 30) "
                + "NOT NULL REFERENCES Station (Id) ON DELETE CASCADE ON UPDATE CASCADE MATCH FULL)");
        db.exec(std::string("CREATE TABLE User (Login CHAR (1, 20) PRIMARY KEY NOT NULL UNIQUE, ")
                + "Password CHAR (1, 12), UserGroup STRING (0, 0) NOT NULL REFERENCES "
                + "UserGroups (Name) ON DELETE CASCADE ON UPDATE CASCADE MATCH FULL)");
        db.exec(std::string("CREATE TABLE UserGroups (Name STRING (0, 0) PRIMARY KEY NOT NULL UNIQUE, ")
                + "ViewMap BOOLEAN DEFAULT false, EditMap BOOLEAN DEFAULT false, "
                + "Authorization BOOLEAN DEFAULT false, AccountManagement BOOLEAN DEFAULT false, "
                + "\"EditStationInfo\" BOOLEAN DEFAULT false, EditSchedule BOOLEAN NOT NULL "
                + "DEFAULT false, EditTrainsList BOOLEAN NOT NULL DEFAULT false, Findtrips BOOLEAN "
                + "NOT NULL DEFAULT false)");
	}
}

void ScorpDBSell::addUser(User user)  
{
    try
    {
        addRowToUser(user.getLogin(), user.getPassword(), user.getUserGroup());
    }
    catch(SCORPDBAtMemoryLocationException e)
    {
        throw e;
    }
}
void ScorpDBSell::addUserGroups(UserGroup user_group)
{
    try
    {
        addRowToGroup(user_group.getName(), user_group.getViewMap() ? "true" : "false",
            user_group.getEditMap() ? "true" : "false", user_group.getAutorization() ? "true" : "false",
            user_group.getAccountManagment() ? "true":"false",
            user_group.getEditStationInfo() ? "true" : "false",
            user_group.getEditSchedule() ? "true":"false",
            user_group.getEditTrainsList() ? "true" : "false",
            user_group.getFindtrips() ? "true":"false");
    }
    catch(SCORPDBAtMemoryLocationException e)
    {
        throw e;
    }
}
void ScorpDBSell::addTransition(Transition transition)
{
    try
    {
        addRowToTransition(std::to_string(transition.getTransition()),  std::to_string(transition.getStationFrom()),std::to_string(transition.getStationTo()));
    }
    catch(SCORPDBAtMemoryLocationException e)
    {
        throw e;
    }
}
void ScorpDBSell::addTrain(Train train)
{
    try
    {
        addRowToTrain(std::to_string(train.getNumber()), std::to_string(train.getRoute()));
    }
    catch(SCORPDBAtMemoryLocationException e)
    {
        throw e;
    }
}
void ScorpDBSell::addStation(Station station)
{
    try
    {
        addRowToStation(std::to_string(station.getId()), station.geName(), std::to_string(station.getX()),std::to_string(station.getY()), std::to_string(station.getCapasity()));
    }
    catch(SCORPDBAtMemoryLocationException e)
    {
        throw e;
    }
}
void ScorpDBSell::addRoutePart(RoutePart route_part)
{
    try
    {
        addRowToRoutePart(std::to_string(route_part.getRoute()), std::to_string(route_part.getTransistion()),
                         route_part.getTimeOffsetFrom(),  route_part.getTimeOffsetTo());
    }
    catch(SCORPDBAtMemoryLocationException e)
    {
        throw e;
    }
}
void ScorpDBSell::addRoute(Route route)
{
    try
    {
        addRowToRoute(std::to_string(route.getId()), route.getName());
    }
    catch(SCORPDBAtMemoryLocationException e)
    {
        throw e;
    }
}

void  ScorpDBSell::changeDataInColume(TableName table_name, std::string key,
                                      std::string colume_name, std::string data)
{
    try
    {
        if (table_name == TableName::USER)
        {
            changeData("User", "Login", key, colume_name, data);
        }
        else if (table_name == TableName::USER_GROUPS)
        {
            changeData("UserGroups", "Name", key, colume_name, data);
        }
        else if (table_name == TableName::ROUTE_PART)
        {
            changeData("RoutePart", "Route", key, colume_name, data);
        }
        else if (table_name == TableName::TRANSITION)
        {
            changeData("Transition", "Transition", key, colume_name, data);
        }
        else if (table_name == TableName::TRAIN)
        {
            changeData("Train", "Number", key, colume_name, data);
        }
        else if (table_name == TableName::ROUTE)
        {
            changeData("Route", "Id", key, colume_name, data);
        }
        else if (table_name == TableName::STATION)
        {
            changeData("Station", "Id", key, colume_name, data);
        }
    }
    catch(SCORPDBAtMemoryLocationException e)
    {
        throw e;
    }
}
void  ScorpDBSell::deleteFromTable(TableName table_name, std::string key)
{
    try
    {
        if (table_name == TableName::USER)
        {
            deleteRowFromTable("User", "Login", key);
        }
        else if (table_name == TableName::USER_GROUPS)
        {
            deleteRowFromTable("UserGroups", "Name", key);
        }
        else if (table_name == TableName::ROUTE_PART)
        {
            deleteRowFromTable("RoutePart", "Route", key);
        }
        else if (table_name == TableName::TRANSITION)
        {
            deleteRowFromTable("Transition", "Transition", key);
        }
        else if (table_name == TableName::TRAIN)
        {
            deleteRowFromTable("Train", "Number", key);
        }
        else if (table_name == TableName::ROUTE)
        {
            deleteRowFromTable("Route", "Id", key);
        }
        else if (table_name == TableName::STATION)
        {
            deleteRowFromTable("Station", "Id", key);
        }
    }
    catch(SCORPDBAtMemoryLocationException e)
    {
        throw e;
    }
}
std::vector<std::string> ScorpDBSell::getRowFromTable(TableName table_name, std::string key)
{
    try
    {
        if (table_name == TableName::USER)
        {
            return getDataFromRow("User", "Login", key);
        }
        else if (table_name == TableName::USER_GROUPS)
        {
            return getDataFromRow("UserGroups", "Name", key);
        }
        else if (table_name == TableName::ROUTE_PART)
        {
            return getDataFromRow("RoutePart", "Route", key);
        }
        else if (table_name == TableName::TRANSITION)
        {
            return getDataFromRow("Transition", "Transition", key);
        }
        else if (table_name == TableName::TRAIN)
        {
            return getDataFromRow("Train", "Number", key);
        }
        else if (table_name == TableName::ROUTE)
        {
            return getDataFromRow("Route", "Id", key);
        }
        else if (table_name == TableName::STATION)
        {
            return getDataFromRow("Station", "Id", key);
        }
    }
    catch(SCORPDBAtMemoryLocationException e)
    {
        throw e;
    }
    return std::vector<std::string>();
}
std::vector<std::vector<std::string>>  ScorpDBSell::getAllRowsFromTable(TableName table_name)
{
    return  getAllDataFromTable(tableNameToString(table_name));
}
bool ScorpDBSell::isUserExist(std::string login) 
{
    return isUserExistInDb(login);
}
bool ScorpDBSell::authenticate(std::string login, std::string password) 
{
    return isValidPassword(login, password);
}

std::array<bool, 8> ScorpDBSell::getUserRights(UserGroupName group)
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
    std::vector<std::string> str_rights = getRowFromTable(TableName::USER_GROUPS, group_name);
    for (size_t i = 0; i < 8; ++i)
    {
        if (str_rights[i+1].compare("true") == 0)
        {
            rights[i] = true;
        }
        else
        {
            rights[i] = false;
        }
    }
    return rights;
}

UserGroupName ScorpDBSell::getUserGroup(std::string login)
{
    std::string group;
    try
    {
        group = getGroupUser(login);
    }
    catch(SCORPDBAtMemoryLocationException e)
    {
        throw e;
        return UserGroupName::USER;
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

ScorpDBSell::~ScorpDBSell(void)
{
}
