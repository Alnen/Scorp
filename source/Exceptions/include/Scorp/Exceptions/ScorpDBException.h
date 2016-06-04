#ifndef SCORP_DB_EXCEPTION_H
#define SCORP_DB_EXCEPTION_H

#include "Scorp/Exceptions/ScorpException.h"

namespace ScorpDBException
{

class OpenFailedException : public ScorpDBBaseException
{
public:
    OpenFailedException(const std::string& path);
    virtual std::string getErrorCode() const;
    std::string getDatabasePath() const;

private:
    std::string m_path;
};

class BadMemoryLocationException : public ScorpDBBaseException
{
public:
    BadMemoryLocationException(const std::string& table, const std::string& itemId);
    virtual std::string getErrorCode() const;
    std::string getTableName() const;
    std::string getItemId() const;

private:
    std::string m_itemId;
    std::string m_tableName;
};

class TableNotFoundException : public ScorpDBBaseException
{
public:
    TableNotFoundException(const std::string& table);
    virtual std::string getErrorCode() const;
    std::string getTableName() const;

private:
    std::string m_tableName;
};

class ColumnNotFoundException : public ScorpDBBaseException
{
public:
    ColumnNotFoundException(const std::string& column, const std::string& table);
    virtual std::string getErrorCode() const;
    std::string getTableName() const;
    std::string getColumnName() const;

private:
    std::string m_tableName;
    std::string m_columnName;
};

class ItemNotFoundException : public ScorpDBBaseException
{
public:
    ItemNotFoundException(const std::string& item_id, const std::string& table);
    virtual std::string getErrorCode() const;
    std::string getItemId() const;
    std::string getTableName() const;

private:
    std::string m_itemId;
    std::string m_tableName;
};

class ItemExistException : public ScorpDBBaseException
{
public:
    ItemExistException(int row, const std::string& column, const std::string& table);
    virtual std::string getErrorCode() const;
    int getRowNumber() const;
    std::string getColumnName() const;
    std::string getTableName() const;

private:
    int m_rowNumber;
    std::string m_columnName;
    std::string m_tableName;
};

}

#endif // SCORP_DB_EXCEPTION_H
