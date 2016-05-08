#ifndef Scorp_DB_Interface_h
#define Scorp_DB_Interface_h
#include"Tables.h"
#include <vector>
class ScorpDBInterface
{
public:
	virtual void addUser(User user) =0;
	virtual void addUserGroups(UserGroup usergroup) =0;
	virtual void addTransition(Transition transition) =0;
	virtual void addTrain(Train train) =0;
	virtual void addStation(Station station) =0 ;
	virtual void addRoutePart(RoutePart routepart) =0;
	virtual void addRoute(Route route) =0;
	virtual void changeDatainColume(std::string tableName,std::string Key, std::string ColumeName, std::string Data) =0;
	virtual void deleteFromTable(std::string tableName, std::string Key)  =0;
	virtual std::string* getRowFromTable(std::string tableName, std::string Key) =0 ;
	virtual std::vector<std::vector<std::string>> getAllRowsFromTable(std::string tableName) =0;
	virtual bool isUserExist(std::string login) =0;
	virtual bool authenticate(std::string login, std::string password) =0;
	virtual std::string getUserGroup(std::string login)=0 ;
	
};
#endif