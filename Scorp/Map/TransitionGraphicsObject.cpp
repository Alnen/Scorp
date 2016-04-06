#include "TransitionGraphicsObject.h"
#include <QPen>
#include <QPainter>
#include <QGraphicsEllipseItem>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>

TransitionGraphicsObject::TransitionGraphicsObject(float center_x, float center_y, float width, float height, QColor fill_color, QGraphicsItem* parent)
    : PointGraphicsObject(center_x, center_y, fill_color, QColor::fromRgb(0, 0, 0), 1.f, parent), m_width(width), m_height(height)
{
    m_boundingRect.setRect(m_centerX - 0.5*m_width, m_centerY - 0.5*m_height, m_width, m_height);
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

QPainterPath TransitionGraphicsObject::shape() const
{
    QPainterPath path;
    path.addRect(m_centerX - 0.5*m_width, m_centerY - 0.5*m_height, m_width, m_height);
    //path.add(m_centerX - m_radius, m_centerY - m_radius, 2*m_radius, 2*m_radius);
    return path;
}

QRectF TransitionGraphicsObject::boundingRect() const
{
    return m_boundingRect;
}

int TransitionGraphicsObject::type() const
{
    return GraphicsObjectType::TransitionType;
}

void TransitionGraphicsObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*widget)
{
    Q_UNUSED(widget);
    QColor fillColor;
    QColor borderColor;
    if (option->state & QStyle::State_Selected)
    {
        fillColor = m_fillColor.dark(200);
        borderColor = m_borderColor.dark(200);
        painter->setPen(QPen(QBrush(QColor::fromRgb(0, 0, 0)), 1.f, Qt::DashLine));
        int select_width = m_width + 6;
        int select_height = m_height + 6;
        m_boundingRect.setRect(m_centerX - 0.5*select_width - 1, m_centerY - 0.5*select_height - 1,
                               select_width + 2, select_height + 2);
        painter->drawRect(m_centerX - 0.5*select_width, m_centerY - 0.5*select_height,
                             select_width, select_height);
    }
    else
    {
        m_boundingRect.setRect(m_centerX - 0.5*m_width, m_centerY - 0.5*m_height, m_width, m_height);
        fillColor = m_fillColor;
        borderColor = m_borderColor;
    }
    painter->setPen(QPen(QBrush(m_borderColor), m_borderWidth));
    painter->setBrush(m_fillColor);
    painter->drawRect(m_centerX - 0.5*m_width, m_centerY - 0.5*m_height, m_width, m_height);
}
