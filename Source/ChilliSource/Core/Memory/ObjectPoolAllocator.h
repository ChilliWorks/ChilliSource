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

namespace ChilliSource
{
    /// Allows allocation of objects of a fixed size from a pre allocated memory block.
    /// Instead of being deleted objects are returned to the pool.
    ///
    /// Pool is created with one of the following policies:
    /// * FIXED: Does not expand and will assert if out of memory (NOTE: Contiguous in memory)
    /// * EXPAND: Once the initial limit is reached the pool will expand (NOTE: Expanding pools are not guaranteed to be contiguous in memory as the buffers are paged on the inital size).
    ///
    /// Note that this is not thread-safe and should not be accessed from multiple
    /// threads at the same time.
    ///
    template <typename T> class ObjectPoolAllocator final : public IAllocator
    {
    public:
        
        enum class LimitPolicy
        {
            k_fixed,
            k_expand
        };
        
        /// Initialises the pool with the given number of objects. The buffer will be allocated
        /// from the free store.
        ///
        /// @param numObjects
        ///     The number of objects in the pool
        /// @param limitPolicy
        ///     How to handle the case where pool limit is reached
        /// 
        ObjectPoolAllocator(u32 numObjects, LimitPolicy limitPolicy = LimitPolicy::k_fixed) noexcept;
        
        
        /// @return the maximum allocation size allowed by the allocator.
        ///
        std::size_t GetMaxAllocationSize() const noexcept override { return m_capacityObjects * sizeof(T); };

        /// Allocates a new object from the pool (if the limit is reached will obey the limit policy)
        ///
        /// NOTE: Use this if you are using the pool directly and not via STL containers
        ///
        /// @param allocationSize
        ///     The size of the allocation.
        ///
        /// @return The allocated object (equivalent to calling new T()).
        ///
        T* Allocate() noexcept;
        
        /// Should be used to allocate only a single object and will
        /// assert if the given size if not sizeof(T).
        ///
        /// NOTE: Used via STL container allocation interface
        ///
        /// @param allocationSize
        ///     The size of the allocation must be sizeof(T)
        ///
        /// @return The allocated memory.
        ///
        void* Allocate(std::size_t allocationSize) noexcept override;
        
        /// Returns the memory held by the object to the pool. It must have been allocated from
        /// this pool or will assert
        ///
        /// NOTE: Used via STL container allocation interface
        ///
        /// @param pointer
        ///     The pointer to deallocate.
        ///
        void Deallocate(void* pointer) noexcept override;
        
        /// Returns the memory held by the object to the pool. It must have been allocated from
        /// this pool or will assert
        ///
        /// @param pointer
        ///     The pointer to deallocate.
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
        
        LimitPolicy m_limitPolicy;
        u32 m_activeAllocationCount = 0;
        u32 m_capacityObjects;

        T* m_buffer;
        T** m_freeStore;
        T** m_freeStoreHead;
    };
    
    //-----------------------------------------------------------------------------
    template <typename T> ObjectPoolAllocator<T>::ObjectPoolAllocator(u32 numObjects, LimitPolicy limitPolicy) noexcept
    : m_limitPolicy(limitPolicy), m_capacityObjects(numObjects)
    {
        m_buffer = new T[numObjects];
        m_freeStore = new T*[numObjects];
        m_freeStoreHead = m_freeStore;
    }
    
    //-----------------------------------------------------------------------------
    template <typename T> T* ObjectPoolAllocator<T>::Allocate() noexcept
    {
        if(m_activeAllocationCount == m_capacityObjects)
        {
            switch(m_limitPolicy)
            {
                case LimitPolicy::k_fixed:
                    CS_LOG_FATAL("ObjectPool out of memory. Allocate more upfront or change to an expansion policy");
                    return nullptr;
                case LimitPolicy::k_expand:
                    //TODO:
                    //Expand();
                    break;
            }
        }
        
        T* free = m_freeStoreHead++;
        ++m_activeAllocationCount;
        return free;
    }
    
    //-----------------------------------------------------------------------------
    template <typename T> void* ObjectPoolAllocator<T>::Allocate(std::size_t allocationSize) noexcept
    {
        CS_ASSERT(allocationSize == sizeof(T), "Pool can only allocate single objects of size T at a time");
        return Allocate();
    }
    
    //-----------------------------------------------------------------------------
    template <typename T> void ObjectPoolAllocator<T>::Deallocate(T* pointer) noexcept
    {
        CS_ASSERT(IsOwnedByPool(pointer) == true, "Pointer you are trying to deallocate from pool is not managed by this pool");
        CS_ASSERT(IsOnFreeStore(pointer) == false, "Pointer you are trying to deallocate from pool has not been allocated");
        CS_ASSERT(m_activeAllocationCount > 0, "The pool has no active allocations.")
        
        --m_freeStoreHead;
        --m_activeAllocationCount;
        *m_freeStoreHead = pointer;
    }
    
    //-----------------------------------------------------------------------------
    template <typename T> void ObjectPoolAllocator<T>::Deallocate(void* pointer) noexcept
    {
        Deallocate((T*)pointer);
    }
    
    //-----------------------------------------------------------------------------
    template <typename T> void ObjectPoolAllocator<T>::Reset() noexcept
    {
        CS_ASSERT(m_activeAllocationCount == 0, "Cannot reset before all allocations have been deallocated.");
        
        m_freeStoreHead = m_freeStore;
    }
    
    //-----------------------------------------------------------------------------
    template <typename T> bool ObjectPoolAllocator<T>::IsOwnedByPool(T* pointer) const noexcept
    {
        return pointer >= m_buffer && pointer < m_buffer + m_capacityObjects;
    }
    
    //-----------------------------------------------------------------------------
    template <typename T> bool ObjectPoolAllocator<T>::IsOnFreeStore(T* pointer) const noexcept
    {
        for(auto i=m_freeStore; i<m_freeStore+m_capacityObjects; ++i)
        {
            if(*i == pointer)
                return true;
        }
        
        return false;
    }
    
    //-----------------------------------------------------------------------------
    template <typename T> ObjectPoolAllocator<T>::~ObjectPoolAllocator() noexcept
    {
        Reset();
        
        delete[] m_freeStore;
        delete[] m_buffer;
    }

#endif
