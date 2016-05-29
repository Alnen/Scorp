#ifndef SCORP_TIME_H
#define SCORP_TIME_H

#include <string>

namespace ScorpCore
{

using size_t = unsigned int;

class Time
{
public:
    Time(size_t s=0);
    Time(size_t m, size_t s);
    Time(size_t h, size_t m, size_t s);
    Time(size_t d, size_t h, size_t m, size_t s);
    void setTime(size_t d, size_t h, size_t m, size_t s);
    void setDays(size_t value);
    void setHours(size_t value);
    void setMinutes(size_t value);
    void setSeconds(size_t value);
    size_t getDays() const;
    size_t getHours() const;
    size_t getMinutes() const;
    size_t getSeconds() const;
    static Time fromString(const std::string& str_time);
    static std::string toString(const Time& time);

private:
    size_t m_days;
    size_t m_hours;
    size_t m_minutes;
    size_t m_seconds;
};

}

#endif // SCORP_TIME_H
