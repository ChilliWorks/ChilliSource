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

#ifndef _CHILLISOURCE_RENDERING_SHADER_RENDERSHADERMANAGER_H_
#define _CHILLISOURCE_RENDERING_SHADER_RENDERSHADERMANAGER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/System/AppSystem.h>
#include <ChilliSource/Core/Memory/ObjectPoolAllocator.h>
#include <ChilliSource/Core/Memory/UniquePtr.h>
#include <ChilliSource/Rendering/Shader/RenderShader.h>

#include <mutex>

namespace ChilliSource
{
    /// Manages the lifecycle of all RenderShader instances.
    ///
    /// On creation of a RenderShader a LoadShaderRenderCommand is queued for the next render
    /// snapshot phase, ensuring that related shader data is processed before the RenderShader
    /// is used.
    ///
    /// On deletion an UnloadShaderRenderCommand is queued and given ownership of the
    /// RenderShader. The render shader is then deleted once the command has been processed.
    ///
    /// This is thread-safe and can be called from any thread. If it is called on a background
    /// thread, care needs to be taken to ensure any created RenderShaders are not used prior
    /// to being loaded.
    ///
    class RenderShaderManager final : public AppSystem
    {
    public:
        CS_DECLARE_NAMEDTYPE(RenderShaderManager);
        
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
        
        /// Creates a new render shader and queues a LoadShaderRenderCommand for the next
        /// Render Snapshot stage in the render pipeline.
        ///
        /// @param vertexShader
        ///     The vertex shader string.
        /// @param fragmentShader
        ///     The fragment shader string.
        ///
        /// @return The render shader instance.
        ///
        UniquePtr<RenderShader> CreateRenderShader(const std::string& vertexShader, const std::string& fragmentShader) noexcept;
        
        /// Removes the render shader from the manager and queues an UnloadShaderRenderCommand for
        /// the next Render Snapshot stage in the render pipeline. The render command is given
        /// ownership of the render shader, ensuring it won't be destroyed until it is no longer
        /// used.
        ///
        /// @param renderShader
        ///     The render shader which should be destroyed.
        ///
        void DestroyRenderShader(UniquePtr<RenderShader> renderShader) noexcept;
        
    private:
        friend class Application;
        
        /// A container for information relating to pending shader load commands, such as the
        /// vertex shader, fragment shader and the related RenderShader.
        ///
        struct PendingLoadCommand final
        {
            std::string m_vertexShader;
            std::string m_fragmentShader;
            RenderShader* m_renderShader = nullptr;
        };
        
        /// A factory method for creating new instances of the system. This must be called by
        /// Application.
        ///
        /// @return The new instance of the system.
        ///
        static RenderShaderManagerUPtr Create() noexcept;
        
        RenderShaderManager();
        
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
        ObjectPoolAllocator<RenderShader> m_renderShaderPool;
        std::vector<PendingLoadCommand> m_pendingLoadCommands;
        std::vector<UniquePtr<RenderShader>> m_pendingUnloadCommands;
    };
}

#endif
