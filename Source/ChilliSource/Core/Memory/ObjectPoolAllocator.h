//
// The MIT License(MIT)
// 
// Copyright(c) 2017 Tag Games Ltd
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#ifndef _CHILLISOURCE_CORE_MEMORY_OBJECTPOOLALLOCATOR_H_
#define _CHILLISOURCE_CORE_MEMORY_OBJECTPOOLALLOCATOR_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Memory/IAllocator.h>

#include <mutex>
#include <vector>

namespace ChilliSource
{
    enum class ObjectPoolAllocatorLimitPolicy
    {
        k_fixed,
        k_expand
    };

    /// Allows allocation of objects of a fixed size from a pre allocated memory block.
    /// Instead of being deleted objects are returned to the pool.
    ///
    /// Pool is created with one of the following policies:
    /// * FIXED: Does not expand and will assert if out of memory (NOTE: Contiguous in memory)
    /// * EXPAND: Once the initial limit is reached the pool will expand (NOTE: Expanding pools are not guaranteed to be contiguous in memory as the buffers are paged on the inital size).
    ///
    /// Note that this is thread-safe
    ///
    template <typename T> class ObjectPoolAllocator final : public IAllocator
    {
    public:
        
        /// Initialises the pool with the given number of objects. The buffer will be allocated
        /// from the free store.
        ///
        /// @param numObjects
        ///     The number of objects in the pool
        /// @param limitPolicy
        ///     How to handle the case where pool limit is reached
        /// 
        ObjectPoolAllocator(std::size_t numObjects, ObjectPoolAllocatorLimitPolicy limitPolicy = ObjectPoolAllocatorLimitPolicy::k_fixed) noexcept;
        
        
        /// @return the maximum allocation size allowed by the allocator.
        ///
        std::size_t GetMaxAllocationSize() const noexcept override { return m_capacityObjects * sizeof(T); };
        
        /// Should be used to allocate only a single object and will
        /// assert if the given size is not aligned with sizeof(T).
        ///
        /// If the limit is reached will obey the limit policy
        ///
        /// NOTE: Used via STL container allocation interface and is NOT constructed using placement new
        ///
        /// @param allocationSize
        ///     The size of the allocation must aligned with sizeof(T)
        ///
        /// @return The allocated memory (not yet constructed).
        ///
        void* Allocate(std::size_t allocationSize) noexcept override;

        /// Allocates a new object from the pool (if the limit is reached will obey the limit policy)
        ///
        /// NOTE: Use this if you are using the pool directly and not via STL containers
        ///
        /// @return The allocated object (equivalent to calling new T()).
        ///
        T* Allocate() noexcept;
        
        /// Returns the memory held by the object to the pool. It must have been allocated from
        /// this pool or will assert
        ///
        /// NOTE: Used via STL container allocation interface and is not destructed
        ///
        /// @param pointer
        ///     The pointer to deallocate.
        /// @param allocationSize
        ///     Initial allocation size
        ///
        void Deallocate(void* pointer, std::size_t allocationSize) noexcept override;
        
        /// Returns the memory held by the object to the pool. It must have been allocated from
        /// this pool or will assert.
        ///
        /// NOTE: Use this if you are using the pool directly and not via STL containers
        ///
        /// @param pointer
        ///     The pointer to deallocate and destruct
        ///
        void Deallocate(T* pointer) noexcept;


        /// Resets the buffer, allowing all previously allocated memory to be reused. Deallocate() must
        /// have been called for all allocated blocks prior to reset() being called.
        /// 
        void Reset() noexcept;

        ~ObjectPoolAllocator() noexcept;

    private:
        ObjectPoolAllocator(ObjectPoolAllocator&) = delete;
        ObjectPoolAllocator& operator=(ObjectPoolAllocator&) = delete;
        ObjectPoolAllocator(ObjectPoolAllocator&&) = delete;
        ObjectPoolAllocator& operator=(ObjectPoolAllocator&&) = delete;
        
        /// @return TRUE if the given memory is owned by this pool
        ///
        bool IsOwnedByPool(T* pointer) const noexcept;
        
        /// @return TRUE if the given memory is currently free to allocate
        ///
        bool IsOnFreeStore(T* pointer) const noexcept;
        
        /// Adds a new page buffer at a fraction of the size of the original
        ///
        /// @param minObjects
        ///     Min number of objects in the new page
        ///
        void Expand(std::size_t minObjects) noexcept;
        
        /// Moves all objects to the free store
        ///
        void PopulateFreeStore() noexcept;
        
        ObjectPoolAllocatorLimitPolicy m_limitPolicy;
        std::size_t m_activeAllocationCount = 0;
		std::size_t m_capacityObjects;

        std::mutex m_mutex;
        
        //Buffers are paged to accomodate expansion without invalidating the existing pointers
        //Fixed size only has a single buffer
        std::vector<T*> m_buffers;
        std::vector<std::size_t> m_capacities;
        T** m_freeStore;
        T** m_freeStoreHead;
    };
    
    //-----------------------------------------------------------------------------
    template <typename T> ObjectPoolAllocator<T>::ObjectPoolAllocator(std::size_t numObjects, ObjectPoolAllocatorLimitPolicy limitPolicy) noexcept
    : m_limitPolicy(limitPolicy), m_capacityObjects(numObjects)
    {
        CS_ASSERT(numObjects > 0, "Cannot create a pool of size 0");
        
        //Create the initial buffer page
        m_buffers.push_back((T*)malloc(sizeof(T) * numObjects));
        m_capacities.push_back(numObjects);
        
        m_freeStore = (T**)malloc(sizeof(T*) * numObjects);
        
        Reset();
    }
    
