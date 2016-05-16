#include<iostream>
#include <exception>
class SCORPBaseExeption: public std::exception {
	public:
	   __fastcall SCORPBaseExeption( std::string Msg) :exception(Msg.c_str()){}
	   virtual std::string getErrorCode()=0;
     
   
};

class SCORPDBAtMemoryLocationExeption: public SCORPBaseExeption {
	std::string keyColum;
	std::string tableName; 
	public:
	   __fastcall SCORPDBAtMemoryLocationExeption( std::string msg) :SCORPBaseExeption(msg.c_str()){}
	   __fastcall SCORPDBAtMemoryLocationExeption( std::string table, std::string key) :SCORPBaseExeption(table.c_str()){tableName=table; keyColum=key;}
	    virtual std::string getErrorCode()
		{
			return "SCORPDB01"; 
		}
		std::string GetTableName()
		{
			return tableName;
		}
		std::string GetKeyColum()
		{
			return keyColum;
		}
   
};