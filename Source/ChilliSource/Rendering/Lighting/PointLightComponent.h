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

#ifndef _CHILLISOURCE_RENDERING_LIGHTING_POINTLIGHTCOMPONENT_H_
#define _CHILLISOURCE_RENDERING_LIGHTING_POINTLIGHTCOMPONENT_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/Colour.h>
#include <ChilliSource/Core/Math/Vector3.h>
#include <ChilliSource/Core/Entity/Component.h>

namespace ChilliSource
{
    /// A component which describes a directional light. While this is in the scene all lit
    /// objects in range will have point lighting applied to it.
    ///
    /// A point light is described in terms of a radius and a minimum light influence which
    /// are used to calculate the light attenuation and range of influence. Where d is
    /// the distance from the fragment to the light, the formulae for these are as follows:
    ///
    ///   attenuationConstant = 1
    ///   attenuationLinear = 2 / radius
    ///   attenuationQuatratic = 1 / (radius * radius)
    ///   attenuation = 1 / (attenuationConstant + attenuationLinear * d + attenuationQuatratic * d * d)
    ///
    /// rangeOfInfluence = radius * (sqrt(intensity / minLightInfluence) + 1)
    ///
    /// This is not thread-safe and should only be accessed from the main thread.
    ///
    class PointLightComponent final : public Component
    {
    public:
        CS_DECLARE_NAMEDTYPE(PointLightComponent);
        
        /// Creates a new point light with the given colour, radius and intensity.
        ///
        /// @param colour
        ///     The colour of the point light.
        /// @param radius
        ///     The radius of the point light.
        /// @param intensity
        ///     (Optional) The intensity of the ambient light. This defaults to 1.0.
        ///
        PointLightComponent(const Colour& colour, f32 radius, f32 intensity = 1.0f) noexcept;

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
        
        /// @return The colour of the point light.
        ///
        void SetColour(const Colour& colour) noexcept { m_colour = colour; }
        
        /// @return The intensity of the point light.
        ///
        void SetIntensity(f32 intensity) noexcept { m_intensity = intensity; }
        
        /// Sets the radius of the point light. See the class documentation for more information.
        ///
        /// @param radius
        ///     The lights radius.
        ///
        void SetRadius(f32 radius) noexcept;
        
        /// Sets the minimum light influence. See the class documentation for more information.
        ///
        /// @param minLightInfluence
        ///     The minimum light influence.
        ///
        void SetMinLightInfluence(f32 minLightInfluence) noexcept;
        
        /// @return The colour of the point light.
        ///
        const Colour& GetColour() const noexcept { return m_colour; }
        
        /// @return The intensity of the point light.
        ///
        f32 GetIntensity() const noexcept { return m_intensity; }
        
        /// @return The colour of the point light with the intesity applied to it.
        ///
        Colour GetFinalColour() const noexcept { return m_colour * m_intensity; }
        
        /// @return The light radius.
        ///
        f32 GetRadius() const noexcept { return m_radius; };
        
        /// @return The min light influence.
        ///
        f32 GetMinLightInfluence() const noexcept { return m_minLightInfluence; };

        /// @return The range in which the light has influence on objects. This is calculated based on
        ///     the Min Light Influence and the radius of the light.
        ///
        f32 GetRangeOfInfluence() const noexcept { return m_rangeOfInfluence; };

    private:
        /// Calculates the range of influence of the light based on the formula described in the
        /// class documentation.
        ///
        void CalcRangeOfInfluence() noexcept;
        
        /// Triggered when either the component is attached to an entity which is already in the
        /// scene or when the owning entity is added to the scene.
        ///
        void OnAddedToScene() noexcept override;
        
        /// Triggered when the entity transform changes, updating the light view matrix.
        ///
        void OnEntityTransformChanged() noexcept;
        
        /// This is called during the render snapshot stage of the render pipeline to collect a copy of
        /// the current state of the renderable portion of the scene. This will add all relevant light
        /// data to this snapshot.
        ///
        /// @param renderSnapshot
        ///     The snapshot that the light data will be added to.
        /// @param frameAllocator
        ///     Allocate any memory for rendering this frame from here
        ///
        void OnRenderSnapshot(RenderSnapshot& renderSnapshot, IAllocator* frameAllocator) noexcept override;
        
        /// Triggered when either the component is removed from an entity which is currently in the
        /// scene, or the owning entity is removed from the scene.
        ///
        void OnRemovedFromScene() noexcept override;
        
        Colour m_colour;
        f32 m_intensity;
        f32 m_radius;
        f32 m_minLightInfluence;
        
        Vector3 m_lightPosition;
        f32 m_rangeOfInfluence = 0.0f;
        Vector3 m_attenuation;
        
        EventConnectionUPtr m_transformChangedConnection;
    };
}

#endif