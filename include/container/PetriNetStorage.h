#ifndef PETRINETIMPL_H
#define PETRINETIMPL_H

#include "TypeStorage.h"
#include "MarkerWrapper.h"
#include "StateWrapper.h"
#include "TransitionWrapper.h"


#include <algorithm>
#include <memory>
#include <vector>
#include <utility>

template <class _PetriNetTraits>
class PetriNetStorage
{
public:
    using PetriNetTraits = _PetriNetTraits;
    using IdType = typename PetriNetTraits::IdType;
    using MarkerList = typename PetriNetTraits::MarkerList;
    using TransitionList = typename PetriNetTraits::TransitionList;
    using StateList = typename PetriNetTraits::StateList;

    template <class T>
    using Storage = typename MapStorageFactory<IdType, std::less<IdType>>::template MapStorage<T>;
    template <class T>
    using SpecializedMarkerWrapper = identity<MarkerWrapper<T, PetriNetTraits>>;
    template <class T>
    using SpecializedStateWrapper = identity<StateWrapper<T, PetriNetTraits>>;
    template <class T>
    using SpecializedTransitionWrapper = identity<TransitionWrapper<T, PetriNetTraits>>;

    template <class T>
    Storage<typename SpecializedMarkerWrapper<T>::type>& getMarkerStorage()
    {
        return m_markerStorage.template getStorage<T>();
    }

    template <class T>
    Storage<typename SpecializedStateWrapper<T>::type>& getStateStorage()
    {
        return m_stateStorage.template getStorage<T>();
    }

    template <class T>
    Storage<typename SpecializedTransitionWrapper<T>::type>& getTransitionStorage()
    {
        return m_transitionStorage.template getStorage<T>();
    }

private:
    TypeListStorage<SpecializedMarkerWrapper, Storage, MarkerList> m_markerStorage;
    TypeListStorage<SpecializedStateWrapper, Storage, StateList> m_stateStorage;
    TypeListStorage<SpecializedTransitionWrapper, Storage, TransitionList> m_transitionStorage;
};


#endif //PETRINETIMPL_H
