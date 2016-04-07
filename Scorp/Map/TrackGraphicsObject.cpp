#include "TrackGraphicsObject.h"
#include <QPen>
#include <QBrush>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>

TrackGraphicsObject::TrackGraphicsObject(TrackDirection direction, QColor fill_color,
    float width, QGraphicsItem *parent)
    : PointGraphicsObject(0.f, 0.f, fill_color, QColor::fromRgb(0, 0, 0), 1.f, parent),
      m_direction(direction), m_width(width)
{
    setPen(QPen(QBrush(fill_color), width));
    setFlags(ItemIsSelectable | ItemIsMovable);
}

TrackGraphicsObject::TrackGraphicsObject(StateGraphicsObject* state,
    TransitionGraphicsObject* transition, TrackDirection direction, QColor fill_color,
    float width, QGraphicsItem *parent)
    : PointGraphicsObject(0.f, 0.f, fill_color, QColor::fromRgb(0, 0, 0), 1.f, parent),
      m_direction(direction), m_width(width)
{   
    setLine(state, transition);
    setPen(QPen(QBrush(fill_color), width));
    setFlags(ItemIsSelectable | ItemIsMovable);
}

TrackGraphicsObject::TrackGraphicsObject(StateGraphicsObject* state1, StateGraphicsObject* state2,
    TrackDirection direction, QColor fill_color, float width, QGraphicsItem *parent)
    : PointGraphicsObject(0.f, 0.f, fill_color, QColor::fromRgb(0, 0, 0), 1.f, parent),
      m_direction(direction), m_width(width)
{
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
    return m_x1;
}

float TrackGraphicsObject::getY1()
{
    return m_y1;
}

float TrackGraphicsObject::getX2()
{
    return m_x2;
}

float TrackGraphicsObject::getY2()
{
    return m_y2;
}

float TrackGraphicsObject::getLength()
{
    return m_length;
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
    float delta_x = transition->getCenterX() - state->getCenterX();
    float delta_y = transition->getCenterY() - state->getCenterY();
    m_length = sqrt(delta_x * delta_x + delta_y * delta_y);
    float scale_factor1 = state->getRadius() / m_length;
    m_x1 = state->getCenterX() + scale_factor1 * delta_x;
    m_y1 = state->getCenterY() + scale_factor1 * delta_y;
    float cos_alpha = delta_x / m_length;
    float sin_alpha = delta_y / m_length;
    float tr_width = transition->getWidth();
    float tr_height = transition->getHeight();
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
    float st_x = state->getCenterX();
    float st_y = state->getCenterY();
    float tr_x = transition->getCenterX();
    float tr_y = transition->getCenterY();
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
    m_centerX = 0.5*(m_x2 + m_x1);
    m_centerY = 0.5*(m_y2 + m_y1);
    m_boundingRect = getBoundingPolygon(3).boundingRect();
    //m_selectedPolygon = getBoundingPolygon(3);
    //QGraphicsLineItem::setLine(m_x1, m_y1, m_x2, m_y2);
}

void TrackGraphicsObject::setLine(StateGraphicsObject* state1, StateGraphicsObject* state2)
{
    float delta_x = state2->getCenterX() - state1->getCenterX();
    float delta_y = state2->getCenterY() - state1->getCenterY();
    m_length = sqrt(delta_x * delta_x + delta_y * delta_y);
    float scale_factor1 = state1->getRadius() / m_length;
    float scale_factor2 = state2->getRadius() / m_length;
    m_x1 = state1->getCenterX() + scale_factor1 * delta_x;
    m_y1 = state1->getCenterY() + scale_factor1 * delta_y;
    m_x2 = state2->getCenterX() - scale_factor2 * delta_x;
    m_y2 = state2->getCenterY() - scale_factor2 * delta_y;
    m_centerX = 0.5*(m_x2 + m_x1);
    m_centerY = 0.5*(m_y2 + m_y1);
    m_boundingRect = getBoundingPolygon(3).boundingRect();
    //m_selectedPolygon = getBoundingPolygon(3);
}

QRectF TrackGraphicsObject::boundingRect() const
{
    return m_boundingRect;//m_selectedPolygon.boundingRect();
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
        QLineF expand_line = expandLine(2);
        QLineF selected_line = expandLine(1);
        QLineF line(m_x1, m_y1, m_x2, m_y2);
        m_x1 = expand_line.p1().x();
        m_y1 = expand_line.p1().y();
        m_x2 = expand_line.p2().x();
        m_y2 = expand_line.p2().y();
        m_boundingRect = getBoundingPolygon(4).boundingRect();
        m_x1 = selected_line.p1().x();
        m_y1 = selected_line.p1().y();
        m_x2 = selected_line.p2().x();
        m_y2 = selected_line.p2().y();
        painter->setPen(QPen(QBrush(QColor::fromRgb(0, 0, 0)), 1.f, Qt::DashLine));
        painter->drawPolygon(getBoundingPolygon(3));
        m_x1 = line.p1().x();
        m_y1 = line.p1().y();
        m_x2 = line.p2().x();
        m_y2 = line.p2().y();
    }
    else
    {
        m_boundingRect = getBoundingPolygon(3).boundingRect();
        fillColor = m_fillColor;
    }
    painter->setPen(QPen(QBrush(fillColor), m_width));
    painter->drawLine(m_x1, m_y1, m_x2, m_y2);
}

void TrackGraphicsObject::select()
{
    QLineF expand_line = expandLine(2);
    QLineF line(m_x1, m_y1, m_x2, m_y2);
    m_x1 = expand_line.p1().x();
    m_y1 = expand_line.p1().y();
    m_x2 = expand_line.p2().x();
    m_y2 = expand_line.p2().y();
    m_boundingRect = getBoundingPolygon(4).boundingRect();
    m_x1 = line.p1().x();
    m_y1 = line.p1().y();
    m_x2 = line.p2().x();
    m_y2 = line.p2().y();
    this->setSelected(true);
    this->update();
}
