#ifndef SEGLISTALLOCATOR
#define SEGLISTALLOCATOR

#include <map>
#include <forward_list>
#include <cstdlib>
#include <iostream>

namespace allocator
{

class CacheAllocator
{
public:
    static const size_t baseChunkSize = 100;
    static const size_t basePowerOfTwo = 1;
    static const size_t numberOfAllocators = 10;
    static const size_t numberOfChachedElementsInBucket = 100;

    class AllocatorInfo
    {
    public:
        AllocatorInfo() = delete;
        AllocatorInfo(const AllocatorInfo&) = delete;
        AllocatorInfo& operator=(const AllocatorInfo&) = delete;

        AllocatorInfo(AllocatorInfo&& other):
            m_chunkSize(other.m_chunkSize),
            m_numberOfCachedElements(other.m_numberOfCachedElements),
            m_currentNumberOfUsedNodes(other.m_currentNumberOfUsedNodes),
            m_memory(std::move(other.m_memory))
        {
        }

        AllocatorInfo& operator=(AllocatorInfo&& other)
        {
            if (m_chunkSize != other.m_chunkSize
                && m_numberOfCachedElements != other.m_numberOfCachedElements)
            {
                throw std::runtime_error("");
            }

            m_currentNumberOfUsedNodes = other.m_currentNumberOfUsedNodes;
            m_memory = std::move(other.m_memory);
            return *this;
        }

        AllocatorInfo(size_t chunkSize, size_t numberOfCachedElements):
                m_chunkSize(chunkSize),
                m_numberOfCachedElements(numberOfCachedElements),
                m_currentNumberOfUsedNodes(0)
        {
            for (size_t i = 0; i < m_numberOfCachedElements; ++i)
            {
                m_memory.emplace_front(malloc(m_chunkSize));
            }
        }

        void* getChunkOfMemory()
        {
            ++m_currentNumberOfUsedNodes;
            if (!m_memory.empty())
            {
                void* chunk = m_memory.front();
                m_memory.pop_front();
                return chunk;
            }
            else
            {
                std::cout << "in " << m_chunkSize << " " << m_currentNumberOfUsedNodes << std::endl;
                return malloc(m_chunkSize);
            }
        }

        void putChunkOfMemory(void* memory)
        {
            if (m_currentNumberOfUsedNodes < m_numberOfCachedElements)
            {
                m_memory.emplace_front(memory);
            }
            else
            {
                std::cout << "out " << m_chunkSize << " " << (int)m_currentNumberOfUsedNodes << std::endl;
                free(memory);
            }
            --m_currentNumberOfUsedNodes;
        }

        ~AllocatorInfo()
        {
            for (void* memoryChunk : m_memory)
            {
                free(memoryChunk);
            }
        }

        bool operator<(const AllocatorInfo& other) const
        {
            return m_chunkSize < other.m_chunkSize;
        }

    private:
        const size_t m_chunkSize;
        const size_t m_numberOfCachedElements;
        size_t m_currentNumberOfUsedNodes;
        std::forward_list<void*> m_memory;
    };

    CacheAllocator(const CacheAllocator &) = delete;
    CacheAllocator(CacheAllocator &&) = delete;
    allocator::CacheAllocator& operator=(const CacheAllocator &) = delete;
    allocator::CacheAllocator& operator=(CacheAllocator &&) = delete;

    static CacheAllocator& getInstance() {
        static CacheAllocator instance;
        return instance;
    }

    void *allocate(std::size_t alloc_size) {
        auto allocatorIterator = m_allocators.lower_bound(alloc_size);
        if (allocatorIterator != m_allocators.end())
        {
            return allocatorIterator->second.getChunkOfMemory();
        }
        else
        {
            return malloc(alloc_size);
        }
    }

    void deallocate(void* elem, std::size_t alloc_size) {
        auto allocatorIterator = m_allocators.lower_bound(alloc_size);
        if (allocatorIterator != m_allocators.end())
        {
            return allocatorIterator->second.putChunkOfMemory(elem);
        }
        else
        {
            return free(elem);
        }
    }

    ~CacheAllocator() {
    }

private:
    CacheAllocator() {
        size_t powerOfTwo = basePowerOfTwo;
        for (size_t i = 0; i < numberOfAllocators; ++i) {
            size_t size = powerOfTwo * baseChunkSize;
            m_allocators.emplace(size, AllocatorInfo(size, numberOfChachedElementsInBucket));
            powerOfTwo *= 2;
        }
    }

    std::map<size_t, AllocatorInfo> m_allocators;
};

}

#endif // SEGLISTALLOCATOR