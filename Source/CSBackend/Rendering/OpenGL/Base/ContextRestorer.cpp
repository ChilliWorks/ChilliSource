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

#ifdef CS_TARGETPLATFORM_ANDROID

#include <CSBackend/Rendering/OpenGL/Base/ContextRestorer.h>

#include <CSBackend/Rendering/OpenGL/Base/MeshBuffer.h>
#include <CSBackend/Rendering/OpenGL/Texture/Cubemap.h>
#include <CSBackend/Rendering/OpenGL/Texture/Texture.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Resource/ResourcePool.h>
#include <ChilliSource/Rendering/Shader/Shader.h>

namespace CSBackend
{
    namespace OpenGL
    {
        //-----------------------------------------------------
        /// Shaders and most textures can be recreated from file,
        /// however the mesh buffers must have a snap shot taken
        //-----------------------------------------------------
        void ContextRestorer::Backup()
        {
            if(m_hasContextBeenBackedUp == false)
            {
                for(auto& buffer : m_meshBuffers)
                {
                    buffer->Backup();
                }
                
                m_hasContextBeenBackedUp = true;
            }
        }
        //-----------------------------------------------------
        /// Rebuild the shaders and textures from file. Re-upload
        /// the mesh buffers
        //-----------------------------------------------------
        void ContextRestorer::Restore()
        {
            if(m_hasContextBeenBackedUp == true)
            {
                CSCore::ResourcePool* resourcePool = CSCore::Application::Get()->GetResourcePool();
                
                //---Shaders
                auto allShaders = resourcePool->GetAllResources<CSRendering::Shader>();
                for (const auto& shader : allShaders)
                {
                	CS_ASSERT(shader->GetStorageLocation() != CSCore::StorageLocation::k_none, "Cannot restore Shader because restoration of OpenGL resources that were not loaded from file is not supported. To resolve this, manually release the resource on suspend and re-create it on resume.");
                }
                resourcePool->RefreshResources<CSRendering::Shader>();
                
                //---Textures
                auto allTextures = resourcePool->GetAllResources<CSRendering::Texture>();
                for (const auto& texture : allTextures)
				{
                    if (texture->GetStorageLocation() == CSCore::StorageLocation::k_none)
                    {
                        Texture* glTexture = static_cast<Texture*>(const_cast<CSRendering::Texture*>(texture.get()));
                        glTexture->Restore();
                    }
                }
                resourcePool->RefreshResources<CSRendering::Texture>();
                
                //---Cubemaps
                auto allCubemaps = resourcePool->GetAllResources<CSRendering::Cubemap>();
                for (const auto& cubemap : allCubemaps)
				{
					if (cubemap->GetStorageLocation() == CSCore::StorageLocation::k_none)
                    {
                        Cubemap* glCubemap = static_cast<Cubemap*>(const_cast<CSRendering::Cubemap*>(cubemap.get()));
                        glCubemap->Restore();
                    }
                }
                resourcePool->RefreshResources<CSRendering::Cubemap>();
                
                //---Meshes
                for(auto& buffer : m_meshBuffers)
                {
                    buffer->Restore();
                }
                
                m_hasContextBeenBackedUp = false;
            }
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        void ContextRestorer::AddMeshBuffer(MeshBuffer* in_meshBuffer)
        {
            m_meshBuffers.push_back(in_meshBuffer);
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        void ContextRestorer::RemoveMeshBuffer(MeshBuffer* in_meshBuffer)
        {
            for(u32 i=0; i<m_meshBuffers.size(); ++i)
            {
                if(m_meshBuffers[i] == in_meshBuffer)
                {
                    m_meshBuffers[i] = m_meshBuffers.back();
                    m_meshBuffers.pop_back();
                    return;
                }
            }
        }
    }
}

#endif
