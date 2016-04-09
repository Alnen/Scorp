#ifndef PETRINET_H
#define PETRINET_H

#include "PetriNetStorage.h"

template <class Marker, class Storage, class IdType>
struct MarkerEraser
{
    MarkerEraser(Storage& storage, IdType markerId, IdType parentId):
            m_storage(storage),
            m_stateId(parentId),
            m_markerId(markerId)
    {
    }

    MarkerEraser() = default;
    MarkerEraser(const MarkerEraser&) = default;

    template <class State>
    bool operator()()
    {
        auto& storage = m_storage.template getStateStorage<State>();
        using ValueType = typename std::remove_reference<decltype(storage)>::type::value_type;
        auto iterator = std::find_if(storage.begin(), storage.end(),
                                     [this](const ValueType& state)
                                     {
                                         return state.first == m_stateId;
                                     });
        if (iterator == storage.end())
        {
            return false;
        }
        auto& markerStorage = iterator->second.template getMarkerStorage<Marker>();
        auto markerIterator = std::find(markerStorage.begin(), markerStorage.end(), m_markerId);
        if (markerIterator == markerStorage.end())
        {
            return false;
        }
        markerStorage.erase(markerIterator);
        return true;
    }

    Storage& m_storage;
    IdType m_stateId;
    IdType m_markerId;
};

template <class Marker, class Storage, class IdType>
struct MarkerAdder
{
    MarkerAdder(Storage& storage, IdType markerId, IdType parentId):
            m_storage(storage),
            m_parentId(parentId),
            m_markerId(markerId)
    {
    }

    MarkerAdder() = default;
    MarkerAdder(const MarkerAdder&) = default;

    template <class State>
    bool operator()()
    {
        auto& storage = m_storage.template getStateStorage<State>();
        using ValueType = typename std::remove_reference<decltype(storage)>::type::value_type;
        auto iterator = std::find_if(storage.begin(), storage.end(),
                                     [this](const ValueType& state)
                                     {
                                         return state.first == m_parentId;
                                     });
        if (iterator == storage.end())
        {
            return false;
        }
        iterator->second.template getMarkerStorage<Marker>().push_back(m_markerId);
        return true;
    }

    Storage& m_storage;
    IdType m_parentId;
    IdType m_markerId;
};

template <class Storage, class StateType>
struct UnnecessaryMarkersEraser
{
    UnnecessaryMarkersEraser(Storage& storage, StateType& stateId):
            m_storage(storage),
            m_state(stateId)
    {
    }

    UnnecessaryMarkersEraser() = default;
    UnnecessaryMarkersEraser(const UnnecessaryMarkersEraser&) = default;

    template <class Marker>
    bool operator()()
    {
        auto& markerStorage = m_storage.template getMarkerStorage<Marker>();
        auto& stateMarkerIdStorage = m_state.template getMarkerStorage<Marker>();

        for (auto id : stateMarkerIdStorage) {
            markerStorage.erase(id);
        }
        stateMarkerIdStorage.clear();

        return false;
    }

    Storage& m_storage;
    StateType& m_state;
};

template <class Storage, class StateWrapper>
struct UnnecessaryTransitionLinksEraser
{
    UnnecessaryTransitionLinksEraser(Storage& storage, StateWrapper& stateId):
            m_storage(storage),
            m_state(stateId)
    {
    }

    UnnecessaryTransitionLinksEraser() = default;
    UnnecessaryTransitionLinksEraser(const UnnecessaryTransitionLinksEraser&) = default;

    template <class Transition>
    bool operator()()
    {
        auto& transitionStorage = m_storage.template getTransitionStorage<Transition>();

        auto& stateInTransitionIdStorage = m_state.template getInTransitionStorage<Transition>();
        for (auto id : stateInTransitionIdStorage) {
            auto& transition = transitionStorage.at(id);
            auto& stateIdStorage = transition.template getInStateStorage<typename StateWrapper::State>();
            stateIdStorage.erase(std::find(stateIdStorage.begin(), stateIdStorage.end(), m_state.getId()));
        }

        auto& stateOutTransitionIdStorage = m_state.template getOutTransitionStorage<Transition>();
        for (auto id : stateOutTransitionIdStorage) {
            auto& transition = transitionStorage.at(id);
            auto& stateIdStorage = transition.template getOutStateStorage<typename StateWrapper::State>();
            stateIdStorage.erase(std::find(stateIdStorage.begin(), stateIdStorage.end(), m_state.getId()));
        }

        return false;
    }

    Storage& m_storage;
    StateWrapper& m_state;
};

