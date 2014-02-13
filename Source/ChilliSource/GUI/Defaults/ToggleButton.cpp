//
//  ToggleButton.cpp
//  moFlo
//
//  Created by Scott Downie on 14/12/2012.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/GUI/Defaults/ToggleButton.h>
#include <ChilliSource/GUI/ImageView.h>

#include <ChilliSource/Rendering/SpriteSheet.h>
#include <ChilliSource/Rendering/SpriteSheetManager.h>
#include <ChilliSource/Rendering/TextureManager.h>
#include <ChilliSource/Rendering/Texture.h>

#include <ChilliSource/Core/ResourceManagerDispenser.h>
#include <ChilliSource/Core/ComponentFactoryDispenser.h>

#include <ChilliSource/Audio/AudioComponent.h>
#include <ChilliSource/Audio/AudioComponentFactory.h>

namespace moFlo
{
    namespace GUI
    {
        Audio::AudioComponentPtr CToggleButton::mspDefaultSelectSound;
        Audio::AudioComponentPtr CToggleButton::mspDefaultDeSelectSound;
        
		DEFINE_META_CLASS(CToggleButton)

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
        CToggleButton::CToggleButton() 
        : mpBackgroundImage(new CImageView()), OnSpriteSheetIndex(0), OffSpriteSheetIndex(0),
        msOnUVs(Core::CVector2::ZERO, Core::CVector2::ONE),
        msOffUVs(Core::CVector2::ZERO, Core::CVector2::ONE),
        mbSelected(false), SizeFromImage(false), HeightMaintain(false), WidthMaintain(false), mbToggledOn(false)
        {
            mpBackgroundImage->SetSize(UnifiedVector2(Core::CVector2(1.0f, 1.0f), Core::CVector2(0, 0)));
            mpBackgroundImage->SetPosition(UnifiedVector2(Core::CVector2(0.5f, 0.5f), Core::CVector2(0, 0)));
            mpBackgroundImage->EnableUserInteraction(false);
            AddSubview(mpBackgroundImage);
			
            mInputEvents.GetPressedInsideEvent() += GUIEventDelegate(this, &CToggleButton::OnButtonSelect);
            mInputEvents.GetReleasedInsideEvent() += GUIEventDelegate(this, &CToggleButton::OnButtonActivated);
            mInputEvents.GetMovedWithinEvent() += GUIEventDelegate(this, &CToggleButton::OnButtonDeselectThreshold);
        }
        //------------------------------------------------------------
        /// Constructor
        ///
        /// From param dictionary
        //------------------------------------------------------------
        CToggleButton::CToggleButton(const Core::ParamDictionary& insParams) 
        : IButton(insParams), mpBackgroundImage(new CImageView()), OnSpriteSheetIndex(0), OffSpriteSheetIndex(0),
        msOnUVs(Core::CVector2::ZERO, Core::CVector2::ONE), msOffUVs(Core::CVector2::ZERO, Core::CVector2::ONE),
        mbSelected(false), SizeFromImage(false), HeightMaintain(false), WidthMaintain(false), mbToggledOn(false)
        {
            std::string strValue;
            
            if(mspDefaultSelectSound)
                SetSelectAudioEffect(mspDefaultSelectSound);
            
            if(mspDefaultDeSelectSound)
                SetDeselectAudioEffect(mspDefaultDeSelectSound);
            
            //---Default
            Core::STORAGE_LOCATION eOnTextureLocation = Core::SL_PACKAGE;
            if(insParams.TryGetValue("OnTextureLocation", strValue))
            {
                eOnTextureLocation = moFlo::Core::CStringConverter::ParseStorageLocation(strValue);
            }
            if(insParams.TryGetValue("OnTexture", strValue))
            {
                SetOnImage(LOAD_RESOURCE(Rendering::ITexture, eOnTextureLocation, strValue));
            }
            //---Off
            Core::STORAGE_LOCATION eOffTextureLocation = Core::SL_PACKAGE;
            if(insParams.TryGetValue("OffTextureLocation", strValue))
            {
                eOffTextureLocation = moFlo::Core::CStringConverter::ParseStorageLocation(strValue);
            }
            if(insParams.TryGetValue("OffTexture", strValue))
            {
                SetOffImage(LOAD_RESOURCE(Rendering::ITexture, eOffTextureLocation, strValue));
            }
            //---Sprite sheet
            Core::STORAGE_LOCATION eOnSpriteSheetLocation = Core::SL_PACKAGE;
            if(insParams.TryGetValue("OnSpriteSheetLocation", strValue))
            {
                eOnSpriteSheetLocation = moFlo::Core::CStringConverter::ParseStorageLocation(strValue);
            }
            if(insParams.TryGetValue("OnSpriteSheet", strValue))
            {
                SetOnSpriteSheet(LOAD_RESOURCE(Rendering::CSpriteSheet, eOnSpriteSheetLocation, strValue));
            }
            //---Sprite sheet
            Core::STORAGE_LOCATION eOffSpriteSheetLocation = Core::SL_PACKAGE;
            if(insParams.TryGetValue("OffSpriteSheetLocation", strValue))
            {
                eOffSpriteSheetLocation = moFlo::Core::CStringConverter::ParseStorageLocation(strValue);
            }
            if(insParams.TryGetValue("OffSpriteSheet", strValue))
            {
                SetOffSpriteSheet(LOAD_RESOURCE(Rendering::CSpriteSheet, eOffSpriteSheetLocation, strValue));
            }
            //---Default index
            if(insParams.TryGetValue("OnSpriteSheetIndex", strValue))
            {
                MOFLOW_ASSERT(OnSpriteSheet, "Sprite sheet index cannot be set without sprite sheet");
                SetOnSpriteSheetIndex(Core::CStringConverter::ParseUnsignedInt(strValue));
            }
            //---Off index
            if(insParams.TryGetValue("OffSpriteSheetIndex", strValue))
            {
				MOFLOW_ASSERT(OffSpriteSheet, "Sprite sheet index cannot be set without sprite sheet");
				SetOffSpriteSheetIndex(Core::CStringConverter::ParseUnsignedInt(strValue));
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
                SizeFromImage = Core::CStringConverter::ParseBool(strValue);
            }
			//---Maintain Width
			if(insParams.TryGetValue("HeightMaintain", strValue))
			{
				HeightMaintain = Core::CStringConverter::ParseBool(strValue);
			}
			//---Maintain Height
			if(insParams.TryGetValue("WidthMaintain", strValue))
			{
				WidthMaintain = Core::CStringConverter::ParseBool(strValue);
			}
			//---Set Maintain Height
			if(insParams.TryGetValue("SetHeightMaintain", strValue))
			{
				Core::CVector2 vSize = Core::CStringConverter::ParseVector2(strValue);
				HeightMaintain = true;
				SetHeightMaintainingAspect(vSize.x, vSize.y);
			}
			//---Set Maintain Width
			if(insParams.TryGetValue("SetWidthMaintain", strValue))
			{
				Core::CVector2 vSize = Core::CStringConverter::ParseVector2(strValue);
				WidthMaintain = true;
				SetWidthMaintainingAspect(vSize.x, vSize.y);
			}
            //---Audio effect
            Core::STORAGE_LOCATION eSelectAudioLocation = Core::SL_PACKAGE;
            Core::STORAGE_LOCATION eDeselectAudioLocation = Core::SL_PACKAGE;
            if(insParams.TryGetValue("SelectAudioEffectLocation", strValue))
            {
                eSelectAudioLocation = moFlo::Core::CStringConverter::ParseStorageLocation(strValue);
            }
			if(insParams.TryGetValue("DeselectAudioEffectLocation", strValue))
            {
                eDeselectAudioLocation = moFlo::Core::CStringConverter::ParseStorageLocation(strValue);
            }
            //---Audio effect
            if(insParams.TryGetValue("SelectAudioEffect", strValue))
            {
                Audio::IAudioComponentFactory* pAudioFactory = GET_COMPONENT_FACTORY(Audio::IAudioComponentFactory);
                SetSelectAudioEffect(pAudioFactory->CreateAudioComponent(eSelectAudioLocation, strValue, false, false));
            }
			if(insParams.TryGetValue("DeselectAudioEffect", strValue))
            {
                Audio::IAudioComponentFactory* pAudioFactory = GET_COMPONENT_FACTORY(Audio::IAudioComponentFactory);
                SetDeselectAudioEffect(pAudioFactory->CreateAudioComponent(eSelectAudioLocation, strValue, false, false));
            }
            
            mpBackgroundImage->SetSize(UnifiedVector2(Core::CVector2(1.0f, 1.0f), Core::CVector2(0, 0)));
            mpBackgroundImage->SetPosition(UnifiedVector2(Core::CVector2(0.5f, 0.5f), Core::CVector2(0, 0)));
            mpBackgroundImage->EnableUserInteraction(false);
            AddSubview(mpBackgroundImage);
			
            mInputEvents.GetPressedInsideEvent() += GUIEventDelegate(this, &CToggleButton::OnButtonSelect);
            mInputEvents.GetReleasedInsideEvent() += GUIEventDelegate(this, &CToggleButton::OnButtonActivated);
            mInputEvents.GetMovedWithinEvent() += GUIEventDelegate(this, &CToggleButton::OnButtonDeselectThreshold);
        }
        //-----------------------------------------------------------
        /// Get Background Image View
        ///
        /// @return Image view
        //-----------------------------------------------------------
        const ImageViewPtr& CToggleButton::GetBackgroundImageView() const
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
        void CToggleButton::SetOnImage(const Rendering::TexturePtr& inpTexture)
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
		const Rendering::TexturePtr& CToggleButton::GetOnImage() const
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
        void CToggleButton::SetOffImage(const Rendering::TexturePtr& inpTexture)
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
		const Rendering::TexturePtr& CToggleButton::GetOffImage() const
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
        void CToggleButton::SetOnUVs(Core::Rectangle insUVs)
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
        void CToggleButton::SetOffUVs(Core::Rectangle insUVs)
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
        void CToggleButton::SetOnSpriteSheet(const Rendering::SpriteSheetPtr& inpSpriteSheet)
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
		const Rendering::SpriteSheetPtr& CToggleButton::GetOnSpriteSheet() const 
		{ 
			return OnSpriteSheet; 
		}
		//-----------------------------------------------------------
		/// Set Off Sprite Sheet
		///
		/// @param Sprite sheet with Off image
		//-----------------------------------------------------------
		void CToggleButton::SetOffSpriteSheet(const Rendering::SpriteSheetPtr& inpSpriteSheet)
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
		const Rendering::SpriteSheetPtr& CToggleButton::GetOffSpriteSheet() const 
		{ 
			return OffSpriteSheet; 
		}
        //-----------------------------------------------------------
        /// Set On Sprite Sheet Index
        ///
        /// @param Index of default state on sprite sheet
        //-----------------------------------------------------------
        void CToggleButton::SetOnSpriteSheetIndex(u32 inudwIndex)
        {
            OnSpriteSheetIndex = inudwIndex;
        }
        //-----------------------------------------------------------
        /// Set Off Sprite Sheet Index
        ///
        /// @param Index of Off state on sprite sheet
        //-----------------------------------------------------------
        void CToggleButton::SetOffSpriteSheetIndex(u32 inudwIndex)
        {
            OffSpriteSheetIndex = inudwIndex;
            mpBackgroundImage->SetSpriteSheetIndex(inudwIndex);
        }
		//-----------------------------------------------------------
		/// Get On Sprite Sheet Index
		///
		/// @return Index of default state on sprite sheet
		//-----------------------------------------------------------
		u32 CToggleButton::GetOnSpriteSheetIndex() const
		{
			return OnSpriteSheetIndex;
		}
		//-----------------------------------------------------------
		/// Get Off Sprite Sheet Index
		///
		/// @return Index of Off state on sprite sheet
		//-----------------------------------------------------------
		u32 CToggleButton::GetOffSpriteSheetIndex() const
		{
			return OffSpriteSheetIndex;
		}
		//-----------------------------------------------------------
		/// Set On Sprite Sheet Index
		///
		/// @param Index of default state on sprite sheet
		//-----------------------------------------------------------
		void CToggleButton::SetOnSpriteSheetIndexID(const std::string& instrID)
		{
			MOFLOW_ASSERT(OnSpriteSheet, "Cannot set sprite sheet index without setting sprite sheet");
			OnSpriteSheetIndexID = instrID;
			SetOnSpriteSheetIndex(OnSpriteSheet->GetFrameIndexByID(instrID));
		}
		//-----------------------------------------------------------
		/// Set Off Sprite Sheet Index ID
		///
		/// @param Index ID of Off state on sprite sheet
		//-----------------------------------------------------------
		void CToggleButton::SetOffSpriteSheetIndexID(const std::string& instrID)
		{
			MOFLOW_ASSERT(OffSpriteSheet, "Cannot set sprite sheet index without setting sprite sheet");
			OffSpriteSheetIndexID = instrID;
			SetOffSpriteSheetIndex(OffSpriteSheet->GetFrameIndexByID(instrID));
		}
		//-----------------------------------------------------------
		/// Get On Sprite Sheet Index ID
		///
		/// @return Index ID of default state on sprite sheet
		//-----------------------------------------------------------
		const std::string& CToggleButton::GetOnSpriteSheetIndexID() const
		{
			return OnSpriteSheetIndexID;
		}
		//-----------------------------------------------------------
		/// Get Off Sprite Sheet Index ID
		///
		/// @return Index ID of Off state on sprite sheet
		//-----------------------------------------------------------
		const std::string& CToggleButton::GetOffSpriteSheetIndexID() const
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
        void CToggleButton::EnableSizeFromImage(bool inbEnable)
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
		bool CToggleButton::IsSizeFromImageEnabled() const
		{
			return SizeFromImage;
		}
        //-----------------------------------------------------------
        /// Set Select Audio Effect
        ///
        /// @param Audio effect played when the button is selected
        //-----------------------------------------------------------
        void CToggleButton::SetSelectAudioEffect(const Audio::AudioComponentPtr& inpEffect)
        {
            mpSelectAudioEffect = inpEffect;
        }
		//-----------------------------------------------------------
		/// Set De-Select Audio Effect
		///
		/// @param Audio effect played when the button is selected
		//-----------------------------------------------------------
		void CToggleButton::SetDeselectAudioEffect(const Audio::AudioComponentPtr& inpEffect)
		{
			mpDeselectAudioEffect = inpEffect;
		}
        void CToggleButton::SetDefaultSelectAudioEffect(Core::STORAGE_LOCATION ineLocation, const std::string instrAudioEffect)
        {
            Audio::IAudioComponentFactory* pAudioFactory = GET_COMPONENT_FACTORY(Audio::IAudioComponentFactory);
            mspDefaultSelectSound = pAudioFactory->CreateAudioComponent(ineLocation, instrAudioEffect, false, false);
        }
        
