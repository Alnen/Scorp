#include<iostream>
#include<SQLiteCpp/SQLiteCpp.h>
#include<SQLiteCpp/Database.h>
#include<SQLiteCpp/Statement.h>
#include <sqlite3.h>
#include "ScorpDB.h"
#include<vector>
int main()
{

	ScorpDB db;


	std::string* LoginAdminRow=db.getDataFromRow("User", "admin");
	std::cout << "row: " << LoginAdminRow[0] << ", " << LoginAdminRow[1] << ", " << LoginAdminRow[2] << std::endl;
	std::cout <<"--------------------------------------------------------------------------------"<<std::endl;
	std::vector<std::vector<std::string>> AlldataFormTableUsers=db.getAllDataFromTable("UserGroups");
	for(std::vector<std::string> row: AlldataFormTableUsers)
	{
		std::cout << "row: ";
		for(std::string column: row)
			std::cout<< column << " " ;
		std::cout <<std::endl;
	}
	std::cout <<"-------------------------------------------------------------------------------"<<std::endl;
	db.changeData("User", "admin", "Password", "adminpass");
	AlldataFormTableUsers=db.getAllDataFromTable("User");
	for(std::vector<std::string> row: AlldataFormTableUsers)
	{
		std::cout << "row: ";
		for(std::string column: row)
			std::cout<< column << " " ;
		std::cout <<std::endl;
	}
	std::cout <<"--------------------------------------------------------------------------------"<<std::endl;
	db.addRowToUser("Operator2", "1111", "Operator");
	AlldataFormTableUsers=db.getAllDataFromTable("User");
	for(std::vector<std::string> row: AlldataFormTableUsers)
	{
		std::cout << "row: ";
		for(std::string column: row)
			std::cout<< column << " " ;
		std::cout <<std::endl;
	}
	std::cout <<"--------------------------------------------------------------------------------"<<std::endl;
	db.addRowToGroup("q1","1","1","1","1","1");
	AlldataFormTableUsers=db.getAllDataFromTable("UserGroups");
	for(std::vector<std::string> row: AlldataFormTableUsers)
	{
		std::cout << "row: ";
		for(std::string column: row)
			std::cout<< column << " " ;
		std::cout <<std::endl;
	}
	std::cout <<"--------------------------------------------------------------------------------"<<std::endl;
	db.deleteRowFromTable("UserGroups", "q1");
	AlldataFormTableUsers=db.getAllDataFromTable("UserGroups");
	for(std::vector<std::string> row: AlldataFormTableUsers)
	{
		std::cout << "row: ";
		for(std::string column: row)
			std::cout<< column << " " ;
		std::cout <<std::endl;
	}
	std::cout <<"--------------------------------------------------------------------------------"<<std::endl;
	system("pause");
	return 0;

}













/*
int main()
{
	try
	{
		// Open a database file
		SQLite::Database    db("ScorpDB.db");
		
		
		 // Compile a SQL query, containing one parameter (index 1)
		SQLite::Statement   query(db, "SELECT * FROM Users ");

		// Bind the integer value 6 to the first parameter of the SQL query
		//query.bind(1, 6);

		// Loop to execute the query step by step, to get rows of result
		while (query.executeStep())
		{
			// Demonstrate how to get some typed column value
			const char*       Users      = query.getColumn(0);
			const char*			Passwords   = query.getColumn(1);
			const char*         Groups    = query.getColumn(2);

			std::cout << "row: " << Users << ", " << Passwords << ", " << Groups << std::endl;
		}
	
	}
	catch (std::exception& e)
	{
		std::cout << "exception: " << e.what() << std::endl;
	}
	system("pause");
	return 0;
}

*/