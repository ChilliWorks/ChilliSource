//
//  ContextRestorer.cpp
//  Chilli Source
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
#include <ChilliSource/Rendering/Material/Material.h>
#include <ChilliSource/Rendering/Model/Model.h>
#include <ChilliSource/Rendering/Model/RenderMesh.h>
#include <ChilliSource/Rendering/Shader/RenderShader.h>
#include <ChilliSource/Rendering/Shader/Shader.h>
#include <ChilliSource/Rendering/Texture/RenderTexture.h>
#include <ChilliSource/Rendering/Texture/Texture.h>

#include <CSBackend/Rendering/OpenGL/Model/GLMesh.h>
#include <CSBackend/Rendering/OpenGL/Shader/GLShader.h>
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
        void GLContextRestorer::OnContextLost() noexcept
        {
            if(m_hasContextBeenBackedUp == false)
            {
                m_hasContextBeenBackedUp = true;
                
                ChilliSource::ResourcePool* resourcePool = ChilliSource::Application::Get()->GetResourcePool();
                
                auto allShaders = resourcePool->GetAllResources<ChilliSource::Shader>();
                for (const auto& shader : allShaders)
                {
                    GLShader* glShader = reinterpret_cast<GLShader*>(shader->GetRenderShader()->GetExtraData());
                    if(glShader)
                    {
                        glShader->InvalidateContext();
                    }
                }
                
                auto allTextures = resourcePool->GetAllResources<ChilliSource::Texture>();
                for (const auto& texture : allTextures)
                {
                    GLTexture* glTexture = reinterpret_cast<GLTexture*>(texture->GetRenderTexture()->GetExtraData());
                    if(glTexture)
                    {
                        glTexture->InvalidateContext();
                    }
                }
                
                auto allModels = resourcePool->GetAllResources<ChilliSource::Model>();
                for (const auto& model : allModels)
                {
                    for(u32 i = 0; i < model->GetNumMeshes(); ++i)
                    {
                        GLMesh* glMesh = reinterpret_cast<GLMesh*>(model->GetRenderMesh(i)->GetExtraData());
                        
                        if(glMesh)
                        {
                            glMesh->InvalidateContext();
                        }
                    }
                }
            }
        }
        //------------------------------------------------------------------------------
        void GLContextRestorer::OnContextRestored() noexcept
        {
            if(m_hasContextBeenBackedUp == true)
            {
                ChilliSource::ResourcePool* resourcePool = ChilliSource::Application::Get()->GetResourcePool();
                
                auto allShaders = resourcePool->GetAllResources<ChilliSource::Shader>();
                for (const auto& shader : allShaders)
                {
                	CS_ASSERT(shader->GetStorageLocation() != ChilliSource::StorageLocation::k_none, "Cannot restore Shader because restoration of OpenGL resources that were not loaded from file is not supported. To resolve this, manually release the resource on suspend and re-create it on resume.");
                }
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
                
                //Iterate the materials and 'dirty' them so they are rebuilt
                auto allMaterials = resourcePool->GetAllResources<ChilliSource::Material>();
                for (auto& material : allMaterials)
                {
                    //TODO: Remove const cast when material system is improved.
                    const_cast<ChilliSource::Material*>(material.get())->SetAmbient(material->GetAmbient());
                }
                
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
                
                m_hasContextBeenBackedUp = false;
            }
        }
        //------------------------------------------------------------------------------
        void GLContextRestorer::OnResume() noexcept
        {
            if(m_initialised)
            {
#ifdef CS_TARGETPLATFORM_ANDROID
                OnContextRestored();
#endif
            }
            
            m_initialised = true;
        }
        
        //------------------------------------------------------------------------------
        void GLContextRestorer::OnRenderSnapshot(ChilliSource::RenderSnapshot& renderSnapshot) noexcept
        {
#ifdef CS_TARGETPLATFORM_ANDROID
            auto preRenderCommandList = renderSnapshot.GetPreRenderCommandList();
            auto postRenderCommandList = renderSnapshot.GetPostRenderCommandList();
            
            for(auto& restoreMeshCommand : m_pendingRestoreMeshCommands)
            {
                preRenderCommandList->AddRestoreMeshCommand(restoreMeshCommand.GetRenderMesh());
            }
            
            for(auto& restoreTextureCommand : m_pendingRestoreTextureCommands)
            {
                preRenderCommandList->AddRestoreTextureCommand(restoreTextureCommand.GetRenderTexture());
            }
            
            m_pendingRestoreMeshCommands.clear();
            m_pendingRestoreTextureCommands.clear();
#endif
        }
        
        //------------------------------------------------------------------------------
        void GLContextRestorer::OnSystemSuspend() noexcept
        {
#ifdef CS_TARGETPLATFORM_ANDROID
            OnContextLost();
#endif
        }
    }
}
