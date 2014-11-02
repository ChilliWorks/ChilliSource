//
//  Font.cpp
//  Chilli Source
//  Created by Scott Downie on 26/10/2010.
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

#include <ChilliSource/Rendering/Font/Font.h>

#include <ChilliSource/Rendering/Texture/TextureAtlas.h>

#include <algorithm>

namespace ChilliSource
{
	namespace Rendering
	{
        namespace
        {
            const u32 k_spacesPerTab = 5;
        }
        
        CS_DEFINE_NAMEDTYPE(Font);
        
        //---------------------------------------------------------------------
        //---------------------------------------------------------------------
        FontUPtr Font::Create()
        {
            return FontUPtr(new Font());
        }
		//---------------------------------------------------------------------
		//---------------------------------------------------------------------
		bool Font::IsA(Core::InterfaceIDType in_interfaceId) const
		{
			return in_interfaceId == Font::InterfaceID;
		}
        //-------------------------------------------
		//-------------------------------------------
		void Font::Build(const Descriptor& in_desc)
		{
            CS_ASSERT(in_desc.m_supportedCharacters.size() > 0, "Font: Cannot build characters with empty character set");
            
            m_characterInfos.clear();
            m_characters = in_desc.m_supportedCharacters;
            m_texture = in_desc.m_texture;
			
            const f32 textureAtlasWidth = (f32)in_desc.m_textureAtlasWidth;
            const f32 textureAtlasHeight = (f32)in_desc.m_textureAtlasHeight;
            
            u32 frameIdx = 0;
            
            auto it = m_characters.begin();
            while(it < m_characters.end())
            {
                auto character = Core::UTF8StringUtils::Next(it);
                
				CharacterInfo info;
				
				const Frame& frame = in_desc.m_frames[frameIdx];
                
				info.m_UVs.m_u = (f32)(frame.m_texCoordU - 0.5f) / textureAtlasWidth;
				info.m_UVs.m_v = (f32)(frame.m_texCoordV - 0.5f) / textureAtlasHeight;
				info.m_UVs.m_s = (f32)(frame.m_width + 1.0f) / textureAtlasWidth;
				info.m_UVs.m_t = (f32)(frame.m_height + 1.0f) / textureAtlasHeight;
				
				info.m_size.x = frame.m_width;
                info.m_size.y = frame.m_height;
                
				info.m_offset.x = frame.m_offsetX;
				info.m_offset.y = frame.m_offsetY;
                
                info.m_origin = frame.m_origin;
                info.m_advance = frame.m_advance;
                
                m_lineHeight = std::max((f32)frame.m_height, m_lineHeight);
                
				m_characterInfos.insert(std::make_pair(character, info));
                
                ++frameIdx;
			}
            
            m_pointSize = in_desc.m_pointSize;
            
            if(in_desc.m_lineHeight > 0)
            {
                m_lineHeight = (f32)in_desc.m_lineHeight;
            }
            
            m_descent = f32(in_desc.m_descent);
            m_verticalPadding = f32(in_desc.m_verticalPadding);
			
			//Just assign the width of a whitespaces based on the similar space character in the
			//font. This means it will scale relative to the font
            CharacterInfo spaceCharacter;
            spaceCharacter.m_advance = in_desc.m_spaceAdvance;
            
            //Space
            m_characterInfos.insert(std::make_pair(k_spaceCharacter, spaceCharacter));
            
            //Non-breaking space
            m_characterInfos.insert(std::make_pair(k_nbspCharacter, spaceCharacter));
            
            //Tab
            spaceCharacter.m_size.x *= k_spacesPerTab;
            m_characterInfos.insert(std::make_pair(k_tabCharacter, spaceCharacter));
            
            //Return
            m_characterInfos.insert(std::make_pair(k_returnCharacter, CharacterInfo()));
        }
		//-------------------------------------------
		//-------------------------------------------
		const TextureCSPtr& Font::GetTexture() const
		{
			return m_texture;
		}
        //---------------------------------------------------------------------
        //---------------------------------------------------------------------
        u32 Font::GetPointSize() const
        {
            return m_pointSize;
        }
		//-------------------------------------------
		//-------------------------------------------
		f32 Font::GetLineHeight() const
		{
			return m_lineHeight;
        }
        //---------------------------------------------------------------------
        //---------------------------------------------------------------------
        f32 Font::GetDescent() const
        {
            return m_descent;
        }
        //---------------------------------------------------------------------
        //---------------------------------------------------------------------
        f32 Font::GetVerticalPadding() const
        {
            return m_verticalPadding;
        }
        //-------------------------------------------
        //-------------------------------------------
        bool Font::SupportsKerning() const
        {
            return false;
        }
		//-------------------------------------------
		//-------------------------------------------
		bool Font::TryGetCharacterInfo(Core::UTF8Char in_char, CharacterInfo& out_info) const
		{
			auto itCharEntry = m_characterInfos.find(in_char);
			
			if (itCharEntry != m_characterInfos.end())
            {
				out_info = itCharEntry->second;
				return true;
			}
			
			return false;
		}
	}
}
