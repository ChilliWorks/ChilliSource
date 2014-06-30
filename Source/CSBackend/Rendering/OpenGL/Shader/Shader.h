//
//  Shader.h
//  Chilli Source
//  Created by Scott Downie on 22/11/2010.
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

#ifndef _CSBACKEND_RENDERING_OPENGL_SHADER_SHADER_H_
#define _CSBACKEND_RENDERING_OPENGL_SHADER_SHADER_H_

#include <ChilliSource/ChilliSource.h>
#include <CSBackend/Rendering/OpenGL/ForwardDeclarations.h>
#include <CSBackend/Rendering/OpenGL/Base/GLIncludes.h>
#include <ChilliSource/Rendering/Shader/Shader.h>

#include <unordered_map>

namespace CSBackend
{
	namespace OpenGL
	{
        //----------------------------------------------------------
        /// OpenGL GLSL shader resource. This holds the combined
        /// vertex and fragment shader programs and provides
        /// accessors to set shader variables.
        ///
        /// @author S Downie
        //----------------------------------------------------------
		class Shader final : public CSRendering::Shader
		{
		public:
			
            CS_DECLARE_NAMEDTYPE(Shader);
            
            //----------------------------------------------------------
            /// Specifies the policy for how the progrm should handle
            /// an missing variable when setting a uniform
            ///
            /// @author S Downie
            //----------------------------------------------------------
            enum class UniformNotFoundPolicy
            {
                k_failHard,
                k_failSilent
            };
            //----------------------------------------------------------
            /// Compile the given vertex and fragment shaders
            /// into a shader program. Asserts on failure.
            ///
            /// @param Vertex shader contents
            /// @param Fragment shader contents
            //----------------------------------------------------------
            void Build(const std::string& in_vs, const std::string& in_fs);
			//----------------------------------------------------------
			/// @author S Downie
			///
			/// @param Comparison Type
            ///
			/// @return Whether the class matches the comparison type
			//----------------------------------------------------------
			bool IsA(CSCore::InterfaceIDType in_interfaceId) const override;
            //----------------------------------------------------------
            /// Set this shader to be the active one.
            ///
            /// @author S Downie
            //----------------------------------------------------------
            void Bind();
            //----------------------------------------------------------
            /// Attempts to set the uniform shader variable with the
            /// given name to the given value.
            ///
            /// @author S Downie
            ///
            /// @param Uniform name
            /// @param Value
            /// @param Failure policy
            //----------------------------------------------------------
            void SetUniform(const std::string& in_varName, s32 in_value, UniformNotFoundPolicy in_notFoundPolicy = UniformNotFoundPolicy::k_failHard);
            //----------------------------------------------------------
            /// Attempts to set the uniform shader variable with the
            /// given name to the given value.
            ///
            /// @author S Downie
            ///
            /// @param Uniform name
            /// @param Value
            /// @param Failure policy
            //----------------------------------------------------------
            void SetUniform(const std::string& in_varName, f32 in_value, UniformNotFoundPolicy in_notFoundPolicy = UniformNotFoundPolicy::k_failHard);
            //----------------------------------------------------------
            /// Attempts to set the uniform shader variable with the
            /// given name to the given value.
            ///
            /// @author S Downie
            ///
            /// @param Uniform name
            /// @param Value
            /// @param Failure policy
            //----------------------------------------------------------
            void SetUniform(const std::string& in_varName, const CSCore::Vector2& in_value, UniformNotFoundPolicy in_notFoundPolicy = UniformNotFoundPolicy::k_failHard);
            //----------------------------------------------------------
            /// Attempts to set the uniform shader variable with the
            /// given name to the given value.
            ///
            /// @author S Downie
            ///
            /// @param Uniform name
            /// @param Value
            /// @param Failure policy
            //----------------------------------------------------------
            void SetUniform(const std::string& in_varName, const CSCore::Vector3& in_value, UniformNotFoundPolicy in_notFoundPolicy = UniformNotFoundPolicy::k_failHard);
            //----------------------------------------------------------
            /// Attempts to set the uniform shader variable with the
            /// given name to the given value.
            ///
            /// @author S Downie
            ///
            /// @param Uniform name
            /// @param Value
            /// @param Failure policy
            //----------------------------------------------------------
            void SetUniform(const std::string& in_varName, const CSCore::Vector4& in_value, UniformNotFoundPolicy in_notFoundPolicy = UniformNotFoundPolicy::k_failHard);
            //----------------------------------------------------------
            /// Attempts to set the uniform shader variable with the
            /// given name to the given value.
            ///
            /// @author S Downie
            ///
            /// @param Uniform name
            /// @param Value
            /// @param Failure policy
            //----------------------------------------------------------
            void SetUniform(const std::string& in_varName, const CSCore::Matrix4& in_value, UniformNotFoundPolicy in_notFoundPolicy = UniformNotFoundPolicy::k_failHard);
            //----------------------------------------------------------
            /// Attempts to set the uniform shader variable with the
            /// given name to the given value.
            ///
            /// @author S Downie
            ///
            /// @param Uniform name
            /// @param Value
            /// @param Failure policy
            //----------------------------------------------------------
            void SetUniform(const std::string& in_varName, const CSCore::Colour& in_value, UniformNotFoundPolicy in_notFoundPolicy = UniformNotFoundPolicy::k_failHard);
            //----------------------------------------------------------
            /// Attempts to set the uniform shader variable with the
            /// given name to the given values.
            ///
            /// @author S Downie
            ///
            /// @param Uniform name
            /// @param Values
            /// @param Failure policy
            //----------------------------------------------------------
            void SetUniform(const std::string& in_varName, const std::vector<CSCore::Vector4>& in_vec4Values, UniformNotFoundPolicy in_notFoundPolicy = UniformNotFoundPolicy::k_failHard);
            //----------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Whether shader has the given uniform variable
            //----------------------------------------------------------
            bool HasUniform(const std::string& in_varName);
            //----------------------------------------------------------
            /// Attempts to set the attribute shader variable with
            /// the given name. Will fail silently if variable not found.
            ///
            /// @author S Downie
            ///
            /// @param Variable name
            /// @param Num of components in the attribute
            /// @param Type
            /// @param Whether normalised or not
            /// @param Stride
            /// @param Pointer to offset in buffer
            //----------------------------------------------------------
            void SetAttribute(const std::string& in_varName, GLint in_size, GLenum in_type, GLboolean in_isNormalized, GLsizei in_stride, const GLvoid* in_offset);
            //----------------------------------------------------------
            /// Destroys the shader and returns the resource to the
            /// state prior to build
            ///
            /// @author S Downie
            //----------------------------------------------------------
			void Destroy();
            //----------------------------------------------------------
            /// Destructor. Destroys the GL program
            ///
            /// @author S Downie
            //----------------------------------------------------------
            ~Shader();
            
