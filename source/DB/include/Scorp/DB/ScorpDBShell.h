#ifndef SCORP_DB_SHELL_H
#define SCORP_DB_SHELL_H

#include <string>
#include <memory>
#include "sqlite/include/Database.h"

class ScorpDBShell
{
public:
    ScorpDBShell();
    virtual bool createDatabase(const std::string& path)=0;
    virtual bool connectToDatabase(const std::string& path);
    virtual std::string getDatabasePath() const;
    virtual ~ScorpDBShell();

protected:
    std::string m_databasePath;
    std::unique_ptr<SQLite::Database> m_database;
};

#endif // SCORP_DB_SHELL_H
