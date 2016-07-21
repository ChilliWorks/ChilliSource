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

#ifndef _CHILLISOURCE_RENDERING_BASE_FRAMEALLOCATORQUEUE_H_
#define _CHILLISOURCE_RENDERING_BASE_FRAMEALLOCATORQUEUE_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Memory/PagedLinearAllocator.h>

#include <condition_variable>
#include <mutex>
#include <deque>
#include <vector>

namespace ChilliSource
{
    /// Creates and manages the frame allocators which will be used during the render
    /// pipeline. These allocators are queued, allowing users to pop one when required
    /// and pushed again once finished with. As there are three main stages to the
    /// render pipeline (snapshot, preparation and command processing) there are three
    /// allocators in the queue. If all are in use, then this will block until one is
    /// returned to the manager.
    ///
    /// This is thread safe.
    ///
    class FrameAllocatorQueue final
    {
    public:
        CS_DECLARE_NOCOPY(FrameAllocatorQueue);
        
        FrameAllocatorQueue() noexcept;
        
        /// Pops the first frame allocator from the queue. If the queue is empty this will wait
        /// until one is pushed before popping.
        ///
        /// @return The popped frame allocator.
        ///
        IAllocator* Pop() noexcept;
        
        /// Reads the first frame allocator from the queue. If the queue is empty this will wait
        /// until one is pushed before reading.
        ///
        /// @return The front frame allocator.
        ///
        IAllocator* Front() noexcept;
        
        /// Pushes an allocator back into the queue for reuse. Prior to being added to the queue
        /// the allocator will be wiped.
        ///
        /// If the given allocator didn't originate from this queue then this will assert.
        ///
        /// @param allocator
        ///     The allocator which should be returned to the queue.
        ///
        void Push(IAllocator* allocator) noexcept;
        
    private:
        std::mutex m_mutex;
        std::condition_variable m_condition;
        std::deque<IAllocator*> m_queue;
        std::vector<PagedLinearAllocatorUPtr> m_allocators;
    };
}

#endif
