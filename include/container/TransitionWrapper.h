#ifndef TRANSITIONWRAPPER_H
#define TRANSITIONWRAPPER_H

#include "IdObjectWrapper.h"
#include "meta/TypeListStorage.h"

template <class _State, class _PetriNetTraits>
class StateWrapper;

template <class _Transition>
class TransitionObjectWrapper
{
public:
    using Transition = _Transition;

    TransitionObjectWrapper(Transition&& transition) : m_transition(std::forward<Transition>(transition))
    {
    }

    template <class... Arguments>
    TransitionObjectWrapper(Arguments&&... arguments) : m_transition(std::forward<Arguments>(arguments)...)
    {
    }

    Transition& getTransition()
    {
        return m_transition;
    }

private:
    Transition m_transition;
};

template <class _PetriNetTraits>
class TransitionCommonStorage
{
public:
    using PetriNetTraits = _PetriNetTraits;
    using StateList = typename PetriNetTraits::StateList;
    using IdType = typename PetriNetTraits::IdType;
    template <class T>
    using StorageTypeFactory = meta::VectorStorage<T, typename PetriNetTraits::template Allocator<T>>;
    using Storage = StorageTypeFactory<IdType>;

    template <class T>
    Storage& getInStateStorage()
    {
        return m_inStateStorage.template getStorage<T>();
    }

    template <class T>
    const Storage& getInStateStorage() const
    {
        return m_inStateStorage.template getStorage<T>();
    }

    template <class T>
    Storage& getOutStateStorage()
    {
        return m_outStateStorage.template getStorage<T>();
    }

    template <class T>
    const Storage& getOutStateStorage() const
    {
        return m_outStateStorage.template getStorage<T>();
    }

private:
    template <class V>
    using IdTypeFactory = meta::TypeHolder<IdType>;

    meta::TypeListStorage<IdTypeFactory, StorageTypeFactory, StateList> m_inStateStorage;
    meta::TypeListStorage<IdTypeFactory, StorageTypeFactory, StateList> m_outStateStorage;
};


template <class _Transition, class _PetriNetTraits>
class TransitionWrapper : public TransitionObjectWrapper<_Transition>,
                          public IdObjectWrapper<typename _PetriNetTraits::IdType>,
                          public TransitionCommonStorage<_PetriNetTraits>
{
public:
    using Transition = _Transition;
    using PetriNetTraits = _PetriNetTraits;
    using IdType = typename PetriNetTraits::IdType;

    TransitionWrapper(IdType id, Transition&& transition) :
            TransitionObjectWrapper<_Transition>(std::forward<Transition>(transition)),
            IdObjectWrapper<IdType>(id)
    {

    }

    template <class... Arguments>
    TransitionWrapper(IdType id, Arguments&&... arguments) :
            TransitionObjectWrapper<_Transition>(std::forward<Arguments>(arguments)...),
            IdObjectWrapper<IdType>(id)
    {
    }

    ~TransitionWrapper() {
    }
};

#endif //TRANSITIONWRAPPER_H
