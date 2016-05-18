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

class ItemNotFoundException : public SCORPBaseException
{
    enum class TypeItrem{USER, USER_GROUP, TRANSITION, TRAIN, STATION, ROUTE, ROUTE_PART};
     std::string item;
public:
    ItemNotFoundException(TypeItrem lostTypeItem)
    {
        if(lostTypeItem==TypeItrem::USER)
        {
            item="User";
        }
        if(lostTypeItem==TypeItrem::USER_GROUP)
        {
            item="User Group";
        }
        if(lostTypeItem==TypeItrem::TRANSITION)
        {
            item="Transition";
        }
        if(lostTypeItem==TypeItrem::TRAIN)
        {
            item="Train";
        }
        if(lostTypeItem==TypeItrem::STATION)
        {
            item="Station";
        }
        if(lostTypeItem==TypeItrem::ROUTE)
        {
            item="Route";
        }
        if(lostTypeItem==TypeItrem::ROUTE_PART)
        {
            item="Route Part";
        }
    }

    virtual std::string getErrorCode()
    {
        return "SCORPDB02";
    }

    std::string getLostItem()
    {
        return item;
    }


};


#endif // SCORP_EXCEPTION_H
