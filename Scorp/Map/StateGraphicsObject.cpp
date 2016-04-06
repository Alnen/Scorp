#include "StateGraphicsObject.h"
#include <QPen>
#include <QPainter>
#include <QGraphicsEllipseItem>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>

StateGraphicsObject::StateGraphicsObject(float center_x, float center_y, float radius, QColor fill_color, QGraphicsItem* parent)
    : PointGraphicsObject(center_x, center_y, fill_color, QColor::fromRgb(0, 0, 0), 1.f, parent), m_radius(radius)
{
    m_isSelected = false;
    m_boundingRect.setRect(m_centerX - m_radius, m_centerY - m_radius, 2*m_radius, 2*m_radius);
}

float StateGraphicsObject::getRadius()
{
    return m_radius;
}

void StateGraphicsObject::setRadius(float radius)
{
    if (radius < 0) m_radius = -radius;
    else m_radius = radius;
}

/*
void StateGraphicsObject::linkTrack(TrackGraphicsObject* track)
{
    for (size_t i = 0; i < m_linkedTracks.size(); ++i)
    {
        if (m_linkedTracks[i] == track)
        {
            return;
        }
    }
    m_linkedTracks.push_back(track);
}

void StateGraphicsObject::unlinkTrack(TrackGraphicsObject* track)
{
    for (size_t i = 0; i < m_linkedTracks.size(); ++i)
    {
        if (m_linkedTracks[i] == track)
        {
            m_linkedTracks.erase(m_linkedTracks.begin() + i);
            return;
        }
    }
}

TrackGraphicsObject* StateGraphicsObject::firstLink()
{
    if (m_linkedTracks.empty())
    {
        return nullptr;
    }
    else
    {
        return m_linkedTracks[0];
    }
}

TrackGraphicsObject* StateGraphicsObject::lastLink()
{
    if (m_linkedTracks.empty())
    {
        return nullptr;
    }
    else
    {
        return m_linkedTracks[m_linkedTracks.size() - 1];
    }
}

int StateGraphicsObject::linkedTracksCount()
{
    return m_linkedTracks.size();
}

std::vector<TrackGraphicsObject*> StateGraphicsObject::getLinkedTracks()
{
    return m_linkedTracks;
}
*/

QPainterPath StateGraphicsObject::shape() const
{
    QPainterPath path;
    /*
    path.addEllipse(m_centerX - m_radius - 8, m_centerY - m_radius - 8,
                     2*(m_radius + 4), 2*(m_radius + 4));
    */
    path.addEllipse(m_centerX - m_radius, m_centerY - m_radius, 2*m_radius, 2*m_radius);
    return path;
}

int StateGraphicsObject::type() const
{
    return GraphicsObjectType::StateType;
}

QRectF StateGraphicsObject::boundingRect() const
{
    /*
    if (m_isSelected)
    {
        int select_radius = m_radius + 4;
        return QRectF(m_centerX - select_radius, m_centerY - select_radius, 2*select_radius, 2*select_radius);
    }
    */
    return m_boundingRect;
    /*
    return QRectF(m_centerX - m_radius - 8, m_centerY - m_radius - 8,
                  2*(m_radius + 4), 2*(m_radius + 4));
    */
}

QVariant StateGraphicsObject::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemSelectedHasChanged)
    {
        //emit selectedChange(this);
    }
    return value;
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
        m_isSelected = true;
        painter->setPen(QPen(QBrush(QColor::fromRgb(0, 0, 0)), 1.f, Qt::DashLine));
        int select_radius = m_radius + 4;
        m_boundingRect.setRect(m_centerX - (select_radius + 1), m_centerY - (select_radius + 1),
                               2*(select_radius + 1), 2*(select_radius + 1));
        painter->drawEllipse(m_centerX - select_radius, m_centerY - select_radius,
                             2*select_radius, 2*select_radius);
    }
    else
    {
        m_boundingRect.setRect(m_centerX - m_radius, m_centerY - m_radius, 2*m_radius, 2*m_radius);
        fillColor = m_fillColor;
        borderColor = m_borderColor;
        m_isSelected = false;
    }
    painter->setPen(QPen(QBrush(borderColor), m_borderWidth));
    painter->setBrush(fillColor);
    painter->drawEllipse(m_centerX - m_radius, m_centerY - m_radius, 2*m_radius, 2*m_radius);
}

void StateGraphicsObject::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
    update();
}

void StateGraphicsObject::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->modifiers() & Qt::ShiftModifier) {
        //stuff << event->pos();
        update();
        return;
    }
    QGraphicsItem::mouseMoveEvent(event);
}

void StateGraphicsObject::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    update();
}
