//
//  StretchableImage.h
//  moFlo
//
//  Created by Scott Downie on 28/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _MOFLO_GUI_STRETCHABLEIMAGEVIEW_H_
#define _MOFLO_GUI_STRETCHABLEIMAGEVIEW_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/GUI/Base/GUIView.h>

//=============================================================
/// GUI - Stretchable Image 
///
/// Creates a scalable image using the nine patch system
//=============================================================

namespace ChilliSource
{
    namespace GUI
    {
        class StretchableImage : public GUIView
        {
        public:

			DECLARE_META_CLASS(StretchableImage)

            struct TextureAtlasIndex
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
            StretchableImage();
            StretchableImage(const Core::ParamDictionary& insParams);
            //---------------------------------------------------------
            /// Set Texture
            ///
            /// @param Texture containing the nine patches
            //---------------------------------------------------------
            inline void SetTexture(const Rendering::TextureSPtr& inpTexture)
            {
                Texture = inpTexture;
            }
            //---------------------------------------------------------
            /// Get Texture
            ///
            /// @return Texture containing the nine patches
            //---------------------------------------------------------
			inline const Rendering::TextureSPtr& GetTexture() const
            {
                return Texture;
            }
            //---------------------------------------------------------
            /// Set Sprite Sheet
            ///
            /// @param Sprite sheet containing the nine patches
            //---------------------------------------------------------
            void SetTextureAtlas(const Rendering::TextureAtlasCSPtr& inpTextureAtlas);
            //---------------------------------------------------------
            /// Get Sprite Sheet
            ///
            /// @return Sprite sheet containing the nine patches
            //---------------------------------------------------------
			const Rendering::TextureAtlasCSPtr& GetTextureAtlas() const;
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
			void SetBaseTextureAtlasID(const std::string& instrID);
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
			const std::string& GetBaseTextureAtlasID() const;
            //---------------------------------------------------------
            /// Set Patch Sprite Sheet Indices
            ///
            /// @param Struct containing the tpage index of each patch
            //---------------------------------------------------------
            void SetTextureAtlasIndices(const TextureAtlasIndex& insIndices);
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
            void SetTextureAtlasIndices(const u32* inpIndices);
            //---------------------------------------------------------
            /// Draw
            /// 
            /// Draw the image constructed from the nine patches
            ///
            /// @param Canvas renderer pointer
            //---------------------------------------------------------
            void Draw(Rendering::CanvasRenderer* inpCanvas);
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
			/// Called when the window receives cursor/touch input
			///
            /// @author S Downie
            ///
			/// @param The pointer.
            /// @param The timestamp
            /// @param The press type.
            ///
			/// @return Whether touch has been consumed
			//-----------------------------------------------------------
			virtual bool OnPointerDown(const Input::PointerSystem::Pointer& in_pointer, f64 in_timestamp, Input::PointerSystem::InputType in_inputType) override;
			//-----------------------------------------------------------
			/// Called when the window receives cursor/touch input
			///
            /// @author S Downie
			///
			/// @param The pointer.
            /// @param The timestamp
            ///
			/// @return Whether touch has been consumed
			//-----------------------------------------------------------
			virtual bool OnPointerMoved(const Input::PointerSystem::Pointer& in_pointer, f64 in_timestamp) override;
            
            struct PatchSize
            {
				ChilliSource::Core::Vector2 vSizeTopLeft;
				ChilliSource::Core::Vector2 vSizeTopRight;
				ChilliSource::Core::Vector2 vSizeBottomLeft;
				ChilliSource::Core::Vector2 vSizeBottomRight;
				ChilliSource::Core::Vector2 vSizeTopCentre;
				ChilliSource::Core::Vector2 vSizeBottomCentre;
				ChilliSource::Core::Vector2 vSizeLeftCentre;
				ChilliSource::Core::Vector2 vSizeRightCentre;
				ChilliSource::Core::Vector2 vSizeMiddleCentre;
            };
			
			void CalculatePatchSize(PatchSize& outSizePatch);
			
		private:

            DECLARE_PROPERTY_A(Rendering::TextureSPtr, Texture, SetTexture, GetTexture);
			DECLARE_PROPERTY_A(Rendering::TextureAtlasCSPtr, TextureAtlas, SetTextureAtlas, GetTextureAtlas);

			DECLARE_PROPERTY_A(std::string, BaseTextureAtlasID, SetBaseTextureAtlasID, GetBaseTextureAtlasID);

			DECLARE_PROPERTY_A(bool, HeightMaintain, EnableHeightMaintainingAspect, IsHeightMaintainingAspectEnabled);
			DECLARE_PROPERTY_A(bool, WidthMaintain, EnableWidthMaintainingAspect, IsWidthMaintainingAspectEnabled);
			DECLARE_PROPERTY_A(bool, CentreTouchConsumption, EnableCentreTouchConsumption, IsCentreTouchConsumptionEnabled);
			
            TextureAtlasIndex msIndices;
        };
    }
}

#endif