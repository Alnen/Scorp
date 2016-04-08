#include "TrackGraphicsObject.h"
#include <QPen>
#include <QBrush>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>

TrackGraphicsObject::TrackGraphicsObject(StateGraphicsObject* state,
    TransitionGraphicsObject* transition, TrackDirection direction, QColor fill_color,
    float width, QGraphicsItem *parent)
    : PointGraphicsObject(0.f, 0.f, fill_color, QColor::fromRgb(0, 0, 0), 1.f, parent),
      m_direction(direction), m_width(width), m_isLinkWithTransition(true)
{   
    m_selectionExtrude = 2.f;
    setLine(state, transition);
    setPen(QPen(QBrush(fill_color), width));
    setFlags(ItemIsSelectable | ItemIsMovable);
}

TrackGraphicsObject::TrackGraphicsObject(StateGraphicsObject* state1, StateGraphicsObject* state2,
    TrackDirection direction, QColor fill_color, float width, QGraphicsItem *parent)
    : PointGraphicsObject(0.f, 0.f, fill_color, QColor::fromRgb(0, 0, 0), 1.f, parent),
      m_direction(direction), m_width(width), m_isLinkWithTransition(false)
{
    m_selectionExtrude = 2.f;
    setLine(state1, state2);
    setPen(QPen(QBrush(fill_color), width));
    setFlags(ItemIsSelectable | ItemIsMovable);
}

TrackDirection TrackGraphicsObject::getDirection()
{
    return m_direction;
}

void TrackGraphicsObject::setDirection(TrackDirection direction)
{
    m_direction = direction;
}

float TrackGraphicsObject::getX1()
{
    return pos().x() + m_x1;
}

float TrackGraphicsObject::getY1()
{
    return pos().y() + m_y1;
}

float TrackGraphicsObject::getX2()
{
    return pos().x() + m_x2;
}

float TrackGraphicsObject::getY2()
{
    return pos().y() + m_y2;
}

float TrackGraphicsObject::getLength()
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

QPointF TrackGraphicsObject::getCenter()
{
    return QPointF(pos().x() + 0.5*(m_x2 + m_x1), pos().y() + 0.5*(m_y2 + m_y1));
}

void TrackGraphicsObject::setFillColor(QColor color)
{
    m_fillColor = color;
    setPen(QPen(QBrush(m_fillColor), m_width));
}

float TrackGraphicsObject::getWidth()
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

QPointF TrackGraphicsObject::getPointByEquation(float x_p, float y_p,
    float x_n, float k, float b, float length)
{
    float delta_x = x_p - x_n;
    float delta_y = y_p - k * x_n - b;
    float scale_factor = length / sqrt(delta_x * delta_x + delta_y * delta_y);
    return QPointF(x_p + scale_factor * delta_x, y_p + scale_factor * delta_y);
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
    setLineLinkedTransitionWithState(state->getCenter().x(), state->getCenter().y(), state->getRadius(),
        transition->getCenter().x(), transition->getCenter().y(), transition->getWidth(), transition->getHeight());
}

void TrackGraphicsObject::setLine(StateGraphicsObject* state1, StateGraphicsObject* state2)
{
    m_isLinkWithTransition = false;
    setLineLinkedStates(state1->getCenter().x(), state1->getCenter().y(), state1->getRadius(),
                        state2->getCenter().x(), state2->getCenter().y(), state2->getRadius());
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
    if (option->state & QStyle::State_Selected)
    {
        fillColor = m_fillColor.dark(200);
        QLineF expand_line = expandLine(m_selectionExtrude+1);
        QLineF selected_line = expandLine(m_selectionExtrude);
        QLineF line(m_x1, m_y1, m_x2, m_y2);
        m_x1 = expand_line.p1().x();
        m_y1 = expand_line.p1().y();
        m_x2 = expand_line.p2().x();
        m_y2 = expand_line.p2().y();
        m_boundingRect = getBoundingPolygon(m_selectionExtrude+1).boundingRect();
        m_x1 = selected_line.p1().x();
        m_y1 = selected_line.p1().y();
        m_x2 = selected_line.p2().x();
        m_y2 = selected_line.p2().y();
        painter->setPen(QPen(QBrush(QColor::fromRgb(0, 0, 0)), 1.f, Qt::DashLine));
        painter->drawPolygon(getBoundingPolygon(m_selectionExtrude));
        m_x1 = line.p1().x();
        m_y1 = line.p1().y();
        m_x2 = line.p2().x();
        m_y2 = line.p2().y();
    }
    else
    {
        m_boundingRect = getBoundingPolygon(m_selectionExtrude).boundingRect();
        fillColor = m_fillColor;
    }
    painter->setPen(QPen(QBrush(fillColor), m_width));
    painter->drawLine(m_x1, m_y1, m_x2, m_y2);
}

