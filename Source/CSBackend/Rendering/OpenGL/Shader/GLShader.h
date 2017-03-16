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

#ifndef _CSBACKEND_RENDERING_OPENGL_SHADER_GLSHADER_H_
#define _CSBACKEND_RENDERING_OPENGL_SHADER_GLSHADER_H_

#include <CSBackend/Rendering/OpenGL/ForwardDeclarations.h>
#include <CSBackend/Rendering/OpenGL/Base/GLIncludes.h>

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/Colour.h>
#include <ChilliSource/Core/Math/Matrix4.h>
#include <ChilliSource/Core/Math/Vector2.h>
#include <ChilliSource/Core/Math/Vector3.h>
#include <ChilliSource/Core/Math/Vector4.h>

#include <unordered_map>
#include <array>

namespace CSBackend
{
    namespace OpenGL
    {
        /// A container for all OpenGL functionality relating to a single shader, including loading,
        /// binding and applying attributes and uniforms.
        ///
        /// This is not thread-safe and should only be accessed from the render thread.
        ///
        class GLShader final
        {
        public:
            CS_DECLARE_NOCOPY(GLShader);
            
			static const u8 k_numAttributes = 8;
            static const std::string k_attributePosition;
            static const std::string k_attributeNormal;
            static const std::string k_attributeTangent;
            static const std::string k_attributeBitangent;
            static const std::string k_attributeTexCoord;
            static const std::string k_attributeColour;
            static const std::string k_attributeWeights;
            static const std::string k_attributeJointIndices;
            
            /// An enum describing the different types of failure policy. This is used when setting
            /// uniforms to judge if an assertion should occur when the uniform doesn't exist.
            ///
            enum class FailurePolicy
            {
                k_hard,
                k_silent
            };
            
            /// Creates and loads a new shader with the given vertex and fragment shader strings.
            ///
            /// @param vertexShader
            ///     The vertex shader string.
            /// @param fragmentShader
            ///     The fragment shader string.
            ///
            GLShader(const std::string& vertexShader, const std::string& fragmentShader) noexcept;
            
            /// Binds the shader such that it is ready for use in rendering.
            ///
            void Bind() noexcept;
            
            /// Sets the uniform with the given name to the given value. If if the hard failure policy
            /// is specified and there is no uniform with the requested name, then this will assert.
            ///
            /// @param name
            ///     The name of the uniform.
            /// @param value
            ///     The value to set the uniform to.
            /// @param failurePolicy
            ///     The failure policy for if the uniform doesn't exist. Defaults to hard.
            ///
            void SetUniform(const std::string& name, s32 value, FailurePolicy failurePolicy = FailurePolicy::k_hard) noexcept;
            
            /// Sets the uniform with the given name to the given value. If if the hard failure policy
            /// is specified and there is no uniform with the requested name, then this will assert.
            ///
            /// @param name
            ///     The name of the uniform.
            /// @param value
            ///     The value to set the uniform to.
            /// @param failurePolicy
            ///     The failure policy for if the uniform doesn't exist. Defaults to hard.
            ///
            void SetUniform(const std::string& name, f32 value, FailurePolicy failurePolicy = FailurePolicy::k_hard) noexcept;
            
            /// Sets the uniform with the given name to the given value. If if the hard failure policy
            /// is specified and there is no uniform with the requested name, then this will assert.
            ///
            /// @param name
            ///     The name of the uniform.
            /// @param value
            ///     The value to set the uniform to.
            /// @param failurePolicy
            ///     The failure policy for if the uniform doesn't exist. Defaults to hard.
            ///
            void SetUniform(const std::string& name, const ChilliSource::Vector2& value, FailurePolicy failurePolicy = FailurePolicy::k_hard) noexcept;
            
            /// Sets the uniform with the given name to the given value. If if the hard failure policy
            /// is specified and there is no uniform with the requested name, then this will assert.
            ///
            /// @param name
            ///     The name of the uniform.
            /// @param value
            ///     The value to set the uniform to.
            /// @param failurePolicy
            ///     The failure policy for if the uniform doesn't exist. Defaults to hard.
            ///
            void SetUniform(const std::string& name, const ChilliSource::Vector3& value, FailurePolicy failurePolicy = FailurePolicy::k_hard) noexcept;
            
