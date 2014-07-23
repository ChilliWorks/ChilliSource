//
//  CameraComponent.h
//  Chilli Source
//  Created by Scott Downie on 12/10/2010.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2010 Tag Games Limited
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

#ifndef _CHILLISOURCE_RENDERING_CAMERA_CAMERACOMPONENT_H_
#define _CHILLISOURCE_RENDERING_CAMERA_CAMERACOMPONENT_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Entity/Component.h>
#include <ChilliSource/Core/Event/EventConnection.h>
#include <ChilliSource/Core/Math/Geometry/Shapes.h>

namespace ChilliSource
{
	namespace Rendering
	{
        //-----------------------------------------------------------
        /// Base class for camera components. The camera is added
        /// to the scene and provides the renderer with the view
        /// and projection matrices required to draw the scene correctly
        ///
        /// @author S Downie
        //-----------------------------------------------------------
		class CameraComponent : public Core::Component
		{
		public:
			CS_DECLARE_NAMEDTYPE(CameraComponent);
            //-----------------------------------------------------------
            /// Governs how the viewport updates with screen resize
            ///
            /// @author S Downie
            //-----------------------------------------------------------
            enum class ViewportResizePolicy
            {
                k_none,
                k_scaleWithScreen
            };
            //----------------------------------------------------------
            /// Constructor
            ///
            /// @author S Downie
            ///
            /// @param Near plane
            /// @param Far plane
            //----------------------------------------------------------
            CameraComponent(f32 in_nearClip, f32 in_farClip);
            //----------------------------------------------------------
            /// Virtual Destructor
            ///
            /// @author S Downie
            //----------------------------------------------------------
            virtual ~CameraComponent(){}
			//------------------------------------------------------
			/// @author S Downie
			///
			/// @param Near Z clipping distance in view space
			//------------------------------------------------------
			void SetNearClipping(f32 in_near);
			//------------------------------------------------------
			/// @author S Downie
			///
			/// @param Far Z clipping distance in view space
			//------------------------------------------------------
			void SetFarClipping(f32 in_far);
			//------------------------------------------------------
			/// @author S Downie
			///
			/// @return Projection matrix
			//------------------------------------------------------
			const Core::Matrix4& GetProjection();
			//------------------------------------------------------
			/// @author S Downie
			///
			/// @return View matrix
			//------------------------------------------------------
			const Core::Matrix4& GetView();
			//------------------------------------------------------
			/// Unproject from a point in screen space to a ray in
			/// world space
            ///
			/// @param Point in screen space
            ///
			/// @return Ray in world space with camera view direction
			//------------------------------------------------------
            Core::Ray Unproject(const Core::Vector2& in_screenPos);
			//------------------------------------------------------
			/// Convert from a point in world space to a point in
			/// screen space
            ///
            /// @param World space pos
            ///
            /// @return Screen space pos
			//------------------------------------------------------
            Core::Vector2 Project(const Core::Vector3& in_worldPos);
			//------------------------------------------------------
			/// @author S Downie
			///
			/// @return Camera frustum
			//------------------------------------------------------
			const Core::Frustum& GetFrustum();
			//------------------------------------------------------
            /// Orientate the given matrix to face the cameras
            /// view vector
            ///
            /// @author S Downie
            ///
            /// @param Matrix to billboard
            ///
            /// @return Billboarded matrix
            //------------------------------------------------------
            Core::Matrix4 Billboard(const Core::Matrix4& in_toBillboard);
            //--------------------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Gets the currently set opaque sort predicate for this camera
            //--------------------------------------------------------------------------------------------------
            const RendererSortPredicateSPtr& GetOpaqueSortPredicate() const;
            //--------------------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Gets the currently set opaque sort predicate for this camera
            //--------------------------------------------------------------------------------------------------
            const RendererSortPredicateSPtr& GetTransparentSortPredicate() const;
            //--------------------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Opaque sort predicate to use for this camera
            //--------------------------------------------------------------------------------------------------
            void SetOpaqueSortPredicate(const RendererSortPredicateSPtr& in_predicate);
            //--------------------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Transparent sort predicate to use for this camera
            //--------------------------------------------------------------------------------------------------
            void SetTransparentSortPredicate(const RendererSortPredicateSPtr& in_predicate);
            //--------------------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Culling predicate to use for this camera
            //--------------------------------------------------------------------------------------------------
            const ICullingPredicateSPtr& GetCullingPredicate() const;
            //--------------------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Culling predicate to use for this camera.
            //--------------------------------------------------------------------------------------------------
            void SetCullingPredicate(const ICullingPredicateSPtr& in_predicate);
            
		private:
			//------------------------------------------------------
			/// Delegates the calculation of the projection matrix
            /// to the concrete camera types.
			///
			/// @author S Downie
            ///
            /// @return Projection matrix
			//------------------------------------------------------
			virtual Core::Matrix4 CalculateProjectionMatrix() = 0;
            //------------------------------------------------------
            /// @author S Downie
            ///
            /// Recalculate frustum planes
            //------------------------------------------------------
            virtual void UpdateFrustum() = 0;
            //------------------------------------------------------
            /// Called when the component is added to an entity
            ///
            /// @author S Downie
            //------------------------------------------------------
            void OnAddedToEntity();
            //------------------------------------------------------
            /// Called when the entity transform changes
            ///
            /// @author S Downie
            //------------------------------------------------------
            void OnTransformChanged();
            
        protected:
            
            Core::Frustum m_frustum;
            f32 m_nearClip;
            f32 m_farClip;
            bool m_isProjCacheValid = false;
            bool m_isFrustumCacheValid = false;
            Core::Screen* m_screen = nullptr;

		private:
			
            Core::Matrix4 m_projMat;
			Core::Matrix4 m_viewMat;
			
            RendererSortPredicateSPtr m_opaqueSortPredicate;
            RendererSortPredicateSPtr m_transparentSortPredicate;
            
            ICullingPredicateSPtr m_cullPredicate;
            
            Core::EventConnectionUPtr m_transformChangedConnection;
		};
	}
}

#endif
