/*
 *  HorizontalEndcappedProgressBar.cpp
 *  iOSTemplate
 *
 *  Created by Stuart McGaw on 04/05/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Rendering/GUI/Defaults/HorizontalEndcappedProgressBar.h>

#include <ChilliSource/Core/Resource/ResourceManagerDispenser.h>

#include <ChilliSource/Rendering/Sprite/SpriteSheet.h>
#include <ChilliSource/Rendering/Sprite/SpriteSheetManager.h>

namespace ChilliSource
{
	namespace Rendering
	{
		DEFINE_META_CLASS(CHorizontalEndcappedProgressBar)
		
		DEFINE_PROPERTY(SpriteSheet);
		DEFINE_PROPERTY(BaseFillSpriteSheetIndexID);
		DEFINE_PROPERTY(BaseBackgroundSpriteSheetIndexID);
		DEFINE_PROPERTY(FillColour);
   
		DEFINE_PROPERTY(BackgroundColour);
		DEFINE_PROPERTY(HeightFromImage);

		CHorizontalEndcappedProgressBar::CHorizontalEndcappedProgressBar() : HeightFromImage(false)
		{

			mpBGImage = HorizontalStretchableImagePtr(new CHorizontalStretchableImage());
			mpBGImage->SetLocalAlignment(Core::AlignmentAnchor::k_middleLeft);
			mpBGImage->EnableAlignmentToParent(true);
			mpBGImage->SetAlignmentToParent(Core::AlignmentAnchor::k_middleLeft);
            mpBGImage->EnableTouchConsumption(false);
			AddSubview(mpBGImage);
			
			mpFillImage = HorizontalStretchableImagePtr(new CHorizontalStretchableImage());
			mpFillImage->SetLocalAlignment(Core::AlignmentAnchor::k_middleLeft);
			mpFillImage->EnableAlignmentToParent(true);
			mpFillImage->SetAlignmentToParent(Core::AlignmentAnchor::k_middleLeft);
            mpFillImage->EnableTouchConsumption(false);
			AddSubview(mpFillImage);
		}
		//------------------------------------------------------
		/// Constructor
		///
		/// From param dictionary
		//------------------------------------------------------
		CHorizontalEndcappedProgressBar::CHorizontalEndcappedProgressBar(const Core::ParamDictionary& insParams) 
		: CProgressBar(insParams), HeightFromImage(false)
		{
			mpBGImage = HorizontalStretchableImagePtr(new CHorizontalStretchableImage());
			mpBGImage->SetLocalAlignment(Core::AlignmentAnchor::k_middleLeft);
			mpBGImage->EnableAlignmentToParent(true);
			mpBGImage->SetAlignmentToParent(Core::AlignmentAnchor::k_middleLeft);
            mpBGImage->EnableTouchConsumption(false);
			AddSubview(mpBGImage);
			
			mpFillImage = HorizontalStretchableImagePtr(new CHorizontalStretchableImage());
			mpFillImage->SetLocalAlignment(Core::AlignmentAnchor::k_middleLeft);
			mpFillImage->EnableAlignmentToParent(true);
			mpFillImage->SetAlignmentToParent(Core::AlignmentAnchor::k_middleLeft);
            mpFillImage->EnableTouchConsumption(false);
			AddSubview(mpFillImage);
			
			std::string strValue;
			
			//---Sprite sheet
            Core::StorageLocation eSpriteSheetLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("SpriteSheetLocation", strValue))
            {
                eSpriteSheetLocation = ChilliSource::Core::CStringConverter::ParseStorageLocation(strValue);
            }
			if(insParams.TryGetValue("SpriteSheet", strValue))
			{
				SetSpriteSheet(LOAD_RESOURCE(CSpriteSheet, eSpriteSheetLocation, strValue));
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
                mpBGImage->SetVisible( Core::CStringConverter::ParseBool(strValue)  );
            }
			//---Background indices
			if(insParams.TryGetValue("BackgroundLeftIndex", strValue))
			{
				MOFLOW_ASSERT(SpriteSheet, "Sprite sheet must be set first");
				mpBGImage->SetSpriteSheetLeftIndex(mpBGImage->GetSpriteSheet()->GetFrameIndexByID(strValue));
			}
			if(insParams.TryGetValue("BackgroundCentreIndex", strValue))
			{
				MOFLOW_ASSERT(SpriteSheet, "Sprite sheet must be set first");
				mpBGImage->SetSpriteSheetCentreIndex(mpBGImage->GetSpriteSheet()->GetFrameIndexByID(strValue));
			}	
			if(insParams.TryGetValue("BackgroundRightIndex", strValue))
			{
				MOFLOW_ASSERT(SpriteSheet, "Sprite sheet must be set first");
				mpBGImage->SetSpriteSheetRightIndex(mpBGImage->GetSpriteSheet()->GetFrameIndexByID(strValue));
			}	
			//---Progress indices
			if(insParams.TryGetValue("ProgressLeftIndex", strValue))
			{
				MOFLOW_ASSERT(SpriteSheet, "Sprite sheet must be set first");
				mpFillImage->SetSpriteSheetLeftIndex(mpFillImage->GetSpriteSheet()->GetFrameIndexByID(strValue));
			}
			if(insParams.TryGetValue("ProgressCentreIndex", strValue))
			{
				MOFLOW_ASSERT(SpriteSheet, "Sprite sheet must be set first");
				mpFillImage->SetSpriteSheetCentreIndex(mpFillImage->GetSpriteSheet()->GetFrameIndexByID(strValue));
			}	
			if(insParams.TryGetValue("ProgressRightIndex", strValue))
			{
				MOFLOW_ASSERT(SpriteSheet, "Sprite sheet must be set first");
				mpFillImage->SetSpriteSheetRightIndex(mpFillImage->GetSpriteSheet()->GetFrameIndexByID(strValue));
			}
            //---Height from image
            if(insParams.TryGetValue("HeightFromImage", strValue))
            {
                HeightFromImage = Core::CStringConverter::ParseBool(strValue);
            }
		
			//---Colours
			if(insParams.TryGetValue("FillColour", strValue))
			{
				SetFillColour(Core::CStringConverter::ParseColourValue(strValue));
			}
			if(insParams.TryGetValue("BackgroundColour", strValue))
			{
				SetBackgroundColour(Core::CStringConverter::ParseColourValue(strValue));
			}
    
		}
		//------------------------------------------------------------------------
		/// Set Sprite Sheet
		///
		/// @param Sprite Sheet
		//------------------------------------------------------------------------
		void CHorizontalEndcappedProgressBar::SetSpriteSheet(const SpriteSheetPtr& inpSpriteSheet)
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
		const SpriteSheetPtr& CHorizontalEndcappedProgressBar::GetSpriteSheet() const
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
		void CHorizontalEndcappedProgressBar::SetBaseFillSpriteSheetIndexID(const std::string& instrID)
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
		const std::string& CHorizontalEndcappedProgressBar::GetBaseFillSpriteSheetIndexID() const
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
		void CHorizontalEndcappedProgressBar::SetBaseBackgroundSpriteSheetIndexID(const std::string& instrID)
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
		const std::string& CHorizontalEndcappedProgressBar::GetBaseBackgroundSpriteSheetIndexID() const
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
		void CHorizontalEndcappedProgressBar::SetFillIndices(u32 inudwLeft, u32 inudwMid, u32 inudwRight)
		{
			mpFillImage->SetSpriteSheetIndices(inudwLeft, inudwMid, inudwRight);	
		}
		//------------------------------------------------------------------------
		/// Set Background Indices
		///
		/// @param Sprite sheet index of left patch
		/// @param Sprite sheet index of centre patch
		/// @param Sprite sheet index of right patch
		//------------------------------------------------------------------------
		void CHorizontalEndcappedProgressBar::SetBackgroundIndices(u32 inudwLeft, u32 inudwMid, u32 inudwRight)
		{
			mpBGImage->SetSpriteSheetIndices(inudwLeft, inudwMid, inudwRight);
		}
		//------------------------------------------------------------------------
		/// Set Fill Colour
		///
		/// @param Colour of progress bar
		//------------------------------------------------------------------------
		void CHorizontalEndcappedProgressBar::SetFillColour(const Core::CColour& inColour)
		{
			FillColour = inColour;
			mpFillImage->SetColour(inColour);
		}
		//------------------------------------------------------------------------
		/// Get Fill Colour
		///
		/// @return Colour of progress bar
		//------------------------------------------------------------------------
		const Core::CColour& CHorizontalEndcappedProgressBar::GetFillColour() const
		{
			return FillColour;
		}
		//------------------------------------------------------------------------
		/// Set Background Colour
		///
		/// @param Colour of background
		//------------------------------------------------------------------------
		void CHorizontalEndcappedProgressBar::SetBackgroundColour(const Core::CColour& inColour)
		{
			BackgroundColour = inColour;
			mpBGImage->SetColour(inColour);
		}
		//------------------------------------------------------------------------
		/// Get Background Colour
		///
		/// @return Colour of background
		//------------------------------------------------------------------------
		const Core::CColour& CHorizontalEndcappedProgressBar::GetBackgroundColour() const
		{
			return BackgroundColour;
		}
		//--------------------------------------------------------
		/// Enable Height From Image
		///
		/// When this is enabled the image view's height will be 
		/// based on the height of the cap
		///
		/// @param Enable/disable
		//--------------------------------------------------------
		void CHorizontalEndcappedProgressBar::EnableHeightFromImage(bool inbEnable)
		{
			
		}
		//--------------------------------------------------------
		/// Is Height From Image Enabled
		///
		/// When this is enabled the image view's size will be 
		/// based on the size of the image
		///
		/// @return Whether the image view's size will be 
		/// based on the size of the image
		//--------------------------------------------------------
		bool CHorizontalEndcappedProgressBar::IsHeightFromImageEnabled() const
		{
			return HeightFromImage;
		}
		//------------------------------------------------------------------------
		/// Draw
		///
		/// Draw this view and any subviews
		///
		/// @param Renderer
		//------------------------------------------------------------------------
		void CHorizontalEndcappedProgressBar::Draw(CCanvasRenderer* inpCanvas)
		{
			if(Visible && SpriteSheet)
			{

				Core::CVector2 vMyAbsSize = GetAbsoluteSize();
				
				f32 fFullProgressWidth = vMyAbsSize.x - mpBGImage->GetCombinedCapWidth() + mpFillImage->GetCombinedCapWidth();
				f32 fFillWidth = GetProgress() * fFullProgressWidth;
				
                f32 fShrink = std::min(fFillWidth - mpFillImage->GetCombinedCapWidth(), 0.0f) * 0.5f;
                
				f32 fFillOffsetX = (mpBGImage->GetCombinedCapWidth() - mpFillImage->GetCombinedCapWidth() - fShrink) * 0.5f;
				
				mpFillImage->SetOffsetFromParentAlignment(0,0,fFillOffsetX,0);
				
				f32 fFillImageHeight = mpFillImage->GetCapHeight();
				f32 fBGImageHeight = mpBGImage->GetCapHeight();
				
				if (HeightFromImage)
				{
					vMyAbsSize.y = fBGImageHeight;
				}
				
				f32 fFillDrawHeight = vMyAbsSize.y - (fBGImageHeight-fFillImageHeight);		
				
				mpBGImage->SetSize(UnifiedVector2(Core::CVector2::ZERO,vMyAbsSize));
				mpBGImage->Draw(inpCanvas);
				
				mpFillImage->SetSize(UnifiedVector2(Core::CVector2::ZERO,Core::CVector2(fFillWidth,fFillDrawHeight)));
				mpFillImage->Draw(inpCanvas);
			}
            
            CProgressBar::Draw(inpCanvas);
		}
	}
}
