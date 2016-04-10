#ifndef PETRINET_H
#define PETRINET_H

#include "PetriNetStorage.h"
#include "PetriNetHelpers.h"

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
