#ifndef MAP_SCEHE_H
#define MAP_SCENE_H

#include <QGraphicsScene>
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

public slots:
    void setMode(MapMode mode);
    void selectItem(PointGraphicsObject* item);

signals:
    void itemsUpdated();
    void modeChanged();
    void itemSelected(QGraphicsItem *item);

private:
    void retainSelectedItems();
    void clearSelectedItems();
    void unselectItems();
    template <class T>
    bool contains(const std::vector<T>& container, T value);

private:
    MapMode m_mode;
    //TrackGraphicsObject* m_createdTrack;
    StateGraphicsObject* m_linkedState;
    TransitionGraphicsObject* m_linkedTransition;
    std::vector<StateGraphicsObject*> m_selectedStates;
    std::vector<TransitionGraphicsObject*> m_selectedTransitions;
    std::vector<TrackGraphicsObject*> m_selectedTracks;
};

#endif // MAP_SCENE_H
