//
//  HorizontalProgressBar.cpp
//  moFloTest
//
//  Created by Scott Downie on 27/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/GUI/ProgressBar/HorizontalProgressBar.h>
#include <ChilliSource/GUI/Image/ImageView.h>

#include <ChilliSource/Rendering/Texture/TextureAtlas.h>
#include <ChilliSource/Rendering/Texture/Texture.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/Core/Resource/ResourcePool.h>

namespace ChilliSource
{
    namespace GUI
    {
		DEFINE_META_CLASS(HorizontalProgressBar)

		DEFINE_PROPERTY(BackgroundTexture);
		DEFINE_PROPERTY(ProgressTexture);
		DEFINE_PROPERTY(BackgroundTextureAtlas);
		DEFINE_PROPERTY(ProgressTextureAtlas);
		DEFINE_PROPERTY(BackgroundTextureAtlasID);
		DEFINE_PROPERTY(ProgressTextureAtlasID);

        //------------------------------------------------------
        /// Constructor
        ///
        /// Create the subviews that make up the container
        //------------------------------------------------------
        HorizontalProgressBar::HorizontalProgressBar() : mpBackgroundImage(new ImageView()), mpProgressImage(new ImageView())
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
		: ProgressBar(insParams), mpBackgroundImage(new ImageView()), mpProgressImage(new ImageView())
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
                eBackgroundTextureLocation = CSCore::ParseStorageLocation(strValue);
            }
            if(insParams.TryGetValue("BackgroundTexture", strValue))
            {
                Core::ResourcePool* resourcePool = Core::Application::Get()->GetResourcePool();
				mpBackgroundImage->SetTexture(resourcePool->LoadResource<Rendering::Texture>(eBackgroundTextureLocation, strValue));
            }
            //---Progress Texture
            Core::StorageLocation eProgressTextureLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("ProgressTextureLocation", strValue))
            {
                eProgressTextureLocation = CSCore::ParseStorageLocation(strValue);
            }
            if(insParams.TryGetValue("ProgressTexture", strValue))
            {
                Core::ResourcePool* resourcePool = Core::Application::Get()->GetResourcePool();
				mpProgressImage->SetTexture(resourcePool->LoadResource<Rendering::Texture>(eProgressTextureLocation, strValue));
            }
            //---Background sprite sheet
            Core::StorageLocation eBackgroundTextureAtlasLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("BackgroundTextureAtlasLocation", strValue))
            {
                eBackgroundTextureAtlasLocation = CSCore::ParseStorageLocation(strValue);
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
                eProgressTextureAtlasLocation = CSCore::ParseStorageLocation(strValue);
            }
			if(insParams.TryGetValue("ProgressTextureAtlas", strValue))
			{
                Core::ResourcePool* resourcePool = Core::Application::Get()->GetResourcePool();
				SetProgressTextureAtlas(resourcePool->LoadResource<Rendering::TextureAtlas>(eProgressTextureAtlasLocation, strValue));
			}
			//---Background index ID
			if(insParams.TryGetValue("BackgroundTextureAtlasID", strValue))
			{
				SetBackgroundTextureAtlasID(strValue);
			}
			//---Progress index ID
			if(insParams.TryGetValue("ProgressTextureAtlasID", strValue))
			{
				CS_ASSERT(ProgressTextureAtlas, "Cannot set sprite sheet index without setting sprite sheet");
				SetProgressTextureAtlasID(strValue);
			}
		}
		//--------------------------------------------------------
		/// Set Background Sprite Sheet
		///
		/// @param Sprite sheet
		//--------------------------------------------------------
		void HorizontalProgressBar::SetBackgroundTextureAtlas(const Rendering::TextureAtlasCSPtr& inpTextureAtlas)
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
		void HorizontalProgressBar::SetProgressTextureAtlas(const Rendering::TextureAtlasCSPtr& inpTextureAtlas)
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
		const Rendering::TextureAtlasCSPtr& HorizontalProgressBar::GetBackgroundTextureAtlas() const
		{
			return BackgroundTextureAtlas;
		}
		//--------------------------------------------------------
		/// Get Progress Sprite Sheet
		///
		/// @return Sprite sheet
		//--------------------------------------------------------
		const Rendering::TextureAtlasCSPtr& HorizontalProgressBar::GetProgressTextureAtlas() const
		{
			return ProgressTextureAtlas;
		}
		//--------------------------------------------------------
		/// Set Background Sprite Sheet Index ID
		///
		/// @param The index ID of the image within the sprite sheet
		//--------------------------------------------------------
		void HorizontalProgressBar::SetBackgroundTextureAtlasID(const std::string& instrID)
		{
			CS_ASSERT(BackgroundTextureAtlas, "Cannot set sprite sheet index without setting sprite sheet");
			BackgroundTextureAtlasID = instrID;
            mpBackgroundImage->SetTextureAtlasID(instrID);
		}
		//--------------------------------------------------------
		/// Set Progress Sprite Sheet Index ID
		///
		/// @param The index ID of the image within the sprite sheet
		//--------------------------------------------------------
		void HorizontalProgressBar::SetProgressTextureAtlasID(const std::string& instrID)
		{
			CS_ASSERT(ProgressTextureAtlas, "Cannot set sprite sheet index without setting sprite sheet");
			ProgressTextureAtlasID = instrID;
            mpProgressImage->SetTextureAtlasID(instrID);
		}
		//--------------------------------------------------------
		/// Get Progress Sprite Sheet Index ID
		///
		/// @return The index ID of the image within the sprite sheet
		//--------------------------------------------------------
		const std::string& HorizontalProgressBar::GetBackgroundTextureAtlasID() const
		{
			return BackgroundTextureAtlasID;
		}
		//--------------------------------------------------------
		/// Get Progress Sprite Sheet Index ID
		///
		/// @return The index ID of the image within the sprite sheet
		//--------------------------------------------------------
		const std::string& HorizontalProgressBar::GetProgressTextureAtlasID() const
		{
			return ProgressTextureAtlasID;
		}
        //------------------------------------------------------
        /// Set Background Image
        ///
        /// @param Texture representing the background
        //------------------------------------------------------
        void HorizontalProgressBar::SetBackgroundImage(const Rendering::TextureCSPtr& inpTexture)
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
		void HorizontalProgressBar::SetProgressImage(const Rendering::TextureCSPtr& inpTexture)
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
		const Rendering::TextureCSPtr& HorizontalProgressBar::GetBackgroundImage() const
		{
			return BackgroundTexture;
		}
		//------------------------------------------------------
		/// Get Progress Image
		///
		/// @return Texture representing the bar
		//------------------------------------------------------
		const Rendering::TextureCSPtr& HorizontalProgressBar::GetProgressImage() const
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
