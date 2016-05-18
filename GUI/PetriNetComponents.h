#ifndef PETRI_NET_COMPONENTS
#define PETRI_NET_COMPONENTS

#include "container/PetriNetTraits.h"
#include "container/PetriNet.h"
#include "meta/RuntimeTypeSwitch.h"
#include "container/TransitionWrapper.h"
#include "container/StateWrapper.h"
#include <utility>

namespace PetryNetComponent
{
class Station
{
private:
    std::string m_name;
    int m_x;
    int m_y;
    int m_capacity;

public:
    Station() : m_name(""), m_x(0), m_y(0), m_capacity(1)
    {
    }

    Station(std::string name, int x, int y, int capacity=1)
        : m_name(name), m_x(x), m_y(y), m_capacity(capacity)
    {
    }

    std::string getName() const
    {
        return m_name;
    }

    int getX() const
    {
        return m_x;
    }

    int getY() const
    {
        return m_y;
    }

    int getCapacity() const
    {
        return m_capacity;
    }
};

class InterState
{
public:
    InterState()
    {
    }
};

class Semaphore
{
public:
    Semaphore()
    {
    }
};

class AccessToken
{
public:
    AccessToken()
    {
    }
};

class Train
{
private:
    int m_number;

public:
    Train(int number) : m_number(number)
    {
    }

    int getNumber() const
    {
        return m_number;
    }
};

class ExitFromStation
{
public:
    ExitFromStation()
    {
    }
};

class EnterToStation
{
public:
    EnterToStation()
    {
    }
};

template <class _PetriNetTraits, class Transition>
class MarkerPropagationSolver
{
public:
    using PetriNetTraits = _PetriNetTraits;
    using IndexType = typename PetriNetTraits::IdType;
    using TransitionList = typename PetriNetTraits::TransitionList;
    using MarkerList = typename PetriNetTraits::MarkerList;
    using PetriNetType = container::PetriNet<PetriNetTraits>;
    template <class Resource, class Deleter>
    using ResourceHolder = typename PetriNetType::template ResourceHolder<Resource, Deleter>;
    using SerializedMarkerInState = typename container::PetriNet<PetriNetTraits>::template SerializedMarkerInState<IndexType>;
    using MarkerFiller = typename container::PetriNet<PetriNetTraits>::MarkerFiller;

    template <class Deleter>
    void operator()(
            container::PetriNet<PetriNetTraits>& petriNet,
            const container::TransitionWrapper<Transition, PetriNetTraits>& transition,
            const std::vector<std::reference_wrapper<ResourceHolder<SerializedMarkerInState, Deleter>>>& inMarkers,
            const std::vector<std::reference_wrapper<MarkerFiller>>& outMarkers)
    {
        for (const auto& outMarker : outMarkers)
        {
            auto& serializedMarkerInState = outMarker.get();
            IndexType markerId = serializedMarkerInState.template createState<typename MarkerList::Head>();
        }
    }
};

using _MarkerList = meta::TypeList<Train, AccessToken>;
using _StateList = meta::TypeList<Station, InterState, Semaphore>;
using _TransitionList = meta::TypeList<ExitFromStation, EnterToStation>;

template <>
struct container::PetriNetTraits<_MarkerList, _StateList, _TransitionList>
{
    using MarkerList = _MarkerList;
    using TransitionList = _TransitionList;
    using StateList = _StateList;
    using IdType = int;
    using IdGenerator = IntegralIdGenerator<IdType>;

    template <class Transition, class State>
    using MarkerExtractor = MarkerExtractor<
            PetriNetTraits<_MarkerList, _TransitionList, _StateList>,
            Transition,
            State>;

    template <class Transition>
    using MarkerPropagationSolver = MarkerPropagationSolver<
            PetriNetTraits<_MarkerList, _TransitionList, _StateList>,
            Transition
    >;

    template <class Type>
    using Allocator = allocator::Allocator<Type>;
};

using RailwayPetriNetTraits = PetriNetTraits<_MarkerList, _TransitionList, _StateList>;
using RailwayPetriNet = container::PetriNet<RailwayPetriNetTraits>;
using IdType = typename RailwayPetriNetTraits::IdType;
}

#endif // PETRI_NET_COMPONENTS
