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
		const TextureAtlas::Frame& TextureAtlas::GetFrame(u32 in_frameIndex) const
		{
            CS_ASSERT(in_frameIndex < m_desc.m_frames.size(), "Texture frame index out of bounds");
			return m_desc.m_frames[in_frameIndex];
		}
        //---------------------------------------------------------------------
        //---------------------------------------------------------------------
        u32 TextureAtlas::GetFrameIndexById(const std::string& in_textureId) const
        {
            u32 hashedId = Core::HashCRC32::GenerateHashCode(in_textureId);
            for(u32 i=0; i<m_desc.m_keys.size(); ++i)
            {
                if(m_desc.m_keys[i] == hashedId)
                {
                    return i;
                }
            }
            
            CS_LOG_FATAL("Texture in atlas not found for key: " + in_textureId);
            return 0;
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
		Core::Rectangle TextureAtlas::GetFrameUVs(u32 in_frameIndex) const
        {
			Core::Rectangle result;
    
            const Frame& frame = m_desc.m_frames[in_frameIndex];
			
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
		Core::Vector2 TextureAtlas::GetFrameSize(u32 in_frameIndex) const
        {
			return Core::Vector2(m_desc.m_frames[in_frameIndex].m_width, m_desc.m_frames[in_frameIndex].m_height);
		}
		//---------------------------------------------------------------------
		//---------------------------------------------------------------------
		Core::Vector2 TextureAtlas::GetFrameOffset(u32 in_frameIndex) const
        {
            return Core::Vector2(m_desc.m_frames[in_frameIndex].m_offsetX, m_desc.m_frames[in_frameIndex].m_offsetY);
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
