//
//  VerticalEndcappedProgressBar.cpp
//  iOSTemplate
//
//  Created by Thomas Kronberg on 12/02/2013.
//  Copyright (c) 2013 Tag Games. All rights reserved.
//

#include <ChilliSource/GUI/ProgressBar/VerticalEndcappedProgressBar.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Resource/ResourcePool.h>
#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/Rendering/Texture/TextureAtlas.h>
#include <ChilliSource/Rendering/Texture/Texture.h>

namespace ChilliSource
{
	namespace GUI
	{
		DEFINE_META_CLASS(VerticalEndcappedProgressBar)
		
		DEFINE_PROPERTY(TextureAtlas);
		DEFINE_PROPERTY(BaseFillTextureAtlasID);
		DEFINE_PROPERTY(BaseBackgroundTextureAtlasID);
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
			
            //---Texture
            Core::StorageLocation eTextureLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("TextureLocation", strValue))
            {
                eTextureLocation = CSCore::ParseStorageLocation(strValue);
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
                eTextureAtlasLocation = CSCore::ParseStorageLocation(strValue);
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
        /// Set Texture
        ///
        /// @param Texture
        //------------------------------------------------------------------------
        void VerticalEndcappedProgressBar::SetTexture(const Rendering::TextureCSPtr& inpTexture)
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
        const Rendering::TextureCSPtr& VerticalEndcappedProgressBar::GetTexture() const
        {
            return Texture;
        }
		//------------------------------------------------------------------------
		/// Set Sprite Sheet
		///
		/// @param Sprite Sheet
		//------------------------------------------------------------------------
		void VerticalEndcappedProgressBar::SetTextureAtlas(const Rendering::TextureAtlasCSPtr& inpTextureAtlas)
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
		const Rendering::TextureAtlasCSPtr& VerticalEndcappedProgressBar::GetTextureAtlas() const
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
		void VerticalEndcappedProgressBar::SetBaseFillTextureAtlasID(const std::string& instrID)
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
		const std::string& VerticalEndcappedProgressBar::GetBaseFillTextureAtlasID() const
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
		void VerticalEndcappedProgressBar::SetBaseBackgroundTextureAtlasID(const std::string& instrID)
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
		const std::string& VerticalEndcappedProgressBar::GetBaseBackgroundTextureAtlasID() const
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
		void VerticalEndcappedProgressBar::SetFillTextureAtlasIds(const std::string& in_top, const std::string& in_middle, const std::string& in_bottom)
		{
            mpFillImage->SetTextureAtlasIds(in_top, in_middle, in_bottom);
		}
		//------------------------------------------------------------------------
		/// Set Background Indices
		///
		/// @param Sprite sheet id of left patch
		/// @param Sprite sheet id of centre patch
		/// @param Sprite sheet id of right patch
		//------------------------------------------------------------------------
		void VerticalEndcappedProgressBar::SetBackgroundTextureAtlasIds(const std::string& in_top, const std::string& in_middle, const std::string& in_bottom)
		{
            mpBGImage->SetTextureAtlasIds(in_top, in_middle, in_bottom);
		}
		//------------------------------------------------------------------------
		/// Set Fill Colour
		///
		/// @param Colour of progress bar
		//------------------------------------------------------------------------
		void VerticalEndcappedProgressBar::SetFillColour(const Core::Colour& inColour)
		{
			FillColour = inColour;
			mpFillImage->SetColour(inColour);
		}
		//------------------------------------------------------------------------
		/// Get Fill Colour
		///
		/// @return Colour of progress bar
		//------------------------------------------------------------------------
		const Core::Colour& VerticalEndcappedProgressBar::GetFillColour() const
		{
			return FillColour;
		}
		//------------------------------------------------------------------------
		/// Set Background Colour
		///
		/// @param Colour of background
		//------------------------------------------------------------------------
		void VerticalEndcappedProgressBar::SetBackgroundColour(const Core::Colour& inColour)
		{
			BackgroundColour = inColour;
			mpBGImage->SetColour(inColour);
		}
		//------------------------------------------------------------------------
		/// Get Background Colour
		///
		/// @return Colour of background
		//------------------------------------------------------------------------
		const Core::Colour& VerticalEndcappedProgressBar::GetBackgroundColour() const
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
			if(Visible && TextureAtlas && Texture)
			{
                
				Core::Vector2 vMyAbsSize = GetAbsoluteSize();
				
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
				
				mpBGImage->SetSize(Core::UnifiedVector2(Core::Vector2::k_zero,vMyAbsSize));
				mpBGImage->Draw(inpCanvas);
				
				mpFillImage->SetSize(Core::UnifiedVector2(Core::Vector2::k_zero,Core::Vector2(fFillDrawWidth,fFillHeight)));
				mpFillImage->Draw(inpCanvas);
			}
            
            ProgressBar::Draw(inpCanvas);
		}
	}
}
