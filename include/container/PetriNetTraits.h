#ifndef PETRINETTRAITS_H
#define PETRINETTRAITS_H

#include "container/internal/IntegralIdGenerator.h"

template <class _MarkerList, class _TransitionList, class _StateList>
struct PetriNetTraits
{
    using MarkerList = _MarkerList;
    using TransitionList = _TransitionList;
    using StateList = _StateList;
    using IdType = int;
    using IdGenerator = IntegralIdGenerator<IdType>;
};

#endif //PETRINETTRAITS_H
