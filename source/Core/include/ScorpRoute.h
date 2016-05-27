#ifndef SCORP_ROUTE_H
#define SCORP_ROUTE_H

#include <string>
#include <vector>
#include "ScorpTime.h"

namespace ScorpCore
{

struct RoutePart
{
    int Station;
    Time arrivalTime;
    Time departureTime;
};

class Route
{
public:
    Route(int id, const std::string& name="");
    int getId() const;
	std::string getName() const;
	void setName(const std::string& name);
	void push_back(RoutePart part);
	void pop_back();
	void insert(int station_id, RoutePart part);
	void erase(int station_id);

private:
    int m_id;
    std::string m_name;
    std::vector<RoutePart> m_parts;
};

}

#endif // SCORP_ROUTE_H
