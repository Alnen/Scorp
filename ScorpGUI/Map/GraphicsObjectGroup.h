#ifndef GRAPHICS_OBJECT_GROUP_H
#define GRAPHICS_OBJECT_GROUP_H

#include <vector>
#include "PointGraphicsObject.h"
#include "MapScene.h"

class GraphicsObjectGroup
{
public:
    GraphicsObjectGroup();
    void addItemsToMapScene(MapScene* scene);
    void addItem(PointGraphicsObject* item);
    void removeItem(PointGraphicsObject* item);

private:
    std::vector<PointGraphicsObject*> m_items;
};

#endif // GRAPHICS_OBJECT_GROUP_H
