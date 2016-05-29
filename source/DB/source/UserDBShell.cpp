#include "Scorp/DB/UserDBShell.h"
#include "Scorp/Exceptions/ScorpDBException.h"

UserDBShell::UserDBShell()
    : ScorpDBShell()
{
}

UserDBShell::~UserDBShell()
{
}

bool UserDBShell::createDatabase(const std::string& path)
{
    m_databasePath = path;
    try
    {
    m_database = std::move(std::unique_ptr<SQLite::Database>(new SQLite::Database(path,
                                                             SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE)));
    (*m_database).exec(std::string("CREATE TABLE User (Login STRING (1, 20) PRIMARY KEY UNIQUE NOT NULL,")
        + " Password STRING (1, 18), [Group]  STRING (1, 15) REFERENCES UserGroup (Name)"
        + " ON DELETE NO ACTION ON UPDATE NO ACTION MATCH SIMPLE NOT NULL DEFAULT User)");
    (*m_database).exec(std::string("CREATE TABLE UserGroup (Name STRING (1, 15) NOT NULL PRIMARY KEY UNIQUE,")
        + " ViewMap BOOLEAN NOT NULL DEFAULT (0), EditMap BOOLEAN NOT NULL DEFAULT (0),"
        + " Authorization BOOLEAN NOT NULL DEFAULT (0), AccountManagement BOOLEAN NOT NULL DEFAULT (0),"
        + " EditStationInfo BOOLEAN NOT NULL DEFAULT (0), EditSchedule BOOLEAN NOT NULL DEFAULT (0),"
        + " EditTrainsList BOOLEAN NOT NULL DEFAULT (0), FindTrips BOOLEAN NOT NULL DEFAULT (0))");
    }
    catch (SQLite::Exception)
    {
        throw ScorpDBException::OpenFailedException(path);
        return false;
    }
    return true;
}

bool UserDBShell::addUser(UserDBObject::User user)
{
    try
    {
        SQLite::Statement query(*m_database, "INSERT INTO User VALUES(:login, :password, :group)");
        query.bind(":login", user.login);
        query.bind(":password", user.password);
        query.bind(":group", userGroupToString(user.group));
        query.exec();
    }
    catch (SQLite::Exception)
    {
        throw ScorpDBException::BadMemoryLocationException("User", user.login);
        return false;
    }
    return true;
}

bool UserDBShell::addUserGroup(UserDBObject::UserGroup user_group)
{
    try
    {
        SQLite::Statement query(*m_database, std::string("INSERT INTO UserGroup VALUES(:name, :viewMap, :editMap,")
                                + " :authorization, :accountManagement, :editStationInfo, :editSchedule,"
                                + " :editTrainsList, :findRoute)");
        query.bind(":name", user_group.name);
        query.bind(":viewMap", user_group.viewMap);
        query.bind(":editMap", user_group.editMap);
        query.bind(":authorization", user_group.autorization);
        query.bind(":accountManagement", user_group.accountManagment);
        query.bind(":editStationInfo", user_group.editStationInfo);
        query.bind(":editSchedule", user_group.editSchedule);
        query.bind(":editTrainsList", user_group.editTrainsList);
        query.bind(":findRoute", user_group.findTrips);
        query.exec();
    }
    catch(SQLite::Exception)
    {
        throw ScorpDBException::BadMemoryLocationException("UserGroup", user_group.name);
        return false;
    }
    return true;
}

bool UserDBShell::changeUser(const std::string& login, const std::string& password, UserGroupName group)
{
    try
    {
        SQLite::Statement query(*m_database, "UPDATE User SET Password = :password, Group = :group WHERE Login = :login");
        query.bind(":password", password);
        query.bind(":group", userGroupToString(group));
        query.bind(":login", login);
        query.exec();
    }
    catch (SQLite::Exception)
    {
        throw ScorpDBException::ItemNotFoundException(login, "User");
        return false;
    }
    return true;
}

bool UserDBShell::changeUserGroup(const std::string& login, UserGroupName group)
{
    try
    {
        SQLite::Statement query(*m_database, "UPDATE User SET Group = :group WHERE Login = :login");
        query.bind(":group", userGroupToString(group));
        query.bind(":login", login);
        query.exec();
    }
    catch (SQLite::Exception)
    {
        throw ScorpDBException::ItemNotFoundException(login, "User");
        return false;
    }
    return true;
}

bool UserDBShell::changeUserPassword(const std::string& login, const std::string& password)
{
    try
    {
        SQLite::Statement query(*m_database, "UPDATE User SET Password = :password WHERE Login = :login");
        query.bind(":password", password);
        query.bind(":login", login);
        query.exec();
    }
    catch (SQLite::Exception)
    {
        throw ScorpDBException::ItemNotFoundException(login, "User");
        return false;
    }
    return true;
}

bool UserDBShell::changeUserRight(UserGroupName group, UserRight right, bool value)
{
    try
    {
        SQLite::Statement query(*m_database, "UPDATE UserGroup SET :right = :value WHERE Group = :group");
        query.bind(":right", userRightToString(right));
        query.bind(":value", value);
        query.bind(":group", userGroupToString(group));
        query.exec();
    }
    catch (SQLite::Exception)
    {
        throw ScorpDBException::ItemNotFoundException(userGroupToString(group), "UserGroup");
        return false;
    }
    return true;
}

