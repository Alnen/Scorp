#include "PetriNetComponents.h"

#include <sstream>

namespace PetriNetComponent
{
// class Station
Station::Station() : m_name(""), m_x(0), m_y(0), m_capacity(1)
{
}

Station::Station(std::string name, int x, int y, int capacity)
    : m_name(name), m_x(x), m_y(y), m_capacity(capacity)
{
}

std::string Station::getName() const
{
    return m_name;
}

int Station::getX() const
{
    return m_x;
}

int Station::getY() const
{
    return m_y;
}

int Station::getCapacity() const
{
    return m_capacity;
}

void Station::serialize(std::ostream& output) const
{
    output << "Station(" << m_name << ", " << m_x << ", "
           << m_y << ", " << m_capacity << ")" << std::endl;
}

void Station::deserialize(std::istream& input)
{
    std::string line;
    std::getline(input, line, '(');
    std::getline(input, line, ')');
    std::stringstream ss(line);
    std::getline(ss, line, ',');
    m_name = line;
    std::getline(ss, line, ',');
    m_x = std::stoi(line);
    std::getline(ss, line, ',');
    m_y = std::stoi(line);
    std::getline(ss, line, ',');
    m_capacity = std::stoi(line);
}

// class InterState
InterState::InterState()
{
}

void InterState::serialize(std::ostream& output) const
{
}

void InterState::deserialize(std::istream& input)
{
}

// class Semaphore
Semaphore::Semaphore()
{
}

void Semaphore::serialize(std::ostream& output) const
{
}

void Semaphore::deserialize(std::istream& input)
{
}

// class AccessToken
AccessToken::AccessToken()
{
}

void AccessToken::serialize(std::ostream& output) const
{
}

void AccessToken::deserialize(std::istream& input)
{
}

// class Train
Train::Train(int number) : m_number(number)
{
}

int Train::getNumber() const
{
    return m_number;
}

void Train::serialize(std::ostream& output) const
{
    output << "Train(" << m_number << ")" << std::endl;
}

void Train::deserialize(std::istream& input)
{
    std::string line;
    std::getline(input, line, '(');
    std::getline(input, line, ')');
    m_number = std::stoi(line);
}

// class ExitFromStation
ExitFromStation::ExitFromStation()
{
}

void ExitFromStation::serialize(std::ostream& output) const
{
}

void ExitFromStation::deserialize(std::istream& input)
{
}

// class EnterToStation
EnterToStation::EnterToStation()
{
}

void EnterToStation::serialize(std::ostream& output) const
{
}

void EnterToStation::deserialize(std::istream& input)
{
}

}
