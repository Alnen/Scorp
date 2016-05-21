#include "../../include/Map/MapScene.h"
#include "../../include/Map/StateGraphicsObject.h"
#include "../../include/Map/LinkGraphicsObject.h"
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QDebug>
#include "../../include/Map/MarkerObject.h"

int state_id_generator = 0;

MapScene::MapScene(QObject *parent)
    : QGraphicsScene(parent), m_mode(MapMode::View), m_linkedState(nullptr)
{
    new_state_id = 0;
    new_link_id = 0;
    connect(this, &MapScene::selectionChanged, this, &MapScene::updateSelectionItems);
    //m_petriNet = std::unique_ptr<RailwayPetriNet>(new RailwayPetriNet());
}

MapScene::MapScene(qreal x, qreal y, qreal width, qreal height, QObject *parent)
    : QGraphicsScene(x, y, width, height, parent), m_mode(MapMode::View), m_linkedState(nullptr)
{
    new_state_id = 0;
    new_link_id = 0;
    connect(this, &MapScene::selectionChanged, this, &MapScene::updateSelectionItems);
    //m_petriNet = std::unique_ptr<RailwayPetriNet>(new RailwayPetriNet());
}

bool MapScene::contains(PointGraphicsObject* item) const
{
    QList<QGraphicsItem*> all_items(this->items());
    for (size_t i = 0; i < all_items.size(); ++i)
    {
        if (all_items[i] == item)
        {
            return true;
        }
    }
    return false;
}

/*
void MapScene::addMarkerCommand(int id, int state_id)
{
    m_markerCommandQueue.push_back(MarkerCommandStruct(MarkerCommand::ADD, id, state_id));
}

void MapScene::moveMarkerCommand(int id, int new_state_id)
{
    m_markerCommandQueue.push_back(MarkerCommandStruct(MarkerCommand::MOVE, id, new_state_id));
}

void MapScene::deleteMarkerCommand(int id)
{
    m_markerCommandQueue.push_back(MarkerCommandStruct(MarkerCommand::DELETE, id));
}

void MapScene::makeCommand()
{
    if (m_markerCommandQueue.size() > 0)
    {
        StateGraphicsObject* curr_state = nullptr;
        if (m_markerCommandQueue[0].command == MarkerCommand::ADD)
        {
            for (auto item : this->items())
            {
                if (((PointGraphicsObject*)item)->type() == GraphicsObjectType::StateType)
                {
                    curr_state = (StateGraphicsObject*)item;
                    if (curr_state->getId() == m_markerCommandQueue[0].param2)
                    {
                        curr_state->addMarker(new MarkerObject(m_markerCommandQueue[0].param1,
                                              TRAIN_COLOR, curr_state));
                        break;
                    }
                }
            }
            curr_state = nullptr;
        }
        else if (m_markerCommandQueue[0].command == MarkerCommand::DELETE)
        {
            MarkerObject* found_marker = nullptr;
            for (auto item : this->items())
            {
                if (((PointGraphicsObject*)item)->type() == GraphicsObjectType::StateType)
                {
                    curr_state = (StateGraphicsObject*)item;
                    found_marker = curr_state->getMarker(m_markerCommandQueue[0].param1);
                    if (found_marker)
                    {
                        curr_state->removeMarker(found_marker);
                        delete found_marker;
                        break;
                    }
                }
            }
        }
        else if (m_markerCommandQueue[0].command == MarkerCommand::MOVE)
        {
            MarkerObject* found_marker = nullptr;
            StateGraphicsObject* old_state = nullptr;
            StateGraphicsObject* new_state = nullptr;
            for (auto item : this->items())
            {
                if (((PointGraphicsObject*)item)->type() == GraphicsObjectType::StateType)
                {
                    curr_state = (StateGraphicsObject*)item;
                    found_marker = curr_state->getMarker(m_markerCommandQueue[0].param1);
                    if (found_marker)
                    {
                        old_state = curr_state;
                    }
                    if (curr_state->getId() == m_markerCommandQueue[0].param2)
                    {
                        new_state = curr_state;
                    }
                    if (old_state && new_state)
                    {
                        break;
                    }
                }
            }
            if (old_state && new_state)
            {
                old_state->removeMarker(found_marker);
                new_state->addMarker(found_marker);
            }
        }
        m_markerCommandQueue.erase(m_markerCommandQueue.begin());
    }
}
*/

