//
//  PointLightComponent.cpp
//  ChilliSource
//  Created by Scott Downie on 31/01/2014.
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

#include <ChilliSource/Rendering/Lighting/PointLightComponent.h>

#include <ChilliSource/Core/Delegate/MakeDelegate.h>
#include <ChilliSource/Core/Entity/Entity.h>
#include <ChilliSource/Rendering/Base/RenderSnapshot.h>
#include <ChilliSource/Rendering/Lighting/PointRenderLight.h>

#include <limits>

namespace ChilliSource
{
    namespace
    {
        constexpr f32 m_defaultMinLightInfluence = 0.05f;
    }
    
    CS_DEFINE_NAMEDTYPE(PointLightComponent);

    //------------------------------------------------------------------------------
    PointLightComponent::PointLightComponent(const Colour& colour, f32 radius, f32 intensity) noexcept
        : m_colour(colour), m_intensity(intensity)
    {
        SetRadius(radius);
        SetMinLightInfluence(m_defaultMinLightInfluence);
    }
    
    //------------------------------------------------------------------------------
    bool PointLightComponent::IsA(InterfaceIDType interfaceId) const noexcept
    {
        return (PointLightComponent::InterfaceID == interfaceId);
    }
    
    //------------------------------------------------------------------------------
    void PointLightComponent::SetRadius(f32 radius) noexcept
    {
        CS_ASSERT(radius > 0.0f, "Point light radius must be greater than zero.");
        
        m_radius = radius;
        
        m_attenuation.x = 1;
        m_attenuation.y = 2 / m_radius;
        m_attenuation.z = 1 / (m_radius * m_radius);
        
        CalcRangeOfInfluence();
    }
    
    //------------------------------------------------------------------------------
    void PointLightComponent::SetMinLightInfluence(f32 minLightInfluence) noexcept
    {
        CS_ASSERT(minLightInfluence > 0.0f, "Point light min light influence must be greater than zero.");
        
        m_minLightInfluence = minLightInfluence;
        
        CalcRangeOfInfluence();
    }
    
    //------------------------------------------------------------------------------
    void PointLightComponent::CalcRangeOfInfluence() noexcept
    {
        m_rangeOfInfluence = m_radius * (sqrt(GetIntensity() / m_minLightInfluence) + 1);
    }
    
    //------------------------------------------------------------------------------
    void PointLightComponent::OnAddedToScene() noexcept
    {
        auto& transform = GetEntity()->GetTransform();
        
        m_lightPosition = transform.GetWorldPosition();
        
        m_transformChangedConnection = transform.GetTransformChangedEvent().OpenConnection(MakeDelegate(this, &PointLightComponent::OnEntityTransformChanged));
    }
    
    //------------------------------------------------------------------------------
    void PointLightComponent::OnEntityTransformChanged() noexcept
    {
        m_lightPosition = GetEntity()->GetTransform().GetWorldPosition();
    }
    
    //------------------------------------------------------------------------------
    void PointLightComponent::OnRenderSnapshot(RenderSnapshot& renderSnapshot, IAllocator* frameAllocator) noexcept
    {
        renderSnapshot.AddPointRenderLight(PointRenderLight(GetFinalColour(), m_lightPosition, m_attenuation, m_rangeOfInfluence));
    }
    
    //------------------------------------------------------------------------------
    void PointLightComponent::OnRemovedFromScene() noexcept
    {
        m_transformChangedConnection.reset();
    }
}

