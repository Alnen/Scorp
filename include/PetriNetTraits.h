#ifndef PETRINETTRAITS_H
#define PETRINETTRAITS_H

template <class _MarkerList, class _TransitionList, class _StateList>
struct PetriNetTraits
{
    using MarkerList = _MarkerList;
    using TransitionList = _TransitionList;
    using StateList = _StateList;
};

#endif //PETRINETTRAITS_H
