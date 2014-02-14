/*
 *  CameraComponent.h
 *  moFlo
 *
 *  Created by Scott Downie on 12/10/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#ifndef _MO_FLO_RENDERING_CAMERA_COMPONENT_H_
#define _MO_FLO_RENDERING_CAMERA_COMPONENT_H_

#include <ChilliSource/Core/ForwardDeclarations.h>
#include <ChilliSource/Core/Entity/Component.h>
#include <ChilliSource/Core/Main/Colour.h>
#include <ChilliSource/Core/Main/Screen.h>
#include <ChilliSource/Core/Math/Shapes.h>

#include <ChilliSource/Rendering/ForwardDeclarations.h>

namespace moFlo
{
	namespace Rendering
	{
		struct CameraDescription
		{
			f32 fFOV;
			f32 fAspect;
			f32 fNearClipping;
			f32 fFarClipping;
			
			bool IsOrthographic;
			bool bShouldRotateToScreen;
			bool bShouldResizeToScreen;
			
			Core::CColour ClearCol; 
			Core::CVector2 vViewSize;
		};
		
		class CCameraComponent : public Core::IComponent
		{
		public:
			DECLARE_NAMED_INTERFACE(CCameraComponent);
			CCameraComponent(const CameraDescription &inCamDesc);
			~CCameraComponent();
			//----------------------------------------------------------
			/// Is A
			///
			/// Returns if it is of the type given
			/// @param Comparison Type
			/// @return Whether the class matches the comparison type
			//----------------------------------------------------------
			bool IsA(Core::InterfaceIDType inInterfaceID) const;
			//----------------------------------------------------------
			/// Set Look At
			///
			/// Set the camera orientation, target and position
			/// @param Position
			/// @param Look target
			/// @param Up direction
			//----------------------------------------------------------
			void SetLookAt(const Core::CVector3& invPos, const Core::CVector3& invTarget, const Core::CVector3& invUp);
			//----------------------------------------------------------
			/// Use Orthographic View
			///
			/// Switch between ortho and perspective
			/// @param On or off
			//----------------------------------------------------------
			void UseOrthographicView(bool inbOrthoEnabled);
            //----------------------------------------------------------
			/// Is Orthographic View
			///
			/// @return On or off
			//----------------------------------------------------------
			bool IsOrthographicView() const;
			//----------------------------------------------------------
			/// Set Viewport Size
			///
			/// @param Vector containing width and height
			//----------------------------------------------------------
			void SetViewportSize(const Core::CVector2 &invSize);
			//----------------------------------------------------------
			/// Set Viewport Size
			///
			/// @param Width 
			/// @param Height
			//----------------------------------------------------------
			void SetViewportSize(u32 inudwWidth, u32 inudwHeight);
			//----------------------------------------------------------
			/// Get Viewport Size
			///
			/// @param Vector containing width and height
			//----------------------------------------------------------
			const Core::CVector2& GetViewportSize();			
			//------------------------------------------------------
			/// Set Field Of View
			///
			/// @param Viewing angle in degrees
			//------------------------------------------------------
			void SetFieldOfView(const f32 infFOVDegress);
			//------------------------------------------------------
			/// Set Aspect Ratio
			///
			/// @param Aspect Ratio (Viewport width/viewport height)
			//------------------------------------------------------
			void SetAspectRatio(const f32 infAspectRatio);
			//------------------------------------------------------
			/// Set Near Clipping
			///
			/// @param Near Z clipping distance in view space
			//------------------------------------------------------
			void SetNearClipping(const f32 infNear);
			//------------------------------------------------------
			/// Set Far Clipping
			///
			/// @param Far Z clipping distance in view space
			//------------------------------------------------------
			void SetFarClipping(const f32 infFar);
			//------------------------------------------------------
			/// Set Clear Colour
			///
			/// @param Render buffer clear colour
			//------------------------------------------------------
			void SetClearColour(const Core::CColour &inCol);
			//------------------------------------------------------
			/// Enable Viewport Rotation with Screen
			///
			/// @param Whether the viewport should rotate when
			/// the screen rotates
			//-----------------------------------------------------
			void EnableViewportRotationWithScreen(bool inbEnable);
			//------------------------------------------------------
			/// Enable Viewport Resize with Screen
			///
			/// @param Whether the viewport should resize when
			/// the screen resizes
			//-----------------------------------------------------
			void EnableViewportResizeWithScreen(bool inbEnable);
			//------------------------------------------------------
			/// Get Field Of View
			///
			/// @return Viewing angle in degrees
			//------------------------------------------------------
			const f32 GetFieldOfView() const;
			//------------------------------------------------------
			/// Get Aspect Ratio
			///
			/// @return Aspect Ratio (Viewport width/viewport height)
			//------------------------------------------------------
			const f32 GetAspectRatio() const;
			//------------------------------------------------------
			/// Get Near Clipping
			///
			/// @return Near Z clipping distance in view space
			//------------------------------------------------------
			const f32 GetNearClipping() const;
			//------------------------------------------------------
			/// Get Far Clipping
			///
			/// @return Far Z clipping distance in view space
			//------------------------------------------------------
			const f32 GetFarClipping() const;
			//------------------------------------------------------
			/// Get Clear Colour
			///
			/// @return Render buffer clear colour
			//------------------------------------------------------
			const Core::CColour& GetClearColour() const;
			
			//------------------------------------------------------
			/// Get Projection 
			///
			/// @return Projection matrix
			//------------------------------------------------------
			const Core::CMatrix4x4& GetProjection();
			//------------------------------------------------------
			/// Get View 
			///
			/// @return View matrix
			//------------------------------------------------------
			const Core::CMatrix4x4& GetView();
			//------------------------------------------------------
			/// Set Viewport Orientation
			///
			/// Rotate the view matrix of this camera to match the 
			/// screen orientation
			/// @param Screen orientation flag
			//------------------------------------------------------
			void SetViewportOrientation(Core::ScreenOrientation ineOrientation);
			//------------------------------------------------------
			/// Get Orthographic Projection 
			///
			/// @return Orthographic projection matrix
			//------------------------------------------------------
			const Core::CMatrix4x4& GetOrthoProjection() const;
			//------------------------------------------------------
			/// Unproject
			///
			/// Project from a point in screen space to a ray in
			/// world space
			/// @param Point in screen space
			/// @return Ray in world space with camera view direction
			//------------------------------------------------------
			virtual Core::Ray Unproject(const Core::CVector2 &invScreenPos);
			//------------------------------------------------------
			/// Project
			///
			/// Convert from a point in world space to a point in
			/// screen space
			//------------------------------------------------------
			virtual Core::CVector2 Project(const Core::CVector3 &invWorldPos);
			
			//------------------------------------------------------
			/// Get Frustum Pointer
			///
			/// @return Pointer to camera frustum
			//------------------------------------------------------
			const Core::CFrustum* GetFrustumPtr() const;
            //------------------------------------------------------
            /// Update Frustum
            ///
            /// Recalculate frustum planes
            //------------------------------------------------------
            void UpdateFrustum();
			//------------------------------------------------------
            /// Billboard
            ///
            /// Orientate the given matrix to face the cameras
            /// view vector
            //------------------------------------------------------
            void Billboard(const Core::CMatrix4x4& inmatBillboarded, Core::CMatrix4x4& outmatBillboarded);

			CameraDescription& GetDescription() { return mDesc; }
            
            //--------------------------------------------------------------------------------------------------
            /// Get Opaque Sort Predicate
            ///
            /// @return Gets the currently set opaque sort predicate for this camera
            //--------------------------------------------------------------------------------------------------
            RendererSortPredicatePtr GetOpaqueSortPredicate() const;
            //--------------------------------------------------------------------------------------------------
            /// Get Transparent Sort Predicate
            ///
            /// @return Gets the currently set opaque sort predicate for this camera
            //--------------------------------------------------------------------------------------------------
            RendererSortPredicatePtr GetTransparentSortPredicate() const;
            //--------------------------------------------------------------------------------------------------
            /// Set Opaque Sort Predicate
            ///
            /// @return Opaque sort predicate to use for this camera
            //--------------------------------------------------------------------------------------------------
            void SetOpaqueSortPredicate(const RendererSortPredicatePtr & inpPredicate);
            //--------------------------------------------------------------------------------------------------
            /// Set Transparent Sort Predicate
            ///
            /// @param Transparent sort predicate to use for this camera
            //--------------------------------------------------------------------------------------------------
            void SetTransparentSortPredicate(const RendererSortPredicatePtr & inpPredicate);
			         
            //--------------------------------------------------------------------------------------------------
            /// Get Culling Predicate
            ///
            /// @param Culling predicate to use for this camera
            //--------------------------------------------------------------------------------------------------
            CullingPredicatePtr GetCullingPredicate() const;
            //--------------------------------------------------------------------------------------------------
            /// Get Perspective Culling Predicate
            ///
            /// @param Culling predicate to use for this camera in perpective mode
            //--------------------------------------------------------------------------------------------------
            void SetPerspectiveCullingPredicate(const CullingPredicatePtr & inpPredicate);
            //--------------------------------------------------------------------------------------------------
            /// Get orthographic Culling Predicate
            ///
            /// @param Culling predicate to use for this camera in orthographic mode
            //--------------------------------------------------------------------------------------------------
            void SetOrthographicCullingPredicate(const CullingPredicatePtr & inpPredicate);
            
		private:
			//------------------------------------------------------
			/// Calculate Perspective Matrix (Normalized)
			///
			/// 
			//------------------------------------------------------
			void CalculatePerspectiveMatrix();
			//------------------------------------------------------
			/// Calculate Orthographic Matrix
			///
			/// 
			//------------------------------------------------------
			void CalculateOrthographicMatrix();

		private:
			
			bool mbProjectionCacheValid;
			
			CameraDescription mDesc;
			Core::CFrustum mFrustum;
			
			Core::ScreenOrientation mViewOrientation;
			
			Core::CMatrix4x4 mmatOrthoProj; //Ortho projection matrix;
			Core::CMatrix4x4 mmatProj;		//Projection matrix depending on whether we are an ortho or perspective camera
			Core::CMatrix4x4 mmatView;		//View matrix
			Core::CMatrix4x4 mmatViewProj;	//View projection;
            
            RendererSortPredicatePtr mpOpaqueSortPredicate;
            RendererSortPredicatePtr mpTransparentSortPredicate;
            
            CullingPredicatePtr      mpOrthographicCulling;
            CullingPredicatePtr      mpPerspectiveCulling;
		};
	}
}

#endif
