#ifndef SCORP_ROUTE_H
#define SCORP_ROUTE_H

#include <string>
#include <vector>
#include "ScorpTime.h"

namespace ScorpCore
{

struct RoutePart
{
    int station;
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
	void push_back(const RoutePart& part);
	void pop_back();
	void insert(size_t index, const RoutePart& part);
	void erase(size_t index);
	void insertAfterStation(int station_id, const RoutePart& part);
	void eraseStation(int station_id);

private:
    int m_id;
    std::string m_name;
    std::vector<RoutePart> m_parts;
};

}

#endif // SCORP_ROUTE_H
