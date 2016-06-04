#ifndef SCORP_CORE_H
#define SCORP_CORE_H

#include <memory>
#include "Scorp/Core/ScorpRoute.h"
#include "Scorp/Core/PetriNetUsing.h"
#include "Scorp/DB/RailwayNetDBShell.h"

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
    RailwayNetDBShell m_databaseManager;
	std::unique_ptr<RailwayPetriNet> m_petriNet;
};

}

#endif // SCORP_CORE_H
