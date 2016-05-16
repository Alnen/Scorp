#include "TreeModelItem.h"
#include <QStringList>

TreeModelItem::TreeModelItem(TreeModelItem *parent)
{
    m_parentItem = parent;
}

TreeModelItem::TreeModelItem(const QList<QVariant> &data, TreeModelItem *parent)
{
    m_parentItem = parent;
    m_itemData = data;
}

TreeModelItem::~TreeModelItem()
{
    qDeleteAll(m_childItems);
}

void TreeModelItem::appendChild(TreeModelItem *item)
{
    m_childItems.append(item);
}

void TreeModelItem::insertChild(int row, TreeModelItem *item)
{
    if ((row >= 0) && (row < m_childItems.count()))
    {
        m_childItems.insert(row, item);
    }
    else
    {
        if (row < 0)
        {
            m_childItems.push_front(item);
        }
        else
        {
            m_childItems.append(item);
        }
    }
}

void TreeModelItem::deleteChild(int row)
{
    m_childItems.removeAt(row);
}

TreeModelItem* TreeModelItem::child(int row) const
{
    if ((row >= 0) && (row < m_childItems.count()))
    {
        return m_childItems.value(row);
    }
    else
    {
        return nullptr;
    }
}

int TreeModelItem::childCount() const
{
    return m_childItems.count();
}

int TreeModelItem::columnCount() const
{
    return m_itemData.count();
}

QVariant TreeModelItem::data(int column) const
{
    return m_itemData.value(column);
}

TreeModelItem *TreeModelItem::parentItem() const
{
    return m_parentItem;
}

int TreeModelItem::row() const
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<TreeModelItem*>(this));

    return 0;
}