void MapScene::addLinkToScene(int index)
{
    if ((index < 0) || (index >= m_links.size()))
    {
        return;
    }
    auto link_parts = m_links[index].getLinkParts(true);
    for (size_t i = 0; i < link_parts.size(); ++i)
    {
        if (i != 2)
        {
            addItem(link_parts[i]);
        }
    }
}

void MapScene::removeLinkFromScene(int index)
{
    if ((index < 0) || (index >= m_links.size()))
    {
        return;
    }
    auto link_parts = m_links[index].getLinkParts(true);
    for (size_t i = 0; i < link_parts.size(); ++i)
    {
        removeItem(link_parts[i]);
    }
}

StateGraphicsObject* MapScene::getSecondLinkedState(int link_id, StateGraphicsObject* first_state)
{
    for (size_t i = 0; i < m_stateLinks.size(); ++i)
    {
        if ((m_stateLinks[i].link == link_id) && (m_stateLinks[i].state != first_state))
        {
            return m_stateLinks[i].state;
        }
    }
    return nullptr;
}

void MapScene::unlinkStates(int link_id)
{
    bool first_found = false;
    for (int i = 0; i < m_stateLinks.size(); ++i)
    {
        if (m_stateLinks[i].link == link_id)
        {
            m_stateLinks.erase(m_stateLinks.begin() + i);
            if (first_found)
            {
                break;
            }
            else
            {
                first_found = true;
            }
            --i;
        }
    }
}

int MapScene::findLinkIndex(int id)
{
    for (size_t i = 0; i < m_links.size(); ++i)
    {
        if (m_links[i].getID() == id)
        {
            return i;
        }
    }
    return -1;
}

void MapScene::retainSelectedItems()
{
    QList<QGraphicsItem*> selected_items(this->selectedItems());
    int parent_id = -1;
    StateGraphicsObject* state_item = nullptr;
    bool is_found = false;
    for (size_t i = 0; i < selected_items.size(); ++i)
    {
        if ((selected_items[i]->type() == GraphicsObjectType::StateType) &&
            (((StateGraphicsObject*)selected_items[i])->parentID() < 0))
        {
            state_item = (StateGraphicsObject*)selected_items[i];
            is_found = false;
            for (int i = 0; i < m_selectedStates.size(); ++i)
            {
                if (m_selectedStates[i]->getId() == state_item->getId())
                {
                    is_found = true;
                    break;
                }
            }
            if (!is_found)
            {
                m_selectedStates.push_back(state_item);
            }
            state_item = nullptr;
        }
        else
        {
            parent_id = ((PointGraphicsObject*)selected_items[i])->parentID();
            if (!contains(m_selectedLinks, parent_id))
            {
                m_selectedLinks.push_back(parent_id);
            }
        }
    }
}

void MapScene::clearSelectedItems()
{
    m_selectedStates.clear();
    m_selectedLinks.clear();
}

void MapScene::unselectItems()
{
    disconnect(this, &MapScene::selectionChanged, this, &MapScene::updateSelectionItems);
    StateGraphicsObject* selected_state = nullptr;
    int selected_link = -1;
    for (auto elem : this->items())
    {
        if (elem == nullptr)
        {
            continue;
        }
        if ((elem->type() == GraphicsObjectType::StateType) &&
            (((StateGraphicsObject*)elem)->parentID() < 0))
        {
            selected_state = (StateGraphicsObject*)elem;
            if (selected_state->selectEnable())
            {
                selected_state->deselect();
            }
            selected_state = nullptr;
        }
        else
        {
            selected_link = findLinkIndex(((PointGraphicsObject*)elem)->parentID());
            if ((selected_link >= 0) && m_links[selected_link].selectEnable())
            {
                m_links[selected_link].deselect();
            }
        }
    }
    connect(this, &MapScene::selectionChanged, this, &MapScene::updateSelectionItems);
    this->update(this->sceneRect());
}

