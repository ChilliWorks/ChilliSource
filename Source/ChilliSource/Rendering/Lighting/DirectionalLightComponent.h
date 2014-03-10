/*
 *  DirectionalLightComponent.h
 *  moFlo
 *
 *  Created by Scott Downie on 31/1/2014.
 *  Copyright 2014 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLOW_RENDERING_COMPONENTS_DIRECTIONALLIGHTCOMPONENT_H_
#define _MOFLOW_RENDERING_COMPONENTS_DIRECTIONALLIGHTCOMPONENT_H_

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
            /// @param Shadow map target texture
            /// @param Shadow map debug target texture
            //----------------------------------------------------------
			DirectionalLightComponent(const TextureSPtr& inpShadowMapTarget, const TextureSPtr& inpShadowMapDebugTarget = TextureSPtr());
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
            const Core::Matrix4x4& GetLightMatrix() const override;
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
            
        private:
            
            Core::Matrix4x4 mmatProj;
            
            TextureSPtr mpShadowMap;
            TextureSPtr mpShadowMapDebug;
            
            Core::ConnectionUPtr m_transformChangedConnection;
            
            f32 mfShadowTolerance;
            
            mutable bool mbMatrixCacheValid;
		};
    }
}

#endif