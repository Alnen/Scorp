#include "ScorpDBSell.h"



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
void ScorpDBSell::AddRoute(Route route)
{
	addRowToRout(std::to_string(route.getId()), route.getName()); 
}

void  ScorpDBSell::changeDatainColume(std::string tableName,std::string Key, std::string ColumeName, std::string Data)
{
	return changeData(tableName, Key, ColumeName, Data);
}
void  ScorpDBSell::deleteFromTable(std::string tableName, std::string Key)
{
	deleteRowFromTable(tableName, Key);
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
