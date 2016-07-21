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
    RenderSnapshot::RenderSnapshot(const RenderTargetGroup* renderTarget, const Integer2& resolution, const Colour& clearColour, const RenderCamera& in_renderCamera) noexcept
        : m_offscreenRenderTarget(renderTarget), m_resolution(resolution), m_clearColour(clearColour), m_renderCamera(in_renderCamera), m_preRenderCommandList(new RenderCommandList()), m_postRenderCommandList(new RenderCommandList()),
          m_renderFrameData()
    {
    }
    
    //------------------------------------------------------------------------------
    void RenderSnapshot::AddAmbientRenderLight(const AmbientRenderLight& renderAmbientLight) noexcept
    {
        CS_ASSERT(!m_renderAmbientLightsClaimed, "Render ambient light list cannot be changed after it has been claimed.");
        
        m_renderAmbientLights.push_back(renderAmbientLight);
    }
    
    //------------------------------------------------------------------------------
    void RenderSnapshot::AddDirectionalRenderLight(const DirectionalRenderLight& renderDirectionalLight) noexcept
    {
        CS_ASSERT(!m_renderDirectionalLightsClaimed, "Render directional light list cannot be changed after it has been claimed.");
        
        m_renderDirectionalLights.push_back(renderDirectionalLight);
    }
    
    //------------------------------------------------------------------------------
    void RenderSnapshot::AddPointRenderLight(const PointRenderLight& renderPointLight) noexcept
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
    void RenderSnapshot::AddRenderDynamicMesh(RenderDynamicMeshAUPtr renderDynamicMesh) noexcept
    {
        CS_ASSERT(!m_renderFrameDataClaimed, "Cannot add RenderDynamicMesh after RenderFrameData has been claimed.");
        
        m_renderFrameData.AddRenderDynamicMesh(std::move(renderDynamicMesh));
    }
    
    //------------------------------------------------------------------------------
    void RenderSnapshot::AddRenderSkinnedAnimation(RenderSkinnedAnimationAUPtr renderSkinnedAnimation) noexcept
    {
        CS_ASSERT(!m_renderFrameDataClaimed, "Cannot add RenderDynamicMesh after RenderFrameData has been claimed.");
        
        m_renderFrameData.AddRenderSkinnedAnimation(std::move(renderSkinnedAnimation));
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
    std::vector<AmbientRenderLight> RenderSnapshot::ClaimAmbientRenderLights() noexcept
    {
        CS_ASSERT(!m_renderAmbientLightsClaimed, "Render ambient lights have already been claimed.");
        
        m_renderAmbientLightsClaimed = true;
        return std::move(m_renderAmbientLights);
    }
    
    //------------------------------------------------------------------------------
    std::vector<DirectionalRenderLight> RenderSnapshot::ClaimDirectionalRenderLights() noexcept
    {
        CS_ASSERT(!m_renderDirectionalLightsClaimed, "Render directional lights have already been claimed.");
        
        m_renderDirectionalLightsClaimed = true;
        return std::move(m_renderDirectionalLights);
    }
    
    //------------------------------------------------------------------------------
    std::vector<PointRenderLight> RenderSnapshot::ClaimPointRenderLights() noexcept
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
    
    //------------------------------------------------------------------------------
    RenderFrameData RenderSnapshot::ClaimRenderFrameData() noexcept
    {
        CS_ASSERT(!m_renderFrameDataClaimed, "RenderFrameData already claimed.");
        
        m_renderFrameDataClaimed = true;
        
        return std::move(m_renderFrameData);
    }
};
