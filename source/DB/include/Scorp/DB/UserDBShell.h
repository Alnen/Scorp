#ifndef USER_DB_SHELL_H
#define USER_DB_SHELL_H

#include <vector>
#include "Scorp/DB/ScorpDBShell.h"
#include "Scorp/DB/UserDBObject.h"

class UserDBShell : public ScorpDBShell
{
public:
    UserDBShell();
    ~UserDBShell();
    bool createDatabase(const std::string& path) override;
    bool addUser(UserDBObject::User user);
    bool addUserGroup(UserDBObject::UserGroup user_group);
    bool changeUser(const std::string& login, const std::string& password, UserGroupName group);
	bool changeUserGroup(const std::string& login, UserGroupName group);
	bool changeUserPassword(const std::string& login, const std::string& password);
    bool changeUserRight(UserGroupName group, UserRight right, bool value);
	bool changeUserRights(UserGroupName group, const std::array<bool, 8>& rights);
	bool removeUser(const std::string& login);
	bool removeUserGroup(UserGroupName group);
	std::array<bool, 8> getUserRights(UserGroupName group);
	UserGroupName getUserGroup(const std::string& login);
	bool isUserExist(const std::string& login);
    bool authenticate(const std::string& login, const std::string& password);
	std::vector<std::pair<std::string, UserGroupName>> getAllUsers();
	std::vector<std::string> getAllOperators();
	std::vector<std::string> getAllAdmins();
};

#endif // USER_DB_SHELL_H
