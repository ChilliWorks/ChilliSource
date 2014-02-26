//
//  HorizontalProgressBar.h
//  moFlo
//
//  Created by Scott Downie on 27/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _MOFLO_GUI_DEFAULTS_HORIZONTALPROGRESSBAR_H_
#define _MOFLO_GUI_DEFAULTS_HORIZONTALPROGRESSBAR_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/GUI/ProgressBar/ProgressBar.h>

namespace ChilliSource
{
	namespace GUI
	{
		class HorizontalProgressBar : public ProgressBar
		{
		public:

			DECLARE_META_CLASS(HorizontalProgressBar)

			HorizontalProgressBar();
			HorizontalProgressBar(const Core::ParamDictionary& insParams);
			//------------------------------------------------------
			/// Draw
			///
			/// Draws a background box with the progress bar
			/// as subview
			///
			/// @param Canvas renderer pointer
			//------------------------------------------------------
			void Draw(Rendering::CanvasRenderer* inpCanvas);
			//------------------------------------------------------
			/// Set Background Image
			///
			/// @param Texture representing the background
			//------------------------------------------------------
			void SetBackgroundImage(const Rendering::TextureSPtr& inpTexture);
			//------------------------------------------------------
			/// Set Progress Image
			///
			/// @param Texture representing the bar
			//------------------------------------------------------
			void SetProgressImage(const Rendering::TextureSPtr& inpTexture);
			//------------------------------------------------------
			/// Get Background Image
			///
			/// @return Texture representing the background
			//------------------------------------------------------
			const Rendering::TextureSPtr& GetBackgroundImage() const;
			//------------------------------------------------------
			/// Get Progress Image
			///
			/// @return Texture representing the bar
			//------------------------------------------------------
			const Rendering::TextureSPtr& GetProgressImage() const;
			//--------------------------------------------------------
			/// Set Background Sprite Sheet
			///
			/// @param Sprite sheet
			//--------------------------------------------------------
			void SetBackgroundSpriteSheet(const Rendering::SpriteSheetPtr& inpSpriteSheet);
			//--------------------------------------------------------
			/// Set Progress Sprite Sheet
			///
			/// @param Sprite sheet
			//--------------------------------------------------------
			void SetProgressSpriteSheet(const Rendering::SpriteSheetPtr& inpSpriteSheet);
			//--------------------------------------------------------
			/// Get Background Sprite Sheet
			///
			/// @return Sprite sheet
			//--------------------------------------------------------
			const Rendering::SpriteSheetPtr& GetBackgroundSpriteSheet() const;
			//--------------------------------------------------------
			/// Get Progress Sprite Sheet
			///
			/// @return Sprite sheet
			//--------------------------------------------------------
			const Rendering::SpriteSheetPtr& GetProgressSpriteSheet() const;
			//--------------------------------------------------------
			/// Set Background Sprite Sheet Index
			///
			/// @param The index of the image within the sprite sheet
			//--------------------------------------------------------
			void SetBackgroundSpriteSheetIndex(u32 inudwIndex);
			//--------------------------------------------------------
			/// Set Progress Sprite Sheet Index
			///
			/// @param The index of the image within the sprite sheet
			//--------------------------------------------------------
			void SetProgressSpriteSheetIndex(u32 inudwIndex);
			//--------------------------------------------------------
			/// Get Progress Sprite Sheet Index
			///
			/// @return The index of the image within the sprite sheet
			//--------------------------------------------------------
			u32 GetBackgroundSpriteSheetIndex() const;
			//--------------------------------------------------------
			/// Get Progress Sprite Sheet Index
			///
			/// @return The index of the image within the sprite sheet
			//--------------------------------------------------------
			u32 GetProgressSpriteSheetIndex() const;
			//--------------------------------------------------------
			/// Set Background Sprite Sheet Index ID
			///
			/// @param The index ID of the image within the sprite sheet
			//--------------------------------------------------------
			void SetBackgroundSpriteSheetIndexID(const std::string& instrID);
			//--------------------------------------------------------
			/// Set Progress Sprite Sheet Index ID
			///
			/// @param The index ID of the image within the sprite sheet
			//--------------------------------------------------------
			void SetProgressSpriteSheetIndexID(const std::string& instrID);
			//--------------------------------------------------------
			/// Get Progress Sprite Sheet Index ID
			///
			/// @return The index ID of the image within the sprite sheet
			//--------------------------------------------------------
			const std::string& GetBackgroundSpriteSheetIndexID() const;
			//--------------------------------------------------------
			/// Get Progress Sprite Sheet Index ID
			///
			/// @return The index ID of the image within the sprite sheet
			//--------------------------------------------------------
			const std::string& GetProgressSpriteSheetIndexID() const;
            //--------------------------------------------------------
            /// Set Progress Image Height
            ///
            /// @param Rel, Abs
            //--------------------------------------------------------
            void SetProgressImageHeight(f32 infRel, f32 infAbs);

		private:

			ImageViewSPtr mpBackgroundImage;
			ImageViewSPtr mpProgressImage;

			DECLARE_PROPERTY_A(Rendering::TextureSPtr, BackgroundTexture, SetBackgroundImage, GetBackgroundImage);
			DECLARE_PROPERTY_A(Rendering::TextureSPtr, ProgressTexture, SetProgressImage, GetProgressImage);

			DECLARE_PROPERTY_A(Rendering::SpriteSheetPtr, BackgroundSpriteSheet, SetBackgroundSpriteSheet, GetBackgroundSpriteSheet);
			DECLARE_PROPERTY_A(Rendering::SpriteSheetPtr, ProgressSpriteSheet, SetProgressSpriteSheet, GetProgressSpriteSheet);

			DECLARE_PROPERTY_A(u32, BackgroundSpriteSheetIndex, SetBackgroundSpriteSheetIndex, GetBackgroundSpriteSheetIndex);
			DECLARE_PROPERTY_A(u32, ProgressSpriteSheetIndex, SetProgressSpriteSheetIndex, GetProgressSpriteSheetIndex);

			DECLARE_PROPERTY_A(std::string, BackgroundSpriteSheetIndexID, SetBackgroundSpriteSheetIndexID, GetBackgroundSpriteSheetIndexID);
			DECLARE_PROPERTY_A(std::string, ProgressSpriteSheetIndexID, SetProgressSpriteSheetIndexID, GetProgressSpriteSheetIndexID);
            
            f32 mfProgressAbsHeight, mfProgressRelHeight;
		};
	}
}

#endif