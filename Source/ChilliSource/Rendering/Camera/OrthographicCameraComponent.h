//
//  OrthographicCameraComponent.h
//  ChilliSource
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

#ifndef _CHILLISOURCE_RENDERING_CAMERA_ORTHOGRAPHICCAMERACOMPONENT_H_
#define _CHILLISOURCE_RENDERING_CAMERA_ORTHOGRAPHICCAMERACOMPONENT_H_

#include <ChilliSource/Rendering/Camera/CameraComponent.h>

namespace ChilliSource
{
    //----------------------------------------------------------
    /// An orthographic camera. Objects do not scale with
    /// distance from the camera.
    ///
    /// @author S Downie
    //----------------------------------------------------------
    class OrthographicCameraComponent final : public CameraComponent
    {
    public:

        CS_DECLARE_NAMEDTYPE(OrthographicCameraComponent);

        /// @param viewportSize
        ///     Size of the viewport
        /// @param nearClip
        ///     Distance to the near clip plane in view space
        /// @param farClip
        ///     Distance to the far clip plane in view space
        /// @param resizePolicy
        ///     Governs how the viewport size responds to changes to the screen size
        ///
        OrthographicCameraComponent(const Vector2& viewportSize, f32 nearClip, f32 farClip, ViewportResizePolicy resizePolicy = ViewportResizePolicy::k_scaleWithScreen);
        //----------------------------------------------------------
        /// @author S Downie
        ///
        /// @param Comparison Type
        ///
        /// @return Whether the class matches the comparison type
        //----------------------------------------------------------
        bool IsA(InterfaceIDType in_interfaceId) const override;
        //----------------------------------------------------------
        /// @author S Downie
        ///
        /// @param Vector containing width and height of viewport
        //----------------------------------------------------------
        void SetViewportSize(const Vector2& in_size);
        
    private:

        //------------------------------------------------------
        /// Calculate the orthographic projection matrix
        /// based on the current viewport size and near and
        /// far planes
        ///
        /// @author S Downie
        ///
        /// @return Projection matrix
        //------------------------------------------------------
        Matrix4 CalculateProjectionMatrix() override;
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
        void OnResolutionChanged(const Vector2& in_resolution);

    private:
        
        ViewportResizePolicy m_resizePolicy;
        
        Vector2 m_viewportSize;
        Vector2 m_currentViewportSize;
        Vector2 m_referenceScreenSize;
        
        EventConnectionUPtr m_screenResizedConnection;
    };
}

#endif
