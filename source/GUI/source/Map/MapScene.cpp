#include "../../include/Map/MapScene.h"
#include "../../include/Map/StateGraphicsObject.h"
#include "../../include/Map/LinkGraphicsObject.h"
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QDebug>
#include "../../include/Map/MarkerObject.h"

static int marker_id_generator = -1;

int state_id_generator = 0;

MapScene::MapScene(QObject *parent)
    : QGraphicsScene(parent), m_mode(MapMode::View), m_linkedState(nullptr),
      m_petriNet(new RailwayPetriNet())
{
    new_state_id = 0;
    new_link_id = 0;
    connect(this, &MapScene::selectionChanged, this, &MapScene::updateSelectionItems);
}

MapScene::MapScene(qreal x, qreal y, qreal width, qreal height, QObject *parent)
    : QGraphicsScene(x, y, width, height, parent), m_mode(MapMode::View), m_linkedState(nullptr),
      m_petriNet(new RailwayPetriNet())
{
    new_state_id = 0;
    new_link_id = 0;
    connect(this, &MapScene::selectionChanged, this, &MapScene::updateSelectionItems);
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

RailwayPetriNet* MapScene::getPetriNet()
{
    return m_petriNet.get();
}

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

void MapScene::updateSelectedItems()
{
    QList<QGraphicsItem*> selected_items(this->selectedItems());
    qDebug() << "updateSelectedItems: size = " << selected_items.size() << "; type = "
             << (((selected_items.size() > 0) && (selected_items[0]->type() == GraphicsObjectType::StateType)) ? "state" : "unknown");
    // unselect old
    for (size_t i = 0; i < m_selectedStates.size(); ++i)
    {
        m_selectedStates[i]->deselect(false);
    }
    int link_id = -1;
    LinkGraphicsObject link(-1, nullptr, nullptr);
    for (size_t i = 0; i < m_selectedLinks.size(); ++i)
    {
        link_id = findLinkIndex(m_selectedLinks[i]);
        if (link_id >= 0)
        {
            link = m_links[link_id];
            link.deselect(true);
            //m_links[link_id].deselect(true);
        }
    }
    m_selectedStates.clear();
    m_selectedLinks.clear();
    // select new
    //QList<QGraphicsItem*> selected_items(this->selectedItems());
    int parent_id = -1;
    StateGraphicsObject* state_item = nullptr;
    for (size_t i = 0; i < selected_items.size(); ++i)
    {
        if ((selected_items[i]->type() == GraphicsObjectType::StateType) &&
            (((StateGraphicsObject*)selected_items[i])->parentID() < 0))
        {
            state_item = (StateGraphicsObject*)selected_items[i];
            if (!contains(m_selectedStates, state_item))
            {
                state_item->select(false);
                m_selectedStates.push_back(state_item);
            }
        }
        else
        {
            parent_id = ((PointGraphicsObject*)selected_items[i])->parentID();
            link_id = findLinkIndex(parent_id);
            if (link_id >= 0)
            {
                //m_links[link_id].select(true);
                if (!contains(m_selectedLinks, parent_id))
                {
                    link = m_links[link_id];
                    link.select(true);
                    m_selectedLinks.push_back(parent_id);
                }
            }
        }
    }
}

void MapScene::clearSelectedItems()
{
    m_selectedStates.clear();
    m_selectedLinks.clear();
}

void MapScene::unselectItems(bool graphics_selection)
{
    //disconnect(this, &MapScene::selectionChanged, this, &MapScene::updateSelectionItems);
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
            //if (selected_state->selectEnable())
            //{
            selected_state->deselect(graphics_selection);
            //}
            selected_state = nullptr;
        }
        else
        {
            selected_link = findLinkIndex(((PointGraphicsObject*)elem)->parentID());
            //if ((selected_link >= 0) && m_links[selected_link].selectEnable())
            if (selected_link >= 0)
            {
                m_links[selected_link].deselect(true);
            }
        }
    }
    //connect(this, &MapScene::selectionChanged, this, &MapScene::updateSelectionItems);
    //this->update(this->sceneRect());
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
        disconnect(this, &MapScene::selectionChanged, this, &MapScene::updateSelectionItems);
        unselectItems(true);
        connect(this, &MapScene::selectionChanged, this, &MapScene::updateSelectionItems);
        clearSelectedItems();
        this->update(this->sceneRect());
        emit modeChanged();
    }
}

