#include "StateGraphicsObject.h"
#include <QPen>
#include <QPainter>
#include <QGraphicsEllipseItem>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>

#include <QDebug>

StateGraphicsObject::StateGraphicsObject(float center_x, float center_y, float radius, QColor fill_color, QGraphicsItem* parent)
    : PointGraphicsObject(center_x, center_y, fill_color, QColor::fromRgb(0, 0, 0), 1.f, parent), m_radius(radius)
{
    m_boundingRect.setRect(-m_radius, -m_radius, 2*m_radius, 2*m_radius);
    m_selectionExtrude = 3.f;
}

float StateGraphicsObject::getRadius()
{
    return m_radius;
}

void StateGraphicsObject::setRadius(float radius)
{
    m_radius = (radius < 0 ? -radius : radius);
}

QPainterPath StateGraphicsObject::shape() const
{
    QPainterPath path;
    path.addEllipse(-m_radius, -m_radius, 2*m_radius, 2*m_radius);
    return path;
}

int StateGraphicsObject::type() const
{
    return GraphicsObjectType::StateType;
}

QRectF StateGraphicsObject::boundingRect() const
{
    return m_boundingRect;
}

void StateGraphicsObject::select()
{
    float select_radius = m_radius + m_selectionExtrude;
    m_boundingRect.setRect(-select_radius-1, -select_radius-1, 2*(select_radius + 1), 2*(select_radius + 1));
    this->setSelected(true);
    this->update();
}

void StateGraphicsObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(widget);
    QColor fillColor;
    QColor borderColor;
    if (option->state & QStyle::State_Selected)
    {
        fillColor = m_fillColor.dark(200);
        borderColor = m_borderColor.dark(200);
        painter->setPen(QPen(QBrush(QColor::fromRgb(0, 0, 0)), 1.f, Qt::DashLine));
        float select_radius = m_radius + m_selectionExtrude;
        m_boundingRect.setRect(-select_radius-1, -select_radius-1, 2*(select_radius + 1), 2*(select_radius + 1));
        painter->drawEllipse(-select_radius, -select_radius, 2*select_radius, 2*select_radius);
    }
    else
    {
        m_boundingRect.setRect(-m_radius, -m_radius, 2*m_radius, 2*m_radius);
        fillColor = m_fillColor;
        borderColor = m_borderColor;
    }
    painter->setPen(QPen(QBrush(borderColor), m_borderWidth));
    painter->setBrush(fillColor);
    painter->drawEllipse(-m_radius, -m_radius, 2*m_radius, 2*m_radius);
}
