//
//  VerticalSliderBar.h
//  Chilli Source
//  Created by Scott Downie on 09/06/2011.
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

#ifndef _CHILLISOURCE_GUI_SLIDERBAR_VERTICALSLIDERBAR_H_
#define _CHILLISOURCE_GUI_SLIDERBAR_VERTICALSLIDERBAR_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/GUI/SliderBar/SliderBar.h>

namespace ChilliSource
{
    namespace GUI
    {
        class VerticalSliderBar : public SliderBar
        {
        public:

			DECLARE_META_CLASS(VerticalSliderBar)

            VerticalSliderBar();
            VerticalSliderBar(const Core::ParamDictionary& insParams);
            //------------------------------------------------------
            /// Draw
            ///
            /// Draws a background box with the progress bar
            /// as subview
            ///
            /// @param Canvas renderer pointer
            //------------------------------------------------------
            void Draw(Rendering::CanvasRenderer* inpCanvas);
            //--------------------------------------------------------
            /// Set Bar Texture
            ///
            /// @param Texture for the bar
            //--------------------------------------------------------
            void SetBarTexture(const Rendering::TextureCSPtr& inpTexture);
			//--------------------------------------------------------
			/// Get Bar Texture
			///
			/// @return Texture for the bar
			//--------------------------------------------------------
            const Rendering::TextureCSPtr& GetBarTexture() const;
			//--------------------------------------------------------
			/// Set Slider Texture
			///
			/// @param Texture for the slider
			//--------------------------------------------------------
            void SetSliderTexture(const Rendering::TextureCSPtr& inpTexture);
			//--------------------------------------------------------
			/// Get Slider Texture
			///
			/// @return Texture for the slider
			//--------------------------------------------------------
            const Rendering::TextureCSPtr& GetSliderTexture() const;
            //--------------------------------------------------------
            /// Set Bar Sprite Sheet
            ///
            /// @param Sprite sheet for the bar
            //--------------------------------------------------------
            void SetBarTextureAtlas(const Rendering::TextureAtlasCSPtr& inpTextureAtlas);
			//--------------------------------------------------------
			/// Get Bar Sprite Sheet
			///
			/// @return Sprite sheet for the bar
			//--------------------------------------------------------
			const Rendering::TextureAtlasCSPtr& GetBarTextureAtlas() const;
			//--------------------------------------------------------
			/// Set Slider Sprite Sheet
			///
			/// @param Sprite sheet for the slider
			//--------------------------------------------------------
			void SetSliderTextureAtlas(const Rendering::TextureAtlasCSPtr& inpTextureAtlas);
			//--------------------------------------------------------
			/// Get Slider Sprite Sheet
			///
			/// @return Sprite sheet for the slider
			//--------------------------------------------------------
			const Rendering::TextureAtlasCSPtr& GetSliderTextureAtlas() const;
			//--------------------------------------------------------
			/// Set Bar Sprite Sheet Index ID
			///
			/// @param The index ID of the image within the sprite sheet
			//--------------------------------------------------------
			void SetBarTextureAtlasID(const std::string& instrID);
			//--------------------------------------------------------
			/// Set Slider Sprite Sheet Index ID
			///
			/// @param The index ID of the image within the sprite sheet
			//--------------------------------------------------------
			void SetSliderTextureAtlasID(const std::string& instrID);
			//--------------------------------------------------------
			/// Get Bar Sprite Sheet Index ID
			///
			/// @return The index ID of the image within the sprite sheet
			//--------------------------------------------------------
			const std::string& GetBarTextureAtlasID() const;
			//--------------------------------------------------------
			/// Get Slider Sprite Sheet Index ID
			///
			/// @return The index ID of the image within the sprite sheet
			//--------------------------------------------------------
			const std::string& GetSliderTextureAtlasID() const;
            //--------------------------------------------------------
            /// Set Slider Size
            ///
            /// @param The unified size of the slider
            //--------------------------------------------------------
            void SetSliderSize(const Core::UnifiedVector2& invUnifiedSize);
			//--------------------------------------------------------
			/// Get Slider Size
			///
			/// @return The unified size of the slider
			//--------------------------------------------------------
			const Core::UnifiedVector2& GetSliderSize() const;
            
            //---Touch Delegates
            //-----------------------------------------------------------
            /// Called when the user drags the slider
            ///
            /// @author S Downie
            ///
            /// @param Touch data
            //-----------------------------------------------------------
            void OnSliderMoved(GUIView* in_view, const Input::Pointer& in_pointer);
			//-----------------------------------------------------------
			/// Update Slider Position
			/// 
			/// Reposition the slider image
			//-----------------------------------------------------------
			void UpdateSliderPosition();

        private:
            
            ImageViewSPtr mpBackgroundImage;
            ImageViewSPtr mpSliderImage;
            
            Core::EventConnectionUPtr m_sliderMovedConnection;

			DECLARE_PROPERTY_A(Core::UnifiedVector2, UnifiedSliderSize, SetSliderSize, GetSliderSize);

            DECLARE_PROPERTY_A(Rendering::TextureCSPtr, BarTexture, SetBarTexture, GetBarTexture);
			DECLARE_PROPERTY_A(Rendering::TextureCSPtr, SliderTexture, SetSliderTexture, GetSliderTexture);
			DECLARE_PROPERTY_A(Rendering::TextureAtlasCSPtr, BarTextureAtlas, SetBarTextureAtlas, GetBarTextureAtlas);
			DECLARE_PROPERTY_A(Rendering::TextureAtlasCSPtr, SliderTextureAtlas, SetSliderTextureAtlas, GetSliderTextureAtlas);

			DECLARE_PROPERTY_A(std::string, BarTextureAtlasID, SetBarTextureAtlasID, GetBarTextureAtlasID);
			DECLARE_PROPERTY_A(std::string, SliderTextureAtlasID, SetSliderTextureAtlasID, GetSliderTextureAtlasID);
        };
    }
}

#endif
