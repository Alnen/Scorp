#include "../../include/Map/StateGraphicsObject.h"
#include "../../include/Map/MarkerObject.h"
#include <QPen>
#include <QPainter>
#include <QGraphicsEllipseItem>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>

#include <QDebug>

StateGraphicsObject::StateGraphicsObject(int object_id, float center_x, float center_y, float radius, QColor fill_color, QGraphicsItem* parent)
    : PointGraphicsObject(object_id, center_x, center_y, fill_color, QColor::fromRgb(0, 0, 0), 1.f, parent), m_radius(radius)
{
    m_boundingRect.setRect(-m_radius, -m_radius, 2*m_radius, 2*m_radius);
    m_selectionExtrude = 4.f;
    //this->setObjectName(QString::number(paintStateCounter));
}

float StateGraphicsObject::getRadius() const
{
    return m_radius;
}

void StateGraphicsObject::setRadius(float radius)
{
    m_radius = (radius < 0 ? -radius : radius);
}

/*
QPainterPath StateGraphicsObject::shape() const
{
    QPainterPath path;
    path.addEllipse(-m_radius, -m_radius, 2*m_radius, 2*m_radius);
    return path;
}
*/

int StateGraphicsObject::type() const
{
    return GraphicsObjectType::StateType;
}

QRectF StateGraphicsObject::boundingRect() const
{
    return m_boundingRect;
}

void StateGraphicsObject::select()
{
    if (!m_selected)
    {
        float select_radius = m_radius + m_selectionExtrude + 1;
        m_boundingRect.setRect(-select_radius, -select_radius, 2*select_radius, 2*select_radius);
        m_selected = true;
        this->setSelected(true);
        this->update();
    }
}

void StateGraphicsObject::deselect()
{
    if (m_selected)
    {
        m_boundingRect.setRect(-m_radius, -m_radius, 2*m_radius, 2*m_radius);
        m_selected = false;
        this->setSelected(false);
        this->update();
    }
}

void StateGraphicsObject::addMarker(MarkerObject* marker)
{
    for (size_t i = 0; i < m_markerList.size(); ++i)
    {
        if (m_markerList[i] == marker)
        {
            return;
        }
    }
    marker->connectToState(this);
    m_markerList.push_back(marker);
}

void StateGraphicsObject::removeMarker(MarkerObject* marker)
{
    for (size_t i = 0; i < m_markerList.size(); ++i)
    {
        if (m_markerList[i] == marker)
        {
            m_markerList.erase(m_markerList.begin() + i);
            return;
        }
    }
}

MarkerObject* StateGraphicsObject::getMarker(int marker_id)
{
    for (size_t i = 0; i < m_markerList.size(); ++i)
    {
        if (m_markerList[i]->getId() == marker_id)
        {
            return m_markerList[i];
        }
    }
    return nullptr;
}

void StateGraphicsObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    //++paintStateCounter;
    Q_UNUSED(widget);
    QColor fillColor;
    QColor borderColor;
    if ((option->state & QStyle::State_Selected) && (!m_selected))
    {
        m_selected = true;
    }
    //qDebug() << "StateGraphicsObject (" << this->objectName() << ") paint = " << paintStateCounter
    //         << "; selected = " << (m_selected ? "true" : "false");

    //if (option->state & QStyle::State_Selected)
    if (m_selected)
    {
        fillColor = m_fillColor.dark(200);
        borderColor = m_borderColor.dark(200);
        painter->setPen(QPen(QBrush(QColor::fromRgb(0, 0, 0)), 1.f, Qt::DashLine));
        float select_radius = m_radius + m_selectionExtrude;
        //m_boundingRect.setRect(-select_radius-1, -select_radius-1, 2*(select_radius + 1), 2*(select_radius + 1));
        //qDebug() << "Draw bound";
        painter->drawEllipse(-select_radius, -select_radius, 2*select_radius, 2*select_radius);
    }
    else
    {
        //m_boundingRect.setRect(-m_radius, -m_radius, 2*m_radius, 2*m_radius);
        fillColor = m_fillColor;
        borderColor = m_borderColor;
    }
    painter->setPen(QPen(QBrush(borderColor), m_borderWidth));
    painter->setBrush(QBrush(fillColor));
    painter->drawEllipse(-m_radius, -m_radius, 2*m_radius, 2*m_radius);
    // draw markers
    if (m_markerList.size() > 0)
    {
        int marker_r = 3;
        QColor marker_border_color = QColor::fromRgb(0, 0, 0);
        //m_markerList[0]->getBorderColor();
        float marker_border_width = 1.f;//m_markerList[0]->getBorderWidth();
        QColor marker_fill_color = QColor::fromRgb(0, 0, 0);
        //m_markerList[0]->getFillColor();
        painter->setPen(QPen(QBrush(marker_border_color), marker_border_width));
        painter->setBrush(QBrush(marker_fill_color));
        painter->drawEllipse(-marker_r, -marker_r, 2*marker_r, 2*marker_r);
        painter->drawText(-3, 4, QString::number(m_markerList.size()));
    }
    //m_markerList[i]
    /*
    for (size_t i = 0; i < m_markerList)
    {
        //
    }
    */
}
