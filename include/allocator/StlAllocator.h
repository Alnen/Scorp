#ifndef STLALLOCATOR
#define STLALLOCATOR

#include "CacheAllocator.h"
#include <stdio.h>
#include <sys/types.h>
#include <limits>

namespace allocator
{

template<typename T>
struct Allocator {

    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;

    using propagate_on_container_copy_assignment = std::true_type;
    using propagate_on_container_move_assignment = std::true_type;
    using propagate_on_container_swap = std::true_type;

    Allocator() = default;
    template<typename U>
    Allocator(const Allocator<U> &) {}
    Allocator(const Allocator &) {}
    Allocator & operator=(const Allocator &) { return *this; }
    Allocator(Allocator &&) = default;
    Allocator & operator=(Allocator &&) = default;

    T* allocate(size_t num_to_allocate)
    {
        if (num_to_allocate == 0)
        {
            return nullptr;
        }
        else
        {
            return static_cast<T*>(CacheAllocator::getInstance().allocate(num_to_allocate * sizeof(T)));
        }
    }

    void deallocate(T* ptr, size_t num_to_free) {
        if (ptr != nullptr || num_to_free != 0) {
            CacheAllocator::getInstance().deallocate(static_cast<void*>(ptr), num_to_free * sizeof(T));
        }
    }

    template<typename U>
    struct rebind
    {
        typedef Allocator<U> other;
    };

    template<typename U, typename... Args>
    void construct(U * object, Args &&... args)
    {
        new (object) U(std::forward<Args>(args)...);
    }

    template<typename U, typename... Args>
    void construct(const U * object, Args &&... args) = delete;

    template<typename U>
    void destroy(U * object)
    {
        object->~U();
    }

    bool operator==(const Allocator & other) const
    {
        return this == &other;
    }

    bool operator!=(const Allocator & other) const
    {
        return !(*this == other);
    }
};

}

#endif // STLALLOCATOR
