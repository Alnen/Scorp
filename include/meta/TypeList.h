#ifndef TYPELIST_H
#define TYPELIST_H

namespace meta {

struct ListEnd;

template<class... _TypeList>
struct TypeList;

template<class _Head, class... _Tail>
struct TypeList<_Head, _Tail...> {
    using Head = _Head;
    using Tail = TypeList<_Tail...>;
};

template<>
struct TypeList<> {
    using Head = ListEnd;
    using Tail = TypeList<>;
};

}

#endif //TYPELIST_H
