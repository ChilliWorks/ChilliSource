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

#include <ChilliSource/Core/Memory/LinearAllocator.h>

#include <ChilliSource/Core/Memory/MemoryUtils.h>

#include <cassert>

namespace ChilliSource
{
    //------------------------------------------------------------------------------
    LinearAllocator::LinearAllocator(std::size_t pageSize) noexcept
        : m_bufferSize(pageSize), m_parentAllocator(nullptr)
    {
        m_buffer = new std::uint8_t[m_bufferSize];
        m_nextPointer = MemoryUtils::Align(m_buffer, sizeof(std::intptr_t));
    }

    //------------------------------------------------------------------------------
    LinearAllocator::LinearAllocator(IAllocator& parentAllocator, std::size_t pageSize) noexcept
        : m_bufferSize(pageSize), m_parentAllocator(&parentAllocator)
    {
        m_buffer = reinterpret_cast<std::uint8_t*>(m_parentAllocator->Allocate(m_bufferSize));
        m_nextPointer = MemoryUtils::Align(m_buffer, sizeof(std::intptr_t));
    }

    //------------------------------------------------------------------------------
    std::size_t LinearAllocator::GetRemainingSpace() const noexcept
    {
        auto freeSpace = m_bufferSize - MemoryUtils::GetPointerOffset(m_nextPointer, m_buffer);
        auto freeSpaceAligned = freeSpace & ~(sizeof(std::intptr_t) - 1);
        return freeSpaceAligned;
    }

    //------------------------------------------------------------------------------
    void* LinearAllocator::Allocate(std::size_t allocationSize) noexcept
    {
        CS_ASSERT(allocationSize <= GetRemainingSpace(), "Allocation is too big to fit in the remaining space.");

        std::uint8_t* output = m_nextPointer;
        m_nextPointer = MemoryUtils::Align(m_nextPointer + allocationSize, sizeof(std::intptr_t));

        ++m_activeAllocationCount;

        return output;
    }

    //------------------------------------------------------------------------------
    void LinearAllocator::Deallocate(void* pointer, std::size_t allocationSize) noexcept
    {
        CS_ASSERT(Contains(pointer), "Cannot deallocate a pointer that did not originate from this allocator.");

        --m_activeAllocationCount;
    }

    //------------------------------------------------------------------------------
    bool LinearAllocator::Contains(void* pointer) noexcept
    {
        return (pointer >= m_buffer && pointer < reinterpret_cast<std::uint8_t*>(m_buffer) + m_bufferSize);
    }

    //------------------------------------------------------------------------------
    void LinearAllocator::Reset() noexcept
    {
        CS_ASSERT(m_activeAllocationCount == 0, "Cannot reset before all allocations have been deallocated.");

        m_nextPointer = MemoryUtils::Align(m_buffer, sizeof(std::intptr_t));
    }

    //------------------------------------------------------------------------------
    LinearAllocator::~LinearAllocator() noexcept
    {
        Reset();

        if (m_parentAllocator)
        {
            m_parentAllocator->Deallocate(m_buffer, m_bufferSize);
        }
        else
        {
            delete[] m_buffer;
        }

        m_buffer = nullptr;
    }
}
