#ifndef SCORPCORE_POSITIONVALIDATOR_H
#define SCORPCORE_POSITIONVALIDATOR_H

#include <ostream>
#include <istream>
#include <iomanip>

namespace manupulator
{

class StreamIsNotValid : std::exception
{
public:
    StreamIsNotValid(std::streamoff current,
                     std::streamoff saved) :
            m_current(current),
            m_saved(saved)
    { }

    virtual const char *what() const noexcept override {
        return "Stream is not valid";
    }

    std::streamoff getCurrent() const {
        return m_current;
    }

    std::streamoff getSaved() const {
        return m_saved;
    }

private:
    std::streamoff m_current;
    std::streamoff m_saved;
};

struct _positioninserter
{
    std::ostream& operator<<(std::ostream& os)
    {
        return os << "\n" << (std::streamoff)os.tellp() - 1 << "\n";
    }
};

struct _positionvalidator
{
    std::istream& operator>>(std::istream& is)
    {
        std::streamoff current_pos = is.tellg();
        std::streamoff saved_pos;
        is >> saved_pos;
        if (saved_pos != current_pos)
        {
            throw StreamIsNotValid(current_pos, saved_pos);
        }
        return is;
    }
};

std::ostream& positioninserter(std::ostream& os)
{
    return _positioninserter() << os;
}

std::istream& positionvalidator(std::istream& is)
{
    return _positionvalidator() >> is;
}

}

#endif //SCORPCORE_POSITIONVALIDATOR_H
