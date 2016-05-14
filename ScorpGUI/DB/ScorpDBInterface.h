#ifndef SCORP_DB_INTERFACE_H
#define SCORP_DB_INTERFACE_H

#include"Tables.h"
#include <vector>
#include <array>

enum class UserGroupName { USER, ADMIN, OPERATOR };

enum class TableName { USER, USER_GROUPS, ROUTE_PART,
                       TRANSITION, TRAIN, ROUTE, STATION
                     };

class ScorpDBInterface
{
public:
    virtual void addUser(User user)=0;
    virtual void addUserGroups(UserGroup user_group)=0;
    virtual void addTransition(Transition transition)=0;
    virtual void addTrain(Train train)=0;
    virtual void addStation(Station station)=0;
    virtual void addRoutePart(RoutePart route_part)=0;
    virtual void addRoute(Route route)=0;
    virtual void changeDataInColume(TableName table_name, std::string key,
                                    std::string colume_name, std::string data)=0;
    virtual void deleteFromTable(TableName table_name, std::string key)=0;
    virtual std::vector<std::string> getRowFromTable(TableName table_name, std::string key)=0;
    virtual std::vector<std::vector<std::string>> getAllRowsFromTable(TableName table_name)=0;
    virtual bool isUserExist(std::string login)=0;
    virtual bool authenticate(std::string login, std::string password)=0;
    virtual std::array<bool, 8> getUserRights(UserGroupName group)=0;
    virtual std::string getUserGroup(std::string login)=0;
};

#endif // SCORP_DB_INTERFACE_H
