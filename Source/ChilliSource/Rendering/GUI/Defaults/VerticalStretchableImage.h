//
//  StretchableImage.h
//  moFlo
//
//  Created by Scott Downie on 28/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _MOFLO_GUI_VerticalStretchableImageVIEW_H_
#define _MOFLO_GUI_VerticalStretchableImageVIEW_H_

#include <ChilliSource/Rendering/GUI/GUIView.h>

//=============================================================
/// GUI - Stretchable Image 
///
/// Creates a scalable image using the nine patch system
//=============================================================

namespace ChilliSource
{
    namespace Rendering
    {
        class CVerticalStretchableImage : public CGUIView
        {
        public:

			DECLARE_META_CLASS(CVerticalStretchableImage)

            struct SpriteSheetIndex
            {
                u32 udwTop;
                u32 udwBottom;
                u32 udwMiddle;
            };
            CVerticalStretchableImage();
            CVerticalStretchableImage(const Core::ParamDictionary& insParams);
            //---------------------------------------------------------
            /// Set Sprite Sheet
            ///
            /// @param Sprite sheet containing the nine patches
            //---------------------------------------------------------
            void SetSpriteSheet(const SpriteSheetPtr& inpSpriteSheet);
            //---------------------------------------------------------
            /// Get Sprite Sheet
            ///
            /// @return Sprite sheet containing the nine patches
            //---------------------------------------------------------
			const SpriteSheetPtr& GetSpriteSheet() const;
			//---------------------------------------------------------
			/// Set Base Sprite Sheet Index ID
			///
			/// Set the "path" to the sprite sheet index IDs. 
			/// e.g. For the following IDs: 
			///			* "BLUE_PANEL_TOP"
			///			* "BLUE_PANEL_BOTTOM"
			///			* "BLUE_PANEL_MIDDLE"
			///
			/// the base ID would be "BLUE_PANEL_"
			//---------------------------------------------------------
			void SetBaseSpriteSheetIndexID(const std::string& instrID);
			//---------------------------------------------------------
			/// Get Base Sprite Sheet Index ID
			///
			/// Get the "path" to the sprite sheet index IDs. 
			/// e.g. For the following IDs: 
			///			* "BLUE_PANEL_TOP"
			///			* "BLUE_PANEL_BOTTOM"
			///			* "BLUE_PANEL_MIDDLE"
			///
			/// the base ID would be "BLUE_PANEL_"
			//---------------------------------------------------------
			const std::string& GetBaseSpriteSheetIndexID() const;
            //---------------------------------------------------------
            /// Set Patch Sprite Sheet Indices
            ///
            /// @param Struct containing the tpage index of each patch
            //---------------------------------------------------------
            void SetSpriteSheetIndices(const SpriteSheetIndex& insIndices);
            //---------------------------------------------------------
            /// Set Patch Sprite Sheet Indices
            ///
            /// Top 
            /// Bottom
            /// Middle
            ///
            /// @param Array containing the tpage index of each patch
            //---------------------------------------------------------
            void SetSpriteSheetIndices(const u32* inpIndices);
			//---------------------------------------------------------
			/// Set Patch Sprite Sheet Indices
			///
			/// @param Sprite sheet index of top patch
			/// @param Sprite sheet index of middle patch
			/// @param Sprite sheet index of bottom patch
			//---------------------------------------------------------
			void SetSpriteSheetIndices(u32 inudwTop, u32 inudwMid, u32 inudwBottom);
            //---------------------------------------------------------
            /// Draw
            /// 
            /// Draw the image constructed from the nine patches
            ///
            /// @param Canvas renderer pointer
            //---------------------------------------------------------
            void Draw(CCanvasRenderer* inpCanvas);
			//--------------------------------------------------------
            /// Set Width Maintaining Aspect
            ///
            /// Change the current width of the image view and resize the height
            /// to maintain the aspect ratio of the image
            ///
            /// @param Unified width
            //--------------------------------------------------------
            void SetWidthMaintainingAspect(f32 infRelWidth, f32 infAbsWidth);
            //--------------------------------------------------------
            /// Set Height Maintaining Aspect
            ///
            /// Change the current height of the image view and resize the width
            /// to maintain the aspect ratio of the image
            ///
            /// @param Unified height
            //--------------------------------------------------------
            void SetHeightMaintainingAspect(f32 infRelHeight, f32 infAbsHeight);
            //--------------------------------------------------------
            /// Enable Width Maintaining Aspect
            ///
            /// Enables auto scaling of the Width to maintain the aspect ratio
            ///
            /// @param boolean to disable or enable
            //--------------------------------------------------------
            void EnableWidthMaintainingAspect(bool inbEnabled);
            //--------------------------------------------------------
            /// Enable Height Maintaining Aspect
            ///
            /// Enables auto scaling of the height to maintain the aspect ratio
            ///
            /// @param boolean to disable or enable
            //--------------------------------------------------------
            void EnableHeightMaintainingAspect(bool inbEnabled);
			//--------------------------------------------------------
            /// Is Width Maintaining Aspect Enabled
            ///
            /// @return auto scaling of the Width to maintain the aspect ratio
            //--------------------------------------------------------
            bool IsWidthMaintainingAspectEnabled() const;
            //--------------------------------------------------------
            /// Is Height Maintaining Aspect Enabled
            ///
            /// @return auto scaling of the height to maintain the aspect ratio
            //--------------------------------------------------------
            bool IsHeightMaintainingAspectEnabled() const;
            //--------------------------------------------------------
            /// Get Combined Cap Height
            ///
            /// @param Sum of the widths of both end caps
            //--------------------------------------------------------
            f32 GetCombinedCapHeight() const;
            //--------------------------------------------------------
            /// Get Combined Cap Width
            ///
            /// @param Sum of the heights of both end caps
            //--------------------------------------------------------
            f32 GetCapWidth() const;
			
			private:

			DECLARE_PROPERTY_A(SpriteSheetPtr, SpriteSheet, SetSpriteSheet, GetSpriteSheet);

			DECLARE_PROPERTY_A(std::string, BaseSpriteSheetIndexID, SetBaseSpriteSheetIndexID, GetBaseSpriteSheetIndexID);

			DECLARE_PROPERTY_A(bool, HeightMaintain, EnableHeightMaintainingAspect, IsHeightMaintainingAspectEnabled);
			DECLARE_PROPERTY_A(bool, WidthMaintain, EnableWidthMaintainingAspect, IsWidthMaintainingAspectEnabled);
			
            SpriteSheetIndex msIndices;
            
           

        };
        
        typedef SHARED_PTR<CVerticalStretchableImage> VerticalStretchableImagePtr;
    }
}

#endif