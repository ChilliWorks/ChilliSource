//
//  HorizontalStretchableImage.h
//  Chilli Source
//  Created by Stuart McGaw on 04/05/2011.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2011 Tag Games Limited
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

#ifndef _CHILLISOURCE_GUI_DEFAULTS_HORIZONTALSTRETCHABLEIMAGE_H_
#define _CHILLISOURCE_GUI_DEFAULTS_HORIZONTALSTRETCHABLEIMAGE_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Math/Geometry/Shapes.h>
#include <ChilliSource/GUI/Base/GUIView.h>
#include <ChilliSource/Rendering/Texture/UVs.h>
#include <ChilliSource/Rendering/Texture/TextureAtlas.h>

namespace ChilliSource
{
	namespace GUI
	{
		class HorizontalStretchableImage : public GUIView
		{	
		public:

			DECLARE_META_CLASS(HorizontalStretchableImage)

			HorizontalStretchableImage();
			HorizontalStretchableImage(const Core::ParamDictionary& insParams);

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
			/// @param Sprite sheet containing the patches
			//---------------------------------------------------------
			void SetTextureAtlas(const Rendering::TextureAtlasCSPtr& inpTextureAtlas);
			//---------------------------------------------------------
			/// Get Sprite Sheet
			///
			/// @return Sprite sheet containing the patches
			//---------------------------------------------------------
			const Rendering::TextureAtlasCSPtr& GetTextureAtlas() const;
			//---------------------------------------------------------
			/// Set Base Sprite Sheet Index ID
			///
			/// Set the "path" to the sprite sheet index IDs. 
			/// e.g. For the following IDs: 
			///			* "BluePanelLeft"
			///			* "BluePanelCentre"
			///			* "BluePanelRight"
			///
			/// the base ID would be "BluePanel"
			//---------------------------------------------------------
			void SetBaseTextureAtlasID(const std::string& instrID);
			//---------------------------------------------------------
			/// Get Base Sprite Sheet Index ID
			///
			/// Get the "path" to the sprite sheet index IDs. 
			/// e.g. For the following IDs: 
			///			* "BluePanelLeft"
			///			* "BluePanelCentre"
			///			* "BluePanelRight"
			///
			/// the base ID would be "BluePanel"
			//---------------------------------------------------------
			const std::string& GetBaseTextureAtlasID() const;
            //---------------------------------------------------------
            /// Set Patch Sprite Sheet Indices
            ///
            /// @param Sprite sheet id of left patch
            /// @param Sprite sheet id of centre patch
            /// @param Sprite sheet id of right patch
            //---------------------------------------------------------
            void SetTextureAtlasIds(const std::string& in_left, const std::string& in_centre, const std::string& in_right);
			//--------------------------------------------------------
			/// Get Combined Cap Width
			///
			/// @param Sum of the widths of both end caps
			//--------------------------------------------------------
			f32 GetCombinedCapWidth() const;
			//--------------------------------------------------------
			///
			/// @return Image height based on the max panel height
			//--------------------------------------------------------
			f32 GetCapHeight() const;
            //-----------------------------------------------------------
			/// Enable Act As Spacer
			///
			/// The image will not draw but simply act as a container
			///
			/// @param Enable/disable
			//-----------------------------------------------------------
			void EnableActAsSpacer(bool inbValue);
			//-----------------------------------------------------------
			/// Is Act As Spacer Enabled
			///
			/// @param Whether the image view is acting as a container
			//-----------------------------------------------------------
			bool IsActAsSpacerEnabled() const;
			//---------------------------------------------------------
			/// Draw
			/// 
			/// Draw the image constructed from the nine patches
			///
			/// @param Canvas renderer pointer
			//---------------------------------------------------------
			void Draw(Rendering::CanvasRenderer* inpCanvas);
            
        private:
            
            enum class Patch
            {
                k_left,
                k_centre,
                k_right,
                k_total
            };
			
		private:

            Rendering::TextureAtlas::Frame m_frames[(u32)Patch::k_total];
            
            DECLARE_PROPERTY_A(Rendering::TextureCSPtr, Texture, SetTexture, GetTexture);
			DECLARE_PROPERTY_A(Rendering::TextureAtlasCSPtr, TextureAtlas, SetTextureAtlas, GetTextureAtlas);
			
			DECLARE_PROPERTY_A(std::string, BaseTextureAtlasID, SetBaseTextureAtlasID, GetBaseTextureAtlasID);
            DECLARE_PROPERTY_A(bool, ActAsSpacer, EnableActAsSpacer, IsActAsSpacerEnabled);
		};

		typedef std::shared_ptr<HorizontalStretchableImage> HorizontalStretchableImagePtr;
	}
}

#endif