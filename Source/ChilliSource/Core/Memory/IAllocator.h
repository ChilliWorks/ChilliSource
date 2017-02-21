//
// The MIT License(MIT)
// 
// Copyright(c) 2016 Ian Copland
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

#ifndef _CHILLISOURCE_CORE_MEMORY_IALLOCATOR_H_
#define _CHILLISOURCE_CORE_MEMORY_IALLOCATOR_H_

#include <ChilliSource/ChilliSource.h>

#include <cstddef>
#include <cstdint>

namespace ChilliSource
{
    /// An interface for all allocator types. This provides the allocation and deallocation
    /// methods which all allocators should implement.
    ///
    /// Implementing this interface allows the allocator to be used with the STD types like
    /// string and vector.
    ///
    /// Whether or not the allocator is thread-safe is dependant on the concrete type.
    ///
    class IAllocator
    {
    public:
        /// @return the maximum allocation size allowed by the allocator.
        ///
        virtual std::size_t GetMaxAllocationSize() const noexcept = 0;

        /// Allocates a new block of memory of the requested size. Note that the underlying
        /// implemention may allocate more memory than has been requested.
        ///
        /// @param allocationSize
        ///     The size of the allocation.
        ///
        /// @return The allocated memory.
        ///
        virtual void* Allocate(std::size_t allocationSize) noexcept = 0;

        /// Deallocates the given memory. This must have been allocated via this allocator.
        /// Note that the underlying implementation may not actually deallocate the memory
        /// at this point, though deallocate must still be called.
        ///
        /// @param pointer
        ///     The pointer to deallocate.
        /// @param allocationSize
        ///     Size of the initial allocation
        ///
        virtual void Deallocate(void* pointer, std::size_t allocationSize) noexcept = 0;

        virtual ~IAllocator() noexcept { }
    };
}

#endif
