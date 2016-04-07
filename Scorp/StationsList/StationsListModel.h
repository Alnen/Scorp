#ifndef STATIONSLISTMODEL_H
#define STATIONSLISTMODEL_H

#include "TreeModel.h"

class StationsListModel
{
public:
    StationsListModel();
    StationsListModel(const QString& station_prefix, const QString& station_postfix,
                      const QString& train_prefix, const QString& train_postfix);
    ~StationsListModel();
    TreeModel* model();
    void appendStation(const QString& name);
    void insertStation(int index, const QString& name);
    void deleteStation(int index);
    void deleteStation(const QString& name);
    void appendTrain(int station_index, int train_number);
    void insertTrain(int station_index, int train_index, int train_number);
    void deleteTrain(int station_index, int train_index);
    void deleteTrain(int number);
    void moveTrain(int station_index, int train_index, int new_station_index, int new_train_index);
    void moveTrain(int train_number, int new_station_index);
    int stationsCount();
    int trainsCount();
    int trainsCount(int station_index);

private:
    TreeModel* m_model;
    QString m_stationPrefix;
    QString m_stationPostfix;
    QString m_trainPrefix;
    QString m_trainPostfix;
};

#endif // STATIONSLISTMODEL_H
