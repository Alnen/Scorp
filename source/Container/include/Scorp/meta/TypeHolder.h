#ifndef SCORPCORE_TYPEHOLDER_H
#define SCORPCORE_TYPEHOLDER_H

namespace meta
{

template <class _Type>
struct TypeHolder
{
    using type = _Type;
};

}

#endif //SCORPCORE_TYPEHOLDER_H
