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

#ifndef _CHILLISOURCE_CORE_MEMORY_PAGEDLINEARALLOCATOR_H_
#define _CHILLISOURCE_CORE_MEMORY_PAGEDLINEARALLOCATOR_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Memory/LinearAllocator.h>
#include <ChilliSource/Core/Memory/UniquePtr.h>

#include <vector>

namespace ChilliSource
{
    /// A paged version of the linear allocator. This allocates from a large buffer by 
    /// simply moving the next allocation pointer through the buffer by the size of the
    /// allocation. All allocations are 'deallocated' at the same time by resetting the 
    /// allocation pointer back to the start of the buffer. If an allocation will not
    /// fit in the current buffer then a new page is allocated. Once a page has been
    /// allocated it will not be deallocated until ResetAndShink() is called.
    ///
    /// A PagedLinearAllocator can be backed by other allocator types, from which pages 
    /// will be allocated, otherwise they are allocated from the free store.
    ///
    /// Note that this is not thread-safe and should not be accessed from multiple
    /// threads at the same time.
    ///
    class PagedLinearAllocator final : public IAllocator
    {
    public:
        static constexpr std::size_t k_defaultPageSize = 4 * 1024;

        /// Initialises a new PagedLinearAllocator with the given page size. Pages will be allocated
        /// from the free store.
        ///
        /// @param pageSize
        ///     The size of a page.
        /// 
        PagedLinearAllocator(std::size_t pageSize = k_defaultPageSize) noexcept;

        /// Initialises a new PagedLinearAllocator with the given page size. Pages will be allocated
        /// from the given parent allocator.
        ///
        /// @param parentAllocator
        ///     The allocator from which pages will be allocated.
        /// @param pageSize
        ///     The size of a page.
        /// 
        PagedLinearAllocator(IAllocator& parentAllocator, std::size_t pageSize = k_defaultPageSize) noexcept;

        /// This thread-safe.
        ///
        /// @return The maximum allocation size from this allocator. This will always be 
        /// the size of a page.
        ///
        std::size_t GetMaxAllocationSize() const noexcept override { return GetPageSize(); }

        /// This thread-safe.
        ///
        /// @return The size of a page. 
        ///
        std::size_t GetPageSize() const noexcept { return m_pageSize; }

        /// @return The number of pages in the allocator.
        ///
        std::size_t GetNumPages() const noexcept;

        /// Allocates a new block of memory of the requested size. If there is no space left in the
        /// buffer for the alloaction then a new page will be allocated. Allocations must be smaller
        /// than the size of a single page.
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

        /// Resets the buffer, allowing all previously allocated memory to be reused. Deallocate() must
        /// have been called for all allocated blocks prior to reset() being called.
        /// 
        void Reset() noexcept;

        /// Resets the buffer and deallocates any extra pages which have been allocated.
        /// 
        void ResetAndShrink() noexcept;

        ~PagedLinearAllocator() noexcept;

    private:
        PagedLinearAllocator(PagedLinearAllocator&) = delete;
        PagedLinearAllocator& operator=(PagedLinearAllocator&) = delete;
        PagedLinearAllocator(PagedLinearAllocator&&) = delete;
        PagedLinearAllocator& operator=(PagedLinearAllocator&&) = delete;

        const std::size_t m_pageSize;

        IAllocator* m_parentAllocator = nullptr;

        union
        {
            std::vector<std::unique_ptr<LinearAllocator>> m_freeStoreLinearAllocators;
            std::vector<UniquePtr<LinearAllocator>> m_parentAllocatorLinearAllocators;
        };
    };
}

#endif
