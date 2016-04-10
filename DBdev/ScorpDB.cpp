#include "ScorpDB.h"
#include  <iostream>

ScorpDB::ScorpDB(void):db("ScorpDB.db", SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE)
{
		
		
}

ScorpDB::ScorpDB(std::string DBPath, bool CreateNew):db(DBPath, SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE)
{
	if(CreateNew)
	{
		db.exec("CREATE TABLE Station (Id INT (1, 30) PRIMARY KEY UNIQUE NOT NULL, Name CHAR UNIQUE NOT NULL, X INT (1, 30) NOT NULL, Y INT (1, 30) NOT NULL, LineCount INT (1, 30) NOT NULL)" );
		db.exec("CREATE TABLE TimeDelay (Station INT (1, 30) PRIMARY KEY UNIQUE NOT NULL, Train INT (1, 30) UNIQUE NOT NULL, DateFrom DATE (1, 30) NOT NULL, DateTo DATE (1, 30) NOT NULL)");
		db.exec("CREATE TABLE Train (Number INT (1, 30) PRIMARY KEY UNIQUE NOT NULL, \"Voyage\" INT (1, 30), \"Location\" INT (1, 30))");
		db.exec("CREATE TABLE Transition (Transition INT (1, 30) PRIMARY KEY NOT NULL UNIQUE, StationFrom INT (1, 30) NOT NULL, StationTo INT (1, 30) NOT NULL)");]
		db.exec("CREATE TABLE Trip (Id INT (1, 30) PRIMARY KEY UNIQUE NOT NULL, Train INT (1, 30) NOT NULL, Transition INT (1, 30) NOT NULL, LineNumber INT (1, 30) NOT NULL, NextTrip INT (1, 30) NOT NULL)");
		db.exec("CREATE TABLE User (Login CHAR (1, 20) PRIMARY KEY NOT NULL, Password CHAR (1, 12), Groups CHAR (1, 12) NOT NULL)");
		db.exec("CREATE TABLE UserGroups (Groups CHAR (1, 12) PRIMARY KEY NOT NULL, ViewMap BOOLEAN DEFAULT false, EditMap BOOLEAN DEFAULT false, Authorization BOOLEAN DEFAULT false, AccountManagement BOOLEAN DEFAULT false, \"EditStationInfo\" BOOLEAN DEFAULT false, EditSchedule BOOLEAN NOT NULL DEFAULT false, EditTrainsList NOT NULL DEFAULT false, Findtrips NOT NULL DEFAULT false)");
		db.exec("CREATE TABLE Voyage (Id INT (1, 30) PRIMARY KEY NOT NULL UNIQUE, StartTrip INT (1, 30) NOT NULL, FinishTrip INT (1, 30) NOT NULL)");
	}

}



std::string* ScorpDB::getDataFromRow(std::string tableName, std::string Key)
{
	 
		SQLite::Statement   query(db, "SELECT * FROM "+tableName +" where Login LIKE '"+Key+"'");
		const int rowSize=query.getColumnCount();
		std::string* queryColums=new std::string[rowSize];		
		query.executeStep();
			for(int i=0; i<rowSize; i++)
			{
				queryColums[i]=query.getColumn(i);
			}		
		return queryColums;

}

