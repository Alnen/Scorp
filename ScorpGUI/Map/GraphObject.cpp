#include "GraphObject.h"

GraphObject::GraphObject(QGraphicsItem* parent) : QGraphicsItemGroup(parent)
{
}

void GraphObject::addVertex(PointGraphicsObject* vertex)
{
    vertices.push_back(vertex);
    addToGroup(vertex);
}

void GraphObject::addEdge(TrackGraphicsObject* edge)
{
    edges.push_back(edge);
    addToGroup(edge);
}

void GraphObject::removeVertex(PointGraphicsObject* vertex)
{
    if (vertex)
    {
        // TODO: delete from vertices
        removeFromGroup(vertex);
    }
}

void GraphObject::removeEdge(TrackGraphicsObject* edge)
{
    if (edge)
    {
        // TODO: delete from edges
        removeFromGroup(edge);
    }
}
