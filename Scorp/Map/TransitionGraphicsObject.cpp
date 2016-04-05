#include "TransitionGraphicsObject.h"
#include <QPen>
#include <QPainter>
#include <QGraphicsEllipseItem>

TransitionGraphicsObject::TransitionGraphicsObject(float center_x, float center_y, float width, float height, QColor fill_color, QGraphicsItem* parent)
    : PointGraphicsObject(center_x, center_y, fill_color, QColor::fromRgb(0, 0, 0), 1.f, parent), m_width(width), m_height(height)
{
}

float TransitionGraphicsObject::getWidth()
{
    return m_width;
}

void TransitionGraphicsObject::setWidth(float width)
{
    if (width < 0) m_width = -width;
    else m_width = width;
}

float TransitionGraphicsObject::getHeight()
{
    return m_height;
}

void TransitionGraphicsObject::setHeight(float height)
{
    if (height < 0) m_height = -height;
    else m_height = height;
}

QRectF TransitionGraphicsObject::boundingRect() const
{
    return QRectF(m_centerX - 0.5*m_width, m_centerY - 0.5*m_height, m_width, m_height);
}

void TransitionGraphicsObject::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    painter->setPen(QPen(QBrush(m_borderColor), m_borderWidth));
    painter->setBrush(m_fillColor);
    painter->drawRect(m_centerX - 0.5*m_width, m_centerY - 0.5*m_height, m_width, m_height);
}
