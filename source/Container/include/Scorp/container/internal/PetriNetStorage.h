#ifndef PETRINETSTORAGE_H
#define PETRINETSTORAGE_H

#include "TypeStorage.h"
#include "Scorp/container/MarkerWrapper.h"
#include "Scorp/container/StateWrapper.h"
#include "Scorp/container/TransitionWrapper.h"


#include <algorithm>
#include <memory>
#include <vector>
#include <utility>

namespace container {
namespace internal {

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
    using Storage = typename meta::MapStorageFactory<
            IdType,
            std::less<IdType>,
            typename PetriNetTraits::template Allocator<std::pair<IdType, T>>
    >::template MapStorage<T>;

    template <class T>
    using SpecializedMarkerWrapper = meta::TypeHolder<container::MarkerWrapper<T, PetriNetTraits>>;
    template <class T>
    using SpecializedStateWrapper = meta::TypeHolder<container::StateWrapper<T, PetriNetTraits>>;
    template <class T>
    using SpecializedTransitionWrapper = meta::TypeHolder<container::TransitionWrapper<T, PetriNetTraits>>;

    PetriNetStorage() = default;

    PetriNetStorage(const PetriNetStorage&) = delete;
    PetriNetStorage(PetriNetStorage&&) = default;

    PetriNetStorage& operator=(const PetriNetStorage&) = delete;
    PetriNetStorage& operator=(PetriNetStorage&&) = default;

    template <class T>
    Storage<typename SpecializedMarkerWrapper<T>::type>& getMarkerStorage()
    {
        return m_markerStorage.template getStorage<T>();
    }

    template <class T>
    const Storage<typename SpecializedMarkerWrapper<T>::type>& getMarkerStorage() const
    {
        return m_markerStorage.template getStorage<T>();
    }

    template <class T>
    Storage<typename SpecializedStateWrapper<T>::type>& getStateStorage()
    {
        return m_stateStorage.template getStorage<T>();
    }

    template <class T>
    const Storage<typename SpecializedStateWrapper<T>::type>& getStateStorage() const
    {
        return m_stateStorage.template getStorage<T>();
    }

    template <class T>
    Storage<typename SpecializedTransitionWrapper<T>::type>& getTransitionStorage()
    {
        return m_transitionStorage.template getStorage<T>();
    }

    template <class T>
    const Storage<typename SpecializedTransitionWrapper<T>::type>& getTransitionStorage() const
    {
        return m_transitionStorage.template getStorage<T>();
    }

private:
    meta::TypeListStorage<SpecializedMarkerWrapper, Storage, MarkerList> m_markerStorage;
    meta::TypeListStorage<SpecializedStateWrapper, Storage, StateList> m_stateStorage;
    meta::TypeListStorage<SpecializedTransitionWrapper, Storage, TransitionList> m_transitionStorage;
};

}
}

#endif //PETRINETSTORAGE_H
