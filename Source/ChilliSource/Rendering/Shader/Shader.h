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

#ifndef _CHILLISOURCE_RENDERING_SHADER_SHADER_H_
#define _CHILLISOURCE_RENDERING_SHADER_SHADER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Resource/Resource.h>
#include <ChilliSource/Core/Memory/UniquePtr.h>

namespace ChilliSource
{
    /// A resource which represents a shader in the underlying render system.
    ///
    /// This is not thread safe and should only be accessed from one thread at a time.
    ///
    class Shader final : public Resource
    {
    public:
        CS_DECLARE_NAMEDTYPE(Shader);
        
        /// Allows querying of whether or not this system implements the interface described by the
        /// given interface Id. Typically this is not called directly as the templated equivalent
        /// IsA<Interface>() is preferred.
        ///
        /// @param interfaceId
        ///     The Id of the interface.
        ///
        /// @return Whether or not the interface is implemented.
        ///
        bool IsA(InterfaceIDType interfaceId) const noexcept override;
        
        /// Builds the shader with the given shader data. The shader must not already be
        /// in the loaded state.
        ///
        /// @param vertexShader
        ///     The vertex shader string.
        /// @param fragmentShaderData
        ///     The fragment shader string.
        ///
        void Build(const std::string& vertexShader, const std::string& fragmentShader) noexcept;
        
        /// @return The underlying RenderShader used by the render system.
        ///
        const RenderShader* GetRenderShader() const noexcept;
        
        ~Shader() noexcept;
        
    protected:
        friend class ResourcePool;
        
        /// A factory method for creating new, empty instances of the resource. This must only be
        /// called by ResourcePool.
        ///
        /// @return The new instance of the resource.
        ///
        static ShaderUPtr Create() noexcept;
        
        /// Destroys the render shader if there is currently one available.
        ///
        void DestroyRenderShader() noexcept;
        
        Shader() = default;
        
        UniquePtr<RenderShader> m_renderShader;
    };
}

#endif
