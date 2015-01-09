//
//  RenderComponentFactory.h
//  Chilli Source
//  Created by S Downie on 29/09/2010.
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

#ifndef _CHILLISOURCE_RENDERING_BASE_RENDERCOMPONENTFACTORY_H_
#define _CHILLISOURCE_RENDERING_BASE_RENDERCOMPONENTFACTORY_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/System/AppSystem.h>
#include <ChilliSource/Rendering/Sprite/SpriteComponent.h>

namespace ChilliSource
{
	namespace Rendering
	{
        //--------------------------------------------------------
        /// System that has convenience methods for creating
        /// render components
        ///
        /// @author S Downie
        //--------------------------------------------------------
		class RenderComponentFactory final : public Core::AppSystem
		{
		public:
            
			CS_DECLARE_NAMEDTYPE(RenderComponentFactory);

            //---------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Interface Id
            ///
            /// @return If the object is of the given interface type
            //---------------------------------------------------------------------------
            bool IsA(Core::InterfaceIDType in_interfaceId) const override;
            //---------------------------------------------------------------------------
			/// Creates a sprite component with the given size and material
			///
            /// @author S Downie
			///
            /// @param Dimensions
            /// @param Material
            /// @param Size policy for setting the size of the sprite based on the
            /// texture size
            ///
            /// @return Ownership of new sprite component
			//---------------------------------------------------------------------------
			SpriteComponentUPtr CreateSpriteComponent(const Core::Vector2& in_size, const MaterialCSPtr& in_material, SpriteComponent::SizePolicy in_sizePolicy);
            //---------------------------------------------------------------------------
			/// Creates a sprite component thats size and texture is based on the
            /// given material and texture atlas
			///
            /// @author S Downie
			///
            /// @param Dimensions
            /// @param Texture atlas
            /// @param Atlas Id
            /// @param Material
            /// @param Size policy for setting the size of the sprite based on the
            /// texture atlas frame size
            ///
            /// @return Ownership of new sprite component
			//---------------------------------------------------------------------------
			SpriteComponentUPtr CreateSpriteComponent(const Core::Vector2& in_size, const TextureAtlasCSPtr& in_textureAtlas, const std::string& in_textureId, const MaterialCSPtr& in_material, SpriteComponent::SizePolicy in_sizePolicy);
            //---------------------------------------------------------------------------
			/// Creates a static mesh component with the given material
			///
            /// @author S Downie
			///
            /// @param Material
            ///
            /// @return Ownership of new mesh component
			//---------------------------------------------------------------------------
			StaticMeshComponentUPtr CreateStaticMeshComponent(const MeshCSPtr& in_model, const MaterialCSPtr& in_material);
            //---------------------------------------------------------------------------
			/// Creates an animated mesh component with the given material.
            ///
            /// NOTE: No animations will have been set on the component
			///
            /// @author S Downie
			///
            /// @param Material
            ///
            /// @return Ownership of new mesh component
			//---------------------------------------------------------------------------
			AnimatedMeshComponentUPtr CreateAnimatedMeshComponent(const MeshCSPtr& in_model, const MaterialCSPtr& in_material);
			//---------------------------------------------------------------------------
			/// Create a camera with a perspective projection
            ///
            /// @author S Downie
            ///
            /// @param Field of view in radians
            /// @param Distance of near clip plane from the camera
            /// @param Distance of far clip plane from the camera
			///
			/// @return Ownership of new camera component
			//---------------------------------------------------------------------------
			PerspectiveCameraComponentUPtr CreatePerspectiveCameraComponent(f32 in_fov, f32 in_near, f32 in_far);
            //---------------------------------------------------------------------------
			/// Create a camera with an orthographic projection
            ///
            /// @author S Downie
            ///
            /// @param Viewport size
            /// @param Distance of near clip plane from the camera
            /// @param Distance of far clip plane from the camera
			///
			/// @return Ownership of new camera component
			//---------------------------------------------------------------------------
            OrthographicCameraComponentUPtr CreateOrthographicCameraComponent(const Core::Vector2& in_viewportSize, f32 in_near, f32 in_far);
            //---------------------------------------------------------------------------
			/// Create a camera with an orthographic projection. The viewport of
            /// the camera is set to the screen resolution
            ///
            /// @author S Downie
            ///
            /// @param Distance of near clip plane from the camera
            /// @param Distance of far clip plane from the camera
			///
			/// @return Ownership of new camera component
			//---------------------------------------------------------------------------
			OrthographicCameraComponentUPtr CreateOrthographicCameraComponent(f32 in_near, f32 in_far);
			//---------------------------------------------------------------------------
			/// Create a new light component that is applied in the ambient render pass.
            /// This light simulates global light scattering as a base colour
			///
			/// @author S Downie
            ///
            /// @return Ownership of new ambient light component
			//---------------------------------------------------------------------------
            AmbientLightComponentUPtr CreateAmbientLightComponent() const;
            //---------------------------------------------------------------------------
			/// Create a new light component that is applied in the diffuse render pass.
            /// This light simulates direct light from an infinitely far away light source
            /// where the light waves travel in parallel along a single vector.
            ///
            /// NOTE: This light type can cast shadows into a shadow map of the given
            /// resolution. If ZERO then no shadows are cast
			///
			/// @author S Downie
            ///
            /// @return Ownership of new directional light component
			//---------------------------------------------------------------------------
			DirectionalLightComponentUPtr CreateDirectionalLightComponent(u32 in_shadowMapRes = 0) const;
            //---------------------------------------------------------------------------
            /// Create a new light component that is applied in the diffuse render pass.
            /// This light simulates direct light from nearby light sources where the light
            /// is omni-directional
            ///
            /// @author S Downie
            ///
            /// @return Ownership of new point light component
            //---------------------------------------------------------------------------
            PointLightComponentUPtr CreatePointLightComponent() const;
            //---------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param The particle effect the particle effect component should be
            /// created with.
            ///
            /// @return A new particle effect component.
            //---------------------------------------------------------------------------
            ParticleEffectComponentUPtr CreateParticleEffectComponent(const ParticleEffectCSPtr& in_particleEffect) const;
            
        private:
            
            friend class Core::Application;
            //--------------------------------------------------------
            /// Creation method used by application to instantiate
            /// the system
            ///
            /// @author S Downie
            ///
            /// @return Ownership of new factory
            //--------------------------------------------------------
			static RenderComponentFactoryUPtr Create();
            //--------------------------------------------------------
            /// Private constructor to enforce the use of factory
            /// method
            ///
            /// @author S Downie
            //--------------------------------------------------------
			RenderComponentFactory() = default;
            //--------------------------------------------------------
            /// Called when the system is created
            ///
            /// @author S Downie
            //--------------------------------------------------------
            void OnInit() override;

		private:
			
            Core::Screen* m_screen = nullptr;
		};
	}
}

#endif
