//
//  HorizontalClippingProgressBar.cpp
//  moFloTest
//
//  Created by Scott Downie on 27/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/GUI/ProgressBar/HorizontalClippingProgressBar.h>

#include <ChilliSource/GUI/Image/ImageView.h>
#include <ChilliSource/Rendering/Texture/TextureAtlas.h>
#include <ChilliSource/Rendering/Texture/TextureManager.h>
#include <ChilliSource/Rendering/Texture/Texture.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/Core/Resource/ResourceManagerDispenser.h>
#include <ChilliSource/Core/Resource/ResourcePool.h>

namespace ChilliSource
{
    namespace GUI
    {
		DEFINE_META_CLASS(HorizontalClippingProgressBar)

		DEFINE_PROPERTY(BackgroundTexture);
		DEFINE_PROPERTY(ProgressTexture);
		DEFINE_PROPERTY(BackgroundTextureAtlas);
		DEFINE_PROPERTY(ProgressTextureAtlas);
		DEFINE_PROPERTY(BackgroundTextureAtlasIndex);
		DEFINE_PROPERTY(ProgressTextureAtlasIndex);
		DEFINE_PROPERTY(BackgroundTextureAtlasID);
		DEFINE_PROPERTY(ProgressTextureAtlasID);
		DEFINE_PROPERTY(HeightFromImage);

