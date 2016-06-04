#include <QPen>
#include <QPainter>
#include <QGraphicsEllipseItem>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include "Scorp/GUI/Map/StateGraphicsObject.h"
#include "Scorp/GUI/Map/MarkerObject.h"

StateGraphicsObject::StateGraphicsObject(int object_id, float center_x, float center_y, float radius, QColor fill_color, QGraphicsItem* parent)
    : PointGraphicsObject(object_id, center_x, center_y, fill_color, QColor::fromRgb(0, 0, 0), 1.f, parent), m_radius(radius),
      m_style(fill_color, QColor::fromRgb(0, 0, 0), 1.f, radius)
{
    m_boundingRect.setRect(-m_radius, -m_radius, 2*m_radius, 2*m_radius);
    m_selectionExtrude = 4.f;
}

float StateGraphicsObject::getRadius() const
{
    return m_radius;
}

void StateGraphicsObject::setRadius(float radius)
{
    m_radius = (radius < 0 ? -radius : radius);
}

int StateGraphicsObject::type() const
{
    return GraphicsObjectType::StateType;
}

QRectF StateGraphicsObject::boundingRect() const
{
    return m_boundingRect;
}

void StateGraphicsObject::select(bool graphics_selection)
{
    if (!m_selected)
    {
        float select_radius = m_radius + m_selectionExtrude + 1;
        m_boundingRect.setRect(-select_radius, -select_radius, 2*select_radius, 2*select_radius);
        m_selected = true;
        if (graphics_selection && (!this->isSelected()))
        {
            this->setSelected(true);
        }
        /*
        m_selected = true;
        if (!this->isSelected())
        {
            this->setSelected(true);
        }
        */
        //this->update();
    }
}

void StateGraphicsObject::deselect(bool graphics_selection)
{
    if (m_selected)
    {
        m_boundingRect.setRect(-m_radius, -m_radius, 2*m_radius, 2*m_radius);
        m_selected = false;
        //if ((graphics_selection || m_parentID >=0) && (this->isSelected()))
        if (graphics_selection && (this->isSelected()))
        {
            this->setSelected(false);
        }
        /*
        if (this->isSelected())
        {
            this->setSelected(false);
        }
        */
        //this->update();
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

MarkerObject* StateGraphicsObject::getMarker(int marker_id) const
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

MarkerObject* StateGraphicsObject::getLastMarker() const
{
    if (!m_markerList.empty())
    {
        return m_markerList[m_markerList.size()-1];
    }
    return nullptr;
}

std::vector<MarkerObject*> StateGraphicsObject::getMarkers() const
{
    return m_markerList;
}

void StateGraphicsObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(widget);
    QColor fillColor;
    QColor borderColor;
    /*
    if ((option->state & QStyle::State_Selected) && (!m_selected))
    {
        m_selected = true;
    }
    */
    //qDebug() << "StateGraphicsObject (" << this->objectName() << ") paint = " << paintStateCounter
    //         << "; selected = " << (m_selected ? "true" : "false");

    if (option->state & QStyle::State_Selected)
    //if (m_selected)
    {
        fillColor = m_fillColor.dark(200);
        borderColor = m_borderColor.dark(200);
        painter->setPen(QPen(QBrush(QColor::fromRgb(0, 0, 0)), 1.f, Qt::DashLine));
        float select_radius = m_radius + m_selectionExtrude;
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
        MapSceneStyle::MarkerStyle marker_style = m_markerList[0]->getStyle();
        if (!marker_style.imagePath.isEmpty())
        {
            painter->drawImage(QRect(-marker_style.radius, -marker_style.radius, 2*marker_style.radius, 2*marker_style.radius),
                               QPixmap(marker_style.imagePath).toImage());
        }
        else
        {
            painter->setPen(QPen(QBrush(marker_style.borderColor), marker_style.borderWidth));
            painter->setBrush(QBrush(marker_style.fillColor));
            painter->drawEllipse(-marker_style.radius, -marker_style.radius, 2*marker_style.radius, 2*marker_style.radius);
        }
        painter->setPen(QPen(QBrush(QColor::fromRgb(0, 0, 0)), 1.f));
        painter->drawText(-15, 15, QString::number(m_markerList.size()));
    }
}

void StateGraphicsObject::setStyle(const MapSceneStyle::StateStyle& style)
{
    m_style = style;
    m_fillColor = m_style.fillColor;
    m_borderColor = m_style.borderColor;
    m_borderWidth = m_style.borderWidth;
    setRadius(m_radius);
}

MapSceneStyle::StateStyle StateGraphicsObject::getStyle() const
{
    return m_style;
}
