/*
 *  HorizontalStretchableImage.h
 *  iOSTemplate
 *
 *  Created by Stuart McGaw on 04/05/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLO_GUI_DEFAULTS_HORIZONTALSTRETCHABLEIMAGE_H_
#define _MOFLO_GUI_DEFAULTS_HORIZONTALSTRETCHABLEIMAGE_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Math/Geometry/Shapes.h>
#include <ChilliSource/GUI/Base/GUIView.h>
#include <ChilliSource/Rendering/Texture/UVs.h>

namespace ChilliSource
{
	namespace GUI
	{
		class HorizontalStretchableImage : public GUIView
		{	
		public:

            struct PanelDesc
            {
                Rendering::UVs m_leftUVs;
                Rendering::UVs m_centreUVs;
                Rendering::UVs m_rightUVs;
                
                Core::Vector2 m_leftSize;
                Core::Vector2 m_centreSize;
                Core::Vector2 m_rightSize;
            };
            
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
			///			* "BLUE_PANEL_LEFT"
			///			* "BLUE_PANEL_CENTRE"
			///			* "BLUE_PANEL_RIGHT"
			///
			/// the base ID would be "BLUE_PANEL_"
			//---------------------------------------------------------
			void SetBaseTextureAtlasID(const std::string& instrID);
			//---------------------------------------------------------
			/// Get Base Sprite Sheet Index ID
			///
			/// Get the "path" to the sprite sheet index IDs. 
			/// e.g. For the following IDs: 
			///			* "BLUE_PANEL_LEFT"
			///			* "BLUE_PANEL_CENTRE"
			///			* "BLUE_PANEL_RIGHT"
			///
			/// the base ID would be "BLUE_PANEL_"
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
			/// Get Combined Cap Width
			///
			/// @param Sum of the widths of both end caps
			//--------------------------------------------------------
			f32 GetCombinedCapWidth() const;
			//--------------------------------------------------------
			/// Get Combined Cap Height
			///
			/// @param Sum of the heights of both end caps
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
			//--------------------------------------------------------
			/// Layout Content
			///
			/// Called when the view is able to retrieve an absolute
			/// value.
			//--------------------------------------------------------
			void LayoutContent();
			//--------------------------------------------------------
            /// Set Size From Image
            ///
            /// Sets the of the image view to the size of the image
            /// without requiring a draw to call. Useful for working
            /// with relative sizes and aspect ratios.
            //--------------------------------------------------------
            void SetSizeFromImage();
			//---------------------------------------------------------
			/// Draw
			/// 
			/// Draw the image constructed from the nine patches
			///
			/// @param Canvas renderer pointer
			//---------------------------------------------------------
			void Draw(Rendering::CanvasRenderer* inpCanvas);
			
			
		private:

            DECLARE_PROPERTY_A(Rendering::TextureCSPtr, Texture, SetTexture, GetTexture);
			DECLARE_PROPERTY_A(Rendering::TextureAtlasCSPtr, TextureAtlas, SetTextureAtlas, GetTextureAtlas);
			
			DECLARE_PROPERTY_A(std::string, BaseTextureAtlasID, SetBaseTextureAtlasID, GetBaseTextureAtlasID);
            DECLARE_PROPERTY_A(bool, HeightFromImage, EnableHeightFromImage, IsHeightFromImageEnabled);
            DECLARE_PROPERTY_A(bool, ActAsSpacer, EnableActAsSpacer, IsActAsSpacerEnabled);
            
            PanelDesc m_panels;
		};

		typedef std::shared_ptr<HorizontalStretchableImage> HorizontalStretchableImagePtr;
	}
}

#endif