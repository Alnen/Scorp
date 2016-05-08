#ifndef TABLES_h
#define TABLES_h
#include<iostream>
class User
{
private:
	std::string Login;
	std::string Password;
	std::string UserGroup;
public:
	User(std::string Login,	std::string Password, std::string UserGroup)
	{
		
		this->Login=Login;
		this->Password=Password;
		this->UserGroup=UserGroup;
	}
	std::string getLogin()
	{
		return Login;
	}
	std::string getPassword()
	{
		return Password;
	}
	std::string getUserGroup()
	{
		return UserGroup;
	}

};
class UserGroup
{
private:
	std::string Name;
	bool ViewMap;
	bool EditMap;
	bool Autorization;
	bool AccountManagment;
	bool EditStationInfo;
	bool EditSchedule;
	bool EditTrainsList;
	bool Findtrips;
public:
	UserGroup(std::string  Name, bool ViewMap, bool EditMap,	bool Autorization, 	bool AccountManagment,	bool EditStationInfo, bool EditSchedule, bool EditTrainsList, 	bool Findtrips)
	{
		this->Name=Name;
		this->ViewMap=ViewMap;
		this->EditMap=EditMap;
		this->Autorization=Autorization;
		this->AccountManagment=AccountManagment;
		this->EditStationInfo=EditStationInfo;
		this->EditSchedule=EditSchedule;
		this->EditTrainsList=EditTrainsList;
		this->Findtrips=Findtrips;
	}
	std::string getName()
	{
		return Name;
	}
	bool getViewMap()
	{
		return ViewMap;
	}
	bool getEditMap()
	{
		return EditMap;
	}
	bool getAutorization()
	{
		return Autorization;
	}
	bool getAccountManagment()
	{
		return AccountManagment;
	}
	bool getEditStationInfo()
	{
		return EditStationInfo;
	}
	bool getEditSchedule()
	{
		return EditSchedule;
	}
	bool getEditTrainsList()
	{
		return EditTrainsList;
	}
	bool getFindtrips()
	{
		return Findtrips;
	}

};

class Transition
{
private:
	int transition;
	int StationFrom;
	int StationTo;
public:
	Transition(int transition,	int StationFrom, int StationTo);
	int gettransition()
	{
		return  transition;
	}

	int getStationFrom()
	{
		return StationFrom;
	}
	int getStationTo()
	{
		return StationTo;
	}
};

class Train
{
private:
	int Number;
	int Route;
public:
	Train(int Number, int Route)
	{
		this->Number=Number;
		this->Route=Route;
	}
	int getNumber()
	{
		return Number;
	}
	int getRoute()
	{
		return Route;
	}

};

class Station
{
private:
	int Id;
	std::string Name;
	int X;
	int Y;
	int Capasity;
public:
	Station(int Id, std::string Name, int X, int Y, int Capasity)
	{
		this->Id=Id;
		this->Name=Name;
		this->X=X;
		this->Y=Y;
		this->Capasity=Capasity;
	}
	int getId()
	{
		return Id;
	}
	std::string geName()
	{
		return Name;
	}
	int getX()
	{
		return X;
	}
	int getY()
	{
		return Y;
	}
	int getCapasity()
	{
		return Capasity;
	}

};
class RoutePart
{
private:
	int Route;
	int Transistion;
	std::string TimeOffsetFrom;
	std::string TimeOffsetTo;
public:
	RoutePart(	int Route, int Transistion, std::string TimeOffsetFrom,	std::string TimeOffsetTo)
	{
		this->Route=Route;
		this->Transistion=Transistion;
		this->TimeOffsetFrom=TimeOffsetFrom;
		this->TimeOffsetTo=TimeOffsetTo;
	}
	int getRoute()
	{
		return Route;
	}
	int getTransistion()
	{
		return Transistion;
	}
	std::string getTimeOffsetFrom()
	{
		return TimeOffsetFrom;
	}
	std::string getTimeOffsetTo()
	{
		return TimeOffsetTo;
	}
};

class Route
{
private:
	int Id;
	std::string Name;
public:
	Route(int Id, std::string Name)
	{
		this->Id=Id;
		this->Name=Name;
	}
	int getId()
	{
		return Id;
	}
	std::string getName()
	{
		return Name;
	}
};

#endif