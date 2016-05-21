#ifndef SCORP_EXCEPTION_H
#define SCORP_EXCEPTION_H

#include <iostream>
#include <exception>

class ScorpBaseException: public std::exception
{
public:
   virtual std::string getErrorCode() const=0;
};

class ScorpDBBaseException : public ScorpBaseException
{
public:
    ScorpDBBaseException()
    {
    }

    virtual std::string getErrorCode() const
    {
        return "SCORPDB01";
    }
};

class ScorpCoreBaseException : public ScorpBaseException
{
public:
    ScorpCoreBaseException()
    {
    }

    virtual std::string getErrorCode() const
    {
        return "SCORPCORE01";
    }
};

#endif // SCORP_EXCEPTION_H