            /// Sets the uniform with the given name to the given value. If if the hard failure policy
            /// is specified and there is no uniform with the requested name, then this will assert.
            ///
            /// @param name
            ///     The name of the uniform.
            /// @param value
            ///     The value to set the uniform to.
            /// @param failurePolicy
            ///     The failure policy for if the uniform doesn't exist. Defaults to hard.
            ///
            void SetUniform(const std::string& name, const ChilliSource::Vector4& value, FailurePolicy failurePolicy = FailurePolicy::k_hard) noexcept;
            
            /// Sets the uniform with the given name to the given value. If if the hard failure policy
            /// is specified and there is no uniform with the requested name, then this will assert.
            ///
            /// @param name
            ///     The name of the uniform.
            /// @param value
            ///     The value to set the uniform to.
            /// @param failurePolicy
            ///     The failure policy for if the uniform doesn't exist. Defaults to hard.
            ///
            void SetUniform(const std::string& name, const ChilliSource::Matrix4& value, FailurePolicy failurePolicy = FailurePolicy::k_hard) noexcept;
            
            /// Sets the uniform with the given name to the given value. If if the hard failure policy
            /// is specified and there is no uniform with the requested name, then this will assert.
            ///
            /// @param name
            ///     The name of the uniform.
            /// @param value
            ///     The value to set the uniform to.
            /// @param failurePolicy
            ///     The failure policy for if the uniform doesn't exist. Defaults to hard.
            ///
            void SetUniform(const std::string& name, const ChilliSource::Colour& value, FailurePolicy failurePolicy = FailurePolicy::k_hard) noexcept;
            
            /// Sets the uniform with the given name to the given array of values. If if the hard failure
            /// policy is specified and there is no uniform with the requested name, then this will assert.
            ///
            /// @param name
            ///     The name of the uniform.
            /// @param values
            ///     The values to set the uniform to.
            /// @param numValues
            ///     The number of values to set.
            /// @param failurePolicy
            ///     The failure policy for if the uniform doesn't exist. Defaults to hard.
            ///
            void SetUniform(const std::string& name, const ChilliSource::Vector4* values, u32 numValues, FailurePolicy failurePolicy = FailurePolicy::k_hard) noexcept;
            
            /// @param index
            ///     Index of the attribute as defined in vertex format
            ///
            /// @return Handle of attribute in shader. -1 if doesn't exist
            ///
            GLint GetAttributeHandle(u32 index) const noexcept { return m_attributeHandles[index]; }
            
            /// Sets the attribute with the given name and data information. If the attribute doesn't
            /// exist then it will be ignored.
            ///
            /// @param index
            ///     The index of the attribute.
            /// @param size
            ///     The size of the data.
            /// @param type
            ///     The data type.
            /// @param isNormalised
            ///     Whether or not the data is normalised.
            /// @param stride
            ///     The stride of the data
            /// @param offset
            ///     The data offset
            ///
			void SetAttribute(u32 index, GLint size, GLenum type, GLboolean isNormalised, GLsizei stride, const GLvoid* offset) noexcept;
            
            /// Called when graphics memory is lost, usually through the GLContext being destroyed
            /// on Android. Function will set a flag to handle safe destructing of this object, preventing
            /// us from trying to delete invalid memory.
            ///
            void Invalidate() noexcept { m_invalidData = true; }
            
            /// Unloads the opengl shader.
            ///
            ~GLShader() noexcept;
            
        private:
            /// Evaulates which attributes exist in the shader and builds a map to their handles.
            ///
            void BuildAttributeHandleMap() noexcept;
            
            /// Finds the handle of the shader with the given name. Uniform handles are cached. This
            /// will assert if the uniform doesn't exist and the hard failure policy is specified.
            ///
            /// @param name
            ///     The name of the uniform.
            /// @param failurePolicy
            ///     The failure policy.
            ///
            /// @return The uniform handle, or -1 if it doesn't exist in the shader and a silent failure
            ///     policy was requested.
            ///
            GLint GetUniformHandle(const std::string& name, FailurePolicy failurePolicy) noexcept;
            
            GLuint m_vertexShaderId = 0;
            GLuint m_fragmentShaderId = 0;
            GLuint m_programId = 0;
            std::unordered_map<std::string, GLint> m_uniformHandles;
            std::array<GLint, k_numAttributes> m_attributeHandles;
            
            bool m_invalidData = false;
        };
    }
}

#endif
