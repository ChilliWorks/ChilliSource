//
//  VerticalSliderBar.h
//  moFloTest
//
//  Created by Scott Downie on 09/06/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//


#ifndef _MO_FLO_GUI_DEFAULTS_VERTICAL_SLIDER_BAR_H_
#define _MO_FLO_GUI_DEFAULTS_VERTICAL_SLIDER_BAR_H_

#include <ChilliSource/GUI/SliderBar/SliderBar.h>

namespace ChilliSource
{
    namespace GUI
    {
        class CVerticalSliderBar : public CSliderBar
        {
        public:

			DECLARE_META_CLASS(CVerticalSliderBar)

            CVerticalSliderBar();
            CVerticalSliderBar(const Core::ParamDictionary& insParams);
			~CVerticalSliderBar();
            //------------------------------------------------------
            /// Draw
            ///
            /// Draws a background box with the progress bar
            /// as subview
            ///
            /// @param Canvas renderer pointer
            //------------------------------------------------------
            void Draw(Rendering::CCanvasRenderer* inpCanvas);
            //--------------------------------------------------------
            /// Set Bar Sprite Sheet
            ///
            /// @param Sprite sheet for the bar
            //--------------------------------------------------------
            void SetBarSpriteSheet(const Rendering::SpriteSheetPtr& inpSpriteSheet);
			//--------------------------------------------------------
			/// Get Bar Sprite Sheet
			///
			/// @return Sprite sheet for the bar
			//--------------------------------------------------------
			const Rendering::SpriteSheetPtr& GetBarSpriteSheet() const;
			//--------------------------------------------------------
			/// Set Slider Sprite Sheet
			///
			/// @param Sprite sheet for the slider
			//--------------------------------------------------------
			void SetSliderSpriteSheet(const Rendering::SpriteSheetPtr& inpSpriteSheet);
			//--------------------------------------------------------
			/// Get Slider Sprite Sheet
			///
			/// @return Sprite sheet for the slider
			//--------------------------------------------------------
			const Rendering::SpriteSheetPtr& GetSliderSpriteSheet() const;
            //--------------------------------------------------------
            /// Set Bar Sprite Sheet Index
            ///
            /// @param The index of the image within the sprite sheet
            //--------------------------------------------------------
            void SetBarSpriteSheetIndex(u32 inudwIndex);
            //--------------------------------------------------------
            /// Set Slider Sprite Sheet Index
            ///
            /// @param The index of the image within the sprite sheet
            //--------------------------------------------------------
            void SetSliderSpriteSheetIndex(u32 inudwIndex);
			//--------------------------------------------------------
			/// Get Bar Sprite Sheet Index
			///
			/// @return The index of the image within the sprite sheet
			//--------------------------------------------------------
			u32 GetBarSpriteSheetIndex() const;
			//--------------------------------------------------------
			/// Get Slider Sprite Sheet Index
			///
			/// @return The index of the image within the sprite sheet
			//--------------------------------------------------------
			u32 GetSliderSpriteSheetIndex() const;
			//--------------------------------------------------------
			/// Set Bar Sprite Sheet Index ID
			///
			/// @param The index ID of the image within the sprite sheet
			//--------------------------------------------------------
			void SetBarSpriteSheetIndexID(const std::string& instrID);
			//--------------------------------------------------------
			/// Set Slider Sprite Sheet Index ID
			///
			/// @param The index ID of the image within the sprite sheet
			//--------------------------------------------------------
			void SetSliderSpriteSheetIndexID(const std::string& instrID);
			//--------------------------------------------------------
			/// Get Bar Sprite Sheet Index ID
			///
			/// @return The index ID of the image within the sprite sheet
			//--------------------------------------------------------
			const std::string& GetBarSpriteSheetIndexID() const;
			//--------------------------------------------------------
			/// Get Slider Sprite Sheet Index ID
			///
			/// @return The index ID of the image within the sprite sheet
			//--------------------------------------------------------
			const std::string& GetSliderSpriteSheetIndexID() const;
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
            /// On Slider Moved
            ///
            /// Called when the user drags the slider
            ///
            /// @param Touch data
            //-----------------------------------------------------------
            void OnSliderMoved(CGUIView* inpView, const Input::TouchInfo & insTouchInfo);
			//-----------------------------------------------------------
			/// Update Slider Position
			/// 
			/// Reposition the slider image
			//-----------------------------------------------------------
			void UpdateSliderPosition();

        private:
            
            ImageViewPtr mpBackgroundImage;
            ImageViewPtr mpSliderImage;

			DECLARE_PROPERTY_A(Core::UnifiedVector2, UnifiedSliderSize, SetSliderSize, GetSliderSize);

			DECLARE_PROPERTY_A(Rendering::SpriteSheetPtr, BarSpriteSheet, SetBarSpriteSheet, GetBarSpriteSheet);
			DECLARE_PROPERTY_A(Rendering::SpriteSheetPtr, SliderSpriteSheet, SetSliderSpriteSheet, GetSliderSpriteSheet);

			DECLARE_PROPERTY_A(u32, BarSpriteSheetIndex, SetBarSpriteSheetIndex, GetBarSpriteSheetIndex);
			DECLARE_PROPERTY_A(u32, SliderSpriteSheetIndex, SetSliderSpriteSheetIndex, GetSliderSpriteSheetIndex);

			DECLARE_PROPERTY_A(std::string, BarSpriteSheetIndexID, SetBarSpriteSheetIndexID, GetBarSpriteSheetIndexID);
			DECLARE_PROPERTY_A(std::string, SliderSpriteSheetIndexID, SetSliderSpriteSheetIndexID, GetSliderSpriteSheetIndexID);
        };
        
        typedef SHARED_PTR<CVerticalSliderBar> VerticalSliderBarPtr;
        typedef WEAK_PTR<CVerticalSliderBar> VerticalSliderBarWeakPtr;
    }
}

#endif