        void CToggleButton::SetDefaultDeselectAudioEffect(Core::STORAGE_LOCATION ineLocation, const std::string instrAudioEffect)
        {
            Audio::IAudioComponentFactory* pAudioFactory = GET_COMPONENT_FACTORY(Audio::IAudioComponentFactory);
            mspDefaultDeSelectSound = pAudioFactory->CreateAudioComponent(ineLocation, instrAudioEffect, false, false);
        }
        //-----------------------------------------------------------
        /// On Button Select
        //-----------------------------------------------------------
        void CToggleButton::OnButtonSelect(CGUIView* inpButton, const Input::TouchInfo & insTouchInfo)
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
        void CToggleButton::OnButtonDeselect(CGUIView* inpButton, const Input::TouchInfo & insTouchInfo)
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
        void CToggleButton::OnButtonDeselectThreshold(CGUIView* inpButton, const Input::TouchInfo & insTouchInfo)
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
        void CToggleButton::OnButtonActivated(CGUIView* inpButton, const Input::TouchInfo & insTouchInfo)
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
        void CToggleButton::ToggleOn()
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
        void CToggleButton::ToggleOff()
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
        void CToggleButton::SetToggle(bool inbToggle)
        {
            inbToggle ? ToggleOn() : ToggleOff();
        }
        //--------------------------------------------------------
        /// Is Toggled
        ///-------------------------------------------------------
        bool CToggleButton::IsToggled() const
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
		void CToggleButton::Draw(Rendering::CCanvasRenderer* inpCanvasRenderer)
		{
			SetSizeFromImage();
			CGUIView::Draw(inpCanvasRenderer);
		}
		
