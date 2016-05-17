#ifndef SIMPLETYPELISTSTORAGE_H
#define SIMPLETYPELISTSTORAGE_H

#include <vector>
#include <boost/container/flat_map.hpp>
#include "container/internal/TypeStorage.h"
#include "meta/TypeList.h"
#include "meta/TypeHolder.h"

namespace meta
{

template <template <class T> class _Functor, template <class T> class _Storage, class _TypeList>
class TypeListStorage;

template <template <class T> class _Functor, template <class T> class _Storage, class _TypeList>
class TypeListStorage : TypeListStorage<_Functor, _Storage, typename _TypeList::Tail>
{
public:
    using TypeList = _TypeList;
    using Type = typename TypeList::Head;
    using Storage = _Storage<typename _Functor<Type>::type>;
    using RestStorage = TypeListStorage<_Functor, _Storage, typename TypeList::Tail>;

    template <class T>
    typename std::enable_if<std::is_same<T, Type>::value, Storage>::type&
    getStorage()
    {
        return m_storage;
    }

    template <class T>
    const typename std::enable_if<std::is_same<T, Type>::value, Storage>::type&
    getStorage() const
    {
        return m_storage;
    }

    template <class T>
    typename std::enable_if<!std::is_same<T, Type>::value, _Storage<typename _Functor<T>::type>>::type&
    getStorage()
    {
        return RestStorage::template getStorage<T>();
    }

    template <class T>
    const typename std::enable_if<!std::is_same<T, Type>::value,  _Storage<typename _Functor<T>::type>>::type&
    getStorage() const
    {
        return RestStorage::template getStorage<T>();
    }

private:
    Storage m_storage;
};

template <template <class T> class _Functor, template <class T>  class _Storage>
class TypeListStorage<_Functor, _Storage, meta::TypeList<>>
{};

template <class T, class Allocator>
using VectorStorage = std::vector<T, Allocator>;

template <class IdType, class Comparator, class Allocator>
struct MapStorageFactory
{
    template <class T>
    using MapStorage = boost::container::flat_map<IdType, T, Comparator, Allocator>;
};

template <class _TypeList, template <class T> class _Container = VectorStorage>
using IdentityTypeListStorage = TypeListStorage<meta::TypeHolder, _Container, _TypeList>;

}
#endif //SIMPLETYPELISTSTORAGE_H
