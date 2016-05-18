#ifndef MAP_SCEHE_H
#define MAP_SCENE_H

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <vector>
#include <memory>
#include "LinkGraphicsObject.h"
#include "../PetriNetUsing.h"

class TestPetriNet;

class StateGraphicsObject;
//class LinkGraphicsObject;
class PointGraphicsObject;

enum class MapMode { View, Move, AddState, AddLink, Delete };
enum class MapViewType { Detailed, Generalized, Mixed };

/*
enum class MarkerCommand { ADD, DELETE, MOVE };

struct MarkerCommandStruct
{
    MarkerCommand command;
    int param1;
    int param2;
    MarkerCommandStruct(MarkerCommand cmd, int p1=0, int p2=0)
        : command(cmd), param1(p1), param2(p2)
    {
    }
};
*/

class MapScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit MapScene(QObject *parent = 0);
    explicit MapScene(qreal x, qreal y, qreal width, qreal height, QObject *parent = 0);
    bool contains(PointGraphicsObject* item) const;
    /*
    void addMarkerCommand(int id, int state_id);
    void moveMarkerCommand(int id, int new_state_id);
    void deleteMarkerCommand(int id);
    void makeCommand();
    */
    StateGraphicsObject* createNewState(int x, int y);
    void createNewLink(StateGraphicsObject* state1, StateGraphicsObject* state2);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *event);

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
    int findLinkIndex(int id);
    void retainSelectedItems();
    void clearSelectedItems();
    void unselectItems();
    template <class T>
    bool contains(const std::vector<T>& container, T value) const;
    void deleteSelectedItems();
    void updateLinksPosition(const std::vector<StateGraphicsObject*>& moved_states);
    void updateStatesPosition();
    void updateObjectsPosition();
    StateGraphicsObject* getStateById(int id);
    StateGraphicsObject* getMarkerById(int id);

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
    enum MarkerColorType { TRAIN_COLOR = 0, MUTEX_COLOR };
    MapMode m_mode;
    StateGraphicsObject* m_linkedState;
    std::vector<StateGraphicsObject*> m_selectedStates;
    std::vector<int> m_selectedLinks;
    std::vector<StateLink> m_stateLinks;
    std::vector<LinkGraphicsObject> m_links;
    int new_state_id;
    int new_link_id;
    //std::vector<MarkerCommandStruct> m_markerCommandQueue;
public:
    std::unique_ptr<RailwayPetriNet> m_petriNet;
};

#endif // MAP_SCENE_H