        //------------------------------------------------------
        /// Constructor
        ///
        /// Create the subviews that make up the container
        //------------------------------------------------------
        HorizontalClippingProgressBar::HorizontalClippingProgressBar() 
			: mpBackgroundImage(new ImageView()), mpProgressImage(new ImageView()), mdwSegments(0), HeightFromImage(false)
        {
			SetSize(0.8f, 0.1f, 0.0f, 0.0f);

            mpBackgroundImage->SetSize(Core::UnifiedVector2(Core::Vector2(1.0f, 1.0f), Core::Vector2(0, 0)));
            mpBackgroundImage->SetPosition(Core::UnifiedVector2(Core::Vector2(0.5f, 0.5f), Core::Vector2(0, 0)));
            AddSubview(mpBackgroundImage);
            
            mpProgressImage->SetSize(Core::UnifiedVector2(Core::Vector2(0.0f, 1.0f), Core::Vector2(0, 0)));
            mpProgressImage->SetLocalAlignment(Rendering::AlignmentAnchor::k_middleLeft);
            mpProgressImage->EnableAlignmentToParent(true);
            mpProgressImage->SetAlignmentToParent(Rendering::AlignmentAnchor::k_middleLeft);
            mpBackgroundImage->AddSubview(mpProgressImage);
        }
        //------------------------------------------------------
        /// Constructor
        ///
        /// From param dictionary
        //------------------------------------------------------
        HorizontalClippingProgressBar::HorizontalClippingProgressBar(const Core::ParamDictionary& insParams) 
			: ProgressBar(insParams), mpBackgroundImage(new ImageView()), mpProgressImage(new ImageView()), mdwSegments(0), HeightFromImage(false)
        {
            mpBackgroundImage->SetSize(Core::UnifiedVector2(Core::Vector2(1.0f, 1.0f), Core::Vector2(0, 0)));
            mpBackgroundImage->SetPosition(Core::UnifiedVector2(Core::Vector2(0.5f, 0.5f), Core::Vector2(0, 0)));
            AddSubview(mpBackgroundImage);
 
            mpProgressImage->SetSize(Core::UnifiedVector2(Core::Vector2(0.0f, 1.0f), Core::Vector2(0, 0)));
            mpProgressImage->SetLocalAlignment(Rendering::AlignmentAnchor::k_middleLeft);
            mpProgressImage->EnableAlignmentToParent(true);
            mpProgressImage->SetAlignmentToParent(Rendering::AlignmentAnchor::k_middleLeft);
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
                SetBackgroundImage(LOAD_RESOURCE(Rendering::Texture, eBackgroundTextureLocation, strValue));
            }
            //---Progress Texture
            Core::StorageLocation eProgressTextureLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("ProgressTextureLocation", strValue))
            {
                eProgressTextureLocation = ChilliSource::Core::ParseStorageLocation(strValue);
            }
            if(insParams.TryGetValue("ProgressTexture", strValue))
            {
                SetProgressImage(LOAD_RESOURCE(Rendering::Texture, eProgressTextureLocation, strValue));
            }
            //---Background sprite sheet
            Core::StorageLocation eBackgroundTextureAtlasLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("BackgroundTextureAtlasLocation", strValue))
            {
                eBackgroundTextureAtlasLocation = ChilliSource::Core::ParseStorageLocation(strValue);
            }
            if(insParams.TryGetValue("BackgroundTextureAtlas", strValue))
            {
                Core::ResourcePool* resourcePool = Core::Application::Get()->GetResourcePool();
				SetBackgroundTextureAtlas(resourcePool->LoadResource<Rendering::TextureAtlas>(eBackgroundTextureAtlasLocation, strValue));
            }
			//---Progress sprite sheet
            Core::StorageLocation eProgressTextureAtlasLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("ProgressTextureAtlasLocation", strValue))
            {
                eProgressTextureAtlasLocation = ChilliSource::Core::ParseStorageLocation(strValue);
            }
			if(insParams.TryGetValue("ProgressTextureAtlas", strValue))
			{
                Core::ResourcePool* resourcePool = Core::Application::Get()->GetResourcePool();
				SetProgressTextureAtlas(resourcePool->LoadResource<Rendering::TextureAtlas>(eProgressTextureAtlasLocation, strValue));
			}
            //---Background index
            if(insParams.TryGetValue("BackgroundTextureAtlasIndex", strValue))
            {
				CS_ASSERT(BackgroundTextureAtlas, "Cannot set sprite sheet index without sprite sheet");
				SetBackgroundTextureAtlasIndex(Core::ParseU32(strValue));
            }
            //---Progress index
            if(insParams.TryGetValue("ProgressTextureAtlasIndex", strValue))
            {
				CS_ASSERT(ProgressTextureAtlas, "Cannot set sprite sheet index without sprite sheet");
				SetProgressTextureAtlasIndex(Core::ParseU32(strValue));
            }
			//---Background index ID
			if(insParams.TryGetValue("BackgroundTextureAtlasID", strValue))
			{
				SetBackgroundTextureAtlasID(strValue);
			}
			//---Progress index ID
			if(insParams.TryGetValue("ProgressTextureAtlasID", strValue))
			{
				SetProgressTextureAtlasID(strValue);
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
		void HorizontalClippingProgressBar::SetBackgroundTextureAtlas(const Rendering::TextureAtlasCSPtr& inpTextureAtlas)
		{
			BackgroundTextureAtlas = inpTextureAtlas;
			mpBackgroundImage->SetTextureAtlas(inpTextureAtlas);
            
            if(!ProgressTextureAtlas)
            {
                SetProgressTextureAtlas(inpTextureAtlas);
            }
		}
		//--------------------------------------------------------
		/// Set Progress Sprite Sheet
		///
		/// @param Sprite sheet
		//--------------------------------------------------------
		void HorizontalClippingProgressBar::SetProgressTextureAtlas(const Rendering::TextureAtlasCSPtr& inpTextureAtlas)
		{
			ProgressTextureAtlas = inpTextureAtlas;
			mpProgressImage->SetTextureAtlas(inpTextureAtlas);
            
            if(!BackgroundTextureAtlas)
            {
                SetBackgroundTextureAtlas(inpTextureAtlas);
            }
		}
		//--------------------------------------------------------
		/// Get Background Sprite Sheet
		///
		/// @return Sprite sheet
		//--------------------------------------------------------
		const Rendering::TextureAtlasCSPtr& HorizontalClippingProgressBar::GetBackgroundTextureAtlas() const
		{
			return BackgroundTextureAtlas;
		}
		//--------------------------------------------------------
		/// Get Progress Sprite Sheet
		///
		/// @return Sprite sheet
		//--------------------------------------------------------
		const Rendering::TextureAtlasCSPtr& HorizontalClippingProgressBar::GetProgressTextureAtlas() const
		{
			return ProgressTextureAtlas;
		}
        //--------------------------------------------------------
        /// Set Background Sprite Sheet Index
        ///
        /// @param The index of the image within the sprite sheet
        //--------------------------------------------------------
        void HorizontalClippingProgressBar::SetBackgroundTextureAtlasIndex(u32 inudwIndex)
        {
			BackgroundTextureAtlasIndex = inudwIndex;
            mpBackgroundImage->SetTextureAtlasIndex(inudwIndex);
        }
        //--------------------------------------------------------
        /// Set Progress Sprite Sheet Index
        ///
        /// @param The index of the image within the sprite sheet
        //--------------------------------------------------------
        void HorizontalClippingProgressBar::SetProgressTextureAtlasIndex(u32 inudwIndex)
        {
			ProgressTextureAtlasIndex = inudwIndex;
            mpProgressImage->SetTextureAtlasIndex(inudwIndex);
        }
		//--------------------------------------------------------
		/// Set Background Sprite Sheet Index ID
		///
		/// @param The index ID of the image within the sprite sheet
		//--------------------------------------------------------
		void HorizontalClippingProgressBar::SetBackgroundTextureAtlasID(const std::string& instrID)
		{
			CS_ASSERT(BackgroundTextureAtlas, "Cannot set sprite sheet index without sprite sheet");
			BackgroundTextureAtlasID = instrID;
			SetBackgroundTextureAtlasIndex(BackgroundTextureAtlas->GetFrameIndexById(instrID));
		}
		//--------------------------------------------------------
		/// Set Progress Sprite Sheet Index ID
		///
		/// @param The index ID of the image within the sprite sheet
		//--------------------------------------------------------
		void HorizontalClippingProgressBar::SetProgressTextureAtlasID(const std::string& instrID)
		{
			CS_ASSERT(ProgressTextureAtlas, "Cannot set sprite sheet index without sprite sheet");
			ProgressTextureAtlasID = instrID;
			SetProgressTextureAtlasIndex(ProgressTextureAtlas->GetFrameIndexById(instrID));
		}
		//--------------------------------------------------------
		/// Get Background Sprite Sheet Index ID
		///
		/// @return The index of the image within the sprite sheet
		//--------------------------------------------------------
		const std::string& HorizontalClippingProgressBar::GetBackgroundTextureAtlasID() const
		{
			return BackgroundTextureAtlasID;
		}
		//--------------------------------------------------------
		/// Get Progress Sprite Sheet Index ID
		///
		/// @return The index ID of the image within the sprite sheet
		//--------------------------------------------------------
		const std::string& HorizontalClippingProgressBar::GetProgressTextureAtlasID() const
		{
			return ProgressTextureAtlasID;
		}
		//--------------------------------------------------------
		/// Get Background Sprite Sheet Index
		///
		/// @return The index of the image within the sprite sheet
		//--------------------------------------------------------
		u32 HorizontalClippingProgressBar::GetBackgroundTextureAtlasIndex() const
		{
			return BackgroundTextureAtlasIndex;
		}
		//--------------------------------------------------------
		/// Get Progress Sprite Sheet Index
		///
		/// @return The index of the image within the sprite sheet
		//--------------------------------------------------------
		u32 HorizontalClippingProgressBar::GetProgressTextureAtlasIndex() const
		{
			return ProgressTextureAtlasIndex;
		}
        //------------------------------------------------------
        /// Set Background Image
        ///
        /// @param Texture representing the background
        //------------------------------------------------------
        void HorizontalClippingProgressBar::SetBackgroundImage(const Rendering::TextureSPtr& inpTexture)
        {
			BackgroundTexture = inpTexture;
            mpBackgroundImage->SetTexture(inpTexture);
        }
        //------------------------------------------------------
        /// Set Progress Image
        ///
        /// @param Texture representing the bar
        //------------------------------------------------------
        void HorizontalClippingProgressBar::SetProgressImage(const Rendering::TextureSPtr& inpTexture)
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
        void HorizontalClippingProgressBar::EnableHeightFromImage(bool inbEnable)
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
		bool HorizontalClippingProgressBar::IsHeightFromImageEnabled() const
		{
			return HeightFromImage;
		}
		//------------------------------------------------------
		/// Get Background Image
		///
		/// @return Texture representing the background
		//------------------------------------------------------
		const Rendering::TextureSPtr& HorizontalClippingProgressBar::GetBackgroundImage() const
		{
			return BackgroundTexture;
		}
		//------------------------------------------------------
		/// Get Progress Image
		///
		/// @return Texture representing the bar
		//------------------------------------------------------
		const Rendering::TextureSPtr& HorizontalClippingProgressBar::GetProgressImage() const
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
        void HorizontalClippingProgressBar::Draw(Rendering::CanvasRenderer* inpCanvas)
        {
            if(Visible)
            {
                //Update the progress bar
                f32 mfVisibleProgress = GetProgress();
				f32 fHeightProgressAbs = 0.0f;
				f32 fHeightProgressRel = 1.0f;
				
				if (HeightFromImage)
				{
					Core::Vector2 vAbsSize = Core::Vector2(mpBackgroundImage->GetAbsoluteSize().x, mpBackgroundImage->GetSizeFromImage().y);
					mpBackgroundImage->SetSize(Core::UnifiedVector2(Core::Vector2::ZERO, vAbsSize));
					
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
                
                GUIView::Draw(inpCanvas);
            }
        }
        //--------------------------------------------------------
        /// Get Background View
        ///
        /// @return Background image view
        //--------------------------------------------------------
        const ImageViewSPtr& HorizontalClippingProgressBar::GetBackgroundView() const
        {
            return mpBackgroundImage;
        }
    }
}
