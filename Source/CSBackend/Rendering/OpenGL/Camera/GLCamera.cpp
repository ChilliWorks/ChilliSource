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

#include <CSBackend/Rendering/OpenGL/Camera/GLCamera.h>

#include <CSBackend/Rendering/OpenGL/Shader/GLShader.h>

namespace CSBackend
{
    namespace OpenGL
    {
        namespace
        {
            const std::string k_uniformCameraPos = "u_cameraPos";
        }
        
        //------------------------------------------------------------------------------
        GLCamera::GLCamera(const ChilliSource::Vector3& position, const ChilliSource::Matrix4& viewMatrix, const ChilliSource::Matrix4& viewProjectionMatrix) noexcept
            : m_position(position), m_viewMatrix(viewMatrix), m_viewProjectionMatrix(viewProjectionMatrix)
        {
        }
        
        //------------------------------------------------------------------------------
        void GLCamera::Apply(GLShader* glShader) const noexcept
        {
            glShader->SetUniform(k_uniformCameraPos, m_position, GLShader::FailurePolicy::k_silent);
        }
    }
}