		void CToggleButton::SetSizeFromImage()
		{
			if(SizeFromImage)
			{
				Core::CVector2 vImageSize = mpBackgroundImage->GetSizeFromImage();
				SetSize(0.0f, 0.0f, vImageSize.x, vImageSize.y);
			}
			else if(HeightMaintain)
            {
                UnifiedVector2 uvSize = GetSize();
                SetHeightMaintainingAspect(uvSize.GetRelative().y, uvSize.GetAbsolute().y);
            }
            else if(WidthMaintain)
            {
                UnifiedVector2 uvSize = GetSize();
                SetWidthMaintainingAspect(uvSize.GetRelative().x, uvSize.GetAbsolute().x);
            }
		}
		
        //--------------------------------------------------------
        /// Layout Content
        ///
        /// Called when the view is able to retrieve an absolute
        /// value.
        //--------------------------------------------------------
		void CToggleButton::LayoutContent()
		{
            SetSizeFromImage();
            CGUIView::LayoutContent();
		}
        
		//--------------------------------------------------------
		/// Get Size From Image
		///
		/// @return Absolute size of the image
		//--------------------------------------------------------
		Core::CVector2 CToggleButton::GetSizeFromImage() const
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
        void CToggleButton::SetWidthMaintainingAspect(f32 infRelWidth, f32 infAbsWidth)
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
        void CToggleButton::SetHeightMaintainingAspect(f32 infRelHeight, f32 infAbsHeight)
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
        void CToggleButton::EnableHeightMaintainingAspect(bool inbEnabled)
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
        void CToggleButton::EnableWidthMaintainingAspect(bool inbEnabled)
        {
			WidthMaintain = inbEnabled;
        }
		//--------------------------------------------------------
		/// Is Width Maintaining Aspect Enabled
		///
		/// @return Whether auto scaling of the width to maintain the aspect ratio
		//--------------------------------------------------------
		bool CToggleButton::IsWidthMaintainingAspectEnabled() const
		{
			return WidthMaintain;
		}
		//--------------------------------------------------------
		/// Is Height Maintaining Aspect Enabled
		///
		/// @return Whether auto scaling of the height to maintain the aspect ratio
		//--------------------------------------------------------
		bool CToggleButton::IsHeightMaintainingAspectEnabled() const
		{
			return HeightMaintain;
		}
        //----------------------------------------------------------
        /// Destructor
        //----------------------------------------------------------
        CToggleButton::~CToggleButton()
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
