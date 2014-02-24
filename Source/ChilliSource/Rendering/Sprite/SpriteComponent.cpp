/*
 *  SpriteComponent.cpp
 *  moFlo
 *
 *  Created by Scott Downie on 30/09/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Rendering/Sprite/SpriteComponent.h>
#include <ChilliSource/Rendering/Material/Material.h>
#include <ChilliSource/Rendering/Base/RenderSystem.h>
#include <ChilliSource/Rendering/Sprite/DynamicSpriteBatcher.h>
#include <ChilliSource/Rendering/Lighting/DirectionalLightComponent.h>
#include <ChilliSource/Rendering/Lighting/PointLightComponent.h>

#include <ChilliSource/Core/Entity/Entity.h>

namespace ChilliSource
{
	namespace Rendering
	{
        DEFINE_NAMED_INTERFACE(CSpriteComponent);
		//----------------------------------------------------------
		/// Constructor
		//----------------------------------------------------------
		CSpriteComponent::CSpriteComponent() : mUVs(Core::CVector2(0, 0), Core::CVector2(1, 1)), mbFlippedVertical(false), mbFlippedHorizontal(false), 
        mbCornerPosCacheValid(false), meAlignment(Core::AlignmentAnchor::k_middleCentre), mbUVCacheValid(false), mbBoundingSphereValid(false), mbAABBValid(false), mbOOBBValid(false)
		{
            mByteColourWithOpacity.r = 255;
            mByteColourWithOpacity.g = 255;
            mByteColourWithOpacity.b = 255;
            mByteColourWithOpacity.a = 255;
		}
		//----------------------------------------------------------
		/// Is A
		//----------------------------------------------------------
		bool CSpriteComponent::IsA(ChilliSource::Core::InterfaceIDType inInterfaceID) const
		{
			return  (inInterfaceID == CSpriteComponent::InterfaceID) || 
                    (inInterfaceID == IRenderComponent::InterfaceID) ||
                    (inInterfaceID == IVolumeComponent::InterfaceID);
		}
		//----------------------------------------------------
		/// Get Axis Aligned Bounding Box
		//----------------------------------------------------
		const Core::AABB& CSpriteComponent::GetAABB()
		{
			if(mpEntityOwner && !mbAABBValid)
			{
                mbAABBValid = true;
                
				//Rebuild the box
				mBoundingBox.SetOrigin(mpEntityOwner->Transform().GetWorldPosition());
				mBoundingBox.SetSize(mvDimensions);
			}
			return mBoundingBox;
		}
		//----------------------------------------------------
		/// Get Object Oriented Bounding Box
		//----------------------------------------------------
		const Core::OOBB& CSpriteComponent::GetOOBB()
		{
			if(mpEntityOwner && !mbOOBBValid)
			{
                mbOOBBValid = true;
                
				mOBBoundingBox.SetSize(mvDimensions);
				mOBBoundingBox.SetTransform(mpEntityOwner->Transform().GetWorldTransform());
			}
			return mOBBoundingBox;
		}
		//----------------------------------------------------
		/// Get Bounding Sphere
		//----------------------------------------------------
		const Core::Sphere& CSpriteComponent::GetBoundingSphere()
		{
			if(mpEntityOwner && !mbBoundingSphereValid)
			{
                mbBoundingSphereValid = true;
                
				mBoundingSphere.vOrigin = mpEntityOwner->Transform().GetWorldPosition();
				mBoundingSphere.fRadius = std::max(mvDimensions.x, mvDimensions.y) * 0.5f;
			}
			return mBoundingSphere;
		}
		//-----------------------------------------------------------
		/// Set Dimensions Unfactored
		//-----------------------------------------------------------
		void CSpriteComponent::SetDimensions(const Core::CVector2 &invDims)
		{
			SetDimensions(invDims.x, invDims.y);
		}
		//-----------------------------------------------------------
		/// Set Dimensions Unfactored
		//-----------------------------------------------------------
		void CSpriteComponent::SetDimensions(const f32 infWidth, const f32 infHeight)
		{
			mbCornerPosCacheValid = false;
			
			mvDimensions = Core::CVector2(infWidth, infHeight);
		}
		//-----------------------------------------------------------
		/// Set Width Unfactored
		//-----------------------------------------------------------
		void CSpriteComponent::SetWidth(const f32 infWidth)
		{
			mbCornerPosCacheValid = false;
			
			mvDimensions.x = infWidth;
		}
		//-----------------------------------------------------------
		/// Set Height Unfactored
		//-----------------------------------------------------------
		void CSpriteComponent::SetHeight(const f32 infHeight)
		{
			mbCornerPosCacheValid = false;
			
			mvDimensions.y = infHeight;
		}
		//-----------------------------------------------------------
		/// Get Dimensions
		//-----------------------------------------------------------
		const Core::CVector2& CSpriteComponent::GetDimensions() const
		{
			return mvDimensions;
		}
		//-----------------------------------------------------------
		/// Set UV's
		//-----------------------------------------------------------
		void CSpriteComponent::SetUVs(const Core::CRect &inUVs)
		{
			mUVs = inUVs;
            
            mbUVCacheValid = false;
		}
		//-----------------------------------------------------------
		/// Set UV's
		//-----------------------------------------------------------
		void CSpriteComponent::SetUVs(const f32 infUStart, const f32 infUWidth, const f32 infVStart, const f32 infVHeight)
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
		const Core::CRect& CSpriteComponent::GetUVs() const
		{
			return mUVs;
		}
		//-----------------------------------------------------------
		/// Set Colour
		//-----------------------------------------------------------
		void CSpriteComponent::SetColour(const Core::CColour &inCol)
		{
			mColour = inCol;
            SetColourWithOpacity(mColour);
		}
		//-----------------------------------------------------------
		/// Set Colour
		//-----------------------------------------------------------
		void CSpriteComponent::SetColour(const f32 infR, const f32 infG, const f32 infB, const f32 infA)
		{
			mColour = Core::CColour(infR, infG, infB, infA);
            SetColourWithOpacity(mColour);
		}
		//-----------------------------------------------------------
		/// Get Colour
		//-----------------------------------------------------------
		const Core::CColour& CSpriteComponent::GetColour() const
		{
			return mColour;
		}
		//-----------------------------------------------------------
		/// Get Current Frame
		//-----------------------------------------------------------
		const Core::CRect& CSpriteComponent::GetCurrentFrame()
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
		void CSpriteComponent::SetFlippedHorizontal(bool inbValue)
		{
			mbFlippedHorizontal = inbValue;
            mbUVCacheValid = false;
		}
		//-----------------------------------------------------------
		/// Get Flipped Horizontal
		//-----------------------------------------------------------
		bool CSpriteComponent::GetFlippedHorizontal() const
		{
			return mbFlippedHorizontal;
		}
		//-----------------------------------------------------------
		/// Set Flipped Vertical
		//-----------------------------------------------------------
		void CSpriteComponent::SetFlippedVertical(bool inbValue)
		{
			mbFlippedVertical = inbValue;
            mbUVCacheValid = false;
		}
		//-----------------------------------------------------------
		/// Get Flipped Vertical
		//-----------------------------------------------------------
		bool CSpriteComponent::GetFlippedVertical() const
		{
			return mbFlippedVertical;
		}
		//-----------------------------------------------------------
		/// Set Origin Alignment
		//-----------------------------------------------------------
		void CSpriteComponent::SetOriginAlignment(Core::AlignmentAnchor ineAlignment)
        {
			meAlignment = ineAlignment;
			mbCornerPosCacheValid = false;
		}
		//-----------------------------------------------------------
		/// Get Origin Alignment
		//-----------------------------------------------------------
		Core::AlignmentAnchor CSpriteComponent::GetOriginAlignment() const
        {
			return meAlignment;
		}
        //-----------------------------------------------------------
        /// Get Sprite Data
        //-----------------------------------------------------------
        const CSpriteComponent::SpriteData& CSpriteComponent::GetSpriteData()
        {
            CalculateSpriteData();
            
            return mSpriteData;
        }
        //-----------------------------------------------------------
        /// Calculate Sprite Data
        //-----------------------------------------------------------
        void CSpriteComponent::CalculateSpriteData()
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
                
                Core::CRect TexCoords = GetCurrentFrame();
                
                mSpriteData.sVerts[(u32)Verts::k_topLeft].vTex = TexCoords.TopLeft();
                mSpriteData.sVerts[(u32)Verts::k_bottomLeft].vTex = TexCoords.BottomLeft();
                mSpriteData.sVerts[(u32)Verts::k_topRight].vTex = TexCoords.TopRight();
                mSpriteData.sVerts[(u32)Verts::k_bottomRight].vTex = TexCoords.BottomRight();
            }
        }
        //-----------------------------------------------------------
        /// Render
        //-----------------------------------------------------------
        void CSpriteComponent::Render(IRenderSystem* inpRenderSystem, CCameraComponent* inpCam, ShaderPass ineShaderPass)
        {
            CalculateSpriteData();
            
            mSpriteData.pMaterial->SetActiveShaderProgram(ineShaderPass);
            if (mSpriteData.pMaterial->GetActiveShaderProgram() != NULL)
            {
                //Add us to the render systems dynamic batch
                //If we force a batch flush here then the previous sprites
                //will be rendered.
                inpRenderSystem->GetDynamicSpriteBatchPtr()->Render(inpRenderSystem, mSpriteData);
            }
        }
        //------------------------------------------------------------
        /// On Transform Changed
        //------------------------------------------------------------
        void CSpriteComponent::OnTransformChanged()
        {
            mbCornerPosCacheValid = false;
            mbBoundingSphereValid = false;
            mbAABBValid = false;
            mbOOBBValid = false;
        }
        //-----------------------------------------------------------
        /// Get Upper Left Corner Position
        //-----------------------------------------------------------
		const ChilliSource::Core::CVector4 & CSpriteComponent::GetUpperLeftCornerPos()
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
		const ChilliSource::Core::CVector4 & CSpriteComponent::GetLowerLeftCornerPos()
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
		const ChilliSource::Core::CVector4 & CSpriteComponent::GetUpperRightCornerPos()
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
		const ChilliSource::Core::CVector4 & CSpriteComponent::GetLowerRightCornerPos()
        {
			if (!mbCornerPosCacheValid) 
            {
				CalculateCornerPositions();
			}
            
			return mavVertexPos[(u32)Verts::k_bottomRight];
		}
		//----------------------------------------------------
		/// On Attached To Entity
		//----------------------------------------------------
		void CSpriteComponent::OnAttachedToEntity()
		{
			mpEntityOwner->Transform().GetTransformChangedEvent() += Core::CTransform::TransformChangedDelegate(this, &CSpriteComponent::OnTransformChanged);
            
            OnTransformChanged();
		}
		//----------------------------------------------------
		/// On Detached From Entity
		//----------------------------------------------------
		void CSpriteComponent::OnDetachedFromEntity()
		{
			mpEntityOwner->Transform().GetTransformChangedEvent() -= Core::CTransform::TransformChangedDelegate(this, &CSpriteComponent::OnTransformChanged);
		}
        //-----------------------------------------------------------
        /// Calculate Corner Positions
        //-----------------------------------------------------------
		void CSpriteComponent::CalculateCornerPositions()
        {
            mmatTransformCache = mpEntityOwner->Transform().GetWorldTransform();
            
			Core::CVector2 vHalfSize(mvDimensions.x * 0.5f, mvDimensions.y * 0.5f);
			Core::CVector2 vAlignedPos;
            Core::Align(meAlignment, vHalfSize, vAlignedPos);
            
            Core::CVector4 vCentrePos(vAlignedPos.x, vAlignedPos.y, 0, 0);
            Core::CVector4 vTemp(-vHalfSize.x, vHalfSize.y, 0, 1.0f);
			vTemp += vCentrePos;
            Core::CMatrix4x4::Multiply(&vTemp, &mmatTransformCache, &mavVertexPos[(u32)Verts::k_topLeft]);
            
            vTemp.x = vHalfSize.x;
            vTemp.y = vHalfSize.y;

			vTemp += vCentrePos;
            Core::CMatrix4x4::Multiply(&vTemp, &mmatTransformCache, &mavVertexPos[(u32)Verts::k_topRight]);
            
            vTemp.x = -vHalfSize.x;
            vTemp.y = -vHalfSize.y;

			vTemp += vCentrePos;
            Core::CMatrix4x4::Multiply(&vTemp, &mmatTransformCache, &mavVertexPos[(u32)Verts::k_bottomLeft]);
            
            vTemp.x = vHalfSize.x;
            vTemp.y = -vHalfSize.y;

			vTemp += vCentrePos;
            Core::CMatrix4x4::Multiply(&vTemp, &mmatTransformCache, &mavVertexPos[(u32)Verts::k_bottomRight]);
            
			mbCornerPosCacheValid = true;
            
            Core::CColour pCurrentColour = GetColour();
            
            f32 fOpacity = mpEntityOwner->Transform().GetWorldOpacity();
            
            SetColourWithOpacity(Core::CColour(pCurrentColour.r * fOpacity,
                                               pCurrentColour.g * fOpacity,
                                               pCurrentColour.b * fOpacity,
                                               pCurrentColour.a * fOpacity));
            
		}
        //-----------------------------------------------------------
        /// Set Colour With Opacity
        //-----------------------------------------------------------
        void CSpriteComponent::SetColourWithOpacity(const Core::CColour &inCol)
        {
            mColourWithOpacity = inCol;
            mByteColourWithOpacity = Core::CColour::ColourToByteColour(mColourWithOpacity);
        }
        //-----------------------------------------------------------
        /// Set Colour With Opacity
        //-----------------------------------------------------------
        void CSpriteComponent::SetColourWithOpacity(const f32 infR, const f32 infG, const f32 infB, const f32 infA)
        {
			mColourWithOpacity = Core::CColour(infR, infG, infB, infA);
            mByteColourWithOpacity = Core::CColour::ColourToByteColour(mColourWithOpacity);
        }
        //-----------------------------------------------------------
        /// Get Colour With Opacity
        //-----------------------------------------------------------
        const Core::CColour& CSpriteComponent::GetColourWithOpacity() const
        {
            return mColourWithOpacity;
        }
        CSpriteComponent::~CSpriteComponent()
		{
            if(mpEntityOwner)
            {
                mpEntityOwner->Transform().GetTransformChangedEvent() -= Core::CTransform::TransformChangedDelegate(this, &CSpriteComponent::OnTransformChanged);
            }
		}
	}
}