void TrackGraphicsObject::select()
{
    QLineF expand_line = expandLine(m_selectionExtrude);
    QLineF line(m_x1, m_y1, m_x2, m_y2);
    m_x1 = expand_line.p1().x();
    m_y1 = expand_line.p1().y();
    m_x2 = expand_line.p2().x();
    m_y2 = expand_line.p2().y();
    m_boundingRect = getBoundingPolygon(m_selectionExtrude+1).boundingRect();
    m_x1 = line.p1().x();
    m_y1 = line.p1().y();
    m_x2 = line.p2().x();
    m_y2 = line.p2().y();
    this->setSelected(true);
    this->update();
}

void TrackGraphicsObject::setLineLinkedStates(float st_x1, float st_y1, float st_r1,
                                              float st_x2, float st_y2, float st_r2)
{
    float delta_x = st_x2 - st_x1;
    float delta_y = st_y2 - st_y1;
    m_length = sqrt(delta_x * delta_x + delta_y * delta_y);
    float scale_factor1 = st_r1 / m_length;
    float scale_factor2 = st_r2 / m_length;
    m_x1 = st_x1 + scale_factor1 * delta_x - pos().x();
    m_y1 = st_y1 + scale_factor1 * delta_y - pos().y();
    m_x2 = st_x2 - scale_factor2 * delta_x - pos().x();
    m_y2 = st_y2 - scale_factor2 * delta_y - pos().y();
    if (this->isSelected())
    {
        this->select();
    }
    else
    {
        m_boundingRect = getBoundingPolygon(m_selectionExtrude).boundingRect();
    }
}

void TrackGraphicsObject::setLineLinkedTransitionWithState(float st_x, float st_y, float st_radius,
                                                           float tr_x, float tr_y, float tr_width, float tr_height)
{
    float delta_x = tr_x - st_x;
    float delta_y = tr_y - st_y;
    m_length = sqrt(delta_x * delta_x + delta_y * delta_y);
    float scale_factor1 = st_radius / m_length;
    m_x1 = st_x + scale_factor1 * delta_x;
    m_y1 = st_y + scale_factor1 * delta_y;
    float cos_alpha = delta_x / m_length;
    float sin_alpha = delta_y / m_length;
    float tr_length = sqrt(tr_width * tr_width + tr_height * tr_height);
    float tr_cos = tr_width / tr_length;
    int part = 0;
    if (sin_alpha >= 0)
    {
        if (cos_alpha >= 0)
        {
            // 1 chapter
            part = (cos_alpha >= tr_cos ? 1 : 2);
        }
        else
        {
            // 2 chapter
            tr_cos = -tr_cos;
            part = (cos_alpha >= tr_cos ? 2 : 3);
        }
    }
    else
    {
        if (cos_alpha >= 0)
        {
            // 4 chapter
            part = (cos_alpha >= tr_cos ? 1 : 4);
        }
        else
        {
            // 3 chapter
            tr_cos = -tr_cos;
            part = (cos_alpha >= tr_cos ? 4 : 3);
        }
    }
    if (st_x == tr_x)
    {
        m_x2 = st_x;
        m_y2 = (part == 2 ? tr_y - 0.5*tr_height : tr_y + 0.5*tr_height);
    }
    else
    {
        float k = (st_y - tr_y)/(st_x - tr_x);
        float b = st_y - k * st_x;
        float a = 0;
        if (part == 1 || part == 3)
        {
            a = (part == 1 ? tr_x - 0.5*tr_width : tr_x + 0.5*tr_width);
            m_x2 = a;
            m_y2 = k * a + b;
        }
        else
        {
            a = (part == 2 ? tr_y - 0.5*tr_height : tr_y + 0.5*tr_height);
            m_x2 = (a - b)/k;
            m_y2 = a;
        }
    }
    m_x1 -= pos().x();
    m_y1 -= pos().y();
    m_x2 -= pos().x();
    m_y2 -= pos().y();
    if (this->isSelected())
    {
        this->select();
    }
    else
    {
        m_boundingRect = getBoundingPolygon(m_selectionExtrude).boundingRect();
    }
}
