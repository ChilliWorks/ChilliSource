/*
 *  SpriteComponent.cpp
 *  moFlo
 *
 *  Created by Scott Downie on 30/09/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Core/Base/ColourUtils.h>
#include <ChilliSource/Core/Delegate/MakeDelegate.h>
#include <ChilliSource/Core/Entity/Entity.h>
#include <ChilliSource/Rendering/Sprite/SpriteComponent.h>
#include <ChilliSource/Rendering/Material/Material.h>
#include <ChilliSource/Rendering/Base/RenderSystem.h>
#include <ChilliSource/Rendering/Sprite/DynamicSpriteBatcher.h>
#include <ChilliSource/Rendering/Lighting/DirectionalLightComponent.h>
#include <ChilliSource/Rendering/Lighting/PointLightComponent.h>

#include <algorithm>

namespace ChilliSource
{
	namespace Rendering
	{
        CS_DEFINE_NAMEDTYPE(SpriteComponent);
		//----------------------------------------------------------
		/// Constructor
		//----------------------------------------------------------
		SpriteComponent::SpriteComponent() : mUVs(Core::Vector2(0, 0), Core::Vector2(1, 1)), mbFlippedVertical(false), mbFlippedHorizontal(false), 
        mbCornerPosCacheValid(false), meAlignment(AlignmentAnchor::k_middleCentre), mbUVCacheValid(false), mbBoundingSphereValid(false), mbAABBValid(false), mbOOBBValid(false)
		{
            mByteColourWithOpacity.r = 255;
            mByteColourWithOpacity.g = 255;
            mByteColourWithOpacity.b = 255;
            mByteColourWithOpacity.a = 255;
		}
		//----------------------------------------------------------
		/// Is A
		//----------------------------------------------------------
		bool SpriteComponent::IsA(ChilliSource::Core::InterfaceIDType inInterfaceID) const
		{
			return  (inInterfaceID == SpriteComponent::InterfaceID) || 
                    (inInterfaceID == RenderComponent::InterfaceID) ||
                    (inInterfaceID == VolumeComponent::InterfaceID);
		}
		//----------------------------------------------------
		/// Get Axis Aligned Bounding Box
		//----------------------------------------------------
		const Core::AABB& SpriteComponent::GetAABB()
		{
			if(GetEntity() && !mbAABBValid)
			{
                mbAABBValid = true;
                
				//Rebuild the box
				mBoundingBox.SetOrigin(GetEntity()->GetTransform().GetWorldPosition());
				mBoundingBox.SetSize(Core::Vector3(mvDimensions, 0.0f));
			}
			return mBoundingBox;
		}
		//----------------------------------------------------
		/// Get Object Oriented Bounding Box
		//----------------------------------------------------
		const Core::OOBB& SpriteComponent::GetOOBB()
		{
			if(GetEntity() && !mbOOBBValid)
			{
                mbOOBBValid = true;
                
				mOBBoundingBox.SetSize(Core::Vector3(mvDimensions, 0.0f));
				mOBBoundingBox.SetTransform(GetEntity()->GetTransform().GetWorldTransform());
			}
			return mOBBoundingBox;
		}
		//----------------------------------------------------
		/// Get Bounding Sphere
		//----------------------------------------------------
		const Core::Sphere& SpriteComponent::GetBoundingSphere()
		{
			if(GetEntity() && !mbBoundingSphereValid)
			{
                mbBoundingSphereValid = true;
                
				mBoundingSphere.vOrigin = GetEntity()->GetTransform().GetWorldPosition();
				mBoundingSphere.fRadius = std::max(mvDimensions.x, mvDimensions.y) * 0.5f;
			}
			return mBoundingSphere;
		}
		//-----------------------------------------------------------
		/// Set Dimensions Unfactored
		//-----------------------------------------------------------
		void SpriteComponent::SetDimensions(const Core::Vector2 &invDims)
		{
			SetDimensions(invDims.x, invDims.y);
		}
		//-----------------------------------------------------------
		/// Set Dimensions Unfactored
		//-----------------------------------------------------------
		void SpriteComponent::SetDimensions(const f32 infWidth, const f32 infHeight)
		{
			mbCornerPosCacheValid = false;
			
			mvDimensions = Core::Vector2(infWidth, infHeight);
		}
		//-----------------------------------------------------------
		/// Set Width Unfactored
		//-----------------------------------------------------------
		void SpriteComponent::SetWidth(const f32 infWidth)
		{
			mbCornerPosCacheValid = false;
			
			mvDimensions.x = infWidth;
		}
		//-----------------------------------------------------------
		/// Set Height Unfactored
		//-----------------------------------------------------------
		void SpriteComponent::SetHeight(const f32 infHeight)
		{
			mbCornerPosCacheValid = false;
			
			mvDimensions.y = infHeight;
		}
		//-----------------------------------------------------------
		/// Get Dimensions
		//-----------------------------------------------------------
		const Core::Vector2& SpriteComponent::GetDimensions() const
		{
			return mvDimensions;
		}
		//-----------------------------------------------------------
		/// Set UV's
		//-----------------------------------------------------------
		void SpriteComponent::SetUVs(const Core::Rectangle &inUVs)
		{
			mUVs = inUVs;
            
            mbUVCacheValid = false;
		}
		//-----------------------------------------------------------
		/// Set UV's
		//-----------------------------------------------------------
		void SpriteComponent::SetUVs(const f32 infUStart, const f32 infUWidth, const f32 infVStart, const f32 infVHeight)
		{
			mUVs.vOrigin.x = infUStart;
			mUVs.vSize.x = infUWidth;
			mUVs.vOrigin.y = infVStart;
			mUVs.vSize.y = infVHeight;
            
            mbUVCacheValid = false;
		}
		//-----------------------------------------------------------
		/// Get UVs
		//-----------------------------------------------------------
		const Core::Rectangle& SpriteComponent::GetUVs() const
		{
			return mUVs;
		}
		//-----------------------------------------------------------
		/// Set Colour
		//-----------------------------------------------------------
		void SpriteComponent::SetColour(const Core::Colour &inCol)
		{
			mColour = inCol;
            SetColourWithOpacity(mColour);
		}
		//-----------------------------------------------------------
		/// Set Colour
		//-----------------------------------------------------------
		void SpriteComponent::SetColour(const f32 infR, const f32 infG, const f32 infB, const f32 infA)
		{
			mColour = Core::Colour(infR, infG, infB, infA);
            SetColourWithOpacity(mColour);
		}
		//-----------------------------------------------------------
		/// Get Colour
		//-----------------------------------------------------------
		const Core::Colour& SpriteComponent::GetColour() const
		{
			return mColour;
		}
		//-----------------------------------------------------------
		/// Get Current Frame
		//-----------------------------------------------------------
		const Core::Rectangle& SpriteComponent::GetCurrentFrame()
		{
			if (mbFlippedHorizontal) 
			{
				mTransformedUVs.vOrigin.x = mUVs.vOrigin.x + mUVs.vSize.x;
				mTransformedUVs.vSize.x = - mUVs.vSize.x;
			} 
			else 
			{
				mTransformedUVs.vOrigin.x = mUVs.vOrigin.x;
				mTransformedUVs.vSize.x = mUVs.vSize.x;
			}

			if (mbFlippedVertical) 
			{
				mTransformedUVs.vOrigin.y = mUVs.vOrigin.y + mUVs.vSize.y;
				mTransformedUVs.vSize.y = - mUVs.vSize.y;
			} 
			else 
			{
				mTransformedUVs.vOrigin.y = mUVs.vOrigin.y;
				mTransformedUVs.vSize.y = mUVs.vSize.y;
			}

			return mTransformedUVs;
		}
		//-----------------------------------------------------------
		/// Set Flipped Horizontal
		//-----------------------------------------------------------
		void SpriteComponent::SetFlippedHorizontal(bool inbValue)
		{
			mbFlippedHorizontal = inbValue;
            mbUVCacheValid = false;
		}
		//-----------------------------------------------------------
		/// Get Flipped Horizontal
		//-----------------------------------------------------------
		bool SpriteComponent::GetFlippedHorizontal() const
		{
			return mbFlippedHorizontal;
		}
		//-----------------------------------------------------------
		/// Set Flipped Vertical
		//-----------------------------------------------------------
		void SpriteComponent::SetFlippedVertical(bool inbValue)
		{
			mbFlippedVertical = inbValue;
            mbUVCacheValid = false;
		}
		//-----------------------------------------------------------
		/// Get Flipped Vertical
		//-----------------------------------------------------------
		bool SpriteComponent::GetFlippedVertical() const
		{
			return mbFlippedVertical;
		}
		//-----------------------------------------------------------
		/// Set Origin Alignment
		//-----------------------------------------------------------
		void SpriteComponent::SetOriginAlignment(AlignmentAnchor ineAlignment)
        {
			meAlignment = ineAlignment;
			mbCornerPosCacheValid = false;
		}
		//-----------------------------------------------------------
		/// Get Origin Alignment
		//-----------------------------------------------------------
		AlignmentAnchor SpriteComponent::GetOriginAlignment() const
        {
			return meAlignment;
		}
        //-----------------------------------------------------------
        /// Get Sprite Data
        //-----------------------------------------------------------
        const SpriteComponent::SpriteData& SpriteComponent::GetSpriteData()
        {
            CalculateSpriteData();
            
            return mSpriteData;
        }
        //-----------------------------------------------------------
        /// Calculate Sprite Data
        //-----------------------------------------------------------
        void SpriteComponent::CalculateSpriteData()
        {
            //Update the vertex positions
            if(!mbCornerPosCacheValid) 
            {
                //We have been transformed so we need to recalculate our vertices
                CalculateCornerPositions();
                
                mSpriteData.sVerts[(u32)Verts::k_topLeft].vPos = mavVertexPos[(u32)Verts::k_topLeft];
                mSpriteData.sVerts[(u32)Verts::k_bottomLeft].vPos = mavVertexPos[(u32)Verts::k_bottomLeft];
                mSpriteData.sVerts[(u32)Verts::k_topRight].vPos = mavVertexPos[(u32)Verts::k_topRight];
                mSpriteData.sVerts[(u32)Verts::k_bottomRight].vPos = mavVertexPos[(u32)Verts::k_bottomRight];
            }
            
            //Update our vertex colours
            mSpriteData.pMaterial = mpMaterial;
            
            mSpriteData.sVerts[(u32)Verts::k_topLeft].Col = mByteColourWithOpacity;
            mSpriteData.sVerts[(u32)Verts::k_bottomLeft].Col = mByteColourWithOpacity;
            mSpriteData.sVerts[(u32)Verts::k_topRight].Col = mByteColourWithOpacity;
            mSpriteData.sVerts[(u32)Verts::k_bottomRight].Col = mByteColourWithOpacity;
            
            //Update our texture co-ordinates
            if(!mbUVCacheValid)
            {
                mbUVCacheValid = true;
                
                Core::Rectangle TexCoords = GetCurrentFrame();
                
                mSpriteData.sVerts[(u32)Verts::k_topLeft].vTex = TexCoords.TopLeft();
                mSpriteData.sVerts[(u32)Verts::k_bottomLeft].vTex = TexCoords.BottomLeft();
                mSpriteData.sVerts[(u32)Verts::k_topRight].vTex = TexCoords.TopRight();
                mSpriteData.sVerts[(u32)Verts::k_bottomRight].vTex = TexCoords.BottomRight();
            }
        }
        //-----------------------------------------------------------
        /// Render
        //-----------------------------------------------------------
        void SpriteComponent::Render(RenderSystem* inpRenderSystem, CameraComponent* inpCam, ShaderPass ineShaderPass)
        {
            CalculateSpriteData();
            
            //Add us to the render systems dynamic batch
            //If we force a batch flush here then the previous sprites
            //will be rendered.
            inpRenderSystem->GetDynamicSpriteBatchPtr()->Render(mSpriteData);
        }
        //------------------------------------------------------------
        /// On Transform Changed
        //------------------------------------------------------------
        void SpriteComponent::OnTransformChanged()
        {
            mbCornerPosCacheValid = false;
            mbBoundingSphereValid = false;
            mbAABBValid = false;
            mbOOBBValid = false;
        }
        //-----------------------------------------------------------
        /// Get Upper Left Corner Position
        //-----------------------------------------------------------
		const ChilliSource::Core::Vector4 & SpriteComponent::GetUpperLeftCornerPos()
        {
			if (!mbCornerPosCacheValid)
            {
				CalculateCornerPositions();
			}
			
			return mavVertexPos[(u32)Verts::k_topLeft];
		}
        //-----------------------------------------------------------
        /// Get Lower Left Corner Position
        //-----------------------------------------------------------
		const ChilliSource::Core::Vector4 & SpriteComponent::GetLowerLeftCornerPos()
        {
			if (!mbCornerPosCacheValid) 
            {
				CalculateCornerPositions();
			}
			
			return mavVertexPos[(u32)Verts::k_bottomLeft];
		}
        //-----------------------------------------------------------
        /// Get Upper Right Corner Position
        //-----------------------------------------------------------
		const ChilliSource::Core::Vector4 & SpriteComponent::GetUpperRightCornerPos()
        {
			if (!mbCornerPosCacheValid) 
            {
				CalculateCornerPositions();
			}
            
			return mavVertexPos[(u32)Verts::k_topRight];
		}
        //-----------------------------------------------------------
        /// Get Lower Right Corner Position
        //-----------------------------------------------------------
		const ChilliSource::Core::Vector4 & SpriteComponent::GetLowerRightCornerPos()
        {
			if (!mbCornerPosCacheValid) 
            {
				CalculateCornerPositions();
			}
            
			return mavVertexPos[(u32)Verts::k_bottomRight];
		}
		//----------------------------------------------------
		//----------------------------------------------------
		void SpriteComponent::OnAddedToScene()
		{
			m_transformChangedConnection = GetEntity()->GetTransform().GetTransformChangedEvent().OpenConnection(Core::MakeDelegate(this, &SpriteComponent::OnTransformChanged));
            
            OnTransformChanged();
		}
		//----------------------------------------------------
		//----------------------------------------------------
		void SpriteComponent::OnRemovedFromScene()
		{
            m_transformChangedConnection = nullptr;
		}
        //-----------------------------------------------------------
        /// Calculate Corner Positions
        //-----------------------------------------------------------
		void SpriteComponent::CalculateCornerPositions()
        {
            mmatTransformCache = GetEntity()->GetTransform().GetWorldTransform();
            
			Core::Vector2 vHalfSize(mvDimensions.x * 0.5f, mvDimensions.y * 0.5f);
			Core::Vector2 vAlignedPos;
            Align(meAlignment, vHalfSize, vAlignedPos);
            
            Core::Vector4 vCentrePos(vAlignedPos.x, vAlignedPos.y, 0, 0);
            Core::Vector4 vTemp(-vHalfSize.x, vHalfSize.y, 0, 1.0f);
			vTemp += vCentrePos;
			mavVertexPos[(u32)Verts::k_topLeft] = vTemp * mmatTransformCache;
            
            vTemp.x = vHalfSize.x;
            vTemp.y = vHalfSize.y;

			vTemp += vCentrePos;
			mavVertexPos[(u32)Verts::k_topRight] = vTemp * mmatTransformCache;
            
            vTemp.x = -vHalfSize.x;
            vTemp.y = -vHalfSize.y;

			vTemp += vCentrePos;
			mavVertexPos[(u32)Verts::k_bottomLeft] = vTemp * mmatTransformCache;
            
            vTemp.x = vHalfSize.x;
            vTemp.y = -vHalfSize.y;

			vTemp += vCentrePos;
			mavVertexPos[(u32)Verts::k_bottomRight] = vTemp * mmatTransformCache;
            
			mbCornerPosCacheValid = true;
            
            Core::Colour pCurrentColour = GetColour();
            
            f32 fOpacity = GetEntity()->GetTransform().GetWorldOpacity();
            
            SetColourWithOpacity(Core::Colour(pCurrentColour.r * fOpacity,
                                               pCurrentColour.g * fOpacity,
                                               pCurrentColour.b * fOpacity,
                                               pCurrentColour.a * fOpacity));
            
		}
        //-----------------------------------------------------------
        /// Set Colour With Opacity
        //-----------------------------------------------------------
        void SpriteComponent::SetColourWithOpacity(const Core::Colour &inCol)
        {
            mColourWithOpacity = inCol;
            mByteColourWithOpacity = Core::ColourUtils::ColourToByteColour(mColourWithOpacity);
        }
        //-----------------------------------------------------------
        /// Set Colour With Opacity
        //-----------------------------------------------------------
        void SpriteComponent::SetColourWithOpacity(const f32 infR, const f32 infG, const f32 infB, const f32 infA)
        {
			mColourWithOpacity = Core::Colour(infR, infG, infB, infA);
            mByteColourWithOpacity = Core::ColourUtils::ColourToByteColour(mColourWithOpacity);
        }
        //-----------------------------------------------------------
        /// Get Colour With Opacity
        //-----------------------------------------------------------
        const Core::Colour& SpriteComponent::GetColourWithOpacity() const
        {
            return mColourWithOpacity;
        }
	}
}
