#include "ScorpDBSell.h"

ScorpDBSell::ScorpDBSell(void):db("ScorpDB.db", SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE)
{
		
		
}

ScorpDBSell::ScorpDBSell(std::string DBPath, bool CreateNew):db(DBPath, SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE)
{
	if(CreateNew)
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
void ScorpDBSell::addUserGroups(UserGroup usergroup) 
{
	addRowToGroup(usergroup.getName(), usergroup.getViewMap() ? "true": "false", 
		usergroup.getEditMap() ? "true": "false", usergroup.getAutorization() ? "true":"false", 
		usergroup.getAccountManagment() ? "true":"false", usergroup.getEditStationInfo() ? "true":"false", 
		usergroup.getEditSchedule() ? "true":"false", usergroup.getEditTrainsList() ? "true":"false", 
		usergroup.getFindtrips() ? "true":"false");
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
void ScorpDBSell::addRoutePart(RoutePart routepart)
{
	addRowToRoutePart(std::to_string(routepart.getRoute()),std::to_string(routepart.getTransistion()), routepart.getTimeOffsetFrom(),  routepart.getTimeOffsetTo());
}
void ScorpDBSell::addRoute(Route route)
{
	addRowToRout(std::to_string(route.getId()), route.getName()); 
}

void  ScorpDBSell::changeDatainColume(std::string tableName,std::string Key, std::string ColumeName, std::string Data)
{
	return changeData(tableName, Key, ColumeName, Data);
}
void  ScorpDBSell::deleteFromTable(std::string tableName, std::string Key) 
{
	deleteRowFromTable(tableName, Key) ;
}
std::string*  ScorpDBSell::getRowFromTable(std::string tableName, std::string Key) 
{
	return getDataFromRow(tableName, Key);
}
std::vector<std::vector<std::string>>  ScorpDBSell::getAllRowsFromTable(std::string tableName) 
{
	return  getAllDataFromTable(tableName);
}
bool ScorpDBSell::isUserExist(std::string login) 
{
	return isUserExistinDb(login);
}
bool ScorpDBSell::authenticate(std::string login, std::string password) 
{
	return isValidPass(login, password);
}
std::string ScorpDBSell::getUserGroup(std::string login) 
{
	return getGroupUser(login);
}


ScorpDBSell::~ScorpDBSell(void)
{
}
