#ifndef SCORP_DB_EXCEPTION_H
#define SCORP_DB_EXCEPTION_H

#include "ScorpException.h"
#include "Scorp/DB/ScorpDBObject.h"

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
    BadMemoryLocationException(TableName table, const std::string& key);
    virtual std::string getErrorCode() const;
    TableName getTableName() const;
    std::string getKeyColumn() const;

private:
    std::string m_keyColumn;
    TableName m_tableName;
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
    ColumnNotFoundException(const std::string& column, TableName table);
    virtual std::string getErrorCode() const;
    TableName getTableName() const;
    std::string getColumnName() const;

private:
    TableName m_tableName;
    std::string m_columnName;
};

class ItemNotFoundException : public ScorpDBBaseException
{
public:
    ItemNotFoundException(const std::string& item_id, TableName table);
    virtual std::string getErrorCode() const;
    std::string getItemId() const;
    TableName getTableName() const;

private:
    std::string m_itemId;
    TableName m_tableName;
};

class ItemExistException : public ScorpDBBaseException
{
public:
    ItemExistException(int row, const std::string& column, TableName table);
    virtual std::string getErrorCode() const;
    int getRowNumber() const;
    std::string getColumnName() const;
    TableName getTableName() const;

private:
    int m_rowNumber;
    std::string m_columnName;
    TableName m_tableName;
};

}

#endif // SCORP_DB_EXCEPTION_H
