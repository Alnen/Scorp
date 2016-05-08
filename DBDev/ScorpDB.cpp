#include "ScorpDBSell.h"
#include  <iostream>



std::string* ScorpDBSell::getDataFromRow(std::string tableName, std::string Key) 
{
	 	std::string keyColumn;
		std::string Table;
		if(tableName.compare("User")==0)
		{
			keyColumn="Login";
			Table="User";
		}
		else if (tableName.compare("UserGroups")==0)
		{
			keyColumn="Name";
			Table="User";
		}
		else if (tableName.compare("RoutePart")==0)
		{
			keyColumn="Route";
			Table="User";
		}
		else if (tableName.compare("Transition")==0)
		{
			keyColumn="Transition";
			Table="User";
		}
		else if (tableName.compare("Train")==0)
		{
			keyColumn="Number";
			Table="User";
		}
		else if (tableName.compare("Route")==0)
		{
			keyColumn="Id";
			Table="User";
		}
		else if (tableName.compare("Station")==0)
		{
			keyColumn="Id";
			Table="User";
		}

		SQLite::Statement   query(db, "SELECT * FROM "+Table +" where "+keyColumn+" LIKE :Key");
		 query.bind(":Key", Key);	
		const int rowSize=query.getColumnCount();
		std::string* queryColums=new std::string[rowSize];		
		query.executeStep();
			for(int i=0; i<rowSize; i++)
			{
				queryColums[i]=query.getColumn(i);
			}		
		return queryColums;

}

