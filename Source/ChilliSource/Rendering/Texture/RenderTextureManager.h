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
#include <ChilliSource/Rendering/Texture/RenderTexture.h>

#include <mutex>

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
    /// This is thead-safe and can be called from any thread. If it is called on a background
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
        ///
        /// @return The new render texture instance.
        ///
        const RenderTexture* CreateRenderTexture(std::unique_ptr<const u8[]> textureData, u32 textureDataSize, const Integer2& dimensions, ImageFormat imageFormat, ImageCompression imageCompression,
                                           TextureFilterMode filterMode, TextureWrapMode wrapModeS, TextureWrapMode wrapModeT, bool isMipmapped) noexcept;
        
        /// Removes the render texture from the manager and queues an UnloadTextureRenderCommand for
        /// the next Render Snapshot stage in the render pipeline. The render command is given
        /// ownership of the render texture, ensuring it won't be destroyed until it is no longer
        /// used.
        ///
        /// @param renderTexture
        ///     The render texture which should be destroyed.
        ///
        void DestroyRenderTexture(const RenderTexture* renderTexture) noexcept;
        
    private:
        friend class Application;
        
        /// A container for information relating to pending texture load commands, such as the
        /// texture data, data size and the related RenderTexture.
        ///
        struct PendingLoadCommand final
        {
            std::unique_ptr<const u8[]> m_textureData;
            u32 m_textureDataSize = 0;
            RenderTexture* m_renderTexture = nullptr;
        };
        
        /// A factory method for creating new instances of the system. This must be called by
        /// Application.
        ///
        /// @return The new instance of the system.
        ///
        static RenderTextureManagerUPtr Create() noexcept;
        
        RenderTextureManager() = default;
        
        /// Called during the Render Snapshot stage of the render pipeline. All pending load and
        /// unload commands are added to the render snapshot.
        ///
        /// @param renderSnapshot
        ///     The render shapshot for storing snapshotted data.
        ///
        void OnRenderSnapshot(RenderSnapshot& renderSnapshot) noexcept override;
        
        std::mutex m_mutex;
        std::vector<RenderTextureUPtr> m_renderTextures; //TODO: This should be changed to an object pool.
        std::vector<PendingLoadCommand> m_pendingLoadCommands;
        std::vector<RenderTextureUPtr> m_pendingUnloadCommands;
    };
}

#endif
