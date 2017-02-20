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

#ifndef _CHILLISOURCE_CORE_MEMORY_OBJECTPOOL_H_
#define _CHILLISOURCE_CORE_MEMORY_OBJECTPOOL_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Memory/IAllocator.h>

#include <queue>

namespace ChilliSource
{
    /// Allows allocation of objects of a fixed size from a pre allocated memory block.
    /// Instead of being deleted objects are returned to the pool.
    ///
    /// Pool is created with one of the following policies:
    /// * FIXED: Does not expand and will assert if out of memory
    /// * EXPAND: Once the initial limit is reached the pool will expand
    /// * OVERWRITE: Once the limit is reached future allocations will overwrite older allocations
    ///
    /// Note that this is not thread-safe and should not be accessed from multiple
    /// threads at the same time.
    ///
    template <typename T> class ObjectPool final
    {
    public:
        
        enum class LimitPolicy
        {
            k_fixed,
            k_expand,
            k_overwrite
        };
        
        /// Initialises the pool with the given number of objects. The buffer will be allocated
        /// from the free store.
        ///
        /// @param numObjects
        ///     The number of objects in the pool
        /// 
        ObjectPool(u32 numObjects) noexcept;

        /// Allocates a new object from the pool (if the limit is reached will obey the limit policy)
        ///
        /// @param allocationSize
        ///     The size of the allocation.
        ///
        /// @return The allocated object (equivalent to calling new T()).
        ///
        T* Allocate() noexcept;

        /// Returns the memory held by the object to the pool
        ///
        /// @param pointer
        ///     The pointer to deallocate.
        ///
        void Deallocate(T* pointer) noexcept;


        /// Resets the buffer, allowing all previously allocated memory to be reused. Deallocate() must
        /// have been called for all allocated blocks prior to reset() being called.
        /// 
        void Reset() noexcept;

        ~ObjectPool() noexcept;

    private:
        ObjectPool(ObjectPool&) = delete;
        ObjectPool& operator=(ObjectPool&) = delete;
        ObjectPool(ObjectPool&&) = delete;
        ObjectPool& operator=(ObjectPool&&) = delete;

        T* m_buffer;
        T* m_nextFree;
    };
    
    //-----------------------------------------------------------------------------
    template <typename T> ObjectPool<T>::ObjectPool(u32 numObjects) noexcept
    {
        m_buffer = new T[numObjects];
    }
    
    //-----------------------------------------------------------------------------
    template <typename T> T* ObjectPool<T>::Allocate() noexcept
    {
        return nullptr;
    }
    
    //-----------------------------------------------------------------------------
    template <typename T> void ObjectPool<T>::Deallocate(T* pointer) noexcept
    {
        
    }
    
    //-----------------------------------------------------------------------------
    template <typename T> void ObjectPool<T>::Reset() noexcept
    {
        
    }
    
    //-----------------------------------------------------------------------------
    template <typename T> ObjectPool<T>::~ObjectPool()
    {
        delete[] m_buffer;
    }

#endif
