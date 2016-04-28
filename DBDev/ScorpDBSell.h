#pragma once
#include "scorpdb.h"
#include "Tables.h"
class ScorpDBSell :
	public ScorpDB
{
public:
	ScorpDBSell(void);
	ScorpDBSell(std::string dbName, bool createNew):ScorpDB(dbName, createNew){};
	void addUser(User user);
	void addUserGroups(UserGroup usergroup);
	void addTransition(Transition transition);
	void addTrain(Train train);
	void addStation(Station station);
	void addRoutePart(RoutePart routepart);
	void AddRoute(Route route);
	void changeDatainColume(std::string tableName,std::string Key, std::string ColumeName, std::string Data);
	void deleteFromTable(std::string tableName, std::string Key);
	std::string* getRowFromTable(std::string tableName, std::string Key);
	std::vector<std::vector<std::string>> getAllRowsFromTable(std::string tableName);
	bool isUserExist(std::string login);
	bool authenticate(std::string login, std::string password);
	std::string getUserGroup(std::string login);
	~ScorpDBSell(void);
};

