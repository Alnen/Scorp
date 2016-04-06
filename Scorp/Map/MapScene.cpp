#include "MapScene.h"

#include "Map/PointGraphicsObject.h"
#include "Map/StateGraphicsObject.h"
#include "Map/TransitionGraphicsObject.h"

#include <QGraphicsSceneMouseEvent>
#include <QDebug>

MapScene::MapScene(QObject *parent)
    : QGraphicsScene(parent), m_mode(MapMode::View)
{
    //m_createdTrack = nullptr;
    m_linkedState = nullptr;
    m_linkedTransition = nullptr;
}

MapScene::MapScene(qreal x, qreal y, qreal width, qreal height, QObject *parent)
    : QGraphicsScene(x, y, width, height, parent), m_mode(MapMode::View)
{
    //m_createdTrack = nullptr;
    m_linkedState = nullptr;
    m_linkedTransition = nullptr;
}

void MapScene::retainSelectedItems()
{
    QList<QGraphicsItem*> selected_items(this->selectedItems());
    if (!selected_items.empty())
    {
        StateGraphicsObject* selected_state;
        TransitionGraphicsObject* selected_transition;
        TrackGraphicsObject* selected_track;
        for (int i = 0; i < selected_items.count(); ++i)
        {
            switch (selected_items[i]->type())
            {
            case GraphicsObjectType::StateType:
                selected_state = qgraphicsitem_cast<StateGraphicsObject*>(selected_items[i]);
                if (!contains(m_selectedStates, selected_state))
                {
                    m_selectedStates.push_back(selected_state);
                }
                break;
            case GraphicsObjectType::TransitionType:
                selected_transition = qgraphicsitem_cast<TransitionGraphicsObject*>(selected_items[i]);
                if (!contains(m_selectedTransitions, selected_transition))
                {
                    m_selectedTransitions.push_back(selected_transition);
                }
                break;
            case GraphicsObjectType::TrackType:
                selected_track = qgraphicsitem_cast<TrackGraphicsObject*>(selected_items[i]);
                if (!contains(m_selectedTracks, selected_track))
                {
                    m_selectedTracks.push_back(selected_track);
                }
                break;
            }
        }
    }
}

void MapScene::clearSelectedItems()
{
    m_selectedStates.clear();
    m_selectedTransitions.clear();
    m_selectedTracks.clear();
}

void MapScene::unselectItems()
{
    QList<QGraphicsItem*> selected_items(this->selectedItems());
    for (int i = 0; i < selected_items.size(); ++i)
    {
        selected_items[i]->setSelected(false);
    }
}

template <class T>
bool MapScene::contains(const std::vector<T>& container, T value)
{
    for (size_t i = 0; i < container.size(); ++i)
    {
        if (container[i] == value)
        {
            return true;
        }
    }
    return false;
}

void MapScene::selectItem(PointGraphicsObject* item)
{
    item->setFillColor(QColor::fromRgb(255, 0, 0));
}

