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

#ifndef _CHILLISOURCE_RENDERING_BASE_RENDERCOMMANDBUFFERSYSTEM_H_
#define _CHILLISOURCE_RENDERING_BASE_RENDERCOMMANDBUFFERSYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/System/AppSystem.h>

#include <ChilliSource/Rendering/Model/RenderMesh.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/LoadMaterialGroupRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/LoadMeshRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/LoadShaderRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/LoadTextureRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/LoadCubemapRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/UnloadMaterialGroupRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/UnloadMeshRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/UnloadShaderRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/UnloadTextureRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/UnloadCubemapRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/RenderCommandBuffer.h>
#include <ChilliSource/Rendering/Shader/RenderShader.h>
#include <ChilliSource/Rendering/Texture/RenderTexture.h>

#include <atomic>
#include <condition_variable>
#include <deque>
#include <mutex>

namespace ChilliSource
{
    /// System to manage a number of RenderCommandBuffers for the renderer. It primary function
    /// is to queue/dequeue command buffers while it also provides means for recycling
    /// of render commands from a frame on suspend to allow them to be entered at the next one.
    ///
    /// This is thread safe, though certain methods need to be called on certain threads.
    ///
    class RenderCommandBufferManager : public AppSystem
    {
    public:
        CS_DECLARE_NAMEDTYPE(RenderCommandBufferManager);
        
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
        
        /// If the queue of command buffers is full then this waits until one has been popped to continue.
        /// It then adds the given render buffer and notifies any threads which are waiting.
        ///
        /// @param renderCommandBuffer
        ///     The render command buffer which should be pushed. Must be moved.
        ///
        void WaitThenPushCommandBuffer(RenderCommandBufferUPtr renderCommandBuffer) noexcept;
        
        /// If the queue of command buffers is empty then this waits until one has been pushed to continue.
        /// It pops the command buffer from the list and notifies any threads which are waiting.
        ///
        /// @return The render command buffer that has been popped.
        ///
        RenderCommandBufferCUPtr WaitThenPopCommandBuffer() noexcept;
        
    private:
        friend class Application;
        friend class LifecycleManager;

        /// A factory method for creating new instances of the system. This must be called by
        /// Application.
        ///
        /// @return The new instance of the system.
        ///
        static RenderCommandBufferManagerUPtr Create() noexcept;

        RenderCommandBufferManager();
        
        /// Will iterate an RenderCommandBuffer and extract any commands that can be recycled
        /// for next frame.
        ///
        /// @param buffer
        ///     The RenderCommandBuffer to recycle.
        ///
        void RecycleRenderCommandBuffer(RenderCommandBufferUPtr buffer) noexcept;
        
        /// Called to process a command queue and extract load/unload commands that we
        /// can recycle and store for use next frame.
        ///
        /// @param renderCommandList
        ///     The render command list to recycle
        ///
        void RecycleCommandList(RenderCommandList* renderCommandList) noexcept;
        
        /// Called to proccess a command to be recycled. Will decide if its a valid
        /// recyclable command and then take a copy of its data so we can recreate the
        /// command later.
        ///
        /// @param renderCommand
        ///     The render command to recycle or ignore
        ///
        void RecycleCommand(RenderCommand* renderCommand) noexcept;
        
        /// Called when the app system is init.
        ///
        /// Called on the main thread.
        ///
        void OnInit() noexcept override;
        
        /// Called when the app system is resumed.
        ///
        /// Called on the main thread.
        ///
        void OnResume() noexcept override;
        
        /// Called during the Render Snapshot stage of the render pipeline. All pending load commands
        /// are added to the render snapshot.
        ///
        /// @param targetType
        ///     Whether the snapshot is for the main screen or an offscreen render target
        /// @param renderSnapshot
        ///     The render shapshot for storing snapshotted data.
        /// @param frameAllocator
        ///     Allocate memory for this render frame from here
        ///
        void OnRenderSnapshot(TargetType targetType, RenderSnapshot& renderSnapshot, IAllocator* frameAllocator) noexcept override;
        
        /// Called when the application delegate is suspended. This event is called directly
        /// from the platform application implementation and will be called before the lifecycle
        /// counterpart.
        ///
        /// This should only be called from the render thread by the
        /// lifecycle manager.
        ///
        void OnSystemSuspend() noexcept;
        
        
    private:
        
        std::mutex m_commandBufferMutex;
        
        std::vector<LoadShaderRenderCommand> m_pendingShaderLoadCommands;
        std::vector<LoadTextureRenderCommand> m_pendingTextureLoadCommands;
        std::vector<LoadCubemapRenderCommand> m_pendingCubemapLoadCommands;
        std::vector<LoadMeshRenderCommand> m_pendingMeshLoadCommands;
        std::vector<LoadMaterialGroupRenderCommand> m_pendingMaterialGroupLoadCommands;
        
        std::vector<UnloadShaderRenderCommand> m_pendingShaderUnloadCommands;
        std::vector<UnloadTextureRenderCommand> m_pendingTextureUnloadCommands;
        std::vector<UnloadCubemapRenderCommand> m_pendingCubemapUnloadCommands;
        std::vector<UnloadMeshRenderCommand> m_pendingMeshUnloadCommands;
        std::vector<UnloadMaterialGroupRenderCommand> m_pendingMaterialGroupUnloadCommands;
        
        std::atomic_bool m_discardCommands;
        
        std::mutex m_renderCommandBuffersMutex;
        std::condition_variable m_renderCommandBuffersCondition;
        std::deque<RenderCommandBufferUPtr> m_renderCommandBuffers;
        
        Renderer* m_renderer = nullptr;
    };
}

#endif
