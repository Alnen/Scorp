#ifndef USERINFO_H
#define USERINFO_H

#include <vector>
#include <QString>

enum class UserRight { ViewMap, EditMap, Authorization, AccountManagement, EditStationInfo,
       EditSchedule, EditTrainsList, FindTrips };

enum class UserGroup { User, Admin, Operator };

class UserInfo
{
public:
    UserInfo();
    UserInfo(QString login, UserGroup group);
    bool signIn(QString login, QString password);
    QString getLogin() const;
    UserGroup getGroup() const;
    void setGroup(UserGroup group);
    bool getRightStatus(UserRight right) const;

private:
    int mapUserRightToInt(UserRight right) const;
    void setRights(UserGroup group);

private:
    QString m_login;
    UserGroup m_group;
    std::vector<bool> m_rights;
};

#endif // USERINFO_H
