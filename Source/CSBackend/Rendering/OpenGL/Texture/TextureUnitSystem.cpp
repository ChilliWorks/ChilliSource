//
//  TextureUnitSystem.cpp
//  Chilli Source
//  Created by Scott Downie on 15/04/2014.
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

#include <CSBackend/Rendering/OpenGL/Texture/TextureUnitSystem.h>

#include <CSBackend/Rendering/OpenGL/Base/GLError.h>
#include <CSBackend/Rendering/OpenGL/Texture/Cubemap.h>
#include <CSBackend/Rendering/OpenGL/Texture/Texture.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Rendering/Base/RenderCapabilities.h>

#include <algorithm>

namespace CSBackend
{
	namespace OpenGL
	{
        CS_DEFINE_NAMEDTYPE(TextureUnitSystem);
        //----------------------------------------------------
        //----------------------------------------------------
        TextureUnitSystemUPtr TextureUnitSystem::Create()
        {
            return TextureUnitSystemUPtr(new TextureUnitSystem());
        }
        //--------------------------------------------------
        //--------------------------------------------------
        bool TextureUnitSystem::IsA(CSCore::InterfaceIDType in_interfaceId) const
        {
            return in_interfaceId == TextureUnitSystem::InterfaceID;
        }
        //---------------------------------------------------
        //---------------------------------------------------
        void TextureUnitSystem::Bind(const Texture* in_texture, u32 in_unit)
        {
            Bind(GL_TEXTURE_2D, (const void*)in_texture, in_texture->GetTextureHandle(), in_unit);
        }
        //---------------------------------------------------
        //---------------------------------------------------
        void TextureUnitSystem::Bind(const Cubemap* in_cubemap, u32 in_unit)
        {
            Bind(GL_TEXTURE_CUBE_MAP, (const void*)in_cubemap, in_cubemap->GetCubemapHandle(), in_unit);
        }
        //---------------------------------------------------
        //---------------------------------------------------
        void TextureUnitSystem::Bind(GLenum in_type, const void* in_object, GLint in_handle, u32 in_unit)
        {
            if((s32)in_unit != m_currentTextureUnit)
            {
                m_currentTextureUnit = (s32)in_unit;
                glActiveTexture(GL_TEXTURE0 + in_unit);
            }
            
            if(m_texUnits.empty() == true)
			{
                CSRendering::RenderCapabilities* renderCapabilities = CSCore::Application::Get()->GetSystem<CSRendering::RenderCapabilities>();
                CS_ASSERT(renderCapabilities, "Cannot find required system: Render Capabilities.");
                
                //Create the available texture unit slots
                m_texUnits.resize(renderCapabilities->GetNumTextureUnits());
                std::fill(m_texUnits.begin(), m_texUnits.end(), nullptr);
			}
            
            //Check to make sure this texture is not already bound to this unit
            if(m_texUnits[in_unit] != in_object)
			{
                glBindTexture(in_type, in_handle);
                m_texUnits[in_unit] = in_object;
            }
            
            CS_ASSERT_NOGLERROR("An OpenGL error occurred while binding texture to unit.");
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        void TextureUnitSystem::Unbind(const Texture* in_texture)
        {
            Unbind((const void*)in_texture);
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        void TextureUnitSystem::Unbind(const Cubemap* in_cubemap)
        {
            Unbind((const void*)in_cubemap);
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        void TextureUnitSystem::Unbind(const void* in_object)
        {
            for(u32 i=0; i<m_texUnits.size(); ++i)
            {
                if(m_texUnits[i] == in_object)
                {
                    //Unbind the texture from this slot by binding zero.
                    if((s32)i != m_currentTextureUnit)
                    {
                        m_currentTextureUnit = (s32)i;
                        glActiveTexture(GL_TEXTURE0 + i);
                    }
                    glBindTexture(GL_TEXTURE_2D, 0);
                    m_texUnits[i] = nullptr;
                }
            }
            
            CS_ASSERT_NOGLERROR("An OpenGL error occurred while unbinding texture.");
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        void TextureUnitSystem::OnDestroy()
        {
            Clear();
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        void TextureUnitSystem::Clear()
        {
            m_texUnits.clear();
            m_currentTextureUnit = -1;
        }
	}
}
