//
//  HorizontalClippingProgressBar.h
//  moFlo
//
//  Created by Scott Downie on 27/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _MOFLO_GUI_DEFAULTS_HORIZONTAL_CLIPPING_PROGRESS_BAR_H_
#define _MOFLO_GUI_DEFAULTS_HORIZONTAL_CLIPPING_PROGRESS_BAR_H_

#include <ChilliSource/Core/Math/Geometry/Shapes.h>
#include <ChilliSource/Rendering/GUI/ProgressBar.h>

namespace ChilliSource
{
	namespace Rendering
	{
		class CHorizontalClippingProgressBar : public CProgressBar
		{
		public:

			DECLARE_META_CLASS(CHorizontalClippingProgressBar)

			CHorizontalClippingProgressBar();
			CHorizontalClippingProgressBar(const Core::ParamDictionary& insParams);
			//------------------------------------------------------
			/// Draw
			///
			/// Draws a background box with the progress bar
			/// as subview
			///
			/// @param Canvas renderer pointer
			//------------------------------------------------------
			void Draw(Rendering::CCanvasRenderer* inpCanvas);
			//------------------------------------------------------
			/// Set Background Image
			///
			/// @param Texture representing the background
			//------------------------------------------------------
			void SetBackgroundImage(const Rendering::TexturePtr& inpTexture);
			//------------------------------------------------------
			/// Set Progress Image
			///
			/// @param Texture representing the bar
			//------------------------------------------------------
			void SetProgressImage(const Rendering::TexturePtr& inpTexture);
			//------------------------------------------------------
			/// Get Background Image
			///
			/// @return Texture representing the background
			//------------------------------------------------------
			const Rendering::TexturePtr& GetBackgroundImage() const;
			//------------------------------------------------------
			/// Get Progress Image
			///
			/// @return Texture representing the bar
			//------------------------------------------------------
			const Rendering::TexturePtr& GetProgressImage() const;
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
			/// Get Background Sprite Sheet Index
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
            /// Enable Height From Image
            ///
            /// When this is enabled the image view's height will be 
            /// based on the size of the image
            ///
            /// @param Enable/disable
            //--------------------------------------------------------
            void EnableHeightFromImage(bool inbEnable);
			//--------------------------------------------------------
			/// Is Height From Image Enabled
			///
			/// When this is enabled the image view's height will be 
			/// based on the size of the image
			///
			/// @return Whether the image view's height will be 
			/// based on the size of the image
			//--------------------------------------------------------
			bool IsHeightFromImageEnabled() const;
			//--------------------------------------------------------
			/// Get Background Sprite Sheet Index ID
			///
			/// @return The index of the image within the sprite sheet
			//--------------------------------------------------------
			const std::string& GetBackgroundSpriteSheetIndexID() const;
			//--------------------------------------------------------
			/// Get Progress Sprite Sheet Index ID
			///
			/// @return The index ID of the image within the sprite sheet
			//--------------------------------------------------------
			const std::string& GetProgressSpriteSheetIndexID() const;
            //--------------------------------------------------------
            /// Get Background View
            ///
            /// @return Background image view
            //--------------------------------------------------------
            const ImageViewPtr& GetBackgroundView() const;

		private:

			DECLARE_PROPERTY_A(Rendering::TexturePtr, BackgroundTexture, SetBackgroundImage, GetBackgroundImage);
			DECLARE_PROPERTY_A(Rendering::TexturePtr, ProgressTexture, SetProgressImage, GetProgressImage);

			DECLARE_PROPERTY_A(Rendering::SpriteSheetPtr, BackgroundSpriteSheet, SetBackgroundSpriteSheet, GetBackgroundSpriteSheet);
			DECLARE_PROPERTY_A(Rendering::SpriteSheetPtr, ProgressSpriteSheet, SetProgressSpriteSheet, GetProgressSpriteSheet);

			DECLARE_PROPERTY_A(u32, BackgroundSpriteSheetIndex, SetBackgroundSpriteSheetIndex, GetBackgroundSpriteSheetIndex);
			DECLARE_PROPERTY_A(u32, ProgressSpriteSheetIndex, SetProgressSpriteSheetIndex, GetProgressSpriteSheetIndex);

			DECLARE_PROPERTY_A(std::string, BackgroundSpriteSheetIndexID, SetBackgroundSpriteSheetIndexID, GetBackgroundSpriteSheetIndexID);
			DECLARE_PROPERTY_A(std::string, ProgressSpriteSheetIndexID, SetProgressSpriteSheetIndexID, GetProgressSpriteSheetIndexID);
			
            DECLARE_PROPERTY_A(bool, HeightFromImage, EnableHeightFromImage, IsHeightFromImageEnabled);

			ImageViewPtr mpBackgroundImage;
			ImageViewPtr mpProgressImage;

			s32 mdwSegments;
			f32 mfSegmentSize;
		};

		typedef SHARED_PTR<CHorizontalClippingProgressBar> HorizontalClippingProgressBarPtr;
		typedef WEAK_PTR<CHorizontalClippingProgressBar> HorizontalClippingProgressBarWeakPtr;
	}
}

#endif