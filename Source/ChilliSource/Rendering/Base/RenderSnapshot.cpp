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

#include <ChilliSource/Rendering/Base/RenderSnapshot.h>

namespace ChilliSource
{
    //------------------------------------------------------------------------------
    RenderSnapshot::RenderSnapshot(const Integer2& resolution, const Colour& clearColour) noexcept
        : m_resolution(resolution), m_clearColour(clearColour), m_preRenderCommandList(new RenderCommandList()), m_postRenderCommandList(new RenderCommandList())
    {
    }
    
    //------------------------------------------------------------------------------
    void RenderSnapshot::SetRenderCamera(const RenderCamera& renderCamera) noexcept
    {
        CS_ASSERT(!m_renderCameraClaimed, "Render camera cannot be set after it has been claimed.");
        
        m_renderCamera = renderCamera;
    }
    
    //------------------------------------------------------------------------------
    void RenderSnapshot::AddRenderAmbientLight(const RenderAmbientLight& renderAmbientLight) noexcept
    {
        CS_ASSERT(!m_renderAmbientLightsClaimed, "Render ambient light list cannot be changed after it has been claimed.");
        
        m_renderAmbientLights.push_back(renderAmbientLight);
    }
    
    //------------------------------------------------------------------------------
    void RenderSnapshot::AddRenderDirectionalLight(const RenderDirectionalLight& renderDirectionalLight) noexcept
    {
        CS_ASSERT(!m_renderDirectionalLightsClaimed, "Render directional light list cannot be changed after it has been claimed.");
        
        m_renderDirectionalLights.push_back(renderDirectionalLight);
    }
    
    //------------------------------------------------------------------------------
    void RenderSnapshot::AddRenderPointLight(const RenderPointLight& renderPointLight) noexcept
    {
        CS_ASSERT(!m_renderPointLightsClaimed, "Render point light list cannot be changed after it has been claimed.");
        
        m_renderPointLights.push_back(renderPointLight);
    }
    
    //------------------------------------------------------------------------------
    void RenderSnapshot::AddRenderObject(const RenderObject& renderObject) noexcept
    {
        CS_ASSERT(!m_renderObjectsClaimed, "Render object list cannot be changed after it has been claimed.");
        
        m_renderObjects.push_back(renderObject);
    }
    
    //------------------------------------------------------------------------------
    void RenderSnapshot::AddRenderDynamicMesh(RenderDynamicMeshUPtr renderDynamicMesh) noexcept
    {
        CS_ASSERT(!m_renderDynamicMeshesClaimed, "Render dynamic meshes list cannot be changed after it has been claimed.");
        
        m_renderDynamicMeshes.push_back(std::move(renderDynamicMesh));
    }
    
    //------------------------------------------------------------------------------
    RenderCommandList* RenderSnapshot::GetPreRenderCommandList() noexcept
    {
        CS_ASSERT(m_preRenderCommandList, "Pre-RenderCommandList cannot be modified after it has been claimed.");
        
        return m_preRenderCommandList.get();
    }
    
    //------------------------------------------------------------------------------
    RenderCommandList* RenderSnapshot::GetPostRenderCommandList() noexcept
    {
        CS_ASSERT(m_postRenderCommandList, "Post-RenderCommandList cannot be modified after it has been claimed.");
        
        return m_postRenderCommandList.get();
    }
    
    //------------------------------------------------------------------------------
    RenderCamera RenderSnapshot::ClaimRenderCamera() noexcept
    {
        CS_ASSERT(!m_renderCameraClaimed, "Render camera has already been claimed.");
        
        m_renderCameraClaimed = true;
        return std::move(m_renderCamera);
    }
    
    //------------------------------------------------------------------------------
    std::vector<RenderAmbientLight> RenderSnapshot::ClaimRenderAmbientLights() noexcept
    {
        CS_ASSERT(!m_renderAmbientLightsClaimed, "Render ambient lights have already been claimed.");
        
        m_renderAmbientLightsClaimed = true;
        return std::move(m_renderAmbientLights);
    }
    
    //------------------------------------------------------------------------------
    std::vector<RenderDirectionalLight> RenderSnapshot::ClaimRenderDirectionalLights() noexcept
    {
        CS_ASSERT(!m_renderDirectionalLightsClaimed, "Render directional lights have already been claimed.");
        
        m_renderDirectionalLightsClaimed = true;
        return std::move(m_renderDirectionalLights);
    }
    
    //------------------------------------------------------------------------------
    std::vector<RenderPointLight> RenderSnapshot::ClaimRenderPointLights() noexcept
    {
        CS_ASSERT(!m_renderPointLightsClaimed, "Render point lights have already been claimed.");
        
        m_renderPointLightsClaimed = true;
        return std::move(m_renderPointLights);
    }
    
    //------------------------------------------------------------------------------
    std::vector<RenderObject> RenderSnapshot::ClaimRenderObjects() noexcept
    {
        CS_ASSERT(!m_renderObjectsClaimed, "Render objects have already been claimed.");
        
        m_renderObjectsClaimed = true;
        return std::move(m_renderObjects);
    }
    
    //------------------------------------------------------------------------------
    std::vector<RenderDynamicMeshUPtr> RenderSnapshot::ClaimRenderDynamicMeshes() noexcept
    {
        CS_ASSERT(!m_renderDynamicMeshesClaimed, "Render dynamic meshes have already been claimed.");
        
        m_renderDynamicMeshesClaimed = true;
        return std::move(m_renderDynamicMeshes);
    }
    
    //------------------------------------------------------------------------------
    RenderCommandListUPtr RenderSnapshot::ClaimPreRenderCommandList() noexcept
    {
        CS_ASSERT(m_preRenderCommandList, "Pre-RenderCommandList has already been claimed.");
        
        return std::move(m_preRenderCommandList);
    }
    
    //------------------------------------------------------------------------------
    RenderCommandListUPtr RenderSnapshot::ClaimPostRenderCommandList() noexcept
    {
        CS_ASSERT(m_postRenderCommandList, "Post-RenderCommandList has already been claimed.");
        
        return std::move(m_postRenderCommandList);
    }
};
