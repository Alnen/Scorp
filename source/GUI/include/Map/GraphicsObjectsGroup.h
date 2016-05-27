#ifndef GRAPHICS_OBJECTS_GROUP_H
#define GRAPHICS_OBJECTS_GROUP_H

#include <vector>

class PointGraphicsObject;
class MapScene;
class LinkGraphicsObject;

class GraphicsObjectsGroup
{
    friend class LinkGraphicsObject;
public:
    GraphicsObjectsGroup();
    ~GraphicsObjectsGroup();
    bool contains(PointGraphicsObject* item);
    void addItem(PointGraphicsObject* item);
    void removeItem(PointGraphicsObject* item);
    void clear();
    std::vector<PointGraphicsObject*> getItems();
    void selectItems(bool graphics_selection);
    void deselectItems(bool graphics_selection);
    void hideItems();
    void showItems();

private:
    std::vector<PointGraphicsObject*> m_items;
};

#endif // GRAPHICS_OBJECTS_GROUP_H
