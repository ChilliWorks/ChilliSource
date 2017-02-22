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

#include <ChilliSource/Rendering/Shader/Shader.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Rendering/Shader/RenderShaderManager.h>

namespace ChilliSource
{
    CS_DEFINE_NAMEDTYPE(Shader);

    //------------------------------------------------------------------------------
    ShaderUPtr Shader::Create() noexcept
    {
        return ShaderUPtr(new Shader());
    }

    //------------------------------------------------------------------------------
    bool Shader::IsA(InterfaceIDType interfaceId) const noexcept
    {
        return Shader::InterfaceID == interfaceId;
    }

    //------------------------------------------------------------------------------
    void Shader::Build(const std::string& vertexShader, const std::string& fragmentShader) noexcept
    {
        DestroyRenderShader();
        
        auto renderShaderManager = Application::Get()->GetSystem<RenderShaderManager>();
        CS_ASSERT(renderShaderManager, "RenderShaderManager must exist.");
        
        m_renderShader = renderShaderManager->CreateRenderShader(vertexShader, fragmentShader);
    }

    //------------------------------------------------------------------------------
    const RenderShader* Shader::GetRenderShader() const noexcept
    {
        CS_ASSERT(GetLoadState() == LoadState::k_loaded, "Cannot access shader before it is loaded.");
        CS_ASSERT(m_renderShader, "Cannot access shader which has not been built.");
        
        return m_renderShader.get();
    }
    
    //------------------------------------------------------------------------------
    void Shader::DestroyRenderShader() noexcept
    {
        if (m_renderShader)
        {
            auto renderShaderManager = Application::Get()->GetSystem<RenderShaderManager>();
            CS_ASSERT(renderShaderManager, "RenderShaderManager must exist.");
            
            renderShaderManager->DestroyRenderShader(std::move(m_renderShader));
        }
    }

    //------------------------------------------------------------------------------
    Shader::~Shader() noexcept
    {
        DestroyRenderShader();
    }
}
