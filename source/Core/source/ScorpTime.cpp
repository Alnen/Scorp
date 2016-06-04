#include <sstream>
#include <string>
#include "Scorp/Core/ScorpTime.h"

namespace ScorpCore
{

Time::Time(size_t s)
    : m_days(0), m_hours(0), m_minutes(0), m_seconds(s >= 60 ? 59 : s)
{
}

Time::Time(size_t m, size_t s)
    : m_days(0), m_hours(0), m_minutes(m >= 60 ? 59 : m), m_seconds(s >= 60 ? 59 : s)
{
}

Time::Time(size_t h, size_t m, size_t s)
    : m_days(0), m_hours(h >= 24 ? 23 : h), m_minutes(m >= 60 ? 59 : m), m_seconds(s >= 60 ? 59 : s)
{
}

Time::Time(size_t d, size_t h, size_t m, size_t s)
    : m_days(d), m_hours(h >= 24 ? 23 : h), m_minutes(m >= 60 ? 59 : m), m_seconds(s >= 60 ? 59 : s)
{
}

void Time::setTime(size_t d, size_t h, size_t m, size_t s)
{
    m_days = d;
    m_hours = (h >= 24 ? 23 : h);
    m_minutes = (m >= 60 ? 59 : m);
    m_seconds = (s >= 60 ? 59 : s);
}

void Time::setDays(size_t value)
{
    m_days = value;
}

void Time::setHours(size_t value)
{
    m_hours = (value >= 24 ? 23 : value);
}

void Time::setMinutes(size_t value)
{
    m_minutes = (value >= 60 ? 59 : value);
}

void Time::setSeconds(size_t value)
{
    m_seconds = (value >= 60 ? 59 : value);
}

size_t Time::getDays() const
{
    return m_days;
}

size_t Time::getHours() const
{
    return m_hours;
}

size_t Time::getMinutes() const
{
    return m_minutes;
}

size_t Time::getSeconds() const
{
    return m_seconds;
}

Time Time::fromString(const std::string& str_time)
{
    Time time;
    std::string item;
    std::stringstream ss(str_time);
    std::getline(ss, item, '-');
    std::getline(ss, item, '-');
    std::getline(ss, item, 'T');
    time.setDays(std::stoi(item));
    std::getline(ss, item, ':');
    time.setHours(std::stoi(item));
    std::getline(ss, item, ':');
    time.setMinutes(std::stoi(item));
    std::getline(ss, item, ':');
    time.setSeconds(std::stoi(item));
    return time;
}

std::string Time::toString(const Time& time)
{
    std::string str_days = std::string(time.getDays() < 10 ? "0" : "")
        + std::to_string(time.getDays());
    std::string str_hours = std::string(time.getHours() < 10 ? "0" : "")
        + std::to_string(time.getHours());
    std::string str_minutes = std::string(time.getMinutes() < 10 ? "0" : "")
        + std::to_string(time.getMinutes());
    std::string str_seconds = std::string(time.getSeconds() < 10 ? "0" : "")
        + std::to_string(time.getSeconds());
    return std::string("0000-00-") + str_days + "T" + str_hours + ":" + str_minutes + ":" + str_seconds;
}

}
