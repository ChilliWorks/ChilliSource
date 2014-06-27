//
//  TextureAtlas.cpp
//  Chilli Source
//  Created by Scott Downie on 22/10/2010.
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

#include <ChilliSource/Rendering/Texture/TextureAtlas.h>

#include <ChilliSource/Core/Cryptographic/HashCRC32.h>

namespace ChilliSource
{
	namespace Rendering
	{
        namespace
        {
            const f32 k_texelOffset = 1.0f;
            const f32 k_halfTexelOffset = k_texelOffset * 0.5f;
        }
        
        CS_DEFINE_NAMEDTYPE(TextureAtlas);
        
        //---------------------------------------------------------------------
        //---------------------------------------------------------------------
        TextureAtlasUPtr TextureAtlas::Create()
        {
            return TextureAtlasUPtr(new TextureAtlas());
        }
		//---------------------------------------------------------------------
		//---------------------------------------------------------------------
		bool TextureAtlas::IsA(Core::InterfaceIDType in_interfaceId) const
		{
			return in_interfaceId == TextureAtlas::InterfaceID;
		}
        //---------------------------------------------------------------------
        //---------------------------------------------------------------------
        void TextureAtlas::Build(const Descriptor& in_desc)
        {
            m_textureAtlasWidth = in_desc.m_textureAtlasWidth;
            m_textureAtlasHeight = in_desc.m_textureAtlasHeight;
            
            for(u32 i=0; i<in_desc.m_keys.size(); ++i)
            {
                Frame frame;
                const FrameRaw& frameRaw = in_desc.m_frames[i];
                
                //Calculate normalise UVs
                f32 inverseWidth = 1.0f / m_textureAtlasWidth;
                f32 inverseHeight = 1.0f / m_textureAtlasHeight;
                frame.m_uvs.m_u = (f32)(frameRaw.m_texCoordU + k_halfTexelOffset) * inverseWidth;
                frame.m_uvs.m_v = (f32)(frameRaw.m_texCoordV + k_halfTexelOffset) * inverseHeight;
                frame.m_uvs.m_s = (f32)(frameRaw.m_croppedWidth - k_texelOffset) * inverseWidth;
                frame.m_uvs.m_t = (f32)(frameRaw.m_croppedHeight - k_texelOffset) * inverseHeight;
                
                frame.m_originalSize = Core::Vector2(frameRaw.m_originalWidth, frameRaw.m_originalHeight);
                frame.m_croppedSize = Core::Vector2(frameRaw.m_croppedWidth, frameRaw.m_croppedHeight);
                frame.m_offset = Core::Vector2(frameRaw.m_offsetX, frameRaw.m_offsetY);
                
                m_frames.insert(std::make_pair(in_desc.m_keys[i], frame));
            }
        }
		//---------------------------------------------------------------------
		//---------------------------------------------------------------------
		const TextureAtlas::Frame& TextureAtlas::GetFrame(const std::string& in_textureId) const
		{
            u32 hashedId = Core::HashCRC32::GenerateHashCode(in_textureId);
            
            auto it = m_frames.find(hashedId);
            
            CS_ASSERT(it != m_frames.end(), "Texture in atlas not found for key: " + in_textureId);
            
            return it->second;
		}
        //---------------------------------------------------------------------
        //---------------------------------------------------------------------
        bool TextureAtlas::HasFrameWithId(const std::string& in_textureId) const
        {
            u32 hashedId = Core::HashCRC32::GenerateHashCode(in_textureId);
            
            return HasFrameWithId(hashedId);
        }	
		//---------------------------------------------------------------------
		//---------------------------------------------------------------------
		Rendering::UVs TextureAtlas::GetFrameUVs(const std::string& in_textureId) const
        {
            const Frame& frame = GetFrame(in_textureId);
            return frame.m_uvs;
		}
		//---------------------------------------------------------------------
		//---------------------------------------------------------------------
		Core::Vector2 TextureAtlas::GetCroppedFrameSize(const std::string& in_textureId) const
        {
            const Frame& frame = GetFrame(in_textureId);
			return frame.m_croppedSize;
		}
        //---------------------------------------------------------------------
        //---------------------------------------------------------------------
        Core::Vector2 TextureAtlas::GetOriginalFrameSize(const std::string& in_textureId) const
        {
            const Frame& frame = GetFrame(in_textureId);
			return frame.m_originalSize;
        }
		//---------------------------------------------------------------------
		//---------------------------------------------------------------------
		Core::Vector2 TextureAtlas::GetFrameOffset(const std::string& in_textureId) const
        {
            const Frame& frame = GetFrame(in_textureId);
            return frame.m_offset;
		}
        //---------------------------------------------------------------------
        //---------------------------------------------------------------------
        const TextureAtlas::Frame& TextureAtlas::GetFrame(u32 in_hashedTextureId) const
        {
            auto it = m_frames.find(in_hashedTextureId);
            
            CS_ASSERT(it != m_frames.end(), "Texture in atlas not found for hashed key: " + Core::ToString(in_hashedTextureId));
            
            return it->second;
        }
        //---------------------------------------------------------------------
        //---------------------------------------------------------------------
        bool TextureAtlas::HasFrameWithId(u32 in_hashedTextureId) const
        {
            auto it = m_frames.find(in_hashedTextureId);
            return (it != m_frames.end());
        }
        //---------------------------------------------------------------------
        //---------------------------------------------------------------------
        Rendering::UVs TextureAtlas::GetFrameUVs(u32 in_hashedTextureId) const
        {
            const Frame& frame = GetFrame(in_hashedTextureId);
            return frame.m_uvs;
        }
        //---------------------------------------------------------------------
        //---------------------------------------------------------------------
        Core::Vector2 TextureAtlas::GetCroppedFrameSize(u32 in_hashedTextureId) const
        {
            const Frame& frame = GetFrame(in_hashedTextureId);
			return frame.m_croppedSize;
        }
        //---------------------------------------------------------------------
        //---------------------------------------------------------------------
        Core::Vector2 TextureAtlas::GetOriginalFrameSize(u32 in_hashedTextureId) const
        {
            const Frame& frame = GetFrame(in_hashedTextureId);
			return frame.m_originalSize;
        }
        //---------------------------------------------------------------------
        //---------------------------------------------------------------------
        Core::Vector2 TextureAtlas::GetFrameOffset(u32 in_hashedTextureId) const
        {
            const Frame& frame = GetFrame(in_hashedTextureId);
            return frame.m_offset;
        }
        //---------------------------------------------------------------------
        //---------------------------------------------------------------------
        u32 TextureAtlas::GetWidth() const
        {
            return m_textureAtlasWidth;
        }
        //---------------------------------------------------------------------
        //---------------------------------------------------------------------
        u32 TextureAtlas::GetHeight() const
        {
            return m_textureAtlasHeight;
        }
	}
}
