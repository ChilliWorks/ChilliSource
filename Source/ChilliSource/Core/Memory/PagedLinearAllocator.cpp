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

#include <ChilliSource/Core/Memory/PagedLinearAllocator.h>

#include <cassert>

namespace ChilliSource
{
    //------------------------------------------------------------------------------
    PagedLinearAllocator::PagedLinearAllocator(std::size_t pageSize) noexcept
        : m_pageSize(pageSize), m_freeStoreLinearAllocators()
    {
        m_freeStoreLinearAllocators.push_back(std::unique_ptr<LinearAllocator>(new LinearAllocator(m_pageSize)));
    }

    //------------------------------------------------------------------------------
    PagedLinearAllocator::PagedLinearAllocator(IAllocator& parentAllocator, std::size_t pageSize) noexcept
        : m_pageSize(pageSize), m_parentAllocator(&parentAllocator), m_parentAllocatorLinearAllocators()
    {
        m_parentAllocatorLinearAllocators.push_back(MakeUnique<LinearAllocator>(*m_parentAllocator, *m_parentAllocator, m_pageSize));
    }

    //------------------------------------------------------------------------------
    std::size_t PagedLinearAllocator::GetNumPages() const noexcept
    {
        if (m_parentAllocator)
        {
            return m_parentAllocatorLinearAllocators.size();
        }
        else
        {
            return m_freeStoreLinearAllocators.size();
        }
    }

    //------------------------------------------------------------------------------
    void* PagedLinearAllocator::Allocate(std::size_t allocationSize) noexcept
    {
        CS_ASSERT(allocationSize <= GetMaxAllocationSize(), "Allocation size is too big.");

        if (m_parentAllocator)
        {
            for (const auto& allocator : m_parentAllocatorLinearAllocators)
            {
                if (allocator->GetRemainingSpace() >= allocationSize)
                {
                    return allocator->Allocate(allocationSize);
                }
            }

            m_parentAllocatorLinearAllocators.push_back(MakeUnique<LinearAllocator>(*m_parentAllocator, *m_parentAllocator, m_pageSize));
            return m_parentAllocatorLinearAllocators.back()->Allocate(allocationSize);
        }
        else
        {
            for (const auto& allocator : m_freeStoreLinearAllocators)
            {
                if (allocator->GetRemainingSpace() >= allocationSize)
                {
                    return allocator->Allocate(allocationSize);
                }
            }

            m_freeStoreLinearAllocators.push_back(std::unique_ptr<LinearAllocator>(new LinearAllocator(m_pageSize)));
            return m_freeStoreLinearAllocators.back()->Allocate(allocationSize);
        }
    }

    //------------------------------------------------------------------------------
    void PagedLinearAllocator::Deallocate(void* pointer, std::size_t allocationSize) noexcept
    {
        if (m_parentAllocator)
        {
            for (const auto& allocator : m_parentAllocatorLinearAllocators)
            {
                if (allocator->Contains(pointer))
                {
                    return allocator->Deallocate(pointer, allocationSize);
                }
            }
        }
        else
        {
            for (const auto& allocator : m_freeStoreLinearAllocators)
            {
                if (allocator->Contains(pointer))
                {
                    return allocator->Deallocate(pointer, allocationSize);
                }
            }
        }

        CS_LOG_FATAL("Cannot deallocate a pointer that did not originate from this allocator.");
    }

    //------------------------------------------------------------------------------
    void PagedLinearAllocator::Reset() noexcept
    {
        if (m_parentAllocator)
        {
            for (auto& allocator : m_parentAllocatorLinearAllocators)
            {
                allocator->Reset();
            }
        }
        else
        {
            for (auto& allocator : m_freeStoreLinearAllocators)
            {
                allocator->Reset();
            }
        }
    }

    //------------------------------------------------------------------------------
    void PagedLinearAllocator::ResetAndShrink() noexcept
    {
        Reset();

        if (m_parentAllocator)
        {
            while (m_parentAllocatorLinearAllocators.size() > 1)
            {
                m_parentAllocatorLinearAllocators.erase(m_parentAllocatorLinearAllocators.begin() + 1);
            }
        }
        else
        {
            while (m_freeStoreLinearAllocators.size() > 1)
            {
                m_freeStoreLinearAllocators.erase(m_freeStoreLinearAllocators.begin() + 1);
            }
        }
    }

    //------------------------------------------------------------------------------
    PagedLinearAllocator::~PagedLinearAllocator() noexcept
    {
        Reset();

        if (m_parentAllocator)
        {
            m_parentAllocatorLinearAllocators.~vector();
        }
        else
        {
            m_freeStoreLinearAllocators.~vector();
        }
    }
}
