#ifndef SCORP_TIME_H
#define SCORP_TIME_H

namespace ScorpCore
{

using size_t = unsigned int;

class Time
{
public:
    Time(size_t s=0);
    Time(size_t m, size_t s);
    Time(size_t h, size_t m, size_t s);
    void setTime(size_t h, size_t m, size_t s);
    void setHours(size_t value);
    void setMinutes(size_t value);
    void setSeconds(size_t value);
    size_t getHours();
    size_t getMinutes();
    size_t getSeconds();

private:
    size_t m_hours;
    size_t m_minutes;
    size_t m_seconds;
};

}

#endif // SCORP_TIME_H
