//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
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

#include <ChilliSource/Rendering/Lighting/DirectionalLightComponent.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Delegate/MakeDelegate.h>
#include <ChilliSource/Core/Image/ImageCompression.h>
#include <ChilliSource/Core/Image/ImageFormat.h>
#include <ChilliSource/Core/Resource/ResourcePool.h>
#include <ChilliSource/Core/Entity/Entity.h>
#include <ChilliSource/Core/Resource/ResourcePool.h>
#include <ChilliSource/Rendering/Base/RenderCapabilities.h>
#include <ChilliSource/Rendering/Base/RenderSnapshot.h>
#include <ChilliSource/Rendering/Target/RenderTargetGroupManager.h>
#include <ChilliSource/Rendering/Texture/Texture.h>
#include <ChilliSource/Rendering/Texture/TextureDesc.h>

namespace ChilliSource
{
    namespace
    {
        constexpr f32 k_defaultShadowTolerance = 0.005f;
        constexpr f32 k_defaultShadowVolumeWidth = 50.0f;
        constexpr f32 k_defaultShadowVolumeHeight = 50.0f;
        constexpr f32 k_defaultShadowVolumeNear = 5.0f;
        constexpr f32 k_defaultShadowVolumeFar = 50.0f;
        
        u32 g_nextShadowMapId = 0;
        
        /// Gets the shadow map resolution for the given ShadowQuality.
        ///
        /// @param shadowQuality
        ///     The quality of the shadows.
        ///
        /// @return The resolution.
        ///
        Integer2 GetShadowMapResolution(DirectionalLightComponent::ShadowQuality shadowQuality) noexcept
        {
            switch (shadowQuality)
            {
                case DirectionalLightComponent::ShadowQuality::k_low:
                    return Integer2(512, 512);
                case DirectionalLightComponent::ShadowQuality::k_medium:
                    return Integer2(1024, 1024);
                case DirectionalLightComponent::ShadowQuality::k_high:
                    return Integer2(2048, 2048);
                default:
                    CS_LOG_FATAL("Invalid shadow quality.");
                    return Integer2::k_zero;
            }
        }
    }
    
    CS_DEFINE_NAMEDTYPE(DirectionalLightComponent);
    
    //------------------------------------------------------------------------------
    DirectionalLightComponent::DirectionalLightComponent(const Colour& colour, f32 intensity) noexcept
        : m_colour(colour), m_intensity(intensity)
    {
    }
    
    //------------------------------------------------------------------------------
    DirectionalLightComponent::DirectionalLightComponent(ShadowQuality shadowQuality, const Colour& colour, f32 intensity) noexcept
        : m_colour(colour), m_intensity(intensity)
    {
        auto renderCapabilities = Application::Get()->GetSystem<RenderCapabilities>();
        if (renderCapabilities->IsShadowMappingSupported())
        {
            m_shadowMapId = ++g_nextShadowMapId;
            m_shadowMapResolution = GetShadowMapResolution(shadowQuality);
            m_shadowTolerance = k_defaultShadowTolerance;
            
            SetShadowVolume(k_defaultShadowVolumeWidth, k_defaultShadowVolumeHeight, k_defaultShadowVolumeNear, k_defaultShadowVolumeFar);
            
            TryCreateShadowMapTarget();
        }
    }
    
    //------------------------------------------------------------------------------
    bool DirectionalLightComponent::IsA(InterfaceIDType interfaceId) const noexcept
    {
        return (DirectionalLightComponent::InterfaceID == interfaceId);
    }
    
    //------------------------------------------------------------------------------
    void DirectionalLightComponent::SetShadowVolume(f32 width, f32 height, f32 near, f32 far) noexcept
    {
        m_lightProjection = Matrix4::CreateOrthographicProjectionLH(width, height, near, far);
    }
    
    //------------------------------------------------------------------------------
    void DirectionalLightComponent::TryCreateShadowMapTarget() noexcept
    {
        CS_ASSERT(!m_shadowMap, "Shadow map already exists.");
        CS_ASSERT(!m_shadowMapTarget, "Shadow map target already exists.");

        if(m_shadowMapResolution.x > 0 && m_shadowMapResolution.y > 0)
        {
            auto mutableShadowMap = Application::Get()->GetResourcePool()->CreateResource<Texture>("_DirectionalLightShadowMap" + ToString(m_shadowMapId));
            
            TextureDesc desc(m_shadowMapResolution, ImageFormat::k_Depth16, ImageCompression::k_none, false);
            mutableShadowMap->Build(nullptr, 0, desc);
            mutableShadowMap->SetLoadState(Resource::LoadState::k_loaded);
            
            m_shadowMap = mutableShadowMap;
            
            m_shadowMapTarget = TargetGroup::CreateDepthTargetGroup(m_shadowMap);
        }
    }
    
    //------------------------------------------------------------------------------
    void DirectionalLightComponent::TryDestroyShadowMapTarget() noexcept
    {
        if (m_shadowMapTarget)
        {
            m_shadowMapTarget.reset();
        }
        
        if (m_shadowMap)
        {
            auto resourcePool = Application::Get()->GetResourcePool();
            
            auto release = m_shadowMap.get();
            m_shadowMap.reset();
            resourcePool->Release(release);
        }
    }
    
    //------------------------------------------------------------------------------
    void DirectionalLightComponent::OnAddedToScene() noexcept
    {
        auto& transform = GetEntity()->GetTransform();
        
        m_direction = Vector3::Rotate(Vector3::k_unitPositiveZ, transform.GetWorldOrientation());
        
        m_transformChangedConnection = transform.GetTransformChangedEvent().OpenConnection(MakeDelegate(this, &DirectionalLightComponent::OnEntityTransformChanged));
    }
    
    //------------------------------------------------------------------------------
    void DirectionalLightComponent::OnEntityTransformChanged() noexcept
    {
        auto& transform = GetEntity()->GetTransform();
        
        m_direction = Vector3::Rotate(Vector3::k_unitPositiveZ, transform.GetWorldOrientation());
    }
    
    //------------------------------------------------------------------------------
    void DirectionalLightComponent::OnRenderSnapshot(RenderSnapshot& renderSnapshot, IAllocator* frameAllocator) noexcept
    {
        if (m_shadowMap)
        {
            const auto& transform = GetEntity()->GetTransform();
            auto worldMatrix = transform.GetWorldTransform();
            auto orientation = transform.GetWorldOrientation();
            renderSnapshot.AddDirectionalRenderLight(DirectionalRenderLight(GetFinalColour(), m_direction, worldMatrix, m_lightProjection, orientation, m_shadowTolerance, m_shadowMapTarget->GetRenderTargetGroup()));
        }
        else
        {
            renderSnapshot.AddDirectionalRenderLight(DirectionalRenderLight(GetFinalColour(), m_direction));
        }
    }
    
    //------------------------------------------------------------------------------
    void DirectionalLightComponent::OnRemovedFromScene() noexcept
    {
        m_transformChangedConnection.reset();
    }
    
    //------------------------------------------------------------------------------
    DirectionalLightComponent::~DirectionalLightComponent() noexcept
    {
        TryDestroyShadowMapTarget();
    }
}

