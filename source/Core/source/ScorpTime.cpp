#include "ScorpTime.h"

namespace ScorpCore
{

Time::Time(size_t s)
{
}

Time::Time(size_t m, size_t s)
{
}

Time::Time(size_t h, size_t m, size_t s)
    : m_hours(h), m_minutes(m >= 60 ? 59 : m), m_seconds(s >= 60 ? 59 : s)
{
}

void Time::setTime(size_t h, size_t m, size_t s)
{
    m_hours = h;
    m_minutes = (m >= 60 ? 59 : m);
    m_seconds = (s >= 60 ? 59 : s);
}

void Time::setHours(size_t value)
{
    m_hours = value;
}

void Time::setMinutes(size_t value)
{
    m_minutes = (value >= 60 ? 59 : value);
}

void Time::setSeconds(size_t value)
{
    m_seconds = (value >= 60 ? 59 : value);
}

size_t Time::getHours()
{
    return m_hours;
}

size_t Time::getMinutes()
{
    return m_minutes;
}

size_t Time::getSeconds()
{
    return m_seconds;
}

}
