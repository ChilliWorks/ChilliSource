//
//  VerticalSliderBar.h
//  moFloTest
//
//  Created by Scott Downie on 09/06/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//


#ifndef _MO_FLO_GUI_DEFAULTS_VERTICAL_SLIDER_BAR_H_
#define _MO_FLO_GUI_DEFAULTS_VERTICAL_SLIDER_BAR_H_

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
            void SetBarTexture(const Rendering::TextureSPtr& inpTexture);
			//--------------------------------------------------------
			/// Get Bar Texture
			///
			/// @return Texture for the bar
			//--------------------------------------------------------
			inline const Rendering::TextureSPtr& GetBarTexture() const
            {
                return BarTexture;
            }
			//--------------------------------------------------------
			/// Set Slider Texture
			///
			/// @param Texture for the slider
			//--------------------------------------------------------
            void SetSliderTexture(const Rendering::TextureSPtr& inpTexture);
			//--------------------------------------------------------
			/// Get Slider Texture
			///
			/// @return Texture for the slider
			//--------------------------------------------------------
			inline const Rendering::TextureSPtr& GetSliderTexture() const
            {
                return SliderTexture;
            }
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
            /// Set Bar Sprite Sheet Index
            ///
            /// @param The index of the image within the sprite sheet
            //--------------------------------------------------------
            void SetBarTextureAtlasIndex(u32 inudwIndex);
            //--------------------------------------------------------
            /// Set Slider Sprite Sheet Index
            ///
            /// @param The index of the image within the sprite sheet
            //--------------------------------------------------------
            void SetSliderTextureAtlasIndex(u32 inudwIndex);
			//--------------------------------------------------------
			/// Get Bar Sprite Sheet Index
			///
			/// @return The index of the image within the sprite sheet
			//--------------------------------------------------------
			u32 GetBarTextureAtlasIndex() const;
			//--------------------------------------------------------
			/// Get Slider Sprite Sheet Index
			///
			/// @return The index of the image within the sprite sheet
			//--------------------------------------------------------
			u32 GetSliderTextureAtlasIndex() const;
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
            void OnSliderMoved(GUIView* in_view, const Input::PointerSystem::Pointer& in_pointer);
			//-----------------------------------------------------------
			/// Update Slider Position
			/// 
			/// Reposition the slider image
			//-----------------------------------------------------------
			void UpdateSliderPosition();

        private:
            
            ImageViewSPtr mpBackgroundImage;
            ImageViewSPtr mpSliderImage;
            
            Core::ConnectionUPtr m_sliderMovedConnection;

			DECLARE_PROPERTY_A(Core::UnifiedVector2, UnifiedSliderSize, SetSliderSize, GetSliderSize);

            DECLARE_PROPERTY_A(Rendering::TextureSPtr, BarTexture, SetBarTexture, GetBarTexture);
			DECLARE_PROPERTY_A(Rendering::TextureSPtr, SliderTexture, SetSliderTexture, GetSliderTexture);
			DECLARE_PROPERTY_A(Rendering::TextureAtlasCSPtr, BarTextureAtlas, SetBarTextureAtlas, GetBarTextureAtlas);
			DECLARE_PROPERTY_A(Rendering::TextureAtlasCSPtr, SliderTextureAtlas, SetSliderTextureAtlas, GetSliderTextureAtlas);

			DECLARE_PROPERTY_A(u32, BarTextureAtlasIndex, SetBarTextureAtlasIndex, GetBarTextureAtlasIndex);
			DECLARE_PROPERTY_A(u32, SliderTextureAtlasIndex, SetSliderTextureAtlasIndex, GetSliderTextureAtlasIndex);

			DECLARE_PROPERTY_A(std::string, BarTextureAtlasID, SetBarTextureAtlasID, GetBarTextureAtlasID);
			DECLARE_PROPERTY_A(std::string, SliderTextureAtlasID, SetSliderTextureAtlasID, GetSliderTextureAtlasID);
        };
    }
}

#endif
