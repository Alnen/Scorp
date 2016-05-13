#ifndef SCORPCORE_FOREACHLOOP_H
#define SCORPCORE_FOREACHLOOP_H

#include <meta/TypeList.h>
#include <utility>

namespace meta
{

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

    Functor& m_functor;
};

}

#endif //SCORPCORE_FOREACHLOOP_H
