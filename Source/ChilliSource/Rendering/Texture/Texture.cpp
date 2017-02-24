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

#include <ChilliSource/Rendering/Texture/Texture.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Rendering/Texture/RenderTexture.h>
#include <ChilliSource/Rendering/Texture/RenderTextureManager.h>
#include <ChilliSource/Rendering/Texture/TextureDesc.h>

namespace ChilliSource
{
    CS_DEFINE_NAMEDTYPE(Texture);

    //------------------------------------------------------------------------------
    TextureUPtr Texture::Create() noexcept
    {
        return TextureUPtr(new Texture());
    }
    
    //------------------------------------------------------------------------------
    bool Texture::IsA(InterfaceIDType interfaceId) const noexcept
    {
        return (Texture::InterfaceID == interfaceId);
    }

    //------------------------------------------------------------------------------
    void Texture::Build(DataUPtr textureData, u32 textureDataSize, const TextureDesc& textureDesc) noexcept
    {
        DestroyRenderTexture();
        
        auto renderTextureManager = Application::Get()->GetSystem<RenderTextureManager>();
        CS_ASSERT(renderTextureManager, "RenderTextureManager must exist.");
        
        m_restoreTextureDataEnabled = textureDesc.IsRestoreTextureDataEnabled();
        
        m_renderTexture = renderTextureManager->CreateTexture2D(std::move(textureData), textureDataSize, textureDesc.GetDimensions(), textureDesc.GetImageFormat(), textureDesc.GetImageCompression(),
                                                                    textureDesc.GetFilterMode(), textureDesc.GetWrapModeS(), textureDesc.GetWrapModeT(), textureDesc.IsMipmappingEnabled(), m_restoreTextureDataEnabled);
    }

    //------------------------------------------------------------------------------
    const Integer2& Texture::GetDimensions() const noexcept
    {
        CS_ASSERT(GetLoadState() == LoadState::k_loaded, "Cannot access texture before it is loaded.");
        CS_ASSERT(m_renderTexture, "Cannot access texture which has not been built.");
        
        return m_renderTexture->GetDimensions();
    }

    //------------------------------------------------------------------------------
    ImageFormat Texture::GetImageFormat() const noexcept
    {
        CS_ASSERT(GetLoadState() == LoadState::k_loaded, "Cannot access texture before it is loaded.");
        CS_ASSERT(m_renderTexture, "Cannot access texture which has not been built.");
        
        return m_renderTexture->GetImageFormat();
    }

    //------------------------------------------------------------------------------
    ImageCompression Texture::GetImageCompression() const noexcept
    {
        CS_ASSERT(GetLoadState() == LoadState::k_loaded, "Cannot access texture before it is loaded.");
        CS_ASSERT(m_renderTexture, "Cannot access texture which has not been built.");
        
        return m_renderTexture->GetImageCompression();
    }
    
    //------------------------------------------------------------------------------
    TextureFilterMode Texture::GetFilterMode() const noexcept
    {
        CS_ASSERT(GetLoadState() == LoadState::k_loaded, "Cannot access texture before it is loaded.");
        CS_ASSERT(m_renderTexture, "Cannot access texture which has not been built.");
        
        return m_renderTexture->GetFilterMode();
    }

    //------------------------------------------------------------------------------
    TextureWrapMode Texture::GetWrapModeS() const noexcept
    {
        CS_ASSERT(GetLoadState() == LoadState::k_loaded, "Cannot access texture before it is loaded.");
        CS_ASSERT(m_renderTexture, "Cannot access texture which has not been built.");
        
        return m_renderTexture->GetWrapModeS();
    }

    //------------------------------------------------------------------------------
    TextureWrapMode Texture::GetWrapModeT() const noexcept
    {
        CS_ASSERT(GetLoadState() == LoadState::k_loaded, "Cannot access texture before it is loaded.");
        CS_ASSERT(m_renderTexture, "Cannot access texture which has not been built.");
        
        return m_renderTexture->GetWrapModeT();
    }

    //------------------------------------------------------------------------------
    bool Texture::IsMipmappingEnabled() const noexcept
    {
        CS_ASSERT(GetLoadState() == LoadState::k_loaded, "Cannot access texture before it is loaded.");
        CS_ASSERT(m_renderTexture, "Cannot access texture which has not been built.");
        
        return m_renderTexture->IsMipmapped();
    }

    //------------------------------------------------------------------------------
    bool Texture::IsRestoreTextureDataEnabled() const noexcept
    {
        CS_ASSERT(GetLoadState() == LoadState::k_loaded, "Cannot access texture before it is loaded.");
        CS_ASSERT(m_renderTexture, "Cannot access texture which has not been built.");
        
        return m_restoreTextureDataEnabled;
    }

    //------------------------------------------------------------------------------
    const RenderTexture* Texture::GetRenderTexture() const noexcept
    {
        CS_ASSERT(GetLoadState() == LoadState::k_loaded, "Cannot access texture before it is loaded.");
        CS_ASSERT(m_renderTexture, "Cannot access texture which has not been built.");
        
        return m_renderTexture.get();
    }
    
    //------------------------------------------------------------------------------
    void Texture::DestroyRenderTexture() noexcept
    {
        if (m_renderTexture)
        {
            auto renderTextureManager = Application::Get()->GetSystem<RenderTextureManager>();
            CS_ASSERT(renderTextureManager, "RenderTextureManager must exist.");
            
            renderTextureManager->DestroyRenderTexture2D(std::move(m_renderTexture));
        }
    }
    
    //------------------------------------------------------------------------------
    Texture::~Texture() noexcept
    {
        DestroyRenderTexture();
    }
}
