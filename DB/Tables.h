#ifndef TABLES_h
#define TABLES_h
//#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

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
    void serialize(std::ofstream& output)
    {
        output << "Transition";
        output << "," << transition;
        output << "," << StationFrom;
        output << "," << StationTo;
    }

    void deserialize(std::ifstream& input)
    {
        std::string type_line;
        std::string transition_line;
        std::string station_from_line;
        std::string station_to_line;
        std::getline(input, type_line, ',');
        std::getline(input, transition_line, ',');
        std::getline(input, station_from_line, ',');
        std::getline(input, station_to_line);
        if (!(std::istringstream(transition_line) >> transition)) transition = 0;
        if (!(std::istringstream(station_from_line) >> StationFrom)) StationFrom = 0;
        if (!(std::istringstream(station_to_line) >> StationTo)) StationTo = 0;
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
    void serialize(std::ofstream& output)
    {
        output << "Train";
        output << "," << Number;
        output << "," << Route;
    }

    void deserialize(std::ifstream& input)
    {
        std::string type_line;
        std::string number_line;
        std::string route_line;
        std::getline(input, type_line, ',');
        std::getline(input, number_line, ',');
        std::getline(input, route_line);
        if (!(std::istringstream(number_line) >> Number)) Number = 0;
        if (!(std::istringstream(route_line) >> Route)) Route = 0;
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
    void serialize(std::ofstream& output)
    {
        output << "Station";
        output << "," << Id;
        output << "," << Name;
        output << "," << X;
        output << "," << Y;
        output << "," << Capasity;
    }

    void deserialize(std::ifstream& input)
    {
        std::string type_line;
        std::string id_line;
        std::string x_line;
        std::string y_line;
        std::string capasity_line;
        std::getline(input, type_line, ',');
        std::getline(input, id_line, ',');
        std::getline(input, Name, ',');
        std::getline(input, x_line, ',');
        std::getline(input, y_line, ',');
        std::getline(input, capasity_line);
        if (!(std::istringstream(id_line) >> Id)) Id = 0;
        if (!(std::istringstream(x_line) >> X)) X = 0;
        if (!(std::istringstream(y_line) >> Y)) Y = 0;
        if (!(std::istringstream(capasity_line) >> Capasity)) Capasity = 0;
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
