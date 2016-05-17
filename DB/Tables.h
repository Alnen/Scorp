#ifndef TABLES_h
#define TABLES_h
//#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

class User
{
private:
    std::string m_login;
    std::string m_password;
    std::string m_userGroup;

public:
    User(std::string login,	std::string password, std::string user_group)
	{
		
        m_login = login;
        m_password = password;
        m_userGroup = user_group;
	}

	std::string getLogin()
	{
        return m_login;
	}

	std::string getPassword()
	{
        return m_password;
	}

	std::string getUserGroup()
	{
        return m_userGroup;
	}

    void serialize(std::ofstream& output)
    {
        output << "User";
        output << "," << m_login;
        output << "," << m_password;
        output << "," << m_userGroup;
    }

    void deserialize(std::ifstream& input)
    {
        std::string type_line;
        std::getline(input, type_line, ',');
        std::getline(input, m_login, ',');
        std::getline(input, m_password, ',');
        std::getline(input, m_userGroup);
    }
};

class UserGroup
{
private:
    std::string m_name;
    bool m_viewMap;
    bool m_editMap;
    bool m_autorization;
    bool m_accountManagment;
    bool m_editStationInfo;
    bool m_editSchedule;
    bool m_editTrainsList;
    bool m_findTrips;

public:
    UserGroup(std::string name, bool view_map, bool edit_map, bool autorization,
              bool account_managment, bool edit_station_info, bool edit_schedule,
              bool edit_trains_list, bool find_trips)
	{
        m_name = name;
        m_viewMap = view_map;
        m_editMap = edit_map;
        m_autorization = autorization;
        m_accountManagment = account_managment;
        m_editStationInfo = edit_station_info;
        m_editSchedule = edit_schedule;
        m_editTrainsList = edit_trains_list;
        m_findTrips = find_trips;
	}

	std::string getName()
	{
        return m_name;
	}

	bool getViewMap()
	{
        return m_viewMap;
	}

	bool getEditMap()
	{
        return m_editMap;
	}

	bool getAutorization()
	{
        return m_autorization;
	}
	bool getAccountManagment()
	{
        return m_accountManagment;
	}

	bool getEditStationInfo()
	{
        return m_editStationInfo;
	}
	bool getEditSchedule()
	{
        return m_editSchedule;
	}

	bool getEditTrainsList()
	{
        return m_editTrainsList;
	}
	bool getFindtrips()
	{
        return m_findTrips;
	}

    void serialize(std::ofstream& output)
    {
        output << "UserGroup";
        output << "," << m_name;
        output << "," << m_viewMap;
        output << "," << m_editMap;
        output << "," << m_autorization;
        output << "," << m_accountManagment;
        output << "," << m_editStationInfo;
        output << "," << m_editSchedule;
        output << "," << m_editTrainsList;
        output << "," << m_findTrips;
    }

    void deserialize(std::ifstream& input)
    {
        std::string type_line;
        std::string view_map_line;
        std::string edit_map_line;
        std::string autorization_line;
        std::string account_managment_line;
        std::string edit_station_info_line;
        std::string edit_schedule_line;
        std::string edit_trains_list_line;
        std::string find_trips_line;

        std::getline(input, type_line, ',');
        std::getline(input, m_name, ',');
        std::getline(input, view_map_line, ',');
        std::getline(input, edit_map_line, ',');
        std::getline(input, autorization_line, ',');
        std::getline(input, account_managment_line, ',');
        std::getline(input, edit_station_info_line, ',');
        std::getline(input, edit_schedule_line, ',');
        std::getline(input, edit_trains_list_line, ',');
        std::getline(input, find_trips_line);
        if (!(std::istringstream(view_map_line) >> m_viewMap)) m_viewMap = 0;
        if (!(std::istringstream(edit_map_line) >> m_editMap)) m_editMap = 0;
        if (!(std::istringstream(autorization_line) >> m_autorization)) m_autorization = 0;
        if (!(std::istringstream(account_managment_line) >> m_accountManagment)) m_accountManagment = 0;
        if (!(std::istringstream(edit_station_info_line) >> m_editStationInfo)) m_editStationInfo = 0;
        if (!(std::istringstream(edit_schedule_line) >> m_editSchedule)) m_editSchedule = 0;
        if (!(std::istringstream(edit_trains_list_line) >> m_editTrainsList)) m_editTrainsList = 0;
        if (!(std::istringstream(find_trips_line) >> m_findTrips)) m_findTrips = 0;
    }
};

class Transition
{
private:
    int m_transition;
    int m_stationFrom;
    int m_stationTo;

public:
    Transition(int transition,	int station_from, int station_to)
    {
        m_transition = transition;
        m_stationFrom = station_from;
        m_stationTo = station_to;
    }

    int getTransition()
	{
        return  m_transition;
	}

