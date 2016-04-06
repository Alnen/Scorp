#include "PointGraphicsObject.h"
#include <QPen>
#include <QBrush>

#include <QPainter>
#include <QGraphicsEllipseItem>

PointGraphicsObject::PointGraphicsObject(float center_x, float center_y, QColor fill_color, QColor border_color,
    float border_width, QGraphicsItem* parent)
    : QAbstractGraphicsShapeItem(parent), m_centerX(center_x), m_centerY(center_y), m_fillColor(fill_color),
      m_borderWidth(border_width), m_borderColor(border_color), m_boundingRect(center_x, center_y, 1, 1)
{
    setPen(QPen(QBrush(m_borderColor), m_borderWidth));
    setBrush(QBrush(m_fillColor));
    setFlags(ItemIsSelectable | ItemIsMovable);
}

PointGraphicsObject::PointGraphicsObject(float center_x, float center_y, QGraphicsItem* parent)
    : PointGraphicsObject(center_x, center_y, QColor::fromRgb(255, 0, 0), QColor::fromRgb(0, 0, 0), 1.f, parent)
{
}

float PointGraphicsObject::getCenterX()
{
    return m_centerX;
}

void PointGraphicsObject::setCenterX(float x)
{
    if (x < 0) m_centerX = 0.f;
    else m_centerX = x;
}

float PointGraphicsObject::getCenterY()
{
    return m_centerY;
}

void PointGraphicsObject::setCenterY(float y)
{
    if (y < 0) m_centerY = 0.f;
    else m_centerY = y;
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
    painter->drawPoint(m_centerX, m_centerY);
}
