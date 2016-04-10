#pragma once
#include<SQLiteCpp/SQLiteCpp.h>
#include<SQLiteCpp/Database.h>
#include<SQLiteCpp/Statement.h>
#include <sqlite3.h>
#include <vector>
class ScorpDB
{
private :
	SQLite::Database    db;
public:
	
	ScorpDB(void);
	ScorpDB(std::string DBPath, bool CgeateNew);	
	void addRowToUser(std::string Login, std::string Password , std::string Group);
	void addRowToGroup(std::string Groups, std::string ViewMap , std::string EditMap, std::string Authorization, std::string AccountManagement, std::string EditStationInfo, std::string EditSchedule, std::string EditTrainsList, std::string Findtrips );
	void addRowToStation(std::string Id, std::string Name, std::string X, std::string Y, std::string LineCount );
	void addRowToTimeDelay(std::string Stattion, std::string Train, std::string DateFrom, std::string DateTo);
	void addRowToTrip(std::string Id, std::string Train , std::string Transition, std::string LineNumber, std::string NextTrip );
	void addRowToTransition(std::string Transition, std::string StationFrom, std::string StationTo);
	void addRowToTrain(std::string Number, std::string Voyage, std::string Location);
	void addRowToVoyage(std::string Id, std::string StartTrip, std::string FinishTrip);
	void changeData(std::string tableName,std::string Key, std::string ColumeName, std::string Data);
	void deleteRowFromTable(std::string tableName, std::string Key);
	std::string* getDataFromRow(std::string tableName, std::string Key);
	std::vector<std::vector<std::string>> getAllDataFromTable(std::string tableName);
	std::vector<std::vector<std::string>> getInfoAboutReis(std::string StationFrom, std::string StationTo, std::string TimeShipment);
	~ScorpDB(void);
};

