//
//  VerticalStretchableImage.h
//  Chilli Source
//  Created by Nicolas Tanda on 22/06/2012
//
//  The MIT License (MIT)
//
//  Copyright (c) 2012 Tag Games Limited
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

#ifndef _CHILLISOURCE_GUI_VerticalStretchableImageVIEW_H_
#define _CHILLISOURCE_GUI_VerticalStretchableImageVIEW_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Math/Geometry/Shapes.h>
#include <ChilliSource/GUI/Base/GUIView.h>
#include <ChilliSource/Rendering/Texture/UVs.h>
#include <ChilliSource/Rendering/Texture/TextureAtlas.h>

//=============================================================
/// GUI - Stretchable Image 
///
/// Creates a scalable image using the nine patch system
//=============================================================

namespace ChilliSource
{
    namespace GUI
    {
        class VerticalStretchableImage : public GUIView
        {
        public:

			DECLARE_META_CLASS(VerticalStretchableImage)

            VerticalStretchableImage();
            VerticalStretchableImage(const Core::ParamDictionary& insParams);
            //---------------------------------------------------------
            /// Set Texture
            ///
            /// @param Texture containing the nine patches
            //---------------------------------------------------------
            void SetTexture(const Rendering::TextureCSPtr& inpTexture);
            //---------------------------------------------------------
            /// Get Texture
            ///
            /// @return Texture containing the nine patches
            //---------------------------------------------------------
            const Rendering::TextureCSPtr& GetTexture() const;
            //---------------------------------------------------------
            /// Set Sprite Sheet
            ///
            /// @param Sprite sheet containing the nine patches
            //---------------------------------------------------------
            void SetTextureAtlas(const Rendering::TextureAtlasCSPtr& inpTextureAtlas);
            //---------------------------------------------------------
            /// Get Sprite Sheet
            ///
            /// @return Sprite sheet containing the nine patches
            //---------------------------------------------------------
			const Rendering::TextureAtlasCSPtr& GetTextureAtlas() const;
			//---------------------------------------------------------
			/// Set Base Sprite Sheet Index ID
			///
			/// Set the "path" to the sprite sheet index IDs. 
			/// e.g. For the following IDs: 
			///			* "BluePanelTop"
			///			* "BluePanelMiddle"
			///			* "BluePanelBottom"
			///
			/// the base ID would be "BluePanel"
			//---------------------------------------------------------
			void SetBaseTextureAtlasID(const std::string& instrID);
			//---------------------------------------------------------
			/// Get Base Sprite Sheet Index ID
			///
			/// Get the "path" to the sprite sheet index IDs. 
			/// e.g. For the following IDs: 
			///			* "BluePanelTop"
			///			* "BluePanelMiddle"
			///			* "BluePanelBottom"
			///
			/// the base ID would be "BluePanel"
			//---------------------------------------------------------
			const std::string& GetBaseTextureAtlasID() const;
			//---------------------------------------------------------
			/// Set Patch Sprite Sheet Indices
			///
			/// @param Sprite sheet id of top patch
			/// @param Sprite sheet id of middle patch
			/// @param Sprite sheet id of bottom patch
			//---------------------------------------------------------
			void SetTextureAtlasIds(const std::string& in_top, const std::string& in_middle, const std::string& in_bottom);
            //---------------------------------------------------------
            /// Draw
            /// 
            /// Draw the image constructed from the nine patches
            ///
            /// @param Canvas renderer pointer
            //---------------------------------------------------------
            void Draw(Rendering::CanvasRenderer* inpCanvas);
            //--------------------------------------------------------
            /// Get Combined Cap Height
            ///
            /// @param Sum of the widths of both end caps
            //--------------------------------------------------------
            f32 GetCombinedCapHeight() const;
            //--------------------------------------------------------
            /// Get Combined Cap Width
            ///
            /// @param Sum of the heights of both end caps
            //--------------------------------------------------------
            f32 GetCapWidth() const;
			
        private:
            
            enum class Patch
            {
                k_top,
                k_middle,
                k_bottom,
                k_total
            };
            
            Rendering::TextureAtlas::Frame m_frames[(u32)Patch::k_total];

            DECLARE_PROPERTY_A(Rendering::TextureCSPtr, Texture, SetTexture, GetTexture);
			DECLARE_PROPERTY_A(Rendering::TextureAtlasCSPtr, TextureAtlas, SetTextureAtlas, GetTextureAtlas);

			DECLARE_PROPERTY_A(std::string, BaseTextureAtlasID, SetBaseTextureAtlasID, GetBaseTextureAtlasID);
            DECLARE_PROPERTY_A(bool, ActAsSpacer, EnableActAsSpacer, IsActAsSpacerEnabled);
	
        };
    }
}

#endif