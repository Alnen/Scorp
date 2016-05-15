#include <type_traits>
#include <stdlib.h>

class IListOfMemoryNodes{
    virtual void* allocate() = 0;
    virtual void deallocate(void* elem) = 0;
};


template <size_t memory_unit_size>
class ListOfMemoryNodes : public IListOfMemoryNodes{


    union MemoryNode { // union
        typename std::aligned_storage<sizeof(memory_unit_size), alignof(memory_unit_size)>::type storage;
        MemoryNode *next; // first chunks of memory form a stack; pointer to the next (or nullptr)
    };

    MemoryNode *first = nullptr; // the topmost first chunk of memory (or nullptr)
    const int size = 10; // size of the last allocated pool of memory

public:
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
    void* allocate(){

        MemoryNode* result = first;

        if(result){
            first = first->next;
        } else {
            first = new MemoryNode;
            first->next = nullptr;
        }

        return static_cast<void*>(result);

    }
    void deallocate(void* elem){
        MemoryNode* node = static_cast<MemoryNode*>(elem);
        node->next = first;
        first = node;
    }

};
