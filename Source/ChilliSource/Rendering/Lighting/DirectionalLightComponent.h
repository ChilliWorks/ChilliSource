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

#ifndef _CHILLISOURCE_RENDERING_LIGHTING_DIRECTIONALLIGHTCOMPONENT_H_
#define _CHILLISOURCE_RENDERING_LIGHTING_DIRECTIONALLIGHTCOMPONENT_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/Colour.h>
#include <ChilliSource/Core/Entity/Component.h>
#include <ChilliSource/Core/Math/Matrix4.h>
#include <ChilliSource/Rendering/Target/TargetGroup.h>

namespace ChilliSource
{
    /// A component which describes a directional light. While this is in the scene all lit
    /// objects will have directional lighting applied to it. Optionally, the light can
    /// cast shadows.
    ///
    /// This is not thread-safe and should only be accessed from the main thread.
    ///
    class DirectionalLightComponent final : public Component
    {
    public:
        CS_DECLARE_NAMEDTYPE(DirectionalLightComponent);
        
        /// An enum describing the different levels of shadow quality. The shadow quality affects the
        /// size of the shadow map texture used: low uses 512 x 512; medium uses 1024 x 1024; and
        /// high uses 2048 x 2048.
        ///
        enum class ShadowQuality
        {
            k_low,
            k_medium,
            k_high
        };
        
        /// Creates a new directional light which does not cast shadows with the given colour and
        /// intensity.
        ///
        /// @param colour
        ///     The colour of the ambient light.
        /// @param intensity
        ///     (Optional) The intensity of the ambient light. This defaults to 1.0.
        ///
        DirectionalLightComponent(const Colour& colour, f32 intensity = 1.0f) noexcept;
        
        /// Creates a new directional light which cast shadows with the given colour
        /// and intensity.
        ///
        /// @param colour
        ///     The colour of the ambient light.
        /// @param intensity
        ///     (Optional) The intensity of the ambient light. This defaults to 1.0.
        ///
        DirectionalLightComponent(ShadowQuality shadowQuality, const Colour& colour, f32 intensity = 1.0f) noexcept;
        
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
        
        /// @return The colour of the directional light.
        ///
        void SetColour(const Colour& colour) noexcept { m_colour = colour; }
        
        /// @return The intensity of the directional light.
        ///
        void SetIntensity(f32 intensity) noexcept { m_intensity = intensity; }
        
        /// Sets the shadow tolerance factor which affects the distance an object must be behind another
        /// object to be considered in shadow. Too small a value will result in shadow banding, however
        /// too large a value will result in "peter panning", where shadows become detatched from their
        /// casting object.
        ///
        /// @param tolerance
        ///     The shadow tolerance.
        ///
        void SetShadowTolerance(f32 tolerance) noexcept { m_shadowTolerance = tolerance; }
        
        /// Sets the shadow volume; the box around the light in which shadows can be cast. The box
        /// is oriented to the direction of the light, with near and far dictating how near and far
        /// along the light direction shadows can be cast, and with and height describing distance
        /// perpendicular to the light direction.
        ///
        /// A smaller shadow volume results in higher quality shadows, but also a smaller area with
        /// shadows cast.
        ///
        /// @param width
        ///     The width of the shadow volume.
        /// @param height
        ///     The height of the shadow volume.
        /// @param near
        ///     The near plane of the shadow volume.
        /// @param far
        ///     The far plane of the shadow volume.
        ///
        void SetShadowVolume(f32 width, f32 height, f32 near, f32 far) noexcept;
        
        /// @return The colour of the directional light.
        ///
        const Colour& GetColour() const noexcept { return m_colour; }
        
        /// @return The intensity of the directional light.
        ///
        f32 GetIntensity() const noexcept { return m_intensity; }
        
        /// @return The colour of the directional light with the intesity applied to it.
        ///
        Colour GetFinalColour() const noexcept { return m_colour * m_intensity; }
        
        /// @return The shadow tolerance factor which affects the distance an object must be behind
        ///     another object to be considered in shadow. Too small a value will result in shadow
        ///     banding, however too large a value will result in "peter panning", where shadows
        ///     become detatched from their casting object.
        ///
        f32 GetShadowTolerance() const noexcept { return m_shadowTolerance; }
        
        /// Cleans up shadow textures if required.
        ///
        ~DirectionalLightComponent() noexcept;
        
    private:
        /// Create the shadow map target and texture if required.
        ///
        void TryCreateShadowMapTarget() noexcept;
        
        /// Destroys the shadow map target and texture if required.
        ///
        void TryDestroyShadowMapTarget() noexcept;
        
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
        /// @param renderSnapshot - Add anything to this that must be captured for rendering
        /// @param frameAllocator - Allocate any memory required for rendering from this
        ///
        /// @return renderSnapshot
        ///     The snapshot that the light data will be added to.
        ///
        void OnRenderSnapshot(RenderSnapshot& renderSnapshot, IAllocator* frameAllocator) noexcept override;
        
        /// Triggered when either the component is removed from an entity which is currently in the
        /// scene, or the owning entity is removed from the scene.
        ///
        void OnRemovedFromScene() noexcept override;
        
        Colour m_colour;
        f32 m_intensity;
        f32 m_shadowTolerance = 0.0f;
        
        Vector3 m_direction;
        Matrix4 m_lightProjection;
        Integer2 m_shadowMapResolution;
        s32 m_shadowMapId = -1;
        TextureCSPtr m_shadowMap;
        TargetGroupUPtr m_shadowMapTarget = nullptr;
        
        EventConnectionUPtr m_transformChangedConnection;
    };
}

#endif