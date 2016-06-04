#include "Scorp/GUI/Map/GraphicsObjectsGroup.h"
#include "Scorp/GUI/Map/PointGraphicsObject.h"
#include "Scorp/GUI/Map/MapScene.h"

GraphicsObjectsGroup::GraphicsObjectsGroup()
{
}

GraphicsObjectsGroup::~GraphicsObjectsGroup()
{
    this->clear();
}

bool GraphicsObjectsGroup::contains(PointGraphicsObject* item) const
{
    for (size_t i = 0; i < m_items.size(); ++i)
    {
        if (m_items[i] == item)
        {
            return true;
        }
    }
    return false;
}

void GraphicsObjectsGroup::addItem(PointGraphicsObject* item)
{
    if (!contains(item))
    {
        m_items.push_back(item);
    }
}

void GraphicsObjectsGroup::removeItem(PointGraphicsObject* item)
{
    for (size_t i = 0; i < m_items.size(); ++i)
    {
        if (m_items[i] == item)
        {
            m_items.erase(m_items.begin() + i);
            return;
        }
    }
}

void GraphicsObjectsGroup::clear()
{
    m_items.clear();
}

PointGraphicsObject* GraphicsObjectsGroup::getItem(size_t index) const
{
    if (index < m_items.size())
    {
        return m_items[index];
    }
    return nullptr;
}

void GraphicsObjectsGroup::setItem(size_t index, PointGraphicsObject* item)
{
    if (index < m_items.size())
    {
        m_items[index] = item;
    }
}

size_t GraphicsObjectsGroup::getItemCount() const
{
    return m_items.size();
}

std::vector<PointGraphicsObject*> GraphicsObjectsGroup::getItems() const
{
    return m_items;
}

void GraphicsObjectsGroup::selectItems(bool graphics_selection)
{
    for (size_t i = 0; i < m_items.size(); ++i)
    {
        m_items[i]->select(graphics_selection);
    }
}

void GraphicsObjectsGroup::deselectItems(bool graphics_selection)
{
    for (size_t i = 0; i < m_items.size(); ++i)
    {
        m_items[i]->deselect(graphics_selection);
    }
}

void GraphicsObjectsGroup::hideItems()
{
    for (size_t i = 0; i < m_items.size(); ++i)
    {
        if (m_items[i])
        {
            m_items[i]->hide();
        }
    }
}

void GraphicsObjectsGroup::showItems()
{
    for (size_t i = 0; i < m_items.size(); ++i)
    {
        if (m_items[i])
        {
            m_items[i]->show();
        }
    }
}
