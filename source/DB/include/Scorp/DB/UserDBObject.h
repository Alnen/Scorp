#ifndef USER_DB_OBJECT_H
#define USER_DB_OBJECT_H

#include <string>

enum class UserGroupName { User, Admin, Operator };
enum class UserRight { ViewMap, EditMap, Authorization, AccountManagement, EditStationInfo,
    EditSchedule, EditTrainsList, FindTrips };

std::string userRightToString(UserRight right);
std::string userGroupToString(UserGroupName group);
UserGroupName userGroupFromString(std::string group_name);
    
namespace UserDBObject
{

struct User
{
    std::string login;
    std::string password;
    UserGroupName group;

    User(const std::string& _login, const std::string& _password, UserGroupName _group)
        : login(_login), password(_password), group(_group)
	{
    }
};

struct UserGroup
{
    std::string name;
    bool viewMap;
    bool editMap;
    bool autorization;
    bool accountManagment;
    bool editStationInfo;
    bool editSchedule;
    bool editTrainsList;
    bool findTrips;

    UserGroup(const std::string& _name, bool _view_map, bool _edit_map, bool _autorization,
              bool _account_managment, bool _edit_station_info, bool _edit_schedule,
              bool _edit_trains_list, bool _find_trips)
        : name(_name), viewMap(_view_map), editMap(_edit_map), autorization(_autorization),
          accountManagment(_account_managment), editStationInfo(_edit_station_info),
          editSchedule(_edit_schedule), editTrainsList(_edit_trains_list), findTrips(_find_trips)
	{
	}
};

}

#endif // USER_DB_OBJECT_H