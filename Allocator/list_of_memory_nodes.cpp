#include <type_traits>
#include <stdlib.h>
#include <math.h>
#include <iostream>

template <int power>
class ListOfMemoryNodes{


    union MemoryNode { // union
        unsigned char storage[(int)pow(2, power)];
        MemoryNode *next; // first chunks of memory form a stack; pointer to the next (or nullptr)
    };

    MemoryNode *first = nullptr; // the topmost first chunk of memory (or nullptr)
    int size = 10; // size of the last allocated pool of memory


public:
    ListOfMemoryNodes<power - 1>* l;
    const int memory_unit = (int)pow(2, power);
    const int memory_unit_prev = (int)pow(2, power - 1);
    ListOfMemoryNodes(){
        l = new ListOfMemoryNodes<power - 1>();
        first = new MemoryNode[size];

        for (int i = 0; i < size; ++i) {
            first[i].next = &first[i+1];
        }
        first[size-1].next = nullptr;
    }
    ~ListOfMemoryNodes(){
        auto p = first;
        while (p) {
            auto tmp = p;
            p = p->next;
            delete tmp;
        }
        first = nullptr;
        delete l;
    }
    void* allocate(std::size_t alloc_size){

        std::cout << "FINDING PROPER SIZE: memory unit size is " << memory_unit << std::endl;
        if((alloc_size <= memory_unit) && (alloc_size > memory_unit_prev)){

        std::cout << "This list memory unit size is " << memory_unit << std::endl;
        std::cout << "This list size is " << size << std::endl;

        MemoryNode* result = first;

                if(result){
                    first = first->next;
                } else {
                    first = new MemoryNode;
                    first->next = nullptr;
                }

        return static_cast<void*>(result);

        } else {
            return l->allocate(alloc_size);
        }
    }
    void deallocate(void* elem, std::size_t alloc_size){
        if((alloc_size <= memory_unit) && (alloc_size > memory_unit_prev)){

            MemoryNode* node = static_cast<MemoryNode*>(elem);
            node->next = first;
            first = node;

        } else {
            l->deallocate(elem, alloc_size);
        }
    }

};

template <>
class ListOfMemoryNodes<1>{


    union MemoryNode { // union
        unsigned char storage[2];
        MemoryNode *next; // first chunks of memory form a stack; pointer to the next (or nullptr)
    };

    MemoryNode *first = nullptr; // the topmost first chunk of memory (or nullptr)
    int size = 10; // size of the last allocated pool of memory

public:
    const int memory_unit = 2;
    ListOfMemoryNodes(){
        first = new MemoryNode[size];

        for (int i = 0; i < size; ++i) {
            first[i].next = &first[i+1];
        }
        first[size-1].next = nullptr;
    }
    ~ListOfMemoryNodes(){
        auto p = first;
        while (p) {
            auto tmp = p;
            p = p->next;
            delete tmp;
        }
        first = nullptr;
    }
    void* allocate(std::size_t alloc_size){

        std::cout << "FINDING PROPER SIZE: memory unit size is " << memory_unit << std::endl;
        std::cout << "This list memory unit size is " << memory_unit << std::endl;
        std::cout << "This list size is " << size << std::endl;

        MemoryNode* result = first;

                if(result){
                    first = first->next;
                } else {
                    first = new MemoryNode;
                    first->next = nullptr;
                }

        return static_cast<void*>(result);

    }
    void deallocate(void* elem, std::size_t alloc_size){

        MemoryNode* node = static_cast<MemoryNode*>(elem);
        node->next = first;
        first = node;
    }

};
