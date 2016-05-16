#include <iostream>
#include <list>
#include <vector>
#include <math.h>
#include "list_of_memory_nodes.h"

class SeglistAllocator {

    const static int max_power_of_two = 10;
    const static int MAX_SIZE = (const int) pow(2, max_power_of_two);

    ListOfMemoryNodes<max_power_of_two>* l;
    int size = 0;

    void create_list()
    { // allocate new pool of memory
        size = max_power_of_two;
        l = new ListOfMemoryNodes<max_power_of_two>();
    }

    SeglistAllocator()
    { // private for singleton
        create_list();
    }

public:
    SeglistAllocator(const SeglistAllocator&) = delete; // for singleton
    void operator=(SeglistAllocator const&)  = delete;

    static SeglistAllocator& instance () { // singleton
        static SeglistAllocator instance;
        return instance;
    }
    void* allocate(std::size_t alloc_size) {
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
        void* answer = l->allocate(alloc_size);
        std::cout << "after allocation" << std::endl;
        return answer;
    }

    void deallocate(void* elem, std::size_t alloc_size)
    {
        std::cout << "before deallocation" << std::endl;
        return l->deallocate(elem, alloc_size);
    }

    ~SeglistAllocator()
    {
        delete l;
    }
};
