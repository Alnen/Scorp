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

void Route::push_back(RoutePart part)
{
    //
}

void Route::pop_back()
{
    //
}

void Route::insert(int station_id, RoutePart part)
{
    //
}

void Route::erase(int station_id)
{
    //
}

}
