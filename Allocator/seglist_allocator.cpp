#include <iostream>
#include <list>
#include <vector>
#include <math.h>
#include "list_of_memory_nodes.cpp"

class SeglistAllocator {

    const static int max_power_of_two = 10;
    const static int MAX_SIZE = pow(2, max_power_of_two) + 1;

    static int* temp_indexes = new int[max_power_of_two];

    class ArrayOfIndexes {
      const std::vector<int> indexes;
    public:
      ArrayOfIndexes():indexes(temp_indexes, temp_indexes + max_power_of_two) {
          for(int i = 0; i < indexes.size();i++){
              indexes[i] = i + 1;
          }
      }
    } ar;

    std::vector<IListOfMemoryNodes*> lists;
    int size = 0;


    void create_list() { // allocate new pool of memory

        if(max_power_of_two > 0){
            size = max_power_of_two;
        } else {
            size = 1;
        }

        for (int i = 0; i < size; ++i) {
            lists.push_back(new ListOfMemoryNodes<ar.indexes[i]>());
        }

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

        void* allocation = nullptr;

        if(alloc_size > MAX_SIZE){
            //!!!!!!!!!!!!!!!!
            lists.push_back(new ListOfMemoryNodes<pow(2, max_power_of_two + 1)>());
            size++;
        }

        for (int i = 0; i < size; ++i) {
            if(alloc_size <= pow(2, i)){
                allocation = lists[i]->allocate();
                break;
            }
        }

        return allocation;
    }
    void deallocate(void* elem, std::size_t alloc_size) {

        for (int i = 0; i < size; ++i) {
            if(alloc_size <= pow(2, i)){
                lists[i]->deallocate(elem);
                break;
            }
        }

    }
    ~SeglistAllocator() {
        lists.clear();
    }
};
