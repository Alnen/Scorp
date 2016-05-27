#ifndef PETRINETUSING
#define PETRINETUSING

#include "PetriNetComponents.h"

#include "Scorp/container/PetriNetTraits.h"
#include "Scorp/container/PetriNet.h"
#include "Scorp/container/PetriNetTraits.h"
#include "Scorp/meta/RuntimeTypeSwitch.h"
#include "Scorp/container/TransitionWrapper.h"
#include "Scorp/container/StateWrapper.h"
#include "Map/MarkerCommandQueue.h"
#include <utility>

template <class _PetriNetTraits, class Transition>
class RailwayMarkerPropagationSolver;

template <class _PetriNetTraits>
class RailwayMarkerPropagationSolver<_PetriNetTraits, PetriNetComponent::EnterToStation>
{
public:
    using PetriNetTraits = _PetriNetTraits;
    using IndexType = typename PetriNetTraits::IdType;
    using TransitionList = typename PetriNetTraits::TransitionList;
    using MarkerList = typename PetriNetTraits::MarkerList;
    using StateList = typename PetriNetTraits::StateList;
    using PetriNetType = container::PetriNet<PetriNetTraits>;
    template <class Resource, class Deleter>
    using ResourceHolder = typename PetriNetType::template ResourceHolder<Resource, Deleter>;
    using SerializedMarkerInState = typename container::PetriNet<PetriNetTraits>::template SerializedMarkerInState<IndexType>;
    using MarkerFiller = typename container::PetriNet<PetriNetTraits>::MarkerFiller;

    template <class Deleter>
    void operator()(
            container::PetriNet<PetriNetTraits>& petriNet,
            const container::TransitionWrapper<PetriNetComponent::EnterToStation, PetriNetTraits>& transition,
            const std::vector<std::reference_wrapper<ResourceHolder<SerializedMarkerInState, Deleter>>>& inMarkers,
            const std::vector<std::reference_wrapper<MarkerFiller>>& outMarkers)
    {
        for (const std::reference_wrapper<MarkerFiller>& outMarker : outMarkers)
        {
            MarkerFiller& serializedMarkerInState = outMarker.get();
            switch (serializedMarkerInState.getState().getObjectSerializedType())
            {

            case meta::TypeEnum<StateList, IndexType>::template getValue<PetriNetComponent::Station>():
            {
                IndexType markerId = serializedMarkerInState.moveState(std::move(inMarkers.front().get()));
                //log move
                MarkerCommandQueue::getInstance().moveMarkerCommand(markerId, serializedMarkerInState.getState().getObjectId());
                break;
            }

            case meta::TypeEnum<StateList, IndexType>::template getValue<PetriNetComponent::Semaphore>():
            {
                IndexType markerId = serializedMarkerInState.template createState<PetriNetComponent::AccessToken>();
                // log new point
                MarkerCommandQueue::getInstance().addMarkerCommand(markerId, serializedMarkerInState.getState().getObjectId());
                break;
            }

            }
        }
    }
};

template <class _PetriNetTraits>
class RailwayMarkerPropagationSolver<_PetriNetTraits, PetriNetComponent::ExitFromStation>
{
public:
    using PetriNetTraits = _PetriNetTraits;
    using IndexType = typename PetriNetTraits::IdType;
    using TransitionList = typename PetriNetTraits::TransitionList;
    using MarkerList = typename PetriNetTraits::MarkerList;
    using StateList = typename PetriNetTraits::StateList;
    using PetriNetType = container::PetriNet<PetriNetTraits>;
    template <class Resource, class Deleter>
    using ResourceHolder = typename PetriNetType::template ResourceHolder<Resource, Deleter>;
    using SerializedMarkerInState = typename container::PetriNet<PetriNetTraits>::template SerializedMarkerInState<IndexType>;
    using MarkerFiller = typename container::PetriNet<PetriNetTraits>::MarkerFiller;

    template <class Deleter>
    void operator()(
            container::PetriNet<PetriNetTraits>& petriNet,
            const container::TransitionWrapper<PetriNetComponent::ExitFromStation, PetriNetTraits>& transition,
            const std::vector<std::reference_wrapper<ResourceHolder<SerializedMarkerInState, Deleter>>>& inMarkers,
            const std::vector<std::reference_wrapper<MarkerFiller>>& outMarkers)
    {
        for (const std::reference_wrapper<ResourceHolder<SerializedMarkerInState, Deleter>>& outMarker : inMarkers)
        {
            ResourceHolder<SerializedMarkerInState, Deleter>& serializedMarkerInState = outMarker.get();
            switch (serializedMarkerInState.getResource().getState().getObjectSerializedType())
            {

            case meta::TypeEnum<StateList, IndexType>::template getValue<PetriNetComponent::Station>():
            {
                IndexType markerId = outMarkers.front().get().moveState(std::move(serializedMarkerInState));
                // log move
                MarkerCommandQueue::getInstance().moveMarkerCommand(markerId, outMarkers.front().get().getState().getObjectId());
                break;
            }

            case meta::TypeEnum<StateList, IndexType>::template getValue<PetriNetComponent::Semaphore>():
            {
                // log access point death
                MarkerCommandQueue::getInstance().deleteMarkerCommand(serializedMarkerInState.getResource().getMarker().getObjectId());
                break;
            }

            }
        }
    }
};

using _MarkerList = meta::TypeList<PetriNetComponent::Train, PetriNetComponent::AccessToken>;
using _StateList = meta::TypeList<PetriNetComponent::Station, PetriNetComponent::InterState, PetriNetComponent::Semaphore>;
using _TransitionList = meta::TypeList<PetriNetComponent::ExitFromStation, PetriNetComponent::EnterToStation>;

struct RailwayPetriNetTraits {
    using MarkerList = _MarkerList;
    using TransitionList = _TransitionList;
    using StateList = _StateList;
    using IdType = int;
    using IdGenerator = IntegralIdGenerator<IdType>;

    template <class Transition, class State>
    using MarkerExtractor = container::internal::MarkerExtractor<
            RailwayPetriNetTraits,
            Transition,
            State>;

    template <class Transition>
    using MarkerPropagationSolver = RailwayMarkerPropagationSolver<
            RailwayPetriNetTraits,
            Transition
    >;

    template <class Type>
    using Allocator = std::allocator<Type>;
};
namespace container
{

template <>
struct PetriNetTraits<_MarkerList, _TransitionList, _StateList>
{
    using MarkerList = _MarkerList;
    using TransitionList = _TransitionList;
    using StateList = _StateList;
    using IdType = int;
    using IdGenerator = IntegralIdGenerator<IdType>;

    template <class Transition, class State>
    using MarkerExtractor = container::internal::MarkerExtractor<
            RailwayPetriNetTraits,
            Transition,
            State>;

    template <class Transition>
    using MarkerPropagationSolver = RailwayMarkerPropagationSolver<
            RailwayPetriNetTraits,
            Transition
    >;

    template <class Type>
    using Allocator = std::allocator<Type>;
};

}

using RailwayPetriNet = container::PetriNet<RailwayPetriNetTraits>;
//using RailwayPetriNet = container::PetriNet<container::PetriNetTraits<_MarkerList, _TransitionList, _StateList>>;
using IdType = typename RailwayPetriNetTraits::IdType;

#endif // PETRINETUSING

