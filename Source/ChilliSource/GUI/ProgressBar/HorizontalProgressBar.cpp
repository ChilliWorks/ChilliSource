//
//  HorizontalProgressBar.cpp
//  moFloTest
//
//  Created by Scott Downie on 27/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/GUI/ProgressBar/HorizontalProgressBar.h>
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
		DEFINE_META_CLASS(HorizontalProgressBar)

		DEFINE_PROPERTY(BackgroundTexture);
		DEFINE_PROPERTY(ProgressTexture);
		DEFINE_PROPERTY(BackgroundSpriteSheet);
		DEFINE_PROPERTY(ProgressSpriteSheet);
		DEFINE_PROPERTY(BackgroundSpriteSheetIndex);
		DEFINE_PROPERTY(ProgressSpriteSheetIndex);
		DEFINE_PROPERTY(BackgroundSpriteSheetIndexID);
		DEFINE_PROPERTY(ProgressSpriteSheetIndexID);

        //------------------------------------------------------
        /// Constructor
        ///
        /// Create the subviews that make up the container
        //------------------------------------------------------
        HorizontalProgressBar::HorizontalProgressBar() : mpBackgroundImage(new ImageView()), mpProgressImage(new ImageView()),
			BackgroundSpriteSheetIndex(0), ProgressSpriteSheetIndex(0)
        {
			SetSize(0.8f, 0.1f, 0.0f, 0.0f);

            mpBackgroundImage->SetSize(Core::UnifiedVector2(Core::Vector2(1.0f, 1.0f), Core::Vector2(0, 0)));
            mpBackgroundImage->SetPosition(Core::UnifiedVector2(Core::Vector2(0.5f, 0.5f), Core::Vector2(0, 0)));
            AddSubview(mpBackgroundImage);
            
            mfProgressAbsHeight = 0.0f;
            mfProgressRelHeight = 1.0f;
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
        HorizontalProgressBar::HorizontalProgressBar(const Core::ParamDictionary& insParams) 
		: ProgressBar(insParams), mpBackgroundImage(new ImageView()), mpProgressImage(new ImageView()), BackgroundSpriteSheetIndex(0), ProgressSpriteSheetIndex(0) 
        {
            mfProgressAbsHeight = 0.0f;
            mfProgressRelHeight = 1.0f;
            
            mpBackgroundImage->SetSize(Core::UnifiedVector2(Core::Vector2(1.0f, 1.0f), Core::Vector2(0, 0)));
            mpBackgroundImage->SetPosition(Core::UnifiedVector2(Core::Vector2(0.5f, 0.5f), Core::Vector2(0, 0)));
            AddSubview(mpBackgroundImage);
            
            mpProgressImage->SetSize(0.0f, 1.0f, 0, 0);
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
                mpBackgroundImage->SetTexture(LOAD_RESOURCE(Rendering::Texture, eBackgroundTextureLocation, strValue));
            }
            //---Progress Texture
            Core::StorageLocation eProgressTextureLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("ProgressTextureLocation", strValue))
            {
                eProgressTextureLocation = ChilliSource::Core::ParseStorageLocation(strValue);
            }
            if(insParams.TryGetValue("ProgressTexture", strValue))
            {
                mpProgressImage->SetTexture(LOAD_RESOURCE(Rendering::Texture, eProgressTextureLocation, strValue));
            }
            //---Background sprite sheet
            Core::StorageLocation eBackgroundSpriteSheetLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("BackgroundSpriteSheetLocation", strValue))
            {
                eBackgroundSpriteSheetLocation = ChilliSource::Core::ParseStorageLocation(strValue);
            }
            if(insParams.TryGetValue("BackgroundSpriteSheet", strValue))
            {
                SetBackgroundSpriteSheet(LOAD_RESOURCE(Rendering::SpriteSheet, eBackgroundSpriteSheetLocation, strValue));
            }
			//---Progress sprite sheet
            Core::StorageLocation eProgressSpriteSheetLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("ProgressSpriteSheetLocation", strValue))
            {
                eProgressSpriteSheetLocation = ChilliSource::Core::ParseStorageLocation(strValue);
            }
			if(insParams.TryGetValue("ProgressSpriteSheet", strValue))
			{
				SetProgressSpriteSheet(LOAD_RESOURCE(Rendering::SpriteSheet, eProgressSpriteSheetLocation, strValue));
			}
            //---Background index
            if(insParams.TryGetValue("BackgroundSpriteSheetIndex", strValue))
            {
				CS_ASSERT(BackgroundSpriteSheet, "Cannot set sprite sheet index without setting sprite sheet");
				SetBackgroundSpriteSheetIndex(Core::ParseU32(strValue));
            }
			//---Progress index
			if(insParams.TryGetValue("ProgressSpriteSheetIndex", strValue))
			{
				CS_ASSERT(ProgressSpriteSheet, "Cannot set sprite sheet index without setting sprite sheet");
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
				CS_ASSERT(ProgressSpriteSheet, "Cannot set sprite sheet index without setting sprite sheet");
				SetProgressSpriteSheetIndexID(strValue);
			}
		}
		//--------------------------------------------------------
		/// Set Background Sprite Sheet
		///
		/// @param Sprite sheet
		//--------------------------------------------------------
		void HorizontalProgressBar::SetBackgroundSpriteSheet(const Rendering::SpriteSheetSPtr& inpSpriteSheet)
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
		void HorizontalProgressBar::SetProgressSpriteSheet(const Rendering::SpriteSheetSPtr& inpSpriteSheet)
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
		const Rendering::SpriteSheetSPtr& HorizontalProgressBar::GetBackgroundSpriteSheet() const
		{
			return BackgroundSpriteSheet;
		}
		//--------------------------------------------------------
		/// Get Progress Sprite Sheet
		///
		/// @return Sprite sheet
		//--------------------------------------------------------
		const Rendering::SpriteSheetSPtr& HorizontalProgressBar::GetProgressSpriteSheet() const
		{
			return ProgressSpriteSheet;
		}
        //--------------------------------------------------------
        /// Set Background Sprite Sheet Index
        ///
        /// @param The index of the image within the sprite sheet
        //--------------------------------------------------------
        void HorizontalProgressBar::SetBackgroundSpriteSheetIndex(u32 inudwIndex)
        {
			BackgroundSpriteSheetIndex = inudwIndex;
            mpBackgroundImage->SetSpriteSheetIndex(inudwIndex);
        }
        //--------------------------------------------------------
        /// Set Progress Sprite Sheet Index
        ///
        /// @param The index of the image within the sprite sheet
        //--------------------------------------------------------
        void HorizontalProgressBar::SetProgressSpriteSheetIndex(u32 inudwIndex)
        {
			ProgressSpriteSheetIndex = inudwIndex;
            mpProgressImage->SetSpriteSheetIndex(inudwIndex);
        }
		//--------------------------------------------------------
		/// Get Progress Sprite Sheet Index
		///
		/// @return The index of the image within the sprite sheet
		//--------------------------------------------------------
		u32 HorizontalProgressBar::GetBackgroundSpriteSheetIndex() const
		{
			return BackgroundSpriteSheetIndex;
		}
		//--------------------------------------------------------
		/// Get Progress Sprite Sheet Index
		///
		/// @return The index of the image within the sprite sheet
		//--------------------------------------------------------
		u32 HorizontalProgressBar::GetProgressSpriteSheetIndex() const
		{
			return ProgressSpriteSheetIndex;
		}
		//--------------------------------------------------------
		/// Set Background Sprite Sheet Index ID
		///
		/// @param The index ID of the image within the sprite sheet
		//--------------------------------------------------------
		void HorizontalProgressBar::SetBackgroundSpriteSheetIndexID(const std::string& instrID)
		{
			CS_ASSERT(BackgroundSpriteSheet, "Cannot set sprite sheet index without setting sprite sheet");
			BackgroundSpriteSheetIndexID = instrID;
			SetBackgroundSpriteSheetIndex(BackgroundSpriteSheet->GetFrameIndexByID(instrID));
		}
		//--------------------------------------------------------
		/// Set Progress Sprite Sheet Index ID
		///
		/// @param The index ID of the image within the sprite sheet
		//--------------------------------------------------------
		void HorizontalProgressBar::SetProgressSpriteSheetIndexID(const std::string& instrID)
		{
			CS_ASSERT(ProgressSpriteSheet, "Cannot set sprite sheet index without setting sprite sheet");
			ProgressSpriteSheetIndexID = instrID;
			SetProgressSpriteSheetIndex(ProgressSpriteSheet->GetFrameIndexByID(instrID));
		}
		//--------------------------------------------------------
		/// Get Progress Sprite Sheet Index ID
		///
		/// @return The index ID of the image within the sprite sheet
		//--------------------------------------------------------
		const std::string& HorizontalProgressBar::GetBackgroundSpriteSheetIndexID() const
		{
			return BackgroundSpriteSheetIndexID;
		}
		//--------------------------------------------------------
		/// Get Progress Sprite Sheet Index ID
		///
		/// @return The index ID of the image within the sprite sheet
		//--------------------------------------------------------
		const std::string& HorizontalProgressBar::GetProgressSpriteSheetIndexID() const
		{
			return ProgressSpriteSheetIndexID;
		}
        //------------------------------------------------------
        /// Set Background Image
        ///
        /// @param Texture representing the background
        //------------------------------------------------------
        void HorizontalProgressBar::SetBackgroundImage(const Rendering::TextureSPtr& inpTexture)
        {
			BackgroundTexture = inpTexture;
            mpBackgroundImage->SetTexture(inpTexture);
            
            if(!ProgressTexture)
            {
                SetProgressImage(inpTexture);
            }
        }
		//------------------------------------------------------
		/// Set Progress Image
		///
		/// @param Texture representing the bar
		//------------------------------------------------------
		void HorizontalProgressBar::SetProgressImage(const Rendering::TextureSPtr& inpTexture)
		{
			ProgressTexture = inpTexture;
			mpProgressImage->SetTexture(inpTexture);
            
            if(!BackgroundTexture)
            {
                SetBackgroundImage(inpTexture);
            }
		}
		//------------------------------------------------------
		/// Get Background Image
		///
		/// @return Texture representing the background
		//------------------------------------------------------
		const Rendering::TextureSPtr& HorizontalProgressBar::GetBackgroundImage() const
		{
			return BackgroundTexture;
		}
		//------------------------------------------------------
		/// Get Progress Image
		///
		/// @return Texture representing the bar
		//------------------------------------------------------
		const Rendering::TextureSPtr& HorizontalProgressBar::GetProgressImage() const
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
        void HorizontalProgressBar::Draw(Rendering::CanvasRenderer* inpCanvas)
        {
            //Update the progress bar
            mpProgressImage->SetSize(GetProgress(), mfProgressRelHeight, 0, mfProgressAbsHeight);
            
            if(Visible)
            {
                GUIView::Draw(inpCanvas);
            }
        }
        //--------------------------------------------------------
        /// Set Progress Image Height
        ///
        /// @param Rel, Abs
        //--------------------------------------------------------
        void HorizontalProgressBar::SetProgressImageHeight(f32 infRel, f32 infAbs)
        {
            mfProgressAbsHeight = infAbs;
            mfProgressRelHeight = infRel;
        }
    }
}
