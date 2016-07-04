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

#include <ChilliSource/Rendering/Base/IContextRestorer.h>
#include <ChilliSource/Rendering/Model/RenderMesh.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/LoadTextureRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/RenderCommandBuffer.h>
#include <ChilliSource/Rendering/Shader/RenderShader.h>
#include <ChilliSource/Rendering/Texture/RenderTexture.h>

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
        
        /// Called when the app system is resumed.
        ///
        /// Called on the main thread.
        ///
        void OnResume() noexcept override;
        
        /// Called when the application delegate is suspended. This
        /// event is called directly from the platform application
        /// implementation and will be called before the lifecycle
        /// counterpart.
        ///
        /// This should only be called from the render thread by the
        /// lifecycle manager.
        ///
        void OnSystemSuspend() noexcept;
        
        /// Will iterate an RenderCommandBuffer and extract any commands that can
        /// be recycled for next frame.
        ///
        /// @param commands
        ///     The RenderCommandBuffer to recycle.
        ///
        void RecycleCommands(const RenderCommandBufferUPtr& commands) noexcept;
        
        /// If the queue of command buffers is full then this waits until one has been popped to continue.
        /// It then adds the given render buffer and notifies any threads which are waiting.
        ///
        /// @param renderCommandBuffer
        ///     The render command buffer which should be pushed. Must be moved.
        ///
        void WaitThenPushCommandBuffer(RenderCommandBufferUPtr renderCommandBuffer) noexcept;
        
        /// If the queue of command buffers is empty then this waits until one has been pushed to continue.
        /// It pops a command buffer from the list and notifies any threads which are waiting.
        ///
        /// @return The render command buffer which has been popped.
        ///
        RenderCommandBufferUPtr WaitThenPopCommandBuffer() noexcept;
        
        ~RenderCommandBufferManager() noexcept {}
        
    private:
        friend class Application;
        
        /// Command data for loading a texture
        ///
        struct TextureLoadCommand final
        {
            std::unique_ptr<const u8[]> m_textureData;
            u32 m_textureDataSize = 0;
            RenderTexture* m_renderTexture = nullptr;
        };
        
        /// Command data for loading a shader
        ///
        struct ShaderLoadCommand final
        {
            std::string m_vertexShader;
            std::string m_fragmentShader;
            RenderShader* m_renderShader = nullptr;
        };
        
        /// Command data for loading a mesh
        ///
        struct MeshLoadCommand final
        {
            RenderMesh* m_renderMesh = nullptr;
            std::unique_ptr<const u8[]> m_vertexData;
            u32 m_vertexDataSize = 0;
            std::unique_ptr<const u8[]> m_indexData;
            u32 m_indexDataSize = 0;
        };
        
        RenderCommandBufferManager();
        
        /// A factory method for creating new instances of the system. This must be called by
        /// Application.
        ///
        /// @return The new instance of the system.
        ///
        static RenderCommandBufferManagerUPtr Create() noexcept;
        
        /// Called during the Render Snapshot stage of the render pipeline. All pending load commands
        /// are added to the render snapshot.
        ///
        /// @param renderSnapshot
        ///     The render shapshot for storing snapshotted data.
        ///
        void OnRenderSnapshot(RenderSnapshot& renderSnapshot) noexcept override;
        
        /// Called to process a command queue and extract load/unload commands that we
        /// can recycle and store for use next frame.
        ///
        /// @param renderCommandQueue
        ///     The render command queue to recycle
        ///
        void RecycleCommandQueue(std::vector<RenderCommand*>& renderCommandQueue) noexcept;
        
        /// Called to proccess a command to be recycled. Will decide if its a valid
        /// recyclable command and then take a copy of its data so we can recreate the
        /// command later.
        ///
        /// @param renderCommand
        ///     The render command to recycle or ignore
        ///
        /// @return If the command was processed
        ///
        bool RecycleCommand(RenderCommand* renderCommand) noexcept;
        
    private:
        
        std::mutex m_commandBufferMutex;
        
        std::vector<ShaderLoadCommand> m_pendingShaderLoadCommands;
        std::vector<TextureLoadCommand> m_pendingTextureLoadCommands;
        std::vector<MeshLoadCommand> m_pendingMeshLoadCommands;
        
        std::vector<RenderShaderUPtr> m_pendingShaderUnloadCommands;
        std::vector<RenderTextureUPtr> m_pendingTextureUnloadCommands;
        std::vector<RenderMeshUPtr> m_pendingMeshUnloadCommands;
        
        IContextRestorerUPtr m_contextRestorer;
        std::atomic_bool m_discardCommands;
        bool m_initialised = false;
        
        std::mutex m_renderCommandBuffersMutex;
        std::condition_variable m_renderCommandBuffersCondition;
        std::deque<RenderCommandBufferUPtr> m_renderCommandBuffers;
    };
}

#endif
