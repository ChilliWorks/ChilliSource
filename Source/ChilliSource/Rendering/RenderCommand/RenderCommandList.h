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

#ifndef _CHILLISOURCE_RENDERING_RENDERCOMMAND_RENDERCOMMANDLIST_H_
#define _CHILLISOURCE_RENDERING_RENDERCOMMAND_RENDERCOMMANDLIST_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/LoadTextureRenderCommand.h>

#include <vector>

namespace ChilliSource
{
    /// Provides the ability to create an ordered list of render commands. Commands are
    /// created contiguously in memory to improve cache locality and reduce fragmentation.
    ///
    /// This is not thread-safe and therefore should only be accessed from one thread
    /// at a time.
    ///
    class RenderCommandList final
    {
    public:
        /// Creates and adds a new load texture command tothe render command list.
        ///
        /// @param renderTexture
        ///     The render texture that should be loaded.
        /// @param textureData
        ///     The data describing the texture.
        /// @param textureDataSize
        ///     The size of the texture data in bytes.
        ///
        void AddLoadTextureCommand(/*RenderTexture* renderTexture, */std::unique_ptr<const u8[]> textureData, u32 textureDataSize) noexcept;
        
        /// @return The ordered list of render commands.
        ///
        const std::vector<const RenderCommand*>& GetOrderedList() const noexcept;
        
    private:
        std::vector<const RenderCommand*> m_orderedCommands;
        std::vector<LoadTextureRenderCommand> m_loadTextureRenderCommands;
    };
}

#endif
