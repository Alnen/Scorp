#ifndef SCORP_DB_SHELL_H
#define SCORP_DB_SHELL_H

#include "ScorpDBObject.h"
#include "DB/sqlite/include/Database.h"
#include <vector>
#include <array>
#include <memory>

class ScorpDBShell
{
public:
    ScorpDBShell(void);
    bool createDatabase(const std::string& path);
    bool connectToDatabase(const std::string& path);
    std::string getDatabasePath() const;
    bool addUser(ScorpDBObject::User user);
    bool addUserGroups(ScorpDBObject::UserGroup user_group);
    bool addTransition(ScorpDBObject::Transition transition);
    bool addTrain(ScorpDBObject::Train train);
    bool addStation(ScorpDBObject::Station station);
    bool addRoutePart(ScorpDBObject::RoutePart route_part);
    bool addRoute(ScorpDBObject::Route route);
    bool changeDataInColume(TableName table_name, const std::string& key, const std::string& colume_name, const std::string& data);
    bool deleteFromTable(TableName table_name, const std::string& key);
    std::vector<std::string> getRowFromTable(TableName table_name, const std::string& key);
    std::vector<std::vector<std::string>> getAllRowsFromTable(TableName table_name);
    bool isUserExist(const std::string& login);
    bool authenticate(const std::string& login, const std::string& password);
    std::array<bool, 8> getUserRights(UserGroupName group);
    UserGroupName getUserGroup(const std::string& login);
    ~ScorpDBShell(void);

private:
    std::string m_databasePath;
    std::unique_ptr<SQLite::Database> m_database;
};

#endif // SCORP_DB_SHELL_H
