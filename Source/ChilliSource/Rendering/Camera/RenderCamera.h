//
//  The MIT License (MIT)
//
//  Copyright (c) 2016 Tag Games Limited
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

#ifndef _CHILLISOURCE_RENDERING_CAMERA_RENDERCAMERA_H_
#define _CHILLISOURCE_RENDERING_CAMERA_RENDERCAMERA_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Math/Matrix4.h>
#include <ChilliSource/Core/Math/Geometry/Shapes.h>

namespace ChilliSource
{
    /// A standard-layout container for data which the renderer requires that relates to a camera,
    /// such as the projection and view matrices, as well as the screen resolution.
    ///
    /// This is immutable and therefore thread safe.
    ///
    class RenderCamera final
    {
    public:
        RenderCamera() = default;
        
        /// Constructs a new instance of the container with the given camera world and projection
        /// matrices.
        ///
        /// @param worldMatrix
        ///     The world matrix of the camera. The view matrix will be derrived from this.
        /// @param projectionMatrix
        ///     The projection matrix of the camera.
        /// @param orientation
        ///     The orientation of the camera.
        ///
        RenderCamera(const Matrix4& worldMatrix, const Matrix4& projectionMatrix, const Quaternion& orientation) noexcept;
        
        /// @return The world matrix of the camera.
        ///
        const Matrix4& GetWorldMatrix() const noexcept { return m_worldMatrix; }
        
        /// @return The projection matrix of the camera.
        ///
        const Matrix4& GetProjectionMatrix() const noexcept { return m_projectionMatrix; }
        
        /// @return The view matrix of the camera.
        ///
        const Matrix4& GetViewMatrix() const noexcept { return m_viewMatrix; }
        
        /// @return The view projection matrix of the camera.
        ///
        const Matrix4& GetViewProjectionMatrix() const noexcept { return m_viewProjectionMatrix; }
        
        /// @return The orientation of the camera.
        ///
        const Quaternion& GetOrientation() const noexcept { return m_orientation; }
        
        /// @return The view frustrum of the camera.
        ///
        const Frustum& GetFrustrum() const noexcept { return m_frustrum; }
        
    private:
        Matrix4 m_worldMatrix;
        Matrix4 m_projectionMatrix;
        Matrix4 m_viewMatrix;
        Matrix4 m_viewProjectionMatrix;
        Quaternion m_orientation;
        Frustum m_frustrum;
    };
}

#endif
