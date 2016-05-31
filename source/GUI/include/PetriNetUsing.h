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
#include <random>

#include <iostream>

size_t getRandom(size_t upperBound);
double getRandom();

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
                std::cout << "PropagationSolver(Enter)::getValue<PetriNetComponent::Station> run" << std::endl;
				//IndexType markerId = serializedMarkerInState.template createState<PetriNetComponent::AccessToken>();
				IndexType markerId = serializedMarkerInState.moveState(std::move(inMarkers.front().get()));
                //log move
                MarkerCommandQueue::getInstance().moveMarkerCommand(markerId, serializedMarkerInState.getState().getObjectId());
				break;
            }

            case meta::TypeEnum<StateList, IndexType>::template getValue<PetriNetComponent::Semaphore>():
            {
                std::cout << "PropagationSolver(Enter)::getValue<PetriNetComponent::Semaphore> run" << std::endl;
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
                std::cout << "PropagationSolver(Exit)::getValue<PetriNetComponent::Station> run" << std::endl;
				IndexType markerId = outMarkers.front().get().moveState(std::move(serializedMarkerInState));
                // log move
                MarkerCommandQueue::getInstance().moveMarkerCommand(markerId, outMarkers.front().get().getState().getObjectId());
                break;
            }

            case meta::TypeEnum<StateList, IndexType>::template getValue<PetriNetComponent::Semaphore>():
            {
                std::cout << "PropagationSolver(Exit)::getValue<PetriNetComponent::Semaphore> run" << std::endl;
				// log access point death
                MarkerCommandQueue::getInstance().deleteMarkerCommand(serializedMarkerInState.getResource().getMarker().getObjectId());
                break;
            }

            }
        }
    }
};

template <class _PetriNetTraits, class _Transition, class _State>
class RailwayMarkerMarkerExtractor
{
public:
    using PetriNetTraits = _PetriNetTraits;
    using Transition = _Transition;
    using State = _State;
    using IdType = typename PetriNetTraits::IdType;
    using MarkerList = typename PetriNetTraits::MarkerList;

    boost::optional<std::pair<IdType, IdType>> operator()(
            const container::PetriNet<PetriNetTraits>& petriNet,
            const container::TransitionWrapper<Transition, PetriNetTraits>& transition,
            container::StateWrapper<State, PetriNetTraits>& state) const
    {
        container::internal::MarkerExtractor1<PetriNetTraits, State> extractor(state);
        meta::ForEachLooper<MarkerList, decltype(extractor)> looper(extractor);
        if (looper()) {
            return boost::optional<std::pair<IdType, IdType>>(extractor.serializedMarker);
        }
        return boost::optional<std::pair<IdType, IdType>>();
    }
};

template <class _PetriNetTraits, class _Transition>
class RailwayMarkerMarkerExtractor<_PetriNetTraits, _Transition, PetriNetComponent::Station>
{
public:
    using PetriNetTraits = _PetriNetTraits;
    using Transition = _Transition;
    using State = PetriNetComponent::Station;
    using IdType = typename PetriNetTraits::IdType;
    using MarkerList = typename PetriNetTraits::MarkerList;
    using MarkerEnum = typename meta::TypeEnum<MarkerList, IdType>;

    boost::optional<std::pair<IdType, IdType>> operator()(
            const container::PetriNet<PetriNetTraits>& petriNet,
            const container::TransitionWrapper<Transition, PetriNetTraits>& transition,
            container::StateWrapper<State, PetriNetTraits>& state) const
    {
        auto& markerStorage = state.template getMarkerStorage<PetriNetComponent::Train>();
        if (!markerStorage.empty() && getRandom() < 0.2)
        {
            IdType value = markerStorage.back();
            markerStorage.pop_back();
            return boost::optional<std::pair<IdType, IdType>>(
                    std::make_pair(
                            value,
                            MarkerEnum::template getValue<PetriNetComponent::Train>()));
        }
        else
        {
            return boost::optional<std::pair<IdType, IdType>>();
        }
    }
};

template <class _PetriNetTraits, class _Transition>
class RailwayMarkerMarkerExtractor<_PetriNetTraits, _Transition, PetriNetComponent::InterState>
{
public:
    using PetriNetTraits = _PetriNetTraits;
    using Transition = _Transition;
    using State = PetriNetComponent::InterState;
    using IdType = typename PetriNetTraits::IdType;
    using MarkerList = typename PetriNetTraits::MarkerList;
    using MarkerEnum = typename meta::TypeEnum<MarkerList, IdType>;

    boost::optional<std::pair<IdType, IdType>> operator()(
            const container::PetriNet<PetriNetTraits>& petriNet,
            const container::TransitionWrapper<Transition, PetriNetTraits>& transition,
            container::StateWrapper<State, PetriNetTraits>& state) const
    {
        auto& markerStorage = state.template getMarkerStorage<PetriNetComponent::Train>();
        if (!markerStorage.empty() && getRandom() < 0.2)
        {
            IdType value = markerStorage.back();
            markerStorage.pop_back();
            return boost::optional<std::pair<IdType, IdType>>(
                    std::make_pair(
                                   value,
                                   MarkerEnum::template getValue<PetriNetComponent::Train>()));
        }
        else
        {
            return boost::optional<std::pair<IdType, IdType>>();
        }
    }
};

using _MarkerList = meta::TypeList<PetriNetComponent::Train, PetriNetComponent::AccessToken>;
using _StateList = meta::TypeList<PetriNetComponent::Station, PetriNetComponent::InterState, PetriNetComponent::Semaphore>;
using _TransitionList = meta::TypeList<PetriNetComponent::ExitFromStation, PetriNetComponent::EnterToStation>;

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
    using MarkerExtractor = RailwayMarkerMarkerExtractor<
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

}

using RailwayPetriNet = container::PetriNet<container::PetriNetTraits<_MarkerList, _TransitionList, _StateList>>;
using IdType = typename RailwayPetriNet::IdType;

#endif // PETRINETUSING

