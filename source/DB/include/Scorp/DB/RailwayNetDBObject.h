#ifndef RAILWAY_NET_DB_OBJECT_H
#define RAILWAY_NET_DB_OBJECT_H

#include <string>
#include "Scorp/Core/ScorpTime.h"

namespace RailwayNetDBObject
{

struct Transition
{
    int id;
    int stationFrom;
    int stationTo;

    Transition() : id(-1), stationFrom(-1), stationTo(-1)
    {
    }
    
    Transition(int _id, int _station_from, int _station_to)
        : id(_id), stationFrom(_station_from), stationTo(_station_to)
    {
    }
};

struct Train
{
    int number;
    int route;
    
    Train() : number(-1), route(-1)
	{
	}
    
    Train(int _number, int _route) : number(_number), route(_route)
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

    Station(int _id = -1) : id(_id), name(""), x(0), y(0), capasity(1)
    {
    }

    Station(int _id, const std::string& _name, int _x, int _y, int _capasity = 1)
        : id(_id), name(_name), x(_x), y(_y), capasity(1)
	{
	}
};

struct RoutePart
{
    int id;
	int route;
    int transition;
    ScorpCore::Time timeOffsetFrom;
    ScorpCore::Time timeOffsetTo;
    
    RoutePart()
        : id(-1), route(-1), transition(-1)
	{
	}
    
    RoutePart(int _id, int _route, int _transition, ScorpCore::Time _time_offset_from, ScorpCore::Time _time_offset_to)
        : id(_id), route(_route), transition(_transition), timeOffsetFrom(_time_offset_from), timeOffsetTo(_time_offset_to)
	{
	}
};

struct Route
{
    int id;
    std::string name;

    Route() : id(-1), name("")
    {
	}

    Route(int _id, const std::string& _name) : id(_id), name(_name)
    {
	}
};

}

#endif // RAILWAY_NET_DB_OBJECT_H
