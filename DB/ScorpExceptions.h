#ifndef SCORP_EXCEPTION_H
#define SCORP_EXCEPTION_H

#include <iostream>
#include <exception>

class SCORPBaseException: public std::exception
{
public:
   virtual std::string getErrorCode()=0;
};

class SCORPDBAtMemoryLocationException : public SCORPBaseException
{
    std::string m_keyColumn;
    std::string m_tableName;
public:
    SCORPDBAtMemoryLocationException(std::string table, std::string key)
       : m_tableName(table), m_keyColumn(key)
    {
    }

    virtual std::string getErrorCode()
    {
        return "SCORPDB01";
    }

    std::string getTableName()
    {
        return m_tableName;
    }

    std::string getKeyColumn()
    {
        return m_keyColumn;
    }
};

#endif // SCORP_EXCEPTION_H
