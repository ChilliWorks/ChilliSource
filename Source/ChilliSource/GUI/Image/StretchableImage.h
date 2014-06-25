//
//  StretchableImage.h
//  Chilli Source
//  Created by Scott Downie on 28/04/2011
//
//  The MIT License (MIT)
//
//  Copyright (c) 2011 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#ifndef _CHILLISOURCE_GUI_STRETCHABLEIMAGEVIEW_H_
#define _CHILLISOURCE_GUI_STRETCHABLEIMAGEVIEW_H_

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
        class StretchableImage : public GUIView
        {
        public:

			DECLARE_META_CLASS(StretchableImage)

            struct PanelDesc
            {
                Rendering::UVs m_topLeftUVs;
                Rendering::UVs m_topCentreUVs;
                Rendering::UVs m_topRightUVs;
                
                Rendering::UVs m_bottomLeftUVs;
                Rendering::UVs m_bottomCentreUVs;
                Rendering::UVs m_bottomRightUVs;
                
                Rendering::UVs m_leftCentreUVs;
                Rendering::UVs m_middleCentreUVs;
                Rendering::UVs m_rightCentreUVs;
                
                Core::Vector2 m_topLeftSize;
                Core::Vector2 m_topCentreSize;
                Core::Vector2 m_topRightSize;
                
                Core::Vector2 m_bottomLeftSize;
                Core::Vector2 m_bottomCentreSize;
                Core::Vector2 m_bottomRightSize;
                
                Core::Vector2 m_leftCentreSize;
                Core::Vector2 m_middleCentreSize;
                Core::Vector2 m_rightCentreSize;
            };
            
            StretchableImage();
            StretchableImage(const Core::ParamDictionary& insParams);
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
			
			//---Touch Delegates
            
            struct PatchSize
            {
				CSCore::Vector2 vSizeTopLeft;
				CSCore::Vector2 vSizeTopRight;
				CSCore::Vector2 vSizeBottomLeft;
				CSCore::Vector2 vSizeBottomRight;
				CSCore::Vector2 vSizeTopCentre;
				CSCore::Vector2 vSizeBottomCentre;
				CSCore::Vector2 vSizeLeftCentre;
				CSCore::Vector2 vSizeRightCentre;
				CSCore::Vector2 vSizeMiddleCentre;
            };
			
			void CalculatePatchSize(PatchSize& outSizePatch);
			
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