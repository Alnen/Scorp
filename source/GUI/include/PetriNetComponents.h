#ifndef PETRI_NET_COMPONENTS
#define PETRI_NET_COMPONENTS

#include <string>
#include <iostream>

namespace PetriNetComponent
{

class ISerializable
{
public:
    virtual void serialize(std::ostream& output) const = 0;
    virtual void deserialize(std::istream& input) = 0;
};

class Station : public ISerializable
{
public:
    Station();
    Station(std::string name, int x, int y, int capacity=1);
    std::string getName() const;
    int getX() const;
    int getY() const;
    int getCapacity() const;
    void serialize(std::ostream& output) const override;
    void deserialize(std::istream& input) override;
    
private:
    std::string m_name;
    int m_x;
    int m_y;
    int m_capacity;
};

class InterState : public ISerializable
{
public:
    InterState();
    void serialize(std::ostream& output) const override;
    void deserialize(std::istream& input) override;
};

class Semaphore : public ISerializable
{
public:
    Semaphore();
    void serialize(std::ostream& output) const override; 
    void deserialize(std::istream& input) override;
};

class AccessToken : public ISerializable
{
public:
    AccessToken();
    void serialize(std::ostream& output) const override;
    void deserialize(std::istream& input) override;
};

class Train : public ISerializable
{
public:
    Train(int number = 0);
    int getNumber() const;
    void serialize(std::ostream& output) const override;
    void deserialize(std::istream& input) override;
    
private:
    int m_number;
};

class ExitFromStation : public ISerializable
{
public:
    ExitFromStation();
    void serialize(std::ostream& output) const override;
    void deserialize(std::istream& input) override;
};

class EnterToStation : public ISerializable
{
public:
    EnterToStation();
    void serialize(std::ostream& output) const override;
    void deserialize(std::istream& input) override;
};

}

#endif // PETRI_NET_COMPONENTS
