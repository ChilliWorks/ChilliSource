//
//  ContextRestorer.cpp
//  ChilliSource
//  Created by Scott Downie on 14/04/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
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

#include <CSBackend/Rendering/OpenGL/Base/GLContextRestorer.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Resource/ResourcePool.h>
#include <ChilliSource/Rendering/Base/RenderSnapshot.h>
#include <ChilliSource/Rendering/Base/TargetType.h>
#include <ChilliSource/Rendering/Material/Material.h>
#include <ChilliSource/Rendering/Model/Model.h>
#include <ChilliSource/Rendering/Model/RenderMesh.h>
#include <ChilliSource/Rendering/Shader/RenderShader.h>
#include <ChilliSource/Rendering/Shader/Shader.h>
#include <ChilliSource/Rendering/Target/RenderTargetGroupManager.h>
#include <ChilliSource/Rendering/Texture/RenderTexture.h>
#include <ChilliSource/Rendering/Texture/Cubemap.h>
#include <ChilliSource/Rendering/Texture/Texture.h>

#include <CSBackend/Rendering/OpenGL/Model/GLMesh.h>
#include <CSBackend/Rendering/OpenGL/Shader/GLShader.h>
#include <CSBackend/Rendering/OpenGL/Target/GLTargetGroup.h>
#include <CSBackend/Rendering/OpenGL/Texture/GLCubemap.h>
#include <CSBackend/Rendering/OpenGL/Texture/GLTexture.h>

namespace CSBackend
{
    namespace OpenGL
    {
        CS_DEFINE_NAMEDTYPE(GLContextRestorer);
        
        //------------------------------------------------------------------------------
        GLContextRestorerUPtr GLContextRestorer::Create() noexcept
        {
            return GLContextRestorerUPtr(new GLContextRestorer());
        }
        //------------------------------------------------------------------------------
        bool GLContextRestorer::IsA(ChilliSource::InterfaceIDType interfaceId) const noexcept
        {
            return (GLContextRestorer::InterfaceID == interfaceId);
        }
        //------------------------------------------------------------------------------
        void GLContextRestorer::InvalidateResources() noexcept
        {
            if(m_hasContextBeenBackedUp == false)
            {
                m_hasContextBeenBackedUp = true;
                
                ChilliSource::ResourcePool* resourcePool = ChilliSource::Application::Get()->GetResourcePool();
                
                auto allShaders = resourcePool->GetAllResources<ChilliSource::Shader>();
                for (const auto& shader : allShaders)
                {
                    GLShader* glShader = static_cast<GLShader*>(shader->GetRenderShader()->GetExtraData());
                    if(glShader)
                    {
                        glShader->Invalidate();
                    }
                }
                
                auto allTextures = resourcePool->GetAllResources<ChilliSource::Texture>();
                for (const auto& texture : allTextures)
                {
                    GLTexture* glTexture = static_cast<GLTexture*>(texture->GetRenderTexture()->GetExtraData());
                    if(glTexture)
                    {
                        glTexture->Invalidate();
                    }
                }
                
                auto allCubemaps = resourcePool->GetAllResources<ChilliSource::Cubemap>();
                for (const auto& cubemap : allCubemaps)
                {
                    GLCubemap* glCubemap = static_cast<GLCubemap*>(cubemap->GetRenderTexture()->GetExtraData());
                    if(glCubemap)
                    {
                        glCubemap->Invalidate();
                    }
                }
                
                auto allModels = resourcePool->GetAllResources<ChilliSource::Model>();
                for (const auto& model : allModels)
                {
                    for(u32 i = 0; i < model->GetNumMeshes(); ++i)
                    {
                        GLMesh* glMesh = static_cast<GLMesh*>(model->GetRenderMesh(i)->GetExtraData());
                        
                        if(glMesh)
                        {
                            glMesh->Invalidate();
                        }
                    }
                }
                
                auto renderTargetGroupManager = ChilliSource::Application::Get()->GetSystem<ChilliSource::RenderTargetGroupManager>();
                for(const auto renderTargetGroup : renderTargetGroupManager->GetRenderTargetGroups())
                {
                    GLTargetGroup* targetGroup = static_cast<GLTargetGroup*>(renderTargetGroup->GetExtraData());
                    targetGroup->Invalidate();
                }
            }
        }
        //------------------------------------------------------------------------------
        void GLContextRestorer::RestoreResources() noexcept
        {
            if(m_hasContextBeenBackedUp == true)
            {
                ChilliSource::ResourcePool* resourcePool = ChilliSource::Application::Get()->GetResourcePool();
                
#if CS_ENABLE_DEBUG
                auto allShaders = resourcePool->GetAllResources<ChilliSource::Shader>();
                for (const auto& shader : allShaders)
                {
                	CS_ASSERT(shader->GetStorageLocation() != ChilliSource::StorageLocation::k_none, "Cannot restore Shader because restoration of OpenGL resources that were not loaded from file is not supported. To resolve this, manually release the resource on suspend and re-create it on resume.");
                }
#endif
                resourcePool->RefreshResources<ChilliSource::Shader>();
                
                auto allTextures = resourcePool->GetAllResources<ChilliSource::Texture>();
                for (const auto& texture : allTextures)
				{
                    if (texture->GetStorageLocation() == ChilliSource::StorageLocation::k_none)
                    {
                        ChilliSource::RestoreTextureRenderCommand command(texture->GetRenderTexture());
                        m_pendingRestoreTextureCommands.push_back(std::move(command));
                    }
                }
                resourcePool->RefreshResources<ChilliSource::Texture>();
                
                auto allCubemaps = resourcePool->GetAllResources<ChilliSource::Cubemap>();
                for (const auto& cubemap : allCubemaps)
                {
                    if (cubemap->GetStorageLocation() == ChilliSource::StorageLocation::k_none)
                    {
                        ChilliSource::RestoreCubemapRenderCommand command(cubemap->GetRenderTexture());
                        m_pendingRestoreCubemapCommands.push_back(std::move(command));
                    }
                }
                resourcePool->RefreshResources<ChilliSource::Cubemap>();
                
                auto allModels = resourcePool->GetAllResources<ChilliSource::Model>();
                for (const auto& model : allModels)
                {
                    if (model->GetStorageLocation() == ChilliSource::StorageLocation::k_none)
                    {
                        for(u32 i = 0; i < model->GetNumMeshes(); ++i)
                        {
                            ChilliSource::RestoreMeshRenderCommand command(model->GetRenderMesh(i));
                            m_pendingRestoreMeshCommands.push_back(std::move(command));
                        }
                    }
                }
                resourcePool->RefreshResources<ChilliSource::Model>();
                
                auto renderTargetGroupManager = ChilliSource::Application::Get()->GetSystem<ChilliSource::RenderTargetGroupManager>();
                for(const auto renderTargetGroup : renderTargetGroupManager->GetRenderTargetGroups())
                {
                    ChilliSource::RestoreRenderTargetGroupCommand command(renderTargetGroup);
                    m_pendingRestoreRenderTargetGroupCommands.push_back(std::move(command));
                }
                
                m_hasContextBeenBackedUp = false;
            }
        }
        //------------------------------------------------------------------------------
        void GLContextRestorer::OnResume() noexcept
        {
            if(m_initialised)
            {
#ifdef CS_TARGETPLATFORM_ANDROID
                RestoreResources();
#endif
            }
            
            m_initialised = true;
        }
        