template <class Storage, class TransitionWrapper>
struct UnnecessaryStateLinksEraser
{
    UnnecessaryStateLinksEraser(Storage& storage, TransitionWrapper& stateId):
            m_storage(storage),
            m_transition(stateId)
    {
    }

    UnnecessaryStateLinksEraser() = default;
    UnnecessaryStateLinksEraser(const UnnecessaryStateLinksEraser&) = default;

    template <class State>
    bool operator()()
    {
        auto& stateStorage = m_storage.template getStateStorage<State>();

        auto& transitionInStateIdStorage = m_transition.template getInStateStorage<State>();
        for (auto id : transitionInStateIdStorage) {
            auto& state = stateStorage.at(id);
            auto& stateIdStorage = state.template getInTransitionStorage<typename TransitionWrapper::Transition>();
            stateIdStorage.erase(std::find(stateIdStorage.begin(), stateIdStorage.end(), m_transition.getId()));
        }

        auto& transitionOutStateIdStorage = m_transition.template getOutStateStorage<State>();
        for (auto id : transitionOutStateIdStorage) {
            auto& state = stateStorage.at(id);
            auto& stateIdStorage = state.template getOutTransitionStorage<typename TransitionWrapper::Transition>();
            stateIdStorage.erase(std::find(stateIdStorage.begin(), stateIdStorage.end(), m_transition.getId()));
        }

        return false;
    }

    Storage& m_storage;
    TransitionWrapper& m_transition;
};

template <class _PetriNetTraits>
class PetriNet
{
public:
    using PetriNetTraits = _PetriNetTraits;
    using IdGenerator = typename PetriNetTraits::IdGenerator;
    using MarkerList = typename PetriNetTraits::MarkerList;
    using TransitionList = typename PetriNetTraits::TransitionList;
    using StateList = typename PetriNetTraits::StateList;
    using IdType = typename  PetriNetTraits::IdType;
    template <class Marker>
    using MarkerWrapperType = typename PetriNetStorage<PetriNetTraits>::template SpecializedMarkerWrapper<Marker>::type;
    template <class State>
    using StateWrapperType = typename PetriNetStorage<PetriNetTraits>::template SpecializedStateWrapper<State>::type;
    template <class Transition>
    using TransitionWrapperType = typename PetriNetStorage<PetriNetTraits>::template SpecializedTransitionWrapper<Transition>::type;

    template <class Marker>
    using MarkerIterator = typename boost::container::flat_map<IdType, MarkerWrapperType<Marker>>::iterator;
    template <class State>
    using StateIterator = typename boost::container::flat_map<IdType, StateWrapperType<State>>::iterator;
    template <class Transition>
    using TransitionIterator = typename boost::container::flat_map<IdType, TransitionWrapperType<Transition>>::iterator;


    PetriNet()
    {
    }

    template <class Marker>
    IdType addMarker(IdType parentId, Marker&& marker)
    {
        IdType markerId = m_idGenerator();
        auto& markerStorage = m_petriNetStorage.template getMarkerStorage<Marker>();
        markerStorage.emplace(
                std::make_pair(
                        markerId,
                        MarkerWrapperType<Marker>(
                                markerId,
                                parentId,
                                std::forward<Marker>(marker)
                        )
                )
        );
        //
        using Functor = MarkerAdder<Marker, decltype(m_petriNetStorage), IdType>;
        Functor functor(m_petriNetStorage, markerId, parentId);
        ForEachLooper<StateList, Functor> eraser(functor);
        if (!eraser())
        {
            throw std::runtime_error("Couldn't find father");
        }
        //
        return markerId;
    }

    template <class Marker>
    Marker& getMarkerById(IdType id)
    {
        auto& markerStorage = m_petriNetStorage.template getMarkerStorage<Marker>();
        auto& markerWrapper = markerStorage[id];
        return markerWrapper.getMarker();
    }

public:
    template <class Marker>
    bool removeMarker(IdType id)
    {
        auto& markerStorage = m_petriNetStorage.template getMarkerStorage<Marker>();
        auto iterator = markerStorage.find(id);
        if (iterator == markerStorage.end())
        {
            return false;
        }
        markerStorage.erase(iterator);

        // Delete id from father's marker storage
        IdType stateId = iterator->second.getStateId();
        IdType markerId = iterator->second.getId();

        using Functor = MarkerEraser<Marker, decltype(m_petriNetStorage), IdType>;
        Functor functor(m_petriNetStorage, markerId, stateId);
        ForEachLooper<StateList, Functor> eraser(functor);
        if (!eraser())
        {
            throw std::runtime_error("Couldn't find father");
        }

        return true;
    }

