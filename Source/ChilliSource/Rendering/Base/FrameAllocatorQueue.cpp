//
//  The MIT License (MIT)
//
//  Copyright (c) 2016 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#include <ChilliSource/Rendering/Base/FrameAllocatorQueue.h>

namespace ChilliSource
{
    namespace
    {
        constexpr u32 k_numAllocators = 3;
        constexpr u32 k_allocatorPageSize = 1024 * 1024;
    }
    
    //------------------------------------------------------------------------------
    FrameAllocatorQueue::FrameAllocatorQueue() noexcept
    {
        for (u32 i = 0; i < k_numAllocators; ++i)
        {
            PagedLinearAllocatorUPtr allocator(new PagedLinearAllocator(k_allocatorPageSize));
            m_queue.push_back(allocator.get());
            m_allocators.push_back(std::move(allocator));
        }
    }
    
    //------------------------------------------------------------------------------
    IAllocator* FrameAllocatorQueue::Pop() noexcept
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        
        while (m_queue.empty())
        {
            m_condition.wait(lock);
        }
        
        auto front = m_queue.front();
        m_queue.pop_front();
        
        return front;
    }
    
    //------------------------------------------------------------------------------
    IAllocator* FrameAllocatorQueue::Front() noexcept
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        
        while (m_queue.empty())
        {
            m_condition.wait(lock);
        }
        
        auto front = m_queue.front();
        
        return front;
    }
    
    //------------------------------------------------------------------------------
    void FrameAllocatorQueue::Push(IAllocator* allocator) noexcept
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        
#if CS_ENABLE_DEBUG
        for (IAllocator* queuedAllocators : m_queue)
        {
            CS_ASSERT(queuedAllocators != allocator, "Allocator is already in queue, cannot be pushed twice!");
        }
#endif
        
        for (auto& pagedLinearAllocator : m_allocators)
        {
            if (pagedLinearAllocator.get() == allocator)
            {
                pagedLinearAllocator->Reset();
                m_queue.push_back(allocator);
                
                m_condition.notify_one();
                return;
            }
        }
        
        CS_LOG_FATAL("Cannot push an allocator that is not owned by this queue");
    }
}
