//
//  HorizontalClippingProgressBar.cpp
//  moFloTest
//
//  Created by Scott Downie on 27/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/GUI/ProgressBar/HorizontalClippingProgressBar.h>

#include <ChilliSource/GUI/Image/ImageView.h>
#include <ChilliSource/Rendering/Sprite/SpriteSheet.h>
#include <ChilliSource/Rendering/Sprite/SpriteSheetManager.h>
#include <ChilliSource/Rendering/Texture/TextureManager.h>
#include <ChilliSource/Rendering/Texture/Texture.h>
#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/Core/Resource/ResourceManagerDispenser.h>

namespace ChilliSource
{
    namespace GUI
    {
		DEFINE_META_CLASS(CHorizontalClippingProgressBar)

		DEFINE_PROPERTY(BackgroundTexture);
		DEFINE_PROPERTY(ProgressTexture);
		DEFINE_PROPERTY(BackgroundSpriteSheet);
		DEFINE_PROPERTY(ProgressSpriteSheet);
		DEFINE_PROPERTY(BackgroundSpriteSheetIndex);
		DEFINE_PROPERTY(ProgressSpriteSheetIndex);
		DEFINE_PROPERTY(BackgroundSpriteSheetIndexID);
		DEFINE_PROPERTY(ProgressSpriteSheetIndexID);
		DEFINE_PROPERTY(HeightFromImage);

