//
//  HighlightButton.cpp
//  moFlo
//
//  Created by Scott Downie on 26/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/GUI/Button/HighlightButton.h>
#include <ChilliSource/GUI/Image/ImageView.h>

#include <ChilliSource/Rendering/Sprite/SpriteSheet.h>
#include <ChilliSource/Rendering/Sprite/SpriteSheetManager.h>
#include <ChilliSource/Rendering/Texture/TextureManager.h>
#include <ChilliSource/Rendering/Texture/Texture.h>

#include <ChilliSource/Core/Base/MakeDelegate.h>
#include <ChilliSource/Core/Base/Screen.h>
#include <ChilliSource/Core/Resource/ResourceManagerDispenser.h>
#include <ChilliSource/Core/Entity/ComponentFactoryDispenser.h>
#include <ChilliSource/Core/String/StringParser.h>

#include <ChilliSource/Audio/3D/AudioComponent.h>
#include <ChilliSource/Audio/3D/AudioComponentFactory.h>

#include <cmath>

namespace ChilliSource
{
    namespace GUI
    {
        Audio::AudioComponentSPtr HighlightButton::mspDefaultSelectSound;
        Audio::AudioComponentSPtr HighlightButton::mspDefaultDeSelectSound;
        
		DEFINE_META_CLASS(HighlightButton)

		DEFINE_PROPERTY(NormalTexture);
		DEFINE_PROPERTY(HighlightTexture);
		DEFINE_PROPERTY(NormalSpriteSheet);
		DEFINE_PROPERTY(HighlightSpriteSheet);

		DEFINE_PROPERTY(HighlightColour);

		DEFINE_PROPERTY(NormalSpriteSheetIndex);
		DEFINE_PROPERTY(HighlightSpriteSheetIndex);
		DEFINE_PROPERTY(NormalSpriteSheetIndexID);
		DEFINE_PROPERTY(HighlightSpriteSheetIndexID);

