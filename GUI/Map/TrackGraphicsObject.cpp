#include "TrackGraphicsObject.h"
#include "StateGraphicsObject.h"
#include "TransitionGraphicsObject.h"
#include <QPen>
#include <QBrush>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>

#include <QDebug>

#include <math.h>
#include <QtMath>

TrackGraphicsObject::TrackGraphicsObject(int object_id, StateGraphicsObject* state,
    TransitionGraphicsObject* transition, TrackDirection direction, QColor fill_color,
    float width, QGraphicsItem *parent)
    : PointGraphicsObject(object_id, 0.f, 0.f, fill_color, QColor::fromRgb(0, 0, 0), 1.f, parent),
      m_direction(direction), m_width(width), m_isLinkWithTransition(true), m_arrowLenght(5.f), m_arrowWidth(6.f),
      m_drawEnable(true)
{   
    m_selectionExtrude = 3.f;
    setLine(state, transition);
    setPen(QPen(QBrush(fill_color), width));
    setFlags(ItemIsSelectable | ItemIsMovable);
}

TrackGraphicsObject::TrackGraphicsObject(int object_id, StateGraphicsObject* state1, StateGraphicsObject* state2,
    TrackDirection direction, QColor fill_color, float width, QGraphicsItem *parent)
    : PointGraphicsObject(object_id, 0.f, 0.f, fill_color, QColor::fromRgb(0, 0, 0), 1.f, parent),
      m_direction(direction), m_width(width), m_isLinkWithTransition(false), m_arrowLenght(5.f), m_arrowWidth(6.f),
      m_drawEnable(true)
{
    m_selectionExtrude = 3.f;
    setLine(state1, state2);
    setPen(QPen(QBrush(fill_color), width));
    setFlags(ItemIsSelectable | ItemIsMovable);
}

TrackDirection TrackGraphicsObject::getDirection() const
{
    return m_direction;
}

void TrackGraphicsObject::setDirection(TrackDirection direction)
{
    m_direction = direction;
}

float TrackGraphicsObject::getX1() const
{
    return pos().x() + m_x1;
}

float TrackGraphicsObject::getY1() const
{
    return pos().y() + m_y1;
}

float TrackGraphicsObject::getX2() const
{
    return pos().x() + m_x2;
}

float TrackGraphicsObject::getY2() const
{
    return pos().y() + m_y2;
}

float TrackGraphicsObject::getLength() const
{
    return m_length;
}

void TrackGraphicsObject::setCenter(float x, float y)
{
    float center_x = (x < 0 ? 0.f : x);
    float center_y = (y < 0 ? 0.f : y);
    float old_center_x = pos().x() + 0.5*(m_x2 + m_x1);
    float old_center_y = pos().y() + 0.5*(m_y2 + m_y1);
    float delta_x = center_x - old_center_x;
    float delta_y = center_y - old_center_y;
    if (this->isSelected())
    {
        this->select();
    }
    else
    {
        m_boundingRect = getBoundingPolygon(3).boundingRect();
    }
    setPos(pos().x() + delta_x, pos().y() + delta_y);
}

QPointF TrackGraphicsObject::getCenter() const
{
    return QPointF(pos().x() + 0.5*(m_x2 + m_x1), pos().y() + 0.5*(m_y2 + m_y1));
}

void TrackGraphicsObject::setFillColor(QColor color)
{
    m_fillColor = color;
    setPen(QPen(QBrush(m_fillColor), m_width));
}

float TrackGraphicsObject::getWidth() const
{
    return m_width;
}

void TrackGraphicsObject::setWidth(float width)
{
    m_width = width;
    setPen(QPen(QBrush(m_fillColor), m_width));
}

QLineF TrackGraphicsObject::expandLine(float width)
{
    float delta_x = m_x1 - m_x2;
    float delta_y = m_y1 - m_y2;
    float scale_factor = width / sqrt(delta_x * delta_x + delta_y * delta_y);
    return QLineF(m_x1 + scale_factor * delta_x, m_y1 + scale_factor * delta_y,
                  m_x2 - scale_factor * delta_x, m_y2 - scale_factor * delta_y);
}

QPolygonF TrackGraphicsObject::getBoundingPolygon(float width)
{
    QPolygonF polygon;
    float dy = m_y1 - m_y2;
    if (dy == 0)
    {
        polygon << QPointF(m_x1, m_y1-width);
        polygon << QPointF(m_x1, m_y1+width);
        polygon << QPointF(m_x2, m_y2+width);
        polygon << QPointF(m_x2, m_y2-width);
    }
    else
    {
        float k_a = -(m_x1 - m_x2)/(m_y1 - m_y2);
        float b_a = m_y1 - k_a * m_x1;
        float b_c = m_y2 - k_a * m_x2;
        polygon << getPointByEquation(m_x1, m_y1, m_x1 - 1, k_a, b_a, width);
        polygon << getPointByEquation(m_x1, m_y1, m_x1 + 1, k_a, b_a, width);
        polygon << getPointByEquation(m_x2, m_y2, m_x2 + 1, k_a, b_c, width);
        polygon << getPointByEquation(m_x2, m_y2, m_x2 - 1, k_a, b_c, width);
    }
    return polygon;
}

