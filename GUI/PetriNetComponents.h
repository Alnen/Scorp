#ifndef PETRI_NET_COMPONENTS
#define PETRI_NET_COMPONENTS

namespace PetryNetComponent
{
class Station
{
private:
    std::string m_name;
    int m_x;
    int m_y;
    int m_capacity;

public:
    Station() : m_name(""), m_x(0), m_y(0), m_capacity(1)
    {
    }

    Station(std::string name, int x, int y, int capacity=1)
        : m_name(name), m_x(x), m_y(y), m_capacity(capacity)
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

class InterState
{
public:
    InterState()
    {
    }
};

class Semaphore
{
public:
    Semaphore()
    {
    }
};

class AccessToken
{
public:
    AccessToken()
    {
    }
};

class Train
{
private:
    int m_number;

public:
    Train(int number) : m_number(number)
    {
    }

    int getNumber() const
    {
        return m_number;
    }
};

class ExitFromStation
{
public:
    ExitFromStation()
    {
    }
};

class EnterToStation
{
public:
    EnterToStation()
    {
    }
};

}

#endif // PETRI_NET_COMPONENTS
