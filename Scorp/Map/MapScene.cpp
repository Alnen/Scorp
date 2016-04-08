#include "MapScene.h"
#include "Map/PointGraphicsObject.h"
#include "Map/StateGraphicsObject.h"
#include "Map/TransitionGraphicsObject.h"
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QDebug>

MapScene::MapScene(QObject *parent)
    : QGraphicsScene(parent), m_mode(MapMode::View)
{
    m_linkedState = nullptr;
    m_linkedTransition = nullptr;
}

MapScene::MapScene(qreal x, qreal y, qreal width, qreal height, QObject *parent)
    : QGraphicsScene(x, y, width, height, parent), m_mode(MapMode::View)
{
    m_linkedState = nullptr;
    m_linkedTransition = nullptr;
}

void MapScene::retainSelectedItems(const QList<QGraphicsItem*>& items_list, bool select_enable)
{
    QList<QGraphicsItem*> selected_items(items_list);
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
                if (select_enable)
                {
                    selected_state->select();
                }
                break;
            case GraphicsObjectType::TransitionType:
                selected_transition = qgraphicsitem_cast<TransitionGraphicsObject*>(selected_items[i]);
                if (!contains(m_selectedTransitions, selected_transition))
                {
                    m_selectedTransitions.push_back(selected_transition);
                }
                if (select_enable)
                {
                    selected_transition->select();
                }
                break;
            case GraphicsObjectType::TrackType:
                selected_track = qgraphicsitem_cast<TrackGraphicsObject*>(selected_items[i]);
                if (!contains(m_selectedTracks, selected_track))
                {
                    m_selectedTracks.push_back(selected_track);
                }
                if (select_enable)
                {
                    selected_track->select();
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

StateGraphicsObject* MapScene::findStateByLinkedTrack(TrackGraphicsObject* track)
{
    for (size_t i = 0; i < m_stateLinks.size(); ++i)
    {
        if (m_stateLinks[i].track == track)
        {
            return m_stateLinks[i].state;
        }
    }
    return nullptr;
}

TransitionGraphicsObject* MapScene::findTransitionByLinkedTrack(TrackGraphicsObject* track)
{
    for (size_t i = 0; i < m_transitionLinks.size(); ++i)
    {
        if (m_transitionLinks[i].track == track)
        {
            return m_transitionLinks[i].transition;
        }
    }
    return nullptr;
}

void MapScene::updateTracksPosition()
{
    retainSelectedItems(selectedItems(), false);
    TransitionGraphicsObject* transition = nullptr;
    StateGraphicsObject* state = nullptr;
    for (size_t i = 0; i < m_selectedStates.size(); ++i)
    {
        for (size_t j = 0; j < m_stateLinks.size(); ++j)
        {
            if (m_stateLinks[j].state == m_selectedStates[i])
            {
                state = m_stateLinks[j].state;
                transition = findTransitionByLinkedTrack(m_stateLinks[j].track);
                if (transition)
                {
                    m_stateLinks[j].track->setLineLinkedTransitionWithState(state->getCenter().x(),
                        state->getCenter().y(), state->getRadius(), transition->getCenter().x(),
                        transition->getCenter().y(), transition->getWidth(), transition->getHeight());
                }
                state = nullptr;
                transition = nullptr;
            }
        }
    }
    for (size_t i = 0; i < m_selectedTransitions.size(); ++i)
    {
        for (size_t j = 0; j < m_transitionLinks.size(); ++j)
        {
            if (m_transitionLinks[j].transition == m_selectedTransitions[i])
            {
                state = findStateByLinkedTrack(m_transitionLinks[j].track);
                transition = m_transitionLinks[j].transition;
                if (state)
                {
                    m_transitionLinks[j].track->setLineLinkedTransitionWithState(state->getCenter().x(),
                        state->getCenter().y(), state->getRadius(), transition->getCenter().x(),
                        transition->getCenter().y(), transition->getWidth(), transition->getHeight());
                }
                state = nullptr;
                transition = nullptr;
            }
        }
    }
    clearSelectedItems();
    this->update(this->sceneRect());
}

void MapScene::deleteSelectedItems()
{
    clearSelectedItems();
    retainSelectedItems(selectedItems(), false);
    // unlink tracks
    for (size_t i = 0; i < m_selectedStates.size(); ++i)
    {
        for (int j = 0; j < m_stateLinks.size(); ++j)
        {
            if (m_stateLinks[j].state == m_selectedStates[i])
            {
                if (!contains(m_selectedTracks, m_stateLinks[j].track))
                {
                    m_selectedTracks.push_back(m_stateLinks[j].track);
                }
                m_stateLinks[j].track = nullptr;
                m_stateLinks.erase(m_stateLinks.begin() + j);
                --j;
            }
        }
    }
    for (size_t i = 0; i < m_selectedTransitions.size(); ++i)
    {
        for (int j = 0; j < m_transitionLinks.size(); ++j)
        {
            if (m_transitionLinks[j].transition == m_selectedTransitions[i])
            {
                if (!contains(m_selectedTracks, m_transitionLinks[j].track))
                {
                    m_selectedTracks.push_back(m_transitionLinks[j].track);
                }
                m_transitionLinks[j].track = nullptr;
                m_transitionLinks.erase(m_transitionLinks.begin() + j);
                --j;
            }
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
    clearSelectedItems();
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
    //QList<QGraphicsItem*> choosen_items = items(mouse_pos);
    //qDebug() << "choosen items: " << choosen_items.count();
    //qDebug() << "selected items: " << selectedItems().count();
    retainSelectedItems(items(mouse_pos));
    if (m_mode == MapMode::Move)
    {
    }
    else if (m_mode == MapMode::AddState)
    {
        clearSelectedItems();
        unselectItems();
        StateGraphicsObject* state = new StateGraphicsObject(mouse_pos.x(), mouse_pos.y(), 10);
        state->setFillColor(QColor::fromRgb(0, 200, 0));
        state->setBorderWidth(3.f);
        state->setBorderColor(QColor::fromRgb(0, 0, 200));
        //state->setPos(mouseEvent->screenPos());
        addItem(state);
        m_selectedStates.push_back(state);
        state->select();
        //qDebug() << "Add state";
        //qDebug() << "m_selectedStates items: " << m_selectedStates.size();
    }
    else if (m_mode == MapMode::AddTransition)
    {
        clearSelectedItems();
        unselectItems();
        m_selectedTransitions.clear();
        TransitionGraphicsObject* transition = new TransitionGraphicsObject(mouse_pos.x(), mouse_pos.y(), 5, 20);
        addItem(transition);
        m_selectedTransitions.push_back(transition);
        transition->select();
        //qDebug() << "Add transition";
        //qDebug() << "m_selectedTransitions items: " << m_selectedTransitions.size();
    }
    else if (m_mode == MapMode::AddTrack)
    {
        TrackGraphicsObject* track;
        /*
        qDebug() << "Select: states = " << m_selectedStates.size()
                 << ", transitions = " << m_selectedTransitions.size()
                 << ", tracks = " << m_selectedTracks.size();
        */
        m_selectedTracks.clear();
        if ((m_selectedStates.empty()) && (m_selectedTransitions.size() == 1))
        {
            if ((!m_linkedState) && (!m_linkedTransition))
            {
                m_linkedTransition = m_selectedTransitions[0];
            }
            else if (m_linkedState)
            {
                track = new TrackGraphicsObject(m_linkedState, m_selectedTransitions[0]);
                track->setFillColor(QColor::fromRgb(200, 0, 200));
                m_stateLinks.push_back(StateTrackLink(m_linkedState, track));
                m_transitionLinks.push_back(TransitionTrackLink(m_selectedTransitions[0], track));
                addItem(track);
                m_selectedTracks.push_back(track);
                m_linkedState = nullptr;
                m_linkedTransition = nullptr;
                track->select();
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
                track->setFillColor(QColor::fromRgb(200, 0, 200));
                m_stateLinks.push_back(StateTrackLink(m_selectedStates[0], track));
                m_transitionLinks.push_back(TransitionTrackLink(m_linkedTransition, track));
                addItem(track);
                m_selectedTracks.push_back(track);
                m_linkedState = nullptr;
                m_linkedTransition = nullptr;
                track->select();
            }
        }
    }
    else if (m_mode == MapMode::Delete)
    {
    }
    clearSelectedItems();
    this->update(this->sceneRect());
    QGraphicsScene::mousePressEvent(mouseEvent);
}

void MapScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (m_mode == MapMode::Move)
    {
        if (selectedItems().count() > 0)
        {
            updateTracksPosition();
        }
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}

void MapScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (m_mode == MapMode::Move)
    {
        updateTracksPosition();
    }
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void MapScene::keyPressEvent(QKeyEvent *event)
{
    if (event->modifiers() & Qt::ControlModifier)
    {
        switch (event->key())
        {
        case Qt::Key_D:
            if (m_mode == MapMode::Delete)
            {
                deleteSelectedItems();
            }
            break;
        }
    }
    else
    {
        switch (event->key())
        {
        case Qt::Key_D:
            this->unselectItems();
            this->update(this->sceneRect());
            break;
        case Qt::Key_Delete:
            if (m_mode == MapMode::Delete)
            {
                deleteSelectedItems();
            }
            break;
        }
    }
    QGraphicsScene::keyPressEvent(event);
}
