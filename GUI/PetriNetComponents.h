#ifndef PETRI_NET_COMPONENTS
#define PETRI_NET_COMPONENTS

#include "container/PetriNetTraits.h"
#include "container/PetriNet.h"

namespace PetryNetComponent
{
class BaseComponent
{
private:
    int m_id;

public:
    BaseComponent() : m_id(0)
    {
    }

    BaseComponent(int id) : m_id(id)
    {
    }

    const int getId() const
    {
        return m_id;
    }
};

class Station : public BaseComponent
{
private:
    std::string m_name;
    int m_x;
    int m_y;
    int m_capacity;

public:
    Station(int id = 0) : BaseComponent(id),
        m_name(""), m_x(0), m_y(0), m_capacity(1)
    {
    }

    Station(int id, std::string name, int x, int y, int capacity=1)
        : BaseComponent(id), m_name(name), m_x(x), m_y(y), m_capacity(capacity)
    {
    }

    std::string getName() const
    {
        return m_name;
    }

    int getX() const
    {
        return m_x;
    }

    int getY() const
    {
        return m_y;
    }

    int getCapacity() const
    {
        return m_capacity;
    }
};

class InterState : public BaseComponent
{
public:
    InterState(int id = 0) : BaseComponent(id)
    {
    }
};

class Semaphore : public BaseComponent
{
public:
    Semaphore(int id = 0) : BaseComponent(id)
    {
    }
};

class AccessToken : public BaseComponent
{
public:
    AccessToken(int id = 0) : BaseComponent(id)
    {
    }
};

class Train : public BaseComponent
{
private:
    int m_number;

public:
    Train(int number) : BaseComponent(), m_number(number)
    {
    }

    Train(int id, int number) : BaseComponent(id), m_number(number)
    {
    }

    int getNumber() const
    {
        return m_number;
    }
};

class ExitFromStation : public BaseComponent
{
public:
    ExitFromStation() : BaseComponent()
    {
    }

    ExitFromStation(int id) : BaseComponent(id)
    {
    }
};

class EnterToStation : public BaseComponent
{
public:
    EnterToStation() : BaseComponent()
    {
    }

    EnterToStation(int id) : BaseComponent(id)
    {
    }
};

using MarkerList = meta::TypeList<Train, AccessToken>;
using StateList = meta::TypeList<Station, InterState, Semaphore>;
using TransitionList = meta::TypeList<ExitFromStation, EnterToStation>;
using RailwayPetriNetTraits = PetriNetTraits<MarkerList, TransitionList, StateList>;
using RailwayPetriNet = container::PetriNet<RailwayPetriNetTraits>;
using IdType = typename RailwayPetriNetTraits::IdType;
}

#endif // PETRI_NET_COMPONENTS
