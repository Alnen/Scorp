#include "seglist_allocator.h"
#include <stdio.h>
#include <sys/types.h>
#include <limits>

template<typename T>
class Allocator {
public :
    //    typedefs
    typedef T value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

public :
    //    convert an allocator<T> to allocator<U>
    template<typename U>
    struct rebind {
        typedef Allocator<U> other;
    };

public :
    inline explicit Allocator() {}
    inline ~Allocator() {}
    inline explicit Allocator(Allocator const&) {}
    template<typename U>
    inline explicit Allocator(Allocator<U> const&) {}

    //    address
    inline pointer address(reference r) { return &r; }
    inline const_pointer address(const_reference r) { return &r; }

    //    memory allocation
    inline pointer allocate(size_type n, const void * = 0) {
        std::cout << "use my allocator to allocate sizeof(T)=" << sizeof(T) << std::endl;
        if (n == 1) {
std::cout << "my allocator allocation" << std::endl;
                    return static_cast<T*>(SeglistAllocator::instance().allocate(sizeof(T)));

                } else {
            std::cout << "std allocator allocation" << std::endl;
                    return std::allocator<T>().allocate(n);
                }
    }
    inline void deallocate(pointer p, size_type n) {
        std::cout << "use my allocator to deallocate sizeof(T)=" << sizeof(T) << std::endl;
        if (n == 1) {
            std::cout << "my allocator deallocation" << std::endl;
                    SeglistAllocator::instance().deallocate(static_cast<void*>(p), sizeof(T));
                } else {
            std::cout << "std allocator deallocation" << std::endl;
                    return std::allocator<T>().deallocate(p, n);
                }
    }

    //    size
    inline size_type max_size() const {
        return std::numeric_limits<size_type>::max() / sizeof(T);
 }

    //    construction/destruction
    inline void construct(pointer p, const T& t) { new(p) T(t); }
    inline void destroy(pointer p) { p->~T(); }

    inline bool operator==(Allocator const&) { return true; }
    inline bool operator!=(Allocator const& a) { return !operator==(a); }
};    //    end of class Allocator

