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

#include <ChilliSource/Rendering/Texture/Cubemap.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Rendering/Texture/RenderTexture.h>
#include <ChilliSource/Rendering/Texture/RenderTextureManager.h>
#include <ChilliSource/Rendering/Texture/TextureDesc.h>

namespace ChilliSource
{
    CS_DEFINE_NAMEDTYPE(Cubemap);

    //------------------------------------------------------------------------------
    CubemapUPtr Cubemap::Create() noexcept
    {
        return CubemapUPtr(new Cubemap());
    }
    
    //------------------------------------------------------------------------------
    bool Cubemap::IsA(InterfaceIDType interfaceId) const noexcept
    {
        return (Cubemap::InterfaceID == interfaceId);
    }

    //------------------------------------------------------------------------------
    void Cubemap::Build(std::array<DataUPtr, 6> textureData, u32 dataSize, const TextureDesc& textureDesc) noexcept
    {
        DestroyRenderTexture();
        
        auto renderTextureManager = Application::Get()->GetSystem<RenderTextureManager>();
        CS_ASSERT(renderTextureManager, "RenderTextureManager must exist.");
        
        m_restoreTextureDataEnabled = textureDesc.IsRestoreTextureDataEnabled();
        
        m_renderTexture = renderTextureManager->CreateCubemap(std::move(textureData), dataSize, textureDesc.GetDimensions(), textureDesc.GetImageFormat(), textureDesc.GetImageCompression(),
                                                              textureDesc.GetFilterMode(), textureDesc.GetWrapModeS(), textureDesc.GetWrapModeT(), textureDesc.IsMipmappingEnabled(), m_restoreTextureDataEnabled);
    }

    //------------------------------------------------------------------------------
    const Integer2& Cubemap::GetDimensions() const noexcept
    {
        CS_ASSERT(GetLoadState() == LoadState::k_loaded, "Cannot access texture before it is loaded.");
        CS_ASSERT(m_renderTexture, "Cannot access texture which has not been built.");
        
        return m_renderTexture->GetDimensions();
    }

    //------------------------------------------------------------------------------
    ImageFormat Cubemap::GetImageFormat() const noexcept
    {
        CS_ASSERT(GetLoadState() == LoadState::k_loaded, "Cannot access texture before it is loaded.");
        CS_ASSERT(m_renderTexture, "Cannot access texture which has not been built.");
        
        return m_renderTexture->GetImageFormat();
    }

    //------------------------------------------------------------------------------
    ImageCompression Cubemap::GetImageCompression() const noexcept
    {
        CS_ASSERT(GetLoadState() == LoadState::k_loaded, "Cannot access texture before it is loaded.");
        CS_ASSERT(m_renderTexture, "Cannot access texture which has not been built.");
        
        return m_renderTexture->GetImageCompression();
    }
    
    //------------------------------------------------------------------------------
    TextureFilterMode Cubemap::GetFilterMode() const noexcept
    {
        CS_ASSERT(GetLoadState() == LoadState::k_loaded, "Cannot access texture before it is loaded.");
        CS_ASSERT(m_renderTexture, "Cannot access texture which has not been built.");
        
        return m_renderTexture->GetFilterMode();
    }

    //------------------------------------------------------------------------------
    TextureWrapMode Cubemap::GetWrapModeS() const noexcept
    {
        CS_ASSERT(GetLoadState() == LoadState::k_loaded, "Cannot access texture before it is loaded.");
        CS_ASSERT(m_renderTexture, "Cannot access texture which has not been built.");
        
        return m_renderTexture->GetWrapModeS();
    }

    //------------------------------------------------------------------------------
    TextureWrapMode Cubemap::GetWrapModeT() const noexcept
    {
        CS_ASSERT(GetLoadState() == LoadState::k_loaded, "Cannot access texture before it is loaded.");
        CS_ASSERT(m_renderTexture, "Cannot access texture which has not been built.");
        
        return m_renderTexture->GetWrapModeT();
    }

    //------------------------------------------------------------------------------
    bool Cubemap::IsMipmappingEnabled() const noexcept
    {
        CS_ASSERT(GetLoadState() == LoadState::k_loaded, "Cannot access texture before it is loaded.");
        CS_ASSERT(m_renderTexture, "Cannot access texture which has not been built.");
        
        return m_renderTexture->IsMipmapped();
    }

    //------------------------------------------------------------------------------
    bool Cubemap::IsRestoreTextureDataEnabled() const noexcept
    {
        CS_ASSERT(GetLoadState() == LoadState::k_loaded, "Cannot access texture before it is loaded.");
        CS_ASSERT(m_renderTexture, "Cannot access texture which has not been built.");
        
        return m_restoreTextureDataEnabled;
    }

    //------------------------------------------------------------------------------
    const RenderTexture* Cubemap::GetRenderTexture() const noexcept
    {
        CS_ASSERT(GetLoadState() == LoadState::k_loaded, "Cannot access texture before it is loaded.");
        CS_ASSERT(m_renderTexture, "Cannot access texture which has not been built.");
        
        return m_renderTexture.get();
    }
    
    //------------------------------------------------------------------------------
    void Cubemap::DestroyRenderTexture() noexcept
    {
        if (m_renderTexture)
        {
            auto renderTextureManager = Application::Get()->GetSystem<RenderTextureManager>();
            CS_ASSERT(renderTextureManager, "RenderTextureManager must exist.");
            
            renderTextureManager->DestroyRenderTextureCubemap(std::move(m_renderTexture));
        }
    }
    
    //------------------------------------------------------------------------------
    Cubemap::~Cubemap() noexcept
    {
        DestroyRenderTexture();
    }
}
