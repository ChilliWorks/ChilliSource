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

#include <ChilliSource/Rendering/Texture/RenderTextureManager.h>

#include <ChilliSource/Rendering/Base/RenderSnapshot.h>
#include <ChilliSource/Rendering/Base/TargetType.h>

namespace ChilliSource
{
    namespace
    {
        constexpr u32 k_texturePoolSize = 100;
    }
    
    CS_DEFINE_NAMEDTYPE(RenderTextureManager);

    //------------------------------------------------------------------------------
    RenderTextureManagerUPtr RenderTextureManager::Create() noexcept
    {
        return RenderTextureManagerUPtr(new RenderTextureManager());
    }
    
    //------------------------------------------------------------------------------
    RenderTextureManager::RenderTextureManager()
    : m_renderTexturePool(k_texturePoolSize, ObjectPoolAllocatorLimitPolicy::k_expand)
    {
        
    }
    
    //------------------------------------------------------------------------------
    bool RenderTextureManager::IsA(InterfaceIDType interfaceId) const noexcept
    {
        return (RenderTextureManager::InterfaceID == interfaceId);
    }
        
    //------------------------------------------------------------------------------
    UniquePtr<RenderTexture> RenderTextureManager::CreateTexture2D(std::unique_ptr<const u8[]> textureData, u32 textureDataSize, const Integer2& dimensions, ImageFormat imageFormat, ImageCompression imageCompression,
                                                               TextureFilterMode filterMode, TextureWrapMode wrapModeS, TextureWrapMode wrapModeT, bool isMipmapped, bool shouldBackupData) noexcept
    {
        UniquePtr<RenderTexture> renderTexture(MakeUnique<RenderTexture>(m_renderTexturePool, dimensions, imageFormat, imageCompression, filterMode, wrapModeS, wrapModeT, isMipmapped, shouldBackupData));
        auto rawRenderTexture = renderTexture.get();
        
        PendingLoadCommand2D loadCommand;
        loadCommand.m_textureData = std::move(textureData);
        loadCommand.m_textureDataSize = textureDataSize;
        loadCommand.m_renderTexture = rawRenderTexture;
        
        std::unique_lock<std::mutex> lock(m_mutex);
        m_pendingLoadCommands2D.push_back(std::move(loadCommand));
        
        return renderTexture;
    }
    
    //------------------------------------------------------------------------------
    UniquePtr<RenderTexture> RenderTextureManager::CreateCubemap(std::array<std::unique_ptr<const u8[]>, 6> textureData, u32 textureDataSize, const Integer2& dimensions, ImageFormat imageFormat, ImageCompression imageCompression,
                                                             TextureFilterMode filterMode, TextureWrapMode wrapModeS, TextureWrapMode wrapModeT, bool isMipmapped, bool shouldBackupData) noexcept
    {
        UniquePtr<RenderTexture> renderTexture(MakeUnique<RenderTexture>(m_renderTexturePool, dimensions, imageFormat, imageCompression, filterMode, wrapModeS, wrapModeT, isMipmapped, shouldBackupData));
        auto rawRenderTexture = renderTexture.get();
        
        PendingLoadCommandCubemap loadCommand;
        loadCommand.m_textureData = std::move(textureData);
        loadCommand.m_textureDataSize = textureDataSize;
        loadCommand.m_renderTexture = rawRenderTexture;
        
        std::unique_lock<std::mutex> lock(m_mutex);
        m_pendingLoadCommandsCubemap.push_back(std::move(loadCommand));
        
        return renderTexture;
    }
    
    //------------------------------------------------------------------------------
    void RenderTextureManager::DestroyRenderTexture2D(UniquePtr<RenderTexture> renderTexture) noexcept
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_pendingUnloadCommands2D.push_back(std::move(renderTexture));
    }
    
    //------------------------------------------------------------------------------
    void RenderTextureManager::DestroyRenderTextureCubemap(UniquePtr<RenderTexture> renderTexture) noexcept
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_pendingUnloadCommandsCubemap.push_back(std::move(renderTexture));
    }
    
    //------------------------------------------------------------------------------
    void RenderTextureManager::OnRenderSnapshot(TargetType targetType, RenderSnapshot& renderSnapshot, IAllocator* frameAllocator) noexcept
    {
        if(targetType == TargetType::k_main)
        {
            auto preRenderCommandList = renderSnapshot.GetPreRenderCommandList();
            auto postRenderCommandList = renderSnapshot.GetPostRenderCommandList();
            
            std::unique_lock<std::mutex> lock(m_mutex);
            
            for (auto& loadCommand : m_pendingLoadCommands2D)
            {
                preRenderCommandList->AddLoadTextureCommand(loadCommand.m_renderTexture, std::move(loadCommand.m_textureData), loadCommand.m_textureDataSize);
            }
            m_pendingLoadCommands2D.clear();
            
            for (auto& loadCommand : m_pendingLoadCommandsCubemap)
            {
                preRenderCommandList->AddLoadCubemapCommand(loadCommand.m_renderTexture, std::move(loadCommand.m_textureData), loadCommand.m_textureDataSize);
            }
            m_pendingLoadCommandsCubemap.clear();
            
            for (auto& unloadCommand : m_pendingUnloadCommands2D)
            {
                postRenderCommandList->AddUnloadTextureCommand(std::move(unloadCommand));
            }
            m_pendingUnloadCommands2D.clear();
            
            for (auto& unloadCommand : m_pendingUnloadCommandsCubemap)
            {
                postRenderCommandList->AddUnloadCubemapCommand(std::move(unloadCommand));
            }
            m_pendingUnloadCommandsCubemap.clear();
        }
    }
}
