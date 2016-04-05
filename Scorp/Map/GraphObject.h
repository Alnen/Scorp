#ifndef GRAPH_OBJECT_H
#define GRAPH_OBJECT_H

#include "PointGraphicsObject.h"
#include "TrackGraphicsObject.h"
#include <QGraphicsItemGroup>
#include <vector>

class GraphObject : QGraphicsItemGroup
{
public:
    GraphObject(QGraphicsItem* parent = nullptr);
    void addVertex(PointGraphicsObject* vertex);
    void addEdge(TrackGraphicsObject* edge);
    void removeVertex(PointGraphicsObject* vertex);
    void removeEdge(TrackGraphicsObject* edge);

private:
    std::vector<PointGraphicsObject*> vertices;
    std::vector<TrackGraphicsObject*> edges;
};

#endif // GRAPH_OBJECT_H
