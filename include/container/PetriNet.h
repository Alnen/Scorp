#ifndef PETRINET_H
#define PETRINET_H

#include <meta/ForEachLooper.h>
#include "container/internal/PetriNetStorage.h"
#include "container/internal/PetriNetHelpers.h"

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

    /*PetriNet(MarkerPropagator&& propagator):
            m_propagator(std::forward<MarkerPropagator>(propagator))
    {
    }*/

    PetriNet(IdGenerator&& idGenerator):
            m_idGenerator(std::forward<IdGenerator>(idGenerator))
    {
    }

    /*PetriNet(MarkerPropagator&& propagator, IdGenerator&& idGenerator):
            m_propagator(std::forward<MarkerPropagator>(propagator)),
            m_idGenerator(std::forward<IdGenerator >(idGenerator))
    {
    }*/

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
        meta::ForEachLooper<StateList, Functor> eraser(functor);
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
        meta::ForEachLooper<StateList, Functor> eraser(functor);
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
        meta::ForEachLooper<MarkerList , MarkersEraser> markersEraser(markersEraserFunctor);
        markersEraser();
        //
        using TransitionLinksEraser = UnnecessaryTransitionLinksEraser<decltype(m_petriNetStorage), StateWrapperType<State>>;
        TransitionLinksEraser transitionLinksEraserFunctor(m_petriNetStorage, iterator->second);
        meta::ForEachLooper<TransitionList , TransitionLinksEraser> transitionLinksEraser(transitionLinksEraserFunctor);
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
        meta::ForEachLooper<TransitionList , StateLinksEraser> transitionLinksEraser(transitionLinksEraserFunctor);
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

    template <class State, class Transition>
    bool addStateToTransitionConnection(IdType stateId, IdType transitionId)
    {
        // TODO: error handling
        auto& stateStorage = m_petriNetStorage.template getStateStorage<State>();
        auto& stateWrapper = stateStorage[stateId];

        auto& transitionStorage = m_petriNetStorage.template getTransitionStorage<Transition>();
        auto& transitionWrapper = transitionStorage[transitionId];

        stateWrapper.template getOutTransitionStorage<Transition>().push_back(transitionId);
        transitionWrapper.template getInStateStorage<State>().push_back(stateId);

        return true;
    };

    template <class Transition, class State>
    bool addTransitionToStateConnection(IdType transitionId, IdType stateId)
    {
        // TODO: error handling
        auto& stateStorage = m_petriNetStorage.template getStateStorage<State>();
        auto& stateWrapper = stateStorage[stateId];

        auto& transitionStorage = m_petriNetStorage.template getTransitionStorage<Transition>();
        auto& transitionWrapper = transitionStorage[transitionId];

        stateWrapper.template getInTransitionStorage<Transition>().push_back(transitionId);
        transitionWrapper.template getOutStateStorage<State>().push_back(stateId);

        return true;
    };

    template <class State, class Transition>
    bool removeStateToTransitionConnection(IdType stateId, IdType transitionId)
    {
        // TODO: error handling
        auto& stateStorage = m_petriNetStorage.template getStateStorage<State>();
        auto& stateWrapper = stateStorage[stateId];
        auto& stateTransitionStorage = stateWrapper.template getOutTransitionStorage<Transition>();
        auto& transitionIdIterator = std::find(stateTransitionStorage.begin(), stateTransitionStorage.end(), transitionId);
        if (transitionIdIterator == stateTransitionStorage.end())
        {
            return false;
        }

        auto& transitionStorage = m_petriNetStorage.template getTransitionStorage<Transition>();
        auto& transitionWrapper = transitionStorage[transitionId];
        auto& transitionStateStorage = transitionWrapper.template getInStateStorage<Transition>();
        auto& stateIdIterator = std::find(transitionStateStorage.begin(), transitionStateStorage.end(), stateId);
        if (stateIdIterator == transitionStateStorage.end())
        {
            return false;
        }

        stateTransitionStorage.erase(transitionIdIterator);
        transitionStateStorage.erase(stateIdIterator);

        return true;
    };

    template <class Transition, class State>
    bool removeTransitionToStateConnection(IdType transitionId, IdType stateId)
    {
        // TODO: error handling
        auto& stateStorage = m_petriNetStorage.template getStateStorage<State>();
        auto& stateWrapper = stateStorage[stateId];
        auto& stateTransitionStorage = stateWrapper.template getInTransitionStorage<Transition>();
        auto& transitionIdIterator = std::find(stateTransitionStorage.begin(), stateTransitionStorage.end(), transitionId);
        if (transitionIdIterator == stateTransitionStorage.end())
        {
            return false;
        }

        auto& transitionStorage = m_petriNetStorage.template getTransitionStorage<Transition>();
        auto& transitionWrapper = transitionStorage[transitionId];
        auto& transitionStateStorage = transitionWrapper.template getOutStateStorage<Transition>();
        auto& stateIdIterator = std::find(transitionStateStorage.begin(), transitionStateStorage.end(), stateId);
        if (stateIdIterator == transitionStateStorage.end())
        {
            return false;
        }

        stateTransitionStorage.erase(transitionIdIterator);
        transitionStateStorage.erase(stateIdIterator);

        return true;
    };

    template <class State, class Transition>
    bool isStateToTransitionConnectionExist(IdType stateId, IdType transitionId)
    {
        // TODO: error handling
        auto& stateStorage = m_petriNetStorage.template getStateStorage<State>();
        auto& stateWrapper = stateStorage[stateId];
        auto& stateTransitionStorage = stateWrapper.template getOutTransitionStorage<Transition>();
        auto& transitionIdIterator = std::find(stateTransitionStorage.begin(), stateTransitionStorage.end(), transitionId);
        if (transitionIdIterator == stateTransitionStorage.end())
        {
            return false;
        }

        auto& transitionStorage = m_petriNetStorage.template getTransitionStorage<Transition>();
        auto& transitionWrapper = transitionStorage[transitionId];
        auto& transitionStateStorage = transitionWrapper.template getInStateStorage<Transition>();
        auto& stateIdIterator = std::find(transitionStateStorage.begin(), transitionStateStorage.end(), stateId);
        if (stateIdIterator == transitionStateStorage.end())
        {
            return false;
        }

        return true;
    };

    template <class Transition, class State>
    bool isTransitionToStateExistConnection(IdType transitionId, IdType stateId)
    {
        // TODO: error handling
        auto& stateStorage = m_petriNetStorage.template getStateStorage<State>();
        auto& stateWrapper = stateStorage[stateId];
        auto& stateTransitionStorage = stateWrapper.template getInTransitionStorage<Transition>();
        auto& transitionIdIterator = std::find(stateTransitionStorage.begin(), stateTransitionStorage.end(), transitionId);
        if (transitionIdIterator == stateTransitionStorage.end())
        {
            return false;
        }

        auto& transitionStorage = m_petriNetStorage.template getTransitionStorage<Transition>();
        auto& transitionWrapper = transitionStorage[transitionId];
        auto& transitionStateStorage = transitionWrapper.template getOutStateStorage<Transition>();
        auto& stateIdIterator = std::find(transitionStateStorage.begin(), transitionStateStorage.end(), stateId);
        if (stateIdIterator == transitionStateStorage.end())
        {
            return false;
        }

        return true;
    };

    template <class... Args>
    void executeMarkersPropagation(Args&&... args)
    {
        for (auto& transition : m_petriNetStorage.template getTransitionStorage<>())
        {

        }
        // For every transition
        // Ask if can perform transition
        // If yes than delete from before and save future transitions
        // If no go to next transition
        // After all transitions are done then apply all saved transformations at once.
    };

private:
    PetriNetStorage<PetriNetTraits> m_petriNetStorage;
    // MarkerPropagator m_propagator;
    IdGenerator m_idGenerator;
};

#endif //PETRINET_H
