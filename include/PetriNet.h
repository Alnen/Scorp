#ifndef PETRINET_H
#define PETRINET_H

#include "PetriNetStorage.h"

template <class Marker, class Storage, class IdType>
struct MarkerEraser
{
    MarkerEraser(Storage& storage, IdType markerId, IdType parentId):
            m_storage(storage),
            m_parentId(parentId),
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
                                         return state.first == m_parentId;
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
    IdType m_parentId;
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

    PetriNet() {

    }

    template <class Marker>
    IdType addMarker(IdType parentId, Marker&& marker) {
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
    Marker& getMarkerById(IdType id) {
        auto& markerStorage = m_petriNetStorage.template getMarkerStorage<Marker>();
        auto& markerWrapper = markerStorage[id];
        return markerWrapper.getMarker();
    }

    template <class Marker>
    bool removeMarker(IdType id) {
        auto& markerStorage = m_petriNetStorage.template getMarkerStorage<Marker>();
        auto iterator = markerStorage.find(id);
        if (iterator == markerStorage.end())
        {
            return false;
        }
        IdType stateId = iterator->second.getStateId();
        IdType markerId = iterator->second.getId();
        markerStorage.erase(iterator);
        //
        using Functor = MarkerEraser<Marker, decltype(m_petriNetStorage), IdType>;
        Functor functor(m_petriNetStorage, markerId, stateId);
        ForEachLooper<StateList, Functor> eraser(functor);
        if (!eraser())
        {
            throw std::runtime_error("Couldn't find father");
        }
        // Delete id from father marker
        return true;
    }

    template <class State>
    IdType addState(State&& state) {
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


    void addTransition(const MarkerList & marker) {

    }


    void removeState(const MarkerList & marker) {

    }


    void removeTransition(const MarkerList & marker) {

    }


    void beginMarker(const MarkerList & marker) {

    }


    void beginState(const MarkerList & marker) {

    }


    void beginTransition(const MarkerList & marker) {

    }


    void endMarker(const MarkerList & marker) {

    }


    void endState(const MarkerList & marker) {

    }


    void endTransition(const MarkerList & marker) {

    }


    void size() {

    }


    bool empty() {
        return true;
    }

private:
    PetriNetStorage<PetriNetTraits> m_petriNetStorage;
    IdGenerator m_idGenerator;
};

#endif //PETRINET_H
