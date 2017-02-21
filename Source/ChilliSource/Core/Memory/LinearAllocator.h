//
// The MIT License(MIT)
// 
// Copyright(c) 2015 Ian Copland
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

#ifndef _CHILLISOURCE_CORE_MEMORY_LINEARALLOCATOR_H_
#define _CHILLISOURCE_CORE_MEMORY_LINEARALLOCATOR_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Memory/IAllocator.h>

#include <vector>

namespace ChilliSource
{
    /// A linear allocator which allocates from a large buffer by simply moving the
    /// next allocation pointer through the buffer by the size of the allocation. All 
    /// allocations are 'deallocated' at the same time by resetting the allocation 
    /// pointer back to the start of the buffer.
    ///
    /// A LinearAllocator can be backed by other allocator types, from which the buffer
    /// will be allocated, otherwise it's allocated from the free store.
    ///
    /// Note that this is not thread-safe and should not be accessed from multiple
    /// threads at the same time.
    ///
    class LinearAllocator final : public IAllocator
    {
    public:
        /// Initialises a new Linear Allocator with the given buffer size. The buffer will be allocated
        /// from the free store.
        ///
        /// @param bufferSize
        ///     The size of the buffer.
        /// 
        LinearAllocator(std::size_t bufferSize) noexcept;

        /// Initialises a new Linear Allocator with the given buffer size. The buffer will be allocated
        /// from the given parent allocator.
        ///
        /// @param parentAllocator
        ///     The allocator from which the buffer will be allocated.
        /// @param bufferSize
        ///     The size of the buffer.
        /// 
        LinearAllocator(IAllocator& parentAllocator, std::size_t bufferSize) noexcept;

        /// This thread-safe.
        ///
        /// @return The maximum allocation size from this allocator. This will always be 
        /// the size of the buffer.
        ///
        std::size_t GetMaxAllocationSize() const noexcept override { return GetBufferSize(); }

        /// This thread-safe.
        ///
        /// @return The size of the buffer. 
        ///
        std::size_t GetBufferSize() const noexcept { return m_bufferSize; }

        /// @return The number of bytes which are free in the buffer. 
        ///
        std::size_t GetRemainingSpace() const noexcept;

        /// Allocates a new block of memory of the requested size. If there is no space left in the
        /// buffer for the alloaction then this will assert.
        ///
        /// @param allocationSize
        ///     The size of the allocation.
        ///
        /// @return The allocated memory.
        ///
        void* Allocate(std::size_t allocationSize) noexcept override;

        /// Decriments the allocation count. This is checked when resetting to ensure that all previously
        /// allocated memory has been deallocated.
        ///
        /// @param pointer
        ///     The pointer to deallocate.
        /// @param allocationSize
        ///     Size of the initial allocation
        ///
        void Deallocate(void* pointer, std::size_t allocationSize) noexcept override;

        /// Evaluates whether or not the given pointer was allocated from this linear
        /// allocator.
        ///
        /// @param pointer
        ///        The pointer.
        ///
        /// @return Whether or not the pointer was allocated from this allocator.
        ///
        bool Contains(void* pointer) noexcept;

        /// Resets the buffer, allowing all previously allocated memory to be reused. Deallocate() must
        /// have been called for all allocated blocks prior to reset() being called.
        /// 
        void Reset() noexcept;

        ~LinearAllocator() noexcept;

    private:
        LinearAllocator(LinearAllocator&) = delete;
        LinearAllocator& operator=(LinearAllocator&) = delete;
        LinearAllocator(LinearAllocator&&) = delete;
        LinearAllocator& operator=(LinearAllocator&&) = delete;

        const std::size_t m_bufferSize;

        IAllocator* m_parentAllocator;

        std::uint8_t* m_buffer;
        std::uint8_t* m_nextPointer = nullptr;

        std::size_t m_activeAllocationCount = 0;
    };
}

#endif
