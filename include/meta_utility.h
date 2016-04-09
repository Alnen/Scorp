#ifndef META_UTILITY_H
#define META_UTILITY_H

#include <utility>
#include "TypeList.h"

template <class _Type>
struct identity
{
    using type = _Type;
};

template <class _Type>
struct pointer_wrapper
{
    using type = _Type*;
};

template <class _TypeList, template <class T> class _Functor>
struct all :
        std::conditional<
            _Functor<typename _TypeList::Head>::value,
            std::true_type,
            all<typename _TypeList::Tail, _Functor>
        >::type
{
};

template <template <class... Items> class _TypeList, template <class T> class _Functor>
struct all<_TypeList<>, _Functor> : std::false_type
{
};

template <class _Type>
struct is_equeal_to_type
{
    template <class _CheckedType>
    using type = std::is_same<_CheckedType, _Type>;
};

template <class _Type>
struct return_same_type
{
    using type = _Type;
};

template <class T>
class TypeWriter;

template <class Head, class Tail>
struct IterateOnce
{
    template <class Functor>
    bool operator()(Functor& functor)
    {
        if (functor.template operator()<Head>())
        {
            return true;
        }
        return IterateOnce<typename Tail::Head, typename Tail::Tail>()(functor);
    }
};

template <>
struct IterateOnce<ListEnd, TypeList<>>
{
    template <class Functor>
    bool operator()(Functor& functor)
    {
        return false;
    }
};

template <class _TypeList, class _Functor>
struct ForEachLooper
{
    using TypeList = _TypeList;
    using Functor = _Functor;

    ForEachLooper() = default;
    ForEachLooper(Functor& functor):
            m_functor(functor)
    {
    }

    template <class... Args>
    ForEachLooper(Args&&... args) :
            m_functor(std::forward<Args>(args)...)
    {
    }

    bool operator()()
    {
        return IterateOnce<typename TypeList::Head, typename TypeList::Tail>()(m_functor);
    }

    Functor m_functor;
};

#endif //META_UTILITY_H
