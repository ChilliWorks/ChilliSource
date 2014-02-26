//
//  ToggleButton.cpp
//  moFlo
//
//  Created by Scott Downie on 14/12/2012.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/GUI/Button/ToggleButton.h>
#include <ChilliSource/GUI/Image/ImageView.h>

#include <ChilliSource/Rendering/Sprite/SpriteSheet.h>
#include <ChilliSource/Rendering/Sprite/SpriteSheetManager.h>
#include <ChilliSource/Rendering/Texture/TextureManager.h>
#include <ChilliSource/Rendering/Texture/Texture.h>

#include <ChilliSource/Core/Resource/ResourceManagerDispenser.h>
#include <ChilliSource/Core/Entity/ComponentFactoryDispenser.h>
#include <ChilliSource/Core/String/StringParser.h>

#include <ChilliSource/Audio/3D/AudioComponent.h>
#include <ChilliSource/Audio/3D/AudioComponentFactory.h>

namespace ChilliSource
{
    namespace GUI
    {
        Audio::AudioComponentSPtr ToggleButton::mspDefaultSelectSound;
        Audio::AudioComponentSPtr ToggleButton::mspDefaultDeSelectSound;
        
		DEFINE_META_CLASS(ToggleButton)

		DEFINE_PROPERTY(OnTexture);
		DEFINE_PROPERTY(OffTexture);
		DEFINE_PROPERTY(OnSpriteSheet);
		DEFINE_PROPERTY(OffSpriteSheet);

		DEFINE_PROPERTY(OnSpriteSheetIndex);
		DEFINE_PROPERTY(OffSpriteSheetIndex);
		DEFINE_PROPERTY(OnSpriteSheetIndexID);
		DEFINE_PROPERTY(OffSpriteSheetIndexID);

