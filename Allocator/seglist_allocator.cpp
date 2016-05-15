#include <iostream>
#include <list>
#include <vector>
#include <math.h>
#include "list_of_memory_nodes.cpp"

class SeglistAllocator {

    const static int max_power_of_two = 10;
    const static int MAX_SIZE = pow(2, max_power_of_two);

    ListOfMemoryNodes<max_power_of_two>* l;
  //  std::vector<IListOfMemoryNodes*> lists;
    int size = 0;


    void create_list() { // allocate new pool of memory

        if(max_power_of_two > 0){
            size = max_power_of_two;
        } else {
            size = 1;
        }

        l = new ListOfMemoryNodes<max_power_of_two>();

    }
    SeglistAllocator() { // private for singleton

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

      //  void* allocation = nullptr;

     /*   if(alloc_size > MAX_SIZE){
            //!!!!!!!!!!!!!!!!
            lists.push_back(new ListOfMemoryNodes<pow(2, max_power_of_two + 1)>());
            size++;
        }*/

        return l->allocate(alloc_size);
    }
    void deallocate(void* elem, std::size_t alloc_size) {

        return l->deallocate(elem, alloc_size);

    }
    ~SeglistAllocator() {
        delete l;
    }
};
