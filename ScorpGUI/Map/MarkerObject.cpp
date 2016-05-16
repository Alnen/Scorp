#include "MarkerObject.h"
#include "StateGraphicsObject.h"

MarkerObject::MarkerObject(int id, int color, StateGraphicsObject* parent)
    : m_id(id), m_color(color), m_parentState(parent)
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

int MarkerObject::getId()
{
    return m_id;
}

int MarkerObject::getColor()
{
    return m_color;
}
