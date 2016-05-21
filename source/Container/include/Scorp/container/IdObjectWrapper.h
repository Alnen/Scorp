#ifndef IDOBJECTWRAPPER_H
#define IDOBJECTWRAPPER_H

namespace container
{

template <class _IdType>
class IdObjectWrapper
{
public:
    template <class Traits> friend class PetriNet;

    using IdType = _IdType;

    IdObjectWrapper() = default;

    IdObjectWrapper(IdType id): m_id(id)
    {
    }

    IdType getId() const
    {
        return m_id;
    }

protected:
    void setId(IdType id)
    {
        m_id = id;
    }

private:
    IdType m_id;
};

}

#endif //IDOBJECTWRAPPER_H
