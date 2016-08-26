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

#ifndef _CHILLISOURCE_RENDERING_RENDERCOMMAND_RENDERCOMMANDQUEUE_H_
#define _CHILLISOURCE_RENDERING_RENDERCOMMAND_RENDERCOMMANDQUEUE_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Rendering/Base/RenderFrameData.h>
#include <ChilliSource/Rendering/Model/RenderDynamicMesh.h>
#include <ChilliSource/Rendering/Model/RenderSkinnedAnimation.h>
#include <ChilliSource/Rendering/RenderCommand/RenderCommandList.h>

#include <vector>

namespace ChilliSource
{
    /// Provides the ability to create a buffer of Render Command Lists. By creating and ordering
    /// all of the Render Command Lists up front, the Render Command Lists can be safely
    /// populated on separate threads without locking.
    ///
    /// This also holds frame data required by commands to ensure that the data exists for as long
    /// as the commands require them.
    ///
    /// This is not thread-safe but can be safely used accross threads as long as each thread
    /// only accessed one queue slot.
    ///
    class RenderCommandBuffer final
    {
    public:
        CS_DECLARE_NOCOPY(RenderCommandBuffer);
        
        RenderCommandBuffer(RenderCommandBuffer&&) = default;
        RenderCommandBuffer& operator=(RenderCommandBuffer&&) = default;
        
        /// Creates a new render command buffer with the requested number of slots.
        ///
        /// @param numSlots
        ///     The number of slots in the queue.
        /// @param frameAllocator
        ///     The allocator used for this render frame
        /// @param renderFramesData
        ///     The render frame data that must persist to the end of the frame. Must be moved.
        ///
        RenderCommandBuffer(u32 numSlots, IAllocator* frameAllocator, std::vector<RenderFrameData> renderFramesData) noexcept;
        
        /// @return The allocator from which all frame allocations should occur.
        ///
        IAllocator* GetFrameAllocator() const noexcept { return m_frameAllocator; }
        
        /// @return The number of slots in the queue.
        ///
        u32 GetNumSlots() const noexcept { return u32(m_queue.size()); }
        
        /// @param slotIndex
        ///     The requested slot index.
        ///
        /// @return The RenderCommandList at the requested slot index.
        ///
        RenderCommandList* GetRenderCommandList(u32 slotIndex) noexcept;
        
        /// @return The queue of command lists.
        ///
        const std::vector<const RenderCommandList*>& GetQueue() const noexcept { return m_queue; }
        
    private:
        std::vector<RenderDynamicMeshAUPtr> m_renderDynamicMeshes;
        std::vector<RenderSkinnedAnimationAUPtr> m_renderSkinnedAnimations;
        std::vector<const RenderCommandList*> m_queue;
        std::vector<RenderCommandListUPtr> m_renderCommandLists; //TODO: This should be changed to a pool.
        const std::vector<RenderFrameData> m_renderFramesData;
        IAllocator* m_frameAllocator;
    };
}

#endif