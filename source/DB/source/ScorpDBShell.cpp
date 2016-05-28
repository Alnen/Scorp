#include "Scorp/DB/ScorpDBShell.h"
#include "Scorp/exceptions/ScorpDBException.h"

ScorpDBShell::ScorpDBShell()
    : m_database(nullptr)
{
}

ScorpDBShell::~ScorpDBShell()
{
}

bool ScorpDBShell::connectToDatabase(const std::string& path)
{
    m_databasePath = path;
    try
    {
        m_database = std::move(std::unique_ptr<SQLite::Database>(new SQLite::Database(path, SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE)));
    }
    catch (SQLite::Exception)
    {
        throw ScorpDBException::OpenFailedException(path);
        return false;
    }
    return true;
}

std::string ScorpDBShell::getDatabasePath() const
{
    return m_databasePath;
}
