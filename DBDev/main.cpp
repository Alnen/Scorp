#include<iostream>
#include<SQLiteCpp/SQLiteCpp.h>
#include<SQLiteCpp/Database.h>
#include<SQLiteCpp/Statement.h>
#include <sqlite3.h>

#include "Tables.h"
#include "ScorpDBSell.h"
#include "ScorpDBInterface.h"
#include<vector>
void ScorpDbTest();
void ScorpDBshellTest();
int main()
{

	
	 ScorpDBshellTest();
	
	system("pause");
	return 0;

}

void ScorpDBshellTest()
{

	ScorpDBInterface *db=new ScorpDBSell("ScorpDB.db", false);
	std::string* LoginAdminRow=db->getRowFromTable("User", "admin");
	std::cout << "row: " << LoginAdminRow[0] << ", " << LoginAdminRow[1] << ", " << LoginAdminRow[2] << std::endl;
	std::cout <<"--------------------------------------------------------------------------------"<<std::endl;
	std::vector<std::vector<std::string>> AlldataFormTableUsers=db->getAllRowsFromTable("UserGroups");
	for(std::vector<std::string> row: AlldataFormTableUsers)
	{
		std::cout << "row: ";
		for(std::string column: row)
			std::cout<< column << " " ;
		std::cout <<std::endl;
	}
	std::cout <<"-------------------------------------------------------------------------------"<<std::endl;
	db->changeDatainColume("User", "admin", "Password", "adminpasss");
	AlldataFormTableUsers=db->getAllRowsFromTable("User");
	for(std::vector<std::string> row: AlldataFormTableUsers)
	{
		std::cout << "row: ";
		for(std::string column: row)
			std::cout<< column << " " ;
		std::cout <<std::endl;
	}
	std::cout <<"--------------------------------------------------------------------------------"<<std::endl;
	User user("Operator2", "1111", "Operator");
	db->addUser(user);
	AlldataFormTableUsers=db->getAllRowsFromTable("User");
	for(std::vector<std::string> row: AlldataFormTableUsers)
	{
		std::cout << "row: ";
		for(std::string column: row)
			std::cout<< column << " " ;
		std::cout <<std::endl;
	}
	std::cout <<"--------------------------------------------------------------------------------"<<std::endl;

	UserGroup usgr("sdsd", true, true,	true, 	true,	true, true, true, 	true);

	db->addUserGroups(usgr);
	AlldataFormTableUsers=db->getAllRowsFromTable("UserGroups");
	for(std::vector<std::string> row: AlldataFormTableUsers)
	{
		std::cout << "row: ";
		for(std::string column: row)
			std::cout<< column << " " ;
		std::cout <<std::endl;
	}
	std::cout <<"--------------------------------------------------------------------------------"<<std::endl;
	db->deleteFromTable("UserGroups", "sdsd");
	AlldataFormTableUsers=db->getAllRowsFromTable("UserGroups");
	for(std::vector<std::string> row: AlldataFormTableUsers)
	{
		std::cout << "row: ";
		for(std::string column: row)
			std::cout<< column << " " ;
		std::cout <<std::endl;
	}
	std::cout <<"--------------------------------------------------------------------------------"<<std::endl;
		
	db->deleteFromTable("User", "Operator2");
	AlldataFormTableUsers=db->getAllRowsFromTable("User");
	for(std::vector<std::string> row: AlldataFormTableUsers)
	{
		std::cout << "row: ";
		for(std::string column: row)
			std::cout<< column << " " ;
		std::cout <<std::endl;
	}
	std::cout <<"--------------------------------------------------------------------------------"<<std::endl;
	std::cout <<"Login admin:"<<std::endl;
	if (db->isUserExist("admin"))  std::cout <<"true"<<std::endl; else std::cout <<"false"<<std::endl;
	std::cout <<"Login adminwww:"<<std::endl;
	if (db->isUserExist("adminwww"))  std::cout <<"true"<<std::endl; else std::cout <<"false"<<std::endl;
	std::cout <<"Login-admin, pass-adminpass:"<<std::endl;
	if (db->authenticate("admin", "adminpass"))  std::cout <<"true"<<std::endl; else std::cout <<"false"<<std::endl;
	std::cout <<"Login-admin, pass-adminpass1:"<<std::endl;
	if (db->authenticate("admin", "adminpass1"))  std::cout <<"true"<<std::endl; else std::cout <<"false"<<std::endl;
	std::cout <<"Login-admin1, pass-adminpass:"<<std::endl;
	if (db->authenticate("admin1", "adminpass"))  std::cout <<"true"<<std::endl; else std::cout <<"false"<<std::endl;
	std::cout <<"--------------------------------------------------------------------------------"<<std::endl;
	std::cout <<"--------------------------------------------------------------------------------"<<std::endl;
	

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