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
#include <ChilliSource/Core/Base/Colour.h>
#include <ChilliSource/Core/Math/Geometry/Shapes.h>
#include <ChilliSource/Rendering/Base/AlignmentAnchors.h>

namespace ChilliSource
{
	namespace Rendering
	{
        const u32 kudwVertsPerSprite = 4;
        const u32 kudwIndicesPerSprite = 6;
        
        //CCW winding order
		//const u16 kauwLocalIndices[] = {0,1,2,1,3,2};
		//CW winding order - We use this for the time being as the model format is CW
		const u16 kauwLocalIndices[] = {0,2,1,1,2,3};
        
		//===============================================================
		/// Description:
		///
		/// A sprite component. This defines a 2D object that can
		/// be manipulated, textured and animated.
		//===============================================================
		class CSpriteComponent : public IRenderComponent
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
                Core::CVector4 vPos;
                Core::CVector2 vTex;
                Core::CColour::ByteColour Col;
            };
            
            struct SpriteData
            {
                SpriteVertex sVerts[kudwVertsPerSprite];
                
                MaterialPtr pMaterial;
            };
			
			DECLARE_NAMED_INTERFACE(CSpriteComponent);
			CSpriteComponent();
            virtual ~CSpriteComponent();
			//----------------------------------------------------------
			/// Is A
			///
			/// Returns if it is of the type given
			/// @param Comparison Type
			/// @return Whether the class matches the comparison type
			//----------------------------------------------------------
			virtual bool IsA(Core::InterfaceIDType inInterfaceID) const;
			//----------------------------------------------------
			/// Get Axis Aligned Bounding Box
			///
			/// All render components have a box used for culling
			/// and coarse intersections. This is cached and 
			/// recomputed when required.
			/// @return Axis aligned bounding box
			//----------------------------------------------------
			virtual const Core::AABB& GetAABB();
			//----------------------------------------------------
			/// Get Object Oriented Bounding Box
			///
			/// All render objects have an OOBB for
			/// picking. This is cached and 
			/// recomputed when required.
			/// @return OOBB
			//----------------------------------------------------
			const Core::OOBB& GetOOBB();
			//----------------------------------------------------
			/// Get Bounding Sphere
			///
			/// All render objects have an bounding sphere for
			/// culling. This is cached and 
			/// recomputed when required.
			/// @return bounding sphere
			//----------------------------------------------------
			const Core::Sphere& GetBoundingSphere();
			//-----------------------------------------------------------
			/// Set Dimensions
			///
			/// @param Vector containing width and height
			//-----------------------------------------------------------
			void SetDimensions(const Core::CVector2 &invDims);
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
			const Core::CVector2& GetDimensions() const;
			//-----------------------------------------------------------
			/// Get Dimensions Unfactored
			///
			/// @return Vector containing width and height, without applying
			/// the screen factor
			//-----------------------------------------------------------
			const Core::CVector2& GetDimensionsUnfactored() const;
			//-----------------------------------------------------------
			/// Set UV's
			///
			/// Set the texture co-ordinates
			///
			/// @param Rect containing uv, st
			//-----------------------------------------------------------
			void SetUVs(const Core::CRect &inUVs);
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
			const Core::CRect& GetUVs() const;
			//-----------------------------------------------------------
			/// Set Colour
			///
			/// Set the RGBA colour
			///
			/// @param Colour containing RGBA
			//-----------------------------------------------------------
			void SetColour(const Core::CColour &inCol);
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
			const Core::CColour& GetColour() const;

			//-----------------------------------------------------------
			/// Get Current Frame
			///
			/// Build the UV's based on the current frame and sprite
			/// manipulations
			///
			/// @param Rect containing the UV and ST
			//-----------------------------------------------------------
			const Core::CRect& GetCurrentFrame();
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
			void SetOriginAlignment(Core::AlignmentAnchor ineAlignment);
			//-----------------------------------------------------------
            /// Get Origin Alignment 
            ///
			/// @return Returns alignment of this sprite's origin
			//-----------------------------------------------------------
			Core::AlignmentAnchor  GetOriginAlignment() const;
            //-----------------------------------------------------------
            /// Get Upper Left Corner Position
            ///
            /// @return Position of corner after transform
            //-----------------------------------------------------------
            const ChilliSource::Core::CVector4 & GetUpperLeftCornerPos();
            //-----------------------------------------------------------
            /// Get Lower Left Corner Position
            ///
            /// @return Position of corner after transform
            //-----------------------------------------------------------
			const ChilliSource::Core::CVector4 & GetLowerLeftCornerPos();
            //-----------------------------------------------------------
            /// Get Upper Right Corner Position
            ///
            /// @return Position of corner after transform
            //-----------------------------------------------------------
			const ChilliSource::Core::CVector4 & GetUpperRightCornerPos();
            //-----------------------------------------------------------
            /// Get Lower Right Corner Position
            ///
            /// @return Position of corner after transform
            //-----------------------------------------------------------
			const ChilliSource::Core::CVector4 & GetLowerRightCornerPos();
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
            void Render(IRenderSystem* inpRenderSystem, CCameraComponent* inpCam, ShaderPass ineShaderPass);
            //-----------------------------------------------------
            /// Render Shadow Map
            ///
            /// Render the mesh to the shadow map
            ///
            /// @param Render system
            /// @param Active camera component
            //-----------------------------------------------------
            void RenderShadowMap(IRenderSystem* inpRenderSystem, CCameraComponent* inpCam){};
			//----------------------------------------------------
			/// On Attached To Entity
			///
			/// Triggered when the component is attached to
			/// an entity
			//----------------------------------------------------
			void OnAttachedToEntity();
			//----------------------------------------------------
			/// On Detached From Entity
			///
			/// Triggered when the component is removed from
			/// an entity
			//----------------------------------------------------
			void OnDetachedFromEntity();
            
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
			void SetColourWithOpacity(const Core::CColour &inCol);
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
			const Core::CColour& GetColourWithOpacity() const;
		private:
			SpriteData mSpriteData;
            
            Core::CMatrix4x4 mmatTransformCache;
            
            Core::CVector4 mavVertexPos[kudwVertsPerSprite];

			Core::CVector2 mvDimensions;
			
			Core::CRect mUVs;
			Core::CRect mTransformedUVs;
            
            Core::CColour mColour;
            
            Core::CColour mColourWithOpacity;
            Core::CColour::ByteColour mByteColourWithOpacity;
			
            Core::AlignmentAnchor     meAlignment;
            
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