//
//  Font.h
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

#ifndef _CHILLISOURCE_RENDERING_FONT_FONT_H_
#define _CHILLISOURCE_RENDERING_FONT_FONT_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Math/Geometry/Shapes.h>
#include <ChilliSource/Core/Resource/Resource.h>
#include <ChilliSource/Core/String/UTF8StringUtils.h>
#include <ChilliSource/Rendering/Texture/UVs.h>

#include <unordered_map>

namespace ChilliSource
{
	namespace Rendering
	{
        const Core::UTF8Char k_returnCharacter = '\n';
        const Core::UTF8Char k_tabCharacter = '\t';
        const Core::UTF8Char k_spaceCharacter = ' ';
        
        //The nbsp character has the hex code C2A0 which converts to
        //160 in decimal after utf-8 conversion
        const Core::UTF8Char k_nbspCharacter = 160;
        
        //---------------------------------------------------------------------
        /// The font resource describing the size, spacing and kerning of
        /// font characters.
        ///
        /// @author S Downie
        //---------------------------------------------------------------------
		class Font final : public Core::Resource
		{
		public:
            
            typedef std::string CharacterSet;
            
            //---------------------------------------------------------------------
            /// Holds the information relating to a single character such as
            /// width, height and UVs
            ///
            /// @author S Downie
            //---------------------------------------------------------------------
			struct CharacterInfo
			{
				Rendering::UVs m_UVs;
                Core::Vector2 m_size;
                Core::Vector2 m_offset;
                f32 m_origin = 0.0f;
                f32 m_advance = 0.0f;
			};
            //---------------------------------------------------------------------
            /// Holds the description of a single character frame packed into
            /// the font. This includes the size, UVs, etc
            ///
            /// @author S Downie
            //---------------------------------------------------------------------
			struct Frame
			{
				s16 m_texCoordU;
				s16 m_texCoordV;
				s16 m_width;
				s16 m_height;
				s16 m_offsetX;
				s16 m_offsetY;
                s16 m_originalWidth;
                s16 m_originalHeight;
                s16 m_origin;
                s16 m_advance;
			};
            //---------------------------------------------------------------------
            /// Holds the description of a font. Used to build the resource
            ///
            /// @author S Downie
            //---------------------------------------------------------------------
            struct Descriptor
            {
                std::vector<Frame> m_frames;
                std::string m_supportedCharacters;
                
                TextureCSPtr m_texture;
                
                u32 m_textureAtlasWidth = 0;
                u32 m_textureAtlasHeight = 0;
                u32 m_pointSize = 0;
                u32 m_lineHeight = 0;
                u32 m_descent = 0;
                u32 m_spaceAdvance = 0;
                u32 m_verticalPadding = 0;
            };
			
			CS_DECLARE_NAMEDTYPE(Font);
			//---------------------------------------------------------------------
			/// @author S Downie
            ///
            /// @param Interface ID
			///
			/// @return Whether this object is of given type
			//---------------------------------------------------------------------
			bool IsA(Core::InterfaceIDType in_interfaceId) const override;
            //---------------------------------------------------------------------
			/// Build the font from the given description
            ///
            /// @author S Downie
			///
			/// @param Description containing texture, UVs and supported characters
			//---------------------------------------------------------------------
			void Build(const Descriptor& in_desc);
			//---------------------------------------------------------------------
			/// @author S Downie
			///
			/// @return Font texture 
			//---------------------------------------------------------------------
			const TextureCSPtr& GetTexture() const;
            //---------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The point size of the font the bitmap font was created from.
            //---------------------------------------------------------------------
            u32 GetPointSize() const;
			//---------------------------------------------------------------------
			/// @author S Downie
			///
			/// @return The height of a line as determined by the font
			//---------------------------------------------------------------------
			f32 GetLineHeight() const;
            //---------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The amount of space below the base line in the font.
            //---------------------------------------------------------------------
            f32 GetDescent() const;
            //---------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The amount of padding added to the top and bottom of a
            /// glyph image.
            //---------------------------------------------------------------------
            f32 GetVerticalPadding() const;
            //---------------------------------------------------------------------
            /// @author R Henning
            ///
            /// @return True is kerning supported, false otherwise
            //---------------------------------------------------------------------
            bool SupportsKerning() const;
            //---------------------------------------------------------------------
			/// @author S McGaw
			///
			/// @param UTF-8 Character
			/// @param [Out] Info struct to be filled with data for the character
            ///
			/// @return Whether the character exists in the font
			//---------------------------------------------------------------------
			bool TryGetCharacterInfo(Core::UTF8Char in_char, CharacterInfo& out_info) const;
		
        private:
            
            friend class Core::ResourcePool;
            //---------------------------------------------------------------------
            /// Factory method to create empty font resource. Used only by the
            /// resource pool.
            ///
            /// @author S Downie
            //---------------------------------------------------------------------
            static FontUPtr Create();
			//---------------------------------------------------------------------
            /// Private constructor to enforce the use of factory method
            ///
            /// @author S Downie
            //---------------------------------------------------------------------
            Font() = default;
            
        private:
            
			std::unordered_map<Core::UTF8Char, CharacterInfo> m_characterInfos;
			CharacterSet m_characters;
            
            TextureCSPtr m_texture;
            
            u32 m_pointSize = 0;
            f32 m_lineHeight = 0.0f;
            f32 m_descent = 0.0f;
            f32 m_verticalPadding;
            
            static f32 s_globalKerningOffset;
		};
	}
}

#endif
