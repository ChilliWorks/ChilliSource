//
//  StretchableImage.cpp
//  moFlo
//
//  Created by Scott Downie on 28/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/Rendering/GUI/Defaults/VerticalStretchableImage.h>

#include <ChilliSource/Rendering/Sprite/SpriteSheet.h>
#include <ChilliSource/Rendering/Sprite/SpriteSheetManager.h>
#include <ChilliSource/Rendering/Base/CanvasRenderer.h>

#include <ChilliSource/Core/Resource/ResourceManagerDispenser.h>
#include <ChilliSource/Core/Base/Screen.h>

namespace moFlo
{
    namespace GUI
    {
		DEFINE_META_CLASS(CVerticalStretchableImage)

		DEFINE_PROPERTY(SpriteSheet);
		DEFINE_PROPERTY(HeightMaintain);
		DEFINE_PROPERTY(WidthMaintain);
		DEFINE_PROPERTY(BaseSpriteSheetIndexID);

		//--------------------------------------------------------
        /// Constructor
        /// 
        /// Empty
        //---------------------------------------------------------
        CVerticalStretchableImage::CVerticalStretchableImage()
			: HeightMaintain(false), WidthMaintain(false)
        {
			memset(&msIndices, 0, sizeof(u32) * 3);
        }
        //---------------------------------------------------------
        /// Constructor
        ///
        /// From param dictionary
        //---------------------------------------------------------
        CVerticalStretchableImage::CVerticalStretchableImage(const Core::ParamDictionary& insParams) 
			: CGUIView(insParams), HeightMaintain(false), WidthMaintain(false)
        {
            std::string strValue;

			memset(&msIndices, 0, sizeof(u32) * 3);
            
            //---Sprite sheet
            Core::StorageLocation eSpriteSheetLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("SpriteSheetLocation", strValue))
            {
                eSpriteSheetLocation = moFlo::Core::CStringConverter::ParseStorageLocation(strValue);
            }
            if(insParams.TryGetValue("SpriteSheet", strValue))
            {
				SetSpriteSheet(LOAD_RESOURCE(Rendering::CSpriteSheet, eSpriteSheetLocation, strValue));
			}

			//---Sprite sheet base name
			if(insParams.TryGetValue("BaseSpriteSheetIndexID", strValue))
            {
				SetBaseSpriteSheetIndexID(strValue);
			}

