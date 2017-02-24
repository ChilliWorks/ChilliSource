//
//  The MIT License (MIT)
//
//  Copyright (c) 2010 Tag Games Limited
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

#ifndef _CHILLISOURCE_RENDERING_TEXTURE_CUBEMAP_H_
#define _CHILLISOURCE_RENDERING_TEXTURE_CUBEMAP_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Memory/UniquePtr.h>
#include <ChilliSource/Core/Image/ImageFormat.h>
#include <ChilliSource/Core/Image/ImageCompression.h>
#include <ChilliSource/Core/Resource/Resource.h>

#include <array>

namespace ChilliSource
{
    /// A cubemap texture resource consisting of 6 textures. All 6 textures must share
    /// the same dimensions, filter mode, etc.
    ///
    /// This is not thread safe and should only be accessed from one thread at a time.
    ///
    /// NOTE: Texture data is specified in the following order:
    ///     * PosX, NegX, PosY, NegY, PosZ, NegZ
    ///
    class Cubemap final : public Resource
    {
    public:
        CS_DECLARE_NAMEDTYPE(Cubemap);
        
        /// The buffer used to hold texture data.
        ///
        using DataUPtr = std::unique_ptr<const u8[]>;
        
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
        
        /// Construct the texture from the given image data. The texture will take ownership of
        /// the image data. The texture must not already be in the loaded state.
        ///
        /// @param textureData
        ///     The texture data buffer and size for each face.
        /// @param dataSize
        ///     Size of each face texture data. Only 1 value as all faces must be the same format and dimensions
        /// @param textureDesc
        ///     The texture description.
        ///
        void Build(std::array<DataUPtr, 6> textureData, u32 dataSize, const TextureDesc& textureDesc) noexcept;

        /// @return The texture dimensions.
        ///
        const Integer2& GetDimensions() const noexcept;
        
        /// @return The format of the image.
        ///
        ImageFormat GetImageFormat() const noexcept;
        
        /// @return The compression type of the image.
        ///
        ImageCompression GetImageCompression() const noexcept;
        
        /// @return The texture filter mode used when rendering the texture.
        ///
        TextureFilterMode GetFilterMode() const noexcept;
        
        /// @return The s-coordinate texture wrap mode used when rendering the texture.
        ///
        TextureWrapMode GetWrapModeS() const noexcept;
        
        /// @return The t-coordinate texture wrap mode used when rendering the texture.
        ///
        TextureWrapMode GetWrapModeT() const noexcept;
        
        /// @return Whether or not the texture should be mipmapped.
        ///
        bool IsMipmappingEnabled() const noexcept;
        
        /// @return Whether or not texture data should be restored on context loss.
        ///
        bool IsRestoreTextureDataEnabled() const noexcept;
        
        /// @return The underlying RenderTexture used by the render system.
        ///
        const RenderTexture* GetRenderTexture() const noexcept;
        
        ~Cubemap() noexcept;
        
    private:
        friend class ResourcePool;
        
        /// A factory method for creating new, empty instances of the resource. This must only be
        /// called by ResourcePool.
        ///
        /// @return The new instance of the resource.
        ///
        static CubemapUPtr Create() noexcept;
        
        /// Destroys the render texture if one currently exists.
        ///
        void DestroyRenderTexture() noexcept;
        
        Cubemap() = default;
        
        UniquePtr<RenderTexture> m_renderTexture;
        bool m_restoreTextureDataEnabled = false;
    };
}

#endif
