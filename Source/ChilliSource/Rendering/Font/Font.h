//
//  Font.h
//  Chilli Source
//
//  Created by Scott Downie on 26/10/2010.
//  Copyright (c) 2010 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_RENDERING_FONT_FONT_H_
#define _CHILLISOURCE_RENDERING_FONT_FONT_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Math/Geometry/Shapes.h>
#include <ChilliSource/Core/Resource/Resource.h>
#include <ChilliSource/Core/String/UTF8String.h>

namespace ChilliSource
{
	namespace Rendering
	{
        const Core::UTF8String::Char k_returnCharacter = '\n';
        const Core::UTF8String::Char k_tabCharacter = '\t';
        const Core::UTF8String::Char k_spaceCharacter = ' ';
        
        //The nbsp character has the hex code C2A0 which converts to
        //160 in decimal after utf-8 conversion
        const Core::UTF8String::Char k_nbspCharacter = 160;
        
        //---------------------------------------------------------------------
        /// The font resource describing the size, spacing and kerning of
        /// font characters.
        ///
        /// @author S Downie
        //---------------------------------------------------------------------
		class Font : public Core::Resource
		{
		public:
            
            typedef Core::UTF8String CharacterSet;
            
            //---------------------------------------------------------------------
            /// Holds the information relating to a single character such as
            /// width, height and UVs
            ///
            /// @author S Downie
            //---------------------------------------------------------------------
			struct CharacterInfo
			{
				Core::Rectangle m_UVs;
                Core::Vector2 m_size;
                Core::Vector2 m_offset;
			};
            //---------------------------------------------------------------------
            /// TODO: Find out how the kerning works and actually document it
            ///
            /// @author R Henning
            //---------------------------------------------------------------------
            struct KernLookup
			{
                //---------------------------------------------------------------------
                /// Constructor
                ///
                /// @author R Henning
                ///
                /// @param TODO
                /// @param TODO
                //---------------------------------------------------------------------
				KernLookup(s16 in_character, u16 in_start)
				: m_character(in_character), m_start(in_start), m_length(0)
				{
				}
				//---------------------------------------------------------------------
                /// @author R Henning
                ///
                /// @param Lookup value
                ///
                /// @return Whether this value is less than the one given
                //---------------------------------------------------------------------
				bool operator < (const KernLookup& in_lookup) const
				{
                    return m_character < in_lookup.m_character;
                }
                    
                s16 m_character;
                u16 m_start;
                u16 m_length;
            };
            //---------------------------------------------------------------------
            /// TODO: Find out how the kerning works and actually document it
            ///
            /// @author R Henning
            //---------------------------------------------------------------------
            struct KernPair
            {
                //---------------------------------------------------------------------
                /// Constructor
                ///
                /// @author R Henning
                ///
                /// @param TODO
                /// @param TODO
                //---------------------------------------------------------------------
                KernPair(s16 in_character, f32 in_spacing)
                : m_character(in_character)
                , m_spacing(in_spacing)
                {
                }
                //---------------------------------------------------------------------
                /// @author R Henning
                ///
                /// @param Pair value
                ///
                /// @return Whether this value is less than the one given
                //---------------------------------------------------------------------
                bool operator < (const KernPair& in_pair) const
                {
                    return m_character < in_pair.m_character;
                }
                
                f32 m_spacing;
                s16 m_character;
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
            /// @author S Downie
			///
			/// @param Font texture
            //---------------------------------------------------------------------
            void SetTexture(const TextureCSPtr& in_texture);
			//---------------------------------------------------------------------
			/// @author S Downie
			///
			/// @return Font texture 
			//---------------------------------------------------------------------
			const TextureCSPtr& GetTexture() const;
			//---------------------------------------------------------------------
			/// Each character is a sprite in a TextureAtlas. The TextureAtlas
            /// also holds the texture
            ///
            /// @author S Downie
			///
			/// @param Sprite data containing UV's, size, etc
			//---------------------------------------------------------------------
			void SetCharacterData(const TextureAtlasCSPtr& in_characterData);
			//---------------------------------------------------------------------
			/// @author S Downie
			///
			/// @return The height of a line as determined by the font
			//---------------------------------------------------------------------
			f32 GetLineHeight() const;
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
			bool TryGetCharacterInfo(Core::UTF8String::Char in_char, CharacterInfo& out_info) const;
			//---------------------------------------------------------------------
            /// @author S Downie
            ///
			/// @param The list of characters that are supported by this font
			//---------------------------------------------------------------------
			void SetCharacterSet(const CharacterSet& in_charSet);
            //---------------------------------------------------------------------
            /// Returns the amount that the glyph represented
            /// by inChar2 can be moved towards inChar1 without
            /// overlapping.
            /// This data is generated by the SpriteTool
            ///
            /// @author R Henning
            ///
            /// @param First character to test
            /// @param Second character to test
            ///
            /// @return Spacing between characters
            //---------------------------------------------------------------------
            f32 GetKerningBetweenCharacters(Core::UTF8String::Char in_char1, Core::UTF8String::Char in_char2) const;
            //---------------------------------------------------------------------
            /// Sets a value that will be used to offset all kerning values
            ///
            /// @author R Henning
            ///
            /// @param Offset
            //---------------------------------------------------------------------
            static void SetGlobalKerningOffset(f32 in_offset);
            //---------------------------------------------------------------------
            /// Sets kerning lookup and pair info
            ///
            /// @author R Henning
            ///
            /// @param Kerning lookup array
            /// @param Kerning pair array
            //---------------------------------------------------------------------
            void SetKerningInfo(const std::vector<KernLookup>& in_lookups, const std::vector<KernPair>& in_pairs);
		
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
            
			std::unordered_map<Core::UTF8String::Char, CharacterInfo> m_characterInfos;
			CharacterSet m_characters;
			
            std::vector<KernLookup> m_kerningLookups;
            std::vector<KernPair> m_kerningPairs;
            
            TextureCSPtr m_texture;
			TextureAtlasCSPtr m_textureAtlas;
            
            f32 m_lineHeight = 0.0f;
            
            static f32 s_globalKerningOffset;
		};
	}
}

#endif
