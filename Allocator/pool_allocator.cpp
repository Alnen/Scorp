#include <iostream>
#include <list>
#include <vector>

template <size_t memory_unit_size>
class PoolAllocator {

    union MemoryNode { // union
        char data[memory_unit_size];
        MemoryNode *next; // first chunks of memory form a stack; pointer to the next (or nullptr)
    };

    MemoryNode *first = nullptr; // the topmost first chunk of memory (or nullptr)
    std::vector<MemoryNode*> allocatedNodes; // all allocated allocatedNodes of memory
    int size = 1; // size of the last allocated pool of memory
    const int MAX_SIZE = 1024;

    void new_pool() { // allocate new pool of memory
        if (size < MAX_SIZE) {
            size *= 2;
        }
        first = new MemoryNode[size];

        // form a stack of chunks of this pool
        allocatedNodes.push_back(first);
        for (int i = 0; i < size; ++i) {
            first[i].next = &first[i+1];
        }
        first[size-1].next = nullptr;
    }
    PoolAllocator() { // private for singleton
        new_pool();
    }
public:
    PoolAllocator(const PoolAllocator&) = delete; // for singleton
    static PoolAllocator& instance () { // singleton
        static PoolAllocator instance;
        return instance;
    }
    void* allocate() {
        if (!first) {
            new_pool();
        }
        MemoryNode* result = first; // allocate the topmost element (saved in first)
        first = first->next; // and pop it from the stack of first chunks
        return static_cast<void*>(result);
    }
    void deallocate(void* elem) {
        MemoryNode* node = static_cast<MemoryNode*>(elem);

        // add to the stack of chunks
        node->next = first;
        first = node;
    }
    ~PoolAllocator() {
        for (auto ptr : allocatedNodes) {
            delete ptr;
        }
    }
};
