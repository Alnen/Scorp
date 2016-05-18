#ifndef PETRINETUSING
#define PETRINETUSING

#include "PetriNetComponents.h"

#include "container/PetriNetTraits.h"
#include "container/PetriNet.h"
#include "container/PetriNetTraits.h"
#include "meta/RuntimeTypeSwitch.h"
#include "container/TransitionWrapper.h"
#include "container/StateWrapper.h"
#include <utility>

template <class _PetriNetTraits, class Transition>
class RailwayMarkerPropagationSolver
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

using _MarkerList = meta::TypeList<PetryNetComponent::Train, PetryNetComponent::AccessToken>;
using _StateList = meta::TypeList<PetryNetComponent::Station, PetryNetComponent::InterState, PetryNetComponent::Semaphore>;
using _TransitionList = meta::TypeList<PetryNetComponent::ExitFromStation, PetryNetComponent::EnterToStation>;

/*
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
    using MarkerPropagationSolver = RailwayMarkerPropagationSolver<
            PetriNetTraits<_MarkerList, _TransitionList, _StateList>,
            Transition
    >;

    template <class Type>
    using Allocator = allocator::Allocator<Type>;
};
*/

using RailwayPetriNetTraits = container::PetriNetTraits<_MarkerList, _TransitionList, _StateList>;
using RailwayPetriNet = container::PetriNet<RailwayPetriNetTraits>;
using IdType = typename RailwayPetriNetTraits::IdType;

#endif // PETRINETUSING

