#ifndef GRAPHICS_OBJECTS_GROUP_H
#define GRAPHICS_OBJECTS_GROUP_H

#include <vector>

class PointGraphicsObject;

using size_t = unsigned int;

class GraphicsObjectsGroup
{
public:    
    GraphicsObjectsGroup();
    ~GraphicsObjectsGroup();
    bool contains(PointGraphicsObject* item) const;
    void addItem(PointGraphicsObject* item);
    void removeItem(PointGraphicsObject* item);
    void clear();
    PointGraphicsObject* getItem(size_t index) const;
    void setItem(size_t index, PointGraphicsObject* item);
    size_t getItemCount() const;
    std::vector<PointGraphicsObject*> getItems() const;
    void selectItems(bool graphics_selection);
    void deselectItems(bool graphics_selection);
    void hideItems();
    void showItems();

private:
    std::vector<PointGraphicsObject*> m_items;
};

#endif // GRAPHICS_OBJECTS_GROUP_H
