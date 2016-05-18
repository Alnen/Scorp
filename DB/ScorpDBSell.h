#ifndef SCORP_DB_SELL_H
#define SCORP_DB_SELL_H

#include "Tables.h"
#include "ScorpDBInterface.h"
#include "DB/sqlite/include/SQLiteCpp.h"
#include "DB/sqlite/include/Database.h"
#include "DB/sqlite/include/Statement.h"
#include "DB/sqlite/sqlite3.h"
#include <vector>

class ScorpDBSell: public ScorpDBInterface	
{
private:
    SQLite::Database db;
    std::vector<std::vector<std::string>> getInfoAboutTable(std::string table_name);
    void addRowToUser(std::string login, std::string password, std::string group) ;
    void addRowToGroup(std::string name, std::string view_map, std::string edit_map,
                       std::string authorization, std::string account_management,
                       std::string edit_station_info, std::string edit_schedule,
                       std::string edit_trains_list, std::string find_trips);
    void addRowToStation(std::string id, std::string name, std::string x,
                         std::string y, std::string capacity);
    void addRowToRoutePart(std::string route, std::string transition,
                           std::string time_offset_from, std::string time_offset_to);
    void addRowToTransition(std::string transition, std::string station_from, std::string station_to);
    void addRowToTrain(std::string number, std::string route);
    void addRowToRoute(std::string id, std::string name);
    void changeData(std::string table_name, std::string key_column, std::string key,
                    std::string colume_name, std::string data);
    void deleteRowFromTable(std::string table_name, std::string key_column, std::string key);
    std::vector<std::string> getDataFromRow(std::string table_name, std::string key_column, std::string key);
    std::vector<std::vector<std::string>> getAllDataFromTable(std::string table_name);
    bool isUserExistInDb(std::string login);
    bool isValidPassword(std::string login, std::string password);
    std::string getGroupUser(std::string login);
    std::string tableNameToString(TableName table_name);

public:
	ScorpDBSell(void);
    ScorpDBSell(std::string db_name, bool is_new);
    void addUser(User user) override;
    void addUserGroups(UserGroup user_group) override;
    void addTransition(Transition transition) override;
    void addTrain(Train train) override;
    void addStation(Station station) override;
    void addRoutePart(RoutePart route_part) override;
    void addRoute(Route route) override;
    void changeDataInColume(TableName table_name, std::string key,
                            std::string colume_name, std::string data) override;
    void deleteFromTable(TableName table_name, std::string key) override;
    std::vector<std::string> getRowFromTable(TableName table_name, std::string key) override;
    std::vector<std::vector<std::string>> getAllRowsFromTable(TableName table_name) override;
    bool isUserExist(std::string login) override;
    bool authenticate(std::string login, std::string password) override;
    std::array<bool, 8> getUserRights(UserGroupName group) override;
    UserGroupName getUserGroup(std::string login) override;
    std::vector<std::string> getRoutsFromAtoB(std::string stA, std::string stB);
	~ScorpDBSell(void);
};

#endif // SCORP_DB_SELL_H
