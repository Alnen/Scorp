#include "Scorp/GUI/StationsList/TreeModel.h"
#include "Scorp/GUI/StationsList/TreeModelItem.h"
#include <QStringList>

TreeModel::TreeModel(QObject *parent) : QAbstractItemModel(parent)
{
    QList<QVariant> rootData;
    rootData << "";
    m_rootItem = new TreeModelItem(rootData);
}

TreeModel::~TreeModel()
{
    delete m_rootItem;
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<TreeModelItem*>(parent.internalPointer())->columnCount();
    else
        return m_rootItem->columnCount();
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    TreeModelItem *item = static_cast<TreeModelItem*>(index.internalPointer());

    return item->data(index.column());
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return QAbstractItemModel::flags(index);
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return m_rootItem->data(section);

    return QVariant();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent)
            const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeModelItem *parentItem;

    if (!parent.isValid())
        parentItem = m_rootItem;
    else
        parentItem = static_cast<TreeModelItem*>(parent.internalPointer());

    TreeModelItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeModelItem *childItem = static_cast<TreeModelItem*>(index.internalPointer());
    TreeModelItem *parentItem = childItem->parentItem();

    if (parentItem == m_rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    TreeModelItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = m_rootItem;
    else
        parentItem = static_cast<TreeModelItem*>(parent.internalPointer());

    return parentItem->childCount();
}

TreeModelItem* TreeModel::root() const
{
    return m_rootItem;
}

void TreeModel::appendChild(TreeModelItem* item)
{
    m_rootItem->appendChild(item);
}

void TreeModel::insertChild(int row, TreeModelItem* item)
{
    m_rootItem->insertChild(row, item);
}

void TreeModel::deleteChild(int row)
{
    if ((row >= 0) && (row < m_rootItem->childCount()))
    {
        m_rootItem->deleteChild(row);
    }
}

TreeModelItem* TreeModel::getChild(int row) const
{
    if ((row >= 0) && (row < m_rootItem->childCount()))
    {
        return m_rootItem->child(row);
    }
    else
    {
        return nullptr;
    }
}

int TreeModel::childCount() const
{
    return m_rootItem->childCount();
}
