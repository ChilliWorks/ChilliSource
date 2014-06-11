/*
 *  SpriteComponent.h
 *  moFlo
 *
 *  Created by Scott Downie on 29/09/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#ifndef _MO_FLO_RENDERING_SPRITE_COMPONENT_H_
#define _MO_FLO_RENDERING_SPRITE_COMPONENT_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Rendering/Base/RenderComponent.h>
#include <ChilliSource/Core/Math/Vector2.h>
#include <ChilliSource/Core/Base/ByteColour.h>
#include <ChilliSource/Core/Base/Colour.h>
#include <ChilliSource/Core/Math/Geometry/Shapes.h>
#include <ChilliSource/Rendering/Base/AlignmentAnchors.h>

namespace ChilliSource
{
	namespace Rendering
	{
        const u32 kudwVertsPerSprite = 4;
        const u32 kudwIndicesPerSprite = 6;
        
        //CCW winding order (back face cull)
		const u16 kauwLocalIndices[] = {0,1,2,1,3,2};
		//CW winding order (front face cull)
		//const u16 kauwLocalIndices[] = {0,2,1,1,2,3};
        
		//===============================================================
		/// Description:
		///
		/// A sprite component. This defines a 2D object that can
		/// be manipulated, textured and animated.
		//===============================================================
		class SpriteComponent : public RenderComponent
		{
		public: 
			
            enum class Verts
            {
                k_topLeft,
                k_bottomLeft,
                k_topRight,
                k_bottomRight
            };
            
            struct SpriteVertex
            {
                Core::Vector4 vPos;
                Core::Vector2 vTex;
                Core::ByteColour Col;
            };
            
            struct SpriteData
            {
                SpriteVertex sVerts[kudwVertsPerSprite];
                
                MaterialCSPtr pMaterial;
            };
			
			CS_DECLARE_NAMEDTYPE(SpriteComponent);
			SpriteComponent();
			//----------------------------------------------------------
			/// Is A
			///
			/// Returns if it is of the type given
			/// @param Comparison Type
			/// @return Whether the class matches the comparison type
			//----------------------------------------------------------
			virtual bool IsA(Core::InterfaceIDType inInterfaceID) const override;
			//----------------------------------------------------
			/// Get Axis Aligned Bounding Box
			///
			/// All render components have a box used for culling
			/// and coarse intersections. This is cached and 
			/// recomputed when required.
			/// @return Axis aligned bounding box
			//----------------------------------------------------
			virtual const Core::AABB& GetAABB() override;
			//----------------------------------------------------
			/// Get Object Oriented Bounding Box
			///
			/// All render objects have an OOBB for
			/// picking. This is cached and 
			/// recomputed when required.
			/// @return OOBB
			//----------------------------------------------------
			const Core::OOBB& GetOOBB() override;
			//----------------------------------------------------
			/// Get Bounding Sphere
			///
			/// All render objects have an bounding sphere for
			/// culling. This is cached and 
			/// recomputed when required.
			/// @return bounding sphere
			//----------------------------------------------------
			const Core::Sphere& GetBoundingSphere() override;
			//-----------------------------------------------------------
			/// Set Dimensions
			///
			/// @param Vector containing width and height
			//-----------------------------------------------------------
			void SetDimensions(const Core::Vector2 &invDims);
			//-----------------------------------------------------------
			/// Set Dimensions
			///
			/// @param Width
			/// @param Height
			//-----------------------------------------------------------
			void SetDimensions(const f32 infWidth, const f32 infHeight);
			//-----------------------------------------------------------
			/// Set Width Unfactored
			///
			/// @param Width
			//-----------------------------------------------------------
			void SetWidth(const f32 infWidth);
			//-----------------------------------------------------------
			/// Set Height Unfactored
			///
			/// @param Height
			//-----------------------------------------------------------
			void SetHeight(const f32 infHeight);
			//-----------------------------------------------------------
			/// Get Dimensions
			///
			/// @return Vector containing width and height
			//-----------------------------------------------------------
			const Core::Vector2& GetDimensions() const;
			//-----------------------------------------------------------
			/// Get Dimensions Unfactored
			///
			/// @return Vector containing width and height, without applying
			/// the screen factor
			//-----------------------------------------------------------
			const Core::Vector2& GetDimensionsUnfactored() const;
			//-----------------------------------------------------------
			/// Set UV's
			///
			/// Set the texture co-ordinates
			///
			/// @param Rect containing uv, st
			//-----------------------------------------------------------
			void SetUVs(const Core::Rectangle &inUVs);
			//-----------------------------------------------------------
			/// Set UV's
			///
			/// Set the UV texture coordinates
			///
			/// @param U
			/// @param S
			/// @param V
			/// @param T
			//-----------------------------------------------------------
			void SetUVs(const f32 infUStart, const f32 infUWidth, const f32 infVStart, const f32 infVHeight);
			//-----------------------------------------------------------
			/// Get UVs (Un-transformed)
			///
			/// @return Rect with UV and ST
			//-----------------------------------------------------------
			const Core::Rectangle& GetUVs() const;
			//-----------------------------------------------------------
			/// Set Colour
			///
			/// Set the RGBA colour
			///
			/// @param Colour containing RGBA
			//-----------------------------------------------------------
			void SetColour(const Core::Colour &inCol);
			//-----------------------------------------------------------
			/// Set Colour
			///
			/// Set the RGBA colour
			///
			/// @param Red
			/// @param Green
			/// @param Blue
			/// @param Alpha
			//-----------------------------------------------------------
			void SetColour(const f32 infR, const f32 infG, const f32 infB, const f32 infA);
			//-----------------------------------------------------------
			/// Get Colour
			///
			/// @return Sprite colour
			//-----------------------------------------------------------
			const Core::Colour& GetColour() const;

			//-----------------------------------------------------------
			/// Get Current Frame
			///
			/// Build the UV's based on the current frame and sprite
			/// manipulations
			///
			/// @param Rect containing the UV and ST
			//-----------------------------------------------------------
			const Core::Rectangle& GetCurrentFrame();
			//-----------------------------------------------------------
			/// Set Flipped Horizontal
			///
			/// Flip the sprite about it's local x-axis
			///
			/// @param Whether to flip or not
			//-----------------------------------------------------------
			void SetFlippedHorizontal(bool inbValue);
			//-----------------------------------------------------------
			/// Get Flipped Horizontal
			///
			/// @param Whether sprite is flipped about it's local x-axis
			//-----------------------------------------------------------
			bool GetFlippedHorizontal() const;
			//-----------------------------------------------------------
			/// Set Flipped Vertical
			///
			/// Flip the sprite about it's local y-axis
			///
			/// @param Whether to flip or not
			//-----------------------------------------------------------
			void SetFlippedVertical(bool inbValue);
			//-----------------------------------------------------------
			/// Get Flipped Vertical
			///
			/// @param Whether sprite is flipped about it's local y-axis
			//-----------------------------------------------------------
			bool GetFlippedVertical() const;
			//-----------------------------------------------------------
            /// Set Origin Alignment
            ///
			/// Controls where in the sprite it's origin is assumed to be
			///
			/// @param Alignment to set origin to be (AlignmentAnchor::k_middleCentre) by default
			//-----------------------------------------------------------
			void SetOriginAlignment(AlignmentAnchor ineAlignment);
			//-----------------------------------------------------------
            /// Get Origin Alignment 
            ///
			/// @return Returns alignment of this sprite's origin
			//-----------------------------------------------------------
			AlignmentAnchor  GetOriginAlignment() const;
            //-----------------------------------------------------------
            /// Get Upper Left Corner Position
            ///
            /// @return Position of corner after transform
            //-----------------------------------------------------------
            const CSCore::Vector4 & GetUpperLeftCornerPos();
            //-----------------------------------------------------------
            /// Get Lower Left Corner Position
            ///
            /// @return Position of corner after transform
            //-----------------------------------------------------------
			const CSCore::Vector4 & GetLowerLeftCornerPos();
            //-----------------------------------------------------------
            /// Get Upper Right Corner Position
            ///
            /// @return Position of corner after transform
            //-----------------------------------------------------------
			const CSCore::Vector4 & GetUpperRightCornerPos();
            //-----------------------------------------------------------
            /// Get Lower Right Corner Position
            ///
            /// @return Position of corner after transform
            //-----------------------------------------------------------
			const CSCore::Vector4 & GetLowerRightCornerPos();
            //-----------------------------------------------------------
            /// Get Sprite Data
            ///
            /// @return Struct containing minimal sprite data for rendering
            //-----------------------------------------------------------
            const SpriteData& GetSpriteData();
            //-----------------------------------------------------------
            /// Calculate Sprite Data
            ///
            /// If the transform, UV or colour cache is invalid
            /// we will recalculate the sprite vertex data
            //-----------------------------------------------------------
            void CalculateSpriteData();
            //-----------------------------------------------------------
            /// Render
            ///
            /// Visited by the render system so we can add ourself
            /// to the sprite batch for rendering and flush the
            /// batch if we have a different material
            ///
            /// @param Render system
            /// @param Active camera component
            /// @param The current shader pass.
            //-----------------------------------------------------------
            void Render(RenderSystem* inpRenderSystem, CameraComponent* inpCam, ShaderPass ineShaderPass) override;
            //-----------------------------------------------------
            /// Render Shadow Map
            ///
            /// Render the mesh to the shadow map
            ///
            /// @param Render system
            /// @param Active camera component
            /// @param Material to render static shadows with
            /// @param Material to render skinned shadows with
            //-----------------------------------------------------
            void RenderShadowMap(RenderSystem* inpRenderSystem, CameraComponent* inpCam, const MaterialCSPtr& in_staticShadowMap, const MaterialCSPtr& in_animShadowMap) override {};
			//----------------------------------------------------
			/// Triggered when the component is attached to
			/// an entity on the scene
            ///
            /// @author S Downie
			//----------------------------------------------------
			void OnAddedToScene() override;
			//----------------------------------------------------
			/// Triggered when the component is removed from
			/// an entity on the scene
            ///
            /// @author S Downie
			//----------------------------------------------------
			void OnRemovedFromScene() override;
            
		protected:	
            //------------------------------------------------------------
            /// On Transform Changed
            ///
            /// Our local transform or our parent transform has changed
            /// We must rebuild our sprite data
            //------------------------------------------------------------
            void OnTransformChanged();
            //-----------------------------------------------------------
            /// Calculate Corner Positions
            ///
            /// If the transform cache is invalid we must calculate
            /// the corner positions from the new world transform
            //-----------------------------------------------------------
			void CalculateCornerPositions();
			//-----------------------------------------------------------
			/// Set Colour With Opacity
			///
			/// Set the RGBA colour of the sprite
            /// taking the parent entity's opacity into consideration
			///
			/// @param Colour containing RGBA
			//-----------------------------------------------------------
			void SetColourWithOpacity(const Core::Colour &inCol);
			//-----------------------------------------------------------
			/// Set Colour With Opacity
			///
			/// Set the RGBA colour of the sprite
            /// taking the parent entity's opacity into consideration
			///
			/// @param Red
			/// @param Green
			/// @param Blue
			/// @param Alpha
			//-----------------------------------------------------------
			void SetColourWithOpacity(const f32 infR, const f32 infG, const f32 infB, const f32 infA);
			//-----------------------------------------------------------
			/// Get Colour With Opacity
			///
			/// @return Sprite colour
			//-----------------------------------------------------------
			const Core::Colour& GetColourWithOpacity() const;
		private:
            
            Core::EventConnectionUPtr m_transformChangedConnection;
            
			SpriteData mSpriteData;
            
            Core::Matrix4 mmatTransformCache;
            
            Core::Vector4 mavVertexPos[kudwVertsPerSprite];

			Core::Vector2 mvDimensions;
			
			Core::Rectangle mUVs;
			Core::Rectangle mTransformedUVs;
            
            Core::Colour mColour;
            
            Core::Colour mColourWithOpacity;
            Core::ByteColour mByteColourWithOpacity;
			
            AlignmentAnchor     meAlignment;
            
			bool mbFlippedHorizontal;
			bool mbFlippedVertical;
            bool mbCornerPosCacheValid;
            bool mbUVCacheValid;
            
            bool mbBoundingSphereValid;
            bool mbAABBValid;
            bool mbOOBBValid;
		};
	}
}

#endif
