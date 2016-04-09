#ifndef MARKERWRAPPER_H
#define MARKERWRAPPER_H

#include "StateWrapper.h"
#include "IdObjectWrapper.h"
#include "PetriNetStorage.h"

template <class _Marker>
class MarkerObjectWrapper
{
public:
    using Marker = _Marker;

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

private:
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

private:
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

    ~MarkerWrapper() {
    }
};

#endif //MARKERWRAPPER_H
