#include "Scorp/DB/UserDBObject.h"

std::string userRightToString(UserRight right)
{
    if (right == UserRight::ViewMap)
    {
        return "ViewMap";
    }
    else if (right == UserRight::EditMap)
    {
        return "EditMap";
    }
    else if (right == UserRight::Authorization)
    {
        return "Authorization";
    }
    else if (right == UserRight::AccountManagement)
    {
        return "AccountManagement";
    }
    else if (right == UserRight::EditStationInfo)
    {
        return "EditStationInfo";
    }
    else if (right == UserRight::EditSchedule)
    {
        return "EditSchedule";
    }
    else if (right == UserRight::EditTrainsList)
    {
        return "EditTrainsList";
    }
    else
    {
        return "FindTrips";
    }
}

std::string userGroupToString(UserGroupName group)
{
    if (group == UserGroupName::Admin)
    {
        return "Admin";
    }
    else if (group == UserGroupName::Operator)
    {
        return "Operator";
    }
    else
    {
        return "User";
    }
}

UserGroupName userGroupFromString(std::string group_name)
{
    if (group_name.compare("Admin") == 0)
    {
        return UserGroupName::Admin;
    }
    else if (group_name.compare("Operator") == 0)
    {
        return UserGroupName::Operator;
    }
    else
    {
        return UserGroupName::User;
    }
}