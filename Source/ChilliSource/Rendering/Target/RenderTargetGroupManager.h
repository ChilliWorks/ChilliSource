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

#ifndef _CHILLISOURCE_RENDERING_TARGET_RENDERTARGETGROUPMANAGER_H_
#define _CHILLISOURCE_RENDERING_TARGET_RENDERTARGETGROUPMANAGER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Memory/ObjectPoolAllocator.h>
#include <ChilliSource/Core/Memory/UniquePtr.h>
#include <ChilliSource/Core/System/AppSystem.h>
#include <ChilliSource/Rendering/Target/RenderTargetGroup.h>

#include <mutex>
#include <vector>

namespace ChilliSource
{
    /// Manages the lifecycle of all RenderTargetGroup instances.
    ///
    /// On creation of a RenderTarget a LoadTargetGroupRenderCommand is queued for the next render
    /// snapshot phase, ensuring that related target data is processed before the RenderTargetGroup
    /// is used.
    ///
    /// On deletion an UnloadTargetGroupRenderCommand is queued and given ownership of the
    /// RenderTargetGroup. The render texture is then deleted once the command has been processed.
    ///
    /// This is thread-safe and can be called from any thread. If it is called on a background
    /// thread, care needs to be taken to ensure any created RenderTargetGroups are not used prior
    /// to being loaded.
    ///
    class RenderTargetGroupManager final : public AppSystem
    {
    public:
        CS_DECLARE_NAMEDTYPE(RenderTargetGroupManager);
        
        /// Allows querying of whether or not this system implements the interface described by the
        /// given interface Id. Typically this is not called directly as the templated equivalent
        /// IsA<Interface>() is preferred.
        ///
        /// @param interfaceId
        ///     The Id of the interface.
        ///
        /// @return Whether or not the interface is implemented.
        ///
        bool IsA(InterfaceIDType interfaceId) const noexcept override;
        
        /// Creates a new RenderTargetGroup and queues a LoadTargetGroupRenderCommand for the next RenderSnapshot
        /// stage in the render pipeline.
        ///
        /// @param colourTarget
        ///     The texture that will be used as the colour target. must not be null.
        /// @param depthTarget
        ///     The texture that will be used as the depth target. must not be null.
        ///
        /// @return The new render target group instance.
        ///
        UniquePtr<RenderTargetGroup> CreateRenderTargetGroup(const RenderTexture* colourTarget, const RenderTexture* depthTarget) noexcept;
        
        /// Creates a new colour only RenderTargetGroup and queues a LoadTargetGroupRenderCommand for the next
        /// RenderSnapshot stage in the render pipeline.
        ///
        /// @param colourTarget
        ///     The texture that will be used as the colour target. must not be null.
        /// @param type
        ///     Whether or not an internal, efficient, depth buffer or stencil buffer should be used.
        ///
        /// @return The new render target group instance.
        ///
        UniquePtr<RenderTargetGroup> CreateColourRenderTargetGroup(const RenderTexture* colourTarget, RenderTargetGroupType type) noexcept;
        
        /// Creates a new depth only RenderTargetGroup and queues a LoadTargetGroupRenderCommand for the next
        /// RenderSnapshot stage in the render pipeline.
        ///
        /// @param depthTarget
        ///     The texture that will be used as the depth target. must not be null.
        ///
        /// @return The new render target group instance.
        ///
        UniquePtr<RenderTargetGroup> CreateDepthRenderTargetGroup(const RenderTexture* depthTarget) noexcept;
     
        /// @return A copy of the render target groups
        ///
        std::vector<const RenderTargetGroup*> GetRenderTargetGroups() noexcept;
        
        /// Removes the render target group from the manager and queues an UnloadTargetGroupRenderCommand
        /// for the next Render Snapshot stage in the render pipeline. The render command is given
        /// ownership of the render target group, ensuring it won't be destroyed until it is no longer
        /// used.
        ///
        /// @param renderTargetGroup
        ///     The render target group which should be destroyed.
        ///
        void DestroyRenderTargetGroup(UniquePtr<RenderTargetGroup> renderTargetGroup) noexcept;
        
    private:
        friend class Application;
        
        /// A factory method for creating new instances of the system. This must be called by
        /// Application.
        ///
        /// @return The new instance of the system.
        ///
        static RenderTargetGroupManagerUPtr Create() noexcept;
        
        RenderTargetGroupManager();
        
        /// Called during the Render Snapshot stage of the render pipeline. All pending load and
        /// unload commands are added to the render snapshot.
        ///
        /// @param targetType
        ///     Whether the snapshot is for the main screen or an offscreen render target
        /// @param renderSnapshot
        ///     The render shapshot for storing snapshotted data.
        /// @param frameAllocator
        ///     Allocate memory for this render frame from here
        ///
        void OnRenderSnapshot(TargetType targetType, RenderSnapshot& renderSnapshot, IAllocator* frameAllocator) noexcept override;
        
        std::mutex m_mutex;
        ObjectPoolAllocator<RenderTargetGroup> m_renderTargetGroupPool;
        std::vector<const RenderTargetGroup*> m_renderTargetGroups;
        std::vector<RenderTargetGroup*> m_pendingLoadCommands;
        std::vector<UniquePtr<RenderTargetGroup>> m_pendingUnloadCommands;
    };
}

#endif
