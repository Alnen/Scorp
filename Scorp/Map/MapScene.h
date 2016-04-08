#ifndef MAP_SCEHE_H
#define MAP_SCENE_H

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include "Map/StateGraphicsObject.h"
#include "Map/TransitionGraphicsObject.h"
#include "Map/TrackGraphicsObject.h"
#include <vector>

enum class MapMode { View, Move, AddState, AddTransition, AddTrack, Delete };

class MapScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit MapScene(QObject *parent = 0);
    explicit MapScene(qreal x, qreal y, qreal width, qreal height, QObject *parent = 0);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *event);

public slots:
    void setMode(MapMode mode);
    void selectItem(PointGraphicsObject* item);

signals:
    void itemsUpdated();
    void modeChanged();
    void itemSelected(QGraphicsItem *item);

private:
    void retainSelectedItems(const QList<QGraphicsItem*>& items_list, bool select_enable = true);
    void clearSelectedItems();
    void unselectItems();
    template <class T>
    bool contains(const std::vector<T>& container, T value);
    StateGraphicsObject* findStateByLinkedTrack(TrackGraphicsObject* track);
    TransitionGraphicsObject* findTransitionByLinkedTrack(TrackGraphicsObject* track);
    void updateTracksPosition();
    void deleteSelectedItems();

private:
    struct StateTrackLink {
        StateGraphicsObject* state;
        TrackGraphicsObject* track;
        StateTrackLink(StateGraphicsObject* n_state, TrackGraphicsObject* n_track)
            : state(n_state), track(n_track)
        {
        }
    };
    struct TransitionTrackLink
    {
        TransitionGraphicsObject* transition;
        TrackGraphicsObject* track;
        TransitionTrackLink(TransitionGraphicsObject* n_transition, TrackGraphicsObject* n_track)
            : transition(n_transition), track(n_track)
        {
        }
    };

private:
    MapMode m_mode;
    StateGraphicsObject* m_linkedState;
    TransitionGraphicsObject* m_linkedTransition;
    std::vector<StateGraphicsObject*> m_selectedStates;
    std::vector<TransitionGraphicsObject*> m_selectedTransitions;
    std::vector<TrackGraphicsObject*> m_selectedTracks;

    std::vector<StateTrackLink> m_stateLinks;
    std::vector<TransitionTrackLink> m_transitionLinks;

};

#endif // MAP_SCENE_H