        //------------------------------------------------------------------------------
        void GLContextRestorer::OnRenderSnapshot(ChilliSource::TargetType targetType, ChilliSource::RenderSnapshot& renderSnapshot, ChilliSource::IAllocator* frameAllocator) noexcept
        {
#ifdef CS_TARGETPLATFORM_ANDROID
            if(targetType == ChilliSource::TargetType::k_main)
            {
                auto preRenderCommandList = renderSnapshot.GetPreRenderCommandList();
                
                for(auto& restoreMeshCommand : m_pendingRestoreMeshCommands)
                {
                    preRenderCommandList->AddRestoreMeshCommand(restoreMeshCommand.GetRenderMesh());
                }
                
                for(auto& restoreTextureCommand : m_pendingRestoreTextureCommands)
                {
                    preRenderCommandList->AddRestoreTextureCommand(restoreTextureCommand.GetRenderTexture());
                }
                
                for(auto& restoreCubemapCommand : m_pendingRestoreCubemapCommands)
                {
                    preRenderCommandList->AddRestoreCubemapCommand(restoreCubemapCommand.GetRenderTexture());
                }
                
                for(auto& restoreRenderTargetGroupCommand : m_pendingRestoreRenderTargetGroupCommands)
                {
                    preRenderCommandList->AddRestoreRenderTargetGroupCommand(restoreRenderTargetGroupCommand.GetTargetRenderGroup());
                }
                
                m_pendingRestoreMeshCommands.clear();
                m_pendingRestoreTextureCommands.clear();
                m_pendingRestoreCubemapCommands.clear();
                m_pendingRestoreRenderTargetGroupCommands.clear();
            }
#endif
        }
        
        //------------------------------------------------------------------------------
        void GLContextRestorer::OnSystemSuspend() noexcept
        {
#ifdef CS_TARGETPLATFORM_ANDROID
            InvalidateResources();
#endif
        }
    }
}
