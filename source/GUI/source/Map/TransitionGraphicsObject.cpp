#include "../../include/Map/TransitionGraphicsObject.h"
#include <QPen>
#include <QPainter>
#include <QGraphicsEllipseItem>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>

#include <QDebug>
#include <QtMath>

TransitionGraphicsObject::TransitionGraphicsObject(int object_id, float center_x, float center_y, float width, float height, QColor fill_color, QGraphicsItem* parent)
    : PointGraphicsObject(object_id, center_x, center_y, fill_color, QColor::fromRgb(0, 0, 0), 1.f, parent), m_width(width), m_height(height)
{
    m_boundingRect.setRect(-0.5*m_width, -0.5*m_height, m_width, m_height);
    m_selectionExtrude = 3.f;
    updateTilt();
}

QPolygonF TransitionGraphicsObject::getVerteciesCoord()
{
    QPolygonF polygon(getVerteciesOffset());
    for (size_t i = 0; i < 4; ++i)
    {
        polygon[i] += getCenter();
    }
    return polygon;
}

QPolygonF TransitionGraphicsObject::getVerteciesOffset()
{
    float tr_length = sqrt(m_width * m_width + m_height * m_height);
    //float tr_cos = m_width / tr_length;
    float tr_sin = (tr_length == 0 ? 1.f : m_height / tr_length);
    float angle1 = qRadiansToDegrees(qAsin(tr_sin));
    float angle2 = 180 - angle1;
    float angle3 = 180 + angle1;
    float angle4 = 360 - angle1;
    angle1 += rotation();
    angle2 += rotation();
    angle3 += rotation();
    angle4 += rotation();
    float r = 0.5 * tr_length;
    QPolygonF polygon;
    polygon << QPointF(r * qCos(qDegreesToRadians(angle1)), r * qSin(qDegreesToRadians(angle1)));
    polygon << QPointF(r * qCos(qDegreesToRadians(angle2)), r * qSin(qDegreesToRadians(angle2)));
    polygon << QPointF(r * qCos(qDegreesToRadians(angle3)), r * qSin(qDegreesToRadians(angle3)));
    polygon << QPointF(r * qCos(qDegreesToRadians(angle4)), r * qSin(qDegreesToRadians(angle4)));
    return polygon;
}

void TransitionGraphicsObject::setRotation(qreal angle)
{
    angle = normalizeAngle(angle);
    if ((angle > 90) && (angle <= 270))
    {
        angle -= 180;
    }
    angle = normalizeAngle(angle);
    PointGraphicsObject::setRotation(angle);
}


float TransitionGraphicsObject::getTilt() const
{
    return m_tilt;
}

float TransitionGraphicsObject::getWidth()
{
    return m_width;
}

void TransitionGraphicsObject::setWidth(float width)
{
    m_width = (width < 0 ? -width : width);
    updateTilt();
}

float TransitionGraphicsObject::getHeight()
{
    return m_height;
}

void TransitionGraphicsObject::setHeight(float height)
{
    m_height = (height < 0 ? -height : height);
    updateTilt();
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

void TransitionGraphicsObject::updateTilt()
{
    float tr_length = sqrt(m_width * m_width + m_height * m_height);
    m_tilt = (tr_length == 0 ? 90 : qRadiansToDegrees(qAsin(m_height / tr_length)));
}

void TransitionGraphicsObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*widget)
{
    Q_UNUSED(widget);
    QColor fillColor;
    QColor borderColor;
    if ((option->state & QStyle::State_Selected) && (!m_selected))
    {
        m_selected = true;
    }
    //if (option->state & QStyle::State_Selected)
    if (m_selected)
    {
        fillColor = m_fillColor.dark(200);
        borderColor = m_borderColor.dark(200);
        painter->setPen(QPen(QBrush(QColor::fromRgb(0, 0, 0)), 1.f, Qt::DashLine));
        float select_width = m_width + m_selectionExtrude*2;
        float select_height = m_height + m_selectionExtrude*2;
        //m_boundingRect.setRect(-0.5*select_width - 1, -0.5*select_height -1, select_width + 2, select_height + 2);
        painter->drawRect(-0.5*select_width, -0.5*select_height, select_width, select_height);
    }
    else
    {
        //m_boundingRect.setRect(-0.5*m_width, -0.5*m_height, m_width, m_height);
        fillColor = m_fillColor;
        borderColor = m_borderColor;
    }
    painter->setPen(QPen(QBrush(m_borderColor), m_borderWidth));
    painter->setBrush(m_fillColor);
    painter->drawRect(-0.5*m_width, -0.5*m_height, m_width, m_height);
}

void TransitionGraphicsObject::select()
{
    if (!m_selected)
    {
        float select_width = m_width + m_selectionExtrude*2;
        float select_height = m_height + m_selectionExtrude*2;
        m_boundingRect.setRect(-0.5*select_width - 1, -0.5*select_height - 1, select_width + 2, select_height + 2);
        m_selected = true;
        this->setSelected(true);
        this->update();
    }
}

void TransitionGraphicsObject::deselect()
{
    if (m_selected)
    {
        m_boundingRect.setRect(-0.5*m_width, -0.5*m_height, m_width, m_height);
        m_selected = false;
        this->setSelected(false);
        this->update();
    }
}
