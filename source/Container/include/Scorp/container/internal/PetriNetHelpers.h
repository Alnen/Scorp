#ifndef PETRINETHELPERS_H
#define PETRINETHELPERS_H

#include <algorithm>

namespace container {
namespace internal {

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

}
}

#endif // PETRINETHELPERS_H