std::vector<std::vector<std::string>> ScorpDBSell::getAllDataFromTable(std::string tableName) 
{
		std::vector<std::vector<std::string>> tableRows;
			std::string Table;
		if(tableName.compare("User")==0)
		{
			Table="User";
		}
		else if (tableName.compare("UserGroups")==0)
		{
			Table="User";
		}
		else if (tableName.compare("RoutePart")==0)
		{
			Table="User";
		}
		else if (tableName.compare("Transition")==0)
		{
			Table="User";
		}
		else if (tableName.compare("Train")==0)
		{
			Table="User";
		}
		else if (tableName.compare("Route")==0)
		{
			Table="User";
		}
		else if (tableName.compare("Station")==0)
		{
			Table="User";
		}

		SQLite::Statement   query(db, "SELECT * FROM "+Table );

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

	void ScorpDBSell::changeData(std::string tableName, std::string Key, std::string ColumeName, std::string Data)
	{
		std::string keyColumn;
		std::string Table;
		std::string colume;
		if(tableName.compare("User")==0)
		{
			keyColumn="Login";
			Table="User";
		}
		else if (tableName.compare("UserGroups")==0)
		{
			keyColumn="Name";
			Table="User";
		}
		else if (tableName.compare("RoutePart")==0)
		{
			keyColumn="Route";
			Table="User";
		}
		else if (tableName.compare("Transition")==0)
		{
			keyColumn="Transition";
			Table="User";
		}
		else if (tableName.compare("Train")==0)
		{
			keyColumn="Number";
			Table="User";
		}
		else if (tableName.compare("Route")==0)
		{
			keyColumn="Id";
			Table="User";
		}
		else if (tableName.compare("Station")==0)
		{
			keyColumn="Id";
			Table="User";
		}
		std::vector<std::vector<std::string>> infoAboutTable=getinfoAboutTable(Table);	
		for(std::vector<std::string> row: infoAboutTable)
		{
			if(row[1].compare(ColumeName)==0)
				colume=ColumeName;
		}
		if(!(keyColumn.empty() &&Table.empty()&&colume.empty()))
		{
			 SQLite::Statement   query(db, "UPDATE "+Table+" SET "+colume+" = :Data WHERE "+keyColumn+"=:Key"  );	
			 query.bind(":Data", Data);
			 query.bind(":Key", Key);		
			 query.exec();
		}
	}

	void ScorpDBSell::addRowToUser(std::string Login, std::string Password , std::string Group)
	{
		SQLite::Statement   query(db, "INSERT INTO User VALUES(:Login, :Password, :Group)"  );	
		 query.bind(":Login", Login);
		 query.bind(":Password", Password);
		 query.bind(":Group", Group);
		 query.exec();
	}

	void ScorpDBSell::addRowToGroup(std::string Name, std::string ViewMap , std::string EditMap, std::string Authorization, std::string AccountManagement, std::string EditStationInfo, std::string EditSchedule, std::string EditTrainsList, std::string FindRoute )
	{
		
		SQLite::Statement   query(db, "INSERT INTO UserGroups VALUES(:Name, :ViewMap, :EditMap, :Authorization, :AccountManagement, :EditStationInfo, :EditSchedule, :EditTrainsList, :FindRoute)");	
		query.bind(":Name", Name);
		query.bind(":ViewMap", ViewMap);
		query.bind(":EditMap", EditMap);
		query.bind(":Authorization", Authorization);
		query.bind(":AccountManagement", AccountManagement);
		query.bind(":EditStationInfo", EditStationInfo);
		query.bind(":EditSchedule", EditSchedule);
		query.bind(":EditTrainsList", EditTrainsList);
		query.bind(":FindRoute", FindRoute);
		query.exec();
	}

	void ScorpDBSell::addRowToStation(std::string Id, std::string Name, std::string X, std::string Y, std::string Capacity )
	{
		SQLite::Statement   query(db, "INSERT INTO Station VALUES(:Id, :Name, :X, :Y, :Capacity)" );	
		query.bind(":Id", Id);
		query.bind(":Name", Name);
		query.bind(":X", X);
		query.bind(":Y", Y);
		query.bind(":Capacity", Capacity);
		query.exec();
	}

	void ScorpDBSell::addRowToRoutePart(std::string Route, std::string Transition, std::string TimeOffsetFrom, std::string TimeOffsetTo)
	{
		SQLite::Statement   query(db, "INSERT INTO TimeDelay VALUES(:Route, :Transition, :TimeOffsetFrom, :TimeOffsetTo)"  );	
		query.bind(":Route", Route);
		query.bind(":Transition", Transition);
		query.bind(":TimeOffsetFrom", TimeOffsetFrom);
		query.bind(":TimeOffsetTo", TimeOffsetTo);
		query.exec();
	}

	
	void ScorpDBSell::addRowToTransition(std::string Transition, std::string StationFrom, std::string StationTo)
	{
		SQLite::Statement   query(db, "INSERT INTO Transition VALUES(:Transition, :StationFrom, :StationTo)"  );	
		query.bind(":Transition", Transition);
		query.bind(":StationFrom", StationFrom);
		query.bind(":StationTo", StationTo);
		query.exec();
	}

	void ScorpDBSell::addRowToTrain(std::string Number, std::string Route)
	{
		
		 SQLite::Statement   query(db, "INSERT INTO Train VALUES(:Number, :Route)" );	
		 query.bind(":Number", Number);
		 query.bind(":Route", Route);
		 query.exec();
	}
	void ScorpDBSell::addRowToRout(std::string Id, std::string Name)
	{
		 SQLite::Statement   query(db, "INSERT INTO Route VALUES(:Id, :Name)" );	
		 query.bind(":Id", Id);
		 query.bind(":Name", Name);
		 query.exec();
	}
	void ScorpDBSell::deleteRowFromTable(std::string tableName, std::string Key)
	{
		std::string keyColumn;
		std::string Table;
		if(tableName.compare("User")==0)
		{
			keyColumn="Login";
			Table="User";
		}
		else if (tableName.compare("UserGroups")==0)
		{
			keyColumn="Name";
			Table="User";
		}
		else if (tableName.compare("RoutePart")==0)
		{
			keyColumn="Route";
			Table="User";
		}
		else if (tableName.compare("Transition")==0)
		{
			keyColumn="Transition";
			Table="User";
		}
		else if (tableName.compare("Train")==0)
		{
			keyColumn="Number";
			Table="User";
		}
		else if (tableName.compare("Route")==0)
		{
			keyColumn="Id";
			Table="User";
		}
		else if (tableName.compare("Station")==0)
		{
			keyColumn="Id";
			Table="User";
		}
		if(!(keyColumn.empty() &&Table.empty()))
		{
			SQLite::Statement   query(db, "DELETE FROM "+tableName+" WHERE "+keyColumn+"=:key" );		
			query.bind(":key", Key);
			query.exec();
		}
		//db.exec("DELETE FROM "+tableName+" WHERE "+keyColumn+"='"+Key+"'" );
	}

	/*std::vector<std::vector<std::string>> ScorpDBSell::getInfoAboutReis(std::string StationFrom, std::string StationTo, std::string TimeShipment)
	{
		std::vector<std::vector<std::string>> tableRows;
		SQLite::Statement   query(db, "SELECT * FROM Voyage" );
	}*/

	
bool ScorpDBSell::isUserExistinDb(std::string login) 
{
	std::vector<std::vector<std::string>> tableRows;
		SQLite::Statement   query(db, "SELECT * FROM User WHERE Login=:login" );
		query.bind(":login",login);
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
		if(tableRows.size()==0) return false;
		else return true;
}
bool ScorpDBSell::isValidPass(std::string login, std::string pass) 
{
	std::vector<std::vector<std::string>> tableRows;
		SQLite::Statement   query(db, "SELECT * FROM User WHERE Login=:login AND Password=:pass" );
		query.bind(":login",login);
		query.bind(":pass", pass);
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
		if(tableRows.size()==0) return false;
		else return true;
}
std::string ScorpDBSell::getGroupUser(std::string login) 
{

	
		std::vector<std::string> queryColums;
		SQLite::Statement   query(db, "SELECT * FROM User where Login=:login");
		query.bind(":login", login);
		const int rowSize=query.getColumnCount();
		std::string currColumn;
		query.executeStep();
			for(int i=0; i<rowSize; i++)
			{
				currColumn=query.getColumn(i);
				queryColums.push_back(currColumn);
			}		
		return queryColums[2];

}





	std::vector<std::vector<std::string>> ScorpDBSell::getinfoAboutTable(std::string tableName)
	{
		std::vector<std::vector<std::string>> tableRows;
			std::string Table;
		if(tableName.compare("User")==0)
		{
			Table="User";
		}
		else if (tableName.compare("UserGroups")==0)
		{
			Table="User";
		}
		else if (tableName.compare("RoutePart")==0)
		{
			Table="User";
		}
		else if (tableName.compare("Transition")==0)
		{
			Table="User";
		}
		else if (tableName.compare("Train")==0)
		{
			Table="User";
		}
		else if (tableName.compare("Route")==0)
		{
			Table="User";
		}
		else if (tableName.compare("Station")==0)
		{
			Table="User";
		}
		SQLite::Statement   query(db, "pragma table_info("+Table+");");

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

	
