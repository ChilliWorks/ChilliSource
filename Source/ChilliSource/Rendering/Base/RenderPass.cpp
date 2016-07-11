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

#include <ChilliSource/Rendering/Base/RenderPass.h>

namespace ChilliSource
{
    //------------------------------------------------------------------------------
    RenderPass::RenderPass() noexcept
        : m_lightType(LightType::k_none), m_ambientLight(AmbientRenderLight(Colour::k_black))
    {
    }
    
    //------------------------------------------------------------------------------
    RenderPass::RenderPass(std::vector<RenderPassObject> renderPassObjects) noexcept
        : m_renderPassObjects(std::move(renderPassObjects)), m_lightType(LightType::k_none), m_ambientLight(AmbientRenderLight(Colour::k_black))
    {
    }
    
    //------------------------------------------------------------------------------
    RenderPass::RenderPass(const AmbientRenderLight& light, std::vector<RenderPassObject> renderPassObjects) noexcept
    : m_renderPassObjects(std::move(renderPassObjects)), m_lightType(LightType::k_ambient), m_ambientLight(light)
    {
    }
    
    //------------------------------------------------------------------------------
    RenderPass::RenderPass(const PointRenderLight& light, std::vector<RenderPassObject> renderPassObjects) noexcept
        : m_renderPassObjects(std::move(renderPassObjects)), m_lightType(LightType::k_point), m_pointLight(light)
    {
    }
    
    //------------------------------------------------------------------------------
    RenderPass::RenderPass(const DirectionalRenderLight& light, std::vector<RenderPassObject> renderPassObjects) noexcept
        : m_renderPassObjects(std::move(renderPassObjects)), m_lightType(LightType::k_directional), m_directionalLight(light)
    {
    }
    
    //------------------------------------------------------------------------------
    const AmbientRenderLight& RenderPass::GetAmbientLight() const noexcept
    {
        CS_ASSERT(m_lightType == LightType::k_ambient, "Ambient light not set for pass");
        
        return m_ambientLight;
    }
    
    //------------------------------------------------------------------------------
    const PointRenderLight& RenderPass::GetPointLight() const noexcept
    {
        CS_ASSERT(m_lightType == LightType::k_point, "Point light not set for pass");
        
        return m_pointLight;
    }
    
    //------------------------------------------------------------------------------
    const DirectionalRenderLight& RenderPass::GetDirectionalLight() const noexcept
    {
        CS_ASSERT(m_lightType == LightType::k_directional, "Ambient light not set for pass");
        
        return m_directionalLight;
    }
}