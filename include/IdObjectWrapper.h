#ifndef IDOBJECTWRAPPER_H
#define IDOBJECTWRAPPER_H

template <class _IdType>
class IdObjectWrapper
{
public:
    using IdType = _IdType;

    IdObjectWrapper(IdType id): m_id(id)
    {
    }

    IdType getId() const
    {
        return m_id;
    }

private:
    IdType m_id;
};

#endif //IDOBJECTWRAPPER_H
