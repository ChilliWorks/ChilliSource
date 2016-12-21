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

#ifndef _CSBACKEND_RENDERING_OPENGL_CAMERA_GLCAMERA_H_
#define _CSBACKEND_RENDERING_OPENGL_CAMERA_GLCAMERA_H_

#include <CSBackend/Rendering/OpenGL/ForwardDeclarations.h>

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Math/Matrix4.h>
#include <ChilliSource/Core/Math/Vector3.h>

namespace CSBackend
{
    namespace OpenGL
    {
        /// Describes the state of the camera, including it's view projection matrix and the
        /// current camera position.
        ///
        /// This is immutable and therefore thread-safe.
        ///
        class GLCamera final
        {
        public:
            GLCamera() = default;
            
            /// Constructs the camera with the given view projection matrix and position data.
            ///
            /// @param position
            ///     The world space position of the camera.
            /// @param viewMatrix
            ///     The view matrix (inverse transform) of the camera
            /// @param viewProjectionMatrix
            ///     The view projection matrix of the camera.
            ///
            GLCamera(const ChilliSource::Vector3& position, const ChilliSource::Matrix4& viewMatrix, const ChilliSource::Matrix4& viewProjectionMatrix) noexcept;
            
            /// @return The world space position of the camera.
            ///
            const ChilliSource::Vector3& GetPosition() const noexcept { return m_position; }
            
            /// @return The view matrix of the camera.
            ///
            const ChilliSource::Matrix4& GetViewMatrix() const noexcept { return m_viewMatrix; }
            
            /// @return The view projection matrix of the camera.
            ///
            const ChilliSource::Matrix4& GetViewProjectionMatrix() const noexcept { return m_viewProjectionMatrix; }
            
            /// Applies the camera to the given shader.
            ///
            /// This must be called on the render thread.
            ///
            /// @param glShader
            ///     The shader the camera data should be applied to.
            ///
            void Apply(GLShader* glShader) const noexcept;
            
        private:
            ChilliSource::Vector3 m_position;
            ChilliSource::Matrix4 m_viewMatrix;
            ChilliSource::Matrix4 m_viewProjectionMatrix;
        };
    }
}

#endif
