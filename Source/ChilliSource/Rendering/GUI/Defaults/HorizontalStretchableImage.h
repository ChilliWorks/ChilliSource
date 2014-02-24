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

#include <ChilliSource/Rendering/GUI/GUIView.h>

#include <ChilliSource/Rendering/ForwardDeclarations.h>

namespace ChilliSource
{
	namespace Rendering
	{
		class CHorizontalStretchableImage : public moFlo::GUI::CGUIView
		{	
		public:

			DECLARE_META_CLASS(CHorizontalStretchableImage)

			CHorizontalStretchableImage();
			CHorizontalStretchableImage(const Core::ParamDictionary& insParams);

			//---------------------------------------------------------
			/// Set Sprite Sheet
			///
			/// @param Sprite sheet containing the patches
			//---------------------------------------------------------
			void SetSpriteSheet(const Rendering::SpriteSheetPtr& inpSpriteSheet);
			//---------------------------------------------------------
			/// Get Sprite Sheet
			///
			/// @return Sprite sheet containing the patches
			//---------------------------------------------------------
			const Rendering::SpriteSheetPtr& GetSpriteSheet() const;
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
			void SetBaseSpriteSheetIndexID(const std::string& instrID);
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
			const std::string& GetBaseSpriteSheetIndexID() const;
			//---------------------------------------------------------
			/// Set Patch Sprite Sheet Indices
			///
			/// Left
			/// Centre
			/// Right
			///
			/// @param Array containing the tpage index of each patch
			//---------------------------------------------------------
			void SetSpriteSheetIndices(const u32* inpIndices);
			//---------------------------------------------------------
			/// Set Patch Sprite Sheet Indices
			///
			/// @param Sprite sheet index of left patch
			/// @param Sprite sheet index of centre patch
			/// @param Sprite sheet index of right patch
			//---------------------------------------------------------
			void SetSpriteSheetIndices(u32 inudwLeft, u32 inudwMid, u32 inudwRight);
			//---------------------------------------------------------
			/// Set Patch Sprite Sheet Left Index
			///
			/// @param tpage index of left patch
			//---------------------------------------------------------
			void SetSpriteSheetLeftIndex(u32 inudwIndex);
			//---------------------------------------------------------
			/// Set Patch Sprite Sheet Centre Index
			///
			/// @param tpage index of centre patch
			//---------------------------------------------------------
			void SetSpriteSheetCentreIndex(u32 inudwIndex);
			//---------------------------------------------------------
			/// Set Patch Sprite Sheet Right Index
			///
			/// @param tpage index of right patch
			//---------------------------------------------------------
			void SetSpriteSheetRightIndex(u32 inudwIndex);
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
			void Draw(moFlo::Rendering::CCanvasRenderer* inpCanvas);
			
			
		private:

			DECLARE_PROPERTY_A(Rendering::SpriteSheetPtr, SpriteSheet, SetSpriteSheet, GetSpriteSheet);
			
			DECLARE_PROPERTY_A(std::string, BaseSpriteSheetIndexID, SetBaseSpriteSheetIndexID, GetBaseSpriteSheetIndexID);
            DECLARE_PROPERTY_A(bool, HeightFromImage, EnableHeightFromImage, IsHeightFromImageEnabled);
            DECLARE_PROPERTY_A(bool, ActAsSpacer, EnableActAsSpacer, IsActAsSpacerEnabled);
				
			u32 mudwLeftCapIndex;
			u32 mudwCentreIndex;
			u32 mudwRightCapIndex;
		};

		typedef SHARED_PTR<CHorizontalStretchableImage> HorizontalStretchableImagePtr;
	}
}

#endif