#ifndef PETRINET_H
#define PETRINET_H

#include <meta/ForEachLooper.h>
#include <meta/TypeEnum.h>
#include <meta/TypeWriter.h>
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

    template <class Transition, class State>
    using MarkerExtractor = typename PetriNetTraits::template MarkerExtractor<Transition, State>;

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

    PetriNet():
        m_propagator(*this)
    {
    }

    PetriNet(IdGenerator&& idGenerator):
        m_propagator(*this),
        m_idGenerator(std::forward<IdGenerator>(idGenerator))
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
    const Marker& getMarkerById(IdType id) const
    {
        auto& markerStorage = m_petriNetStorage.template getMarkerStorage<Marker>();
        auto& markerWrapper = markerStorage.find(id)->second;
        return markerWrapper.getMarker();
    }

    template <class State>
    State& getStateById(IdType id)
    {
        auto& stateStorage = m_petriNetStorage.template getStateStorage<State>();
        auto& stateWrapper = stateStorage[id];
        return stateWrapper.getState();
    }

    template <class State>
    const State& getStateById(IdType id) const
    {
        auto& stateStorage = m_petriNetStorage.template getStateStorage<State>();
        auto& stateWrapper = stateStorage.find(id)->second;
        return stateWrapper.getState();
    }

    template <class Transition>
    Transition& getTransitionById(IdType id)
    {
        auto& transitionStorage = m_petriNetStorage.template getTransitionStorage<Transition>();
        auto& transitionWrapper = transitionStorage[id];
        return transitionWrapper.getTransition();
    }

    template <class Transition>
    const Transition& getTransitionById(IdType id) const
    {
        auto& transitionStorage = m_petriNetStorage.template getTransitionStorage<Transition>();
        auto& transitionWrapper = transitionStorage.find(id)->second;
        return transitionWrapper.getTransition();
    }

    //

    template <class Marker>
    const MarkerWrapperType<Marker>& getMarkerWrapperById(IdType id) const
    {
        auto& markerStorage = m_petriNetStorage.template getMarkerStorage<Marker>();
        auto& markerWrapper = markerStorage.find(id)->second;
        return markerWrapper;
    }

    template <class State>
    const StateWrapperType<State>& getStateWrapperById(IdType id) const
    {
        auto& stateStorage = m_petriNetStorage.template getStateStorage<State>();
        auto& stateWrapper = stateStorage.find(id)->second;
        return stateWrapper;
    }

    template <class Transition>
    const StateWrapperType<Transition>& getTransitionWrapperById(IdType id) const
    {
        auto& transitionStorage = m_petriNetStorage.template getTransitionStorage<Transition>();
        auto& transitionWrapper = transitionStorage.find(id)->second;
        return transitionWrapper;
    }

    //

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
        auto& stateWrapper = *stateStorage.find(stateId);

        auto& transitionStorage = m_petriNetStorage.template getTransitionStorage<Transition>();
        auto& transitionWrapper = *transitionStorage.find(transitionId);

        stateWrapper.second.template getOutTransitionStorage<Transition>().push_back(transitionId);
        transitionWrapper.second.template getInStateStorage<State>().push_back(stateId);

        return true;
    };

    template <class Transition, class State>
    bool addTransitionToStateConnection(IdType transitionId, IdType stateId)
    {
        // TODO: error handling
        auto& stateStorage = m_petriNetStorage.template getStateStorage<State>();
        auto& stateWrapper = *stateStorage.find(stateId);

        auto& transitionStorage = m_petriNetStorage.template getTransitionStorage<Transition>();
        auto& transitionWrapper = *transitionStorage.find(transitionId);

        stateWrapper.second.template getInTransitionStorage<Transition>().push_back(transitionId);
        transitionWrapper.second.template getOutStateStorage<State>().push_back(stateId);

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
        std::cout << "Hello" << std::endl;
        m_propagator();
        // For every transition
        // Ask if can perform transition
        // If yes than delete from before and save future transitions
        // If no go to next transition
        // After all transitions are done then apply all saved transformations at once.
    };

    template<class IndexType>
    class SerializedObject {
    public:
        SerializedObject(IndexType objectId, IndexType objectTypeId) :
                m_objectId(objectId),
                m_objectSerializedType(objectTypeId) { }

        SerializedObject(const SerializedObject<IndexType> &) = default;

        IndexType getObjectId() const {
            return m_objectId;
        }

        IndexType getObjectSerializedType() const {
            return m_objectSerializedType;
        }

    private:
        IndexType m_objectId;
        IndexType m_objectSerializedType;
    };

private:
    class MarkerPropagationExecutor;

    class TransitionMarkerPropagator;

    template <class _Transition>
    class TransitionStateMarkerPropagator;

    PetriNetStorage<PetriNetTraits> m_petriNetStorage;
    MarkerPropagationExecutor m_propagator;
    IdGenerator m_idGenerator;

template<class IndexType>
class SerializedMarkerInState
{
public:
    using SerializedMarker = SerializedObject<IndexType>;
    using SerializedState = SerializedObject<IndexType>;

    SerializedMarkerInState(IndexType stateId, IndexType stateTypeId, IndexType markerId, IndexType markerTypeId) :
            m_state(stateId, stateTypeId),
            m_marker(markerId, markerTypeId)
    { }

    SerializedMarkerInState(const SerializedState &state, const SerializedMarker &marker) :
            m_state(state),
            m_marker(marker)
    { }

    const SerializedState &getState() const
    {
        return m_state;
    }

    const SerializedMarker &getMarker() const
    {
        return m_marker;
    }

private:
    SerializedState m_state;
    SerializedMarker m_marker;
};

