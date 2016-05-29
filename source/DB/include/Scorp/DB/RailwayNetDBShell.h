#ifndef RAILWAY_NET_DB_SHELL_H
#define RAILWAY_NET_DB_SHELL_H

#include <vector>
#include "Scorp/Core/ScorpTime.h"
#include "Scorp/DB/ScorpDBShell.h"
#include "Scorp/DB/RailwayNetDBObject.h"

class RailwayNetDBShell : public ScorpDBShell
{
public:
    RailwayNetDBShell();
    ~RailwayNetDBShell();
    bool createDatabase(const std::string& path) override;
    bool addTransition(RailwayNetDBObject::Transition transition);
    bool addTrain(RailwayNetDBObject::Train train);
    bool addStation(RailwayNetDBObject::Station station);
    bool addRoutePart(RailwayNetDBObject::RoutePart route_part);
    bool addRoute(RailwayNetDBObject::Route route);

    bool changeTransition(int id, int station_from, int station_to);
    bool changeTransitionStationFrom(int id, int value);
    bool changeTransitionStationTo(int id, int value);
    bool changeTrainRoute(int number, int value);
    bool changeStation(int id, const std::string& name, int x, int y, int capasity);
    bool changeStationName(int id, const std::string& name);
    bool changeStationCoord(int id, int x, int y);
    bool changeStationCapasity(int id, int value);
    bool changeRoutePartTime(int id, const ScorpCore::Time& time_from, const ScorpCore::Time& time_to);
    bool changeRoutePartTimeFrom(int id, const ScorpCore::Time& time);
    bool changeRoutePartTimeTo(int id, const ScorpCore::Time& time);
    bool changeRouteName(int id, const std::string& name);

    bool removeTransition(int id);
    bool removeTrain(int number);
    bool removeStation(int id);
    bool removeRoutePart(int id);
    bool removeRoute(int id);

    RailwayNetDBObject::Transition getTransition(int id);
    RailwayNetDBObject::Train getTrain(int number);
    RailwayNetDBObject::Station getStation(int id);
    RailwayNetDBObject::RoutePart getRoutePart(int id);
    RailwayNetDBObject::Route getRoute(int id);

    std::vector<RailwayNetDBObject::Transition> getTransitions();
    std::vector<RailwayNetDBObject::Train> getTrains();
    std::vector<RailwayNetDBObject::Station> getStations();
    std::vector<RailwayNetDBObject::RoutePart> getRouteParts();
    std::vector<RailwayNetDBObject::Route> getRoutes();
    
    bool removeAllTransitions();
    bool removeAllTrains();
    bool removeAllStations();
    bool removeAllRouteParts();
    bool removeAllRoutes();
    
    std::vector<std::pair<int, std::string>> getStationNameList();
    //std::vector<RailwayNetDBObject::Route> getRoutes(int station_1, int station_2);
    //std::vector<std::string> getRoutsFromAtoB(std::string stA, std::string stB);
};

#endif // RAILWAY_NET_DB_SHELL_H