    template <class State>
    IdType addState(State&& state)
    {
        IdType newId = m_idGenerator();
        auto& stateStorage = m_petriNetStorage.template getStateStorage<State>();
        stateStorage.emplace(
                std::make_pair(
                        newId,
                        StateWrapperType<State>(
                                newId,
                                std::forward<State>(state)
                        )
                )
        );

        return newId;
    }

    template <class Transition>
    IdType addTransition(Transition&& transition)
    {
        IdType transitionId = m_idGenerator();
        auto& transitionStorage = m_petriNetStorage.template getTransitionStorage<Transition>();
        transitionStorage.emplace(
                std::make_pair(
                        transitionId,
                        TransitionWrapperType<Transition>(
                                transitionId,
                                std::forward<Transition>(transition)
                        )
                )
        );

        return transitionId;
    }


    template <class State>
    bool removeState(IdType stateId)
    {
        auto& stateStorage = m_petriNetStorage.template getStateStorage<State>();
        auto iterator = stateStorage.find(stateId);
        if (iterator == stateStorage.end())
        {
            return false;
        }
        // Remove markers
        using MarkersEraser = UnnecessaryMarkersEraser<decltype(m_petriNetStorage), StateWrapperType<State>>;
        MarkersEraser markersEraserFunctor(m_petriNetStorage, iterator->second);
        ForEachLooper<MarkerList , MarkersEraser> markersEraser(markersEraserFunctor);
        markersEraser();
        //
        using TransitionLinksEraser = UnnecessaryTransitionLinksEraser<decltype(m_petriNetStorage), StateWrapperType<State>>;
        TransitionLinksEraser transitionLinksEraserFunctor(m_petriNetStorage, iterator->second);
        ForEachLooper<TransitionList , TransitionLinksEraser> transitionLinksEraser(transitionLinksEraserFunctor);
        transitionLinksEraser();
        //
        stateStorage.erase(iterator);
        return true;
    }

    template <class Transition>
    bool removeTransition(IdType transitionId)
    {
        auto& transitionStorage = m_petriNetStorage.template getTransitionStorage<Transition>();
        auto iterator = transitionStorage.find(transitionId);
        if (iterator == transitionStorage.end())
        {
            return false;
        }
        //
        using StateLinksEraser = UnnecessaryStateLinksEraser<decltype(m_petriNetStorage), TransitionWrapperType<Transition>>;
        StateLinksEraser transitionLinksEraserFunctor(m_petriNetStorage, iterator->second);
        ForEachLooper<TransitionList , StateLinksEraser> transitionLinksEraser(transitionLinksEraserFunctor);
        transitionLinksEraser();
        //
        transitionStorage.erase(iterator);
        return true;
    }


    template <class Marker>
    MarkerIterator<Marker> beginMarker()
    {
        return m_petriNetStorage.template getMarkerStorage<Marker>().begin();
    }


    template <class State>
    StateIterator<State> beginState()
    {
        return m_petriNetStorage.template getStateStorage<State>().begin();
    }


    template <class Transition>
    TransitionIterator<Transition> beginTransition()
    {
        return m_petriNetStorage.template getTransitionStorage<Transition>().begin();
    }


    template <class Marker>
    MarkerIterator<Marker> endMarker()
    {
        return m_petriNetStorage.template getMarkerStorage<Marker>().end();
    }


    template <class State>
    StateIterator<State> endState()
    {
        return m_petriNetStorage.template getStateStorage<State>().end();
    }


    template <class Transition>
    TransitionIterator<Transition> endTransition()
    {
        return m_petriNetStorage.template getTransitionStorage<Transition>().end();
    }


    template <class Marker>
    size_t sizeMarker()
    {
        return m_petriNetStorage.template getMarkerStorage<Marker>().size();
    }


    template <class State>
    size_t sizeState()
    {
        return m_petriNetStorage.template getStateStorage<State>().size();
    }


    template <class Transition>
    size_t sizeTransition()
    {
        return m_petriNetStorage.template getTransitionStorage<Transition>().size();
    }


    template <class Marker>
    bool emptyMarker()
    {
        return m_petriNetStorage.template getMarkerStorage<Marker>().empty();
    }


    template <class State>
    bool emptyState()
    {
        return m_petriNetStorage.template getStateStorage<State>().empty();
    }


    template <class Transition>
    bool emptyTransition()
    {
        return m_petriNetStorage.template getTransitionStorage<Transition>().empty();
    }

private:
    PetriNetStorage<PetriNetTraits> m_petriNetStorage;
    IdGenerator m_idGenerator;
};

#endif //PETRINET_H
