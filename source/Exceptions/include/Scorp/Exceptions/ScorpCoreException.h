#ifndef SCORP_CORE_EXCEPTION_H
#define SCORP_CORE_EXCEPTION_H

#include "ScorpException.h"

namespace ScorpCoreException
{

class MarkerNotFoundException : public ScorpCoreBaseException
{
public:
    MarkerNotFoundException(int marker_id);
    virtual std::string getErrorCode() const;
    int getMarkerId() const;

private:
    int m_markerId;
};

class StateNotFoundException : public ScorpCoreBaseException
{
public:
    StateNotFoundException(int state_id);
    virtual std::string getErrorCode() const;
    int getStateId() const;

private:
    int m_stateId;
};

class TransitionNotFoundException : public ScorpCoreBaseException
{
public:
    TransitionNotFoundException(int transition_id);
    virtual std::string getErrorCode() const;
    int getTransitionId() const;

private:
    int m_transitionId;
};

class AdditionFailedException : public ScorpCoreBaseException
{
public:
    enum class ItemType { Marker, State, Transition, Unknown };
    AdditionFailedException(ItemType item_type = ItemType::Unknown);
    virtual std::string getErrorCode() const;
    ItemType getItemType() const;

private:
    ItemType m_itemType;
};

class RemovalFailedException : public ScorpCoreBaseException
{
public:
    enum class ItemType { Marker, State, Transition, Unknown };
    RemovalFailedException(ItemType item_type = ItemType::Unknown);
    virtual std::string getErrorCode() const;
    ItemType getItemType() const;

private:
    ItemType m_itemType;
};

}

#endif // SCORP_CORE_EXCEPTION_H
