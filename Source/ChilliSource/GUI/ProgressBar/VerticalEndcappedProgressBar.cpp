//
//  VerticalEndcappedProgressBar.cpp
//  iOSTemplate
//
//  Created by Thomas Kronberg on 12/02/2013.
//  Copyright (c) 2013 Tag Games. All rights reserved.
//

#include <ChilliSource/GUI/ProgressBar/VerticalEndcappedProgressBar.h>

#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/Core/Resource/ResourceManagerDispenser.h>
#include <ChilliSource/Rendering/Sprite/SpriteSheet.h>
#include <ChilliSource/Rendering/Sprite/SpriteSheetManager.h>

namespace ChilliSource
{
	namespace GUI
	{
		DEFINE_META_CLASS(VerticalEndcappedProgressBar)
		
		DEFINE_PROPERTY(SpriteSheet);
		DEFINE_PROPERTY(BaseFillSpriteSheetIndexID);
		DEFINE_PROPERTY(BaseBackgroundSpriteSheetIndexID);
		DEFINE_PROPERTY(FillColour);
        
		DEFINE_PROPERTY(BackgroundColour);
		DEFINE_PROPERTY(WidthFromImage);
        
		VerticalEndcappedProgressBar::VerticalEndcappedProgressBar() : WidthFromImage(false)
		{
            
			mpBGImage = VerticalStretchableImageSPtr(new VerticalStretchableImage());
			mpBGImage->SetLocalAlignment(Rendering::AlignmentAnchor::k_bottomCentre);
			mpBGImage->EnableAlignmentToParent(true);
			mpBGImage->SetAlignmentToParent(Rendering::AlignmentAnchor::k_bottomCentre);
			AddSubview(mpBGImage);
			
			mpFillImage = VerticalStretchableImageSPtr(new VerticalStretchableImage());
			mpFillImage->SetLocalAlignment(Rendering::AlignmentAnchor::k_bottomCentre);
			mpFillImage->EnableAlignmentToParent(true);
			mpFillImage->SetAlignmentToParent(Rendering::AlignmentAnchor::k_bottomCentre);
			AddSubview(mpFillImage);
		}
		//------------------------------------------------------
		/// Constructor
		///
		/// From param dictionary
		//------------------------------------------------------
		VerticalEndcappedProgressBar::VerticalEndcappedProgressBar(const Core::ParamDictionary& insParams)
		: ProgressBar(insParams), WidthFromImage(false)
		{
            
			mpBGImage = VerticalStretchableImageSPtr(new VerticalStretchableImage());
			mpBGImage->SetLocalAlignment(Rendering::AlignmentAnchor::k_bottomCentre);
			mpBGImage->EnableAlignmentToParent(true);
			mpBGImage->SetAlignmentToParent(Rendering::AlignmentAnchor::k_bottomCentre);
			AddSubview(mpBGImage);
			
			mpFillImage = VerticalStretchableImageSPtr(new VerticalStretchableImage());
			mpFillImage->SetLocalAlignment(Rendering::AlignmentAnchor::k_bottomCentre);
			mpFillImage->EnableAlignmentToParent(true);
			mpFillImage->SetAlignmentToParent(Rendering::AlignmentAnchor::k_bottomCentre);
			AddSubview(mpFillImage);
			
			std::string strValue;
			
			//---Sprite sheet
            Core::StorageLocation eSpriteSheetLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("SpriteSheetLocation", strValue))
            {
                eSpriteSheetLocation = ChilliSource::Core::ParseStorageLocation(strValue);
            }
			if(insParams.TryGetValue("SpriteSheet", strValue))
			{
				SetSpriteSheet(LOAD_RESOURCE(Rendering::SpriteSheet, eSpriteSheetLocation, strValue));
			}
			//---Sprite sheet ID
			if(insParams.TryGetValue("BaseFillSpriteSheetIndexID", strValue))
			{
				SetBaseFillSpriteSheetIndexID(strValue);
			}
			if(insParams.TryGetValue("BaseBackgroundSpriteSheetIndexID", strValue))
			{
				SetBaseBackgroundSpriteSheetIndexID(strValue);
			}
            
            if( insParams.TryGetValue("BaseBackgroundVisible", strValue) )
            {
                mpBGImage->SetVisible( Core::ParseBool(strValue)  );
            }
            //---Width from image
            if(insParams.TryGetValue("WidthFromImage", strValue))
            {
                WidthFromImage = Core::ParseBool(strValue);
            }
            
