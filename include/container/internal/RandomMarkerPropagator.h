#ifndef RANDOMMARKERPROPAGATOR_H
#define RANDOMMARKERPROPAGATOR_H

#include <boost/optional.hpp>
#include <container/PetriNet.h>
#include <container/PetriNetTraits.h>
#include "container/TransitionWrapper.h"
#include <container/StateWrapper.h>
#include <utility>

template <class _PetriNetTraits>
class PetriNet;

template <class _PetriNetTraits, class State>
class MarkerExtractor1
{
public:
    using PetriNetTraits = _PetriNetTraits;
    using IndexType = typename PetriNetTraits::IdType;
    using MarkerEnum = meta::TypeEnum<typename PetriNetTraits::MarkerList, IndexType>;

    MarkerExtractor1() = default;
    MarkerExtractor1(const MarkerExtractor1<PetriNetTraits, State>&) = default;

    MarkerExtractor1(const StateWrapper<State, PetriNetTraits>& state):
         m_state(state)
    {
    }

    template <class Marker>
    bool operator()()
    {
        const auto& markerStorage = m_state.template getMarkerStorage<Marker>();
        if (!markerStorage.empty())
        {
            serializedMarker = std::make_pair(markerStorage.front(), MarkerEnum::template getValue<Marker>());
            return true;
        } else {
            return false;
        }
    }

    std::pair<IndexType, IndexType> serializedMarker;
    const StateWrapper<State, PetriNetTraits>& m_state;
};

template <class _PetriNetTraits, class _Transition, class _State>
class MarkerExtractor
{
public:
    using PetriNetTraits = _PetriNetTraits;
    using Transition = _Transition;
    using State = _State;
    using IdType = typename PetriNetTraits::IdType;
    using MarkerList = typename PetriNetTraits::MarkerList;

    boost::optional<std::pair<IdType, IdType>> operator()(
            const PetriNet<PetriNetTraits>& petriNet,
            const TransitionWrapper<Transition, PetriNetTraits>& transition,
            const StateWrapper<State, PetriNetTraits>& state) const
    {
        MarkerExtractor1<PetriNetTraits, State> extractor(state);
        meta::ForEachLooper<MarkerList, decltype(extractor)> looper(extractor);
        if (looper()) {
            return boost::optional<std::pair<IdType, IdType>>(extractor.serializedMarker);
        }
        return boost::optional<std::pair<IdType, IdType>>();
    }
};

#endif //SCORPCORE_RANDOMMARKERPROPAGATOR_H
