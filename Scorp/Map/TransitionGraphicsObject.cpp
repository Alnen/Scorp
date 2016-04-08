#include "TransitionGraphicsObject.h"
#include <QPen>
#include <QPainter>
#include <QGraphicsEllipseItem>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>

#include <QDebug>

TransitionGraphicsObject::TransitionGraphicsObject(float center_x, float center_y, float width, float height, QColor fill_color, QGraphicsItem* parent)
    : PointGraphicsObject(center_x, center_y, fill_color, QColor::fromRgb(0, 0, 0), 1.f, parent), m_width(width), m_height(height)
{
    m_boundingRect.setRect(-0.5*m_width, -0.5*m_height, m_width, m_height);
    m_selectionExtrude = 2.f;
}

float TransitionGraphicsObject::getWidth()
{
    return m_width;
}

void TransitionGraphicsObject::setWidth(float width)
{
    m_width = (width < 0 ? -width : width);
}

float TransitionGraphicsObject::getHeight()
{
    return m_height;
}

void TransitionGraphicsObject::setHeight(float height)
{
    m_height = (height < 0 ? -height : height);
}

QPainterPath TransitionGraphicsObject::shape() const
{
    QPainterPath path;
    path.addRect(-0.5*m_width, -0.5*m_height, m_width, m_height);
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
        float select_width = m_width + m_selectionExtrude*2;
        float select_height = m_height + m_selectionExtrude*2;
        m_boundingRect.setRect(-0.5*select_width - 1, -0.5*select_height -1, select_width + 2, select_height + 2);
        painter->drawRect(-0.5*select_width, -0.5*select_height, select_width, select_height);
    }
    else
    {
        m_boundingRect.setRect(-0.5*m_width, -0.5*m_height, m_width, m_height);
        fillColor = m_fillColor;
        borderColor = m_borderColor;
    }
    painter->setPen(QPen(QBrush(m_borderColor), m_borderWidth));
    painter->setBrush(m_fillColor);
    painter->drawRect(-0.5*m_width, -0.5*m_height, m_width, m_height);
}

void TransitionGraphicsObject::select()
{
    float select_width = m_width + m_selectionExtrude*2;
    float select_height = m_height + m_selectionExtrude*2;
    m_boundingRect.setRect(-0.5*select_width - 1, -0.5*select_height - 1, select_width + 2, select_height + 2);
    this->setSelected(true);
    this->update();
}
