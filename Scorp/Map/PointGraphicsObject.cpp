#include "PointGraphicsObject.h"
#include <QPen>
#include <QBrush>

#include <QPainter>
#include <QGraphicsEllipseItem>

PointGraphicsObject::PointGraphicsObject(float center_x, float center_y, QColor fill_color, QColor border_color,
    float border_width, QGraphicsItem* parent)
    : QAbstractGraphicsShapeItem(parent), m_fillColor(fill_color),
      m_borderWidth(border_width), m_borderColor(border_color), m_boundingRect(center_x, center_y, 1, 1),
      m_selectionExtrude(2.f)
{
    setPos(center_x, center_y);
    setPen(QPen(QBrush(m_borderColor), m_borderWidth));
    setBrush(QBrush(m_fillColor));
    setFlags(ItemIsSelectable | ItemIsMovable);
}

PointGraphicsObject::PointGraphicsObject(float center_x, float center_y, QGraphicsItem* parent)
    : PointGraphicsObject(center_x, center_y, QColor::fromRgb(255, 0, 0), QColor::fromRgb(0, 0, 0), 1.f, parent)
{
}

void PointGraphicsObject::setCenter(float x, float y)
{
    float center_x = (x < 0 ? 0.f : x);
    float center_y = (y < 0 ? 0.f : y);
    setPos(center_x, center_y);
}

QPointF PointGraphicsObject::getCenter()
{
    return pos();
}

QColor PointGraphicsObject::getFillColor()
{
    return m_fillColor;
}

void PointGraphicsObject::setFillColor(QColor color)
{
    m_fillColor = color;
    setBrush(QBrush(m_fillColor));
}

QColor PointGraphicsObject::getBorderColor()
{
    return m_borderColor;
}

void PointGraphicsObject::setBorderColor(QColor color)
{
    m_borderColor = color;
    setPen(QPen(QBrush(m_borderColor), m_borderWidth));
}

float PointGraphicsObject::getBorderWidth()
{
    return m_borderWidth;
}

void PointGraphicsObject::setBorderWidth(float width)
{
    m_borderWidth = width;
    setPen(QPen(QBrush(m_borderColor), m_borderWidth));
}

void PointGraphicsObject::select()
{
    this->update();
}

QRectF PointGraphicsObject::boundingRect() const
{
    return m_boundingRect;
}

int PointGraphicsObject::type() const
{
    return GraphicsObjectType::PointType;
}

void PointGraphicsObject::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    painter->setPen(QPen(QBrush(m_borderColor), m_borderWidth));
    painter->setBrush(m_fillColor);
    painter->drawPoint(0, 0);
}