            //---Sprite sheet indices
            if(insParams.TryGetValue("Top", strValue))
            {
                msIndices.udwTop = Core::CStringConverter::ParseUnsignedInt(strValue);
            }
            if(insParams.TryGetValue("Bottom", strValue))
            {
                msIndices.udwBottom = Core::CStringConverter::ParseUnsignedInt(strValue);
            }
            if(insParams.TryGetValue("Middle", strValue))
            {
                msIndices.udwMiddle = Core::CStringConverter::ParseUnsignedInt(strValue);
            }
			//---Maintain Width
			if(insParams.TryGetValue("HeightMaintain", strValue))
			{
				HeightMaintain = Core::CStringConverter::ParseBool(strValue);
			}
			//---Maintain Height
			if(insParams.TryGetValue("WidthMaintain", strValue))
			{
				WidthMaintain = Core::CStringConverter::ParseBool(strValue);
			}
			//---Set Maintain Width
			if(insParams.TryGetValue("SetHeightMaintain", strValue))
			{
				Core::CVector2 vSize = Core::CStringConverter::ParseVector2(strValue);
				SetHeightMaintainingAspect(vSize.x, vSize.y);
			}
			//---Set Maintain Height
			if(insParams.TryGetValue("SetWidthMaintain", strValue))
			{
				Core::CVector2 vSize = Core::CStringConverter::ParseVector2(strValue);
				SetWidthMaintainingAspect(vSize.x, vSize.y);
			}
        }
        //---------------------------------------------------------
        /// Set Sprite Sheet
        ///
        /// @param Sprite sheet containing the nine patches
        //---------------------------------------------------------
        void CVerticalStretchableImage::SetSpriteSheet(const Rendering::SpriteSheetPtr& inpSpriteSheet)
        {
            SpriteSheet = inpSpriteSheet;
        }
		//---------------------------------------------------------
		/// Get Sprite Sheet
		///
		/// @return Sprite sheet containing the nine patches
		//---------------------------------------------------------
		const Rendering::SpriteSheetPtr& CVerticalStretchableImage::GetSpriteSheet() const
		{
			return SpriteSheet;
		}
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
		void CVerticalStretchableImage::SetBaseSpriteSheetIndexID(const std::string& instrID)
		{
			if(SpriteSheet)
			{
				BaseSpriteSheetIndexID = instrID;

				msIndices.udwTop = SpriteSheet->GetFrameIndexByID(instrID + "TOP");
				msIndices.udwBottom = SpriteSheet->GetFrameIndexByID(instrID + "BOTTOM");
				msIndices.udwMiddle = SpriteSheet->GetFrameIndexByID(instrID + "MIDDLE");
			}
		}
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
		const std::string& CVerticalStretchableImage::GetBaseSpriteSheetIndexID() const
		{
			return BaseSpriteSheetIndexID;
		}
		//---------------------------------------------------------
        /// Set Patch Sprite Sheet Indices
        ///
        /// @param Struct containing the tpage index of each patch
        //---------------------------------------------------------
        void CVerticalStretchableImage::SetSpriteSheetIndices(const CVerticalStretchableImage::SpriteSheetIndex& insIndices)
        {
            msIndices = insIndices;
        }
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
        void CVerticalStretchableImage::SetSpriteSheetIndices(const u32* inpIndices)
        {
            msIndices.udwTop = inpIndices[0];
            msIndices.udwBottom = inpIndices[1];
            msIndices.udwMiddle = inpIndices[2];
        }
		//---------------------------------------------------------
		/// Set Patch Sprite Sheet Indices
		///
		/// @param Sprite sheet index of top patch
		/// @param Sprite sheet index of middle patch
		/// @param Sprite sheet index of bottom patch
		//---------------------------------------------------------
		void CVerticalStretchableImage::SetSpriteSheetIndices(u32 inudwTop, u32 inudwMid, u32 inudwBottom)
		{
			msIndices.udwTop = inudwTop;
			msIndices.udwMiddle = inudwMid;
			msIndices.udwBottom = inudwBottom;
		}
        //---------------------------------------------------------
        /// Draw
        /// 
        /// Draw the image constructed from the nine patches
        ///
        /// @param Canvas renderer pointer
        //---------------------------------------------------------
        void CVerticalStretchableImage::Draw(Rendering::CCanvasRenderer* inpCanvas)
        {
			//Check if this is on screen
			Core::CVector2 vTopRight = GetAbsoluteScreenSpaceAnchorPoint(Core::AlignmentAnchor::k_topRight);
			Core::CVector2 vBottomLeft = GetAbsoluteScreenSpaceAnchorPoint(Core::AlignmentAnchor::k_bottomLeft);
			
			if(vTopRight.y < 0 || vBottomLeft.y > Core::CScreen::GetOrientedHeight() || vTopRight.x < 0 || vBottomLeft.x > Core::CScreen::GetOrientedWidth())
			{
				//Offscreen
				return;
			}
			
            if(Visible && SpriteSheet)
            {			
                Core::CVector2 vPanelSize = GetAbsoluteSize();
                Core::CVector2 vPanelPos = GetAbsoluteScreenSpacePosition();
                Core::CVector2 vTopLeft = GetAbsoluteAnchorPoint(Core::AlignmentAnchor::k_topLeft);
                Core::CVector2 vPatchPos;
                
                Core::CColour AbsColour = GetAbsoluteColour();
                
                //We need to use a matrix so that we can rotate all the patches with respect
                //to the view
                Core::CMatrix3x3 matTransform;
                Core::CMatrix3x3 matPatchTransform;
                Core::CMatrix3x3 matViewTransform;
                
                matViewTransform.SetTransform(vPanelPos, Core::CVector2(1, 1), GetRotation());
                
                //Get the patch sizes
                Core::CVector2 vTPatchSize = SpriteSheet->GetSizeForFrame(msIndices.udwTop);
                Core::CVector2 vBPatchSize = SpriteSheet->GetSizeForFrame(msIndices.udwBottom);
                
                vTPatchSize.x = vPanelSize.x;
                vBPatchSize.x = vPanelSize.x;

                Core::CVector2 vTCPatchSize;
                Core::CVector2 vBCPatchSize;
                Core::CVector2 vMCPatchSize;
                
                //Check to see if they are going to fit in the bounds of the view
                f32 fTotal = vTPatchSize.y + vBPatchSize.y;
                if(fTotal > vPanelSize.y)
                {
                    //The corners are too tall let's
                    //squish them
                    f32 fScale = vPanelSize.y/fTotal;
                    vTPatchSize.y *= fScale;
                    vBPatchSize.y *= fScale;
                }

                fTotal = vTPatchSize.y;
                if(fTotal > vPanelSize.y)
                {
                    //The corners are too tall let's
                    //squish them
                    f32 fScale = vPanelSize.y/fTotal;
                    vTPatchSize.y *= fScale;
                }
                fTotal = vBPatchSize.y;
                if(fTotal > vPanelSize.y)
                {
                    //The corners are too tall let's
                    //squish them
                    f32 fScale = vPanelSize.y/fTotal;
                    vBPatchSize.y *= fScale;
                }
                
                //Render ourself
                //Draw the corners first
                matPatchTransform.Translate(vTopLeft);
                
                Core::CMatrix3x3::Multiply(&matPatchTransform, &matViewTransform, &matTransform);
                inpCanvas->DrawBox(matTransform,
                                   vTPatchSize, 
								   SpriteSheet->GetTexture(),
                                   SpriteSheet->GetUVsForFrame(msIndices.udwTop), 
                                   AbsColour, 
                                   Core::AlignmentAnchor::k_topLeft);
                
                
                matPatchTransform.Translate(GetAbsoluteAnchorPoint(Core::AlignmentAnchor::k_bottomLeft));
                Core::CMatrix3x3::Multiply(&matPatchTransform, &matViewTransform, &matTransform);
                inpCanvas->DrawBox(matTransform, 
                                   vBPatchSize, 
								   SpriteSheet->GetTexture(),
                                   SpriteSheet->GetUVsForFrame(msIndices.udwBottom), 
                                   AbsColour, 
                                   Core::AlignmentAnchor::k_bottomLeft);

 
                //Draw the centre
                vMCPatchSize.x = vTPatchSize.x;
                vMCPatchSize.y = vPanelSize.y - (vTPatchSize.y + vBPatchSize.y);
                vPatchPos.x = vTopLeft.x;
                vPatchPos.y = vTopLeft.y - vTPatchSize.y;
               
                matPatchTransform.Translate(vPatchPos);
                Core::CMatrix3x3::Multiply(&matPatchTransform, &matViewTransform, &matTransform);
                inpCanvas->DrawBox(matTransform,
                                   vMCPatchSize, 
								   SpriteSheet->GetTexture(),
                                   SpriteSheet->GetUVsForFrame(msIndices.udwMiddle), 
                                   AbsColour, 
                                   Core::AlignmentAnchor::k_topLeft);
                
                //Render subviews
                CGUIView::Draw(inpCanvas);
            }
		}
		//--------------------------------------------------------
		/// Set Width Maintaining Aspect
		///
		/// Change the width of the image and resize the height
		/// to maintain the aspect ratio
		///
		/// @param Unified width
		//--------------------------------------------------------
		void CVerticalStretchableImage::SetWidthMaintainingAspect(f32 infRelWidth, f32 infAbsWidth)
		{
            Core::CVector2 vCurrentSize = GetAbsoluteSize();
			f32 fAspectRatio = vCurrentSize.y / vCurrentSize.x;
			SetSize(infRelWidth, 0.0f, infAbsWidth, 0.0f);
			
			f32 fScaleY = GetAbsoluteScale().y;
			if(fScaleY == 0.0f)
				return;
			
			vCurrentSize = GetAbsoluteSize();
            f32 fAbsHeight = (fAspectRatio * vCurrentSize.x) / fScaleY;
			SetSize(infRelWidth, 0.0f, infAbsWidth, fAbsHeight);
		}
		//--------------------------------------------------------
		/// Set Height Maintaining Aspect
		///
		/// Change the height of the image and resize the width
		/// to maintain the aspect ratio
		///
		/// @param Unified height
		//--------------------------------------------------------
		void CVerticalStretchableImage::SetHeightMaintainingAspect(f32 infRelHeight, f32 infAbsHeight)
		{
            Core::CVector2 vCurrentSize = GetAbsoluteSize();
			f32 fAspectRatio = vCurrentSize.x / vCurrentSize.y;
			SetSize(0.0f, infRelHeight, 0.0f, infAbsHeight);
			
			f32 fScaleX = GetAbsoluteScale().x;
			if(fScaleX == 0.0f)
				return;
			
			vCurrentSize = GetAbsoluteSize();
            f32 fAbsWidth = (fAspectRatio * vCurrentSize.y) / fScaleX;
			SetSize(0.0f, infRelHeight, fAbsWidth, infAbsHeight);
		}
		//--------------------------------------------------------
		/// Enable Height Maintaining Aspect
		///
		/// Enables auto scaling of the height to maintain the aspect ratio
		///
		/// @param boolean to disable or enable
		//--------------------------------------------------------
		void CVerticalStretchableImage::EnableHeightMaintainingAspect(bool inbEnabled)
		{
			HeightMaintain = inbEnabled;
		}
		//--------------------------------------------------------
		/// Enable Width Maintaining Aspect
		///
		/// Enables auto scaling of the height to maintain the aspect ratio
		///
		/// @param boolean to disable or enable
		//--------------------------------------------------------
		void CVerticalStretchableImage::EnableWidthMaintainingAspect(bool inbEnabled)
		{
			WidthMaintain = inbEnabled;            
		}
		//--------------------------------------------------------
		/// Is Width Maintaining Aspect Enabled
		///
		/// @return auto scaling of the Width to maintain the aspect ratio
		//--------------------------------------------------------
		bool CVerticalStretchableImage::IsWidthMaintainingAspectEnabled() const
		{
			return WidthMaintain;
		}
		//--------------------------------------------------------
		/// Is Height Maintaining Aspect Enabled
		///
		/// @return auto scaling of the height to maintain the aspect ratio
		//--------------------------------------------------------
		bool CVerticalStretchableImage::IsHeightMaintainingAspectEnabled() const
		{
			return HeightMaintain;
		}
		//--------------------------------------------------------
		/// Get Combined Cap Height
		///
		/// @param Sum of the widths of both end caps
		//--------------------------------------------------------
		f32 CVerticalStretchableImage::GetCombinedCapHeight() const
		{
			return SpriteSheet->GetSizeForFrame(msIndices.udwTop).y + SpriteSheet->GetSizeForFrame(msIndices.udwBottom).y;
		}
		//--------------------------------------------------------
		/// Get Combined Cap Width
		///
		/// @param Sum of the heights of both end caps
		//--------------------------------------------------------
		f32 CVerticalStretchableImage::GetCapWidth() const
		{
			return SpriteSheet->GetSizeForFrame(msIndices.udwTop).x;
		}
    }
}
