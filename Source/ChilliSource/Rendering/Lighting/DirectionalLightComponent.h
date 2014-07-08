//
//  DirectionalLightComponent.h
//  Chilli Source
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

#ifndef _CHILLISOURCE_RENDERING_COMPONENTS_DIRECTIONALLIGHTCOMPONENT_H_
#define _CHILLISOURCE_RENDERING_COMPONENTS_DIRECTIONALLIGHTCOMPONENT_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Rendering/Lighting/LightComponent.h>

namespace ChilliSource
{
	namespace Rendering
	{
		class DirectionalLightComponent : public LightComponent
		{
		public:
			CS_DECLARE_NAMEDTYPE(DirectionalLightComponent);
			
            //----------------------------------------------------------
            /// Constructor
            ///
            /// @param The resolution of the shadow map. No shadow map
            /// will be used if 0 is passed in.
            //----------------------------------------------------------
			DirectionalLightComponent(u32 in_shadowMapRes);
			//----------------------------------------------------------
			/// Is A
			///
			/// Returns if it is of the type given
			/// @param Comparison Type
			/// @return Whether the class matches the comparison type
			//----------------------------------------------------------
			bool IsA(Core::InterfaceIDType inInterfaceID) const override;
            //----------------------------------------------------------
			/// Set Shadow Volume
			///
			/// @param Width
            /// @param Height
            /// @param Near
            /// @param Far
			//----------------------------------------------------------
			void SetShadowVolume(f32 infWidth, f32 infHeight, f32 infNear, f32 infFar);
            //----------------------------------------------------------
			/// Get Shadow Tolerance
			///
			/// @return Shadow tolerance factor
			//----------------------------------------------------------
			f32 GetShadowTolerance() const;
            //----------------------------------------------------------
			/// Set Shadow Tolerance
			///
			/// @param Shadow tolerance factor
			//----------------------------------------------------------
			void SetShadowTolerance(f32 infTolerance);
            //----------------------------------------------------------
            /// Get Direction
            ///
            /// @return Direction vector of light
            /// (only applies to directional lights)
            //----------------------------------------------------------
            Core::Vector3 GetDirection() const;
            //----------------------------------------------------------
            /// Get Light Matrix
            ///
            /// @return Matrix to transform into light space
            //----------------------------------------------------------
            const Core::Matrix4& GetLightMatrix() const override;
            //----------------------------------------------------
			/// Triggered when the component is attached to
			/// an entity on the scene
            ///
            /// @author S Downie
			//----------------------------------------------------
            void OnAddedToScene() override;
			//----------------------------------------------------
			/// Triggered when the component is removed from
			/// an entityon the scene
            ///
            /// @author S Downie
			//----------------------------------------------------
            void OnRemovedFromScene() override;
            //----------------------------------------------------
            /// On Entity Transform Changed
            ///
            /// Triggered when the entity transform changes
            /// and invalidates the light VP matrix
            //----------------------------------------------------
            void OnEntityTransformChanged();
            //----------------------------------------------------------
			/// Get Shadow Map Ptr
			///
			/// @return Shadow map texture
			//----------------------------------------------------------
			const TextureSPtr& GetShadowMapPtr() const;
            //----------------------------------------------------------
			/// Get Shadow Map Debug Ptr
			///
			/// @return Shadow map debug colour texture
			//----------------------------------------------------------
			const TextureSPtr& GetShadowMapDebugPtr() const;
            //----------------------------------------------------------
            /// Destructor
            ///
            /// @author Ian Copland
            //----------------------------------------------------------
			~DirectionalLightComponent();
        private:
            //----------------------------------------------------
            /// Create shadow map textures if they don't already
            /// exist
            ///
            /// @author Ian Copland
            //----------------------------------------------------
            void CreateShadowMapTextures();
            //----------------------------------------------------
            /// Destroys the shadow map textures if they currently
            /// exist.
            ///
            /// @author Ian Copland
            //----------------------------------------------------
            void DestroyShadowMapTextures();
            
            RenderCapabilities* m_renderCapabilities = nullptr;
            
            Core::Matrix4 mmatProj;
            mutable bool mbMatrixCacheValid;
            
            Core::EventConnectionUPtr m_transformChangedConnection;
            
            f32 mfShadowTolerance;
            u32 m_shadowMapRes;
            u32 m_shadowMapId;
            TextureSPtr m_shadowMap;
            TextureSPtr m_shadowMapDebug;
		};
    }
}

#endif