bool UserDBShell::changeUserRights(UserGroupName group, const std::array<bool, 8>& rights)
{
    try
    {
        SQLite::Statement query(*m_database, std::string("UPDATE UserGroup SET ViewMap = :viewMap,")
            + " EditMap = :editMap, Autorization = :autorization, AccountManagment = :accountManagment,"
            + " EditStationInfo = :editStationInfo, EditSchedule = :editSchedule,"
            + " EditTrainsList = :editTrainsList, FindTrips = :findTrips WHERE Name = :group");
        query.bind(":viewMap", rights[0]);
        query.bind(":editMap", rights[1]);
        query.bind(":autorization", rights[2]);
        query.bind(":accountManagment", rights[3]);
        query.bind(":editStationInfo", rights[4]);
        query.bind(":editSchedule", rights[5]);
        query.bind(":editTrainsList", rights[6]);
        query.bind(":findTrips", rights[7]);
        query.bind(":group", userGroupToString(group));
        query.exec();
    }
    catch (SQLite::Exception)
    {
        throw ScorpDBException::ItemNotFoundException(userGroupToString(group), "UserGroup");
        return false;
    }
    return true;
}

bool UserDBShell::removeUser(const std::string& login)
{
    try
    {
        SQLite::Statement query(*m_database, "DELETE FROM User WHERE Login = :login");
        query.bind(":login", login);
        query.exec();
    }
    catch (SQLite::Exception)
    {
        throw ScorpDBException::ItemNotFoundException(login, "User");
        return false;
    }
    return true;
}

bool UserDBShell::removeUserGroup(UserGroupName group)
{
    try
    {
        SQLite::Statement query(*m_database, "DELETE FROM UserGroup WHERE Name = :name");
        query.bind(":name", userGroupToString(group));
        query.exec();
    }
    catch (SQLite::Exception)
    {
        throw ScorpDBException::ItemNotFoundException(userGroupToString(group), "UserGroup");
        return false;
    }
    return true;
}

std::array<bool, 8> UserDBShell::getUserRights(UserGroupName group)
{
    std::array<bool, 8> user_rights;
    try
    {
        SQLite::Statement query(*m_database, std::string("SELECT ViewMap, EditMap, Authorization,")
            + " AccountManagement, EditStationInfo, EditSchedule, EditTrainsList, FindTrips"
            + " FROM UserGroup WHERE Name = :name");
        query.bind(":name", userGroupToString(group));
        query.executeStep();
        for (size_t i = 0; i < 8; ++i)
        {
            user_rights[i] = query.getColumn(i).getInt();
        }
    }
    catch (SQLite::Exception)
    {
        throw ScorpDBException::ItemNotFoundException(userGroupToString(group), "UserGroup");
    }
    return user_rights;
}

UserGroupName UserDBShell::getUserGroup(const std::string& login)
{
    UserGroupName group = UserGroupName::User;
    try
    {
        SQLite::Statement query(*m_database, "SELECT \"Group\" FROM User WHERE Login = :login");
        query.bind(":login", login);
        if (query.executeStep())
        {
            group = userGroupFromString(query.getColumn(0).getText());
        }
    }
    catch (SQLite::Exception)
    {
        throw ScorpDBException::ItemNotFoundException(login, "User");
    }
    return group;
}

bool UserDBShell::isUserExist(const std::string& login)
{
    SQLite::Statement query(*m_database, "SELECT * FROM User WHERE Login = :login");
    query.bind(":login", login);
    while (query.executeStep())
    {
        return true;
    }
    return false;
}

bool UserDBShell::authenticate(const std::string& login, const std::string& password)
{
    std::string user_password;
    SQLite::Statement query(*m_database, "SELECT Password FROM User WHERE Login = :login");
    query.bind(":login", login);
    if (query.executeStep())
    {
        user_password = query.getColumn(0).getText();
        if (user_password.compare(password) == 0)
        {
            return true;
        }
        return false;
    }
    return false;
}

std::vector<std::pair<std::string, UserGroupName>> UserDBShell::getAllUsers()
{
    std::vector<std::pair<std::string, UserGroupName>> user_list;
    std::string login;
    UserGroupName group;
    try
    {
        SQLite::Statement query(*m_database, "SELECT Login, Group FROM User");
        while (query.executeStep())
        {
            login = query.getColumn(0).getText();
            group = userGroupFromString(query.getColumn(1).getText());
            user_list.push_back(std::pair<std::string, UserGroupName>(login, group));
        }
    }
    catch (SQLite::Exception)
    {
        throw ScorpDBException::BadMemoryLocationException("User", login);
        return std::vector<std::pair<std::string, UserGroupName>>();
    }
    return user_list;
}

std::vector<std::string> UserDBShell::getAllOperators()
{
    std::vector<std::string> operator_list;
    std::string login;
    try
    {
        SQLite::Statement query(*m_database, "SELECT Login FROM User WHERE Group = Operator");
        while (query.executeStep())
        {
            login = query.getColumn(0).getText();
            operator_list.push_back(login);
        }
    }
    catch (SQLite::Exception)
    {
        throw ScorpDBException::BadMemoryLocationException("User", login);
        return std::vector<std::string>();
    }
    return operator_list;
}

std::vector<std::string> UserDBShell::getAllAdmins()
{
    std::vector<std::string> admin_list;
    std::string login;
    try
    {
        SQLite::Statement query(*m_database, "SELECT Login FROM User WHERE Group = Admin");
        while (query.executeStep())
        {
            login = query.getColumn(0).getText();
            admin_list.push_back(login);
        }
    }
    catch (SQLite::Exception)
    {
        throw ScorpDBException::BadMemoryLocationException("User", login);
        return std::vector<std::string>();
    }
    return admin_list;
}
