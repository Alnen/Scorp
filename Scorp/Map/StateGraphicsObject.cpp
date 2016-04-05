#include "StateGraphicsObject.h"
#include <QPen>
#include <QPainter>
#include <QGraphicsEllipseItem>

StateGraphicsObject::StateGraphicsObject(float center_x, float center_y, float radius, QColor fill_color, QGraphicsItem* parent)
    : PointGraphicsObject(center_x, center_y, fill_color, QColor::fromRgb(0, 0, 0), 1.f, parent), m_radius(radius)
{
}

float StateGraphicsObject::getRadius()
{
    return m_radius;
}

void StateGraphicsObject::setRadius(float radius)
{
    if (radius < 0) m_radius = -radius;
    else m_radius = radius;
}

QRectF StateGraphicsObject::boundingRect() const
{
    return QRectF(m_centerX - m_radius, m_centerY - m_radius, 2*m_radius, 2*m_radius);
}

QVariant StateGraphicsObject::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemSelectedHasChanged)
    {
        //emit selectedChange(this);
    }
    return value;
}

void StateGraphicsObject::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    painter->setPen(QPen(QBrush(m_borderColor), m_borderWidth));
    painter->setBrush(m_fillColor);
    painter->drawEllipse(m_centerX - m_radius, m_centerY - m_radius, 2*m_radius, 2*m_radius);
}