			//---Colours
			if(insParams.TryGetValue("FillColour", strValue))
			{
				SetFillColour(Core::ParseColour(strValue));
			}
			if(insParams.TryGetValue("BackgroundColour", strValue))
			{
				SetBackgroundColour(Core::ParseColour(strValue));
			}
            
		}
		//------------------------------------------------------------------------
		/// Set Sprite Sheet
		///
		/// @param Sprite Sheet
		//------------------------------------------------------------------------
		void VerticalEndcappedProgressBar::SetSpriteSheet(const Rendering::SpriteSheetSPtr& inpSpriteSheet)
		{
			SpriteSheet = inpSpriteSheet;
			mpBGImage->SetSpriteSheet(inpSpriteSheet);
			mpFillImage->SetSpriteSheet(inpSpriteSheet);
		}
		//------------------------------------------------------------------------
		/// Get Sprite Sheet
		///
		/// @return Sprite Sheet
		//------------------------------------------------------------------------
		const Rendering::SpriteSheetSPtr& VerticalEndcappedProgressBar::GetSpriteSheet() const
		{
			return SpriteSheet;
		}
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
		void VerticalEndcappedProgressBar::SetBaseFillSpriteSheetIndexID(const std::string& instrID)
		{
			BaseFillSpriteSheetIndexID = instrID;
			mpFillImage->SetBaseSpriteSheetIndexID(instrID);
		}
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
		const std::string& VerticalEndcappedProgressBar::GetBaseFillSpriteSheetIndexID() const
		{
			return BaseFillSpriteSheetIndexID;
		}
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
		void VerticalEndcappedProgressBar::SetBaseBackgroundSpriteSheetIndexID(const std::string& instrID)
		{
			BaseBackgroundSpriteSheetIndexID = instrID;
			mpBGImage->SetBaseSpriteSheetIndexID(instrID);
		}
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
		const std::string& VerticalEndcappedProgressBar::GetBaseBackgroundSpriteSheetIndexID() const
		{
			return BaseBackgroundSpriteSheetIndexID;
		}
		//------------------------------------------------------------------------
		/// Set Fill Indices
		///
		/// @param Sprite sheet index of left patch
		/// @param Sprite sheet index of centre patch
		/// @param Sprite sheet index of right patch
		//------------------------------------------------------------------------
		void VerticalEndcappedProgressBar::SetFillIndices(u32 inudwTop, u32 inudwMid, u32 inudwBottom)
		{
            mpFillImage->SetSpriteSheetIndices(inudwTop, inudwMid, inudwBottom);
		}
		//------------------------------------------------------------------------
		/// Set Background Indices
		///
		/// @param Sprite sheet index of left patch
		/// @param Sprite sheet index of centre patch
		/// @param Sprite sheet index of right patch
		//------------------------------------------------------------------------
		void VerticalEndcappedProgressBar::SetBackgroundIndices(u32 inudwTop, u32 inudwMid, u32 inudwBottom)
		{
            mpBGImage->SetSpriteSheetIndices(inudwTop, inudwMid, inudwBottom);
		}
		//------------------------------------------------------------------------
		/// Set Fill Colour
		///
		/// @param Colour of progress bar
		//------------------------------------------------------------------------
		void VerticalEndcappedProgressBar::SetFillColour(const Core::CColour& inColour)
		{
			FillColour = inColour;
			mpFillImage->SetColour(inColour);
		}
		//------------------------------------------------------------------------
		/// Get Fill Colour
		///
		/// @return Colour of progress bar
		//------------------------------------------------------------------------
		const Core::CColour& VerticalEndcappedProgressBar::GetFillColour() const
		{
			return FillColour;
		}
		//------------------------------------------------------------------------
		/// Set Background Colour
		///
		/// @param Colour of background
		//------------------------------------------------------------------------
		void VerticalEndcappedProgressBar::SetBackgroundColour(const Core::CColour& inColour)
		{
			BackgroundColour = inColour;
			mpBGImage->SetColour(inColour);
		}
		//------------------------------------------------------------------------
		/// Get Background Colour
		///
		/// @return Colour of background
		//------------------------------------------------------------------------
		const Core::CColour& VerticalEndcappedProgressBar::GetBackgroundColour() const
		{
			return BackgroundColour;
		}
		//--------------------------------------------------------
		/// Enable Width From Image
		///
		/// When this is enabled the image view's height will be
		/// based on the height of the cap
		///
		/// @param Enable/disable
		//--------------------------------------------------------
		void VerticalEndcappedProgressBar::EnableWidthFromImage(bool inbEnable)
		{
			
		}
		//--------------------------------------------------------
		/// Is Width From Image Enabled
		///
		/// When this is enabled the image view's size will be
		/// based on the size of the image
		///
		/// @return Whether the image view's size will be
		/// based on the size of the image
		//--------------------------------------------------------
		bool VerticalEndcappedProgressBar::IsWidthFromImageEnabled() const
		{
			return WidthFromImage;
		}
		//------------------------------------------------------------------------
		/// Draw
		///
		/// Draw this view and any subviews
		///
		/// @param Renderer
		//------------------------------------------------------------------------
		void VerticalEndcappedProgressBar::Draw(Rendering::CanvasRenderer* inpCanvas)
		{
			if(Visible && SpriteSheet)
			{
                
				Core::CVector2 vMyAbsSize = GetAbsoluteSize();
				
				f32 fFullProgressHeight = vMyAbsSize.y - mpBGImage->GetCombinedCapHeight();
				f32 fFillHeight = GetProgress() * fFullProgressHeight + mpFillImage->GetCombinedCapHeight() - 2.0f;
				
				f32 fFillOffsetY = (mpBGImage->GetCombinedCapHeight() - mpFillImage->GetCombinedCapHeight()) * 0.5f + 1.0f;
				
				mpFillImage->SetOffsetFromParentAlignment(Core::UnifiedVector2(0,0,0,fFillOffsetY));
				
				f32 fFillImageWidth = mpFillImage->GetCapWidth();
				f32 fBGImageWidth = mpBGImage->GetCapWidth();
				
				if (WidthFromImage)
				{
					vMyAbsSize.x = fBGImageWidth;
				}
				
				f32 fFillDrawWidth = vMyAbsSize.x - (fBGImageWidth-fFillImageWidth);
				
				mpBGImage->SetSize(Core::UnifiedVector2(Core::CVector2::ZERO,vMyAbsSize));
				mpBGImage->Draw(inpCanvas);
				
				mpFillImage->SetSize(Core::UnifiedVector2(Core::CVector2::ZERO,Core::CVector2(fFillDrawWidth,fFillHeight)));
				mpFillImage->Draw(inpCanvas);
			}
            
            ProgressBar::Draw(inpCanvas);
		}
	}
}
