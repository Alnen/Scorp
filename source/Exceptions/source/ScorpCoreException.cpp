#include "Scorp/Exceptions/ScorpCoreException.h"

namespace ScorpCoreException
{

// class MarkerNotFoundException
MarkerNotFoundException::MarkerNotFoundException(int marker_id)
    : m_markerId(marker_id)
{
}

std::string MarkerNotFoundException::getErrorCode() const
{
    return "SCORPCORE02";
}

int MarkerNotFoundException::getMarkerId() const
{
    return m_markerId;
}

// class StateNotFoundException
StateNotFoundException::StateNotFoundException(int state_id)
    : m_stateId(state_id)
{
}

std::string StateNotFoundException::getErrorCode() const
{
    return "SCORPCORE03";
}

int StateNotFoundException::getStateId() const
{
    return m_stateId;
}

// class TransitionNotFoundException
TransitionNotFoundException::TransitionNotFoundException(int transition_id)
    : m_transitionId(transition_id)
{
}

std::string TransitionNotFoundException::getErrorCode() const
{
    return "SCORPCORE04";
}

int TransitionNotFoundException::getTransitionId() const
{
    return m_transitionId;
}

// class AdditionFailedException
AdditionFailedException::AdditionFailedException(ItemType item_type)
    : m_itemType(item_type)
{
}

std::string AdditionFailedException::getErrorCode() const
{
    return "SCORPCORE05";
}

AdditionFailedException::ItemType AdditionFailedException::getItemType() const
{
    return m_itemType;
}

// class RemovalFailedException
RemovalFailedException::RemovalFailedException(ItemType item_type)
    : m_itemType(item_type)
{
}

std::string RemovalFailedException::getErrorCode() const
{
    return "SCORPCORE06";
}

RemovalFailedException::ItemType RemovalFailedException::getItemType() const
{
    return m_itemType;
}

}
