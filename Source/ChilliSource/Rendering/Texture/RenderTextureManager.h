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

#ifndef _CHILLISOURCE_RENDERING_TEXTURE_RENDERTEXTUREMANAGER_H_
#define _CHILLISOURCE_RENDERING_TEXTURE_RENDERTEXTUREMANAGER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Memory/ObjectPoolAllocator.h>
#include <ChilliSource/Core/Memory/UniquePtr.h>
#include <ChilliSource/Core/System/AppSystem.h>
#include <ChilliSource/Rendering/Texture/RenderTexture.h>

#include <mutex>
#include <array>

namespace ChilliSource
{
    /// Manages the lifecycle of all RenderTexture instances.
    ///
    /// On creation of a RenderTexture a LoadTextureRenderCommand is queued for the next render
    /// snapshot phase, ensuring that related textured data is processed before the RenderTexture
    /// is used.
    ///
    /// On deletion an UnloadTextureRenderCommand is queued and given ownership of the
    /// RenderTexture. The render texture is then deleted once the command has been processed.
    ///
    /// This is thread-safe and can be called from any thread. If it is called on a background
    /// thread, care needs to be taken to ensure any created RenderTextures are not used prior
    /// to being loaded.
    ///
    class RenderTextureManager final : public AppSystem
    {
    public:
        CS_DECLARE_NAMEDTYPE(RenderTextureManager);
        
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
        
        /// Creates a new render texture and queues a LoadTextureRenderCommand for the next
        /// Render Snapshot stage in the render pipeline.
        ///
        /// @param textureData
        ///     The texture data buffer.
        /// @param textureDataSize
        ///     The size of the texture data buffer.
        /// @param dimensions
        ///     The texture dimensions.
        /// @param imageFormat
        ///     The image format.
        /// @param imageCompression
        ///     The image compression type.
        /// @param filterMode
        ///     The texture filter mode.
        /// @param wrapModeS
        ///     The s-coordinate wrap mode.
        /// @param wrapModeT
        ///     The t-coordinate wrap mode.
        /// @param isMipmapped
        ///     Whether or not mipmaps are generated for the texture.
        /// @param shouldBackupData
        ///     If the texture data should be backed up in main memory for restoring it later.
        ///
        /// @return The new render texture instance.
        ///
        UniquePtr<RenderTexture> CreateTexture2D(std::unique_ptr<const u8[]> textureData, u32 textureDataSize, const Integer2& dimensions, ImageFormat imageFormat, ImageCompression imageCompression,
                                             TextureFilterMode filterMode, TextureWrapMode wrapModeS, TextureWrapMode wrapModeT, bool isMipmapped, bool shouldBackupData) noexcept;
        
        /// Creates a new render texture and queues a LoadCubemapRenderCommand for the next
        /// Render Snapshot stage in the render pipeline.
        ///
        /// @param textureData
        ///     The texture data buffer for all 6 faces.
        /// @param textureDataSize
        ///     The size of the texture data buffer.
        /// @param dimensions
        ///     The texture dimensions.
        /// @param imageFormat
        ///     The image format.
        /// @param imageCompression
        ///     The image compression type.
        /// @param filterMode
        ///     The texture filter mode.
        /// @param wrapModeS
        ///     The s-coordinate wrap mode.
        /// @param wrapModeT
        ///     The t-coordinate wrap mode.
        /// @param isMipmapped
        ///     Whether or not mipmaps are generated for the texture.
        /// @param shouldBackupData
        ///     If the texture data should be backed up in main memory for restoring it later.
        ///
        /// @return The new render texture instance.
        ///
        UniquePtr<RenderTexture> CreateCubemap(std::array<std::unique_ptr<const u8[]>, 6> textureData, u32 textureDataSize, const Integer2& dimensions, ImageFormat imageFormat, ImageCompression imageCompression,
                                           TextureFilterMode filterMode, TextureWrapMode wrapModeS, TextureWrapMode wrapModeT, bool isMipmapped, bool shouldBackupData) noexcept;
        
        /// Removes the render texture from the manager and queues an UnloadTextureRenderCommand for
        /// the next Render Snapshot stage in the render pipeline. The render command is given
        /// ownership of the render texture, ensuring it won't be destroyed until it is no longer
        /// used.
        ///
        /// @param renderTexture
        ///     The render texture which should be destroyed.
        ///
        void DestroyRenderTexture2D(UniquePtr<RenderTexture> renderTexture) noexcept;
        
        /// Removes the render texture from the manager and queues an UnloadTextureRenderCommand for
        /// the next Render Snapshot stage in the render pipeline. The render command is given
        /// ownership of the render texture, ensuring it won't be destroyed until it is no longer
        /// used.
        ///
        /// @param renderTexture
        ///     The render texture which should be destroyed.
        ///
        void DestroyRenderTextureCubemap(UniquePtr<RenderTexture> renderTexture) noexcept;
        
    private:
        friend class Application;
        
        /// A container for information relating to pending texture load commands, such as the
        /// texture data, data size and the related RenderTexture.
        ///
        struct PendingLoadCommand2D final
        {
            std::unique_ptr<const u8[]> m_textureData;
            u32 m_textureDataSize = 0;
            RenderTexture* m_renderTexture = nullptr;
        };
        
        /// A container for information relating to pending cubemap load commands, such as the
        /// texture data, data size and the related RenderTexture.
        ///
        struct PendingLoadCommandCubemap final
        {
            std::array<std::unique_ptr<const u8[]>, 6> m_textureData;
            u32 m_textureDataSize = 0;
            RenderTexture* m_renderTexture = nullptr;
        };
        
        /// A factory method for creating new instances of the system. This must be called by
        /// Application.
        ///
        /// @return The new instance of the system.
        ///
        static RenderTextureManagerUPtr Create() noexcept;
        
        RenderTextureManager();
        
        /// Called during the Render Snapshot stage of the render pipeline. All pending load and
        /// unload commands are added to the render snapshot.
        ///
        /// @param targetType
        ///     Whether the snapshot is for the main screen or an offscreen render target
        /// @param renderSnapshot
        ///     The render shapshot for storing snapshotted data.
        /// @param frameAllocator
        ///     Allocate memory from this render frame from here
        ///
        void OnRenderSnapshot(TargetType targetType, RenderSnapshot& renderSnapshot, IAllocator* frameAllocator) noexcept override;
        
        std::mutex m_mutex;
        ObjectPoolAllocator<RenderTexture> m_renderTexturePool;
        std::vector<PendingLoadCommand2D> m_pendingLoadCommands2D;
        std::vector<PendingLoadCommandCubemap> m_pendingLoadCommandsCubemap;
        std::vector<UniquePtr<RenderTexture>> m_pendingUnloadCommands2D;
        std::vector<UniquePtr<RenderTexture>> m_pendingUnloadCommandsCubemap;
    };
}

#endif
