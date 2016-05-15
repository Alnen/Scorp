#ifndef USERINFO_H
#define USERINFO_H

#include <vector>
#include <array>
#include <QString>

enum class UserRight { ViewMap, EditMap, Authorization, AccountManagement, EditStationInfo,
       EditSchedule, EditTrainsList, FindTrips };

enum class UserGroupName;

class UserInfo
{
public:
    UserInfo();
    UserInfo(QString login, UserGroupName group);
    QString getLogin() const;
    UserGroupName getGroup() const;
    void setGroup(UserGroupName group);
    void setUserRights(std::array<bool, 8> rights);
    bool getRightStatus(UserRight right) const;
    void setNewUser(QString login, UserGroupName group, std::array<bool, 8> rights);

private:
    size_t mapUserRightToInt(UserRight right) const;

private:
    QString m_login;
    UserGroupName m_group;
    std::array<bool, 8> m_rights;
};

#endif // USERINFO_H
