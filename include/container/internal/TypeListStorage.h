#ifndef SIMPLETYPELISTSTORAGE_H
#define SIMPLETYPELISTSTORAGE_H

#include <vector>
#include <boost/container/flat_map.hpp>
#include "TypeStorage.h"
#include "meta/TypeList.h"
#include "meta/TypeHolder.h"

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
    typename std::enable_if<std::is_same<T, Type>::value, Storage&>::type
    getStorage()
    {
        return m_storage;
    }

    template <class T>
    typename std::enable_if<!std::is_same<T, Type>::value, _Storage<typename _Functor<T>::type>&>::type
    getStorage()
    {
        return RestStorage::template getStorage<T>();
    }

private:
    Storage m_storage;
};

template <template <class T> class _Functor, template <class T>  class _Storage>
class TypeListStorage<_Functor, _Storage, meta::TypeList<>>
{};

template <class T>
using VectorStorage = std::vector<T, std::allocator<T>>;

template <class IdType, class Comparator>
struct MapStorageFactory
{
    template <class T>
    using MapStorage = boost::container::flat_map<IdType, T, Comparator>;
};

template <class _TypeList, template <class T> class _Container = VectorStorage>
using IdentityTypeListStorage = TypeListStorage<meta::TypeHolder, _Container, _TypeList>;

#endif //SIMPLETYPELISTSTORAGE_H
