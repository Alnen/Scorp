#ifndef PETRI_NET_COMPONENTS
#define PETRI_NET_COMPONENTS

#include <string>

namespace PetriNetComponent
{

class ISerializable
{
public:
    virtual void serialize(std::ostream& str) const = 0;
    virtual void deserialize(std::istream& str) const = 0;
};

class Station : public ISerializable
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
    
    void serialize(std::ostream& str) const
    {
        //
    }
    
    void deserialize(std::istream& str) const
    {
        //
    }
};

class InterState : public ISerializable
{
public:
    InterState()
    {
    }
    
    void serialize(std::ostream& str) const
    {
        //
    }
    
    void deserialize(std::istream& str) const
    {
        //
    }
};

class Semaphore : public ISerializable
{
public:
    Semaphore()
    {
    }
    
    void serialize(std::ostream& str) const
    {
        //
    }
    
    void deserialize(std::istream& str) const
    {
        //
    }
};

class AccessToken : public ISerializable
{
public:
    AccessToken()
    {
    }
    
    void serialize(std::ostream& str) const
    {
        //
    }
    
    void deserialize(std::istream& str) const
    {
        //
    }
};

class Train : public ISerializable
{
private:
    int m_number;

public:
    Train(int number = 0) : m_number(number)
    {
    }

    int getNumber() const
    {
        return m_number;
    }
    
    void serialize(std::ostream& str) const
    {
        //
    }
    
    void deserialize(std::istream& str) const
    {
        //
    }
};

class ExitFromStation : public ISerializable
{
public:
    ExitFromStation()
    {
    }
    
    void serialize(std::ostream& str) const
    {
        //
    }
    
    void deserialize(std::istream& str) const
    {
        //
    }
};

class EnterToStation : public ISerializable
{
public:
    EnterToStation()
    {
    }
    
    void serialize(std::ostream& str) const
    {
        //
    }
    
    void deserialize(std::istream& str) const
    {
        //
    }
};

}

#endif // PETRI_NET_COMPONENTS
