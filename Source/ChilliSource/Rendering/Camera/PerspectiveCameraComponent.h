//
//  PerspectiveCameraComponent.h
//  Chilli Source
//  Created by Scott Downie on 23/06/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
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

#ifndef _CHILLISOURCE_RENDERING_CAMERA_PERSPECTIVECAMERACOMPONENT_H_
#define _CHILLISOURCE_RENDERING_CAMERA_PERSPECTIVECAMERACOMPONENT_H_

#include <ChilliSource/Rendering/Camera/CameraComponent.h>

namespace ChilliSource
{
	namespace Rendering
	{
        //----------------------------------------------------------
        /// An perspective camera. Objects scale with
        /// distance from the camera.
        ///
        /// @author S Downie
        //----------------------------------------------------------
		class PerspectiveCameraComponent final : public CameraComponent
		{
		public:
			CS_DECLARE_NAMEDTYPE(PerspectiveCameraComponent);
            //----------------------------------------------------------
            /// Constructor
            ///
            /// @author S Downie
            ///
            /// @param Aspect ratio
            /// @param Full vertical field of view in radians
            /// @param Resize policy
            /// @param Near plane
            /// @param Far plane
            //----------------------------------------------------------
			PerspectiveCameraComponent(f32 in_aspectRatio, f32 in_fov, ViewportResizePolicy in_resizePolicy, f32 in_nearClip, f32 in_farClip);
			//----------------------------------------------------------
			/// Is A
			///
			/// Returns if it is of the type given
			/// @param Comparison Type
			/// @return Whether the class matches the comparison type
			//----------------------------------------------------------
			bool IsA(Core::InterfaceIDType in_interfaceId) const override;
			//------------------------------------------------------
			/// @author S Downie
			///
			/// @param Full vertical viewing angle in radians
			//------------------------------------------------------
			void SetFieldOfView(f32 in_fov);
			//------------------------------------------------------
			/// @author S Downie
			///
			/// @param Aspect Ratio (Viewport width/viewport height)
			//------------------------------------------------------
			void SetAspectRatio(f32 in_aspectRatio);
			//------------------------------------------------------
			/// @author Ian Copland
			///
			/// @return The full vertical viewing angle in radians.
			//------------------------------------------------------
			f32 GetFieldOfView() const;
			//------------------------------------------------------
			/// @author Ian Copland
			///
			/// @return Aspect Ratio (Viewport width/viewport height)
			//------------------------------------------------------
			f32 GetAspectRatio() const;
            
		private:
			//------------------------------------------------------
			/// Calculate the perspective projection matrix
            /// based on the current FOV, aspect ratio and near and
            /// far planes.
			///
			/// @author S Downie
            ///
            /// @return Projection matrix
			//------------------------------------------------------
            Core::Matrix4 CalculateProjectionMatrix() override;
            //------------------------------------------------------
            /// @author S Downie
            ///
            /// Recalculate frustum planes
            //------------------------------------------------------
            void UpdateFrustum() override;
            //------------------------------------------------------
			/// Called when the resolution changes and resize with
			/// screen is enabled.
            ///
			/// @author I Copland
			//------------------------------------------------------
			void OnResolutionChanged(const Core::Vector2& in_resolution);

		private:
			
			f32 m_fov;
            f32 m_aspectRatio;
            ViewportResizePolicy m_resizePolicy;
            Core::EventConnectionUPtr m_screenResizedConnection;
		};
	}
}

#endif
