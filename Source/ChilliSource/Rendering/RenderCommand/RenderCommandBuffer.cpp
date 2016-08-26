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

#include <ChilliSource/Rendering/RenderCommand/RenderCommandBuffer.h>

namespace ChilliSource
{
    //------------------------------------------------------------------------------
    RenderCommandBuffer::RenderCommandBuffer(u32 numSlots, IAllocator* frameAllocator, std::vector<RenderFrameData> renderFramesData) noexcept
        : m_renderFramesData(std::move(renderFramesData)), m_frameAllocator(frameAllocator)
    {
        m_renderCommandLists.reserve(numSlots);
        for (u32 i = 0; i < numSlots; ++i)
        {
            m_renderCommandLists.push_back(RenderCommandListUPtr(new RenderCommandList()));
        }
        
        m_queue.reserve(numSlots);
        for (const auto& renderCommandList : m_renderCommandLists)
        {
            m_queue.push_back(renderCommandList.get());
        }
    }
    
    //------------------------------------------------------------------------------
    RenderCommandList* RenderCommandBuffer::GetRenderCommandList(u32 slotIndex) noexcept
    {
        CS_ASSERT(slotIndex < GetNumSlots(), "Index out of bounds.");
        
        return m_renderCommandLists[slotIndex].get();
    }
}