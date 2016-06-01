#ifndef MARKERWRAPPER_H
#define MARKERWRAPPER_H

#include "StateWrapper.h"
#include "IdObjectWrapper.h"
#include "Scorp/container/internal/PetriNetStorage.h"

#include <iostream>

namespace container
{

template <class _Marker>
class MarkerObjectWrapper
{
public:
    using Marker = _Marker;

    MarkerObjectWrapper() = default;

    MarkerObjectWrapper(Marker&& marker) : m_marker(std::forward<Marker>(marker))
    {
    }

    template <class... Arguments>
    MarkerObjectWrapper(Arguments&&... arguments) : m_marker(std::forward<Arguments>(arguments)...)
    {
    }

    Marker& getMarker()
    {
        return m_marker;
    }

protected:
    Marker m_marker;
};

template <class _PetriNetTraits>
class StateIdWrapper
{
public:
    using PetriNetTraits = _PetriNetTraits;
    using IdType = typename PetriNetTraits::IdType;

    StateIdWrapper(IdType parentId):
            m_stateId(parentId)
    {
    }

    IdType getStateId()
    {
        return m_stateId;
    }

    // TODO: fix
    void setStateId(IdType parentId)
    {
        m_stateId = parentId;
    }

protected:
    IdType m_stateId;
};

template <class _Marker, class _PetriNetTraits>
class MarkerWrapper : public MarkerObjectWrapper<_Marker>,
                      public IdObjectWrapper<typename _PetriNetTraits::IdType>,
                      public StateIdWrapper<_PetriNetTraits>
{
public:
    using Marker = _Marker;
    using PetriNetTraits = _PetriNetTraits;
    using IdType = typename PetriNetTraits::IdType;

    MarkerWrapper(const MarkerWrapper&) = delete;
    MarkerWrapper(MarkerWrapper&&) = default;

    MarkerWrapper& operator=(const MarkerWrapper&) = delete;
    MarkerWrapper& operator=(MarkerWrapper&&) = default;

    MarkerWrapper() :
            MarkerObjectWrapper<Marker>(),
            IdObjectWrapper<IdType>(0),
            StateIdWrapper<_PetriNetTraits>(0)
    {
    }

    MarkerWrapper(IdType id, IdType parentId, Marker&& state) :
            MarkerObjectWrapper<Marker>(std::forward<Marker>(state)),
            IdObjectWrapper<IdType>(id),
            StateIdWrapper<_PetriNetTraits>(parentId)
    {
    }

    template <class... Arguments>
    MarkerWrapper(IdType id, IdType parentId, Arguments&&... arguments) :
            MarkerObjectWrapper<Marker>(std::forward<Arguments>(arguments)...),
            IdObjectWrapper<IdType>(id),
            StateIdWrapper<_PetriNetTraits>(parentId)
    {
    }
    
    void serialize(std::ostream& output) const
    {
        output << "{" << (int)IdObjectWrapper<IdType>::m_id << ";";
        output << (int)StateIdWrapper<PetriNetTraits>::m_stateId << ";";
        MarkerObjectWrapper<Marker>::m_marker.serialize(output);
        output << "}";
    }
    
    void deserialize(std::istream& input)
    {
        std::string item;
        std::getline(input, item, '{');
        std::getline(input, item, ';');
        IdObjectWrapper<IdType>::m_id = std::stoi(item);
        std::getline(input, item, ';');
        StateIdWrapper<PetriNetTraits>::m_stateId = std::stoi(item);
        MarkerObjectWrapper<Marker>::m_marker.deserialize(input);
        std::getline(input, item, '}');
    }

    ~MarkerWrapper() {
    }
};

}

#endif //MARKERWRAPPER_H
