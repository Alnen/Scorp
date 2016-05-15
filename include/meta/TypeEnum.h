#ifndef SCORPCORE_TYPEENUM_H
#define SCORPCORE_TYPEENUM_H

#include "meta/TypeList.h"
#include "meta/TypeHolder.h"

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

};

template <class _Head, class _Tail, int TypeIndex>
struct TypeHandlerExtractor
{
    static_assert(TypeIndex > 0 ,"");

    using Head = _Head;
    using Tail = _Tail;

    constexpr decltype(TypeHandlerExtractor<typename Tail::Head, typename Tail::Tail, TypeIndex - 1>()())
    operator ()() const
    {
        return TypeHandlerExtractor<typename Tail::Head, typename Tail::Tail, TypeIndex - 1>()();
    }
};

template <class _Head, class _Tail>
struct TypeHandlerExtractor<_Head, _Tail, 0>
{
    using Head = _Head;
    using Tail = _Tail;

    meta::TypeHolder<Head>
    operator ()()
    {
        return meta::TypeHolder<Head>();
    }
};

template <class TypeList, class IndexType>
struct TypeExtractor
{



};

}

namespace meta
{

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
    constexpr static decltype(TypeHandlerExtractor<typename TypeList::Head, typename TypeList::Tail, TypeIndex>()())
    getTypeHolder()
    {
        return TypeHandlerExtractor<typename TypeList::Head, typename TypeList::Tail, TypeIndex>()();
    }
};

template <class _TypeList>
using IntTypeEnum = TypeEnum<_TypeList, int>;

}

#endif //SCORPCORE_TYPEENUM_H
