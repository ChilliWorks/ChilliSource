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
#include <ChilliSource/Core/Base/Application.h>
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
            m_screen = Core::Application::Get()->GetSystem<Core::Screen>();
            
			EnableViewportResizeWithScreen(mDesc.bShouldResizeToScreen);
            
            switch(mDesc.m_type)
            {
                case CameraType::k_orthographic:
                    CalculateOrthographicMatrix();
                    break;
                case CameraType::k_perspective:
                    CalculatePerspectiveMatrix();
                    break;
            }
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
		void CameraComponent::SetLookAt(const Core::Vector3& invPos, const Core::Vector3& invTarget, const Core::Vector3& invUp)
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
		Core::Ray CameraComponent::Unproject(const Core::Vector2 &invScreenPos)
		{
            Core::Matrix4 matProj = Core::Matrix4::Inverse((GetView() * GetProjection()));
            
            Core::Vector2 vScreenSize = m_screen->GetResolution();
			//Normalise the screen space co-ordinates into clip space
			f32 nx = ((2.0f * (invScreenPos.x/vScreenSize.x)) - 1.0f);
			f32 ny = ((2.0f * (invScreenPos.y/vScreenSize.y)) - 1.0f);
			
			Core::Vector4 vNear(nx, ny, -1.0f, 1.0f);
            Core::Vector4 vFar(nx,ny, 1.0f, 1.0f);
            vNear = vNear * matProj;
            vFar = vFar * matProj;
            
            Core::Ray cRay;
            
			vNear /= vNear.w;
			cRay.vOrigin = Core::Vector3(vNear.x, vNear.y, vNear.z);

			vFar /= vFar.w;
			cRay.vDirection = Core::Vector3(vFar.x, vFar.y, vFar.z) - cRay.vOrigin;
            
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
		Core::Vector2 CameraComponent::Project(const Core::Vector3 &invWorldPos)
		{
			//Convert the world space position to clip space
			Core::Matrix4 matToClip = (GetView() * GetProjection());
			Core::Vector4 vScreenPos = Core::Vector4(invWorldPos, 1.0f) * matToClip;
			
            Core::Vector2 vScreenSize = m_screen->GetResolution();
			
			// Normalize co-ordinates
			vScreenPos.x = vScreenPos.x / vScreenPos.w;
			vScreenPos.y = vScreenPos.y / vScreenPos.w;

			//Convert from clip space to screen space
			vScreenPos.x = (vScreenSize.x * 0.5f)* vScreenPos.x + vScreenSize.x * 0.5f;
			vScreenPos.y = (vScreenSize.y * 0.5f)* vScreenPos.y + vScreenSize.y * 0.5f;

			//Return 2D screen space co-ordinates
			return Core::Vector2(vScreenPos.x, vScreenPos.y);
		}
        //----------------------------------------------------------
        //----------------------------------------------------------
        void CameraComponent::SetType(CameraType in_type)
        {
            mDesc.m_type = in_type;
            
            mbProjectionCacheValid = false;
        }
        //----------------------------------------------------------
        //----------------------------------------------------------
        CameraType CameraComponent::GetType() const
        {
            return mDesc.m_type;
        }
		//----------------------------------------------------------
		/// Set Viewport Size
		///
		/// @param Vector containing width and height
		//----------------------------------------------------------
		void CameraComponent::SetViewportSize(const Core::Vector2 &invSize)
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
		const Core::Vector2& CameraComponent::GetViewportSize()
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
			f32 Top = mDesc.fNearClipping * (f32)tanf(mDesc.fFOV * 0.5f * Core::MathUtils::kPI / 360.0f);
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

			mmatProj = Core::Matrix4
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
			mmatOrthoProj = Core::Matrix4::CreateOrthographicProjectionRH(mDesc.vViewSize.x, mDesc.vViewSize.y, mDesc.fNearClipping, mDesc.fFarClipping);
			mbProjectionCacheValid = true;
		}
		//------------------------------------------------------
		/// Get Orthographic Projection 
		///
		/// @return Orthographic projection matrix
		//------------------------------------------------------
		const Core::Matrix4& CameraComponent::GetOrthoProjection() const
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
		const Core::Matrix4& CameraComponent::GetProjection() 
		{
			if(!mbProjectionCacheValid)
			{
				//Update our projection matrix
				//If we are using a perspective matrix we will also
				//need the orthographic matrix for screen space overlays
				CalculateOrthographicMatrix();
                
                switch(mDesc.m_type)
                {
                    case CameraType::k_orthographic:
                        mmatProj = mmatOrthoProj;
                        break;
                    case CameraType::k_perspective:
                        CalculatePerspectiveMatrix();
                        break;
                }
			}

			return mmatProj;
		}
		//------------------------------------------------------
		/// Get View 
		///
		/// @return View matrix
		//------------------------------------------------------
		const Core::Matrix4& CameraComponent::GetView()
		{
			if(GetEntity())
			{
				mmatView = Core::Matrix4::Inverse(GetEntity()->GetTransform().GetWorldTransform());
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
			mmatViewProj = GetView() * GetProjection();

			//Re-calculate the clip planes
			mFrustum.CalculateClippingPlanes(mmatViewProj);
		}
		//------------------------------------------------------
		/// Billboard
		///
		/// Orientate the given matrix to face the cameras
		/// view vector
		//------------------------------------------------------
		void CameraComponent::Billboard(const Core::Matrix4& inmatBillboarded, Core::Matrix4& outmatBillboarded)
		{
            const Core::Matrix4 matView = GetView();
			outmatBillboarded = inmatBillboarded * matView;
            
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
            switch(mDesc.m_type)
            {
                case CameraType::k_orthographic:
                    return mpOrthographicCulling;
                case CameraType::k_perspective:
                    return mpPerspectiveCulling;
            }
            
            return nullptr;
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
		/// Enable Viewport Resize with Screen
		///
		/// @param Whether the viewport should resize when
		/// the screen resizes
		//-----------------------------------------------------
		void CameraComponent::EnableViewportResizeWithScreen(bool inbEnable)
		{
            if (inbEnable == true && m_screenResizedConnection == nullptr)
            {
                m_screenResizedConnection = m_screen->GetResolutionChangedEvent().OpenConnection(Core::MakeDelegate<CameraComponent, CameraComponent, void, const Core::Vector2&>(this, &CameraComponent::OnResolutionChanged));
            }
            else if (inbEnable == false && m_screenResizedConnection != nullptr)
            {
                m_screenResizedConnection = nullptr;
            }
		}
        //------------------------------------------------------
        //------------------------------------------------------
        void CameraComponent::OnResolutionChanged(const Core::Vector2& in_resolution)
        {
            SetViewportSize(in_resolution);
            SetAspectRatio(in_resolution.x / in_resolution.y);
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
		}
	}
}

