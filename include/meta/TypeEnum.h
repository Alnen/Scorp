#ifndef SCORPCORE_TYPEENUM_H
#define SCORPCORE_TYPEENUM_H

#include "meta/TypeList.h"
namespace {

template <class _Type>
struct TypeHandler
{
    using Type = _Type;
};


template <class _Head, class _Tail, class _IntegralType>
struct TypeEnumImpl
{
    using IntegralType = _IntegralType;
    using Head = _Head;
    using Tail = _Tail;

    template <class ListMember>
    constexpr static typename std::enable_if<std::is_same<ListMember, Head>::value, IntegralType>::type
    getValue()
    {
        return 0;
    }

    template <class ListMember>
    constexpr static typename std::enable_if<!std::is_same<ListMember, Head>::value, IntegralType>::type
    getValue()
    {
        return TypeEnumImpl<typename Tail::Head, typename Tail::Tail, IntegralType>::template getValue<ListMember>() + 1;
    }

    template <IntegralType TypeIndex>
    constexpr static typename std::enable_if<TypeIndex == 0, TypeHandler<Head>>::type
    getTypeHandler()
    {
        return TypeHandler<Head>();
    }

    template <IntegralType TypeIndex>
    constexpr static typename std::enable_if<(TypeIndex > 0),
            decltype(TypeEnumImpl<typename Tail::Head, typename Tail::Tail, IntegralType>::template getTypeHandler<TypeIndex - 1>())>::type
    getTypeHandler()
    {
        return TypeEnumImpl<typename Tail::Head, typename Tail::Tail, IntegralType>::template getTypeHandler<TypeIndex - 1>();
    }
};

}

template <class _TypeList, class _IntegralType>
struct TypeEnum
{
    static_assert(std::is_integral<_IntegralType>::value, "");

    using IntegralType = _IntegralType;
    using TypeList = _TypeList;

    template <class ListMember>
    constexpr static IntegralType getValue()
    {
        return TypeEnumImpl<typename TypeList::Head, typename TypeList::Tail, IntegralType>::template getValue<ListMember>();
    }

    template <IntegralType TypeIndex>
    constexpr static decltype(TypeEnumImpl<typename TypeList::Head, typename TypeList::Tail, IntegralType>::template getTypeHandler<TypeIndex>())
    getTypeHandler()
    {
        return TypeEnumImpl<typename TypeList::Head, typename TypeList::Tail, IntegralType>::template getTypeHandler<TypeIndex>();
    }
};

template <class _TypeList>
using IntTypeEnum = TypeEnum<_TypeList, int>;

#endif //SCORPCORE_TYPEENUM_H