void TrackGraphicsObject::setLine(StateGraphicsObject* state, TransitionGraphicsObject* transition)
{
    m_isLinkWithTransition = true;
    QPointF st_center = state->getCenter();
    QPointF tr_center = transition->getCenter();
    float delta_x = tr_center.x() - st_center.x();
    float delta_y = tr_center.y() - st_center.y();
    if ((delta_x == 0) && (delta_y == 0))
    {
        setPos(st_center);
        setDrawDisable();
        return;
    }
    m_length = qSqrt(delta_x * delta_x + delta_y * delta_y);
    float scale_factor1 = (state->getRadius() + 0.5 * state->getBorderWidth()) / m_length;
    m_x1 = st_center.x() + scale_factor1 * delta_x;
    m_y1 = st_center.y() + scale_factor1 * delta_y;
    setPos(m_x1, m_y1);
    float cos_alpha = delta_x / m_length;
    float sin_alpha = delta_y / m_length;
    m_angle = getAngle(sin_alpha, cos_alpha);
    QPolygonF tr_coords(transition->getVerteciesCoord());
    float angle1 = transition->getTilt();
    float angle2 = 180 - angle1;
    float angle3 = 180 + angle1;
    float angle4 = 360 - angle1;
    //qDebug() << "rot: " << transition->rotation();
    angle1 = normalizeAngle(angle1 + transition->rotation());
    angle2 = normalizeAngle(angle2 + transition->rotation());
    angle3 = normalizeAngle(angle3 + transition->rotation());
    angle4 = normalizeAngle(angle4 + transition->rotation());
    float tr_angle = getAngle(sin_alpha, cos_alpha);
    //qDebug() << "tr_angle: " << tr_angle;
    QPointF border_point1, border_point2;
    if ((tr_angle >= angle1) && (tr_angle < angle2))
    {
        // section 2
        border_point1 = tr_coords[2];
        border_point2 = tr_coords[3];
    }
    else if ((tr_angle >= angle2) && (tr_angle < angle3))
    {
        // section 3
        border_point1 = tr_coords[3];
        border_point2 = tr_coords[0];
    }
    else if ((tr_angle >= angle3) && (tr_angle < angle4))
    {
        // section 4
        border_point1 = tr_coords[0];
        border_point2 = tr_coords[1];
    }
    else
    {
        //section 1
        border_point1 = tr_coords[1];
        border_point2 = tr_coords[2];
    }
    QPointF p2 = getLineIntersection(state->getCenter(), transition->getCenter(), border_point1, border_point2);
    delta_x = p2.x() - m_x1;
    delta_y = p2.y() - m_y1;
    if ((delta_x == 0) && (delta_y == 0))
    {
        setDrawDisable();
        return;
    }
    m_length = qSqrt(delta_x * delta_x + delta_y * delta_y);
    m_x1 = 0;
    m_y1 = m_y2 = 0;
    m_x2 = m_length;
    setRotation(m_angle);
    if (this->isSelected())
    {
        this->select();
    }
    else
    {
        m_boundingRect.setRect(0, -0.5*m_arrowWidth, m_length, m_arrowWidth);
    }
}

void TrackGraphicsObject::setLine(StateGraphicsObject* state1, StateGraphicsObject* state2)
{
    m_isLinkWithTransition = false;
    QPointF st_center1 = state1->getCenter();
    QPointF st_center2 = state2->getCenter();
    float delta_x = st_center2.x() - st_center1.x();
    float delta_y = st_center2.y() - st_center1.y();
    if ((delta_x == 0) && (delta_y == 0))
    {
        setPos(st_center1);
        setDrawDisable();
        return;
    }
    m_length = qSqrt(delta_x * delta_x + delta_y * delta_y);
    float scale_factor1 = (state1->getRadius() + 0.5*state1->getBorderWidth()) / m_length;
    float scale_factor2 = (state2->getRadius() + 0.5*state2->getBorderWidth()) / m_length;
    m_x1 = st_center1.x() + scale_factor1 * delta_x;
    m_y1 = st_center1.y() + scale_factor1 * delta_y;
    m_x2 = st_center2.x() - scale_factor2 * delta_x;
    m_y2 = st_center2.y() - scale_factor2 * delta_y;
    setPos(m_x1, m_y1);
    delta_x = m_x2 - m_x1;
    delta_y = m_y2 - m_y1;
    if ((delta_x == 0) && (delta_y == 0))
    {
        setDrawDisable();
        return;
    }
    m_length = qSqrt(delta_x * delta_x + delta_y * delta_y);
    m_x1 = 0;
    m_y1 = m_y2 = 0;
    m_x2 = m_length;
    m_angle = getAngle(delta_y/m_length, delta_x/m_length);
    setRotation(m_angle);
    if (this->isSelected())
    {
        this->select();
    }
    else
    {
        m_boundingRect.setRect(0, -0.5*m_arrowWidth, m_length, m_arrowWidth);
    }
}

