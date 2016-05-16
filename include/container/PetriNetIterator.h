#ifndef SCORPCORE_PETRINETITERATOR_H
#define SCORPCORE_PETRINETITERATOR_H

#include <set>
#include <deque>
#include <vector>
#include <meta/RuntimeTypeSwitch.h>

namespace container
{
template <class _PetriNetTraits>
class PetriNet;

namespace iterator
{
template <class _PetriNetTraits>
class PetriNetIterator;

template <class IdType>
struct MarkerExtractor
{
    template <class Marker, class Wrapper>
    const std::vector<IdType>& operator()(const Wrapper& wrapper) const
    {
        return wrapper.template getMarkerStorage<Marker>();
    }
};

template <class IdType>
struct InStateExtractor
{
    template <class Marker, class Wrapper>
    const std::vector<IdType>& operator()(const Wrapper& wrapper) const
    {
        return wrapper.template getInStateStorage<Marker>();
    }
};

template <class IdType>
struct OutStateExtractor
{
    template <class Marker, class Wrapper>
    const std::vector<IdType>& operator()(const Wrapper& wrapper) const
    {
        return wrapper.template getOutStateStorage<Marker>();
    }
};

template <class IdType>
struct InTransitionExtractor
{
    template <class Marker, class Wrapper>
    const std::vector<IdType>& operator()(const Wrapper& wrapper) const
    {
        return wrapper.template getInTransitionStorage<Marker>();
    }
};

template <class IdType>
struct OutTransitionExtractor
{
    template <class Marker, class Wrapper>
    const std::vector<IdType>& operator()(const Wrapper& wrapper) const
    {
        return wrapper.template getOutTransitionStorage<Marker>();
    }
};

template <
        class _PetriNetTraits,
        typename PetriNetIterator<_PetriNetTraits>::EventType EVENT_TYPE,
        template <class> class VectorExtractor,
        class Wrapper,
        class TypeList>
class VectorSerializer
{
public:
    using PetriNetTraits = _PetriNetTraits;
    using IdType = typename PetriNetTraits::IdType;
    template <class _TypeList>
    using TypeEnum = typename meta::template TypeEnum<_TypeList, IdType>;

    using Event = typename PetriNetIterator<PetriNetTraits>::Event;
    using SerializedObject = typename container::PetriNet<PetriNetTraits>::template SerializedObject<IdType>;

    VectorSerializer(
            std::deque<Event>& elementToVisit,
            const container::PetriNet<PetriNetTraits>& petriNet,
            const Wrapper& wrapper):
        m_elementToVisit(elementToVisit),
        m_petriNet(petriNet),
        m_wrapper(wrapper)
    {
    };

    template <class Type>
    bool operator()()
    {
        VectorExtractor<IdType> extractor;
        for (IdType objectId : extractor.template operator()<Type>(m_wrapper)) {
            m_elementToVisit.emplace_back(EVENT_TYPE, SerializedObject(objectId, TypeEnum<TypeList>::template getValue<Type>()));
        }
        return false;
    }

private:
    std::deque<Event>& m_elementToVisit;
    const PetriNet<PetriNetTraits>& m_petriNet;
    const Wrapper& m_wrapper;
};

template <class _PetriNetTraits>
class StateSerializer
{
public:
    using PetriNetTraits = _PetriNetTraits;
    using IdType = typename PetriNetTraits::IdType;
    using StateList = typename PetriNetTraits::StateList;
    using TransitionList = typename PetriNetTraits::TransitionList;
    using MarkerList = typename PetriNetTraits::MarkerList;
    template <class TypeList>
    using TypeEnum = typename meta::template TypeEnum<TypeList, IdType>;

    using Event = typename PetriNetIterator<PetriNetTraits>::Event;
    using EventType = typename PetriNetIterator<PetriNetTraits>::EventType;
    using SerializedObject = typename PetriNet<PetriNetTraits>::template SerializedObject<IdType>;

    StateSerializer(
            IdType stateId,
            std::deque<Event>& elementToVisit,
            const container::PetriNet<PetriNetTraits>& petriNet):
        m_stateId(stateId),
        m_elementToVisit(elementToVisit),
        m_petriNet(petriNet)
    {
    }

