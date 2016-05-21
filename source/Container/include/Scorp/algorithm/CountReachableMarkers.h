#ifndef SCORPCORE_COUNTREACHABLEMARKERS_H
#define SCORPCORE_COUNTREACHABLEMARKERS_H

#include "Scorp/container/PetriNetIterator.h"

namespace algorithm
{

template <class PetriNetTraits>
size_t countReachableMarkers(container::iterator::PetriNetIterator<PetriNetTraits> begin,
                             container::iterator::PetriNetIterator<PetriNetTraits> end)
{
    using EventType = typename container::iterator::PetriNetIterator<PetriNetTraits>::EventType;
    size_t count = 0;
    for (auto& it = begin; it != end ; ++it)
    {
        if ((*it).first == EventType::MARKER)
        {
            ++count;
        }
    }
    return count;
}

}

#endif //SCORPCORE_COUNTREACHABLEMARKERS_H