		private:
            friend CSRendering::ShaderUPtr CSRendering::Shader::Create();
			//----------------------------------------------------------
            /// Private constructor to enforce the use of the factory
            /// method
            ///
            /// @author S Downie
            //----------------------------------------------------------
			Shader();
			//----------------------------------------------------------
			/// Compiles a pre-loaded shader. Asserts on failure.
            ///
            /// @author S Downie
			///
			/// @param Shader source code
            /// @param Shader type vertex or fragment
			//----------------------------------------------------------
			void CompileShader(const std::string& in_shaderSource, GLenum in_shaderType);
			//----------------------------------------------------------
			/// Having compiled the vertex and fragment shaders
            /// this will link them into a single program. Asserts on
            /// failure
            ///
            /// @author S Downie
			///
			/// @param Vertex shader Id
            /// @param Fragment shader Id
			//----------------------------------------------------------
			void CreateProgram(GLuint in_vs, GLuint in_fs);
            //----------------------------------------------------------
            /// Assign the attribute handles based on the supported
            /// shader attribute names
            ///
            /// @author S Downie
            //----------------------------------------------------------
            void PopulateAttributeHandles();
            //----------------------------------------------------------
			/// Get the location of the given uniform variable.
            /// Subsequent calls to the same variable will return the
            /// cached value
            ///
            /// @author S Downie
            ///
            /// @param Uniform name
            ///
            /// @return Location or -1 if not found
            //----------------------------------------------------------
            GLint GetUniformHandle(const std::string& in_name);
            
		private:

            GLuint m_vertexShaderId;
			GLuint m_fragmentShaderId;
			GLuint m_programId;
            
            //---Uniforms
            std::unordered_map<std::string, GLint> m_uniformHandles;
        
            //---Attributes
            std::unordered_map<std::string, GLint> m_attribHandles;
		};
	}
}

#endif