    //-----------------------------------------------------------------------------
    template <typename T> void* ObjectPoolAllocator<T>::Allocate(std::size_t allocationSize) noexcept
    {
        CS_ASSERT(allocationSize % sizeof(T) == 0, "Allocations must be sizeof(T) aligned");
        
        std::size_t numToAllocate = allocationSize / sizeof(T);
        
        std::unique_lock<std::mutex> lock(m_mutex);
        
		std::size_t remaining = m_capacityObjects - m_activeAllocationCount;
        
        if(remaining < numToAllocate)
        {
            switch(m_limitPolicy)
            {
                case ObjectPoolAllocatorLimitPolicy::k_fixed:
                    CS_LOG_FATAL("ObjectPool out of memory. Allocate more upfront or change to an expansion policy");
                    return nullptr;
                case ObjectPoolAllocatorLimitPolicy::k_expand:
                    Expand(numToAllocate - remaining);
                    break;
            }
        }
        
        T* free = *m_freeStoreHead;
        m_freeStoreHead += numToAllocate;
        m_activeAllocationCount += numToAllocate;
        return free;
    }
    
    //-----------------------------------------------------------------------------
    template <typename T> T* ObjectPoolAllocator<T>::Allocate() noexcept
    {
        void* memory = Allocate(sizeof(T));
        T* object = new (memory) T();
        return object;
    }
    
    //-----------------------------------------------------------------------------
    template <typename T> void ObjectPoolAllocator<T>::Deallocate(void* pointer, std::size_t allocationSize) noexcept
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        
        CS_ASSERT(allocationSize % sizeof(T) == 0, "Allocations must be sizeof(T) aligned");
        CS_ASSERT(IsOwnedByPool((T*)pointer) == true, "Pointer you are trying to deallocate from pool is not managed by this pool");
        CS_ASSERT(IsOnFreeStore((T*)pointer) == false, "Pointer you are trying to deallocate from pool has not been allocated");
        CS_ASSERT(m_activeAllocationCount > 0, "The pool has no active allocations.")
        
        auto numObjects = allocationSize / sizeof(T);
        
        m_freeStoreHead -= numObjects;
        m_activeAllocationCount -= numObjects;
        *m_freeStoreHead = (T*)pointer;
    }
    
    //-----------------------------------------------------------------------------
    template <typename T> void ObjectPoolAllocator<T>::Deallocate(T* pointer) noexcept
    {
        pointer->~T();
        Deallocate((void*)pointer, sizeof(T));
    }
    
    //-----------------------------------------------------------------------------
    template <typename T> void ObjectPoolAllocator<T>::Reset() noexcept
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        
        CS_ASSERT(m_activeAllocationCount == 0, "Cannot reset before all allocations have been deallocated.");
        
        m_freeStoreHead = m_freeStore;
        
        PopulateFreeStore();
    }
    
    //-----------------------------------------------------------------------------
    template <typename T> bool ObjectPoolAllocator<T>::IsOwnedByPool(T* pointer) const noexcept
    {
        for(u32 i=0; i<m_buffers.size(); ++i)
        {
            if(pointer >= m_buffers[i] && pointer < m_buffers[i] + m_capacities[i])
            {
                return true;
            }
        }
        
        return false;
    }
    
    //-----------------------------------------------------------------------------
    template <typename T> bool ObjectPoolAllocator<T>::IsOnFreeStore(T* pointer) const noexcept
    {
        for(auto i=m_freeStoreHead; i<m_freeStore+m_capacityObjects; ++i)
        {
            if(*i == pointer)
                return true;
        }
        
        return false;
    }
    
    //-----------------------------------------------------------------------------
    template <typename T> void ObjectPoolAllocator<T>::Expand(std::size_t minObjects) noexcept
    {
        //We use a fraction of the total buffer size for the new buffer
        auto numObjects = std::max(minObjects, (std::size_t)m_capacityObjects/4);
        T* buffer = (T*)malloc(sizeof(T) * numObjects);
        m_buffers.push_back(buffer);
        m_capacities.push_back(numObjects);
        m_capacityObjects += numObjects;
        
        //Delete the old free store and rebuild with the new buffer appended
        auto headOffset = m_freeStoreHead - m_freeStore;
        free(m_freeStore);
        
        m_freeStore = (T**)malloc(sizeof(T*) * m_capacityObjects);
        m_freeStoreHead = m_freeStore + headOffset;
        
        PopulateFreeStore();
    }
    
    //-----------------------------------------------------------------------------
    template <typename T> void ObjectPoolAllocator<T>::PopulateFreeStore() noexcept
    {
        u32 freeStoreIndex = 0;
        
        for(u32 i=0; i<m_buffers.size(); ++i)
        {
            for(std::size_t j=0; j<m_capacities[i]; ++j, ++freeStoreIndex)
            {
                m_freeStore[freeStoreIndex] = m_buffers[i] + j;
            }
        }
    }
    
    //-----------------------------------------------------------------------------
    template <typename T> ObjectPoolAllocator<T>::~ObjectPoolAllocator() noexcept
    {
        Reset();
        
        free(m_freeStore);
        
        for(u32 i=0; i<m_buffers.size(); ++i)
        {
            free(m_buffers[i]);
        }
    }
}

#endif
