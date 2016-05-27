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

#include <ChilliSource/Rendering/RenderCommand/RenderCommandList.h>

#include <ChilliSource/Rendering/RenderCommand/Commands/LoadMeshRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/LoadShaderRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/LoadTextureRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/UnloadMeshRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/UnloadShaderRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/UnloadTextureRenderCommand.h>

namespace ChilliSource
{
    //------------------------------------------------------------------------------
    void RenderCommandList::AddLoadMeshCommand(RenderMesh* renderMesh, std::unique_ptr<const u8[]> vertexData, u32 vertexDataSize, std::unique_ptr<const u8[]> indexData, u32 indexDataSize) noexcept
    {
        RenderCommandUPtr renderCommand(new LoadMeshRenderCommand(renderMesh, std::move(vertexData), vertexDataSize, std::move(indexData), indexDataSize));
        
        m_orderedCommands.push_back(renderCommand.get());
        m_renderCommands.push_back(std::move(renderCommand));
    }
    
    //------------------------------------------------------------------------------
    void RenderCommandList::AddLoadShaderCommand(RenderShader* renderShader, const std::string& vertexShader, const std::string& fragmentShader) noexcept
    {
        RenderCommandUPtr renderCommand(new LoadShaderRenderCommand(renderShader, vertexShader, fragmentShader));
        
        m_orderedCommands.push_back(renderCommand.get());
        m_renderCommands.push_back(std::move(renderCommand));
    }
    
    //------------------------------------------------------------------------------
    void RenderCommandList::AddLoadTextureCommand(RenderTexture* renderTexture, std::unique_ptr<const u8[]> textureData, u32 textureDataSize) noexcept
    {
        RenderCommandUPtr renderCommand(new LoadTextureRenderCommand(renderTexture, std::move(textureData), textureDataSize));
        
        m_orderedCommands.push_back(renderCommand.get());
        m_renderCommands.push_back(std::move(renderCommand));
    }
    
    //------------------------------------------------------------------------------
    void RenderCommandList::AddUnloadMeshCommand(RenderMeshUPtr renderMesh) noexcept
    {
        RenderCommandUPtr renderCommand(new UnloadMeshRenderCommand(std::move(renderMesh)));
        
        m_orderedCommands.push_back(renderCommand.get());
        m_renderCommands.push_back(std::move(renderCommand));
    }
    
    //------------------------------------------------------------------------------
    void RenderCommandList::AddUnloadShaderCommand(RenderShaderUPtr renderShader) noexcept
    {
        RenderCommandUPtr renderCommand(new UnloadShaderRenderCommand(std::move(renderShader)));
        
        m_orderedCommands.push_back(renderCommand.get());
        m_renderCommands.push_back(std::move(renderCommand));
    }
    
    //------------------------------------------------------------------------------
    void RenderCommandList::AddUnloadTextureCommand(RenderTextureUPtr renderTexture) noexcept
    {
        RenderCommandUPtr renderCommand(new UnloadTextureRenderCommand(std::move(renderTexture)));
        
        m_orderedCommands.push_back(renderCommand.get());
        m_renderCommands.push_back(std::move(renderCommand));
    }
}
