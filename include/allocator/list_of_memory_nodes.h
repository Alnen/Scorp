#include <type_traits>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <vector>

template <int power>
class ListOfMemoryNodes{

    struct MemoryNode { // union
        unsigned char storage[(int)pow(2, power)];
        MemoryNode *next; // first chunks of memory form a stack; pointer to the next (or nullptr)
    };

    MemoryNode *first = nullptr; // the topmost first chunk of memory (or nullptr)
    //int size = 10; // size of the last allocated pool of memory
    std::vector<MemoryNode*> vector_of_starts;

public:
    int size = 10;
    ListOfMemoryNodes<power - 1>* l;
    const int memory_unit = (int)pow(2, power);
    const int memory_unit_prev = (int)pow(2, power - 1);

    ListOfMemoryNodes(){
        std::cout << "In constructor of list with power " << power << std::endl;

        l = new ListOfMemoryNodes<power - 1>();
        first = new MemoryNode[size];
        vector_of_starts.push_back(first);

        for (int i = 0; i < size; ++i) {
            first[i].next = &first[i+1];
        }
        first[size-1].next = nullptr;
    }

    ~ListOfMemoryNodes(){

        std::cout << "In destructor of list with power " << power << std::endl;
        for(unsigned i = 0; i < vector_of_starts.size();i++){
            delete[] vector_of_starts[i];
        }
        vector_of_starts.clear();

        delete l;

    }

    void* allocate(std::size_t alloc_size){
        std::cout << "FINDING PROPER SIZE: memory unit size is " << memory_unit << std::endl;
        if((alloc_size <= memory_unit) && (alloc_size > memory_unit_prev))
        {
            std::cout << "This list memory unit size is " << memory_unit << std::endl;
            // std::cout << "This list size is " << size << std::endl;

            MemoryNode* result;
            if (first)
            {
                std::cout << "First is not nullptr" << std::endl;
                result = first;
                first = first->next;
            }
            else
            {
                std::cout << "First is  nullptr" << std::endl;
                MemoryNode* new_array = new MemoryNode[size];
                for (int i = 0; i < size; ++i) {
                    new_array[i].next = &new_array[i+1];
                }
                new_array[size-1].next = nullptr;

                //node->next = nullptr;

                first = new_array;
                vector_of_starts.push_back(first);

                //  first = new MemoryNode;
                result = first;
                //  first->next = nullptr;
            }

            return static_cast<void*>(result);
        }
        else
        {
            return l->allocate(alloc_size);
        }
    }

    void deallocate(void* elem, std::size_t alloc_size)
    {
        std::cout << "In deallocate function" << std::endl;

        if((alloc_size <= memory_unit) && (alloc_size > memory_unit_prev))
        {

            MemoryNode* node = static_cast<MemoryNode*>(elem);
            node->next = first;
            first = node;

        }
        else
        {
            l->deallocate(elem, alloc_size);
        }

        std::cout << "After deallocate function" << std::endl;
    }
};

template <>
class ListOfMemoryNodes<1>{

    union MemoryNode
    { // union
        unsigned char storage[2];
        MemoryNode *next; // first chunks of memory form a stack; pointer to the next (or nullptr)
    };

    MemoryNode *first = nullptr; // the topmost first chunk of memory (or nullptr)

    // MemoryNode* init = nullptr;
    std::vector<MemoryNode*> vector_of_starts;
    //int size = 10; // size of the last allocated pool of memory

public:
    int size = 10;
    const int memory_unit = 2;

    ListOfMemoryNodes()
    {
        std::cout << "In constructor of list with power " << 1 << std::endl;
        first = new MemoryNode[size];
        vector_of_starts.push_back(first);

        for (int i = 0; i < size; ++i) {
            first[i].next = &first[i+1];
        }
        first[size-1].next = nullptr;
    }

    ~ListOfMemoryNodes()
    {
        std::cout << "In destructor of list with power " << 1 << std::endl;
        for(unsigned i = 0; i < vector_of_starts.size();i++){
            delete[] vector_of_starts[i];
        }
        vector_of_starts.clear();
        std::cout << "End of destructor of list with power " << 1 << std::endl;

    }

    void* allocate(std::size_t alloc_size)
    {
        std::cout << "FINDING PROPER SIZE: memory unit size is " << memory_unit << std::endl;
        std::cout << "This list memory unit size is " << memory_unit << std::endl;
        // std::cout << "This list size is " << size << std::endl;

        MemoryNode* result;

        if(first)
        {
            std::cout << "First is not nullptr" << std::endl;
            result = first;
            first = first->next;

        }
        else
        {
            std::cout << "First is  nullptr" << std::endl;
            MemoryNode* new_array = new MemoryNode[size];
            for (int i = 0; i < size; ++i) {
                new_array[i].next = &new_array[i+1];
            }
            new_array[size-1].next = nullptr;

            //node->next = nullptr;

            first = new_array;
            vector_of_starts.push_back(first);

            // first = new MemoryNode;
            result = first;
            // first->next = nullptr;
        }

        return static_cast<void*>(result);
    }

    void deallocate(void* elem, std::size_t alloc_size)
    {
        std::cout << "In deallocate function" << std::endl;

        MemoryNode* node = static_cast<MemoryNode*>(elem);
        node->next = first;
        first = node;

        std::cout << "After deallocate function" << std::endl;
    }

};
