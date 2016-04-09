#ifndef TYPESTORAGE_H
#define TYPESTORAGE_H

#include <algorithm>
#include <vector>
#include <utility>

/*template <class _Type>
class TypeStorage
{
public:
    using Type = _Type;
    using iterator = typename std::vector<Type>::iterator;
    using const_iterator = typename std::vector<Type>::const_iterator;

    void add(Type&& value)
    {
        m_storage.push_back(std::forward<Type&&>(value));
    }

    iterator remove(Type&& value)
    {
        iterator element = std::find(begin(), end(), std::forward<Type&&>(value));
        if (element == end())
        {
            return element;
        }
        return m_storage.erase(element);
    }

    const_iterator begin() const
    {
        return m_storage.begin();
    }

    iterator begin()
    {
        return m_storage.begin();
    }

    const_iterator end() const
    {
        return m_storage.end();
    }

    iterator end()
    {
        return m_storage.end();
    }

    Type& operator[](size_t index) const
    {
        return m_storage[index];
    }

    Type& operator[](size_t index)
    {
        return m_storage[index];
    }

    size_t size()
    {
        return m_storage.size();
    }

    bool empty()
    {
        return m_storage.empty();
    }

private:
    std::vector<Type> m_storage;
};*/

template <class T>
using TypeStorage = std::vector<T, std::allocator<T>>;

#endif //TYPESTORAGE_H
