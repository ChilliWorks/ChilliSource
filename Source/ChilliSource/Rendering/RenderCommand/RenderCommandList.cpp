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

#include <ChilliSource/Rendering/RenderCommand/Commands/ApplyAmbientLightRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/ApplyCameraRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/ApplyDirectionalLightRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/ApplyDynamicMeshRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/ApplyMaterialRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/ApplyMeshRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/ApplyPointLightRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/BeginRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/EndRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/LoadMaterialGroupRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/LoadMeshRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/LoadShaderRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/LoadTextureRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/RenderInstanceRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/UnloadMaterialGroupRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/UnloadMeshRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/UnloadShaderRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/UnloadTextureRenderCommand.h>

namespace ChilliSource
{
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
    void RenderCommandList::AddLoadMaterialGroupCommand(RenderMaterialGroup* renderMaterialGroup) noexcept
    {
        RenderCommandUPtr renderCommand(new LoadMaterialGroupRenderCommand(renderMaterialGroup));
        
        m_orderedCommands.push_back(renderCommand.get());
        m_renderCommands.push_back(std::move(renderCommand));
    }
    
    //------------------------------------------------------------------------------
    void RenderCommandList::AddLoadMeshCommand(RenderMesh* renderMesh, std::unique_ptr<const u8[]> vertexData, u32 vertexDataSize, std::unique_ptr<const u8[]> indexData, u32 indexDataSize) noexcept
    {
        RenderCommandUPtr renderCommand(new LoadMeshRenderCommand(renderMesh, std::move(vertexData), vertexDataSize, std::move(indexData), indexDataSize));
        
        m_orderedCommands.push_back(renderCommand.get());
        m_renderCommands.push_back(std::move(renderCommand));
    }
    
    //------------------------------------------------------------------------------
    void RenderCommandList::AddBeginCommand(const Integer2& resolution, const Colour& clearColour) noexcept
    {
        RenderCommandUPtr renderCommand(new BeginRenderCommand(resolution, clearColour));
        
        m_orderedCommands.push_back(renderCommand.get());
        m_renderCommands.push_back(std::move(renderCommand));
    }
    
    //------------------------------------------------------------------------------
    void RenderCommandList::AddApplyCameraCommand(const Vector3& position, const Matrix4& viewProjectionMatrix) noexcept
    {
        RenderCommandUPtr renderCommand(new ApplyCameraRenderCommand(position, viewProjectionMatrix));
        
        m_orderedCommands.push_back(renderCommand.get());
        m_renderCommands.push_back(std::move(renderCommand));
    }
    
    //------------------------------------------------------------------------------
    void RenderCommandList::AddApplyAmbientLightCommand(const Colour& colour) noexcept
    {
        RenderCommandUPtr renderCommand(new ApplyAmbientLightRenderCommand(colour));
        
        m_orderedCommands.push_back(renderCommand.get());
        m_renderCommands.push_back(std::move(renderCommand));
    }
    
    //------------------------------------------------------------------------------
    void RenderCommandList::AddApplyDirectionalLightCommand(const Colour& colour, const Vector3& direction, const Matrix4& lightViewProjection, f32 shadowTolerance, const RenderTexture* shadowMapRenderTexture) noexcept
    {
        RenderCommandUPtr renderCommand(new ApplyDirectionalLightRenderCommand(colour, direction, lightViewProjection, shadowTolerance, shadowMapRenderTexture));
        
        m_orderedCommands.push_back(renderCommand.get());
        m_renderCommands.push_back(std::move(renderCommand));
    }
    
    //------------------------------------------------------------------------------
    void RenderCommandList::AddApplyPointLightCommand(const Colour& colour, const Vector3& position, const Vector3& attenuation) noexcept
    {
        RenderCommandUPtr renderCommand(new ApplyPointLightRenderCommand(colour, position, attenuation));
        
        m_orderedCommands.push_back(renderCommand.get());
        m_renderCommands.push_back(std::move(renderCommand));
    }
    
    //------------------------------------------------------------------------------
    void RenderCommandList::AddApplyMaterialCommand(const RenderMaterial* renderMaterial) noexcept
    {
        RenderCommandUPtr renderCommand(new ApplyMaterialRenderCommand(renderMaterial));
        
        m_orderedCommands.push_back(renderCommand.get());
        m_renderCommands.push_back(std::move(renderCommand));
    }
    
    //------------------------------------------------------------------------------
    void RenderCommandList::AddApplyMeshCommand(const RenderMesh* renderMesh) noexcept
    {
        RenderCommandUPtr renderCommand(new ApplyMeshRenderCommand(renderMesh));
        
        m_orderedCommands.push_back(renderCommand.get());
        m_renderCommands.push_back(std::move(renderCommand));
    }
    
    //------------------------------------------------------------------------------
    void RenderCommandList::AddApplyDynamicMeshCommand(const RenderDynamicMesh* renderDynamicMesh) noexcept
    {
        RenderCommandUPtr renderCommand(new ApplyDynamicMeshRenderCommand(renderDynamicMesh));
        
        m_orderedCommands.push_back(renderCommand.get());
        m_renderCommands.push_back(std::move(renderCommand));
    }
    
    //------------------------------------------------------------------------------
    void RenderCommandList::AddRenderInstanceCommand(const Matrix4& worldMatrix) noexcept
    {
        RenderCommandUPtr renderCommand(new RenderInstanceRenderCommand(worldMatrix));
        
        m_orderedCommands.push_back(renderCommand.get());
        m_renderCommands.push_back(std::move(renderCommand));
    }
    
    //------------------------------------------------------------------------------
    void RenderCommandList::AddEndCommand() noexcept
    {
        RenderCommandUPtr renderCommand(new EndRenderCommand());
        
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
    void RenderCommandList::AddUnloadMaterialGroupCommand(RenderMaterialGroupUPtr renderMaterialGroup) noexcept
    {
        RenderCommandUPtr renderCommand(new UnloadMaterialGroupRenderCommand(std::move(renderMaterialGroup)));
        
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
    
    //------------------------------------------------------------------------------
    void RenderCommandList::AddUnloadShaderCommand(RenderShaderUPtr renderShader) noexcept
    {
        RenderCommandUPtr renderCommand(new UnloadShaderRenderCommand(std::move(renderShader)));
        
        m_orderedCommands.push_back(renderCommand.get());
        m_renderCommands.push_back(std::move(renderCommand));
    }
}