QRectF TrackGraphicsObject::boundingRect() const
{
    return m_boundingRect;
}

int TrackGraphicsObject::type() const
{
    return GraphicsObjectType::TrackType;
}

void TrackGraphicsObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(widget);
    QColor fillColor;
    if ((option->state & QStyle::State_Selected) && (!m_selected))
    {
        m_selected = true;
    }
    if (m_selected)
    {
        fillColor = m_fillColor.dark(200);
        //m_boundingRect.setRect(-1, -0.5*m_arrowWidth - 1, m_length + 2, m_arrowWidth + 2);
        painter->setPen(QPen(QBrush(QColor::fromRgb(0, 0, 0)), 1.f, Qt::DashLine));
        painter->drawRect(m_boundingRect);
    }
    else
    {
        //m_boundingRect.setRect(0, -0.5*m_arrowWidth, m_length, m_arrowWidth);
        fillColor = m_fillColor;
    }
    painter->setPen(QPen(QBrush(fillColor), m_width));
    painter->drawLine(m_x1, m_y1, m_x2, m_y2);
    if ((m_direction == TrackDirection::BOTH) || (m_direction == TrackDirection::FROM_FIRST_TO_SECOND))
    {
        painter->drawLine(m_x2, m_y2, m_x2 - m_arrowLenght, m_y2 + 0.5*m_arrowWidth);
        painter->drawLine(m_x2, m_y2, m_x2 - m_arrowLenght, m_y2 - 0.5*m_arrowWidth);
    }
    if ((m_direction == TrackDirection::BOTH) || (m_direction == TrackDirection::FROM_SECOND_TO_FIRST))
    {
        painter->drawLine(m_x1, m_y1, m_x1 + m_arrowLenght, m_y1 + 0.5*m_arrowWidth);
        painter->drawLine(m_x1, m_y1, m_x1 + m_arrowLenght, m_y1 - 0.5*m_arrowWidth);
    }
}

void TrackGraphicsObject::select()
{
    if (!m_selected)
    {
        m_boundingRect.setRect(-1, -0.5*m_arrowWidth - 1, m_length + 2, m_arrowWidth + 2);
        m_selected = true;
        this->setSelected(true);
        this->update();
    }
}

void TrackGraphicsObject::deselect()
{
    if (m_selected)
    {
        m_boundingRect.setRect(0, -0.5*m_arrowWidth, m_length, m_arrowWidth);
        m_selected = false;
        this->setSelected(false);
        this->update();
    }
}

QPointF TrackGraphicsObject::getPointFromScaledLine(float scale_factor)
{
    float r = scale_factor * m_length;
    return QPointF(qCos(qDegreesToRadians(m_angle)) * r, qSin(qDegreesToRadians(m_angle)) * r) + pos();
}

void TrackGraphicsObject::setDrawDisable()
{
    m_drawEnable = false;
    m_x1 = m_x2 = 0;
    m_y1 = m_y2 = 0;
    m_length = 0;
    m_angle = 0;
    setRotation(0);
}

QPointF TrackGraphicsObject::getLineIntersection(QPointF p1, QPointF p2, QPointF p3, QPointF p4)
{
    float fx = -1.f, fy = -1.f;
    if (((p1.x() == p2.x()) && (p1.y() == p2.y()))
        || ((p3.x() == p4.x()) && (p3.y() == p4.y()))
        || ((p1.x() == p2.x()) && (p3.x() == p4.x()))
        || ((p1.y() == p2.y()) && (p3.y() == p4.y()))
        )
    {
        return QPointF(fx, fy);
    }
    QPointF equation;
    if (p1.x() == p2.x())
    {
        fx = p1.x();
        if (p3.y() == p4.y())
        {
            fy = p3.y();
        }
        else
        {
            equation = getEquationByPoints(p3, p4);
            fy = equation.x() * fx + equation.y();
        }
    }
    else if (p1.y() == p2.y())
    {
        fy = p1.y();
        if (p3.x() == p4.x())
        {
            fx = p3.x();
        }
        else
        {
            equation = getEquationByPoints(p3, p4);
            fx = (fy - equation.y()) / equation.x();
        }
    }
    else
    {
        equation = getEquationByPoints(p1, p2);
        if (p3.x() == p4.x())
        {
            fx = p3.x();
            fy = equation.x() * fx + equation.y();
        }
        else if (p3.y() == p4.y())
        {
            fy = p3.y();
            fx = (fy - equation.y()) / equation.x();
        }
        else
        {
            QPointF equation2 = getEquationByPoints(p3, p4);
            fx = (equation.y() - equation2.y()) / (equation2.x() - equation.x());
            fy = equation.x() * fx + equation.y();
        }
    }
    return QPointF(fx, fy);
}
