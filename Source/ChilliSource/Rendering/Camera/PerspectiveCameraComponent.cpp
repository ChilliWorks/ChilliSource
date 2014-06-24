//
//  PerspectiveCameraComponent.cpp
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

#include <ChilliSource/Rendering/Camera/PerspectiveCameraComponent.h>

#include <ChilliSource/Core/Base/Screen.h>
#include <ChilliSource/Core/Delegate/MakeDelegate.h>
#include <ChilliSource/Core/Event/IConnectableEvent.h>

namespace ChilliSource
{
	namespace Rendering
	{
		CS_DEFINE_NAMEDTYPE(PerspectiveCameraComponent);
        //----------------------------------------------------------
        //----------------------------------------------------------
        PerspectiveCameraComponent::PerspectiveCameraComponent(f32 in_aspectRatio, f32 in_fov, ViewportResizePolicy in_resizePolicy, f32 in_nearClip, f32 in_farClip)
        : CameraComponent(in_nearClip, in_farClip), m_aspectRatio(in_aspectRatio), m_fov(in_fov), m_resizePolicy(in_resizePolicy)
        {
            switch(m_resizePolicy)
            {
                case ViewportResizePolicy::k_none:
                    break;
                case ViewportResizePolicy::k_scaleWithScreen:
                    m_screenResizedConnection = m_screen->GetResolutionChangedEvent().OpenConnection(Core::MakeDelegate(this, &PerspectiveCameraComponent::OnResolutionChanged));
                    break;
            }
        }
		//----------------------------------------------------------
		//----------------------------------------------------------
		bool PerspectiveCameraComponent::IsA(CSCore::InterfaceIDType in_interfaceId) const
		{
			return (in_interfaceId == CameraComponent::InterfaceID || in_interfaceId == PerspectiveCameraComponent::InterfaceID);
		}
        //------------------------------------------------------
		//------------------------------------------------------
		void PerspectiveCameraComponent::SetFieldOfView(f32 in_fov)
		{
			m_fov = in_fov;
            
			m_isProjCacheValid = false;
		}
		//------------------------------------------------------
		//------------------------------------------------------
		void PerspectiveCameraComponent::SetAspectRatio(f32 in_aspectRatio)
		{
			m_aspectRatio = in_aspectRatio;
            
			m_isProjCacheValid = false;
		}
		//------------------------------------------------------
		//------------------------------------------------------
		f32 PerspectiveCameraComponent::GetFieldOfView() const
		{
			return m_fov;
		}
		//------------------------------------------------------
		//------------------------------------------------------
		f32 PerspectiveCameraComponent::GetAspectRatio() const
		{
			return m_aspectRatio;
		}
		//------------------------------------------------------
		//------------------------------------------------------
        Core::Matrix4 PerspectiveCameraComponent::CalculateProjectionMatrix()
		{
			return Core::Matrix4::CreatePerspectiveProjectionLH(m_fov, m_aspectRatio, m_nearClip, m_farClip);
		}
		//------------------------------------------------------
		//------------------------------------------------------
		void PerspectiveCameraComponent::UpdateFrustum()
		{
			m_frustum.CalculateClippingPlanes(GetView() * GetProjection());
		}
        //------------------------------------------------------
        //------------------------------------------------------
        void PerspectiveCameraComponent::OnResolutionChanged(const Core::Vector2& in_resolution)
        {
            switch(m_resizePolicy)
            {
                case ViewportResizePolicy::k_none:
                    break;
                case ViewportResizePolicy::k_scaleWithScreen:
                    SetAspectRatio(in_resolution.x / in_resolution.y);
                    break;
            }
        }
	}
}