		DEFINE_PROPERTY(SizeFromImage);
		DEFINE_PROPERTY(HeightMaintain);
		DEFINE_PROPERTY(WidthMaintain);
        //-----------------------------------------------------------
        /// Constructor
        ///
        /// Create the widget by adding the background image
        //-----------------------------------------------------------
        ToggleButton::ToggleButton() 
        : mpBackgroundImage(new ImageView()), OnSpriteSheetIndex(0), OffSpriteSheetIndex(0),
        msOnUVs(Core::CVector2::ZERO, Core::CVector2::ONE),
        msOffUVs(Core::CVector2::ZERO, Core::CVector2::ONE),
        mbSelected(false), SizeFromImage(false), HeightMaintain(false), WidthMaintain(false), mbToggledOn(false)
        {
            mpBackgroundImage->SetSize(Core::UnifiedVector2(Core::CVector2(1.0f, 1.0f), Core::CVector2(0, 0)));
            mpBackgroundImage->SetPosition(Core::UnifiedVector2(Core::CVector2(0.5f, 0.5f), Core::CVector2(0, 0)));
            mpBackgroundImage->EnableUserInteraction(false);
            AddSubview(mpBackgroundImage);
			
            mInputEvents.GetPressedInsideEvent() += GUIEventDelegate(this, &ToggleButton::OnButtonSelect);
            mInputEvents.GetReleasedInsideEvent() += GUIEventDelegate(this, &ToggleButton::OnButtonActivated);
            mInputEvents.GetMovedWithinEvent() += GUIEventDelegate(this, &ToggleButton::OnButtonDeselectThreshold);
        }
        //------------------------------------------------------------
        /// Constructor
        ///
        /// From param dictionary
        //------------------------------------------------------------
        ToggleButton::ToggleButton(const Core::ParamDictionary& insParams) 
        : Button(insParams), mpBackgroundImage(new ImageView()), OnSpriteSheetIndex(0), OffSpriteSheetIndex(0),
        msOnUVs(Core::CVector2::ZERO, Core::CVector2::ONE), msOffUVs(Core::CVector2::ZERO, Core::CVector2::ONE),
        mbSelected(false), SizeFromImage(false), HeightMaintain(false), WidthMaintain(false), mbToggledOn(false)
        {
            std::string strValue;
            
            if(mspDefaultSelectSound)
                SetSelectAudioEffect(mspDefaultSelectSound);
            
            if(mspDefaultDeSelectSound)
                SetDeselectAudioEffect(mspDefaultDeSelectSound);
            
            //---Default
            Core::StorageLocation eOnTextureLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("OnTextureLocation", strValue))
            {
                eOnTextureLocation = ChilliSource::Core::ParseStorageLocation(strValue);
            }
            if(insParams.TryGetValue("OnTexture", strValue))
            {
                SetOnImage(LOAD_RESOURCE(Rendering::ITexture, eOnTextureLocation, strValue));
            }
            //---Off
            Core::StorageLocation eOffTextureLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("OffTextureLocation", strValue))
            {
                eOffTextureLocation = ChilliSource::Core::ParseStorageLocation(strValue);
            }
            if(insParams.TryGetValue("OffTexture", strValue))
            {
                SetOffImage(LOAD_RESOURCE(Rendering::ITexture, eOffTextureLocation, strValue));
            }
            //---Sprite sheet
            Core::StorageLocation eOnSpriteSheetLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("OnSpriteSheetLocation", strValue))
            {
                eOnSpriteSheetLocation = ChilliSource::Core::ParseStorageLocation(strValue);
            }
            if(insParams.TryGetValue("OnSpriteSheet", strValue))
            {
                SetOnSpriteSheet(LOAD_RESOURCE(Rendering::CSpriteSheet, eOnSpriteSheetLocation, strValue));
            }
            //---Sprite sheet
            Core::StorageLocation eOffSpriteSheetLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("OffSpriteSheetLocation", strValue))
            {
                eOffSpriteSheetLocation = ChilliSource::Core::ParseStorageLocation(strValue);
            }
            if(insParams.TryGetValue("OffSpriteSheet", strValue))
            {
                SetOffSpriteSheet(LOAD_RESOURCE(Rendering::CSpriteSheet, eOffSpriteSheetLocation, strValue));
            }
            //---Default index
            if(insParams.TryGetValue("OnSpriteSheetIndex", strValue))
            {
                CS_ASSERT(OnSpriteSheet, "Sprite sheet index cannot be set without sprite sheet");
                SetOnSpriteSheetIndex(Core::ParseU32(strValue));
            }
            //---Off index
            if(insParams.TryGetValue("OffSpriteSheetIndex", strValue))
            {
				CS_ASSERT(OffSpriteSheet, "Sprite sheet index cannot be set without sprite sheet");
				SetOffSpriteSheetIndex(Core::ParseU32(strValue));
            }
			//---Default index ID
			if(insParams.TryGetValue("OnSpriteSheetIndexID", strValue))
			{
				SetOnSpriteSheetIndexID(strValue);
			}
			//---Off index ID
			if(insParams.TryGetValue("OffSpriteSheetIndexID", strValue))
			{
				SetOffSpriteSheetIndexID(strValue);
			}
			//---Size from image
            if(insParams.TryGetValue("SizeFromImage", strValue))
            {
                SizeFromImage = Core::ParseBool(strValue);
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
				Core::CVector2 vSize = Core::ParseVector2(strValue);
				HeightMaintain = true;
				SetHeightMaintainingAspect(vSize.x, vSize.y);
			}
			//---Set Maintain Width
			if(insParams.TryGetValue("SetWidthMaintain", strValue))
			{
				Core::CVector2 vSize = Core::ParseVector2(strValue);
				WidthMaintain = true;
				SetWidthMaintainingAspect(vSize.x, vSize.y);
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
            //---Audio effect
            if(insParams.TryGetValue("SelectAudioEffect", strValue))
            {
                Audio::AudioComponentFactory* pAudioFactory = GET_COMPONENT_FACTORY(Audio::AudioComponentFactory);
                SetSelectAudioEffect(pAudioFactory->CreateAudioComponent(eSelectAudioLocation, strValue, false, false));
            }
			if(insParams.TryGetValue("DeselectAudioEffect", strValue))
            {
                Audio::AudioComponentFactory* pAudioFactory = GET_COMPONENT_FACTORY(Audio::AudioComponentFactory);
                SetDeselectAudioEffect(pAudioFactory->CreateAudioComponent(eSelectAudioLocation, strValue, false, false));
            }
            
            mpBackgroundImage->SetSize(Core::UnifiedVector2(Core::CVector2(1.0f, 1.0f), Core::CVector2(0, 0)));
            mpBackgroundImage->SetPosition(Core::UnifiedVector2(Core::CVector2(0.5f, 0.5f), Core::CVector2(0, 0)));
            mpBackgroundImage->EnableUserInteraction(false);
            AddSubview(mpBackgroundImage);
			
            mInputEvents.GetPressedInsideEvent() += GUIEventDelegate(this, &ToggleButton::OnButtonSelect);
            mInputEvents.GetReleasedInsideEvent() += GUIEventDelegate(this, &ToggleButton::OnButtonActivated);
            mInputEvents.GetMovedWithinEvent() += GUIEventDelegate(this, &ToggleButton::OnButtonDeselectThreshold);
        }
        //-----------------------------------------------------------
        /// Get Background Image View
        ///
        /// @return Image view
        //-----------------------------------------------------------
        const ImageViewSPtr& ToggleButton::GetBackgroundImageView() const
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
        void ToggleButton::SetOnImage(const Rendering::TexturePtr& inpTexture)
        {
            OnTexture = inpTexture;
            
            if(!OffTexture)
            {
                SetOffImage(inpTexture);
            }
        }
		//-----------------------------------------------------------
		/// Get Default Image
		///
		/// @return Texture for non-selected button
		//-----------------------------------------------------------
		const Rendering::TexturePtr& ToggleButton::GetOnImage() const
		{
			return OnTexture;
		}
        //-----------------------------------------------------------
        /// Set Off Image
        ///
        /// Image for selected button
        ///
        /// @param Texture shared pointer
        //-----------------------------------------------------------
        void ToggleButton::SetOffImage(const Rendering::TexturePtr& inpTexture)
        {
            OffTexture = inpTexture;
            mpBackgroundImage->SetTexture(inpTexture);
            mpBackgroundImage->SetUVs(msOffUVs);
            
            if(!OnTexture)
            {
                SetOnImage(inpTexture);
            }
        }
		//-----------------------------------------------------------
		/// Get Off Image
		///
		/// @return Texture for selected button
		//-----------------------------------------------------------
		const Rendering::TexturePtr& ToggleButton::GetOffImage() const
		{
			return OffTexture;
		}
        //-----------------------------------------------------------

        /// Set Default UVs
        ///
        /// Sets the UV coordinates for the default texture
        ///
        /// @param Rectangle for the UVs
        //-----------------------------------------------------------
        void ToggleButton::SetOnUVs(Core::Rectangle insUVs)
        {
            msOnUVs = insUVs;
            
            if(mpBackgroundImage->GetTexture() == OnTexture)
                mpBackgroundImage->SetUVs(msOnUVs);
        }
        //-----------------------------------------------------------
        /// Set Off UVs
        ///
        /// Sets the UV coordinates for the Off texture
        ///
        /// @param Rectangle for the UVs
        //-----------------------------------------------------------
        void ToggleButton::SetOffUVs(Core::Rectangle insUVs)
        {
            msOffUVs = insUVs;
            
            if(mpBackgroundImage->GetTexture() == OffTexture)
                mpBackgroundImage->SetUVs(msOffUVs);
        }

        //-----------------------------------------------------------
        /// Set On Sprite Sheet
        ///
        /// @param Sprite sheet with default image
        //-----------------------------------------------------------
        void ToggleButton::SetOnSpriteSheet(const Rendering::SpriteSheetPtr& inpSpriteSheet)
        {
            OnSpriteSheet = inpSpriteSheet;
            mpBackgroundImage->SetSpriteSheet(inpSpriteSheet);
            
            if(!OffSpriteSheet && OnSpriteSheet)
            {
                SetOffSpriteSheet(inpSpriteSheet);
            }
        }
		//-----------------------------------------------------------
		/// Get On Sprite Sheet
		///
		/// @return Sprite sheet with default image
		//-----------------------------------------------------------
		const Rendering::SpriteSheetPtr& ToggleButton::GetOnSpriteSheet() const
		{ 
			return OnSpriteSheet; 
		}
		//-----------------------------------------------------------
		/// Set Off Sprite Sheet
		///
		/// @param Sprite sheet with Off image
		//-----------------------------------------------------------
		void ToggleButton::SetOffSpriteSheet(const Rendering::SpriteSheetPtr& inpSpriteSheet)
		{
			OffSpriteSheet = inpSpriteSheet;
			mpBackgroundImage->SetSpriteSheet(inpSpriteSheet);
            
            if(!OnSpriteSheet && OffSpriteSheet)
            {
                SetOnSpriteSheet(inpSpriteSheet);
            }
		}
		//-----------------------------------------------------------
		/// Get Off Sprite Sheet
		///
		/// @return Sprite sheet with Off image
		//-----------------------------------------------------------
		const Rendering::SpriteSheetPtr& ToggleButton::GetOffSpriteSheet() const
		{ 
			return OffSpriteSheet; 
		}
        //-----------------------------------------------------------
        /// Set On Sprite Sheet Index
        ///
        /// @param Index of default state on sprite sheet
        //-----------------------------------------------------------
        void ToggleButton::SetOnSpriteSheetIndex(u32 inudwIndex)
        {
            OnSpriteSheetIndex = inudwIndex;
        }
        //-----------------------------------------------------------
        /// Set Off Sprite Sheet Index
        ///
        /// @param Index of Off state on sprite sheet
        //-----------------------------------------------------------
        void ToggleButton::SetOffSpriteSheetIndex(u32 inudwIndex)
        {
            OffSpriteSheetIndex = inudwIndex;
            mpBackgroundImage->SetSpriteSheetIndex(inudwIndex);
        }
		//-----------------------------------------------------------
		/// Get On Sprite Sheet Index
		///
		/// @return Index of default state on sprite sheet
		//-----------------------------------------------------------
		u32 ToggleButton::GetOnSpriteSheetIndex() const
		{
			return OnSpriteSheetIndex;
		}
		//-----------------------------------------------------------
		/// Get Off Sprite Sheet Index
		///
		/// @return Index of Off state on sprite sheet
		//-----------------------------------------------------------
		u32 ToggleButton::GetOffSpriteSheetIndex() const
		{
			return OffSpriteSheetIndex;
		}
		//-----------------------------------------------------------
		/// Set On Sprite Sheet Index
		///
		/// @param Index of default state on sprite sheet
		//-----------------------------------------------------------
		void ToggleButton::SetOnSpriteSheetIndexID(const std::string& instrID)
		{
			CS_ASSERT(OnSpriteSheet, "Cannot set sprite sheet index without setting sprite sheet");
			OnSpriteSheetIndexID = instrID;
			SetOnSpriteSheetIndex(OnSpriteSheet->GetFrameIndexByID(instrID));
		}
		//-----------------------------------------------------------
		/// Set Off Sprite Sheet Index ID
		///
		/// @param Index ID of Off state on sprite sheet
		//-----------------------------------------------------------
		void ToggleButton::SetOffSpriteSheetIndexID(const std::string& instrID)
		{
			CS_ASSERT(OffSpriteSheet, "Cannot set sprite sheet index without setting sprite sheet");
			OffSpriteSheetIndexID = instrID;
			SetOffSpriteSheetIndex(OffSpriteSheet->GetFrameIndexByID(instrID));
		}
		//-----------------------------------------------------------
		/// Get On Sprite Sheet Index ID
		///
		/// @return Index ID of default state on sprite sheet
		//-----------------------------------------------------------
		const std::string& ToggleButton::GetOnSpriteSheetIndexID() const
		{
			return OnSpriteSheetIndexID;
		}
		//-----------------------------------------------------------
		/// Get Off Sprite Sheet Index ID
		///
		/// @return Index ID of Off state on sprite sheet
		//-----------------------------------------------------------
		const std::string& ToggleButton::GetOffSpriteSheetIndexID() const
		{
			return OffSpriteSheetIndexID;
		}
		//--------------------------------------------------------
        /// Enable Size From Image
        ///
        /// When this is enabled the image view's size will be 
        /// based on the size of the image
        ///
        /// @param Enable/disable
        //--------------------------------------------------------
        void ToggleButton::EnableSizeFromImage(bool inbEnable)
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
		bool ToggleButton::IsSizeFromImageEnabled() const
		{
			return SizeFromImage;
		}
        //-----------------------------------------------------------
        /// Set Select Audio Effect
        ///
        /// @param Audio effect played when the button is selected
        //-----------------------------------------------------------
        void ToggleButton::SetSelectAudioEffect(const Audio::AudioComponentSPtr& inpEffect)
        {
            mpSelectAudioEffect = inpEffect;
        }
		//-----------------------------------------------------------
		/// Set De-Select Audio Effect
		///
		/// @param Audio effect played when the button is selected
		//-----------------------------------------------------------
		void ToggleButton::SetDeselectAudioEffect(const Audio::AudioComponentSPtr& inpEffect)
		{
			mpDeselectAudioEffect = inpEffect;
		}
        void ToggleButton::SetDefaultSelectAudioEffect(Core::StorageLocation ineLocation, const std::string instrAudioEffect)
        {
            Audio::AudioComponentFactory* pAudioFactory = GET_COMPONENT_FACTORY(Audio::AudioComponentFactory);
            mspDefaultSelectSound = pAudioFactory->CreateAudioComponent(ineLocation, instrAudioEffect, false, false);
        }
        
        void ToggleButton::SetDefaultDeselectAudioEffect(Core::StorageLocation ineLocation, const std::string instrAudioEffect)
        {
            Audio::AudioComponentFactory* pAudioFactory = GET_COMPONENT_FACTORY(Audio::AudioComponentFactory);
            mspDefaultDeSelectSound = pAudioFactory->CreateAudioComponent(ineLocation, instrAudioEffect, false, false);
        }
        //-----------------------------------------------------------
        /// On Button Select
        //-----------------------------------------------------------
        void ToggleButton::OnButtonSelect(GUIView* inpButton, const Input::TouchInfo & insTouchInfo)
        {
			if(!mbSelected)
			{
				mbSelected = true;
				
                if(mpSelectAudioEffect && !mpSelectAudioEffect->IsPlaying())
                {
                    mpSelectAudioEffect->Play();
                }
			}
        }
        //-----------------------------------------------------------
        /// On Button De-select
        //-----------------------------------------------------------
        void ToggleButton::OnButtonDeselect(GUIView* inpButton, const Input::TouchInfo & insTouchInfo)
        {
			if(mbSelected)
			{
				if(mpDeselectAudioEffect && !mpDeselectAudioEffect->IsPlaying())
				{
					mpDeselectAudioEffect->Play();
				}
				
				mbSelected = false;
			}
        }
        //-----------------------------------------------------------
        /// On Button De-select Threshold
        //-----------------------------------------------------------
        void ToggleButton::OnButtonDeselectThreshold(GUIView* inpButton, const Input::TouchInfo & insTouchInfo)
        {
            const f32 kfThreshold = 5.0f;
			if(mbSelected && (std::abs(insTouchInfo.vLocation.x - insTouchInfo.vPreviousLocation.x) > kfThreshold || std::abs(insTouchInfo.vLocation.y - insTouchInfo.vPreviousLocation.y) > kfThreshold))
			{
				if(mpDeselectAudioEffect && !mpDeselectAudioEffect->IsPlaying())
				{
					mpDeselectAudioEffect->Play();
				}
				
				mbSelected = false;
			}
        }
        //-----------------------------------------------------------
        /// On Button Activated
        //-----------------------------------------------------------
        void ToggleButton::OnButtonActivated(GUIView* inpButton, const Input::TouchInfo & insTouchInfo)
        {
			if(mbSelected)
			{
				OnButtonDeselect(inpButton, insTouchInfo);
                
                mbToggledOn = !mbToggledOn;
				
                if(mbToggledOn)
                {
                    ToggleOn();
                }
                else
                {
                    ToggleOff();
                }
                
				mOnButtonActivatedEvent.Invoke(this);
			}
        }
        //--------------------------------------------------------
        /// Toggle On
        ///
        /// Force the look of the button to be toggled on
        /// this will not generate a button activated event
        ///-------------------------------------------------------
        void ToggleButton::ToggleOn()
        {
            mbToggledOn = true;
            
            if(OnTexture)
            {
                mpBackgroundImage->SetTexture(OnTexture);
                mpBackgroundImage->SetUVs(msOnUVs);
            }
            else if(OnSpriteSheet)
            {
                if (OnSpriteSheet)
                {
                    mpBackgroundImage->SetSpriteSheet(OnSpriteSheet);
                    mpBackgroundImage->SetSpriteSheetIndex(OnSpriteSheetIndex);
                }
            }
        }
        //--------------------------------------------------------
        /// Toggle Off
        ///
        /// Force the look of the button to be toggled on
        /// this will not generate a button activated event
        ///-------------------------------------------------------
        void ToggleButton::ToggleOff()
        {
            mbToggledOn = false;
            
            if(OffTexture)
            {
                mpBackgroundImage->SetTexture(OffTexture);
                mpBackgroundImage->SetUVs(msOffUVs);
            }
            else if(OffSpriteSheet)
            {
                if (OffSpriteSheet)
                {
                    mpBackgroundImage->SetSpriteSheet(OffSpriteSheet);
                    mpBackgroundImage->SetSpriteSheetIndex(OffSpriteSheetIndex);
                }
            }
        }
        //--------------------------------------------------------
        /// Set Toggle
        ///-------------------------------------------------------
        void ToggleButton::SetToggle(bool inbToggle)
        {
            inbToggle ? ToggleOn() : ToggleOff();
        }
        //--------------------------------------------------------
        /// Is Toggled
        ///-------------------------------------------------------
        bool ToggleButton::IsToggled() const
        {
            return mbToggledOn;
        }
		//-----------------------------------------------------------
		/// Draw
		///
		/// Overloaded draw method
		///
		/// @param Canvas renderer
		//-----------------------------------------------------------
		void ToggleButton::Draw(Rendering::CCanvasRenderer* inpCanvasRenderer)
		{
			SetSizeFromImage();
			GUIView::Draw(inpCanvasRenderer);
		}
		
		void ToggleButton::SetSizeFromImage()
		{
			if(SizeFromImage)
			{
				Core::CVector2 vImageSize = mpBackgroundImage->GetSizeFromImage();
				SetSize(0.0f, 0.0f, vImageSize.x, vImageSize.y);
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
        /// Layout Content
        ///
        /// Called when the view is able to retrieve an absolute
        /// value.
        //--------------------------------------------------------
		void ToggleButton::LayoutContent()
		{
            SetSizeFromImage();
            GUIView::LayoutContent();
		}
        
		//--------------------------------------------------------
		/// Get Size From Image
		///
		/// @return Absolute size of the image
		//--------------------------------------------------------
		Core::CVector2 ToggleButton::GetSizeFromImage() const
		{
			Core::CVector2 vAbsSize;
            if(GetBackgroundImageView()->GetTexture())
			{
				return Core::CVector2((f32)GetBackgroundImageView()->GetTexture()->GetWidth(), (f32)GetBackgroundImageView()->GetTexture()->GetHeight());
			}
			else if(GetBackgroundImageView()->GetSpriteSheet())
			{
				return GetBackgroundImageView()->GetSpriteSheet()->GetSizeForFrame(GetBackgroundImageView()->GetSpriteSheetIndex());
			}
			
			return Core::CVector2::ZERO;
		}
        //--------------------------------------------------------
        /// Set Width Maintaining Aspect
        ///
        /// Change the width of the image and resize the height
        /// to maintain the aspect ratio
        ///
        /// @param Unified width
        //--------------------------------------------------------
        void ToggleButton::SetWidthMaintainingAspect(f32 infRelWidth, f32 infAbsWidth)
        {
            WidthMaintain = true;
            
            SetSize(infRelWidth, 0.0f, infAbsWidth, 0.0f);
			
			f32 fScaleY = GetAbsoluteScale().y;
			if(fScaleY == 0.0f)
				return;
            
            Core::CVector2 vCurrentSize = GetSizeFromImage();
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
        void ToggleButton::SetHeightMaintainingAspect(f32 infRelHeight, f32 infAbsHeight)
        {
            HeightMaintain = true;
            
            SetSize(0.0f, infRelHeight, 0.0f, infAbsHeight);
			
			f32 fScaleX = GetAbsoluteScale().x;
			if(fScaleX == 0.0f)
				return;
            
            Core::CVector2 vCurrentSize = GetSizeFromImage();
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
        void ToggleButton::EnableHeightMaintainingAspect(bool inbEnabled)
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
        void ToggleButton::EnableWidthMaintainingAspect(bool inbEnabled)
        {
			WidthMaintain = inbEnabled;
        }
		//--------------------------------------------------------
		/// Is Width Maintaining Aspect Enabled
		///
		/// @return Whether auto scaling of the width to maintain the aspect ratio
		//--------------------------------------------------------
		bool ToggleButton::IsWidthMaintainingAspectEnabled() const
		{
			return WidthMaintain;
		}
		//--------------------------------------------------------
		/// Is Height Maintaining Aspect Enabled
		///
		/// @return Whether auto scaling of the height to maintain the aspect ratio
		//--------------------------------------------------------
		bool ToggleButton::IsHeightMaintainingAspectEnabled() const
		{
			return HeightMaintain;
		}
        //----------------------------------------------------------
        /// Destructor
        //----------------------------------------------------------
        ToggleButton::~ToggleButton()
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