std::vector<std::vector<std::string>> ScorpDB::getAllDataFromTable(std::string tableName)
{
		std::vector<std::vector<std::string>> tableRows;
		SQLite::Statement   query(db, "SELECT * FROM "+tableName );
		const int rowSize=query.getColumnCount();		
		std::vector<std::string> queryColums;
		std::string currColumn;
		while (query.executeStep())
		{
		
			for(int i=0; i<rowSize; i++)
			{
				currColumn=query.getColumn(i);
				queryColums.push_back(currColumn);
			}
			tableRows.push_back(queryColums);
			queryColums.clear();
			
		}
		return tableRows;

}

	void ScorpDB::changeData(std::string tableName, std::string Key, std::string ColumeName, std::string Data)
	{
		std::string keyColumn;
		
		if(tableName.compare("User")==0)
			keyColumn="Login";
		else if (tableName.compare("UserGroups")==0)
			keyColumn="Groups";
		else if (tableName.compare("Trip")==0)
			keyColumn="Id";
		else if (tableName.compare("Transition")==0)
			keyColumn="Transition";
		else if (tableName.compare("Train")==0)
			keyColumn="Number";
		else if (tableName.compare("TimeDelay")==0)
			keyColumn="Station";
		else if (tableName.compare("Station")==0)
			keyColumn="Id";
		else if (tableName.compare("Voyage")==0)
			keyColumn="Id";
		 db.exec("UPDATE "+tableName+" SET "+ColumeName+" = '"+Data+"' WHERE "+keyColumn+"='"+Key+"'");
	}

	void ScorpDB::addRowToUser(std::string Login, std::string Password , std::string Group)
	{
		 db.exec("INSERT INTO User VALUES('"+Login+"', '"+Password+"', '"+Group+"')" );
	}

	void ScorpDB::addRowToGroup(std::string Groups, std::string ViewMap , std::string EditMap, std::string Authorization, std::string AccountManagement, std::string EditStationInfo, std::string EditSchedule, std::string EditTrainsList, std::string Findtrips )
	{
		 db.exec("INSERT INTO UserGroups VALUES('"+Groups+"', '"+ViewMap+"', '"+EditMap+"', '"+Authorization+"', '"+AccountManagement+"', '"+EditStationInfo+"', '"+EditSchedule+"', '"+EditTrainsList+"', '"+Findtrips+"')" );
	}

	void ScorpDB::addRowToStation(std::string Id, std::string Name, std::string X, std::string Y, std::string LineCount )
	{
		db.exec("INSERT INTO Station VALUES('"+Id+"', '"+Name+"', '"+X+"', '"+Y+"', '"+LineCount+"')" );
	}

	void ScorpDB::addRowToTimeDelay(std::string Stattion, std::string Train, std::string DateFrom, std::string DateTo)
	{
		db.exec("INSERT INTO TimeDelay VALUES('"+Stattion+"', '"+Train+"', '"+DateFrom+"', '"+DateTo+"')" );
	}

	void ScorpDB::addRowToTrip(std::string Id, std::string Train , std::string Transition, std::string LineNumber, std::string NextTrip )
	{
		db.exec("INSERT INTO Trip VALUES('"+Id+"', '"+Train+"', '"+Transition+"', '"+LineNumber+"', '"+NextTrip+"')" );
	}

	void ScorpDB::addRowToTransition(std::string Transition, std::string StationFrom, std::string StationTo)
	{
		db.exec("INSERT INTO Transition VALUES('"+Transition+"', '"+StationFrom+"', '"+StationTo+"')" );
	}

	void ScorpDB::addRowToTrain(std::string Number, std::string Voyage, std::string Location)
	{
		db.exec("INSERT INTO Train VALUES('"+Number+"', '"+Voyage+"', '"+Location+"')" );
	}
	void ScorpDB::addRowToVoyage(std::string Id, std::string StartTrip, std::string FinishTrip)
	{
		db.exec("INSERT INTO Train VALUES('"+Id+"', '"+StartTrip+"', '"+FinishTrip+"')" );
	}
	void ScorpDB::deleteRowFromTable(std::string tableName, std::string Key)
	{
		std::string keyColumn;
		
		if(tableName.compare("User")==0)
			keyColumn="Login";
		else if (tableName.compare("UserGroups")==0)
			keyColumn="Groups";
		else if (tableName.compare("Trip")==0)
			keyColumn="Id";
		else if (tableName.compare("Transition")==0)
			keyColumn="Transition";
		else if (tableName.compare("Train")==0)
			keyColumn="Number";
		else if (tableName.compare("TimeDelay")==0)
			keyColumn="Station";
		else if (tableName.compare("Station")==0)
			keyColumn="Id";
		else if (tableName.compare("Voyage")==0)
			keyColumn="Id";
		 db.exec("DELETE FROM "+tableName+" WHERE "+keyColumn+"='"+Key+"'" );
	}

	std::vector<std::vector<std::string>> ScorpDB::getInfoAboutReis(std::string StationFrom, std::string StationTo, std::string TimeShipment)
	{
		std::vector<std::vector<std::string>> tableRows;
		SQLite::Statement   query(db, "SELECT * FROM Voyage" );
	}

	//Пункт отправки
	//

	ScorpDB::~ScorpDB(void)
	{
	}
