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

#ifndef _CHILLISOURCE_RENDERING_RENDERCOMMAND_COMMANDS_LOADCUBEMAPRENDERCOMMAND_H_
#define _CHILLISOURCE_RENDERING_RENDERCOMMAND_COMMANDS_LOADCUBEMAPRENDERCOMMAND_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Rendering/RenderCommand/RenderCommand.h>
#include <ChilliSource/Rendering/Texture/RenderTexture.h>

#include <array>

namespace ChilliSource
{
    /// A render command for loading the cubemap data for each face pertaining to a single render texture into
    /// render memory.
    ///
    /// This must be instantiated via a RenderCommandList.
    ///
    /// This is immutable and therefore thread-safe.
    ///
    class LoadCubemapRenderCommand final : public RenderCommand
    {
    public:
        /// @return The render texture that should be loaded.
        ///
        RenderTexture* GetRenderTexture() const noexcept { return m_renderTexture; };
        
        /// @return The data describing the texture.
        ///
        const std::array<std::unique_ptr<const u8[]>, 6>& GetTextureData() const noexcept { return m_textureData; }
        
        /// Moves the texture data out of this class. Use with caution as this command
        /// will be in a broken state after this is used.
        ///
        /// @return The data describing the texture for each face.
        ///
        std::array<std::unique_ptr<const u8[]>, 6> ClaimTextureData() noexcept;
        
        /// @return The size of the texture data in bytes.
        ///
        u32 GetTextureDataSize() const noexcept { return m_textureDataSize; }
        
    private:
        friend class RenderCommandList;
        
        /// Constructs a new instance with the given render texture and texture data.
        ///
        /// @param renderTexture
        ///     The render texture that should be loaded.
        /// @param textureData
        ///     The data describing the texture for each face.
        /// @param textureDataSize
        ///     The size of the texture data in bytes.
        ///
        LoadCubemapRenderCommand(RenderTexture* renderTexture, std::array<std::unique_ptr<const u8[]>, 6> textureData, u32 textureDataSize) noexcept;
        
        RenderTexture* m_renderTexture;
        std::array<std::unique_ptr<const u8[]>, 6> m_textureData;
        u32 m_textureDataSize;
    };
}

#endif
