#ifndef IDGENERATOR_H
#define IDGENERATOR_H

#include <utility>

template <class IdType>
class IntegralIdGenerator
{
    static_assert(std::is_integral<IdType>::value, "IdType must have integral type");

public:

    IdType operator()()
    {
        return m_generator++;
    };

private:
    IdType m_generator = IdType();
};

#endif //IDGENERATOR_H
