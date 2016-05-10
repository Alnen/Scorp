#ifndef SCORPCORE_ALL_H
#define SCORPCORE_ALL_H

#include <type_traits>

namespace meta
{


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

}

#endif //SCORPCORE_ALL_H
