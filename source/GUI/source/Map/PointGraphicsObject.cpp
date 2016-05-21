#include "../../include/Map/PointGraphicsObject.h"
#include <QPen>
#include <QBrush>
#include <QPainter>
#include <QGraphicsEllipseItem>
#include <QtMath>

PointGraphicsObject::PointGraphicsObject(int object_id, float center_x, float center_y, QColor fill_color, QColor border_color,
    float border_width, QGraphicsItem* parent)
    : QAbstractGraphicsShapeItem(parent), m_id(object_id), m_parentID(-1), m_fillColor(fill_color),
      m_borderWidth(border_width), m_borderColor(border_color), m_boundingRect(center_x, center_y, 1, 1),
      m_selectionExtrude(2.f), m_selected(false)
{
    setPos(center_x, center_y);
    setPen(QPen(QBrush(m_borderColor), m_borderWidth));
    setBrush(QBrush(m_fillColor));
    setFlags(ItemIsSelectable | ItemIsMovable);
}

PointGraphicsObject::PointGraphicsObject(int object_id, float center_x, float center_y, QGraphicsItem* parent)
    : PointGraphicsObject(object_id, center_x, center_y, QColor::fromRgb(255, 0, 0), QColor::fromRgb(0, 0, 0), 1.f, parent)
{
}

void PointGraphicsObject::setCenter(float x, float y)
{
    float center_x = (x < 0 ? 0.f : x);
    float center_y = (y < 0 ? 0.f : y);
    setPos(center_x, center_y);
}

QPointF PointGraphicsObject::getCenter() const
{
    return pos();
}

QColor PointGraphicsObject::getFillColor() const
{
    return m_fillColor;
}

void PointGraphicsObject::setFillColor(QColor color)
{
    m_fillColor = color;
    setBrush(QBrush(m_fillColor));
}

QColor PointGraphicsObject::getBorderColor() const
{
    return m_borderColor;
}

void PointGraphicsObject::setBorderColor(QColor color)
{
    m_borderColor = color;
    setPen(QPen(QBrush(m_borderColor), m_borderWidth));
}

float PointGraphicsObject::getBorderWidth() const
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
    if (!m_selected)
    {
        m_selected = true;
        this->setSelected(true);
        this->update();
    }
}

void PointGraphicsObject::deselect()
{
    if (m_selected)
    {
        m_selected = false;
        this->setSelected(false);
        this->update();
    }
}

bool PointGraphicsObject::selectEnable() const
{
    return m_selected;
}

void PointGraphicsObject::setParentID(int parent)
{
    if (parent < 0)
    {
        m_parentID = -1;
    }
    else
    {
        m_parentID = parent;
    }
}

int PointGraphicsObject::parentID() const
{
    return m_parentID;
}

int PointGraphicsObject::getId() const
{
    return m_id;
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

float PointGraphicsObject::normalizeAngle(float angle)
{
    while (angle < 0)
    {
        angle += 360;
    }
    while (angle > 360)
    {
        angle -= 360;
    }
    return angle;
}

float PointGraphicsObject::getAngle(float sin_a, float cos_a)
{
    float angle = qRadiansToDegrees(qAsin(sin_a));
    if (cos_a < 0)
    {
        return 180 - angle;
    }
    else
    {
        if (sin_a < 0)
        {
            return 360 + angle;
        }
        else
        {
            return angle;
        }
    }
}

QPointF PointGraphicsObject::getEquationByPoints(QPointF p1, QPointF p2)
{
    float k = (p2.y() - p1.y())/(p2.x() - p1.x());
    float b = p1.y() - k * p1.x();
    return QPointF(k, b);
}

QPointF PointGraphicsObject::getEquationIntersectionPoint(float k1, float b1, float k2, float b2)
{
    if (k2 == k1)
    {
        return QPointF();
    }
    float x = (b1 - b2)/(k2 - k1);
    float y = k1 * x + b1;
    return QPointF(x, y);
}

QPointF PointGraphicsObject::getPointByEquation(float x_p, float y_p,
    float x_n, float k, float b, float length)
{
    float delta_x = x_p - x_n;
    float delta_y = y_p - k * x_n - b;
    float scale_factor = length / sqrt(delta_x * delta_x + delta_y * delta_y);
    return QPointF(x_p + scale_factor * delta_x, y_p + scale_factor * delta_y);
}
