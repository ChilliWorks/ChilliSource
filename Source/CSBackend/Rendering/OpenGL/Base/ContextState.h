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

#ifndef _CSBACKEND_RENDERING_OPENGL_BASE_CONTEXTSTATE_H_
#define _CSBACKEND_RENDERING_OPENGL_BASE_CONTEXTSTATE_H_

#include <CSBackend/Rendering/OpenGL/ForwardDeclarations.h>

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Math/Matrix4.h>
#include <ChilliSource/Core/Math/Vector3.h>

namespace CSBackend
{
    namespace OpenGL
    {
        /// Describes the current state of the opengl context, including the currently
        /// applied render target, camera, lights and render resources.
        ///
        /// This is not thread-safe and should only be accessed from a single thread at
        /// a time.
        ///
        class ContextState final
        {
        public:
            /// Describes the state of the camera, including it's view projection matrix and the
            /// current camera position.
            ///
            /// This is immutable and therefore thread-safe.
            ///
            class CameraState final
            {
            public:
                CameraState() = default;
                
                /// Constructs the container with the given view projection matrix and position data.
                ///
                /// @param position
                ///     The world space position of the camera.
                /// @param viewProjectionMatrix
                ///     The view projection matrix of the camera.
                ///
                CameraState(const ChilliSource::Vector3& position, const ChilliSource::Matrix4& viewProjectionMatrix) noexcept;
                
                /// @return The world space position of the camera.
                ///
                const ChilliSource::Vector3& GetPosition() const noexcept { return m_position; }
                
                /// @return The view projection matrix of the camera.
                ///
                const ChilliSource::Matrix4& GetViewProjectionMatrix() const noexcept { return m_viewProjectionMatrix; }
                
            private:
                ChilliSource::Vector3 m_position;
                ChilliSource::Matrix4 m_viewProjectionMatrix;
            };
            
            /// @return The currently bound render shader
            ///
            const ChilliSource::RenderShader* GetRenderShader() const noexcept { return m_renderShader; }
            
            /// @return The currently bound render texture
            ///
            const ChilliSource::RenderTexture* GetRenderTexture() const noexcept { return m_renderTexture; }
            
            /// @return The currently bound render material
            ///
            const ChilliSource::RenderMaterial* GetRenderMaterial() const noexcept { return m_renderMaterial; }
            
            /// @return The currently bound render mesh
            ///
            const ChilliSource::RenderMesh* GetRenderMesh() const noexcept { return m_renderMesh; }
            
            /// @return The current state of the camera.
            ///
            const CameraState& GetCameraState() const noexcept { return m_cameraState; }
            
            /// Sets the render shader which is currently bound in the context.
            ///
            /// @param renderShader
            ///     The currently bound render shader.
            ///
            void SetRenderShader(const ChilliSource::RenderShader* renderShader) noexcept { m_renderShader = renderShader; }
            
            /// Sets the render texture which is currently bound in the context.
            ///
            /// @param renderTexture
            ///     The currently bound render texture.
            ///
            void SetRenderTexture(const ChilliSource::RenderTexture* renderTexture) noexcept { m_renderTexture = renderTexture; }
            
            /// Sets the render material which is currently bound in the context.
            ///
            /// @param renderMaterial
            ///     The currently bound render material.
            ///
            void SetRenderMaterial(const ChilliSource::RenderMaterial* renderMaterial) noexcept { m_renderMaterial = renderMaterial; }
            
            /// Sets the render mesh which is currently bound in the context.
            ///
            /// @param renderMesh
            ///     The currently bound render mesh.
            ///
            void SetRenderMesh(const ChilliSource::RenderMesh* renderMesh) noexcept { m_renderMesh = renderMesh; }
            
            /// Sets the current camera state.
            ///
            /// @param cameraState
            ///     The current state of the camera.
            ///
            void SetCameraState(const CameraState& cameraState) noexcept { m_cameraState = cameraState; }
            
        private:
            const ChilliSource::RenderShader* m_renderShader = nullptr;
            const ChilliSource::RenderTexture* m_renderTexture = nullptr;
            const ChilliSource::RenderMaterial* m_renderMaterial = nullptr;
            const ChilliSource::RenderMesh* m_renderMesh = nullptr;
            CameraState m_cameraState;
        };
    }
}

#endif
