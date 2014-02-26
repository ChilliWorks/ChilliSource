/*
*  HorizontalEndcappedProgressBar.h
*  iOSTemplate
*
*  Created by Stuart McGaw on 04/05/2011.
*  Copyright 2011 Tag Games. All rights reserved.
*
*/

#ifndef _MOFLOW_GUI_DEFAULTS_HORIZONTALENDCAPPEDPROGRESSBAR_H_
#define _MOFLOW_GUI_DEFAULTS_HORIZONTALENDCAPPEDPROGRESSBAR_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/GUI/ProgressBar/ProgressBar.h>
#include <ChilliSource/GUI/Image/HorizontalStretchableImage.h>

namespace ChilliSource
{
	namespace GUI
	{
		class HorizontalEndcappedProgressBar : public ProgressBar
		{
		public:

			DECLARE_META_CLASS(HorizontalEndcappedProgressBar)

			HorizontalEndcappedProgressBar();
			HorizontalEndcappedProgressBar(const Core::ParamDictionary& insParams);

			//------------------------------------------------------------------------
			/// Set Sprite Sheet
			///
			/// @param Sprite Sheet
			//------------------------------------------------------------------------
			void SetSpriteSheet(const Rendering::SpriteSheetSPtr& inpSpriteSheet);
			//------------------------------------------------------------------------
			/// Get Sprite Sheet
			///
			/// @return Sprite Sheet
			//------------------------------------------------------------------------
			const Rendering::SpriteSheetSPtr& GetSpriteSheet() const;
			//---------------------------------------------------------
			/// Set Base Fill Sprite Sheet Index ID
			///
			/// Set the "path" to the sprite sheet index IDs. 
			/// e.g. For the following IDs: 
			///			* "BLUE_PANEL_LEFT"
			///			* "BLUE_PANEL_CENTRE"
			///			* "BLUE_PANEL_RIGHT"
			///
			/// the base ID would be "BLUE_PANEL_"
			//---------------------------------------------------------
			void SetBaseFillSpriteSheetIndexID(const std::string& instrID);
			//---------------------------------------------------------
			/// Get Base Fill Sprite Sheet Index ID
			///
			/// Get the "path" to the sprite sheet index IDs. 
			/// e.g. For the following IDs: 
			///			* "BLUE_PANEL_LEFT"
			///			* "BLUE_PANEL_CENTRE"
			///			* "BLUE_PANEL_RIGHT"
			///
			/// the base ID would be "BLUE_PANEL_"
			//---------------------------------------------------------
			const std::string& GetBaseFillSpriteSheetIndexID() const;
			//---------------------------------------------------------
			/// Set Base Background Sprite Sheet Index ID
			///
			/// Set the "path" to the sprite sheet index IDs. 
			/// e.g. For the following IDs: 
			///			* "BLUE_PANEL_LEFT"
			///			* "BLUE_PANEL_CENTRE"
			///			* "BLUE_PANEL_RIGHT"
			///
			/// the base ID would be "BLUE_PANEL_"
			//---------------------------------------------------------
			void SetBaseBackgroundSpriteSheetIndexID(const std::string& instrID);
			//---------------------------------------------------------
			/// Get Base Background Sprite Sheet Index ID
			///
			/// Get the "path" to the sprite sheet index IDs. 
			/// e.g. For the following IDs: 
			///			* "BLUE_PANEL_LEFT"
			///			* "BLUE_PANEL_CENTRE"
			///			* "BLUE_PANEL_RIGHT"
			///
			/// the base ID would be "BLUE_PANEL_"
			//---------------------------------------------------------
			const std::string& GetBaseBackgroundSpriteSheetIndexID() const;
			//------------------------------------------------------------------------
			/// Set Fill Indices
			///
			/// @param Sprite sheet index of left patch
			/// @param Sprite sheet index of centre patch
			/// @param Sprite sheet index of right patch
			//------------------------------------------------------------------------
			void SetFillIndices(u32 inudwLeft, u32 inudwMid, u32 inudwRight);
			//------------------------------------------------------------------------
			/// Set Background Indices
			///
			/// @param Sprite sheet index of left patch
			/// @param Sprite sheet index of centre patch
			/// @param Sprite sheet index of right patch
			//------------------------------------------------------------------------
			void SetBackgroundIndices(u32 inudwLeft, u32 inudwMid, u32 inudwRight);
            //--------------------------------------------------------
            /// Enable Height From Image
            ///
            /// When this is enabled the image view's height will be 
            /// based on the height of the cap
            ///
            /// @param Enable/disable
            //--------------------------------------------------------
            void EnableHeightFromImage(bool inbEnable);
			//--------------------------------------------------------
			/// Is Height From Image Enabled
			///
			/// When this is enabled the image view's size will be 
			/// based on the size of the image
			///
			/// @return Whether the image view's size will be 
			/// based on the size of the image
			//--------------------------------------------------------
			bool IsHeightFromImageEnabled() const;
			//------------------------------------------------------------------------
			/// Set Fill Colour
			///
			/// @param Colour of progress bar
			//------------------------------------------------------------------------
			void SetFillColour(const Core::CColour& inColour);
			//------------------------------------------------------------------------
			/// Get Fill Colour
			///
			/// @return Colour of progress bar
			//------------------------------------------------------------------------
			const Core::CColour& GetFillColour() const;
			//------------------------------------------------------------------------
			/// Set Background Colour
			///
			/// @param Colour of background
			//------------------------------------------------------------------------
			void SetBackgroundColour(const Core::CColour& inColour);
			//------------------------------------------------------------------------
			/// Get Background Colour
			///
			/// @return Colour of background
			//------------------------------------------------------------------------
			const Core::CColour& GetBackgroundColour() const;
			//------------------------------------------------------------------------
			/// Draw
			///
			/// Draw this view and any subviews
			///
			/// @param Renderer
			//------------------------------------------------------------------------
			virtual void Draw(Rendering::CanvasRenderer* inpCanvas);

		private:

			DECLARE_PROPERTY_A(Rendering::SpriteSheetSPtr, SpriteSheet, SetSpriteSheet, GetSpriteSheet);

			DECLARE_PROPERTY_A(std::string, BaseFillSpriteSheetIndexID, SetBaseFillSpriteSheetIndexID, GetBaseFillSpriteSheetIndexID);
			DECLARE_PROPERTY_A(std::string, BaseBackgroundSpriteSheetIndexID, SetBaseBackgroundSpriteSheetIndexID, GetBaseBackgroundSpriteSheetIndexID);

			DECLARE_PROPERTY_A(Core::CColour, FillColour, SetFillColour, GetFillColour);
			DECLARE_PROPERTY_A(Core::CColour, BackgroundColour, SetBackgroundColour, GetBackgroundColour);
			
        
            DECLARE_PROPERTY_A(bool, HeightFromImage, EnableHeightFromImage, IsHeightFromImageEnabled);
			
			HorizontalStretchableImagePtr mpFillImage;
			HorizontalStretchableImagePtr mpBGImage;
            
		};
	}
}

#endif