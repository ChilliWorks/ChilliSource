//
//  TextureAtlas.cpp
//  Chilli Source
//
//  Created by Scott Downie on 22/10/2010.
//  Copyright 2010 Tag Games. All rights reserved.
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
            m_desc = in_desc;
        }
        //---------------------------------------------------------------------
        //---------------------------------------------------------------------
        const TextureAtlas::Frame& TextureAtlas::GetFrame(u32 in_index) const
        {
            CS_ASSERT(in_index < m_desc.m_frames.size(), "Index outside atlas frame bounds");
            return m_desc.m_frames[in_index];
        }
		//---------------------------------------------------------------------
		//---------------------------------------------------------------------
		const TextureAtlas::Frame& TextureAtlas::GetFrame(const std::string& in_textureId) const
		{
            u32 hashedId = Core::HashCRC32::GenerateHashCode(in_textureId);
            for(u32 i=0; i<m_desc.m_keys.size(); ++i)
            {
                if(m_desc.m_keys[i] == hashedId)
                {
                    return m_desc.m_frames[i];
                }
            }
            
            CS_LOG_FATAL("Texture in atlas not found for key: " + in_textureId);
            return m_desc.m_frames[0];
		}
        //---------------------------------------------------------------------
        //---------------------------------------------------------------------
        bool TextureAtlas::HasFrameWithId(const std::string& in_textureId) const
        {
            u32 hashedId = Core::HashCRC32::GenerateHashCode(in_textureId);
            for(u32 i=0; i<m_desc.m_keys.size(); ++i)
            {
                if(m_desc.m_keys[i] == hashedId)
                {
                    return true;
                }
            }
            
            return false;
        }	
		//---------------------------------------------------------------------
		//---------------------------------------------------------------------
		Core::Rectangle TextureAtlas::GetFrameUVs(const std::string& in_textureId) const
        {
			Core::Rectangle result;
    
            const Frame& frame = GetFrame(in_textureId);
			
            f32 inverseWidth = 1.0f / m_desc.m_textureAtlasWidth;
            f32 inverseHeight = 1.0f / m_desc.m_textureAtlasHeight;
            
			result.vOrigin.x = (f32)(frame.m_texCoordU + k_halfTexelOffset) * inverseWidth;
			result.vOrigin.y = (f32)(frame.m_texCoordV + k_halfTexelOffset) * inverseHeight;
			result.vSize.x = (f32)(frame.m_width - k_texelOffset) * inverseWidth;
			result.vSize.y = (f32)(frame.m_height - k_texelOffset) * inverseHeight;
            
            return result;
		}
		//---------------------------------------------------------------------
		//---------------------------------------------------------------------
		Core::Vector2Old TextureAtlas::GetFrameSize(const std::string& in_textureId) const
        {
            const Frame& frame = GetFrame(in_textureId);
			return Core::Vector2Old(frame.m_width, frame.m_height);
		}
		//---------------------------------------------------------------------
		//---------------------------------------------------------------------
		Core::Vector2Old TextureAtlas::GetFrameOffset(const std::string& in_textureId) const
        {
            const Frame& frame = GetFrame(in_textureId);
            return Core::Vector2Old(frame.m_offsetX, frame.m_offsetY);
		}
        //---------------------------------------------------------------------
        //---------------------------------------------------------------------
        u32 TextureAtlas::GetWidth() const
        {
            return m_desc.m_textureAtlasWidth;
        }
        //---------------------------------------------------------------------
        //---------------------------------------------------------------------
        u32 TextureAtlas::GetHeight() const
        {
            return m_desc.m_textureAtlasHeight;
        }
	}
}
