#include "TrackGraphicsObject.h"
#include <QPen>
#include <QBrush>

TrackGraphicsObject::TrackGraphicsObject(StateGraphicsObject* state, TransitionGraphicsObject* transition, TrackDirection direction, QColor color,
    float width, QGraphicsItem *parent) : QGraphicsLineItem(parent), m_direction(direction), m_color(color), m_width(width)
{   
    float delta_x = transition->getCenterX() - state->getCenterX();
    float delta_y = transition->getCenterY() - state->getCenterY();
    m_length = sqrt(delta_x * delta_x + delta_y * delta_y);
    float scale_factor1 = state->getRadius() / m_length;
    m_x1 = state->getCenterX() + scale_factor1 * delta_x;
    m_y1 = state->getCenterY() + scale_factor1 * delta_y;
    float cos_alpha = delta_x / m_length;
    float sin_alpha = delta_y / m_length;
    float tr_width = transition->getWidth();
    float tr_height = transition->getHeight();
    float tr_length = sqrt(tr_width * tr_width + tr_height * tr_height);
    float tr_cos = tr_width / tr_length;
    int part = 0;
    if (sin_alpha >= 0)
    {
        if (cos_alpha >= 0)
        {
            // 1 chapter
            part = (cos_alpha >= tr_cos ? 1 : 2);
        }
        else
        {
            // 2 chapter
            tr_cos = -tr_cos;
            part = (cos_alpha >= tr_cos ? 2 : 3);
        }
    }
    else
    {
        if (cos_alpha >= 0)
        {
            // 4 chapter
            part = (cos_alpha >= tr_cos ? 1 : 4);
        }
        else
        {
            // 3 chapter
            tr_cos = -tr_cos;
            part = (cos_alpha >= tr_cos ? 4 : 3);
        }
    }
    float st_x = state->getCenterX();
    float st_y = state->getCenterY();
    float tr_x = transition->getCenterX();
    float tr_y = transition->getCenterY();
    if (st_x == tr_x)
    {
        m_x2 = st_x;
        m_y2 = (part == 2 ? tr_y - 0.5*tr_height : tr_y + 0.5*tr_height);
    }
    else
    {
        float k = (st_y - tr_y)/(st_x - tr_x);
        float b = st_y - k * st_x;
        float a = 0;
        if (part == 1 || part == 3)
        {
            a = (part == 1 ? tr_x - 0.5*tr_width : tr_x + 0.5*tr_width);
            m_x2 = a;
            m_y2 = k * a + b;
        }
        else
        {
            a = (part == 2 ? tr_y - 0.5*tr_height : tr_y + 0.5*tr_height);
            m_x2 = (a - b)/k;
            m_y2 = a;
        }
    }
    this->setLine(m_x1, m_y1, m_x2, m_y2);
    setPen(QPen(QBrush(color), width));
}

TrackGraphicsObject::TrackGraphicsObject(StateGraphicsObject* state1, StateGraphicsObject* state2, TrackDirection direction, QColor color,
    float width, QGraphicsItem *parent) : QGraphicsLineItem(parent), m_direction(direction), m_color(color), m_width(width)
{
    float delta_x = state2->getCenterX() - state1->getCenterX();
    float delta_y = state2->getCenterY() - state1->getCenterY();
    m_length = sqrt(delta_x * delta_x + delta_y * delta_y);
    float scale_factor1 = state1->getRadius() / m_length;
    float scale_factor2 = state2->getRadius() / m_length;
    m_x1 = state1->getCenterX() + scale_factor1 * delta_x;
    m_y1 = state1->getCenterY() + scale_factor1 * delta_y;
    m_x2 = state2->getCenterX() - scale_factor2 * delta_x;
    m_y2 = state2->getCenterY() - scale_factor2 * delta_y;

    this->setLine(m_x1, m_y1, m_x2, m_y2);
    setPen(QPen(QBrush(color), width));
}

PointGraphicsObject* TrackGraphicsObject::getFirstVertex()
{
    return m_first_vertex;
}

PointGraphicsObject* TrackGraphicsObject::getSecondVertex()
{
    return m_second_vertex;
}

TrackDirection TrackGraphicsObject::getDirection()
{
    return m_direction;
}

void TrackGraphicsObject::setDirection(TrackDirection direction)
{
    m_direction = direction;
}

float TrackGraphicsObject::getX1()
{
    return m_x1;
}

float TrackGraphicsObject::getY1()
{
    return m_y1;
}

float TrackGraphicsObject::getX2()
{
    return m_x2;
}

float TrackGraphicsObject::getY2()
{
    return m_y2;
}

float TrackGraphicsObject::getLength()
{
    return m_length;
}

QColor TrackGraphicsObject::getColor()
{
    return m_color;
}

void TrackGraphicsObject::setColor(QColor color)
{
    m_color = color;
    setPen(QPen(QBrush(m_color), m_width));
}

float TrackGraphicsObject::getWidth()
{
    return m_width;
}

void TrackGraphicsObject::setWidth(float width)
{
    m_width = width;
    setPen(QPen(QBrush(m_color), m_width));
}
