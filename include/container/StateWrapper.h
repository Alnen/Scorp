#ifndef STATEWRAPPER_H
#define STATEWRAPPER_H

#include "container/internal/TypeListStorage.h"
#include "PetriNetTraits.h"
#include "IdObjectWrapper.h"

template <class _Transition, class _PetriNetTraits>
class TransitionWrapper;

template <class _State>
class StateObjectWrapper
{
public:
    using State = _State;

    StateObjectWrapper(State&& state) : m_state(std::forward<State>(state))
    {
    }

    template <class... Arguments>
    StateObjectWrapper(Arguments&&... arguments) : m_state(std::forward<Arguments>(arguments)...)
    {
    }

    State& getState()
    {
        return m_state;
    }

private:
    State m_state;
};

template <class _PetriNetTraits>
class StateCommonStorage
{
public:
    using PetriNetTraits = _PetriNetTraits;
    using IdType = typename PetriNetTraits::IdType;
    using MarkerList = typename PetriNetTraits::MarkerList;
    using TransitionList = typename PetriNetTraits::TransitionList;
    template <class T>
    using StorageTypeFactory = VectorStorage<T>;
    using Storage = StorageTypeFactory<IdType>;

    template <class T>
    Storage& getMarkerStorage()
    {
        return m_markerStorage.template getStorage<T>();
    }

    template <class T>
    Storage& getInTransitionStorage()
    {
        return m_inTransitionStorage.template getStorage<T>();
    }

    template <class T>
    Storage& getOutTransitionStorage()
    {
        return m_outTransitionStorage.template getStorage<T>();
    }

private:
    template <class V>
    using IdTypeFactory = return_same_type<IdType>;

    TypeListStorage<IdTypeFactory, StorageTypeFactory, MarkerList> m_markerStorage;
    TypeListStorage<IdTypeFactory, StorageTypeFactory, TransitionList> m_inTransitionStorage;
    TypeListStorage<IdTypeFactory, StorageTypeFactory, TransitionList> m_outTransitionStorage;
};

template <class _State, class _PetriNetTraits>
class StateWrapper : public StateObjectWrapper<_State>,
                     public IdObjectWrapper<typename _PetriNetTraits::IdType>,
                     public StateCommonStorage<_PetriNetTraits>
{
public:
    using State = _State;
    using PetriNetTraits = _PetriNetTraits;
    using IdType = typename PetriNetTraits::IdType;

    StateWrapper(IdType id, State&& state) :
        StateObjectWrapper<State>(std::forward<State>(state)),
        IdObjectWrapper<IdType>(id)
    {

    }

    template <class... Arguments>
    StateWrapper(IdType id, Arguments&&... arguments) :
        StateObjectWrapper<State>(std::forward<Arguments>(arguments)...),
        IdObjectWrapper<IdType>(id)
    {
    }

    ~StateWrapper() {
    }
};

#endif //STATEWRAPPER_H
