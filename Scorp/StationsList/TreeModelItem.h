#ifndef TREE_MODEL_ITEM_H
#define TREE_MODEL_ITEM_H

#include <QList>
#include <QVariant>

class TreeModelItem
{
public:
    explicit TreeModelItem(TreeModelItem *parent = 0);
    explicit TreeModelItem(const QList<QVariant> &data, TreeModelItem *parent = 0);
    ~TreeModelItem();

    void appendChild(TreeModelItem *item);
    void insertChild(int row, TreeModelItem *item);
    void deleteChild(int row);

    TreeModelItem *child(int row) const;
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    TreeModelItem *parentItem() const;

private:
    QList<TreeModelItem*> m_childItems;
    QList<QVariant> m_itemData;
    TreeModelItem *m_parentItem;
};

#endif // TREE_MODEL_ITEM_H
