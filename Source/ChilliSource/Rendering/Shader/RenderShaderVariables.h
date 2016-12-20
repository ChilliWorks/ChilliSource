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

#ifndef _CHILLISOURCE_RENDERING_SHADER_RENDERSHADERVARIABLES_H_
#define _CHILLISOURCE_RENDERING_SHADER_RENDERSHADERVARIABLES_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/Colour.h>
#include <ChilliSource/Core/Math/Vector2.h>
#include <ChilliSource/Core/Math/Vector3.h>
#include <ChilliSource/Core/Math/Vector4.h>
#include <ChilliSource/Core/Math/Matrix4.h>

#include <unordered_map>

namespace ChilliSource
{
    /// A container for custom shader variables.
    ///
    /// This is immutable and therefore thread-safe.
    ///
    class RenderShaderVariables final
    {
    public:
        
        /// Creates a new instance with the given shader variables.
        ///
        /// @param floatVars
        ///     The variables of type: float.
        /// @param vec2Vars
        ///     The variables of type: Vector2.
        /// @param vec3Vars
        ///     The variables of type: Vector3.
        /// @param vec4Vars
        ///     The variables of type: Vector4.
        /// @param mat4Vars
        ///     The variables of type: Matrix4.
        /// @param colourVars
        ///     The variables of type: Colour.
        ///
        RenderShaderVariables(const std::unordered_map<std::string, f32>& floatVars, const std::unordered_map<std::string, Vector2>& vec2Vars, const std::unordered_map<std::string, Vector3>& vec3Vars,
                              const std::unordered_map<std::string, Vector4>& vec4Vars, const std::unordered_map<std::string, Matrix4>& mat4Vars, const std::unordered_map<std::string, Colour>& colourVars) noexcept;
        
        ///
        /// Copy constructor
        ///
        RenderShaderVariables(const RenderShaderVariables& toCopy);
        
        /// @return The variables of type: float.
        ///
        const std::unordered_map<std::string, f32>& GetFloatVariables() const noexcept { return m_floatVars; }
        
        /// @return The variables of type: Vector2.
        ///
        const std::unordered_map<std::string, Vector2>& GetVector2Variables() const noexcept { return m_vec2Vars; }
        
        /// @return The variables of type: Vector3.
        ///
        const std::unordered_map<std::string, Vector3>& GetVector3Variables() const noexcept { return m_vec3Vars; }
        
        /// @return The variables of type: Vector4.
        ///
        const std::unordered_map<std::string, Vector4>& GetVector4Variables() const noexcept { return m_vec4Vars; }
        
        /// @return The variables of type: Matrix4.
        ///
        const std::unordered_map<std::string, Matrix4>& GetMatrix4Variables() const noexcept { return m_mat4Vars; }
        
        /// @return The variables of type: Colour.
        ///
        const std::unordered_map<std::string, Colour>& GetColourVariables() const noexcept { return m_colourVars; }
        
    private:
        std::unordered_map<std::string, f32> m_floatVars;
        std::unordered_map<std::string, Vector2> m_vec2Vars;
        std::unordered_map<std::string, Vector3> m_vec3Vars;
        std::unordered_map<std::string, Vector4> m_vec4Vars;
        std::unordered_map<std::string, Matrix4> m_mat4Vars;
        std::unordered_map<std::string, Colour> m_colourVars;
    };
}

#endif
