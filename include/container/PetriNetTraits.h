#ifndef PETRINETTRAITS_H
#define PETRINETTRAITS_H

#include "container/internal/IntegralIdGenerator.h"
#include "container/internal/RandomMarkerPropagator.h"
#include "TransitionWrapper.h"
#include "PetriNet.h"
#include "allocator/StlAllocator.h"

namespace container
{

template <class _MarkerList, class _TransitionList, class _StateList>
struct PetriNetTraits
{
    using MarkerList = _MarkerList;
    using TransitionList = _TransitionList;
    using StateList = _StateList;
    using IdType = int;
    using IdGenerator = IntegralIdGenerator<IdType>;

    template <class Transition, class State>
    using MarkerExtractor = internal::MarkerExtractor<
            PetriNetTraits<_MarkerList, _TransitionList, _StateList>,
            Transition,
            State>;

    template <class Transition>
    using MarkerPropagationSolver = internal::MarkerPropagationSolver<
            PetriNetTraits<MarkerList, TransitionList, StateList>,
            Transition
    >;

    template <class Type>
    using Allocator = allocator::Allocator<Type>;
};

}

#endif //PETRINETTRAITS_H
