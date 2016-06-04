#ifndef MAP_SCEHE_H
#define MAP_SCEHE_H

#include <vector>
#include <memory>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include "Scorp/GUI/Map/LinkGraphicsObject.h"
#include "Scorp/GUI/Map/MapSceneStyle.h"
#include "Scorp/GUI/Map/MarkerObject.h"
#include "Scorp/GUI/Map/StateGraphicsObject.h"
#include "Scorp/Core/PetriNetUsing.h"

class StateGraphicsObject;
class PointGraphicsObject;

class MapScene : public QGraphicsScene
{
    Q_OBJECT
public:
    enum class MapMode { View, Move, AddState, AddLink, AddTrain, AddAccessToken, Delete, DeleteTrain };

    explicit MapScene(QObject *parent = 0);
    explicit MapScene(qreal x, qreal y, qreal width, qreal height, QObject *parent = 0);
    bool contains(PointGraphicsObject* item) const;
    RailwayPetriNet* getPetriNet() const;
    void setStyle(const MapSceneStyle::MapSceneStyle& style);
    MapSceneStyle::MapSceneStyle getStyle() const;
    void clearMap();
    void buildMapByContainer();
	
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *event);
	
	StateGraphicsObject* createNewState(int x, int y);
    void createNewLink(StateGraphicsObject* state1, StateGraphicsObject* state2);
	void createNewMarker(StateGraphicsObject* state, MarkerObject::MarkerType marker_type);
    void removeMarker(StateGraphicsObject* state);
    template<class StateType>
    void addMarkersToState(StateGraphicsObject* state_obj,
                           const RailwayPetriNet::StateWrapperType<StateType>& state_wrapper);
    void buildStationsByContainer();
    void buildLinksByContainer();

public slots:
    void setMode(MapMode mode);
    void selectItem(PointGraphicsObject* item);
    void updateSelectionItems();

signals:
    void itemsUpdated();
    void modeChanged();
    void itemSelected(QGraphicsItem *item);

private:
    void addLinkToScene(int index);
    void removeLinkFromScene(int index);
    StateGraphicsObject* getSecondLinkedState(int link_id, StateGraphicsObject* first_state);
    void unlinkStates(int link_id);
    int findLinkIndex(int id) const;
    void updateSelectedItems();
    void retainSelectedItems();
    void clearSelectedItems();
    void unselectItems(bool graphics_selection);
    template <class T>
    bool contains(const std::vector<T>& container, T value) const;
    void deleteSelectedItems();
    void updateLinksPosition(const std::vector<StateGraphicsObject*>& moved_states);
    void updateStatesPosition();
    void updateObjectsPosition();

private:
    struct StateLink {
        StateGraphicsObject* state;
        int link;
        bool is_first;
        StateLink(StateGraphicsObject* state_ptr, int link_id, bool is_first_state)
            : state(state_ptr), link(link_id), is_first(is_first_state)
        {
        }
    };

private:
    MapSceneStyle::MapSceneStyle m_style;
    MapMode m_mode;
    StateGraphicsObject* m_linkedState;
    std::vector<StateGraphicsObject*> m_selectedStates;
    std::vector<int> m_selectedLinks;
    std::vector<StateLink> m_stateLinks;
    std::vector<LinkGraphicsObject> m_links;
    int new_state_id;
    int new_link_id;
    std::unique_ptr<RailwayPetriNet> m_petriNet;
};

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

template<class StateType>
void MapScene::addMarkersToState(StateGraphicsObject* state_obj, const RailwayPetriNet::StateWrapperType<StateType>& state_wrapper)
{
    MarkerObject* marker = nullptr;
    // add trains markers
    auto& train_storage = state_wrapper.template getMarkerStorage<PetriNetComponent::Train>();
    for (size_t i = 0; i < train_storage.size(); ++i)
    {
        marker = new MarkerObject((int)train_storage[i], MarkerObject::MarkerType::Train);
        marker->setStyle(m_style.trainStyle);
        state_obj->addMarker(marker);
    }
    // add access token markers
    auto& access_token_storage = state_wrapper.template getMarkerStorage<PetriNetComponent::AccessToken>();
    for (size_t i = 0; i < access_token_storage.size(); ++i)
    {
        marker = new MarkerObject((int)access_token_storage[i], MarkerObject::MarkerType::AccessToken);
        marker->setStyle(m_style.accessTokenStyle);
        state_obj->addMarker(marker);
    }
}

#endif // MAP_SCEHE_H
