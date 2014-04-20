/*
*  CameraComponent.cpp
*  moFlo
*
*  Created by Scott Downie on 12/10/2010.
*  Copyright 2010 Tag Games. All rights reserved.
*
*/

#include <ChilliSource/Rendering/Camera/CameraComponent.h>
#include <ChilliSource/Core/Base/Screen.h>
#include <ChilliSource/Core/Math/MathUtils.h>
#include <ChilliSource/Core/Entity/Entity.h>
#include <ChilliSource/Core/Base/ApplicationEvents.h>
#include <ChilliSource/Core/Base/MakeDelegate.h>
#include <ChilliSource/Core/Base/Screen.h>

namespace ChilliSource
{
	namespace Rendering
	{
		CS_DEFINE_NAMEDTYPE(CameraComponent);
		//------------------------------------------------------
		/// Constructor
		///
		/// @param Camera description setting the FOV etc
		//------------------------------------------------------
		CameraComponent::CameraComponent(const CameraDescription &inCamDesc) 
			: mDesc(inCamDesc), mbProjectionCacheValid(false)
		{
			EnableViewportRotationWithScreen(mDesc.bShouldRotateToScreen);
			EnableViewportResizeWithScreen(mDesc.bShouldResizeToScreen);
			mDesc.IsOrthographic ? CalculateOrthographicMatrix() : CalculatePerspectiveMatrix();
		}
		//----------------------------------------------------------
		/// Is A
		///
		/// Returns if it is of the type given
		/// @param Comparison Type
		/// @return Whether the class matches the comparison type
		//----------------------------------------------------------
		bool CameraComponent::IsA(ChilliSource::Core::InterfaceIDType inInterfaceID) const
		{
			return (inInterfaceID == CameraComponent::InterfaceID);
		}
		//----------------------------------------------------------
		/// Set Look At
		///
		/// Set the camera orientation, target and position
		/// @param Position
		/// @param Look target
		/// @param Up direction
		//----------------------------------------------------------
		void CameraComponent::SetLookAt(const Core::Vector3Old& invPos, const Core::Vector3Old& invTarget, const Core::Vector3Old& invUp)
		{
            Core::Entity * pParent(GetEntity());

            CS_ASSERT((pParent!=nullptr), "CameraComponent has no parent entity");
            
            pParent->GetTransform().SetLookAt(invPos, invTarget, invUp);
		}
		//------------------------------------------------------
		/// Unproject
		///
		/// Project from a point in screen space to a ray in
		/// world space
		/// @param Point in screen space with orientation
		/// @return Ray in world space with camera view direction
		//------------------------------------------------------
		Core::Ray CameraComponent::Unproject(const Core::Vector2Old &invScreenPos)
		{
            Core::Matrix4x4Old matProj = (GetView() * GetProjection()).Inverse();
            
            Core::Vector2Old vScreenSize(Core::Screen::GetOrientedDimensions());
			//Normalise the screen space co-ordinates into clip space
			f32 nx = ((2.0f * (invScreenPos.x/vScreenSize.x)) - 1.0f);
			f32 ny = ((2.0f * (invScreenPos.y/vScreenSize.y)) - 1.0f);
			
			Core::Vector4Old vNear(nx, ny, -1.0f, 1.0f);
            Core::Vector4Old vFar(nx,ny, 1.0f, 1.0f);
            vNear = vNear * matProj;
            vFar = vFar * matProj;
            
            Core::Ray cRay;
            
            cRay.vOrigin = vNear/vNear.w;
            cRay.vDirection = vFar/vFar.w - cRay.vOrigin;
            
            cRay.fLength = cRay.vDirection.Length();
            cRay.vDirection /= cRay.fLength;
            
            return cRay;
		}
		//------------------------------------------------------
		/// Project
		///
		/// Convert from a point in world space to a point in
		/// screen space
		//------------------------------------------------------
		Core::Vector2Old CameraComponent::Project(const Core::Vector3Old &invWorldPos)
		{
			//Convert the world space position to clip space
			Core::Matrix4x4Old matToClip = (GetView() * GetProjection());
			Core::Vector4Old vScreenPos = Core::Vector4Old(invWorldPos, 1.0f) * matToClip;
			
            Core::Vector2Old vScreenSize(Core::Screen::GetOrientedDimensions());
			
			// Normalize co-ordinates
			vScreenPos.x = vScreenPos.x / vScreenPos.w;
			vScreenPos.y = vScreenPos.y / vScreenPos.w;

			//Convert from clip space to screen space
			vScreenPos.x = (vScreenSize.x * 0.5f)* vScreenPos.x + vScreenSize.x * 0.5f;
			vScreenPos.y = (vScreenSize.y * 0.5f)* vScreenPos.y + vScreenSize.y * 0.5f;

			//Return 2D screen space co-ordinates
			return (Core::Vector2Old)vScreenPos;
		}
		//----------------------------------------------------------
		/// Use Orthographic View
		///
		/// Switch between ortho and perspective
		/// @param On or off
		//----------------------------------------------------------
		void CameraComponent::UseOrthographicView(bool inbOrthoEnabled)
		{
			mDesc.IsOrthographic = inbOrthoEnabled;

			mbProjectionCacheValid = false;
		}
        //----------------------------------------------------------
        /// Is Orthographic View
        ///
        /// @return On or off
        //----------------------------------------------------------
        bool CameraComponent::IsOrthographicView() const
        {
            return mDesc.IsOrthographic;
        }
		//----------------------------------------------------------
		/// Set Viewport Size
		///
		/// @param Vector containing width and height
		//----------------------------------------------------------
		void CameraComponent::SetViewportSize(const Core::Vector2Old &invSize)
		{
			mDesc.vViewSize = invSize;
			mbProjectionCacheValid = false;
		}
		//----------------------------------------------------------
		/// Set Viewport Size
		///
		/// @param Width 
		/// @param Height
		//----------------------------------------------------------
		void CameraComponent::SetViewportSize(u32 inudwWidth, u32 inudwHeight)
		{
			mDesc.vViewSize.x = (f32)inudwWidth;
			mDesc.vViewSize.y = (f32)inudwHeight;
			mbProjectionCacheValid = false;
		}
		//----------------------------------------------------------
		/// Get Viewport Size
		///
		/// @param Vector containing width and height
		//----------------------------------------------------------
		const Core::Vector2Old& CameraComponent::GetViewportSize()
		{ 
			return mDesc.vViewSize; 
		}	
		//------------------------------------------------------
		/// Calculate Perspective Matrix (Normalized)
		///
		/// 
		//------------------------------------------------------
		void CameraComponent::CalculatePerspectiveMatrix()
		{
			f32 Top = mDesc.fNearClipping * (f32)tanf(mDesc.fFOV * Core::MathUtils::kPI / 360.0f);
			f32 Bottom = -Top;
			f32 Left = Bottom * mDesc.fAspect;
			f32 Right = Top * mDesc.fAspect;	
			f32 Depth = mDesc.fFarClipping - mDesc.fNearClipping;

			f32 A = (Right + Left)/(Right - Left);
			f32 B = (Top + Bottom)/(Top - Bottom);
			f32 C = (mDesc.fFarClipping + mDesc.fNearClipping)/(Depth);
			f32 D = (2.0f * mDesc.fFarClipping * mDesc.fNearClipping)/(Depth);
			f32 F = (2.0f * mDesc.fNearClipping)/(Top - Bottom);
			f32 G = (2.0f * mDesc.fNearClipping)/(Right - Left);

			mmatProj = Core::Matrix4x4Old
				(
				G, 0, 0, 0, 
				0, F, 0, 0, 
				A, B, -C, -1, 
				0, 0, -D, 0
				);

			mbProjectionCacheValid = true;
		}
		//------------------------------------------------------
		/// Calculate Orthographic Matrix
		///
		/// 
		//------------------------------------------------------
		void CameraComponent::CalculateOrthographicMatrix()
		{
			mmatOrthoProj = Core::Matrix4x4Old::CreateOrthoMatrix(mDesc.vViewSize.x, mDesc.vViewSize.y, mDesc.fNearClipping, mDesc.fFarClipping);
			mbProjectionCacheValid = true;
		}
		//------------------------------------------------------
		/// Set Viewport Orientation
		///
		/// Rotate the view matrix of this camera to match the 
		/// screen orientation
		/// @param Screen orientation flag
		//------------------------------------------------------
		void CameraComponent::SetViewportOrientation(Core::ScreenOrientation ineOrientation)
		{
			//Save the camera's new orientation
			mViewOrientation = ineOrientation;

			//Invalidate our view projection
			mbProjectionCacheValid = false;
		}
		//------------------------------------------------------
		/// Get Orthographic Projection 
		///
		/// @return Orthographic projection matrix
		//------------------------------------------------------
		const Core::Matrix4x4Old& CameraComponent::GetOrthoProjection() const
		{
			return mmatOrthoProj;
		}
		//------------------------------------------------------
		/// Get Projection 
		///
		/// If the transform has changed we recalculate the
		/// projection matrix 
		/// 
		/// @return Projection matrix
		//------------------------------------------------------
		const Core::Matrix4x4Old& CameraComponent::GetProjection() 
		{
			if(!mbProjectionCacheValid)
			{
				//Update our projection matrix
				//If we are using a perspective matrix we will also
				//need the orthographic matrix for screen space overlays
				CalculateOrthographicMatrix();
				if(mDesc.IsOrthographic) 
				{
					mmatProj = mmatOrthoProj;
				} 
				else 
				{
					CalculatePerspectiveMatrix();
				}
			}

			return mmatProj;
		}
		//------------------------------------------------------
		/// Get View 
		///
		/// @return View matrix
		//------------------------------------------------------
		const Core::Matrix4x4Old& CameraComponent::GetView()
		{
			if(GetEntity())
			{
				mmatView = GetEntity()->GetTransform().GetWorldTransform().Inverse();
			}

			return mmatView;
		}
		//------------------------------------------------------
		/// Get Frustum Pointer
		///
		/// @return Pointer to camera frustum
		//------------------------------------------------------
		const Core::Frustum* CameraComponent::GetFrustumPtr() const
		{
			return &mFrustum;
		}
		//------------------------------------------------------
		/// Update Frustum
		///
		/// Recalculate frustum planes
		//------------------------------------------------------
		void CameraComponent::UpdateFrustum()
		{
			Core::Matrix4x4Old::Multiply(&GetView(), &GetProjection(), &mmatViewProj);

			//Re-calculate the clip planes
			mFrustum.CalculateClippingPlanes(mmatViewProj);
		}
		//------------------------------------------------------
		/// Billboard
		///
		/// Orientate the given matrix to face the cameras
		/// view vector
		//------------------------------------------------------
		void CameraComponent::Billboard(const Core::Matrix4x4Old& inmatBillboarded, Core::Matrix4x4Old& outmatBillboarded)
		{
            const Core::Matrix4x4Old matView = GetView();
            Core::Matrix4x4Old::Multiply(&inmatBillboarded, &matView, &outmatBillboarded);
            
			outmatBillboarded.m[12] = inmatBillboarded.m[12];
			outmatBillboarded.m[13] = inmatBillboarded.m[13];
			outmatBillboarded.m[14] = inmatBillboarded.m[14];
		}
		//------------------------------------------------------
		/// Set Field Of View
		///
		/// @param Viewing angle in degrees
		//------------------------------------------------------
		void CameraComponent::SetFieldOfView(const f32 infFOVDegress)
		{
			mDesc.fFOV = infFOVDegress;

			mbProjectionCacheValid = false;
		}
		//------------------------------------------------------
		/// Set Aspect Ratio
		///
		/// @param Aspect Ratio (Viewport width/viewport height)
		//------------------------------------------------------
		void CameraComponent::SetAspectRatio(const f32 infAspectRatio)
		{
			mDesc.fAspect = infAspectRatio;

			mbProjectionCacheValid = false;
		}
		//------------------------------------------------------
		/// Set Near Clipping
		///
		/// @param Near Z clipping distance in view space
		//------------------------------------------------------
		void CameraComponent::SetNearClipping(const f32 infNear)
		{
			mDesc.fNearClipping = infNear;

			mbProjectionCacheValid = false;
		}
		//------------------------------------------------------
		/// Set Far Clipping
		///
		/// @param Far Z clipping distance in view space
		//------------------------------------------------------
		void CameraComponent::SetFarClipping(const f32 infFar)
		{
			mDesc.fFarClipping = infFar;

			mbProjectionCacheValid = false;
		}
		//------------------------------------------------------
		/// Get Field Of View
		///
		/// @return Viewing angle in degrees
		//------------------------------------------------------
		const f32 CameraComponent::GetFieldOfView() const
		{
			return mDesc.fFOV;
		}
		//------------------------------------------------------
		/// Get Aspect Ratio
		///
		/// @return Aspect Ratio (Viewport width/viewport height)
		//------------------------------------------------------
		const f32 CameraComponent::GetAspectRatio() const
		{
			return mDesc.fAspect;
		}
		//------------------------------------------------------
		/// Get Near Clipping
		///
		/// @return Near Z clipping distance in view space
		//------------------------------------------------------
		const f32 CameraComponent::GetNearClipping() const
		{
			return mDesc.fNearClipping;
		}
		//------------------------------------------------------
		/// Get Far Clipping
		///
		/// @return Far Z clipping distance in view space
		//------------------------------------------------------
		const f32 CameraComponent::GetFarClipping() const
		{
			return mDesc.fFarClipping;
		}
		//------------------------------------------------------
		/// Set Clear Colour
		///
		/// @param Render buffer clear colour
		//------------------------------------------------------
		void CameraComponent::SetClearColour(const Core::Colour &inCol)
		{
			mDesc.ClearCol = inCol;
		}
		//------------------------------------------------------
		/// Get Clear Colour
		///
		/// @return Render buffer clear colour
		//------------------------------------------------------
		const Core::Colour& CameraComponent::GetClearColour() const
		{
			return mDesc.ClearCol;
		}
        //--------------------------------------------------------------------------------------------------
        /// Get Opaque Sort Predicate
        ///
        /// @return Gets the currently set opaque sort predicate for this scene
        //--------------------------------------------------------------------------------------------------
        RendererSortPredicateSPtr CameraComponent::GetOpaqueSortPredicate() const
        {
            return mpOpaqueSortPredicate;
        }
        //--------------------------------------------------------------------------------------------------
        /// Get Transparent Sort Predicate
        ///
        /// @return Gets the currently set opaque sort predicate for this scene
        //--------------------------------------------------------------------------------------------------
        RendererSortPredicateSPtr CameraComponent::GetTransparentSortPredicate() const
        {
            return mpTransparentSortPredicate;
        }
        //--------------------------------------------------------------------------------------------------
        /// Set Opaque Sort Predicate
        ///
        /// @return Opaque sort predicate to use for this scene
        //--------------------------------------------------------------------------------------------------
        void CameraComponent::SetOpaqueSortPredicate(const RendererSortPredicateSPtr & inpPredicate)
        {
            mpOpaqueSortPredicate = inpPredicate;
        }
        //--------------------------------------------------------------------------------------------------
        /// Set Transparent Sort Predicate
        ///
        /// @param Transparent sort predicate to use for this scene
        //--------------------------------------------------------------------------------------------------
        void CameraComponent::SetTransparentSortPredicate(const RendererSortPredicateSPtr & inpPredicate)
        {
            mpTransparentSortPredicate = inpPredicate;
        }
        //--------------------------------------------------------------------------------------------------
        /// Get Culling Predicate
        ///
        /// @param Culling predicate to use for this camera
        //--------------------------------------------------------------------------------------------------
        ICullingPredicateSPtr CameraComponent::GetCullingPredicate() const
        {
            return IsOrthographicView() ? mpOrthographicCulling : mpPerspectiveCulling;
        }
        //--------------------------------------------------------------------------------------------------
        /// Get Perspective Culling Predicate
        ///
        /// @param Culling predicate to use for this camera in perpective mode
        //--------------------------------------------------------------------------------------------------
        void CameraComponent::SetPerspectiveCullingPredicate(const ICullingPredicateSPtr & inpPredicate)
        {
            mpPerspectiveCulling = inpPredicate;
        }
        //--------------------------------------------------------------------------------------------------
        /// Get orthographic Culling Predicate
        ///
        /// @param Culling predicate to use for this camera in orthographic mode
        //--------------------------------------------------------------------------------------------------
        void CameraComponent::SetOrthographicCullingPredicate(const ICullingPredicateSPtr & inpPredicate)
        {
            mpOrthographicCulling = inpPredicate;
        }        
		//------------------------------------------------------
		/// Enable Viewport Rotation with Screen
		///
		/// @param Whether the viewport should rotate when
		/// the screen rotates
		//-----------------------------------------------------
		void CameraComponent::EnableViewportRotationWithScreen(bool inbEnable)
		{
			m_screenOrientationChangedConnection = inbEnable ? Core::ApplicationEvents::GetScreenOrientationChangedEvent().OpenConnection(Core::MakeDelegate(this, &CameraComponent::SetViewportOrientation)) : nullptr;
		}
		//------------------------------------------------------
		/// Enable Viewport Resize with Screen
		///
		/// @param Whether the viewport should resize when
		/// the screen resizes
		//-----------------------------------------------------
		void CameraComponent::EnableViewportResizeWithScreen(bool inbEnable)
		{
            m_screenResizedConnection = inbEnable ? Core::ApplicationEvents::GetScreenResizedEvent().OpenConnection(Core::MakeDelegate<CameraComponent, CameraComponent, void, u32, u32>(this, &CameraComponent::SetViewportSize))
                                                             : nullptr;
		}
		//-----------------------------------------------------
		/// Destructor
		///
		//-----------------------------------------------------
		CameraComponent::~CameraComponent()
		{
			if(mDesc.bShouldResizeToScreen)
			{
				EnableViewportResizeWithScreen(false);
			}
			if(mDesc.bShouldRotateToScreen)
			{
				EnableViewportRotationWithScreen(false);
			}
		}
	}
}

