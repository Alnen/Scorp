#ifndef SCORPCORE_RUNTIMETYPESWITCH_H
#define SCORPCORE_RUNTIMETYPESWITCH_H

#include <utility>
#include <stdexcept>
#include "TypeEnum.h"
#include "ForEachLooper.h"

namespace meta {

template<class Functor, class TypeList, class IndexType>
struct RuntimeTypeChooser {
    using TypeEnum = typename meta::TypeEnum<TypeList, IndexType>;

    template<class... Args>
    RuntimeTypeChooser(IndexType m_typeId, Args&&... args) :
            m_typeId(m_typeId),
            m_functor(std::forward<Args>(args)...) { }

    template<class Type>
    bool operator()() {
        if (TypeEnum::template getValue<Type>() == m_typeId) {
            m_functor.template operator()<Type>();
            return true;
        }
        return false;
    }

    IndexType m_typeId;
    Functor m_functor;
};

template<class Functor, class TypeList, class IndexType, class... Args>
Functor calculateBasedOnRealtime(IndexType typeId, Args&&... args) {
    RuntimeTypeChooser<Functor, TypeList, IndexType> functor(typeId, std::forward<Args>(args)...);
    meta::ForEachLooper<TypeList, decltype(functor)> looper(functor);
    if (!looper()) {
        throw std::runtime_error("Type not in TypeList");
    }

    return std::move(functor.m_functor);
}

}

#endif //SCORPCORE_RUNTIMETYPESWITCH_H