#ifndef PETRINET_H
#define PETRINET_H

#include <meta/ForEachLooper.h>
#include <meta/TypeEnum.h>
#include <meta/TypeWriter.h>
#include <meta/RuntimeTypeSwitch.h>
#include <iostream>
#include "container/internal/PetriNetStorage.h"
#include "container/internal/PetriNetHelpers.h"
#include "container/PetriNetIterator.h"

namespace container
{

namespace iterator {
template <class PetriNetTraits>
class PetriNetIterator;
}

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

    PetriNet(const PetriNet&) = delete;
    PetriNet(PetriNet&&) = delete;

    void operator=(const PetriNet&) = delete;
    void operator=(PetriNet&&) = delete;

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

    template <class Marker, class... Args>
    IdType addMarker(IdType parentId, Args&&... args)
    {
        IdType markerId = m_idGenerator();
        auto& markerStorage = m_petriNetStorage.template getMarkerStorage<Marker>();
        markerStorage.emplace(
                std::make_pair(
                        markerId,
                        MarkerWrapperType<Marker>(
                                markerId,
                                parentId,
                                std::forward<Args>(args)...
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
        auto& stateWrapper = stateStorage.find(id)->second;
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
        auto& transitionWrapper = transitionStorage.find(id)->second;
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
    const TransitionWrapperType<Transition>& getTransitionWrapperById(IdType id) const
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

        // Delete id from father's marker storage
        IdType stateId = iterator->second.getStateId();
        IdType markerId = iterator->second.getId();

        markerStorage.erase(iterator);

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
        auto& stateWrapper = stateStorage.find(stateId)->second;

        auto& transitionStorage = m_petriNetStorage.template getTransitionStorage<Transition>();
        auto& transitionWrapper = transitionStorage.find(transitionId)->second;

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
    size_t executeMarkersPropagation(Args&&... args)
    {
        std::cout << "Hello" << std::endl;
        return m_propagator();
    };

    template <class IndexType>
    class SerializedObject {
    public:
        SerializedObject(IndexType objectId, IndexType objectTypeId) :
                m_objectId(objectId),
                m_objectSerializedType(objectTypeId) { }

        SerializedObject() = default;
        SerializedObject(const SerializedObject<IndexType> &) = default;

        IndexType getObjectId() const {
            return m_objectId;
        }

        IndexType getObjectSerializedType() const {
            return m_objectSerializedType;
        }

    private:
        IndexType m_objectId = IndexType();
        IndexType m_objectSerializedType = IndexType();
    };

    template <class Transition>
    iterator::PetriNetIterator<PetriNetTraits> getIteratorBeginFromTransition(IdType transitionId) const
    {
        return iterator::PetriNetIterator<PetriNetTraits>(
                *this,
                iterator::PetriNetIterator<PetriNetTraits>::Event(
                        iterator::PetriNetIterator<PetriNetTraits>::EventType::TRANSITION,
                        SerializedObject<IdType>(transitionId, meta::TypeEnum<TransitionList, IdType>::template getValue<Transition>())),
                false);
    }

    template <class State>
    typename iterator::PetriNetIterator<PetriNetTraits> getIteratorBeginFromState(IdType stateId) const
    {
        return typename iterator::PetriNetIterator<PetriNetTraits>(
                *this,
                typename iterator::PetriNetIterator<PetriNetTraits>::Event(
                        iterator::PetriNetIterator<PetriNetTraits>::EventType::STATE,
                        SerializedObject<IdType>(stateId, meta::TypeEnum<StateList, IdType>::template getValue<State>())),
                false);
    }

    typename iterator::PetriNetIterator<PetriNetTraits> getIteratorEnd() const
    {
        return iterator::PetriNetIterator<PetriNetTraits>(
                *this,
                typename iterator::PetriNetIterator<PetriNetTraits>::Event(
                        iterator::PetriNetIterator<PetriNetTraits>::EventType::STATE,
                        SerializedObject<IdType>(-1, -1)),
                true);
    }

private:
    class MarkerPropagationExecutor;

    class PerTransitionMarkerCollector;

    template <class _Transition>
    class PerStateMarkerCollector;

    PetriNetStorage<PetriNetTraits> m_petriNetStorage;
    MarkerPropagationExecutor m_propagator;
    IdGenerator m_idGenerator;


public:
    template<class IndexType>
    class SerializedMarkerInState
    {
    public:
        using SerializedMarker = SerializedObject<IndexType>;
        using SerializedState = SerializedObject<IndexType>;

        SerializedMarkerInState() :
                m_state(SerializedState()),
                m_marker(SerializedMarker())
        { }

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
private:
    template<class IndexType>
    class SerializedTransitionMarkerPropagation
    {
    public:
        using SerializedMarker = SerializedObject<IndexType>;
        using SerializedTransition = SerializedObject<IndexType>;

        SerializedTransitionMarkerPropagation(const SerializedTransition &serializedTransition,
                                              std::vector<SerializedMarkerInState<IndexType>> &&serializedMarkers) :
                m_transiton(serializedTransition),
                m_serializedMarkers(std::move(serializedMarkers)) { };

        const SerializedTransition &getSerializedTransition() const
        {
            return m_transiton;
        }

        const std::vector<SerializedMarkerInState<IndexType>>& getSerializedMarkers() const
        {
            return m_serializedMarkers;
        }

        std::vector<SerializedMarkerInState<IndexType>>& getSerializedMarkers()
        {
            return m_serializedMarkers;
        }

    private:
        SerializedTransition m_transiton;
        std::vector<SerializedMarkerInState<IndexType>> m_serializedMarkers;
    };

    template<class _Transition>
    class PerStateMarkerCollector {
    public:
        using PetriNetTraits = _PetriNetTraits;
        using Transition = _Transition;
        using IndexType = typename PetriNetTraits::IdType;
        using SerializedMarker = SerializedObject<IndexType>;
        using StateEnum = meta::TypeEnum<typename PetriNetTraits::StateList, IndexType>;
        using MarkerEnum = meta::TypeEnum<typename PetriNetTraits::MarkerList, IndexType>;

        PerStateMarkerCollector(PetriNet<PetriNetTraits> &petriNet,
                                        TransitionWrapper<Transition, PetriNetTraits> &transition) :
                m_transition(transition),
                m_petriNet(petriNet) { }

        template<class State>
        bool operator()() {
            bool result = false;

            MarkerExtractor<Transition, State> markerExtractor;
            for (auto stateId : m_transition.template getInStateStorage<State>())
            {
                const auto &state = m_petriNet.template getStateWrapperById<State>(stateId);
                if (boost::optional<std::pair<IdType, IdType>> marker = markerExtractor(m_petriNet, m_transition, state))
                {
                    m_serializedMarkers.emplace_back(
                            state.getId(),
                            StateEnum::template getValue<State>(),
                            marker.get().first,
                            marker.get().second);
                }
                else
                {
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

    class PerTransitionMarkerCollector {
    public:
        using PetriNetTraits = _PetriNetTraits;
        using StateList = typename PetriNetTraits::StateList;
        using IndexType = typename PetriNetTraits::IdType;
        using TransitionEnum = meta::TypeEnum<typename PetriNetTraits::TransitionList, IndexType>;
        using SerializedTransition = SerializedObject<IndexType>;

        PerTransitionMarkerCollector(PetriNet<PetriNetTraits> &petriNet) :
                m_petriNet(petriNet) {
        }

        template<class Transition>
        bool operator()() {
            for (auto transitionIterator = m_petriNet.template beginTransition<Transition>();
                 transitionIterator != m_petriNet.template endTransition<Transition>();
                 ++transitionIterator) {
                TransitionWrapper<Transition, PetriNetTraits> &transition = transitionIterator->second;
                PerStateMarkerCollector<Transition> propagator(m_petriNet, transition);
                meta::ForEachLooper<StateList, decltype(propagator)> looper(propagator);
                if (!looper()) {
                    m_serializedTransitionPropagation.emplace_back(
                            SerializedTransition(
                                    transition.getId(),
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

    // Marker Decision
public:
    template <class Resource, class Deleter>
    class ResourceHolder
    {
    public:
        ResourceHolder(const Resource& resource):
                m_resource(resource)
        {
        }

        ResourceHolder(const Resource& resource, Deleter& deleter):
                m_resource(resource),
                m_deleter(deleter)
        {
        }

        ResourceHolder(const ResourceHolder<Resource, Deleter>&) = delete;
        void operator=(const ResourceHolder<Resource, Deleter>&) = delete;

        ResourceHolder(ResourceHolder<Resource, Deleter>&& other) :
                m_deleter(std::move(other.m_deleter))
        {
            if (other.m_needClanup)
            {
                m_resource = other.m_resource;
                m_needClanup = true;
                other.m_needClanup = false;
            } else {
                m_needClanup = false;
            }
        }

        void operator=(ResourceHolder<Resource, Deleter>&& other)
        {
            if (m_needClanup)
            {
                m_deleter(m_resource);
            }
            if (other.m_needClanup)
            {
                m_resource = other.m_resource;
                m_needClanup = true;
                other.m_needClanup = false;
            } else {
                m_needClanup = false;
            }
            m_deleter = std::move(other.m_deleter);
        }

        Resource& getResource()
        {
            return m_resource;
        }

        const Resource& getResource() const
        {
            return m_resource;
        }

        Resource release()
        {
            m_needClanup = false;
            return m_resource;
        }

        ~ResourceHolder()
        {
            if (m_needClanup)
            {
                m_deleter(m_resource);
            }
        }

    private:
        bool m_needClanup = true;
        Resource m_resource = Resource();
        Deleter m_deleter;
    };
private:
    struct MarkerDeleter
    {
        MarkerDeleter(IdType markerId, PetriNet<PetriNetTraits>& petriNet):
                m_markerId(markerId),
                m_petriNet(petriNet)
        {
        }

        template <class Marker>
        void operator()()
        {
            std::cout << "[MarkerDeleter]Called to delete marker " << m_markerId << std::endl;
            std::cout << "[MarkerDeleter] befor " << m_petriNet.sizeMarker<Marker>() << std::endl;
            m_petriNet.removeMarker<Marker>(m_markerId);
            std::cout << "[MarkerDeleter] after " << m_petriNet.sizeMarker<Marker>() << std::endl;
        }

        IdType m_markerId;
        PetriNet<PetriNetTraits>& m_petriNet;
    };

    template <class State>
    class MarkerStorageFinder
    {
    public:
        using IndexType = IdType;

        MarkerStorageFinder(StateWrapper<State, PetriNetTraits>& state):
                m_state(state)
        {
        }

        template <class Marker>
        void operator()()
        {
            m_storage = &m_state.template getMarkerStorage<Marker>();
        }

        std::vector<IndexType, typename PetriNetTraits::template Allocator<IndexType>>& getStatesMarkerStorage() const
        {
            if (m_storage == nullptr)
            {
                throw std::runtime_error("Couldn't find state's marker storage");
            }
            return *m_storage;
        }

    private:
        std::vector<IndexType, typename PetriNetTraits::template Allocator<IndexType>>* m_storage = nullptr;
        StateWrapper<State, PetriNetTraits>& m_state;
    };

    class StatesMarkerStorageFinder
    {
    public:
        using IndexType = IdType;
        template <class _State>
        using MarkerStorageFinder = typename PetriNet<PetriNetTraits>::template MarkerStorageFinder<_State>;

        StatesMarkerStorageFinder(PetriNet<PetriNetTraits>& petriNet,  IndexType stateId, IndexType markerTypeId):
            m_petriNet(petriNet),
            m_stateId(stateId),
            m_markerTypeId(markerTypeId)
        {
        }

        template <class State>
        void operator()()
        {
            auto& stateStorage = m_petriNet.m_petriNetStorage.template getStateStorage<State>();
            auto& stateWrapper = stateStorage.find(m_stateId)->second;

            MarkerStorageFinder<State> markerStorageFinder = std::move(meta::calculateBasedOnRealtime<MarkerStorageFinder<State>, MarkerList>(m_markerTypeId, stateWrapper));
            m_vector = &markerStorageFinder.getStatesMarkerStorage();
        }

        std::vector<IndexType, typename PetriNetTraits::template Allocator<IndexType>>& getStatesMarkerStorage() const
        {
            if (m_vector == nullptr)
            {
                throw std::runtime_error("Couldn't find state's marker storage");
            }
            return *m_vector;
        }

    private:
        std::vector<IndexType, typename PetriNetTraits::template Allocator<IndexType>>* m_vector = nullptr;
        PetriNet<PetriNetTraits>& m_petriNet;
        IndexType m_stateId;
        IndexType m_markerTypeId;
    };

    class MarkerParentChanger
    {
    public:
        MarkerParentChanger(PetriNet<PetriNetTraits>& petriNet, IdType markerId, IdType parentId):
            m_petriNet(petriNet),
            m_markerId(markerId),
            m_parentId(parentId)
        {
        }

        template <class Marker>
        void operator()()
        {
            auto& markerStorage = m_petriNet.m_petriNetStorage.template getMarkerStorage<Marker>();
            auto& markerWrapper = markerStorage.find(m_markerId)->second;

            markerWrapper.setId(m_parentId);
        }

    private:
        PetriNet<PetriNetTraits>& m_petriNet;
        IdType m_markerId;
        IdType m_parentId;
    };
public:
    class MarkerFiller
    {
    public:
        using IndexType = typename PetriNetTraits::IdType;
        using SerializedState = SerializedObject<IndexType>;
        using StatesMarkerStorageFinder = typename PetriNet<PetriNetTraits>::StatesMarkerStorageFinder;

        MarkerFiller(PetriNet<PetriNetTraits>& petriNet, IndexType stateId, IndexType stateTypeId):
                m_petriNet(petriNet),
                m_state(stateId, stateTypeId)
        {
        }

        MarkerFiller() = delete;
        MarkerFiller(const MarkerFiller&) = delete;
        MarkerFiller(MarkerFiller&&) = default;
        MarkerFiller& operator=(const MarkerFiller&) = delete;
        MarkerFiller& operator=(MarkerFiller&&) = default;

    private:
        std::vector<IndexType, typename PetriNetTraits::template Allocator<IndexType>>& getMarkerStorage(const SerializedState& state, IndexType markerTypeId)
        {
            StatesMarkerStorageFinder markerStorageFinder = std::move(meta::calculateBasedOnRealtime<StatesMarkerStorageFinder, StateList>(
                    state.getObjectSerializedType(),
                    m_petriNet,
                    state.getObjectId(),
                    markerTypeId));
            return markerStorageFinder.getStatesMarkerStorage();
        }

    public:
        template <class Deleter>
        IndexType moveState(ResourceHolder<SerializedMarkerInState<IndexType>, Deleter>&& serializedMarkerHandler)
        {
            // save marker from deletion
            SerializedMarkerInState<IndexType> serializedMarker = serializedMarkerHandler.release();

            // remove from old parent
            auto& markerStorage = getMarkerStorage(
                    serializedMarker.getState(),
                    serializedMarker.getMarker().getObjectSerializedType());
            std::remove(markerStorage.begin(), markerStorage.end(), serializedMarker.getMarker().getObjectId());
            // add to new parent
            markerStorage = getMarkerStorage(
                    m_state,
                    serializedMarker.getMarker().getObjectSerializedType());
            markerStorage.emplace_back(serializedMarker.getMarker().getObjectId());
            // Correct marker's parent id
            meta::calculateBasedOnRealtime<MarkerParentChanger, MarkerList>(
                    serializedMarker.getState().getObjectSerializedType(),
                    m_petriNet,
                    serializedMarker.getMarker().getObjectSerializedType(),
                    m_state.getObjectId());

            m_filled = true;
            return serializedMarker.getMarker().getObjectId();
        }

        template <class Marker, class... Args>
        IndexType createState(Args&&... args)
        {
            m_filled = true;
            return m_petriNet.template addMarker<Marker>(m_state.getObjectId(), std::forward<Args>(args)...);
        }

        bool isFilled() const
        {
            return m_filled;
        }
		
		const SerializedState& getState()
        {
            return m_state;
        }

    private:
        bool m_filled = false;
        SerializedState m_state;
        PetriNet<PetriNetTraits>& m_petriNet;
    };

private:

    template <class Transition>
    struct StateCollector
    {
        using SerializedState = SerializedObject<IdType>;
        using StateEnum = typename meta::TypeEnum<StateList, IdType>;

        StateCollector(const TransitionWrapper<Transition, PetriNetTraits>& transition, PetriNet<PetriNetTraits>& petriNet):
            m_transition(transition),
            m_petriNet(petriNet)
        {
        }

        template <class State>
        bool operator()()
        {
            for (IdType stateId : m_transition.template getOutStateStorage<State>())
            {
                m_serializedStates.emplace_back(m_petriNet, stateId, StateEnum::template getValue<State>());
            }
            return false;
        }

        std::vector<MarkerFiller> m_serializedStates;
        PetriNet<PetriNetTraits>& m_petriNet;
        const TransitionWrapper<Transition, PetriNetTraits>& m_transition;
    };

    struct MarkerPropagationDisicion
    {
        using IndexType = typename PetriNetTraits::IdType;
        using MarkerList = typename PetriNetTraits::MarkerList;
        using MakrerEnum = typename meta::TypeEnum<MarkerList, IndexType>;
        template <class Transition>
        using DecisionMaker = typename PetriNetTraits::template MarkerPropagationSolver<Transition>;

        template <class Transition>
        void operator()(PetriNet<PetriNetTraits>& petriNet,
                        std::vector<SerializedMarkerInState<IndexType>>& inputMarkers,
                        const TransitionWrapper<Transition, PetriNetTraits>& transition) const
        {
            auto deleter = [&petriNet](const SerializedMarkerInState<IndexType>& serializedMarkerInState) mutable {
                std::cout << "Called to delete marker " << serializedMarkerInState.getMarker().getObjectId()
                        << " of type " << serializedMarkerInState.getMarker().getObjectSerializedType() << std::endl;
                meta::calculateBasedOnRealtime<MarkerDeleter, MarkerList>(
                        serializedMarkerInState.getMarker().getObjectSerializedType(),
                        serializedMarkerInState.getMarker().getObjectId(),
                        petriNet);
            };
            std::vector<ResourceHolder<PetriNet<PetriNetTraits>::SerializedMarkerInState<IndexType>, decltype(deleter)>> handledMarkers;
            for(const auto& serializedMarker : inputMarkers)
            {
                handledMarkers.emplace_back(serializedMarker, deleter);
            }
            std::cout << "handledMarkers size " << handledMarkers.size() << std::endl;

            StateCollector<Transition> stateCollector(transition, petriNet);
            meta::ForEachLooper<StateList, decltype(stateCollector)> looper(stateCollector);
            looper();

            DecisionMaker<Transition> decisionMaker;

            std::vector<std::reference_wrapper<
                    ResourceHolder<
                            PetriNet<PetriNetTraits>::SerializedMarkerInState<IndexType>,
                            decltype(deleter)
                    >
            >> handledMarkersRefs(handledMarkers.begin(), handledMarkers.end());
            std::vector<std::reference_wrapper<MarkerFiller>> outputMarkersRefs;
            for (auto& out : stateCollector.m_serializedStates)
            {
                outputMarkersRefs.emplace_back(out);
            }
            decisionMaker(petriNet, transition, handledMarkersRefs, outputMarkersRefs);

            for (auto& out : outputMarkersRefs)
            {
                if (!out.get().isFilled())
                {
                    throw std::runtime_error("something not filled");
                }
            }
        }
    };

    // Marker Propagation executor

    class SerializedMarkerProgationExecutor
    {
    public:
        SerializedMarkerProgationExecutor(
                PetriNet<PetriNetTraits> &petriNet,
                IdType  transitionId,
                std::vector<SerializedMarkerInState<IdType>>& m_serializedTransitionPropagation):
            m_petriNet(petriNet),
            m_transitionId(transitionId),
            m_serializedMarkers(m_serializedTransitionPropagation)
        {
        }

        template <class Transition>
        void operator()()
        {
            auto& transition =  m_petriNet.getTransitionWrapperById<Transition>(m_transitionId);
            MarkerPropagationDisicion desicionMaker;
            desicionMaker.template operator()<Transition>(
                    m_petriNet,
                    m_serializedMarkers,
                    transition);
        }

    private:
        PetriNet<PetriNetTraits> &m_petriNet;
        IdType  m_transitionId;
        std::vector<SerializedMarkerInState<IdType>>& m_serializedMarkers;
    };

    // Marker Propagation

    class MarkerPropagationExecutor {
    public:
        using PetriNetTraits = _PetriNetTraits;
        using TransitionList = typename PetriNetTraits::TransitionList;
        using IndexType = typename PetriNetTraits::IdType;

        MarkerPropagationExecutor(PetriNet<PetriNetTraits>& m_petriNet) :
                m_petriNet(m_petriNet) {}

        size_t operator()() const {
            using Train = typename decltype(meta::TypeEnum<MarkerList, IdType>::template getTypeHolder<0>())::type;
            using AccessToken = typename decltype(meta::TypeEnum<MarkerList, IdType>::template getTypeHolder<1>())::type;
            std::cout << "Trains count " << m_petriNet.sizeMarker<Train>() << std::endl;
            std::cout << "AccessToken count " << m_petriNet.sizeMarker<AccessToken>() << std::endl;
            PerTransitionMarkerCollector propagator(m_petriNet);
            meta::ForEachLooper<TransitionList, decltype(propagator)> looper(propagator);
            looper();
            auto serializedTransitions = propagator.getOutSerializedTransitionMarkers();
            for (SerializedTransitionMarkerPropagation<IndexType> &serializedTransition : serializedTransitions) {
                std::cout << "Transition id: " << serializedTransition.getSerializedTransition().getObjectId()
                    << " Transition type: " << serializedTransition.getSerializedTransition().getObjectSerializedType()
                    << std::endl;
                for (auto& serializedMarker : serializedTransition.getSerializedMarkers()) {
                    std::cout << "State id: " << serializedMarker.getState().getObjectId()
                        << " State type: " << serializedMarker.getState().getObjectSerializedType()
                        << " Marker id: " << serializedMarker.getMarker().getObjectId()
                        << " Marker type: " << serializedMarker.getMarker().getObjectSerializedType()
                        << std::endl;
                }
                std::cout << std::endl;
                meta::calculateBasedOnRealtime<SerializedMarkerProgationExecutor, TransitionList>(
                        serializedTransition.getSerializedTransition().getObjectSerializedType(),
                        m_petriNet,
                        serializedTransition.getSerializedTransition().getObjectId(),
                        serializedTransition.getSerializedMarkers());
            }
            std::cout << "Trains count " << m_petriNet.sizeMarker<Train>() << std::endl;
            std::cout << "AccessToken count " << m_petriNet.sizeMarker<AccessToken>() << std::endl;
            return serializedTransitions.size();
        }

private:
    PetriNet<PetriNetTraits> &m_petriNet;
};
};

}

#endif //PETRINET_H
