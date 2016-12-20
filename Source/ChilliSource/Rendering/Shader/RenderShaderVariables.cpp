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

#include <ChilliSource/Rendering/Shader/RenderShaderVariables.h>

namespace ChilliSource
{
    //------------------------------------------------------------------------------
    RenderShaderVariables::RenderShaderVariables(const std::unordered_map<std::string, f32>& floatVars, const std::unordered_map<std::string, Vector2>& vec2Vars, const std::unordered_map<std::string, Vector3>& vec3Vars,
                                                 const std::unordered_map<std::string, Vector4>& vec4Vars, const std::unordered_map<std::string, Matrix4>& mat4Vars,
                                                 const std::unordered_map<std::string, Colour>& colourVars) noexcept
        : m_floatVars(floatVars), m_vec2Vars(vec2Vars), m_vec3Vars(vec3Vars), m_vec4Vars(vec4Vars), m_mat4Vars(mat4Vars), m_colourVars(colourVars)
    {
    }
    
    //------------------------------------------------------------------------------
    RenderShaderVariables::RenderShaderVariables(const RenderShaderVariables& toCopy)
    {
        m_floatVars = toCopy.m_floatVars;
        m_vec2Vars = toCopy.m_vec2Vars;
        m_vec3Vars = toCopy.m_vec3Vars;
        m_vec4Vars = toCopy.m_vec4Vars;
        m_colourVars = toCopy.m_colourVars;
    }
}
