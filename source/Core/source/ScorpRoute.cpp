#include "ScorpRoute.h"

namespace ScorpCore
{

Route::Route(int id, const std::string& name)
    : m_id(id), m_name(name)
{
}

int Route::getId() const
{
    return m_id;
}

std::string Route::getName() const
{
    return m_name;
}

void Route::setName(const std::string& name)
{
    m_name = name;
}

void Route::push_back(const RoutePart& part)
{
    m_parts.push_back(part);
}

void Route::pop_back()
{
    m_parts.pop_back();
}

void Route::insert(size_t index, const RoutePart& part)
{
    if (index < m_parts.size())
    {
        m_parts.insert(m_parts.begin()+index+1, part);
    }
}

void Route::erase(size_t index)
{
    if (index < m_parts.size())
    {
        m_parts.erase(m_parts.begin()+index);
    }
}

void Route::insertAfterStation(int station_id, const RoutePart& part)
{
    for (size_t i = 0; i < m_parts.size(); ++i)
    {
        if (m_parts[i].station == station_id)
        {
            m_parts.insert(m_parts.begin()+i+1, part);
            return;
        }
    }
}

void Route::eraseStation(int station_id)
{
    for (size_t i = 0; i < m_parts.size(); ++i)
    {
        if (m_parts[i].station == station_id)
        {
            m_parts.erase(m_parts.begin()+i);
            return;
        }
    }
}

}
