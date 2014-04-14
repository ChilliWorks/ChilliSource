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

#include <ChilliSource/Backend/Rendering/OpenGL/Base/ContextRestorer.h>

#include <ChilliSource/Backend/Rendering/OpenGL/Base/MeshBuffer.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Resource/ResourcePool.h>
#include <ChilliSource/Rendering/Shader/Shader.h>

namespace ChilliSource
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
                for(auto buffer : m_meshBuffers)
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
                Core::ResourcePool* resourcePool = Core::Application::Get()->GetResourcePool();
                
                //---Shaders
                resourcePool->RefreshResources<Rendering::Shader>();
                
                //---Textures
                //TODO: Once they have been converted to pooled resources
                
                //---Cubemaps
                //TODO: Once they have been converted to pooled resources
                
                //---Meshes
                for(auto buffer : m_meshBuffers)
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
