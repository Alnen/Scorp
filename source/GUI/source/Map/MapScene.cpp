#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QDebug>
#include "Scorp/GUI/Map/MapScene.h"
#include "Scorp/GUI/Map/StateGraphicsObject.h"
#include "Scorp/GUI/Map/LinkGraphicsObject.h"
#include "Scorp/GUI/Map/MarkerObject.h"
#include "Scorp/Core/MarkerCommandQueue.h"

static int marker_id_generator = -1;

int state_id_generator = 0;

MapScene::MapScene(QObject *parent) : MapScene(0, 0, 100, 100, parent)
{
}

MapScene::MapScene(qreal x, qreal y, qreal width, qreal height, QObject *parent)
    : QGraphicsScene(x, y, width, height, parent), m_mode(MapMode::View), m_linkedState(nullptr),
      m_petriNet(new RailwayPetriNet()), new_state_id(0), new_link_id(0)
{
    m_style.trainStyle.radius = 8.f;
    m_style.trainStyle.imagePath = tr("images/train.png");
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

RailwayPetriNet* MapScene::getPetriNet() const
{
    return m_petriNet.get();
}

void MapScene::setStyle(const MapSceneStyle::MapSceneStyle& style)
{
    m_style = style;
}

MapSceneStyle::MapSceneStyle MapScene::getStyle() const
{
    return m_style;
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

int MapScene::findLinkIndex(int id) const
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
    for (size_t i = 0; i < m_selectedLinks.size(); ++i)
    {
        link_id = findLinkIndex(m_selectedLinks[i]);
        if (link_id >= 0)
        {
            m_links[link_id].deselect(true);
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
                if (!contains(m_selectedLinks, parent_id))
                {
                    m_links[link_id].select(true);
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
            if (m_linkedState && (link_state != m_linkedState))
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
    else if (m_mode == MapMode::AddTrain)
    {
        if ((choosen_items.size() == 1) && (choosen_items[0]->type() == GraphicsObjectType::StateType)
             && (((StateGraphicsObject*)choosen_items[0])->parentID() < 0))
        {
            createNewMarker((StateGraphicsObject*)choosen_items[0], MarkerObject::MarkerType::Train);
        }
    }
    else if (m_mode == MapMode::AddAccessToken)
    {
        if ((choosen_items.size() == 1) && (choosen_items[0]->type() == GraphicsObjectType::StateType))
        {
            int link_id = ((StateGraphicsObject*)choosen_items[0])->parentID();
            if ((link_id >= 0) && m_links[findLinkIndex(link_id)].isSemaphore((StateGraphicsObject*)choosen_items[0]))
            {
                createNewMarker((StateGraphicsObject*)choosen_items[0], MarkerObject::MarkerType::AccessToken);
            }
        }
    }
    else if (m_mode == MapMode::DeleteTrain)
    {
        if ((choosen_items.size() == 1) && (choosen_items[0]->type() == GraphicsObjectType::StateType))
        {
            removeMarker((StateGraphicsObject*)choosen_items[0]);
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
            disconnect(this, &MapScene::selectionChanged, this, &MapScene::updateSelectionItems);
            unselectItems(true);
            connect(this, &MapScene::selectionChanged, this, &MapScene::updateSelectionItems);
            clearSelectedItems();
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
                disconnect(this, &MapScene::selectionChanged, this, &MapScene::updateSelectionItems);
                unselectItems(true);
                connect(this, &MapScene::selectionChanged, this, &MapScene::updateSelectionItems);
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
    StateGraphicsObject* state = new StateGraphicsObject(state_id, x, y, m_style.stationStyle.radius);
    state->setStyle(m_style.stationStyle);
    //unselectItems();
    //clearSelectedItems();
    addItem(state);
    return state;
}

void MapScene::createNewLink(StateGraphicsObject* state1, StateGraphicsObject* state2)
{
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

void MapScene::createNewMarker(StateGraphicsObject* state, MarkerObject::MarkerType marker_type)
{
    if (m_petriNet.get())
    {
        ++marker_id_generator;
        MarkerObject* marker;
        int marker_id;
        if (marker_type == MarkerObject::MarkerType::Train)
        {
            marker_id = m_petriNet->addMarker<PetriNetComponent::Train>(state->getId(), PetriNetComponent::Train(marker_id_generator));
            marker = new MarkerObject(marker_id, marker_type, state);
            marker->setStyle(m_style.trainStyle);
        }
        else
        {
            marker_id = m_petriNet->addMarker<PetriNetComponent::AccessToken>(state->getId(), PetriNetComponent::AccessToken());
            marker = new MarkerObject(marker_id, marker_type, state);
            marker->setStyle(m_style.accessTokenStyle);
        }
        state->addMarker(marker);
        this->update(this->sceneRect());
    }
}

void MapScene::removeMarker(StateGraphicsObject* state)
{
    if (m_petriNet.get())
    {
        MarkerObject* marker = state->getLastMarker();
        if (marker)
        {
            if (marker->getType() == MarkerObject::MarkerType::Train)
            {
                m_petriNet->removeMarker<PetriNetComponent::Train>(marker->getId());
            }
            else
            {
                m_petriNet->removeMarker<PetriNetComponent::AccessToken>(marker->getId());
            }
            state->removeMarker(marker);
            this->update(this->sceneRect());
        }
    }
}

void MapScene::clearMap()
{
    m_linkedState = nullptr;
    this->clearSelectedItems();
    m_stateLinks.clear();
    m_links.clear();
    this->clear();
    m_petriNet.reset(new RailwayPetriNet());
}

void MapScene::buildStationsByContainer()
{
    StateGraphicsObject* state_obj = nullptr;
    for (auto it = m_petriNet->beginState<PetriNetComponent::Station>(); it != m_petriNet->endState<PetriNetComponent::Station>(); ++it)
    {
        // create state object
        auto& state_wrapper = it->second;
        auto state_info = state_wrapper.getState();
        state_obj = new StateGraphicsObject(it->first, state_info.getX(), state_info.getY(), m_style.stationStyle.radius);
        state_obj->setStyle(m_style.stationStyle);
        addItem(state_obj);
        addMarkersToState<PetriNetComponent::Station>(state_obj, state_wrapper);
    }
}

void MapScene::buildLinksByContainer()
{
    int j = 0;
    for (auto it = m_petriNet->beginState<PetriNetComponent::Station>(); it != m_petriNet->endState<PetriNetComponent::Station>(); ++it)
    {
        qDebug() << "[buildLinksByContainer] step " << QString::number(j);
        ++j;
        int state1_id = it->first;
        auto& state_wrapper = it->second;
        auto& exit_tr_storage = state_wrapper.template getOutTransitionStorage<PetriNetComponent::ExitFromStation>();
        qDebug() << "[buildLinksByContainer] e 1: " << exit_tr_storage.size();
        for (size_t i = 0; i < exit_tr_storage.size(); ++i)
        {
            // find link items id
            auto& exit_tr_wrapper = m_petriNet->getTransitionWrapperById<PetriNetComponent::ExitFromStation>(exit_tr_storage[i]);
            auto& inter_state_storage = exit_tr_wrapper.template getOutStateStorage<PetriNetComponent::InterState>();
            if (inter_state_storage.size() != 1)
            {
                qDebug() << "[buildLinksByContainer] continue 1: " << inter_state_storage.size();
                continue;
            }
            auto& semaphore_storage = exit_tr_wrapper.template getInStateStorage<PetriNetComponent::Semaphore>();
            if (semaphore_storage.size() != 1)
            {
                qDebug() << "[buildLinksByContainer] continue 2";
                continue;
            }
            auto& inter_state_wrapper = m_petriNet->getStateWrapperById<PetriNetComponent::InterState>(inter_state_storage[0]);
            auto& semaphore_wrapper = m_petriNet->getStateWrapperById<PetriNetComponent::Semaphore>(semaphore_storage[0]);
            auto& enter_tr_storage1 = inter_state_wrapper.template getOutTransitionStorage<PetriNetComponent::EnterToStation>();
            auto& enter_tr_storage2 = semaphore_wrapper.template getInTransitionStorage<PetriNetComponent::EnterToStation>();
            if ((enter_tr_storage1.size() != 1) || (enter_tr_storage2.size() != 1)
                || (((int)enter_tr_storage1[0]) != ((int)enter_tr_storage2[0])))
            {
                qDebug() << "[buildLinksByContainer] continue 3";
                continue;
            }
            auto& enter_tr_wrapper = m_petriNet->getTransitionWrapperById<PetriNetComponent::EnterToStation>(enter_tr_storage1[0]);
            auto& out_state_storage = enter_tr_wrapper.template getOutStateStorage<PetriNetComponent::Station>();
            if (out_state_storage.size() != 1)
            {
                qDebug() << "[buildLinksByContainer] continue 4";
                continue;
            }
            int state2_id = out_state_storage[0];
            // find stations object for link
            StateGraphicsObject* curr_state = nullptr;
            StateGraphicsObject* first_state = nullptr;
            StateGraphicsObject* second_state = nullptr;
            for (QGraphicsItem* item : this->items())
            {
                if (item->type() == GraphicsObjectType::StateType)
                {
                    curr_state = (StateGraphicsObject*)item;
                    if (curr_state->getId() == state1_id)
                    {
                        first_state = curr_state;
                    }
                    else if (curr_state->getId() == state2_id)
                    {
                        second_state = curr_state;
                    }
                    if (first_state && second_state)
                    {
                        break;
                    }
                }
            }
            if ((!first_state) || (!second_state))
            {
                qDebug() << "[buildLinksByContainer] continue 5";
                continue;
            }
            // create link object
            qDebug() << "[buildLinksByContainer] e 2";
            std::array<int, 4> id_array{{
                (int)exit_tr_storage[i],
                (int)enter_tr_storage1[0],
                (int)inter_state_storage[0],
                (int)semaphore_storage[0]
            }};
            m_links.push_back(LinkGraphicsObject(++new_link_id, first_state, second_state, m_petriNet.get(), id_array));
            m_stateLinks.push_back(StateLink(first_state, m_links[m_links.size()-1].getID(), true));
            m_stateLinks.push_back(StateLink(second_state, m_links[m_links.size()-1].getID(), false));
            addLinkToScene(m_links.size()-1);
            qDebug() << "[buildLinksByContainer] e 3";
            // add markers to inter state
            StateGraphicsObject* inter_state_obj = (StateGraphicsObject*)m_links[m_links.size()-1].getDetailsItem(LinkGraphicsObject::DetailsItem::InterState);
            addMarkersToState<PetriNetComponent::InterState>(inter_state_obj, inter_state_wrapper);
            // add markers to semaphore
            StateGraphicsObject* semaphore_obj = (StateGraphicsObject*)m_links[m_links.size()-1].getDetailsItem(LinkGraphicsObject::DetailsItem::Semaphore);
            addMarkersToState<PetriNetComponent::Semaphore>(semaphore_obj, semaphore_wrapper);
            qDebug() << "[buildLinksByContainer] e 4";
        }
        qDebug() << "[buildLinksByContainer] end";
    }
}

void MapScene::buildMapByContainer()
{
    buildStationsByContainer();
    buildLinksByContainer();
    this->update(this->sceneRect());
}

void MapScene::makeMarkerCommand()
{
    MarkerCommandQueue::getInstance();
    if (MarkerCommandQueue::getInstance().commandExist())
    {
        StateGraphicsObject* curr_state = nullptr;
        MarkerObject* marker = nullptr;
        MarkerCommandStruct cmd = MarkerCommandQueue::getInstance().getCommand();
        if (cmd.command == MarkerCommand::Add)
        {
            //qDebug() << "makeCommand: ADD (" << cmd.param1 << ")";
            for (auto item : this->items())
            {
                if (((PointGraphicsObject*)item)->type() == GraphicsObjectType::StateType)
                {
                    curr_state = (StateGraphicsObject*)item;
                    if (curr_state->getId() == cmd.param2)
                    {
                        if (cmd.markerType == MarkerCommandStruct::MarkerType::Train)
                        {
                            marker = new MarkerObject(cmd.param1, MarkerObject::MarkerType::Train,
                                curr_state);
                            marker->setStyle(m_style.trainStyle);
                        }
                        else
                        {
                            marker = new MarkerObject(cmd.param1, MarkerObject::MarkerType::AccessToken,
                                curr_state);
                            marker->setStyle(m_style.accessTokenStyle);
                        }
                        curr_state->addMarker(marker);
                        break;
                    }
                }
            }
            curr_state = nullptr;
        }
        else if (cmd.command == MarkerCommand::Delete)
        {
            //qDebug() << "makeCommand: DELETE (" << cmd.param1 << ")";
            MarkerObject* found_marker = nullptr;
            for (auto item : this->items())
            {
                if (((PointGraphicsObject*)item)->type() == GraphicsObjectType::StateType)
                {
                    curr_state = (StateGraphicsObject*)item;
                    found_marker = curr_state->getMarker(cmd.param1);
                    if (found_marker)
                    {
                        curr_state->removeMarker(found_marker);
                        delete found_marker;
                        break;
                    }
                }
            }
        }
        else if (cmd.command == MarkerCommand::Move)
        {
            //qDebug() << "makeCommand: MOVE (" << cmd.param1
            //         << ", " << cmd.param2 << ")";
            MarkerObject* found_marker = nullptr;
            StateGraphicsObject* old_state = nullptr;
            StateGraphicsObject* new_state = nullptr;
            for (auto item : this->items())
            {
                if (((PointGraphicsObject*)item)->type() == GraphicsObjectType::StateType)
                {
                    curr_state = (StateGraphicsObject*)item;
                    MarkerObject* current_found_marker = curr_state->getMarker(cmd.param1);
                    if (current_found_marker)
                    {
                        found_marker = current_found_marker;
                        old_state = curr_state;
                    }
                    if (curr_state->getId() == cmd.param2)
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
    }
}

void MapScene::makeAllMarkerCommands()
{
    while (MarkerCommandQueue::getInstance().commandExist())
    {
        makeMarkerCommand();
    }
}
