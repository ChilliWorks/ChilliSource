/*
 *  HorizontalEndcappedProgressBar.cpp
 *  iOSTemplate
 *
 *  Created by Stuart McGaw on 04/05/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/GUI/ProgressBar/HorizontalEndcappedProgressBar.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Resource/ResourcePool.h>
#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/Rendering/Texture/TextureAtlas.h>
#include <ChilliSource/Rendering/Texture/Texture.h>

namespace ChilliSource
{
	namespace GUI
	{
		DEFINE_META_CLASS(HorizontalEndcappedProgressBar)
		
		DEFINE_PROPERTY(TextureAtlas);
		DEFINE_PROPERTY(BaseFillTextureAtlasID);
		DEFINE_PROPERTY(BaseBackgroundTextureAtlasID);
		DEFINE_PROPERTY(FillColour);
   
		DEFINE_PROPERTY(BackgroundColour);
		DEFINE_PROPERTY(HeightFromImage);

		HorizontalEndcappedProgressBar::HorizontalEndcappedProgressBar() : HeightFromImage(false)
		{

			mpBGImage = HorizontalStretchableImagePtr(new HorizontalStretchableImage());
			mpBGImage->SetLocalAlignment(Rendering::AlignmentAnchor::k_middleLeft);
			mpBGImage->EnableAlignmentToParent(true);
			mpBGImage->SetAlignmentToParent(Rendering::AlignmentAnchor::k_middleLeft);
            mpBGImage->EnableTouchConsumption(false);
			AddSubview(mpBGImage);
			
			mpFillImage = HorizontalStretchableImagePtr(new HorizontalStretchableImage());
			mpFillImage->SetLocalAlignment(Rendering::AlignmentAnchor::k_middleLeft);
			mpFillImage->EnableAlignmentToParent(true);
			mpFillImage->SetAlignmentToParent(Rendering::AlignmentAnchor::k_middleLeft);
            mpFillImage->EnableTouchConsumption(false);
			AddSubview(mpFillImage);
		}
		//------------------------------------------------------
		/// Constructor
		///
		/// From param dictionary
		//------------------------------------------------------
		HorizontalEndcappedProgressBar::HorizontalEndcappedProgressBar(const Core::ParamDictionary& insParams) 
		: ProgressBar(insParams), HeightFromImage(false)
		{
			mpBGImage = HorizontalStretchableImagePtr(new HorizontalStretchableImage());
			mpBGImage->SetLocalAlignment(Rendering::AlignmentAnchor::k_middleLeft);
			mpBGImage->EnableAlignmentToParent(true);
			mpBGImage->SetAlignmentToParent(Rendering::AlignmentAnchor::k_middleLeft);
            mpBGImage->EnableTouchConsumption(false);
			AddSubview(mpBGImage);
			
			mpFillImage = HorizontalStretchableImagePtr(new HorizontalStretchableImage());
			mpFillImage->SetLocalAlignment(Rendering::AlignmentAnchor::k_middleLeft);
			mpFillImage->EnableAlignmentToParent(true);
			mpFillImage->SetAlignmentToParent(Rendering::AlignmentAnchor::k_middleLeft);
            mpFillImage->EnableTouchConsumption(false);
			AddSubview(mpFillImage);
			
			std::string strValue;
            
            //---Texture
            Core::StorageLocation eTextureLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("TextureLocation", strValue))
            {
                eTextureLocation = ChilliSource::Core::ParseStorageLocation(strValue);
            }
			if(insParams.TryGetValue("Texture", strValue))
			{
                Core::ResourcePool* resourcePool = Core::Application::Get()->GetResourcePool();
				SetTexture(resourcePool->LoadResource<Rendering::Texture>(eTextureLocation, strValue));
			}
			
			//---Sprite sheet
            Core::StorageLocation eTextureAtlasLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("TextureAtlasLocation", strValue))
            {
                eTextureAtlasLocation = ChilliSource::Core::ParseStorageLocation(strValue);
            }
			if(insParams.TryGetValue("TextureAtlas", strValue))
			{
                Core::ResourcePool* resourcePool = Core::Application::Get()->GetResourcePool();
				SetTextureAtlas(resourcePool->LoadResource<Rendering::TextureAtlas>(eTextureAtlasLocation, strValue));
			}
			//---Sprite sheet ID
			if(insParams.TryGetValue("BaseFillTextureAtlasID", strValue))
			{
				SetBaseFillTextureAtlasID(strValue);
			}
			if(insParams.TryGetValue("BaseBackgroundTextureAtlasID", strValue))
			{
				SetBaseBackgroundTextureAtlasID(strValue);
			}
            
            if( insParams.TryGetValue("BaseBackgroundVisible", strValue) )
            {
                mpBGImage->SetVisible( Core::ParseBool(strValue)  );
            }
            //---Height from image
            if(insParams.TryGetValue("HeightFromImage", strValue))
            {
                HeightFromImage = Core::ParseBool(strValue);
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
        /// Set Texture
        ///
        /// @param Texture
        //------------------------------------------------------------------------
        void HorizontalEndcappedProgressBar::SetTexture(const Rendering::TextureCSPtr& inpTexture)
        {
            Texture = inpTexture;
            mpBGImage->SetTexture(inpTexture);
            mpFillImage->SetTexture(inpTexture);
        }
        //------------------------------------------------------------------------
        /// Get Texture
        ///
        /// @return Texture
        //------------------------------------------------------------------------
        const Rendering::TextureCSPtr& HorizontalEndcappedProgressBar::GetTexture() const
        {
            return Texture;
        }
		//------------------------------------------------------------------------
		/// Set Sprite Sheet
		///
		/// @param Sprite Sheet
		//------------------------------------------------------------------------
		void HorizontalEndcappedProgressBar::SetTextureAtlas(const Rendering::TextureAtlasCSPtr& inpTextureAtlas)
		{
			TextureAtlas = inpTextureAtlas;
			mpBGImage->SetTextureAtlas(inpTextureAtlas);
			mpFillImage->SetTextureAtlas(inpTextureAtlas);
		}
		//------------------------------------------------------------------------
		/// Get Sprite Sheet
		///
		/// @return Sprite Sheet
		//------------------------------------------------------------------------
		const Rendering::TextureAtlasCSPtr& HorizontalEndcappedProgressBar::GetTextureAtlas() const
		{
			return TextureAtlas;
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
		void HorizontalEndcappedProgressBar::SetBaseFillTextureAtlasID(const std::string& instrID)
		{
			BaseFillTextureAtlasID = instrID;
			mpFillImage->SetBaseTextureAtlasID(instrID);
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
		const std::string& HorizontalEndcappedProgressBar::GetBaseFillTextureAtlasID() const
		{
			return BaseFillTextureAtlasID;
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
		void HorizontalEndcappedProgressBar::SetBaseBackgroundTextureAtlasID(const std::string& instrID)
		{
			BaseBackgroundTextureAtlasID = instrID;
			mpBGImage->SetBaseTextureAtlasID(instrID);
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
		const std::string& HorizontalEndcappedProgressBar::GetBaseBackgroundTextureAtlasID() const
		{
			return BaseBackgroundTextureAtlasID;
		}
        //------------------------------------------------------------------------
		/// Set Fill Indices
		///
		/// @param Sprite sheet id of left patch
		/// @param Sprite sheet id of centre patch
		/// @param Sprite sheet id of right patch
		//------------------------------------------------------------------------
		void HorizontalEndcappedProgressBar::SetFillTextureAtlasIds(const std::string& in_left, const std::string& in_centre, const std::string& in_right)
		{
            mpFillImage->SetTextureAtlasIds(in_left, in_centre, in_right);
		}
		//------------------------------------------------------------------------
		/// Set Background Indices
		///
		/// @param Sprite sheet id of left patch
		/// @param Sprite sheet id of centre patch
		/// @param Sprite sheet id of right patch
		//------------------------------------------------------------------------
		void HorizontalEndcappedProgressBar::SetBackgroundTextureAtlasIds(const std::string& in_left, const std::string& in_centre, const std::string& in_right)
		{
            mpBGImage->SetTextureAtlasIds(in_left, in_centre, in_right);
		}
		//------------------------------------------------------------------------
		/// Set Fill Colour
		///
		/// @param Colour of progress bar
		//------------------------------------------------------------------------
		void HorizontalEndcappedProgressBar::SetFillColour(const Core::Colour& inColour)
		{
			FillColour = inColour;
			mpFillImage->SetColour(inColour);
		}
		//------------------------------------------------------------------------
		/// Get Fill Colour
		///
		/// @return Colour of progress bar
		//------------------------------------------------------------------------
		const Core::Colour& HorizontalEndcappedProgressBar::GetFillColour() const
		{
			return FillColour;
		}
		//------------------------------------------------------------------------
		/// Set Background Colour
		///
		/// @param Colour of background
		//------------------------------------------------------------------------
		void HorizontalEndcappedProgressBar::SetBackgroundColour(const Core::Colour& inColour)
		{
			BackgroundColour = inColour;
			mpBGImage->SetColour(inColour);
		}
		//------------------------------------------------------------------------
		/// Get Background Colour
		///
		/// @return Colour of background
		//------------------------------------------------------------------------
		const Core::Colour& HorizontalEndcappedProgressBar::GetBackgroundColour() const
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
		void HorizontalEndcappedProgressBar::EnableHeightFromImage(bool inbEnable)
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
		bool HorizontalEndcappedProgressBar::IsHeightFromImageEnabled() const
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
		void HorizontalEndcappedProgressBar::Draw(Rendering::CanvasRenderer* inpCanvas)
		{
			if(Visible && TextureAtlas && Texture)
			{

				Core::Vector2 vMyAbsSize = GetAbsoluteSize();
				
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
				
				mpBGImage->SetSize(Core::UnifiedVector2(Core::Vector2::k_zero,vMyAbsSize));
				mpBGImage->Draw(inpCanvas);
				
				mpFillImage->SetSize(Core::UnifiedVector2(Core::Vector2::k_zero,Core::Vector2(fFillWidth,fFillDrawHeight)));
				mpFillImage->Draw(inpCanvas);
			}
            
            ProgressBar::Draw(inpCanvas);
		}
	}
}