        //------------------------------------------------------
        /// Constructor
        ///
        /// Create the subviews that make up the container
        //------------------------------------------------------
        CHorizontalClippingProgressBar::CHorizontalClippingProgressBar() 
			: mpBackgroundImage(new CImageView()), mpProgressImage(new CImageView()), mdwSegments(0), HeightFromImage(false)
        {
			SetSize(0.8f, 0.1f, 0.0f, 0.0f);

            mpBackgroundImage->SetSize(Core::UnifiedVector2(Core::CVector2(1.0f, 1.0f), Core::CVector2(0, 0)));
            mpBackgroundImage->SetPosition(Core::UnifiedVector2(Core::CVector2(0.5f, 0.5f), Core::CVector2(0, 0)));
            AddSubview(mpBackgroundImage);
            
            mpProgressImage->SetSize(Core::UnifiedVector2(Core::CVector2(0.0f, 1.0f), Core::CVector2(0, 0)));
            mpProgressImage->SetLocalAlignment(Core::AlignmentAnchor::k_middleLeft);
            mpProgressImage->EnableAlignmentToParent(true);
            mpProgressImage->SetAlignmentToParent(Core::AlignmentAnchor::k_middleLeft);
            mpBackgroundImage->AddSubview(mpProgressImage);
        }
        //------------------------------------------------------
        /// Constructor
        ///
        /// From param dictionary
        //------------------------------------------------------
        CHorizontalClippingProgressBar::CHorizontalClippingProgressBar(const Core::ParamDictionary& insParams) 
			: CProgressBar(insParams), mpBackgroundImage(new CImageView()), mpProgressImage(new CImageView()), mdwSegments(0), HeightFromImage(false)
        {
            mpBackgroundImage->SetSize(Core::UnifiedVector2(Core::CVector2(1.0f, 1.0f), Core::CVector2(0, 0)));
            mpBackgroundImage->SetPosition(Core::UnifiedVector2(Core::CVector2(0.5f, 0.5f), Core::CVector2(0, 0)));
            AddSubview(mpBackgroundImage);
 
            mpProgressImage->SetSize(Core::UnifiedVector2(Core::CVector2(0.0f, 1.0f), Core::CVector2(0, 0)));
            mpProgressImage->SetLocalAlignment(Core::AlignmentAnchor::k_middleLeft);
            mpProgressImage->EnableAlignmentToParent(true);
            mpProgressImage->SetAlignmentToParent(Core::AlignmentAnchor::k_middleLeft);
            mpBackgroundImage->AddSubview(mpProgressImage);
            
            std::string strValue;

            //---Background Texture
            Core::StorageLocation eBackgroundTextureLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("BackgroundTextureLocation", strValue))
            {
                eBackgroundTextureLocation = ChilliSource::Core::ParseStorageLocation(strValue);
            }
            if(insParams.TryGetValue("BackgroundTexture", strValue))
            {
                SetBackgroundImage(LOAD_RESOURCE(Rendering::ITexture, eBackgroundTextureLocation, strValue));
            }
            //---Progress Texture
            Core::StorageLocation eProgressTextureLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("ProgressTextureLocation", strValue))
            {
                eProgressTextureLocation = ChilliSource::Core::ParseStorageLocation(strValue);
            }
            if(insParams.TryGetValue("ProgressTexture", strValue))
            {
                SetProgressImage(LOAD_RESOURCE(Rendering::ITexture, eProgressTextureLocation, strValue));
            }
            //---Background sprite sheet
            Core::StorageLocation eBackgroundSpriteSheetLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("BackgroundSpriteSheetLocation", strValue))
            {
                eBackgroundSpriteSheetLocation = ChilliSource::Core::ParseStorageLocation(strValue);
            }
            if(insParams.TryGetValue("BackgroundSpriteSheet", strValue))
            {
                SetBackgroundSpriteSheet(LOAD_RESOURCE(Rendering::CSpriteSheet, eBackgroundSpriteSheetLocation, strValue));
            }
			//---Progress sprite sheet
            Core::StorageLocation eProgressSpriteSheetLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("ProgressSpriteSheetLocation", strValue))
            {
                eProgressSpriteSheetLocation = ChilliSource::Core::ParseStorageLocation(strValue);
            }
			if(insParams.TryGetValue("ProgressSpriteSheet", strValue))
			{
				SetProgressSpriteSheet(LOAD_RESOURCE(Rendering::CSpriteSheet, eProgressSpriteSheetLocation, strValue));
			}
            //---Background index
            if(insParams.TryGetValue("BackgroundSpriteSheetIndex", strValue))
            {
				CS_ASSERT(BackgroundSpriteSheet, "Cannot set sprite sheet index without sprite sheet");
				SetBackgroundSpriteSheetIndex(Core::ParseU32(strValue));
            }
            //---Progress index
            if(insParams.TryGetValue("ProgressSpriteSheetIndex", strValue))
            {
				CS_ASSERT(ProgressSpriteSheet, "Cannot set sprite sheet index without sprite sheet");
				SetProgressSpriteSheetIndex(Core::ParseU32(strValue));
            }
			//---Background index ID
			if(insParams.TryGetValue("BackgroundSpriteSheetIndexID", strValue))
			{
				SetBackgroundSpriteSheetIndexID(strValue);
			}
			//---Progress index ID
			if(insParams.TryGetValue("ProgressSpriteSheetIndexID", strValue))
			{
				SetProgressSpriteSheetIndexID(strValue);
			}
			if(insParams.TryGetValue("Segments", strValue))
			{
				mdwSegments=Core::ParseS32(strValue);
			}
			//---Height From Image
			if(insParams.TryGetValue("HeightFromImage", strValue))
			{
				HeightFromImage = Core::ParseBool(strValue);
			}

			if(mdwSegments != 0)
			{
				mfSegmentSize = 1.0f/((f32)mdwSegments);
			}
        }
		//--------------------------------------------------------
		/// Set Background Sprite Sheet
		///
		/// @param Sprite sheet
		//--------------------------------------------------------
		void CHorizontalClippingProgressBar::SetBackgroundSpriteSheet(const Rendering::SpriteSheetPtr& inpSpriteSheet)
		{
			BackgroundSpriteSheet = inpSpriteSheet;
			mpBackgroundImage->SetSpriteSheet(inpSpriteSheet);
            
            if(!ProgressSpriteSheet)
            {
                SetProgressSpriteSheet(inpSpriteSheet);
            }
		}
		//--------------------------------------------------------
		/// Set Progress Sprite Sheet
		///
		/// @param Sprite sheet
		//--------------------------------------------------------
		void CHorizontalClippingProgressBar::SetProgressSpriteSheet(const Rendering::SpriteSheetPtr& inpSpriteSheet)
		{
			ProgressSpriteSheet = inpSpriteSheet;
			mpProgressImage->SetSpriteSheet(inpSpriteSheet);
            
            if(!BackgroundSpriteSheet)
            {
                SetBackgroundSpriteSheet(inpSpriteSheet);
            }
		}
		//--------------------------------------------------------
		/// Get Background Sprite Sheet
		///
		/// @return Sprite sheet
		//--------------------------------------------------------
		const Rendering::SpriteSheetPtr& CHorizontalClippingProgressBar::GetBackgroundSpriteSheet() const
		{
			return BackgroundSpriteSheet;
		}
		//--------------------------------------------------------
		/// Get Progress Sprite Sheet
		///
		/// @return Sprite sheet
		//--------------------------------------------------------
		const Rendering::SpriteSheetPtr& CHorizontalClippingProgressBar::GetProgressSpriteSheet() const
		{
			return ProgressSpriteSheet;
		}
        //--------------------------------------------------------
        /// Set Background Sprite Sheet Index
        ///
        /// @param The index of the image within the sprite sheet
        //--------------------------------------------------------
        void CHorizontalClippingProgressBar::SetBackgroundSpriteSheetIndex(u32 inudwIndex)
        {
			BackgroundSpriteSheetIndex = inudwIndex;
            mpBackgroundImage->SetSpriteSheetIndex(inudwIndex);
        }
        //--------------------------------------------------------
        /// Set Progress Sprite Sheet Index
        ///
        /// @param The index of the image within the sprite sheet
        //--------------------------------------------------------
        void CHorizontalClippingProgressBar::SetProgressSpriteSheetIndex(u32 inudwIndex)
        {
			ProgressSpriteSheetIndex = inudwIndex;
            mpProgressImage->SetSpriteSheetIndex(inudwIndex);
        }
		//--------------------------------------------------------
		/// Set Background Sprite Sheet Index ID
		///
		/// @param The index ID of the image within the sprite sheet
		//--------------------------------------------------------
		void CHorizontalClippingProgressBar::SetBackgroundSpriteSheetIndexID(const std::string& instrID)
		{
			CS_ASSERT(BackgroundSpriteSheet, "Cannot set sprite sheet index without sprite sheet");
			BackgroundSpriteSheetIndexID = instrID;
			SetBackgroundSpriteSheetIndex(BackgroundSpriteSheet->GetFrameIndexByID(instrID));
		}
		//--------------------------------------------------------
		/// Set Progress Sprite Sheet Index ID
		///
		/// @param The index ID of the image within the sprite sheet
		//--------------------------------------------------------
		void CHorizontalClippingProgressBar::SetProgressSpriteSheetIndexID(const std::string& instrID)
		{
			CS_ASSERT(ProgressSpriteSheet, "Cannot set sprite sheet index without sprite sheet");
			ProgressSpriteSheetIndexID = instrID;
			SetProgressSpriteSheetIndex(ProgressSpriteSheet->GetFrameIndexByID(instrID));
		}
		//--------------------------------------------------------
		/// Get Background Sprite Sheet Index ID
		///
		/// @return The index of the image within the sprite sheet
		//--------------------------------------------------------
		const std::string& CHorizontalClippingProgressBar::GetBackgroundSpriteSheetIndexID() const
		{
			return BackgroundSpriteSheetIndexID;
		}
		//--------------------------------------------------------
		/// Get Progress Sprite Sheet Index ID
		///
		/// @return The index ID of the image within the sprite sheet
		//--------------------------------------------------------
		const std::string& CHorizontalClippingProgressBar::GetProgressSpriteSheetIndexID() const
		{
			return ProgressSpriteSheetIndexID;
		}
		//--------------------------------------------------------
		/// Get Background Sprite Sheet Index
		///
		/// @return The index of the image within the sprite sheet
		//--------------------------------------------------------
		u32 CHorizontalClippingProgressBar::GetBackgroundSpriteSheetIndex() const
		{
			return BackgroundSpriteSheetIndex;
		}
		//--------------------------------------------------------
		/// Get Progress Sprite Sheet Index
		///
		/// @return The index of the image within the sprite sheet
		//--------------------------------------------------------
		u32 CHorizontalClippingProgressBar::GetProgressSpriteSheetIndex() const
		{
			return ProgressSpriteSheetIndex;
		}
        //------------------------------------------------------
        /// Set Background Image
        ///
        /// @param Texture representing the background
        //------------------------------------------------------
        void CHorizontalClippingProgressBar::SetBackgroundImage(const Rendering::TexturePtr& inpTexture)
        {
			BackgroundTexture = inpTexture;
            mpBackgroundImage->SetTexture(inpTexture);
        }
        //------------------------------------------------------
        /// Set Progress Image
        ///
        /// @param Texture representing the bar
        //------------------------------------------------------
        void CHorizontalClippingProgressBar::SetProgressImage(const Rendering::TexturePtr& inpTexture)
        {
			ProgressTexture = inpTexture;
            mpProgressImage->SetTexture(inpTexture);
        }
        //--------------------------------------------------------
        /// Enable Height From Image
        ///
        /// When this is enabled the image view's height will be 
        /// based on the size of the image
        ///
        /// @param Enable/disable
        //--------------------------------------------------------
        void CHorizontalClippingProgressBar::EnableHeightFromImage(bool inbEnable)
        {
            HeightFromImage = inbEnable;
        }
		//--------------------------------------------------------
		/// Is Height From Image Enabled
		///
		/// When this is enabled the image view's height will be 
		/// based on the size of the image
		///
		/// @return Whether the image view's height will be 
		/// based on the size of the image
		//--------------------------------------------------------
		bool CHorizontalClippingProgressBar::IsHeightFromImageEnabled() const
		{
			return HeightFromImage;
		}
		//------------------------------------------------------
		/// Get Background Image
		///
		/// @return Texture representing the background
		//------------------------------------------------------
		const Rendering::TexturePtr& CHorizontalClippingProgressBar::GetBackgroundImage() const
		{
			return BackgroundTexture;
		}
		//------------------------------------------------------
		/// Get Progress Image
		///
		/// @return Texture representing the bar
		//------------------------------------------------------
		const Rendering::TexturePtr& CHorizontalClippingProgressBar::GetProgressImage() const
		{
			return ProgressTexture;
		}
        //------------------------------------------------------
        /// Draw
        ///
        /// Draws a background box with the progress bar
        /// as subview
        ///
        /// @param Canvas renderer pointer
        //------------------------------------------------------
        void CHorizontalClippingProgressBar::Draw(Rendering::CCanvasRenderer* inpCanvas)
        {
            if(Visible)
            {
                //Update the progress bar
                f32 mfVisibleProgress = GetProgress();
				f32 fHeightProgressAbs = 0.0f;
				f32 fHeightProgressRel = 1.0f;
				
				if (HeightFromImage)
				{
					Core::CVector2 vAbsSize = Core::CVector2(mpBackgroundImage->GetAbsoluteSize().x, mpBackgroundImage->GetSizeFromImage().y);
					mpBackgroundImage->SetSize(Core::UnifiedVector2(Core::CVector2::ZERO, vAbsSize));
					
					fHeightProgressAbs = mpProgressImage->GetSizeFromImage().y;
					fHeightProgressRel = 0.0f;
				}
                
                if(mdwSegments != 0)
                {
                    s32 dwVisibleSegments = (s32)(GetProgress()/mfSegmentSize);
                    mfVisibleProgress = ((f32)dwVisibleSegments)*mfSegmentSize;
                }
                
                if(mfVisibleProgress > 0.0f)
                {
                    Core::Rectangle rectProgress;
                    rectProgress.vOrigin.x = 0.0f;
                    rectProgress.vOrigin.y = 0.0f;
                    rectProgress.vSize.x = mfVisibleProgress;
                    rectProgress.vSize.y = 1.0f;
                    mpProgressImage->SetUVOffsets(rectProgress);
                }
                mpProgressImage->SetSize(mfVisibleProgress, fHeightProgressRel, 0, fHeightProgressAbs);
                
                CGUIView::Draw(inpCanvas);
            }
        }
        //--------------------------------------------------------
        /// Get Background View
        ///
        /// @return Background image view
        //--------------------------------------------------------
        const ImageViewPtr& CHorizontalClippingProgressBar::GetBackgroundView() const
        {
            return mpBackgroundImage;
        }
    }
}
