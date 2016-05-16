#ifndef SEGLISTALLOCATOR
#define SEGLISTALLOCATOR

#include <iostream>
#include <list>
#include <vector>
#include <math.h>
#include "ListOfMemoryNodes.h"

namespace allocator
{

class SeglistAllocator {

    constexpr static int getPowerOfTwo(size_t _power) {
        return _power == 0 ? 1 : 2 *  getPowerOfTwo(_power - 1);
    }

    static const int max_power_of_two = 10;
    static const int MAX_SIZE;

    ListOfMemoryNodes<max_power_of_two> *l;
    int size = 0;

    void create_list() { // allocate new pool of memory
        size = max_power_of_two;
        l = new ListOfMemoryNodes<max_power_of_two>();
    }

    SeglistAllocator() { // private for singleton
        create_list();
    }

public:
    SeglistAllocator(const SeglistAllocator &) = delete; // for singleton
    void operator=(SeglistAllocator const &) = delete;

    static SeglistAllocator &instance() { // singleton
        static SeglistAllocator instance;
        return instance;
    }

    void *allocate(std::size_t alloc_size) {
        /*
        void* allocation = nullptr;
        if(alloc_size > MAX_SIZE)
         {
            //!!!!!!!!!!!!!!!!
            lists.push_back(new ListOfMemoryNodes<pow(2, max_power_of_two + 1)>());
            size++;
        }
         */
        std::cout << "before allocation" << std::endl;
        void *answer = l->allocate(alloc_size);
        std::cout << "after allocation" << std::endl;
        return answer;
    }

    void deallocate(void *elem, std::size_t alloc_size) {
        std::cout << "before deallocation" << std::endl;
        return l->deallocate(elem, alloc_size);
    }

    ~SeglistAllocator() {
        delete l;
    }
};

}

#endif // SEGLISTALLOCATOR