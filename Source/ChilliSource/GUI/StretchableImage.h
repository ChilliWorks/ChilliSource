//
//  StretchableImage.h
//  moFlo
//
//  Created by Scott Downie on 28/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _MOFLO_GUI_STRETCHABLEIMAGEVIEW_H_
#define _MOFLO_GUI_STRETCHABLEIMAGEVIEW_H_

#include <ChilliSource/GUI/GUIView.h>

//=============================================================
/// GUI - Stretchable Image 
///
/// Creates a scalable image using the nine patch system
//=============================================================

namespace moFlo
{
    namespace GUI
    {
        class CStretchableImage : public CGUIView
        {
        public:

			DECLARE_META_CLASS(CStretchableImage)

            struct SpriteSheetIndex
            {
                u32 udwTopLeft;
                u32 udwTopRight;
                u32 udwBottomLeft;
                u32 udwBottomRight;
                u32 udwTopCentre;
                u32 udwBottomCentre;
                u32 udwLeftCentre;
                u32 udwRightCentre;
                u32 udwMiddleCentre;
            };
            CStretchableImage();
            CStretchableImage(const Core::ParamDictionary& insParams);
            //---------------------------------------------------------
            /// Set Sprite Sheet
            ///
            /// @param Sprite sheet containing the nine patches
            //---------------------------------------------------------
            void SetSpriteSheet(const Rendering::SpriteSheetPtr& inpSpriteSheet);
            //---------------------------------------------------------
            /// Get Sprite Sheet
            ///
            /// @return Sprite sheet containing the nine patches
            //---------------------------------------------------------
			const Rendering::SpriteSheetPtr& GetSpriteSheet() const;
			//---------------------------------------------------------
			/// Set Base Sprite Sheet Index ID
			///
			/// Set the "path" to the sprite sheet index IDs. 
			/// e.g. For the following IDs: 
			///			* "BLUE_PANEL_TOP_LEFT"
			///			* "BLUE_PANEL_TOP_RIGHT"
			///			* "BLUE_PANEL_BOTTOM_LEFT"
			///			* "BLUE_PANEL_BOTTOM_RIGHT"
			///			* "BLUE_PANEL_TOP_CENTRE"
			///			* "BLUE_PANEL_BOTTOM_CENTRE"
			///			* "BLUE_PANEL_MIDDLE_CENTRE"
			///			* "BLUE_PANEL_MIDDLE_RIGHT"
			///			* "BLUE_PANEL_MIDDLE_LEFT"
			///
			/// the base ID would be "BLUE_PANEL_"
			//---------------------------------------------------------
			void SetBaseSpriteSheetIndexID(const std::string& instrID);
			//---------------------------------------------------------
			/// Get Base Sprite Sheet Index ID
			///
			/// Get the "path" to the sprite sheet index IDs. 
			/// e.g. For the following IDs: 
			///			* "BLUE_PANEL_TOP_LEFT"
			///			* "BLUE_PANEL_TOP_RIGHT"
			///			* "BLUE_PANEL_BOTTOM_LEFT"
			///			* "BLUE_PANEL_BOTTOM_RIGHT"
			///			* "BLUE_PANEL_TOP_CENTRE"
			///			* "BLUE_PANEL_BOTTOM_CENTRE"
			///			* "BLUE_PANEL_MIDDLE_CENTRE"
			///			* "BLUE_PANEL_MIDDLE_RIGHT"
			///			* "BLUE_PANEL_MIDDLE_LEFT"
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
            /// Top Left
            /// Top Right
            /// Bottom Left
            /// Bottom Right
            /// Top Centre
            /// Bottom Centre
            /// Left Centre
            /// Right Centre
            /// Middle Centre
            ///
            /// @param Array containing the tpage index of each patch
            //---------------------------------------------------------
            void SetSpriteSheetIndices(const u32* inpIndices);
            //---------------------------------------------------------
            /// Draw
            /// 
            /// Draw the image constructed from the nine patches
            ///
            /// @param Canvas renderer pointer
            //---------------------------------------------------------
            void Draw(Rendering::CCanvasRenderer* inpCanvas);
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
            /// Enable Centre Touch Consumption
            ///
            /// Enables the touch to go through the middle part of the image
            ///
            /// @param boolean to disable or enable
            //--------------------------------------------------------
            void EnableCentreTouchConsumption(bool inbEnabled);
			//--------------------------------------------------------
            /// Is Centre Touch Consumption Enabled
            ///
            /// @return whether the touch though the middle is enabled or not
            //--------------------------------------------------------
            bool IsCentreTouchConsumptionEnabled() const;
			
			//---Touch Delegates
			//-----------------------------------------------------------
			/// On Touch Began
			///
			/// Called when the window receives cursor/touch input
			///
			/// @param Touch data
			/// @return Whether touch has been consumed
			//-----------------------------------------------------------
			virtual bool OnTouchBegan(const Input::TouchInfo & insTouchInfo);
			//-----------------------------------------------------------
			/// On Touch Moved
			///
			/// Called when the window receives cursor/touch input
			///
			/// @param Touch data
			/// @return Whether touch has been consumed
			//-----------------------------------------------------------
			virtual bool OnTouchMoved(const Input::TouchInfo & insTouchInfo);
            
            struct PatchSize
            {
				moFlo::Core::CVector2 vSizeTopLeft;
				moFlo::Core::CVector2 vSizeTopRight;
				moFlo::Core::CVector2 vSizeBottomLeft;
				moFlo::Core::CVector2 vSizeBottomRight;
				moFlo::Core::CVector2 vSizeTopCentre;
				moFlo::Core::CVector2 vSizeBottomCentre;
				moFlo::Core::CVector2 vSizeLeftCentre;
				moFlo::Core::CVector2 vSizeRightCentre;
				moFlo::Core::CVector2 vSizeMiddleCentre;
            };
			
			void CalculatePatchSize(PatchSize& outSizePatch);
			
		private:

			DECLARE_PROPERTY_A(Rendering::SpriteSheetPtr, SpriteSheet, SetSpriteSheet, GetSpriteSheet);

			DECLARE_PROPERTY_A(std::string, BaseSpriteSheetIndexID, SetBaseSpriteSheetIndexID, GetBaseSpriteSheetIndexID);

			DECLARE_PROPERTY_A(bool, HeightMaintain, EnableHeightMaintainingAspect, IsHeightMaintainingAspectEnabled);
			DECLARE_PROPERTY_A(bool, WidthMaintain, EnableWidthMaintainingAspect, IsWidthMaintainingAspectEnabled);
			DECLARE_PROPERTY_A(bool, CentreTouchConsumption, EnableCentreTouchConsumption, IsCentreTouchConsumptionEnabled);
			
            SpriteSheetIndex msIndices;
        };
    }
}

#endif