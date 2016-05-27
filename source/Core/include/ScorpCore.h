#ifndef SCORP_CORE_H
#define SCORP_CORE_H

#include "ScorpRoute.h"

namespace ScorpCore
{

class Core
{
public:
    Core();

private:
    std::vector<int> m_stationList;
	std::vector<int> m_trainList;
	std::vector<Route> m_routeList;
	//
};

}

#endif // SCORP_CORE_H