void MapScene::setMode(MapMode mode)
{
    m_linkedState = nullptr;
    m_linkedTransition = nullptr;
    if (m_mode != mode)
    {
        m_mode = mode;
        unselectItems();
        clearSelectedItems();
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
    //qDebug() << "x: " << (int)mouse_pos.x() << "; y: " << (int)mouse_pos.y();
    retainSelectedItems();
    if (m_mode == MapMode::Move)
    {
        //retainSelectedItems();
    }
    else if (m_mode == MapMode::AddState)
    {
        StateGraphicsObject* station = new StateGraphicsObject(mx, my, 10);
        station->setFillColor(QColor::fromRgb(0, 200, 0));
        station->setBorderWidth(3.f);
        station->setBorderColor(QColor::fromRgb(0, 0, 200));
        addItem(station);
    }
    else if (m_mode == MapMode::AddTransition)
    {
        TransitionGraphicsObject* transition = new TransitionGraphicsObject(mx, my, 5, 20);
        addItem(transition);
    }
    else if (m_mode == MapMode::AddTrack)
    {
        TrackGraphicsObject* track;
        //retainSelectedItems();
        qDebug() << "Select: states = " << m_selectedStates.size()
                 << ", transitions = " << m_selectedTransitions.size()
                 << ", tracks = " << m_selectedTracks.size();
        if (m_selectedTracks.empty())
        {
            if ((m_selectedStates.empty()) && (m_selectedTransitions.size() == 1))
            {
                if ((!m_linkedState) && (!m_linkedTransition))
                {
                    m_linkedTransition = m_selectedTransitions[0];
                }
                else if (m_linkedState)
                {
                    track = new TrackGraphicsObject(m_linkedState, m_selectedTransitions[0]);
                    track->setColor(QColor::fromRgb(200, 0, 200));
                    m_linkedState->linkedTracks.push_back(track);
                    m_selectedTransitions[0]->linkedTracks.push_back(track);
                    addItem(track);
                    m_linkedState = nullptr;
                    m_linkedTransition = nullptr;
                    emit itemsUpdated();
                }
            }
            else if ((m_selectedStates.size() == 1) && (m_selectedTransitions.empty()))
            {
                if ((!m_linkedState) && (!m_linkedTransition))
                {
                    m_linkedState = m_selectedStates[0];
                }
                else if (m_linkedTransition)
                {
                    track = new TrackGraphicsObject(m_selectedStates[0], m_linkedTransition);
                    track->setColor(QColor::fromRgb(200, 0, 200));
                    m_linkedTransition->linkedTracks.push_back(track);
                    m_selectedStates[0]->linkedTracks.push_back(track);
                    addItem(track);
                    m_linkedState = nullptr;
                    m_linkedTransition = nullptr;
                    emit itemsUpdated();
                }
            }
        }
    }
    else if (m_mode == MapMode::Delete)
    {
        //retainSelectedItems();
        // unlink tracks
        for (size_t i = 0; i < m_selectedStates.size(); ++i)
        {
            for (int j = m_selectedStates[i]->linkedTracks.size() - 1; j >= 0; --j)
            {
                if (!contains(m_selectedTracks, m_selectedStates[i]->linkedTracks[j]))
                {
                    m_selectedTracks.push_back(m_selectedStates[i]->linkedTracks[j]);
                }
                m_selectedStates[i]->linkedTracks.pop_back();
            }
        }
        for (size_t i = 0; i < m_selectedTransitions.size(); ++i)
        {
            for (int j = m_selectedTransitions[i]->linkedTracks.size() - 1; j >= 0; --j)
            {
                if (!contains(m_selectedTracks, m_selectedTransitions[i]->linkedTracks[j]))
                {
                    m_selectedTracks.push_back(m_selectedTransitions[i]->linkedTracks[j]);
                }
                m_selectedTransitions[i]->linkedTracks.pop_back();
            }
        }
        // delete tracks, states, transitions
        while (!m_selectedTracks.empty())
        {
            this->removeItem(m_selectedTracks[m_selectedTracks.size() - 1]);
            m_selectedTracks[m_selectedTracks.size() - 1] = nullptr;
            m_selectedTracks.pop_back();
        }
        while (!m_selectedStates.empty())
        {
            this->removeItem(m_selectedStates[m_selectedStates.size() - 1]);
            m_selectedStates[m_selectedStates.size() - 1] = nullptr;
            m_selectedStates.pop_back();
        }
        while (!m_selectedTransitions.empty())
        {
            this->removeItem(m_selectedTransitions[m_selectedTransitions.size() - 1]);
            m_selectedTransitions[m_selectedTransitions.size() - 1] = nullptr;
            m_selectedTransitions.pop_back();
        }
        emit itemsUpdated();
    }
    clearSelectedItems();
    QGraphicsScene::mousePressEvent(mouseEvent);
}

void MapScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (m_mode == MapMode::Move)
    {
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}

void MapScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    //emit itemsUpdated();
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}
