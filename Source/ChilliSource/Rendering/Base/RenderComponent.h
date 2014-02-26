/*
 *  RenderableComponent.h
 *  moFlo
 *
 *  Created by Scott Downie on 30/09/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#ifndef _MO_FLO_RENDERING_RENDER_COMPONENT_H_
#define _MO_FLO_RENDERING_RENDER_COMPONENT_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Volume/VolumeComponent.h>
#include <ChilliSource/Core/Math/Geometry/Shapes.h>
#include <ChilliSource/Rendering/Material/Material.h>

namespace ChilliSource
{
	namespace Rendering
	{
		//====================================================
		/// Description:
		///
		/// Any object that can be rendered
		//====================================================
		class RenderComponent : public Core::IVolumeComponent
		{
		public:
			DECLARE_NAMED_INTERFACE(RenderComponent);
			RenderComponent();
			virtual ~RenderComponent(){}
			
			//----------------------------------------------------
			/// Get Axis Aligned Bounding Box
			///
			/// All render components have a box used for culling
			/// and coarse intersections. This is cached and
			/// recomputed when required.
			/// @return Axis aligned bounding box
			//----------------------------------------------------
			virtual const Core::AABB& GetAABB(){return mBoundingBox;}
			//----------------------------------------------------
			/// Get Object Oriented Bounding Box
			///
			/// All render objects have an OOBB for
			/// picking. This is cached and
			/// recomputed when required.
			/// @return OOBB
			//----------------------------------------------------
			virtual const Core::OOBB& GetOOBB(){return mOBBoundingBox;}
			//----------------------------------------------------
			/// Get Bounding Sphere
			///
			/// All render objects have an bounding sphere for
			/// culling. This is cached and
			/// recomputed when required.
			/// @return bounding sphere
			//----------------------------------------------------
			virtual const Core::Sphere& GetBoundingSphere(){return mBoundingSphere;}
			//----------------------------------------------------
			/// Is Visible
			///
			/// @return Whether or not to render
			//----------------------------------------------------
			bool IsVisible() const;
			//----------------------------------------------------
			/// Set Visible
			///
			/// @param Whether or not to render
			//----------------------------------------------------
			void SetVisible(bool inbVisible);
            //----------------------------------------------------
			/// Should Cull
			///
			/// @return Whether or not to cull the object
			//----------------------------------------------------
			bool IsCullingEnabled() const;
			//----------------------------------------------------
			/// Set Cull Enabled
			///
			/// @param Whether or not to cull the object
			//----------------------------------------------------
			void SetCullingEnabled(bool inbCull);
            //-----------------------------------------------------
            /// Set Shadow Casting Enabled
            ///
            /// @param Whether the render component casts shadows
            //-----------------------------------------------------
            void SetShadowCastingEnabled(bool inbEnabled);
            //-----------------------------------------------------
            /// Is Shadow Casting Enabled
            ///
            /// @return Whether the render component casts shadows
            //-----------------------------------------------------
            bool IsShadowCastingEnabled() const;
			//-----------------------------------------------------
            /// Get Transformation Matrix
            ///
            /// @return Objects transform
            //-----------------------------------------------------
            const Core::CMatrix4x4& GetTransformationMatrix();
			//-----------------------------------------------------
            /// Render 
            ///
            /// Purely abstract method that render components
            /// overload in order to draw themselves. 
            ///
            /// @param Render system
            /// @param Active camera component
            /// @param The current shader pass.
            //-----------------------------------------------------
            virtual void Render(RenderSystem* inpRenderSystem, CameraComponent* inpCam, ShaderPass inePass) = 0;
            //-----------------------------------------------------
            /// Render Shadow Map
            ///
            /// Render the mesh to the shadow map
            ///
            /// @param Render system
            /// @param Active camera component
            //-----------------------------------------------------
            virtual void RenderShadowMap(RenderSystem* inpRenderSystem, CameraComponent* inpCam) = 0;
            //-----------------------------------------------------------
			/// Set Material
			///
			/// Set the material that the sprite will use
			///
			/// @param Handle to material
			//-----------------------------------------------------------
			virtual void SetMaterial(const MaterialPtr &inpMaterial);
			//-----------------------------------------------------------
			/// Get Material
			///
			/// Get the material that the sprite uses
			///
			/// @return Handle to material
			//-----------------------------------------------------------
			virtual const MaterialPtr& GetMaterial() const;
            //-----------------------------------------------------------
			/// Get Material
			///
			/// Get the material that the sprite uses
			///
			/// @return Handle to material
			//-----------------------------------------------------------
			MaterialPtr& GetMaterial();
			//-----------------------------------------------------------
			/// Is Transparent
			///
			/// Returns whether or not this component has any transparency
			///
			/// @return whether or not this has transparency
			//-----------------------------------------------------------
			virtual bool IsTransparent();
            //-----------------------------------------------------------
			/// Get Sort Value
			///
			/// Returns a user defined f32 value used for sort predicates
			///
			/// @return f32 sort value
			//-----------------------------------------------------------
            f32 GetSortValue() const { return mfSortValue; }
            //-----------------------------------------------------------
			/// Set Sort Value
			///
			/// Sets a user defined f32 value used for sort predicates
			///
			/// @param f32 sort value
			//-----------------------------------------------------------
            void SetSortValue(const f32 infSortValue) { mfSortValue = infSortValue; }
            
		protected:
			
			Core::AABB mBoundingBox;
			Core::OOBB mOBBoundingBox;
			Core::Sphere mBoundingSphere;

			MaterialPtr mpMaterial;
            
        private:
            
            f32 mfSortValue;
            
			bool mbVisible;
            bool mbShouldCull;
            
            bool mbCastsShadows;
		};
	}
}

#endif