template <class T>
bool MapScene::contains(const std::vector<T>& container, T value) const
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

void MapScene::deleteSelectedItems()
{
    clearSelectedItems();
    retainSelectedItems();
    // unlink states
    for (int i = 0; i < m_selectedLinks.size(); ++i)
    {
        unlinkStates(m_selectedLinks[i]);
    }
    for (size_t i = 0; i < m_selectedStates.size(); ++i)
    {
        for (int j = 0; j < m_stateLinks.size(); ++j)
        {
            if (m_stateLinks[j].state == m_selectedStates[i])
            {
                if (!contains(m_selectedLinks, m_stateLinks[j].link))
                {
                    m_selectedLinks.push_back(m_stateLinks[j].link);
                }
                unlinkStates(m_stateLinks[j].link);
                j = -1;
            }
        }
    }
    // delete links, states
    for (size_t i = 0; i < m_selectedLinks.size(); ++i)
    {
        removeLinkFromScene(findLinkIndex(m_selectedLinks[i]));
    }
    for (size_t i = 0; i < m_selectedStates.size(); ++i)
    {
        this->removeItem(m_selectedStates[i]);
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
    /*
    QList<QGraphicsItem*> selected_items(this->items(mouse_pos));
    retainSelectedItems(items(mouse_pos));
    if (selected_items.empty())
    {
        unselectItems();
    }
    */
    if (m_mode == MapMode::AddState)
    {
        createNewState((int)mouse_pos.x(), (int)mouse_pos.y());
        /*
        StateGraphicsObject* state = new StateGraphicsObject(++new_state_id, mouse_pos.x(), mouse_pos.y(), 10);
        state->setFillColor(QColor::fromRgb(0, 200, 0));
        state->setBorderWidth(3.f);
        state->setBorderColor(QColor::fromRgb(0, 0, 200));
        //MarkerObject* marker = new MarkerObject(0, TRAIN_COLOR, state);
        //state->addMarker(marker);
        addItem(state);
        //unselectItems();
        //clearSelectedItems();
        //state->select();
        */
    }
    else if (m_mode == MapMode::AddLink)
    {
        if (m_selectedLinks.empty() && (m_selectedStates.size() == 1))
        {
            if (m_linkedState)
            {
                // generate transition
                //unselectItems();
                createNewLink(m_linkedState, m_selectedStates[0]);
                m_linkedState = nullptr;
                //this->update(this->sceneRect());
            }
            else
            {
                m_linkedState = m_selectedStates[0];
            }
        }
    }
    QGraphicsScene::mousePressEvent(mouseEvent);
}

void MapScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (m_mode == MapMode::Move)
    {
        if (selectedItems().count() > 0)
        {
            updateObjectsPosition();
        }
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}

void MapScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (m_mode == MapMode::Move)
    {
        updateObjectsPosition();
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
        case Qt::Key_Escape:
            if (m_mode == MapMode::AddLink)
            {
                unselectItems();
                clearSelectedItems();
                m_linkedState = nullptr;
                this->update(this->sceneRect());
            }
            break;
        }
    }
    QGraphicsScene::keyPressEvent(event);
}

void MapScene::updateSelectionItems()
{
    qDebug() << "selectedItems: " << selectedItems().count();
    disconnect(this, &MapScene::selectionChanged, this, &MapScene::updateSelectionItems);
    unselectItems();
    clearSelectedItems();
    retainSelectedItems();
    qDebug() << "Selection Changed >> states: " << m_selectedStates.size() << "; links: " << m_selectedLinks.size();
    for (int i = 0; i < m_selectedStates.size(); ++i)
    {
        m_selectedStates[i]->select();
    }
    int link_index = -1;
    for (int i = 0; i < m_selectedLinks.size(); ++i)
    {
        link_index = findLinkIndex(m_selectedLinks[i]);
        if (link_index >= 0)
        {
            m_links[link_index].select();
        }
    }
    this->update(this->sceneRect());
    connect(this, &MapScene::selectionChanged, this, &MapScene::updateSelectionItems);
}

