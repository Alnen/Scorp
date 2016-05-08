#ifndef Scorp_DB_Sell_h
#define Scorp_DB_Sell_h
#include "Tables.h"
#include<SQLiteCpp/SQLiteCpp.h>
#include<SQLiteCpp/Database.h>
#include<SQLiteCpp/Statement.h>
#include <sqlite3.h>
#include <vector>
#include "ScorpDBInterface.h"
class ScorpDBSell: public ScorpDBInterface	
{

private:
	SQLite::Database    db;
	std::vector<std::vector<std::string>> getinfoAboutTable(std::string tableName);
	void addRowToUser(std::string Login, std::string Password , std::string Group) ;
	void addRowToGroup(std::string Name, std::string ViewMap , std::string EditMap, std::string Authorization, std::string AccountManagement, std::string EditStationInfo, std::string EditSchedule, std::string EditTrainsList, std::string Findtrips );
	void addRowToStation(std::string Id, std::string Name, std::string X, std::string Y, std::string Capacity );
	void addRowToRoutePart(std::string Route, std::string Transition, std::string TimeOffsetFrom, std::string TimeOffsetTo);
	void addRowToTransition(std::string Transition, std::string StationFrom, std::string StationTo);
	void addRowToTrain(std::string Number, std::string Route);
	void addRowToRout(std::string Id, std::string Name);
	void changeData(std::string tableName,std::string Key, std::string ColumeName, std::string Data);
	void deleteRowFromTable(std::string tableName, std::string Key);
	std::string* getDataFromRow(std::string tableName, std::string Key) ;
	std::vector<std::vector<std::string>> getAllDataFromTable(std::string tableName) ;
	bool isUserExistinDb(std::string login) ;
	bool isValidPass(std::string login, std::string pass) ;
	std::string getGroupUser(std::string login) ;

public:
	ScorpDBSell(void);
	ScorpDBSell(std::string dbName, bool createNew);
	void addUser(User user)  override ; 
	void addUserGroups(UserGroup usergroup)  override ;
	void addTransition(Transition transition)  override;
	void addTrain(Train train)  override;
	void addStation(Station station)  override;
	void addRoutePart(RoutePart routepart)  override;
	void addRoute(Route route)  override;
	void changeDatainColume(std::string tableName,std::string Key, std::string ColumeName, std::string Data)  override;
	void deleteFromTable(std::string tableName, std::string Key)  override;
	std::string* getRowFromTable(std::string tableName, std::string Key)  override;
	std::vector<std::vector<std::string>> getAllRowsFromTable(std::string tableName)  override;
	bool isUserExist(std::string login)   override;
	bool authenticate(std::string login, std::string password)  override;
	std::string getUserGroup(std::string login)  override;
	~ScorpDBSell(void);
};

#endif