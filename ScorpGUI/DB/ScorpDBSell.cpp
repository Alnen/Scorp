#include "ScorpDBSell.h"

ScorpDBSell::ScorpDBSell(void)
    : db("ScorpDB.db", SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE)
{	
}

ScorpDBSell::ScorpDBSell(std::string db_path, bool is_new)
    : db(db_path, SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE)
{
    if(is_new)
	{
		db.exec("CREATE TABLE Station (Id INT (1, 30) PRIMARY KEY UNIQUE NOT NULL, Name CHAR UNIQUE NOT NULL, X INT (1, 30) NOT NULL, Y INT (1, 30) NOT NULL, LineCount INT (1, 30) NOT NULL)" );
		db.exec("CREATE TABLE TimeDelay (Station INT (1, 30) PRIMARY KEY UNIQUE NOT NULL, Train INT (1, 30) UNIQUE NOT NULL, DateFrom DATE (1, 30) NOT NULL, DateTo DATE (1, 30) NOT NULL)");
		db.exec("CREATE TABLE Train (Number INT (1, 30) PRIMARY KEY UNIQUE NOT NULL, \"Voyage\" INT (1, 30), \"Location\" INT (1, 30))");
		db.exec("CREATE TABLE Transition (Transition INT (1, 30) PRIMARY KEY NOT NULL UNIQUE, StationFrom INT (1, 30) NOT NULL, StationTo INT (1, 30) NOT NULL)");
		db.exec("CREATE TABLE Trip (Id INT (1, 30) PRIMARY KEY UNIQUE NOT NULL, Train INT (1, 30) NOT NULL, Transition INT (1, 30) NOT NULL, LineNumber INT (1, 30) NOT NULL, NextTrip INT (1, 30) NOT NULL)");
		db.exec("CREATE TABLE User (Login CHAR (1, 20) PRIMARY KEY NOT NULL, Password CHAR (1, 12), Groups CHAR (1, 12) NOT NULL)");
		db.exec("CREATE TABLE UserGroups (Groups CHAR (1, 12) PRIMARY KEY NOT NULL, ViewMap BOOLEAN DEFAULT false, EditMap BOOLEAN DEFAULT false, Authorization BOOLEAN DEFAULT false, AccountManagement BOOLEAN DEFAULT false, \"EditStationInfo\" BOOLEAN DEFAULT false, EditSchedule BOOLEAN NOT NULL DEFAULT false, EditTrainsList NOT NULL DEFAULT false, Findtrips NOT NULL DEFAULT false)");
		db.exec("CREATE TABLE Voyage (Id INT (1, 30) PRIMARY KEY NOT NULL UNIQUE, StartTrip INT (1, 30) NOT NULL, FinishTrip INT (1, 30) NOT NULL)");
	}
}

void ScorpDBSell::addUser(User user)  
{
	addRowToUser(user.getLogin(), user.getPassword(), user.getUserGroup());
}
void ScorpDBSell::addUserGroups(UserGroup user_group)
{
    addRowToGroup(user_group.getName(), user_group.getViewMap() ? "true" : "false",
        user_group.getEditMap() ? "true" : "false", user_group.getAutorization() ? "true" : "false",
        user_group.getAccountManagment() ? "true":"false",
        user_group.getEditStationInfo() ? "true" : "false",
        user_group.getEditSchedule() ? "true":"false",
        user_group.getEditTrainsList() ? "true" : "false",
        user_group.getFindtrips() ? "true":"false");
}
void ScorpDBSell::addTransition(Transition transition)
{
	addRowToTransition(std::to_string(transition.gettransition()),  std::to_string(transition.getStationFrom()),std::to_string(transition.getStationTo()));
}
void ScorpDBSell::addTrain(Train train)
{
	addRowToTrain(std::to_string(train.getNumber()), std::to_string(train.getRoute()));
}
void ScorpDBSell::addStation(Station station)
{
	addRowToStation(std::to_string(station.getId()), station.geName(), std::to_string(station.getX()),std::to_string(station.getY()), std::to_string(station.getCapasity())); 
}
void ScorpDBSell::addRoutePart(RoutePart route_part)
{
    addRowToRoutePart(std::to_string(route_part.getRoute()), std::to_string(route_part.getTransistion()),
                      route_part.getTimeOffsetFrom(),  route_part.getTimeOffsetTo());
}
void ScorpDBSell::addRoute(Route route)
{
	addRowToRout(std::to_string(route.getId()), route.getName()); 
}

void  ScorpDBSell::changeDataInColume(TableName table_name, std::string key,
                                      std::string colume_name, std::string data)
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
void  ScorpDBSell::deleteFromTable(TableName table_name, std::string key)
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
std::vector<std::string> ScorpDBSell::getRowFromTable(TableName table_name, std::string key)
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
        //getRowFromTable("UserGroups", group_name);
    }
    getRowFromTable(TableName::USER_GROUPS, group_name);
    return rights;
}

UserGroupName ScorpDBSell::getUserGroup(std::string login)
{
    std::string group = getGroupUser(login);
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
