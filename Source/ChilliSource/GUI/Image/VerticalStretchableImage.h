//
//  StretchableImage.h
//  moFlo
//
//  Created by Scott Downie on 28/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_GUI_VerticalStretchableImageVIEW_H_
#define _CHILLISOURCE_GUI_VerticalStretchableImageVIEW_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Math/Geometry/Shapes.h>
#include <ChilliSource/GUI/Base/GUIView.h>
#include <ChilliSource/Rendering/Texture/UVs.h>

//=============================================================
/// GUI - Stretchable Image 
///
/// Creates a scalable image using the nine patch system
//=============================================================

namespace ChilliSource
{
    namespace GUI
    {
        class VerticalStretchableImage : public GUIView
        {
        public:

			DECLARE_META_CLASS(VerticalStretchableImage)

            struct PanelDesc
            {
                Rendering::UVs m_topUVs;
                Rendering::UVs m_middleUVs;
                Rendering::UVs m_bottomUVs;
                
                Core::Vector2 m_topSize;
                Core::Vector2 m_middleSize;
                Core::Vector2 m_bottomSize;
            };
            VerticalStretchableImage();
            VerticalStretchableImage(const Core::ParamDictionary& insParams);
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
			///			* "BLUE_PANEL_TOP"
			///			* "BLUE_PANEL_BOTTOM"
			///			* "BLUE_PANEL_MIDDLE"
			///
			/// the base ID would be "BLUE_PANEL_"
			//---------------------------------------------------------
			void SetBaseTextureAtlasID(const std::string& instrID);
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
			const std::string& GetBaseTextureAtlasID() const;
			//---------------------------------------------------------
			/// Set Patch Sprite Sheet Indices
			///
			/// @param Sprite sheet id of top patch
			/// @param Sprite sheet id of middle patch
			/// @param Sprite sheet id of bottom patch
			//---------------------------------------------------------
			void SetTextureAtlasIds(const std::string& in_top, const std::string& in_middle, const std::string& in_bottom);
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

            DECLARE_PROPERTY_A(Rendering::TextureCSPtr, Texture, SetTexture, GetTexture);
			DECLARE_PROPERTY_A(Rendering::TextureAtlasCSPtr, TextureAtlas, SetTextureAtlas, GetTextureAtlas);

			DECLARE_PROPERTY_A(std::string, BaseTextureAtlasID, SetBaseTextureAtlasID, GetBaseTextureAtlasID);

			DECLARE_PROPERTY_A(bool, HeightMaintain, EnableHeightMaintainingAspect, IsHeightMaintainingAspectEnabled);
			DECLARE_PROPERTY_A(bool, WidthMaintain, EnableWidthMaintainingAspect, IsWidthMaintainingAspectEnabled);
			
            PanelDesc m_panels;
        };
    }
}

#endif