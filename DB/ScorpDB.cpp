#include "ScorpDBShell.h"
#include  <iostream>
#include "ScorpExceptions.h"
#include <string>

std::vector<std::string> ScorpDBShell::getDataFromRow(std::string table_name,
                                                     std::string key_column, std::string key)
{
    std::vector<std::string> data_row;
    if ((!table_name.empty()) && (!key_column.empty()) && (!key.empty()))
    {
        try
        {
            SQLite::Statement   query(db, "SELECT * FROM "+table_name
                                      +" where "+key_column+" LIKE :Key");
            query.bind(":Key", key);
            int row_size = query.getColumnCount();
            query.executeStep();
            std::string col = "";
            for(int i=0; i < row_size; ++i)
            {
                col = query.getColumn(i).getText();
                data_row.push_back(col);
            }
        }
        catch(SQLite::Exception e)
        {
            throw SCORPDBAtMemoryLocationException(table_name, key);
        }
    }
    return data_row;
}

std::vector<std::vector<std::string>> ScorpDBShell::getAllDataFromTable(std::string table_name)
{
    std::vector<std::vector<std::string>> table_rows;
    if (!table_name.empty())
    {
        try
        {
            SQLite::Statement query(db, "SELECT * FROM "+table_name);
            int row_size = query.getColumnCount();
            std::vector<std::string> query_colums;
            std::string curr_column;
            while (query.executeStep())
            {
                for(int i = 0; i < row_size; ++i)
                {
                    curr_column = query.getColumn(i).getText();
                    query_colums.push_back(curr_column);
                }
                table_rows.push_back(query_colums);
                query_colums.clear();
            }
        }
        catch(SQLite::Exception e)
        {
            throw SCORPDBAtMemoryLocationException(table_name, "");
        }
    }
    return table_rows;
}

void ScorpDBShell::changeData(std::string table_name, std::string key_column, std::string key,
                             std::string colume_name, std::string data)
{
    std::string colume;
    std::vector<std::vector<std::string>> infoAboutTable = getInfoAboutTable(table_name);
    for(std::vector<std::string> column_info: infoAboutTable)
    {
        if (column_info[1].compare(colume_name) == 0)
            colume = colume_name;
    }
    try
    {
        if ((!key_column.empty()) && (!table_name.empty()) && (!key.empty())
            && (!colume_name.empty()) && (!colume_name.empty()))
        {
             SQLite::Statement   query(db, "UPDATE "+table_name+" SET "+colume+" = :Data WHERE "+key_column+"=:Key");
             query.bind(":Data", data);
             query.bind(":Key", key);
             query.exec();
        }
    }
    catch(SQLite::Exception e)
    {
        throw SCORPDBAtMemoryLocationException(table_name, key_column);
    }
}

void ScorpDBShell::addRowToUser(std::string login, std::string password , std::string group)
{
    try
    {
        SQLite::Statement query(db, "INSERT INTO User VALUES(:Login, :Password, :Group)");
        query.bind(":Login", login);
        query.bind(":Password", password);
        query.bind(":Group", group);
        query.exec();
    }
    catch(SQLite::Exception e)
    {
        throw SCORPDBAtMemoryLocationException("User", login);
    }
}

void ScorpDBShell::addRowToGroup(std::string name, std::string view_map, std::string edit_map,
                                std::string authorization, std::string account_management,
                                std::string edit_station_info, std::string edit_schedule,
                                std::string edit_trains_list, std::string find_route)
{
    try
    {
        SQLite::Statement query(db, "INSERT INTO UserGroups VALUES(:Name, :ViewMap, :EditMap, :Authorization, :AccountManagement, :EditStationInfo, :EditSchedule, :EditTrainsList, :FindRoute)");
        query.bind(":Name", name);
        query.bind(":ViewMap", view_map);
        query.bind(":EditMap", edit_map);
        query.bind(":Authorization", authorization);
        query.bind(":AccountManagement", account_management);
        query.bind(":EditStationInfo", edit_station_info);
        query.bind(":EditSchedule", edit_schedule);
        query.bind(":EditTrainsList", edit_trains_list);
        query.bind(":FindRoute", find_route);
        query.exec();
    }
    catch(SQLite::Exception e)
    {
        throw SCORPDBAtMemoryLocationException("UserGroups", name);
    }
}

void ScorpDBShell::addRowToStation(std::string id, std::string name, std::string x, std::string y,
                                  std::string capacity)
{
    try
    {
        SQLite::Statement query(db, "INSERT INTO Station VALUES(:Id, :Name, :X, :Y, :Capacity)");
        query.bind(":Id", id);
        query.bind(":Name", name);
        query.bind(":X", x);
        query.bind(":Y", y);
        query.bind(":Capacity", capacity);
        query.exec();
    }
    catch(SQLite::Exception e)
    {
        throw SCORPDBAtMemoryLocationException("Station", id);
    }
}

void ScorpDBShell::addRowToRoutePart(std::string route, std::string transition,
                                    std::string time_offset_from, std::string time_offset_to)
{
    try{
        SQLite::Statement query(db, "INSERT INTO TimeDelay VALUES(:Route, :Transition, :TimeOffsetFrom, :TimeOffsetTo)");
        query.bind(":Route", route);
        query.bind(":Transition", transition);
        query.bind(":TimeOffsetFrom", time_offset_from);
        query.bind(":TimeOffsetTo", time_offset_to);
        query.exec();
    }
    catch(SQLite::Exception e)
    {
        throw SCORPDBAtMemoryLocationException("RoutePart", route+" "+ transition);
    }
}