    template <class State>
    void operator()()
    {
        auto& stateWrapper = m_petriNet.template getStateWrapperById<State>(m_stateId);
        // Marker serialization
        m_elementToVisit.emplace_back(
                EventType::MARKER_STORAGE_BEGIN,
                SerializedObject(m_stateId, TypeEnum<StateList>::template getValue<State>()));
        meta::makeForEachLooper<
                VectorSerializer<PetriNetTraits, EventType::MARKER, MarkerExtractor, decltype(stateWrapper), MarkerList>,
                MarkerList
        >(
                m_elementToVisit,
                m_petriNet,
                stateWrapper
        );
        // impl
        m_elementToVisit.emplace_back(
                EventType::MARKER_STORAGE_END,
                SerializedObject(m_stateId, TypeEnum<StateList>::template getValue<State>()));
        // Transition serialization
        m_elementToVisit.emplace_back(
                EventType::IN_TRANSITION_STORAGE_BEGIN,
                SerializedObject(m_stateId, TypeEnum<StateList>::template getValue<State>()));
        meta::makeForEachLooper<
                VectorSerializer<PetriNetTraits, EventType::TRANSITION, InTransitionExtractor, decltype(stateWrapper), TransitionList >,
                TransitionList
        >(
                m_elementToVisit,
                m_petriNet,
                stateWrapper
        );
        m_elementToVisit.emplace_back(
                EventType::IN_TRANSITION_STORAGE_END,
                SerializedObject(m_stateId, TypeEnum<StateList>::template getValue<State>()));
        // State
        m_elementToVisit.emplace_back(
                EventType::OUT_TRANSITION_STORAGE_BEGIN,
                SerializedObject(m_stateId, TypeEnum<StateList>::template getValue<State>()));
        meta::makeForEachLooper<
                VectorSerializer<PetriNetTraits, EventType::TRANSITION, OutTransitionExtractor, decltype(stateWrapper), TransitionList >,
                TransitionList
        >(
                m_elementToVisit,
                m_petriNet,
                stateWrapper
        );
        m_elementToVisit.emplace_back(
                EventType::OUT_TRANSITION_STORAGE_END,
                SerializedObject(m_stateId, TypeEnum<StateList>::template getValue<State>()));
        //
    }

private:
    IdType m_stateId;
    std::deque<Event>& m_elementToVisit;
    const PetriNet<PetriNetTraits>& m_petriNet;
};

template <class _PetriNetTraits>
class TransitionSerializer
{
public:
    using PetriNetTraits = _PetriNetTraits;
    using IdType = typename PetriNetTraits::IdType;
    using StateList = typename PetriNetTraits::StateList;
    using TransitionList = typename PetriNetTraits::TransitionList;
    using MarkerList = typename PetriNetTraits::MarkerList;
    template <class TypeList>
    using TypeEnum = typename meta::template TypeEnum<TypeList, IdType>;

    using Event = typename PetriNetIterator<PetriNetTraits>::Event;
    using EventType = typename PetriNetIterator<PetriNetTraits>::EventType;
    using SerializedObject = typename PetriNet<PetriNetTraits>::template SerializedObject<IdType>;

    TransitionSerializer(
            IdType transitionId,
            std::deque<Event>& elementToVisit,
            const container::PetriNet<PetriNetTraits>& petriNet):
            m_transitionId(transitionId),
            m_elementToVisit(elementToVisit),
            m_petriNet(petriNet)
    {
    }

    template <class Transition>
    void operator()()
    {
        auto& transitionWrapper = m_petriNet.template getTransitionWrapperById<Transition>(m_transitionId);

        // Transition serialization
        m_elementToVisit.emplace_back(
                EventType::IN_STATE_STORAGE_BEGIN,
                SerializedObject(m_transitionId, TypeEnum<TransitionList>::template getValue<Transition>()));
        meta::makeForEachLooper<
                VectorSerializer<PetriNetTraits, EventType::STATE, InStateExtractor, decltype(transitionWrapper), StateList >,
                StateList
        >(
                m_elementToVisit,
                m_petriNet,
                transitionWrapper
        );
        m_elementToVisit.emplace_back(
                EventType::IN_STATE_STORAGE_END,
                SerializedObject(m_transitionId, TypeEnum<TransitionList>::template getValue<Transition>()));
        // State
        m_elementToVisit.emplace_back(
                EventType::OUT_STATE_STORAGE_BEGIN,
                SerializedObject(m_transitionId, TypeEnum<TransitionList>::template getValue<Transition>()));
        meta::makeForEachLooper<
                VectorSerializer<PetriNetTraits, EventType::STATE, OutStateExtractor, decltype(transitionWrapper), StateList >,
                StateList
        >(
                m_elementToVisit,
                m_petriNet,
                transitionWrapper
        );
        m_elementToVisit.emplace_back(
                EventType::OUT_STATE_STORAGE_END,
                SerializedObject(m_transitionId, TypeEnum<TransitionList>::template getValue<Transition>()));
        //
    }

private:
    IdType m_transitionId;
    std::deque<Event>& m_elementToVisit;
    const PetriNet<PetriNetTraits>& m_petriNet;
};

template <class _PetriNetTraits>
class PetriNetIterator
{
public:
    using StateList = typename _PetriNetTraits::StateList;
    using TransitionList = typename _PetriNetTraits::TransitionList;

