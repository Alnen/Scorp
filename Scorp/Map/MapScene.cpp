#include "MapScene.h"

#include "Map/PointGraphicsObject.h"
#include "Map/StateGraphicsObject.h"
#include "Map/TransitionGraphicsObject.h"

#include <QGraphicsSceneMouseEvent>
#include <QDebug>

MapScene::MapScene(QObject *parent)
    : QGraphicsScene(parent), m_mode(MapMode::View), m_mouseButtonDown(false)
{
    m_currentStation = nullptr;
}

MapScene::MapScene(qreal x, qreal y, qreal width, qreal height, QObject *parent)
    : QGraphicsScene(x, y, width, height, parent), m_mode(MapMode::View), m_mouseButtonDown(false)
{
    m_currentStation = nullptr;
    //connect(m_currentStation, &StationObject::selectedChange, this, &MapScene::selectItem);
}

void MapScene::selectItem(PointGraphicsObject* item)
{
    item->setFillColor(QColor::fromRgb(255, 0, 0));
}

void MapScene::setMode(MapMode mode)
{
    if (m_mode != mode)
    {
        m_mode = mode;
        emit modeChanged();
    }
}

void MapScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton)
        return;
    QPointF mouse_pos = mouseEvent->scenePos();
    int mx = (int)mouse_pos.x();
    int my = (int)mouse_pos.y();
    qDebug() << "x: " << (int)mouse_pos.x() << "; y: " << (int)mouse_pos.y();
    switch (m_mode)
    {
    case MapMode::Move:
        break;
    case MapMode::AddStation:
        //StationObject* station = new StationObject(mx, my, 10);
        m_currentStation = new StateGraphicsObject(mx, my, 10);
        m_currentStation->setFillColor(QColor::fromRgb(0, 200, 0));
        m_currentStation->setBorderWidth(3.f);
        m_currentStation->setBorderColor(QColor::fromRgb(0, 0, 200));
        addItem(m_currentStation);
        break;
        /*
    case MapMode::AddTrace:
        break;
        */
        /*
    case MapMode::Delete:
        break;
        */
    }
    QGraphicsScene::mousePressEvent(mouseEvent);
}

void MapScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void MapScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}