template<class IndexType>
class SerializedTransitionMarkerPropagation {
public:
    using SerializedMarker = SerializedObject<IndexType>;
    using SerializedTransition = SerializedObject<IndexType>;

    SerializedTransitionMarkerPropagation(const SerializedTransition &serializedTransition,
                                          std::vector<SerializedMarkerInState<IndexType>> &&serializedMarkers) :
            m_transiton(serializedTransition),
            m_serializedMarkers(std::move(serializedMarkers)) { };

    const SerializedTransition &getSerializedTransition() const {
        return m_transiton;
    }

    const std::vector<SerializedMarkerInState<IndexType>> &getSerializedMarkers() const {
        return m_serializedMarkers;
    }

private:
    SerializedTransition m_transiton;
    std::vector<SerializedMarkerInState<IndexType>> m_serializedMarkers;
};

template<class _Transition>
class TransitionStateMarkerPropagator {
public:
    using PetriNetTraits = _PetriNetTraits;
    using Transition = _Transition;
    using IndexType = typename PetriNetTraits::IdType;
    using SerializedMarker = SerializedObject<IndexType>;
    using StateEnum = meta::TypeEnum<typename PetriNetTraits::StateList, IndexType>;
    using MarkerEnum = meta::TypeEnum<typename PetriNetTraits::MarkerList, IndexType>;

    TransitionStateMarkerPropagator(PetriNet<PetriNetTraits> &petriNet,
                                    TransitionWrapper<Transition, PetriNetTraits> &transition) :
            m_transition(transition),
            m_petriNet(petriNet) { }

    template<class State>
    bool operator()() {
        bool result = false;

        MarkerExtractor<Transition, State> markerExtractor;
        for (auto stateId : m_transition.template getInStateStorage<State>()) {
            const auto &state = m_petriNet.template getStateWrapperById<State>(stateId);
            if (boost::optional<std::pair<IdType, IdType>> marker = markerExtractor(m_petriNet, m_transition, state)) {
                m_serializedMarkers.emplace_back(
                        state.getId(),
                        StateEnum::template getValue<State>(),
                        marker.get().first,
                        marker.get().second);
            } else {
                result = true;
                break;
            }
        }
        return result;
    }

    std::vector<SerializedMarkerInState<IndexType>> &&moveOutSerializedMarkers() {
        return std::move(m_serializedMarkers);
    }

private:
    std::vector<SerializedMarkerInState<IndexType>> m_serializedMarkers;
    const PetriNet<PetriNetTraits> &m_petriNet;
    const TransitionWrapper<Transition, PetriNetTraits> &m_transition;
};

class TransitionMarkerPropagator {
public:
    using PetriNetTraits = _PetriNetTraits;
    using StateList = typename PetriNetTraits::StateList;
    using IndexType = typename PetriNetTraits::IdType;
    using TransitionEnum = meta::TypeEnum<typename PetriNetTraits::TransitionList, IndexType>;
    using SerializedTransition = SerializedObject<IndexType>;

    TransitionMarkerPropagator(PetriNet<PetriNetTraits> &petriNet) :
            m_petriNet(petriNet) {
    }

    template<class Transition>
    bool operator()() {
        for (auto transitionIterator = m_petriNet.template beginTransition<Transition>();
             transitionIterator != m_petriNet.template endTransition<Transition>();
             ++transitionIterator) {
            TransitionWrapper<Transition, PetriNetTraits> &transition = transitionIterator->second;
            TransitionStateMarkerPropagator<Transition> propagator(m_petriNet, transition);
            meta::ForEachLooper<StateList, decltype(propagator)> looper(propagator);
            if (!looper()) {
                m_serializedTransitionPropagation.emplace_back(
                        SerializedTransition(transition.getId(),
                                             TransitionEnum::template getValue<Transition>()),
                        propagator.moveOutSerializedMarkers());
            }
        }

        return false;
    }

    std::vector<SerializedTransitionMarkerPropagation<IndexType>> getOutSerializedTransitionMarkers() {
        return std::move(m_serializedTransitionPropagation);
    }

private:
    std::vector<SerializedTransitionMarkerPropagation<IndexType>> m_serializedTransitionPropagation;
    PetriNet<PetriNetTraits> &m_petriNet;
};

class MarkerPropagationExecutor {
public:
    using PetriNetTraits = _PetriNetTraits;
    using TransitionList = typename PetriNetTraits::TransitionList;
    using IndexType = typename PetriNetTraits::IdType;

    MarkerPropagationExecutor(PetriNet<PetriNetTraits>& m_petriNet) :
            m_petriNet(m_petriNet) {}

    void operator()() {
        TransitionMarkerPropagator propagator(m_petriNet);
        meta::ForEachLooper<TransitionList, decltype(propagator)> looper(propagator);
        looper();
        auto serializedTransitions = propagator.getOutSerializedTransitionMarkers();
        for (SerializedTransitionMarkerPropagation<IndexType> &serializedTransition : serializedTransitions) {
            std::cout << "Transition id: " << serializedTransition.getSerializedTransition().getObjectId()
            << " Transition type: " << serializedTransition.getSerializedTransition().getObjectSerializedType()
            << std::endl;
            for (auto serializedMarker : serializedTransition.getSerializedMarkers()) {
                std::cout << "State id: " << serializedMarker.getState().getObjectId()
                << " State type: " << serializedMarker.getState().getObjectSerializedType()
                << " Marker id: " << serializedMarker.getMarker().getObjectId()
                << " Marker type: " << serializedMarker.getMarker().getObjectSerializedType()
                << std::endl;
            }
            std::cout << std::endl;
        }
    }

private:
    PetriNet<PetriNetTraits> &m_petriNet;
};
};

#endif //PETRINET_H
