#ifndef TREE_MODEL_H
#define TREE_MODEL_H

#include <QAbstractItemModel>

class TreeModelItem;

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit TreeModel(QObject *parent = 0);
    ~TreeModel();

    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

    TreeModelItem* root();
    void appendChild(TreeModelItem* item);
    void insertChild(int row, TreeModelItem* item);
    void deleteChild(int row);
    TreeModelItem* getChild(int row);
    int childCount();

private:
    TreeModelItem *m_rootItem;
};

#endif // TREE_MODEL_H