void ScorpDBShell::addRowToTransition(std::string transition, std::string station_from, std::string station_to)
{
    try
    {
        SQLite::Statement query(db, "INSERT INTO Transition VALUES(:Transition, :StationFrom, :StationTo)");
        query.bind(":Transition", transition);
        query.bind(":StationFrom", station_from);
        query.bind(":StationTo", station_to);
        query.exec();
    }
    catch(SQLite::Exception e)
    {
        throw SCORPDBAtMemoryLocationException("Transition", transition);
    }
}

void ScorpDBShell::addRowToTrain(std::string number, std::string route)
{
    try
    {
         SQLite::Statement query(db, "INSERT INTO Train VALUES(:Number, :Route)" );
         query.bind(":Number", number);
         query.bind(":Route", route);
         query.exec();
     }
    catch(SQLite::Exception e)
    {
        throw SCORPDBAtMemoryLocationException("Train", number);
    }
}

void ScorpDBShell::addRowToRoute(std::string id, std::string name)
{
    try
    {
         SQLite::Statement query(db, "INSERT INTO Route VALUES(:Id, :Name)");
         query.bind(":Id", id);
         query.bind(":Name", name);
         query.exec();
    }
    catch(SQLite::Exception e)
    {
        throw SCORPDBAtMemoryLocationException("Route", id);
    }
}

void ScorpDBShell::deleteRowFromTable(std::string table_name, std::string key_column, std::string key)
{
    try
    {
        if ((!key_column.empty()) && (!table_name.empty()))
        {
            SQLite::Statement query(db, "DELETE FROM "+table_name+" WHERE "+key_column+"=:key");
            query.bind(":key", key);
            query.exec();
        }
    }
    catch(SQLite::Exception e)
    {
        throw SCORPDBAtMemoryLocationException(table_name, key);
    }
}
	
bool ScorpDBShell::isUserExistInDb(std::string login)
{
    std::vector<std::vector<std::string>> table_rows;
    SQLite::Statement query(db, "SELECT * FROM User WHERE Login=:login");
    query.bind(":login", login);
    int row_size = query.getColumnCount();
    std::vector<std::string> query_colums;
    std::string curr_column;
    while (query.executeStep())
    {
        for(int i = 0; i < row_size; ++i)
        {
            curr_column = query.getColumn(i).getText();
            query_colums.push_back(curr_column);
        }
        table_rows.push_back(query_colums);
        query_colums.clear();
    }
    if (table_rows.size() == 0)
        return false;
    else
        return true;
}

bool ScorpDBShell::isValidPassword(std::string login, std::string pass)
{
    std::vector<std::vector<std::string>> table_rows;
    SQLite::Statement query(db, "SELECT * FROM User WHERE Login=:login AND Password=:pass");
    query.bind(":login", login);
    query.bind(":pass", pass);
    int row_size = query.getColumnCount();
    std::vector<std::string> query_colums;
    std::string curr_column;
    while (query.executeStep())
    {
        for(int i = 0; i < row_size; ++i)
        {
            curr_column = query.getColumn(i).getText();
            query_colums.push_back(curr_column);
        }
        table_rows.push_back(query_colums);
        query_colums.clear();
    }
    if (table_rows.size() == 0)
        return false;
    else
        return true;
}

std::string ScorpDBShell::getGroupUser(std::string login) 
{
    try
	{
        std::vector<std::string> query_colums;
        SQLite::Statement query(db, "SELECT * FROM User where Login=:login");
        query.bind(":login", login);
        int row_size = query.getColumnCount();
        std::string curr_column;
        query.executeStep();
        for(int i = 0; i < row_size; ++i)
        {
            curr_column = query.getColumn(i).getText();
            query_colums.push_back(curr_column);
        }
        return query_colums[2];
    }
    catch(SQLite::Exception e)
    {
        throw SCORPDBAtMemoryLocationException("User", login);
    }
    return "";
}

std::vector<std::vector<std::string>> ScorpDBShell::getInfoAboutTable(std::string table_name)
{
    std::vector<std::vector<std::string>> table_rows;
    try
    {
        SQLite::Statement query(db, "pragma table_info("+table_name+");");
        int row_size = query.getColumnCount();
        std::vector<std::string> query_colums;
        std::string curr_column;
        while (query.executeStep())
        {
            for(int i = 0; i < row_size; ++i)
            {
                curr_column = query.getColumn(i).getText();
                query_colums.push_back(curr_column);
            }
            table_rows.push_back(query_colums);
            query_colums.clear();
        }
    }
    catch(SQLite::Exception e)
    {
        throw SCORPDBAtMemoryLocationException(table_name, "");
    }
    return table_rows;
}

std::string ScorpDBShell::tableNameToString(TableName table_name)
{
    if (table_name == TableName::USER)
    {
        return "User";
    }
    else if (table_name == TableName::USER_GROUPS)
    {
        return "UserGroups";
    }
    else if (table_name == TableName::ROUTE_PART)
    {
        return "RoutePart";
    }
    else if (table_name == TableName::TRANSITION)
    {
        return "Transition";
    }
    else if (table_name == TableName::TRAIN)
    {
        return "Train";
    }
    else if (table_name == TableName::ROUTE)
    {
        return "Route";
    }
    else if (table_name == TableName::STATION)
    {
        return "Station";
    }
}
