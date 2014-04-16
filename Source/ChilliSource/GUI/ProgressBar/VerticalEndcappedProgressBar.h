//
//  VerticalEndcappedProgressBar.h
//  iOSTemplate
//
//  Created by Thomas Kronberg on 12/02/2013.
//  Copyright (c) 2013 Tag Games. All rights reserved.
//

#ifndef _MOFLOW_GUI_DEFAULTS_VERTICALENDCAPPEDPROGRESSBAR_H_
#define _MOFLOW_GUI_DEFAULTS_VERTICALENDCAPPEDPROGRESSBAR_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/GUI/ProgressBar/ProgressBar.h>
#include <ChilliSource/GUI/Image/VerticalStretchableImage.h>

namespace ChilliSource
{
	namespace GUI
	{
		class VerticalEndcappedProgressBar : public ProgressBar
		{
		public:
            
			DECLARE_META_CLASS(VerticalEndcappedProgressBar)
            
			VerticalEndcappedProgressBar();
			VerticalEndcappedProgressBar(const Core::ParamDictionary& insParams);
            
			//------------------------------------------------------------------------
			/// Set Sprite Sheet
			///
			/// @param Sprite Sheet
			//------------------------------------------------------------------------
			void SetTextureAtlas(const Rendering::TextureAtlasCSPtr& inpTextureAtlas);
			//------------------------------------------------------------------------
			/// Get Sprite Sheet
			///
			/// @return Sprite Sheet
			//------------------------------------------------------------------------
			const Rendering::TextureAtlasCSPtr& GetTextureAtlas() const;
            //------------------------------------------------------------------------
			/// Set Texture
			///
			/// @param Texture
			//------------------------------------------------------------------------
            void SetTexture(const Rendering::TextureCSPtr& inpTexture);
			//------------------------------------------------------------------------
			/// Get Texture
			///
			/// @return Texture
			//------------------------------------------------------------------------
            const Rendering::TextureCSPtr& GetTexture() const;
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
			void SetBaseFillTextureAtlasID(const std::string& instrID);
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
			const std::string& GetBaseFillTextureAtlasID() const;
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
			void SetBaseBackgroundTextureAtlasID(const std::string& instrID);
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
			const std::string& GetBaseBackgroundTextureAtlasID() const;
			//------------------------------------------------------------------------
			/// Set Fill Indices
			///
			/// @param Sprite sheet id of top patch
			/// @param Sprite sheet id of mid patch
			/// @param Sprite sheet id of bottom patch
			//------------------------------------------------------------------------
			void SetFillTextureAtlasIds(const std::string& in_top, const std::string& in_middle, const std::string& in_bottom);
			//------------------------------------------------------------------------
			/// Set Background Indices
			///
			/// @param Sprite sheet id of top patch
			/// @param Sprite sheet id of mid patch
			/// @param Sprite sheet id of bottom patch
			//------------------------------------------------------------------------
			void SetBackgroundTextureAtlasIds(const std::string& in_top, const std::string& in_middle, const std::string& in_bottom);
            //--------------------------------------------------------
            /// Enable Width From Image
            ///
            /// When this is enabled the image view's width will be
            /// based on the width of the cap
            ///
            /// @param Enable/disable
            //--------------------------------------------------------
            void EnableWidthFromImage(bool inbEnable);
			//--------------------------------------------------------
			/// Is Width From Image Enabled
			///
			/// When this is enabled the image view's size will be
			/// based on the size of the image
			///
			/// @return Whether the image view's size will be
			/// based on the size of the image
			//--------------------------------------------------------
			bool IsWidthFromImageEnabled() const;
			//------------------------------------------------------------------------
			/// Set Fill Colour
			///
			/// @param Colour of progress bar
			//------------------------------------------------------------------------
			void SetFillColour(const Core::Colour& inColour);
			//------------------------------------------------------------------------
			/// Get Fill Colour
			///
			/// @return Colour of progress bar
			//------------------------------------------------------------------------
			const Core::Colour& GetFillColour() const;
			//------------------------------------------------------------------------
			/// Set Background Colour
			///
			/// @param Colour of background
			//------------------------------------------------------------------------
			void SetBackgroundColour(const Core::Colour& inColour);
			//------------------------------------------------------------------------
			/// Get Background Colour
			///
			/// @return Colour of background
			//------------------------------------------------------------------------
			const Core::Colour& GetBackgroundColour() const;
			//------------------------------------------------------------------------
			/// Draw
			///
			/// Draw this view and any subviews
			///
			/// @param Renderer
			//------------------------------------------------------------------------
			virtual void Draw(Rendering::CanvasRenderer* inpCanvas);
            
		private:
            
            DECLARE_PROPERTY_A(Rendering::TextureCSPtr, Texture, SetTexture, GetTexture);
			DECLARE_PROPERTY_A(Rendering::TextureAtlasCSPtr, TextureAtlas, SetTextureAtlas, GetTextureAtlas);
            
			DECLARE_PROPERTY_A(std::string, BaseFillTextureAtlasID, SetBaseFillTextureAtlasID, GetBaseFillTextureAtlasID);
			DECLARE_PROPERTY_A(std::string, BaseBackgroundTextureAtlasID, SetBaseBackgroundTextureAtlasID, GetBaseBackgroundTextureAtlasID);
            
			DECLARE_PROPERTY_A(Core::Colour, FillColour, SetFillColour, GetFillColour);
			DECLARE_PROPERTY_A(Core::Colour, BackgroundColour, SetBackgroundColour, GetBackgroundColour);
			
            
            DECLARE_PROPERTY_A(bool, WidthFromImage, EnableWidthFromImage, IsWidthFromImageEnabled);
			
			VerticalStretchableImageSPtr mpFillImage;
			VerticalStretchableImageSPtr mpBGImage;
            
		};
	}
}

#endif