#include "Scorp/GUI/Map/MarkerObject.h"
#include "Scorp/GUI/Map/StateGraphicsObject.h"

MarkerObject::MarkerObject(int id, MarkerType type, StateGraphicsObject* parent)
    : m_id(id), m_type(type), m_parentState(parent)
{
}

void MarkerObject::connectToState(StateGraphicsObject* state)
{
    m_parentState = state;
}

void MarkerObject::disconnectFromState()
{
    m_parentState = nullptr;
}

int MarkerObject::getId() const
{
    return m_id;
}

MarkerObject::MarkerType MarkerObject::getType() const
{
    return m_type;
}

int MarkerObject::MarkerTypeToInt(MarkerType type)
{
    if (type == MarkerType::Train)
    {
        return 0;
    }
    else if (type == MarkerType::AccessToken)
    {
        return 1;
    }
    return -1;
}

void MarkerObject::setStyle(const MapSceneStyle::MarkerStyle& style)
{
    m_style = style;
}

MapSceneStyle::MarkerStyle MarkerObject::getStyle() const
{
    return m_style;
}
