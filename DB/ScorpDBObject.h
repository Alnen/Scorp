#ifndef SCORP_DB_OBJECT_H
#define SCORP_DB_OBJECT_H

#include <fstream>
#include <string>
#include <sstream>

enum class UserGroupName { USER, ADMIN, OPERATOR };

enum class TableName { USER, USER_GROUPS, ROUTE_PART,
                       TRANSITION, TRAIN, ROUTE, STATION
                     };

namespace ScorpDBObject
{

struct User
{
    std::string login;
    std::string password;
    std::string group;

    User(std::string _login, std::string _password, std::string _group)
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

    UserGroup(std::string _name, bool _view_map, bool _edit_map, bool _autorization,
              bool _account_managment, bool _edit_station_info, bool _edit_schedule,
              bool _edit_trains_list, bool _find_trips)
        : name(_name), viewMap(_view_map), editMap(_edit_map), autorization(_autorization),
          accountManagment(_account_managment), editStationInfo(_edit_station_info),
          editSchedule(_edit_schedule), editTrainsList(_edit_trains_list), findTrips(_find_trips)
	{
	}
};

struct Transition
{
    int id;
    int stationFrom;
    int stationTo;

    Transition(int _id, int _station_from, int _station_to)
        : id(_id), stationFrom(_station_from), stationTo(_station_to)
    {
    }
};

struct Train
{
    int number;
    int route;

    Train(int _number, int _route)
        : number(_number), route(_route)
	{
	}
};

struct Station
{
    int id;
    std::string name;
    int x;
    int y;
    int capasity;

    Station(int _id = 0)
        : id(_id), name(""), x(0), y(0), capasity(1)
    {
    }

    Station(int _id, std::string _name, int _x, int _y, int _capasity = 1)
        : id(_id), name(_name), x(_x), y(_y), capasity(1)
	{
	}
};

struct RoutePart
{
    int route;
    int transistion;
    std::string timeOffsetFrom;
    std::string timeOffsetTo;

    RoutePart(int _route, int _transistion, std::string _time_offset_from, std::string _time_offset_to)
        : route(_route), transistion(_transistion), timeOffsetFrom(_time_offset_from), timeOffsetTo(_time_offset_to)
	{
	}
};

struct Route
{
    int id;
    std::string name;

    Route(int _id, std::string _name)
        : id(_id), name(_name)
    {
	}
};

}

#endif // SCORP_DB_OBJECT_H
