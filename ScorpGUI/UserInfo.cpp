#include "UserInfo.h"
#include "DB/ScorpDBInterface.h"
#include <QObject>

UserInfo::UserInfo()
    : m_login(QObject::tr("Unknown")), m_group(UserGroupName::USER)
{
    //setRights(UserGroupName::USER);
}

UserInfo::UserInfo(QString login, UserGroupName group)
    : m_login(login), m_group(group)
{
    //setRights(group);
}

bool UserInfo::signIn(QString login, QString password)
{
    // connect to database and check
    // if OK
    //m_login = login;
    //m_group = UserGroup::OP; // or Admin
    return true;
}

QString UserInfo::getLogin() const
{
    return m_login;
}

UserGroupName UserInfo::getGroup() const
{
    return m_group;
}

void UserInfo::setGroup(UserGroupName group)
{
    if (group != m_group)
    {
        m_group = group;
        //setRights(group);
    }
}

void UserInfo::setUserRights(std::array<bool, 8> rights)
{
    for (size_t i = 0; i < 8; ++i)
    {
        m_rights[i] = rights[i];
    }
}

bool UserInfo::getRightStatus(UserRight right) const
{
    return m_rights[mapUserRightToInt(right)];
}

size_t UserInfo::mapUserRightToInt(UserRight right) const
{
    if (right == UserRight::ViewMap)
    {
        return 0;
    }
    else if (right == UserRight::ViewMap)
    {
        return 1;
    }
    else if (right == UserRight::Authorization)
    {
        return 2;
    }
    else if (right == UserRight::AccountManagement)
    {
        return 3;
    }
    else if (right == UserRight::EditStationInfo)
    {
        return 4;
    }
    else if (right == UserRight::EditSchedule)
    {
        return 5;
    }
    else if (right == UserRight::EditTrainsList)
    {
        return 6;
    }
    else if (right == UserRight::FindTrips)
    {
        return 7;
    }
    return 0;
}

/*
void UserInfo::setRights(UserGroupName group)
{
    if (group == UserGroupName::USER)
    {
        m_rights[mapUserRightToInt(UserRight::ViewMap)] = true;
        m_rights[mapUserRightToInt(UserRight::EditMap)] = false;
        m_rights[mapUserRightToInt(UserRight::Authorization)] = true;
        m_rights[mapUserRightToInt(UserRight::AccountManagement)] = false;
        m_rights[mapUserRightToInt(UserRight::EditStationInfo)] = false;
        m_rights[mapUserRightToInt(UserRight::EditSchedule)] = false;
        m_rights[mapUserRightToInt(UserRight::EditTrainsList)] = false;
        m_rights[mapUserRightToInt(UserRight::FindTrips)] = true;
    }
    else if (group == UserGroupName::ADMIN)
    {
        m_rights[mapUserRightToInt(UserRight::ViewMap)] = true;
        m_rights[mapUserRightToInt(UserRight::EditMap)] = false;
        m_rights[mapUserRightToInt(UserRight::Authorization)] = true;
        m_rights[mapUserRightToInt(UserRight::AccountManagement)] = true;
        m_rights[mapUserRightToInt(UserRight::EditStationInfo)] = false;
        m_rights[mapUserRightToInt(UserRight::EditSchedule)] = false;
        m_rights[mapUserRightToInt(UserRight::EditTrainsList)] = false;
        m_rights[mapUserRightToInt(UserRight::FindTrips)] = true;
    }
    else if (group == UserGroupName::OPERATOR)
    {
        m_rights[mapUserRightToInt(UserRight::ViewMap)] = true;
        m_rights[mapUserRightToInt(UserRight::EditMap)] = true;
        m_rights[mapUserRightToInt(UserRight::Authorization)] = true;
        m_rights[mapUserRightToInt(UserRight::AccountManagement)] = false;
        m_rights[mapUserRightToInt(UserRight::EditStationInfo)] = true;
        m_rights[mapUserRightToInt(UserRight::EditSchedule)] = true;
        m_rights[mapUserRightToInt(UserRight::EditTrainsList)] = true;
        m_rights[mapUserRightToInt(UserRight::FindTrips)] = true;
    }
}
*/