	int getStationFrom()
	{
        return m_stationFrom;
	}

	int getStationTo()
	{
        return m_stationTo;
	}

    void serialize(std::ofstream& output)
    {
        output << "Transition";
        output << "," << m_transition;
        output << "," << m_stationFrom;
        output << "," << m_stationTo;
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
        if (!(std::istringstream(transition_line) >> m_transition)) m_transition = 0;
        if (!(std::istringstream(station_from_line) >> m_stationFrom)) m_stationFrom = 0;
        if (!(std::istringstream(station_to_line) >> m_stationTo)) m_stationTo = 0;
    }
};

class Train
{
private:
    int m_number;
    int m_route;

public:
    Train(int number, int route)
	{
        m_number = number;
        m_route = route;
	}

	int getNumber()
	{
        return m_number;
	}
	int getRoute()
	{
        return m_route;
	}
    void serialize(std::ofstream& output)
    {
        output << "Train";
        output << "," << m_number;
        output << "," << m_route;
    }

    void deserialize(std::ifstream& input)
    {
        std::string type_line;
        std::string number_line;
        std::string route_line;
        std::getline(input, type_line, ',');
        std::getline(input, number_line, ',');
        std::getline(input, route_line);
        if (!(std::istringstream(number_line) >> m_number)) m_number = 0;
        if (!(std::istringstream(route_line) >> m_route)) m_route = 0;
    }
};

class Station
{
private:
    int m_id;
    std::string m_name;
    int m_x;
    int m_y;
    int m_capasity;
public:
    Station(int id, std::string name, int x, int y, int capasity)
	{
        m_id = id;
        m_name = name;
        m_x = x;
        m_y = y;
        m_capasity = capasity;
	}
	int getId()
	{
        return m_id;
	}

	std::string geName()
	{
        return m_name;
	}

	int getX()
	{
        return m_x;
	}

	int getY()
	{
        return m_y;
	}

	int getCapasity()
	{
        return m_capasity;
	}

    void serialize(std::ofstream& output)
    {
        output << "Station";
        output << "," << m_id;
        output << "," << m_name;
        output << "," << m_x;
        output << "," << m_y;
        output << "," << m_capasity;
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
        std::getline(input, m_name, ',');
        std::getline(input, x_line, ',');
        std::getline(input, y_line, ',');
        std::getline(input, capasity_line);
        if (!(std::istringstream(id_line) >> m_id)) m_id = 0;
        if (!(std::istringstream(x_line) >> m_x)) m_x = 0;
        if (!(std::istringstream(y_line) >> m_y)) m_y = 0;
        if (!(std::istringstream(capasity_line) >> m_capasity)) m_capasity = 0;
    }
};

class RoutePart
{
private:
    int m_route;
    int m_transistion;
    std::string m_timeOffsetFrom;
    std::string m_timeOffsetTo;

public:
    RoutePart(int route, int transistion, std::string time_offset_from,	std::string time_offset_to)
	{
        m_route = route;
        m_transistion = transistion;
        m_timeOffsetFrom = time_offset_from;
        m_timeOffsetTo = time_offset_to;
	}

	int getRoute()
	{
        return m_route;
	}

	int getTransistion()
	{
        return m_transistion;
	}

	std::string getTimeOffsetFrom()
	{
        return m_timeOffsetFrom;
	}

	std::string getTimeOffsetTo()
	{
        return m_timeOffsetTo;
	}

    void serialize(std::ofstream& output)
    {
        output << "RoutePart";
        output << "," << m_route;
        output << "," << m_transistion;
        output << "," << m_timeOffsetFrom;
        output << "," << m_timeOffsetTo;
    }

    void deserialize(std::ifstream& input)
    {
        std::string type_line;
        std::string route_line;
        std::string tr_line;
        std::getline(input, type_line, ',');
        std::getline(input, route_line, ',');
        std::getline(input, tr_line, ',');
        std::getline(input, m_timeOffsetFrom, ',');
        std::getline(input, m_timeOffsetTo);
        if (!(std::istringstream(route_line) >> m_route)) m_route = 0;
        if (!(std::istringstream(tr_line) >> m_transistion)) m_transistion = 0;
    }
};

class Route
{
private:
    int m_id;
    std::string m_name;

public:
    Route(int id, std::string name)
	{
        m_id = id;
        m_name = name;
	}

	int getId()
	{
        return m_id;
	}

	std::string getName()
	{
        return m_name;
	}

    void serialize(std::ofstream& output)
    {
        output << "Route";
        output << "," << m_id;
        output << "," << m_name;
    }

    void deserialize(std::ifstream& input)
    {
        std::string type_line;
        std::string id_line;
        std::getline(input, type_line, ',');
        std::getline(input, id_line, ',');
        std::getline(input, m_name);
        if (!(std::istringstream(id_line) >> m_id)) m_id = 0;
    }
};

#endif