void MapScene::mousePressEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton)
        return;
    QPointF mouse_pos = mouseEvent->scenePos();
    QList<QGraphicsItem*> choosen_items = items(mouse_pos);
    if (m_mode == MapMode::AddState)
    {
        createNewState((int)mouse_pos.x(), (int)mouse_pos.y());
    }
    else if (m_mode == MapMode::AddLink)
    {
        StateGraphicsObject* link_state = nullptr;
        if ((choosen_items.size() == 1) && (choosen_items[0]->type() == GraphicsObjectType::StateType))
        {
            link_state = (StateGraphicsObject*)choosen_items[0];
        }
        if (link_state)
        {
            if (m_linkedState)
            {
                createNewLink(m_linkedState, link_state);
                link_state = nullptr;
                m_linkedState = nullptr;
            }
            else
            {
                m_linkedState = link_state;
            }
        }
    }
    else if (m_mode == MapMode::AddMarker)
    {
        if ((choosen_items.size() == 1) && (choosen_items[0]->type() == GraphicsObjectType::StateType)
             && (((StateGraphicsObject*)choosen_items[0])->parentID() < 0))
        {
            createNewMarker((StateGraphicsObject*)choosen_items[0]);
        }
    }
    QGraphicsScene::mousePressEvent(mouseEvent);
}

void MapScene::mouseMoveEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
    if (m_mode == MapMode::Move)
    {
        if (selectedItems().count() > 0)
        {
            updateObjectsPosition();
            this->update(this->sceneRect());
        }
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}

void MapScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
    if (m_mode == MapMode::Move)
    {
        updateObjectsPosition();
        this->update(this->sceneRect());
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
            unselectItems(true);
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
                unselectItems(true);
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
    qDebug() << "updateSelectionItems: " << selectedItems().count();
    disconnect(this, &MapScene::selectionChanged, this, &MapScene::updateSelectionItems);
    updateSelectedItems();
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
    int state_id = m_petriNet->addState<PetriNetComponent::Station>(PetriNetComponent::Station(station_name, x, y, 2));
    //int state_id = state_id_generator++;
    StateGraphicsObject* state = new StateGraphicsObject(state_id, x, y, 10);
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
    //m_links.push_back(LinkGraphicsObject(++new_link_id, state1, state2, nullptr));
    m_links.push_back(LinkGraphicsObject(++new_link_id, state1, state2, m_petriNet.get()));
    m_stateLinks.push_back(StateLink(state1, m_links[m_links.size()-1].getID(), true));
    m_stateLinks.push_back(StateLink(state2, m_links[m_links.size()-1].getID(), false));
    addLinkToScene(m_links.size()-1);
    disconnect(this, &MapScene::selectionChanged, this, &MapScene::updateSelectionItems);
    unselectItems(true);
    connect(this, &MapScene::selectionChanged, this, &MapScene::updateSelectionItems);
    clearSelectedItems();
    m_links[m_links.size()-1].select(true);
}

void MapScene::createNewMarker(StateGraphicsObject* state)
{
    if (m_petriNet.get())
    {
        ++marker_id_generator;
        int marker_id = m_petriNet->addMarker<PetriNetComponent::Train>(state->getId(), PetriNetComponent::Train(marker_id_generator));
        state->addMarker(new MarkerObject(marker_id, TRAIN_COLOR, state));
        this->update(this->sceneRect());
    }
}
