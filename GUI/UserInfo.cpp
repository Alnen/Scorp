#include "UserInfo.h"
#include "DB/ScorpDBInterface.h"
#include <QObject>

#include <QDebug>

UserInfo::UserInfo()
    : m_login(QObject::tr("guest")), m_group(UserGroupName::USER)
{
}

UserInfo::UserInfo(QString login, UserGroupName group)
    : m_login(login), m_group(group)
{
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
    else if (right == UserRight::EditMap)
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

void UserInfo::setNewUser(QString login, UserGroupName group, std::array<bool, 8> rights)
{
    m_login = login;
    m_group = group;
    for (size_t i = 0; i < 8; ++i)
    {
        m_rights[i] = rights[i];
    }
}
