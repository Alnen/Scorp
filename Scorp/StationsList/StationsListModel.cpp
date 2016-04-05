#include "StationsListModel.h"
#include "TreeModelItem.h"

StationsListModel::StationsListModel()
{
    m_model = new TreeModel();
}

StationsListModel::~StationsListModel()
{
    if (m_model)
    {
        delete m_model;
    }
}

TreeModel* StationsListModel::model()
{
    return m_model;
}

void StationsListModel::appendStation(const QString& name)
{
    QList<QVariant> dataList;
    dataList << name;
    TreeModelItem* station = new TreeModelItem(dataList, m_model->root());
    m_model->appendChild(station);
}

void StationsListModel::insertStation(int index, const QString& name)
{
    QList<QVariant> dataList;
    dataList << name;
    TreeModelItem* station = new TreeModelItem(dataList, m_model->root());
    m_model->insertChild(index, station);
}

void StationsListModel::deleteStation(int index)
{
    m_model->deleteChild(index);
}

void StationsListModel::deleteStation(const QString& name)
{
    TreeModelItem* station = nullptr;
    QString station_name;
    for (int i = 0; i < m_model->childCount(); ++i)
    {
        station = m_model->getChild(i);
        station_name = station->data(0).toString();
        if (station_name.compare(name) == 0)
        {
            station = nullptr;
            m_model->deleteChild(i);
            return;
        }
    }
    station = nullptr;
}

void StationsListModel::appendTrain(int station_index, int train_number)
{
    QList<QVariant> dataList;
    dataList << train_number;
    TreeModelItem* station_item = m_model->getChild(station_index);
    TreeModelItem* train_item = new TreeModelItem(dataList, station_item);
    station_item->appendChild(train_item);
}

void StationsListModel::insertTrain(int station_index, int train_index, int train_number)
{
    if ((station_index >= 0) && (station_index < m_model->root()->childCount()))
    {
        QList<QVariant> dataList;
        dataList << train_number;
        TreeModelItem* station_item = m_model->getChild(station_index);
        TreeModelItem* train_item = new TreeModelItem(dataList, station_item);
        station_item->insertChild(train_index, train_item);
    }
}

void StationsListModel::deleteTrain(int station_index, int train_index)
{
    if ((station_index >= 0) && (station_index < m_model->root()->childCount()))
    {
        TreeModelItem* station_item = m_model->getChild(station_index);
        station_item->deleteChild(train_index);
    }
}

void StationsListModel::deleteTrain(int number)
{
    if (number >= 0)
    {
        TreeModelItem* station = nullptr;
        TreeModelItem* train = nullptr;
        int train_number = -1;
        for (int i = 0; i < m_model->childCount(); ++i)
        {
            station = m_model->getChild(i);
            for (int j = 0; j < station->childCount(); ++j)
            {
                train = station->child(j);
                train_number = train->data(0).toInt();
                train = nullptr;
                if (train_number == number)
                {
                    station->deleteChild(j);
                    station = nullptr;
                    return;
                }
            }
        }
        station = nullptr;
    }
}

void StationsListModel::moveTrain(int station_index, int train_index, int new_station_index, int new_train_index)
{
    TreeModelItem* train = m_model->getChild(station_index)->child(train_index);
    int number = train->data(0).toInt();
    train = nullptr;
    deleteTrain(station_index, train_index);
    insertTrain(new_station_index, new_train_index, number);
}

void StationsListModel::moveTrain(int train_number, int new_station_index)
{
    if (train_number >= 0)
    {
        TreeModelItem* station = nullptr;
        TreeModelItem* train = nullptr;
        int number = -1;
        int old_station_index = -1;
        int old_train_index = -1;
        bool is_found = false;
        for (int i = 0; i < m_model->childCount(); ++i)
        {
            station = m_model->getChild(i);
            for (int j = 0; j < station->childCount(); ++j)
            {
                train = station->child(j);
                number = train->data(0).toInt();
                train = nullptr;
                if (train_number == number)
                {
                    old_station_index = i;
                    old_train_index = j;
                    is_found = true;
                    break;
                }
            }
            if (is_found) break;
        }
        if (old_station_index >= 0)
        {
            deleteTrain(old_station_index, old_train_index);
            appendTrain(new_station_index, train_number);
        }
        station = nullptr;
    }
}

int StationsListModel::stationsCount()
{
    return m_model->childCount();
}

int StationsListModel::trainsCount()
{
    int sum_count = 0;
    for (int i = 0; i < m_model->childCount(); ++i)
    {
        sum_count += m_model->getChild(i)->childCount();
    }
    return sum_count;
}

int StationsListModel::trainsCount(int station_index)
{
    if ((station_index >= 0) && (station_index < m_model->childCount()))
    {
        return m_model->getChild(station_index)->childCount();
    }
    else
    {
        return 0;
    }
}
