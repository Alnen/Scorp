#include "../include/Scorp/Exceptions/ScorpDBException.h"
//#include "DB/ScorpDBInterface.h"

namespace ScorpDBException
{

// class OpenFailedException
OpenFailedException::OpenFailedException(const std::string& path) : m_path(path)
{
}

std::string OpenFailedException::getErrorCode() const
{
    return "SCORPDB02";
}

std::string OpenFailedException::getDatabasePath() const
{
    return m_path;
}

// class BadMemoryLocationException
BadMemoryLocationException::BadMemoryLocationException(TableName table, const std::string& key)
   : m_tableName(table), m_keyColumn(key)
{
}

std::string BadMemoryLocationException::getErrorCode() const
{
    return "SCORPDB03";
}

TableName BadMemoryLocationException::getTableName() const
{
    return m_tableName;
}

std::string BadMemoryLocationException::getKeyColumn() const
{
    return m_keyColumn;
}

// class TableNotFoundException
TableNotFoundException::TableNotFoundException(const std::string& table)
    : m_tableName(table)
{
}

std::string TableNotFoundException::getErrorCode() const
{
    return "SCORPDB04";
}

std::string TableNotFoundException::getTableName() const
{
    return m_tableName;
}

// class ColumnNotFoundException
ColumnNotFoundException::ColumnNotFoundException(const std::string& column, TableName table)
    : m_columnName(column), m_tableName(table)
{
}

std::string ColumnNotFoundException::getErrorCode() const
{
    return "SCORPDB05";
}

TableName ColumnNotFoundException::getTableName() const
{
    return m_tableName;
}

std::string ColumnNotFoundException::getColumnName() const
{
    return m_columnName;
}

// class ItemNotFoundException
ItemNotFoundException::ItemNotFoundException(const std::string& item_id, TableName table)
    : m_itemId(item_id), m_tableName(table)
{
}

std::string ItemNotFoundException::getErrorCode() const
{
    return "SCORPDB06";
}

std::string ItemNotFoundException::getItemId() const
{
    return m_itemId;
}

TableName ItemNotFoundException::getTableName() const
{
    return m_tableName;
}

// class ItemExistException
ItemExistException::ItemExistException(int row, const std::string& column, TableName table)
    : m_tableName(table), m_columnName(column), m_rowNumber(row)
{
}

std::string ItemExistException::getErrorCode() const
{
    return "SCORPDB07";
}

int ItemExistException::getRowNumber() const
{
    return m_rowNumber;
}

std::string ItemExistException::getColumnName() const
{
    return m_columnName;
}

TableName ItemExistException::getTableName() const
{
    return m_tableName;
}

}