    enum class EventType
    {
        MARKER,
        STATE,
        TRANSITION,
        STATE_DESCRIPTION_BEGIN,
        STATE_DESCRIPTION_END,
        TRANSITION_DESCRIPTION_BEGIN,
        TRANSITION_DESCRIPTION_END,
        MARKER_STORAGE_BEGIN,
        MARKER_STORAGE_END,
        IN_STATE_STORAGE_BEGIN,
        IN_STATE_STORAGE_END,
        OUT_STATE_STORAGE_BEGIN,
        OUT_STATE_STORAGE_END,
        IN_TRANSITION_STORAGE_BEGIN,
        IN_TRANSITION_STORAGE_END,
        OUT_TRANSITION_STORAGE_BEGIN,
        OUT_TRANSITION_STORAGE_END
    };

private:
    void increment()
    {
        if (!m_elementToVisit.empty())
        {
            Event currentEvent = m_elementToVisit.front();
            m_elementToVisit.pop_front();
            if (m_visitedIds.find(currentEvent.second.getObjectId()) == m_visitedIds.end())
            {
                m_visitedIds.emplace(currentEvent.second.getObjectId());
                switch (currentEvent.first)
                {
                    case EventType::STATE:
                    {
                        m_elementToVisit.emplace_back(EventType::STATE_DESCRIPTION_BEGIN, currentEvent.second);
                        meta::calculateBasedOnRealtime<StateSerializer<PetriNetTraits>, StateList>(
                                currentEvent.second.getObjectSerializedType(),
                                currentEvent.second.getObjectId(),
                                m_elementToVisit,
                                *m_petriNet);
                        m_elementToVisit.emplace_back(EventType::STATE_DESCRIPTION_END, currentEvent.second);
                        break;
                    }
                    case EventType::TRANSITION:
                    {
                        m_elementToVisit.emplace_back(EventType::TRANSITION_DESCRIPTION_BEGIN, currentEvent.second);
                        meta::calculateBasedOnRealtime<TransitionSerializer<PetriNetTraits>, TransitionList>(
                                currentEvent.second.getObjectSerializedType(),
                                currentEvent.second.getObjectId(),
                                m_elementToVisit,
                                *m_petriNet);
                        m_elementToVisit.emplace_back(EventType::TRANSITION_DESCRIPTION_END, currentEvent.second);
                        break;
                    }
                }
            }
        }
        else
        {
            m_finished = true;
        }
    }

public:
    using PetriNetTraits = _PetriNetTraits;
    using IdType = typename PetriNetTraits::IdType;
    using SerializedObject = typename PetriNet<PetriNetTraits>::template SerializedObject<IdType>;
    using Event = std::pair<EventType, SerializedObject>;

    PetriNetIterator(const container::PetriNet<PetriNetTraits>& petriNet, Event firstObject, bool isEnd):
        m_finished(isEnd),
        m_petriNet(&petriNet)
    {
        m_elementToVisit.emplace_back(firstObject);
    }

    PetriNetIterator() = delete;

    PetriNetIterator(const PetriNetIterator<PetriNetTraits>& other):
        m_finished(other.m_finished),
        m_visitedIds(other.m_visitedIds),
        m_elementToVisit(other.m_elementToVisit),
        m_petriNet(other.m_petriNet)
    {
    }

    void operator=(const PetriNetIterator<PetriNetTraits>& other)
    {
        m_finished = other.m_finished;
        m_petriNet = other.m_petriNet;
        m_elementToVisit = other.m_elementToVisit;
        m_visitedIds = other.m_visitedIds;
    }

    PetriNetIterator(PetriNetIterator<PetriNetTraits>&& other):
        m_finished(other.m_finished),
        m_petriNet(other.m_petriNet)
    {
        m_visitedIds.swap(other.m_visitedIds);
        m_elementToVisit.swap(other.m_elementToVisit);
    }

    void operator=(PetriNetIterator<PetriNetTraits>&& other)
    {
        m_finished = other.m_finished;
        m_petriNet = other.m_petriNet;
        m_visitedIds.swap(other.m_visitedIds);
        m_elementToVisit.swap(other.m_elementToVisit);
    }

    const Event& operator*() const
    {
        return m_elementToVisit.front();
    }

    const Event& operator->() const
    {
        return m_elementToVisit.front();
    }

    void operator++()
    {
        increment();
    }

    void operator++(int)
    {
        increment();
    }

    bool operator==(const PetriNetIterator<PetriNetTraits>& other) const
    {
        if (m_finished && other.m_finished)
        {
            return true;
        }
        return m_finished == other.m_finished
               && m_petriNet == other.m_petriNet
               && m_visitedIds == other.m_visitedIds;

    }

    bool operator!=(const PetriNetIterator<PetriNetTraits>& other) const
    {
        return !(*this == other);
    }

private:
    bool m_finished = false;
    std::deque<Event> m_elementToVisit;
    std::set<IdType> m_visitedIds;
    const container::PetriNet<PetriNetTraits>* m_petriNet;
};

}
}

#endif //SCORPCORE_PETRINETITERATOR_H
