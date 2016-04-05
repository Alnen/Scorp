#ifndef MAP_SCEHE_H
#define MAP_SCENE_H

#include <QGraphicsScene>
#include "Map/StateGraphicsObject.h"

enum class MapMode { View, Move, AddStation, AddTrace, Delete };

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
    void modeChanged();
    void itemSelected(QGraphicsItem *item);

private:
    MapMode m_mode;
    bool m_mouseButtonDown;
    StateGraphicsObject* m_currentStation;
};

#endif // MAP_SCENE_H