void MapScene::updateLinksPosition(const std::vector<StateGraphicsObject*>& moved_states)
{
    StateGraphicsObject* state = nullptr;
    for (size_t i = 0; i < moved_states.size(); ++i)
    {
        for (size_t j = 0; j < m_stateLinks.size(); ++j)
        {
            if (m_stateLinks[j].state == moved_states[i])
            {
                state = getSecondLinkedState(m_stateLinks[j].link, m_stateLinks[j].state);
                if (state)
                {
                    if (m_stateLinks[j].is_first)
                    {
                        m_links[findLinkIndex(m_stateLinks[j].link)].setPosition(m_stateLinks[j].state, state);
                    }
                    else
                    {
                        m_links[findLinkIndex(m_stateLinks[j].link)].setPosition(state, m_stateLinks[j].state);
                    }
                }
            }
        }
    }
}

void MapScene::updateStatesPosition()
{
    StateGraphicsObject* state1 = nullptr;
    StateGraphicsObject* state2 = nullptr;
    QPointF point;
    int link_index = -1;
    std::vector<StateGraphicsObject*> moved_states;
    for (size_t i = 0; i < m_selectedLinks.size(); ++i)
    {
        link_index = findLinkIndex(m_selectedLinks[i]);
        for (size_t j = 0; j < m_stateLinks.size(); ++j)
        {
            if (m_stateLinks[j].link == m_selectedLinks[i])
            {
                if (m_stateLinks[j].is_first)
                {
                    state1 = m_stateLinks[j].state;
                }
                else
                {
                    state2 = m_stateLinks[j].state;
                }
                if (state1 && state2)
                {
                    break;
                }
            }
        }
        if (state1 && state2)
        {
            point = m_links[link_index].getP1(state1->getRadius()+ 0.5*state1->getBorderWidth());
            state1->setCenter(point.x(), point.y());
            point = m_links[link_index].getP2(state2->getRadius()+ 0.5*state2->getBorderWidth());
            state2->setCenter(point.x(), point.y());
            moved_states.push_back(state1);
            moved_states.push_back(state2);
        }
        state1 = nullptr;
        state2 = nullptr;
    }
    updateLinksPosition(moved_states);
}

void MapScene::updateObjectsPosition()
{
    if (!m_selectedLinks.empty())
    {
        updateStatesPosition();
    }
    if (!m_selectedStates.empty())
    {
        updateLinksPosition(m_selectedStates);
    }
}

StateGraphicsObject* MapScene::createNewState(int x, int y)
{
    std::string station_name = "Station " + std::to_string(++new_state_id);
    //auto state_id = m_petriNet->addState<PetryNetComponent::Station>(PetryNetComponent::Station(station_name, x, y, 2));
    int state_id = state_id_generator++;
    StateGraphicsObject* state = new StateGraphicsObject((int)state_id, x, y, 10);
    state->setFillColor(QColor::fromRgb(0, 200, 0));
    state->setBorderWidth(3.f);
    state->setBorderColor(QColor::fromRgb(0, 0, 200));
    //unselectItems();
    //clearSelectedItems();
    //MarkerObject* marker = new MarkerObject(0, TRAIN_COLOR, state);
    //state->addMarker(marker);
    addItem(state);
    return state;
}

void MapScene::createNewLink(StateGraphicsObject* state1, StateGraphicsObject* state2)
{
    //LinkGraphicsObject link(++new_link_id, state1, state2, m_petriNet.get());
    LinkGraphicsObject link(++new_link_id, state1, state2, nullptr);
    m_links.push_back(link);
    m_stateLinks.push_back(StateLink(state1, m_links[m_links.size()-1].getID(), true));
    m_stateLinks.push_back(StateLink(state2, m_links[m_links.size()-1].getID(), false));
    addLinkToScene(m_links.size()-1);
    unselectItems();
    m_links[m_links.size()-1].select();
}