		DEFINE_PROPERTY(SizeFromImage);
		DEFINE_PROPERTY(HeightMaintain);
		DEFINE_PROPERTY(WidthMaintain);
        //-----------------------------------------------------------
        /// Constructor
        ///
        /// Create the widget by adding the background image
        //-----------------------------------------------------------
        HighlightButton::HighlightButton() 
        : mpBackgroundImage(new ImageView()), NormalSpriteSheetIndex(0), HighlightSpriteSheetIndex(0), HighlightColour(0.7f, 0.7f, 0.7f, 1.0f),
        msDefaultUVs(Core::Vector2::ZERO, Core::Vector2::ONE),
        msHighlightUVs(Core::Vector2::ZERO, Core::Vector2::ONE),
        mbSelected(false), SizeFromImage(false), HeightMaintain(false), WidthMaintain(false), WidthFromImage(false), HeightFromImage(false), mbFillMaintain(false), mbFitMaintain(false)
        {
            mpBackgroundImage->SetSize(Core::UnifiedVector2(Core::Vector2(1.0f, 1.0f), Core::Vector2(0, 0)));
            mpBackgroundImage->SetPosition(Core::UnifiedVector2(Core::Vector2(0.5f, 0.5f), Core::Vector2(0, 0)));
            mpBackgroundImage->EnableUserInteraction(false);
            AddSubview(mpBackgroundImage);
			
            m_pressedInsideConnection = mInputEvents.GetPressedInsideEvent().OpenConnection(Core::MakeDelegate(this, &HighlightButton::OnButtonSelect));
            m_releasedInsideConnection = mInputEvents.GetReleasedInsideEvent().OpenConnection(Core::MakeDelegate(this, &HighlightButton::OnButtonActivated));
            m_movedWithinConnection = mInputEvents.GetMovedWithinEvent().OpenConnection(Core::MakeDelegate(this, &HighlightButton::OnButtonDeselectThreshold));
            m_movedOutsideConnection = mInputEvents.GetMovedOutsideEvent().OpenConnection(Core::MakeDelegate(this, &HighlightButton::OnButtonDeselect));
        }
        //------------------------------------------------------------
        /// Constructor
        ///
        /// From param dictionary
        //------------------------------------------------------------
        HighlightButton::HighlightButton(const Core::ParamDictionary& insParams) 
        : Button(insParams), mpBackgroundImage(new ImageView()), NormalSpriteSheetIndex(0), HighlightSpriteSheetIndex(0),
        msDefaultUVs(Core::Vector2::ZERO, Core::Vector2::ONE),
        msHighlightUVs(Core::Vector2::ZERO, Core::Vector2::ONE),
        mbSelected(false), HighlightColour(0.7f, 0.7f, 0.7f, 1.0f),
		SizeFromImage(false), HeightMaintain(false), WidthMaintain(false), WidthFromImage(false), HeightFromImage(false), mbFillMaintain(false), mbFitMaintain(false)
        {
            std::string strValue;
            
            if(mspDefaultSelectSound)
                SetSelectAudioEffect(mspDefaultSelectSound);
            
            if(mspDefaultDeSelectSound)
                SetDeselectAudioEffect(mspDefaultDeSelectSound);
            
            //---Default
            Core::StorageLocation eNormalTextureLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("NormalTextureLocation", strValue))
            {
                eNormalTextureLocation = ChilliSource::Core::ParseStorageLocation(strValue);
            }
            if(insParams.TryGetValue("NormalTexture", strValue))
            {
                SetNormalImage(LOAD_RESOURCE(Rendering::Texture, eNormalTextureLocation, strValue));
            }
            //---Highlight
            Core::StorageLocation eHighlightTextureLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("HighlightTextureLocation", strValue))
            {
                eHighlightTextureLocation = ChilliSource::Core::ParseStorageLocation(strValue);
            }
            if(insParams.TryGetValue("HighlightTexture", strValue))
            {
                SetHighlightImage(LOAD_RESOURCE(Rendering::Texture, eHighlightTextureLocation, strValue));
            }
            //---Sprite sheet
            Core::StorageLocation eNormalSpriteSheetLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("NormalSpriteSheetLocation", strValue))
            {
                eNormalSpriteSheetLocation = ChilliSource::Core::ParseStorageLocation(strValue);
            }
            if(insParams.TryGetValue("NormalSpriteSheet", strValue))
            {
                SetNormalSpriteSheet(LOAD_RESOURCE(Rendering::SpriteSheet, eNormalSpriteSheetLocation, strValue));
            }
            //---Sprite sheet
            Core::StorageLocation eHighlightSpriteSheetLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("HighlightSpriteSheetLocation", strValue))
            {
                eHighlightSpriteSheetLocation = ChilliSource::Core::ParseStorageLocation(strValue);
            }
            if(insParams.TryGetValue("HighlightSpriteSheet", strValue))
            {
                SetHighlightSpriteSheet(LOAD_RESOURCE(Rendering::SpriteSheet, eHighlightSpriteSheetLocation, strValue));
            }
            //---Default index
            if(insParams.TryGetValue("NormalSpriteSheetIndex", strValue))
            {
                CS_ASSERT(NormalSpriteSheet, "Sprite sheet index cannot be set without sprite sheet");
                SetNormalSpriteSheetIndex(Core::ParseU32(strValue));
            }
            //---Highlight index
            if(insParams.TryGetValue("HighlightSpriteSheetIndex", strValue))
            {
				CS_ASSERT(HighlightSpriteSheet, "Sprite sheet index cannot be set without sprite sheet");
				SetHighlightSpriteSheetIndex(Core::ParseU32(strValue));
            }
			//---Default index ID
			if(insParams.TryGetValue("NormalSpriteSheetIndexID", strValue))
			{
				SetNormalSpriteSheetIndexID(strValue);
			}
			//---Highlight index ID
			if(insParams.TryGetValue("HighlightSpriteSheetIndexID", strValue))
			{
				SetHighlightSpriteSheetIndexID(strValue);
			}
            //---Highlight Colour
            if(insParams.TryGetValue("HighlightColour", strValue))
            {
                SetHighlightColour(Core::ParseColour(strValue));
            }
			//---Size from image
            if(insParams.TryGetValue("SizeFromImage", strValue))
            {
                SizeFromImage = Core::ParseBool(strValue);
            }
            //---Width from image
            if(insParams.TryGetValue("WidthFromImage", strValue))
            {
                EnableWidthFromImage(Core::ParseBool(strValue));
            }
            //---Height from image
            if(insParams.TryGetValue("HeightFromImage", strValue))
            {
                EnableHeightFromImage(Core::ParseBool(strValue));
            }
			//---Maintain Width
			if(insParams.TryGetValue("HeightMaintain", strValue))
			{
				HeightMaintain = Core::ParseBool(strValue);
			}
			//---Maintain Height
			if(insParams.TryGetValue("WidthMaintain", strValue))
			{
				WidthMaintain = Core::ParseBool(strValue);
			}
			//---Set Maintain Height
			if(insParams.TryGetValue("SetHeightMaintain", strValue))
			{
				Core::Vector2 vSize = Core::ParseVector2(strValue);
				HeightMaintain = true;
				SetHeightMaintainingAspect(vSize.x, vSize.y);
			}
			//---Set Maintain Width
			if(insParams.TryGetValue("SetWidthMaintain", strValue))
			{
				Core::Vector2 vSize = Core::ParseVector2(strValue);
				WidthMaintain = true;
				SetWidthMaintainingAspect(vSize.x, vSize.y);
			}
            //---Set Fill Maintain
            if(insParams.TryGetValue("SetFillMaintain", strValue))
            {
                Core::Vector4 vSize = Core::ParseVector4(strValue);
                SetFillMaintainingAspect(vSize.x, vSize.y, vSize.z, vSize.w);
            }
            //---Set Fit Maintain
            if(insParams.TryGetValue("SetFitMaintain", strValue))
            {
                Core::Vector4 vSize = Core::ParseVector4(strValue);
                SetFitMaintainingAspect(vSize.x, vSize.y, vSize.z, vSize.w);
            }
            //---Audio effect
            Core::StorageLocation eSelectAudioLocation = Core::StorageLocation::k_package;
            Core::StorageLocation eDeselectAudioLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("SelectAudioEffectLocation", strValue))
            {
                eSelectAudioLocation = ChilliSource::Core::ParseStorageLocation(strValue);
            }
			if(insParams.TryGetValue("DeselectAudioEffectLocation", strValue))
            {
                eDeselectAudioLocation = ChilliSource::Core::ParseStorageLocation(strValue);
            }
            if(insParams.TryGetValue("SelectAudioEffect", strValue))
            {
                Audio::AudioComponentFactory* pAudioFactory = GET_COMPONENT_FACTORY(Audio::AudioComponentFactory);
                SetSelectAudioEffect(pAudioFactory->CreateAudioComponent(eSelectAudioLocation, strValue, false, false));
            }
			if(insParams.TryGetValue("DeselectAudioEffect", strValue))
            {
                Audio::AudioComponentFactory* pAudioFactory = GET_COMPONENT_FACTORY(Audio::AudioComponentFactory);
                SetDeselectAudioEffect(pAudioFactory->CreateAudioComponent(eDeselectAudioLocation, strValue, false, false));
            }
            
            mpBackgroundImage->SetSize(Core::UnifiedVector2(Core::Vector2(1.0f, 1.0f), Core::Vector2(0, 0)));
            mpBackgroundImage->SetPosition(Core::UnifiedVector2(Core::Vector2(0.5f, 0.5f), Core::Vector2(0, 0)));
            mpBackgroundImage->EnableUserInteraction(false);
            AddSubview(mpBackgroundImage);
			
            m_pressedInsideConnection = mInputEvents.GetPressedInsideEvent().OpenConnection(Core::MakeDelegate(this, &HighlightButton::OnButtonSelect));
            m_releasedInsideConnection = mInputEvents.GetReleasedInsideEvent().OpenConnection(Core::MakeDelegate(this, &HighlightButton::OnButtonActivated));
            m_movedWithinConnection = mInputEvents.GetMovedWithinEvent().OpenConnection(Core::MakeDelegate(this, &HighlightButton::OnButtonDeselectThreshold));
            m_movedOutsideConnection = mInputEvents.GetMovedOutsideEvent().OpenConnection(Core::MakeDelegate(this, &HighlightButton::OnButtonDeselect));
        }
        //-----------------------------------------------------------
        /// Get Background Image View
        ///
        /// @return Image view
        //-----------------------------------------------------------
        const ImageViewSPtr& HighlightButton::GetBackgroundImageView() const
        {
            return mpBackgroundImage;
        }
        //-----------------------------------------------------------
        /// Set Default Image
        ///
        /// Image for unselected button
        ///
        /// @param Texture shared pointer
        //-----------------------------------------------------------
        void HighlightButton::SetNormalImage(const Rendering::TextureSPtr& inpTexture)
        {
            NormalTexture = inpTexture;
			mpBackgroundImage->SetTexture(inpTexture);
            mpBackgroundImage->SetUVs(msDefaultUVs);
            
            if(!HighlightTexture)
            {
                SetHighlightImage(inpTexture);
            }
        }
		//-----------------------------------------------------------
		/// Get Default Image
		///
		/// @return Texture for non-selected button
		//-----------------------------------------------------------
		const Rendering::TextureSPtr& HighlightButton::GetNormalImage() const
		{
			return NormalTexture;
		}
        //-----------------------------------------------------------
        /// Set Highlight Image
        ///
        /// Image for selected button
        ///
        /// @param Texture shared pointer
        //-----------------------------------------------------------
        void HighlightButton::SetHighlightImage(const Rendering::TextureSPtr& inpTexture)
        {
            HighlightTexture = inpTexture;
            
            if(!NormalTexture)
            {
                SetNormalImage(inpTexture);
            }
        }
		//-----------------------------------------------------------
		/// Get Highlight Image
		///
		/// @return Texture for selected button
		//-----------------------------------------------------------
		const Rendering::TextureSPtr& HighlightButton::GetHighlightImage() const
		{
			return HighlightTexture;
		}
        //-----------------------------------------------------------

        /// Set Default UVs
        ///
        /// Sets the UV coordinates for the default texture
        ///
        /// @param Rectangle for the UVs
        //-----------------------------------------------------------
        void HighlightButton::SetDefaultUVs(Core::Rectangle insUVs)
        {
            msDefaultUVs = insUVs;
            
            if(mpBackgroundImage->GetTexture() == NormalTexture)
                mpBackgroundImage->SetUVs(msDefaultUVs);
        }
        //-----------------------------------------------------------
        /// Set Highlight UVs
        ///
        /// Sets the UV coordinates for the Highlight texture
        ///
        /// @param Rectangle for the UVs
        //-----------------------------------------------------------
        void HighlightButton::SetHighlightUVs(Core::Rectangle insUVs)
        {
            msHighlightUVs = insUVs;
            
            if(mpBackgroundImage->GetTexture() == HighlightTexture)
                mpBackgroundImage->SetUVs(msHighlightUVs);
        }

        //-----------------------------------------------------------
        /// Set Normal Sprite Sheet
        ///
        /// @param Sprite sheet with default image
        //-----------------------------------------------------------
        void HighlightButton::SetNormalSpriteSheet(const Rendering::SpriteSheetSPtr& inpSpriteSheet)
        {
            NormalSpriteSheet = inpSpriteSheet;
            mpBackgroundImage->SetSpriteSheet(inpSpriteSheet);
            
            if(!HighlightSpriteSheet && NormalSpriteSheet)
            {
                SetHighlightSpriteSheet(inpSpriteSheet);
            }
        }
		//-----------------------------------------------------------
		/// Get Normal Sprite Sheet
		///
		/// @return Sprite sheet with default image
		//-----------------------------------------------------------
		const Rendering::SpriteSheetSPtr& HighlightButton::GetNormalSpriteSheet() const
		{ 
			return NormalSpriteSheet; 
		}
		//-----------------------------------------------------------
		/// Set Highlight Sprite Sheet
		///
		/// @param Sprite sheet with highlight image
		//-----------------------------------------------------------
		void HighlightButton::SetHighlightSpriteSheet(const Rendering::SpriteSheetSPtr& inpSpriteSheet)
		{
			HighlightSpriteSheet = inpSpriteSheet;
			mpBackgroundImage->SetSpriteSheet(inpSpriteSheet);
            
            if(!NormalSpriteSheet && HighlightSpriteSheet)
            {
                SetNormalSpriteSheet(inpSpriteSheet);
            }
		}
		//-----------------------------------------------------------
		/// Get Highlight Sprite Sheet
		///
		/// @return Sprite sheet with highlight image
		//-----------------------------------------------------------
		const Rendering::SpriteSheetSPtr& HighlightButton::GetHighlightSpriteSheet() const 
		{ 
			return HighlightSpriteSheet; 
		}
        //-----------------------------------------------------------
        /// Set Normal Sprite Sheet Index
        ///
        /// @param Index of default state on sprite sheet
        //-----------------------------------------------------------
        void HighlightButton::SetNormalSpriteSheetIndex(u32 inudwIndex)
        {
            NormalSpriteSheetIndex = inudwIndex;
            mpBackgroundImage->SetSpriteSheetIndex(inudwIndex);
        }
        //-----------------------------------------------------------
        /// Set Highlight Sprite Sheet Index
        ///
        /// @param Index of highlight state on sprite sheet
        //-----------------------------------------------------------
        void HighlightButton::SetHighlightSpriteSheetIndex(u32 inudwIndex)
        {
            HighlightSpriteSheetIndex = inudwIndex;
        }
		//-----------------------------------------------------------
		/// Get Normal Sprite Sheet Index
		///
		/// @return Index of default state on sprite sheet
		//-----------------------------------------------------------
		u32 HighlightButton::GetNormalSpriteSheetIndex() const
		{
			return NormalSpriteSheetIndex;
		}
		//-----------------------------------------------------------
		/// Get Highlight Sprite Sheet Index
		///
		/// @return Index of highlight state on sprite sheet
		//-----------------------------------------------------------
		u32 HighlightButton::GetHighlightSpriteSheetIndex() const
		{
			return HighlightSpriteSheetIndex;
		}
		//-----------------------------------------------------------
		/// Set Normal Sprite Sheet Index
		///
		/// @param Index of default state on sprite sheet
		//-----------------------------------------------------------
		void HighlightButton::SetNormalSpriteSheetIndexID(const std::string& instrID)
		{
			CS_ASSERT(NormalSpriteSheet, "Cannot set sprite sheet index without setting sprite sheet");
			NormalSpriteSheetIndexID = instrID;
			SetNormalSpriteSheetIndex(NormalSpriteSheet->GetFrameIndexByID(instrID));
		}
		//-----------------------------------------------------------
		/// Set Highlight Sprite Sheet Index ID
		///
		/// @param Index ID of highlight state on sprite sheet
		//-----------------------------------------------------------
		void HighlightButton::SetHighlightSpriteSheetIndexID(const std::string& instrID)
		{
			CS_ASSERT(HighlightSpriteSheet, "Cannot set sprite sheet index without setting sprite sheet");
			HighlightSpriteSheetIndexID = instrID;
			SetHighlightSpriteSheetIndex(HighlightSpriteSheet->GetFrameIndexByID(instrID));
		}
		//-----------------------------------------------------------
		/// Get Normal Sprite Sheet Index ID
		///
		/// @return Index ID of default state on sprite sheet
		//-----------------------------------------------------------
		const std::string& HighlightButton::GetNormalSpriteSheetIndexID() const
		{
			return NormalSpriteSheetIndexID;
		}
		//-----------------------------------------------------------
		/// Get Highlight Sprite Sheet Index ID
		///
		/// @return Index ID of highlight state on sprite sheet
		//-----------------------------------------------------------
		const std::string& HighlightButton::GetHighlightSpriteSheetIndexID() const
		{
			return HighlightSpriteSheetIndexID;
		}
		//--------------------------------------------------------
        /// Enable Size From Image
        ///
        /// When this is enabled the image view's size will be 
        /// based on the size of the image
        ///
        /// @param Enable/disable
        //--------------------------------------------------------
        void HighlightButton::EnableSizeFromImage(bool inbEnable)
        {
            SizeFromImage = inbEnable;
        }
		//--------------------------------------------------------
		/// Is Size From Image Enabled
		///
		/// When this is enabled the image view's size will be 
		/// based on the size of the image
		///
		/// @return Whether the image view's size will be 
		/// based on the size of the image
		//--------------------------------------------------------
		bool HighlightButton::IsSizeFromImageEnabled() const
		{
			return SizeFromImage;
		}
		//--------------------------------------------------------
		/// Set Highlight Colour
		///
		/// @param Tint colour to apply when the button is
		/// selected
		//--------------------------------------------------------
		void HighlightButton::SetHighlightColour(const Core::Colour & inValue)
		{
			HighlightColour = inValue;
		}
		//--------------------------------------------------------
		/// Get Highlight Colour
		///
		/// @return Tint colour to apply when the button is
		/// selected
		//--------------------------------------------------------
		const Core::Colour & HighlightButton::GetHighlightColour() const
		{
			return HighlightColour;
		}
        //-----------------------------------------------------------
        /// Set Select Audio Effect
        ///
        /// @param Audio effect played when the button is selected
        //-----------------------------------------------------------
        void HighlightButton::SetSelectAudioEffect(const Audio::AudioComponentSPtr& inpEffect)
        {
            mpSelectAudioEffect = inpEffect;
        }
		//-----------------------------------------------------------
		/// Set De-Select Audio Effect
		///
		/// @param Audio effect played when the button is selected
		//-----------------------------------------------------------
		void HighlightButton::SetDeselectAudioEffect(const Audio::AudioComponentSPtr& inpEffect)
		{
			mpDeselectAudioEffect = inpEffect;
		}
        void HighlightButton::SetDefaultSelectAudioEffect(Core::StorageLocation ineLocation, const std::string instrAudioEffect)
        {
            Audio::AudioComponentFactory* pAudioFactory = GET_COMPONENT_FACTORY(Audio::AudioComponentFactory);
            mspDefaultSelectSound = pAudioFactory->CreateAudioComponent(ineLocation, instrAudioEffect, false, false);
        }
        
        void HighlightButton::SetDefaultDeselectAudioEffect(Core::StorageLocation ineLocation, const std::string instrAudioEffect)
        {
            Audio::AudioComponentFactory* pAudioFactory = GET_COMPONENT_FACTORY(Audio::AudioComponentFactory);
            mspDefaultDeSelectSound = pAudioFactory->CreateAudioComponent(ineLocation, instrAudioEffect, false, false);
        }
        //-----------------------------------------------------------
        //-----------------------------------------------------------
        void HighlightButton::OnButtonSelect(GUIView* in_button, const Input::PointerSystem::Pointer& in_pointer)
        {
			if(!mbSelected)
			{
                mvSelectedPos = in_pointer.m_location;
                
				mbSelected = true;
				
				if(mpSelectAudioEffect && !mpSelectAudioEffect->IsPlaying())
				{
					mpSelectAudioEffect->Play();
				}
				
				if(HighlightTexture)
				{
					mpBackgroundImage->SetTexture(HighlightTexture);
                    mpBackgroundImage->SetUVs(msHighlightUVs);
                    mpBackgroundImage->SetColour(HighlightColour);
				}
				else if(NormalSpriteSheet)
				{
					bool bUniqueHighlight = HighlightSpriteSheetIndex != NormalSpriteSheetIndex;
					
					if (bUniqueHighlight && HighlightSpriteSheet)
					{
						mpBackgroundImage->SetSpriteSheet(HighlightSpriteSheet);
						mpBackgroundImage->SetSpriteSheetIndex(HighlightSpriteSheetIndex);
					} 
					else 
					{
						mpBackgroundImage->SetColour(HighlightColour);
					}

				}
			}
        }
        //-----------------------------------------------------------
        //-----------------------------------------------------------
        void HighlightButton::OnButtonDeselect(GUIView* in_button, const Input::PointerSystem::Pointer& in_pointer)
        {
			if(mbSelected)
			{
				if(mpDeselectAudioEffect && !mpDeselectAudioEffect->IsPlaying())
				{
					mpDeselectAudioEffect->Play();
				}
				
				if(NormalTexture)
				{
					mpBackgroundImage->SetTexture(NormalTexture);
                    mpBackgroundImage->SetUVs(msDefaultUVs);
                    mpBackgroundImage->SetColour(Core::Colour::k_white);
				}
				else if(NormalSpriteSheet)
				{
					mpBackgroundImage->SetColour(Core::Colour::k_white);
					mpBackgroundImage->SetSpriteSheetIndex(NormalSpriteSheetIndex);
					mpBackgroundImage->SetSpriteSheet(NormalSpriteSheet);
				}
				
				mbSelected = false;
			}
        }
        //-----------------------------------------------------------
        //-----------------------------------------------------------
        void HighlightButton::OnButtonDeselectThreshold(GUIView* in_button, const Input::PointerSystem::Pointer& in_pointer)
        {
            const f32 kfThreshold = (f32)(Core::Screen::GetOrientedWidth()) * 0.02f;
            f32 fDistX = std::abs(in_pointer.m_location.x - mvSelectedPos.x);
            f32 fDisty = std::abs(in_pointer.m_location.y - mvSelectedPos.y);
			if(fDistX >= kfThreshold || fDisty >= kfThreshold)
			{
                OnButtonDeselect(in_button, in_pointer);
			}
        }
        //-----------------------------------------------------------
        //-----------------------------------------------------------
        void HighlightButton::OnButtonActivated(GUIView* in_button, const Input::PointerSystem::Pointer& in_pointer)
        {
			if(mbSelected)
			{
				OnButtonDeselect(in_button, in_pointer);
				mOnButtonActivatedEvent.NotifyConnections(this);
			}
        }
		//-----------------------------------------------------------
		/// Draw
		///
		/// Overloaded draw method
		///
		/// @param Canvas renderer
		//-----------------------------------------------------------
		void HighlightButton::Draw(Rendering::CanvasRenderer* inpCanvasRenderer)
		{
			SetSizeFromImage();
			GUIView::Draw(inpCanvasRenderer);
		}
		
		void HighlightButton::SetSizeFromImage()
		{
			if(SizeFromImage)
			{
				Core::Vector2 vImageSize = mpBackgroundImage->GetSizeFromImage();
				SetSize(0.0f, 0.0f, vImageSize.x, vImageSize.y);
			}
            else if(HeightFromImage)
            {
                SetHeightFromImage();
            }
            else if(WidthFromImage)
            {
                SetWidthFromImage();
            }
			else
            {
                UpdateSizeMaintainingAspect();
            }
		}
        //--------------------------------------------------------
        /// Update Size Maintaining Aspect
        ///
        /// Image view can be sized from assets etc. This function
        /// calculates the size based on these factors
        //--------------------------------------------------------
        void HighlightButton::UpdateSizeMaintainingAspect()
        {
            if(mbFillMaintain)
            {
                SetFillMaintainingAspect(mvFillMaintainTarget.GetRelative().x, mvFillMaintainTarget.GetRelative().y, mvFillMaintainTarget.GetAbsolute().x, mvFillMaintainTarget.GetAbsolute().y);
            }
            else if(mbFitMaintain)
            {
                SetFitMaintainingAspect(mvFitMaintainTarget.GetRelative().x, mvFitMaintainTarget.GetRelative().y, mvFitMaintainTarget.GetAbsolute().x, mvFitMaintainTarget.GetAbsolute().y);
            }
            else if(HeightMaintain)
            {
                Core::UnifiedVector2 uvSize = GetSize();
                SetHeightMaintainingAspect(uvSize.GetRelative().y, uvSize.GetAbsolute().y);
            }
            else if(WidthMaintain)
            {
                Core::UnifiedVector2 uvSize = GetSize();
                SetWidthMaintainingAspect(uvSize.GetRelative().x, uvSize.GetAbsolute().x);
            }
        }
		
        //--------------------------------------------------------
        /// Enable Width From Image
        ///
        /// When this is enabled the image view's width will be
        /// based on the width of the image
        ///
        /// @param Enable/disable
        //--------------------------------------------------------
        void HighlightButton::EnableWidthFromImage(bool inbEnable)
        {
            WidthFromImage = inbEnable;
            
            LayoutContent();
            LayoutChildrensContent();
        }
        //--------------------------------------------------------
        /// Is Width From Image Enabled
        ///
        /// When this is enabled the image view's width will be
        /// based on the width of the image
        ///
        /// @return Whether the image view's width will be
        /// based on the width of the image
        //--------------------------------------------------------
        bool HighlightButton::IsWidthFromImageEnabled() const
        {
            return WidthFromImage;
        }
        //--------------------------------------------------------
        /// Set Width From Image
        ///
        /// Sets the of the image view to the width of the image
        /// without requiring a draw to call. Useful for working
        /// with relative sizes and aspect ratios.
        //--------------------------------------------------------
        void HighlightButton::SetWidthFromImage()
        {
            SetSize(0.0f, GetSize().GetRelative().y, mpBackgroundImage->GetSizeFromImage().x, GetSize().GetAbsolute().y);
        }
        //--------------------------------------------------------
        /// Enable Height From Image
        ///
        /// When this is enabled the image view's height will be
        /// based on the height of the image
        ///
        /// @param Enable/disable
        //--------------------------------------------------------
        void HighlightButton::EnableHeightFromImage(bool inbEnable)
        {
            HeightFromImage = inbEnable;
            
            LayoutContent();
            LayoutChildrensContent();
        }
        //--------------------------------------------------------
        /// Is Height From Image Enabled
        ///
        /// When this is enabled the image view's height will be
        /// based on the height of the image
        ///
        /// @return Whether the image view's height will be
        /// based on the height of the image
        //--------------------------------------------------------
        bool HighlightButton::IsHeightFromImageEnabled() const
        {
            return HeightFromImage;
        }
        //--------------------------------------------------------
        /// Set Height From Image
        ///
        /// Sets the of the image view to the height of the image
        /// without requiring a draw to call. Useful for working
        /// with relative sizes and aspect ratios.
        //--------------------------------------------------------
        void HighlightButton::SetHeightFromImage()
        {
            SetSize(GetSize().GetRelative().x, 0.0f, GetSize().GetAbsolute().x, mpBackgroundImage->GetSizeFromImage().y);
        }
        //--------------------------------------------------------
        /// Layout Content
        ///
        /// Called when the view is able to retrieve an absolute
        /// value.
        //--------------------------------------------------------
		void HighlightButton::LayoutContent()
		{
            SetSizeFromImage();
            GUIView::LayoutContent();
		}
        
		//--------------------------------------------------------
		/// Get Size From Image
		///
		/// @return Absolute size of the image
		//--------------------------------------------------------
		Core::Vector2 HighlightButton::GetSizeFromImage() const
		{
			return GetBackgroundImageView()->GetSizeFromImage();
		}
        //--------------------------------------------------------
        /// Set Width Maintaining Aspect
        ///
        /// Change the width of the image and resize the height
        /// to maintain the aspect ratio
        ///
        /// @param Unified width
        //--------------------------------------------------------
        void HighlightButton::SetWidthMaintainingAspect(f32 infRelWidth, f32 infAbsWidth)
        {
            WidthMaintain = true;
            
            SetSize(infRelWidth, 0.0f, infAbsWidth, 0.0f);
			
			f32 fScaleY = GetAbsoluteScale().y;
			if(fScaleY == 0.0f)
				return;
            
            Core::Vector2 vCurrentSize = GetSizeFromImage();
            f32 fAspectRatio = vCurrentSize.y / vCurrentSize.x;
            
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
        void HighlightButton::SetHeightMaintainingAspect(f32 infRelHeight, f32 infAbsHeight)
        {
            HeightMaintain = true;
            
            SetSize(0.0f, infRelHeight, 0.0f, infAbsHeight);
			
			f32 fScaleX = GetAbsoluteScale().x;
			if(fScaleX == 0.0f)
				return;
            
            Core::Vector2 vCurrentSize = GetSizeFromImage();
            f32 fAspectRatio = vCurrentSize.x / vCurrentSize.y;
            
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
        void HighlightButton::EnableHeightMaintainingAspect(bool inbEnabled)
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
        void HighlightButton::EnableWidthMaintainingAspect(bool inbEnabled)
        {
			WidthMaintain = inbEnabled;
        }
		//--------------------------------------------------------
		/// Is Width Maintaining Aspect Enabled
		///
		/// @return Whether auto scaling of the width to maintain the aspect ratio
		//--------------------------------------------------------
		bool HighlightButton::IsWidthMaintainingAspectEnabled() const
		{
			return WidthMaintain;
		}
		//--------------------------------------------------------
		/// Is Height Maintaining Aspect Enabled
		///
		/// @return Whether auto scaling of the height to maintain the aspect ratio
		//--------------------------------------------------------
		bool HighlightButton::IsHeightMaintainingAspectEnabled() const
		{
			return HeightMaintain;
		}
        //--------------------------------------------------------
        /// Set Fill Maintaining Aspect
        ///
        /// Based on the size of the image and the target size
        /// resize the image view to be the minimum of the target
        /// size while maintaining the aspect ratio
        ///
        /// @param Unified size
        //--------------------------------------------------------
        void HighlightButton::SetFillMaintainingAspect(f32 infRelWidth, f32 infRelHeight, f32 infAbsWidth, f32 infAbsHeight)
        {
            mbFillMaintain = true;
            mvFillMaintainTarget.vRelative.x = infRelWidth;
            mvFillMaintainTarget.vRelative.y = infRelHeight;
            mvFillMaintainTarget.vAbsolute.x = infAbsWidth;
            mvFillMaintainTarget.vAbsolute.y = infAbsHeight;
            
            if(GetParentViewPtr() == nullptr)
                return;
            
            Core::Vector2 vParentSize(GetParentViewPtr()->GetAbsoluteSize());
            Core::Vector2 vImageSize(GetSizeFromImage());
            
            f32 fParentRatio = (vParentSize.x * infRelWidth) / (vParentSize.y * infRelHeight);
            f32 fImageRatio = vImageSize.x / vImageSize.y;
            
            if(fImageRatio <= fParentRatio)
            {
                SetWidthMaintainingAspect(infRelWidth, infAbsWidth);
            }
            else
            {
                SetHeightMaintainingAspect(infRelHeight, infAbsHeight);
            }
        }
        //--------------------------------------------------------
        /// Set Fit Maintaining Aspect
        ///
        /// Based on the size of the image and the target size
        /// resize the image view to be the maximum of the target
        /// size while maintaining the aspect ratio
        ///
        /// @param Unified size
        //--------------------------------------------------------
        void HighlightButton::SetFitMaintainingAspect(f32 infRelWidth, f32 infRelHeight, f32 infAbsWidth, f32 infAbsHeight)
        {
            mbFitMaintain = true;
            mvFitMaintainTarget.vRelative.x = infRelWidth;
            mvFitMaintainTarget.vRelative.y = infRelHeight;
            mvFitMaintainTarget.vAbsolute.x = infAbsWidth;
            mvFitMaintainTarget.vAbsolute.y = infAbsHeight;
            
            if(GetParentViewPtr() == nullptr)
                return;
            
            Core::Vector2 vParentSize(GetParentViewPtr()->GetAbsoluteSize());
            Core::Vector2 vImageSize(GetSizeFromImage());
            
            f32 fParentRatio = (vParentSize.x * infRelWidth) / (vParentSize.y * infRelHeight);
            f32 fImageRatio = vImageSize.x / vImageSize.y;
            
            if(fImageRatio > fParentRatio)
            {
                SetWidthMaintainingAspect(infRelWidth, infAbsWidth);
            }
            else
            {
                SetHeightMaintainingAspect(infRelHeight, infAbsHeight);
            }
        }
        //----------------------------------------------------------
        /// Destructor
        //----------------------------------------------------------
        HighlightButton::~HighlightButton()
        {
            if(mpSelectAudioEffect)
            {
                mpSelectAudioEffect->Stop();
            }
			if(mpDeselectAudioEffect)
            {
                mpDeselectAudioEffect->Stop();
            }
        }
    }
}
