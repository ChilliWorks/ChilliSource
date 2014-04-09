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
			void SetBackgroundTextureAtlas(const Rendering::TextureAtlasCSPtr& inpTextureAtlas);
			//--------------------------------------------------------
			/// Set Progress Sprite Sheet
			///
			/// @param Sprite sheet
			//--------------------------------------------------------
			void SetProgressTextureAtlas(const Rendering::TextureAtlasCSPtr& inpTextureAtlas);
			//--------------------------------------------------------
			/// Get Background Sprite Sheet
			///
			/// @return Sprite sheet
			//--------------------------------------------------------
			const Rendering::TextureAtlasCSPtr& GetBackgroundTextureAtlas() const;
			//--------------------------------------------------------
			/// Get Progress Sprite Sheet
			///
			/// @return Sprite sheet
			//--------------------------------------------------------
			const Rendering::TextureAtlasCSPtr& GetProgressTextureAtlas() const;
			//--------------------------------------------------------
			/// Set Background Sprite Sheet Index
			///
			/// @param The index of the image within the sprite sheet
			//--------------------------------------------------------
			void SetBackgroundTextureAtlasIndex(u32 inudwIndex);
			//--------------------------------------------------------
			/// Set Progress Sprite Sheet Index
			///
			/// @param The index of the image within the sprite sheet
			//--------------------------------------------------------
			void SetProgressTextureAtlasIndex(u32 inudwIndex);
			//--------------------------------------------------------
			/// Get Progress Sprite Sheet Index
			///
			/// @return The index of the image within the sprite sheet
			//--------------------------------------------------------
			u32 GetBackgroundTextureAtlasIndex() const;
			//--------------------------------------------------------
			/// Get Progress Sprite Sheet Index
			///
			/// @return The index of the image within the sprite sheet
			//--------------------------------------------------------
			u32 GetProgressTextureAtlasIndex() const;
			//--------------------------------------------------------
			/// Set Background Sprite Sheet Index ID
			///
			/// @param The index ID of the image within the sprite sheet
			//--------------------------------------------------------
			void SetBackgroundTextureAtlasID(const std::string& instrID);
			//--------------------------------------------------------
			/// Set Progress Sprite Sheet Index ID
			///
			/// @param The index ID of the image within the sprite sheet
			//--------------------------------------------------------
			void SetProgressTextureAtlasID(const std::string& instrID);
			//--------------------------------------------------------
			/// Get Progress Sprite Sheet Index ID
			///
			/// @return The index ID of the image within the sprite sheet
			//--------------------------------------------------------
			const std::string& GetBackgroundTextureAtlasID() const;
			//--------------------------------------------------------
			/// Get Progress Sprite Sheet Index ID
			///
			/// @return The index ID of the image within the sprite sheet
			//--------------------------------------------------------
			const std::string& GetProgressTextureAtlasID() const;
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

			DECLARE_PROPERTY_A(Rendering::TextureAtlasCSPtr, BackgroundTextureAtlas, SetBackgroundTextureAtlas, GetBackgroundTextureAtlas);
			DECLARE_PROPERTY_A(Rendering::TextureAtlasCSPtr, ProgressTextureAtlas, SetProgressTextureAtlas, GetProgressTextureAtlas);

			DECLARE_PROPERTY_A(u32, BackgroundTextureAtlasIndex, SetBackgroundTextureAtlasIndex, GetBackgroundTextureAtlasIndex);
			DECLARE_PROPERTY_A(u32, ProgressTextureAtlasIndex, SetProgressTextureAtlasIndex, GetProgressTextureAtlasIndex);

			DECLARE_PROPERTY_A(std::string, BackgroundTextureAtlasID, SetBackgroundTextureAtlasID, GetBackgroundTextureAtlasID);
			DECLARE_PROPERTY_A(std::string, ProgressTextureAtlasID, SetProgressTextureAtlasID, GetProgressTextureAtlasID);
            
            f32 mfProgressAbsHeight, mfProgressRelHeight;
		};
	}
}

#endif