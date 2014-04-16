//
//  HorizontalSliderBar.h
//  moFloTest
//
//  Created by Scott Downie on 09/06/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _MO_FLO_GUI_DEFAULTS_HORIZONTAL_SLIDER_BAR_H_
#define _MO_FLO_GUI_DEFAULTS_HORIZONTAL_SLIDER_BAR_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/GUI/SliderBar/SliderBar.h>

namespace ChilliSource
{
	namespace GUI
	{
		class HorizontalSliderBar : public SliderBar
		{
		public:
            
			DECLARE_META_CLASS(HorizontalSliderBar)
            
			HorizontalSliderBar();
			HorizontalSliderBar(const Core::ParamDictionary& insParams);
			
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
			/// Called when the user drags the slider.
            ///
            /// @author S Downie
			///
			/// @param The view.
            /// @param The pointer.
			//-----------------------------------------------------------
			void OnSliderMoved(GUIView* inpView, const Input::PointerSystem::Pointer& in_pointer);
			//-----------------------------------------------------------
			/// Get Slider Value
			///
			/// The slider is clamped between -0.5 and 0.5
			///
			/// @return Slider value
			//-----------------------------------------------------------
			f32 GetSliderValue() const;
            
		private:
            
			ImageViewSPtr mpBackgroundImage;
			ImageViewSPtr mpSliderImage;
            
            Core::ConnectionUPtr m_movedWithinConnection;
            
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