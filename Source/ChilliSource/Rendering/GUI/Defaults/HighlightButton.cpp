//
//  HighlightButton.cpp
//  moFlo
//
//  Created by Scott Downie on 26/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/Rendering/GUI/Defaults/HighlightButton.h>
#include <ChilliSource/Rendering/GUI/ImageView.h>

#include <ChilliSource/Rendering/Sprite/SpriteSheet.h>
#include <ChilliSource/Rendering/Sprite/SpriteSheetManager.h>
#include <ChilliSource/Rendering/Texture/TextureManager.h>
#include <ChilliSource/Rendering/Texture/Texture.h>

#include <ChilliSource/Core/Resource/ResourceManagerDispenser.h>
#include <ChilliSource/Core/Entity/ComponentFactoryDispenser.h>

#include <ChilliSource/Audio/3D/AudioComponent.h>
#include <ChilliSource/Audio/3D/AudioComponentFactory.h>

namespace moFlo
{
    namespace GUI
    {
        Audio::AudioComponentPtr CHighlightButton::mspDefaultSelectSound;
        Audio::AudioComponentPtr CHighlightButton::mspDefaultDeSelectSound;
        
		DEFINE_META_CLASS(CHighlightButton)

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
        CHighlightButton::CHighlightButton() 
        : mpBackgroundImage(new CImageView()), NormalSpriteSheetIndex(0), HighlightSpriteSheetIndex(0), HighlightColour(0.7f, 0.7f, 0.7f, 1.0f),
        msDefaultUVs(Core::CVector2::ZERO, Core::CVector2::ONE),
        msHighlightUVs(Core::CVector2::ZERO, Core::CVector2::ONE),
        mbSelected(false), SizeFromImage(false), HeightMaintain(false), WidthMaintain(false), WidthFromImage(false), HeightFromImage(false), mbFillMaintain(false), mbFitMaintain(false)
        {
            mpBackgroundImage->SetSize(UnifiedVector2(Core::CVector2(1.0f, 1.0f), Core::CVector2(0, 0)));
            mpBackgroundImage->SetPosition(UnifiedVector2(Core::CVector2(0.5f, 0.5f), Core::CVector2(0, 0)));
            mpBackgroundImage->EnableUserInteraction(false);
            AddSubview(mpBackgroundImage);
			
            mInputEvents.GetPressedInsideEvent() += GUIEventDelegate(this, &CHighlightButton::OnButtonSelect);
            mInputEvents.GetReleasedInsideEvent() += GUIEventDelegate(this, &CHighlightButton::OnButtonActivated);
            mInputEvents.GetMovedWithinEvent() += GUIEventDelegate(this, &CHighlightButton::OnButtonDeselectThreshold);
        }
        //------------------------------------------------------------
        /// Constructor
        ///
        /// From param dictionary
        //------------------------------------------------------------
        CHighlightButton::CHighlightButton(const Core::ParamDictionary& insParams) 
        : IButton(insParams), mpBackgroundImage(new CImageView()), NormalSpriteSheetIndex(0), HighlightSpriteSheetIndex(0),
        msDefaultUVs(Core::CVector2::ZERO, Core::CVector2::ONE),
        msHighlightUVs(Core::CVector2::ZERO, Core::CVector2::ONE),
        mbSelected(false), HighlightColour(0.7f, 0.7f, 0.7f, 1.0f),
		SizeFromImage(false), HeightMaintain(false), WidthMaintain(false), WidthFromImage(false), HeightFromImage(false), mbFillMaintain(false), mbFitMaintain(false)
        {
            std::string strValue;
            
            if(mspDefaultSelectSound)
                SetSelectAudioEffect(mspDefaultSelectSound);
            
            if(mspDefaultDeSelectSound)
                SetDeselectAudioEffect(mspDefaultDeSelectSound);
            
            //---Default
            Core::STORAGE_LOCATION eNormalTextureLocation = Core::SL_PACKAGE;
            if(insParams.TryGetValue("NormalTextureLocation", strValue))
            {
                eNormalTextureLocation = moFlo::Core::CStringConverter::ParseStorageLocation(strValue);
            }
            if(insParams.TryGetValue("NormalTexture", strValue))
            {
                SetNormalImage(LOAD_RESOURCE(Rendering::ITexture, eNormalTextureLocation, strValue));
            }
            //---Highlight
            Core::STORAGE_LOCATION eHighlightTextureLocation = Core::SL_PACKAGE;
            if(insParams.TryGetValue("HighlightTextureLocation", strValue))
            {
                eHighlightTextureLocation = moFlo::Core::CStringConverter::ParseStorageLocation(strValue);
            }
            if(insParams.TryGetValue("HighlightTexture", strValue))
            {
                SetHighlightImage(LOAD_RESOURCE(Rendering::ITexture, eHighlightTextureLocation, strValue));
            }
            //---Sprite sheet
            Core::STORAGE_LOCATION eNormalSpriteSheetLocation = Core::SL_PACKAGE;
            if(insParams.TryGetValue("NormalSpriteSheetLocation", strValue))
            {
                eNormalSpriteSheetLocation = moFlo::Core::CStringConverter::ParseStorageLocation(strValue);
            }
            if(insParams.TryGetValue("NormalSpriteSheet", strValue))
            {
                SetNormalSpriteSheet(LOAD_RESOURCE(Rendering::CSpriteSheet, eNormalSpriteSheetLocation, strValue));
            }
            //---Sprite sheet
            Core::STORAGE_LOCATION eHighlightSpriteSheetLocation = Core::SL_PACKAGE;
            if(insParams.TryGetValue("HighlightSpriteSheetLocation", strValue))
            {
                eHighlightSpriteSheetLocation = moFlo::Core::CStringConverter::ParseStorageLocation(strValue);
            }
            if(insParams.TryGetValue("HighlightSpriteSheet", strValue))
            {
                SetHighlightSpriteSheet(LOAD_RESOURCE(Rendering::CSpriteSheet, eHighlightSpriteSheetLocation, strValue));
            }
            //---Default index
            if(insParams.TryGetValue("NormalSpriteSheetIndex", strValue))
            {
                MOFLOW_ASSERT(NormalSpriteSheet, "Sprite sheet index cannot be set without sprite sheet");
                SetNormalSpriteSheetIndex(Core::CStringConverter::ParseUnsignedInt(strValue));
            }
            //---Highlight index
            if(insParams.TryGetValue("HighlightSpriteSheetIndex", strValue))
            {
				MOFLOW_ASSERT(HighlightSpriteSheet, "Sprite sheet index cannot be set without sprite sheet");
				SetHighlightSpriteSheetIndex(Core::CStringConverter::ParseUnsignedInt(strValue));
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
                SetHighlightColour(Core::CStringConverter::ParseColourValue(strValue));
            }
			//---Size from image
            if(insParams.TryGetValue("SizeFromImage", strValue))
            {
                SizeFromImage = Core::CStringConverter::ParseBool(strValue);
            }
            //---Width from image
            if(insParams.TryGetValue("WidthFromImage", strValue))
            {
                EnableWidthFromImage(Core::CStringConverter::ParseBool(strValue));
            }
            //---Height from image
            if(insParams.TryGetValue("HeightFromImage", strValue))
            {
                EnableHeightFromImage(Core::CStringConverter::ParseBool(strValue));
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
            //---Set Fill Maintain
            if(insParams.TryGetValue("SetFillMaintain", strValue))
            {
                Core::CVector4 vSize = Core::CStringConverter::ParseVector4(strValue);
                SetFillMaintainingAspect(vSize.x, vSize.y, vSize.z, vSize.w);
            }
            //---Set Fit Maintain
            if(insParams.TryGetValue("SetFitMaintain", strValue))
            {
                Core::CVector4 vSize = Core::CStringConverter::ParseVector4(strValue);
                SetFitMaintainingAspect(vSize.x, vSize.y, vSize.z, vSize.w);
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
            if(insParams.TryGetValue("SelectAudioEffect", strValue))
            {
                Audio::IAudioComponentFactory* pAudioFactory = GET_COMPONENT_FACTORY(Audio::IAudioComponentFactory);
                SetSelectAudioEffect(pAudioFactory->CreateAudioComponent(eSelectAudioLocation, strValue, false, false));
            }
			if(insParams.TryGetValue("DeselectAudioEffect", strValue))
            {
                Audio::IAudioComponentFactory* pAudioFactory = GET_COMPONENT_FACTORY(Audio::IAudioComponentFactory);
                SetDeselectAudioEffect(pAudioFactory->CreateAudioComponent(eDeselectAudioLocation, strValue, false, false));
            }
            
            mpBackgroundImage->SetSize(UnifiedVector2(Core::CVector2(1.0f, 1.0f), Core::CVector2(0, 0)));
            mpBackgroundImage->SetPosition(UnifiedVector2(Core::CVector2(0.5f, 0.5f), Core::CVector2(0, 0)));
            mpBackgroundImage->EnableUserInteraction(false);
            AddSubview(mpBackgroundImage);
			
            mInputEvents.GetPressedInsideEvent() += GUIEventDelegate(this, &CHighlightButton::OnButtonSelect);
            mInputEvents.GetReleasedInsideEvent() += GUIEventDelegate(this, &CHighlightButton::OnButtonActivated);
            mInputEvents.GetMovedWithinEvent() += GUIEventDelegate(this, &CHighlightButton::OnButtonDeselectThreshold);
        }
        //-----------------------------------------------------------
        /// Get Background Image View
        ///
        /// @return Image view
        //-----------------------------------------------------------
        const ImageViewPtr& CHighlightButton::GetBackgroundImageView() const
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
        void CHighlightButton::SetNormalImage(const Rendering::TexturePtr& inpTexture)
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
		const Rendering::TexturePtr& CHighlightButton::GetNormalImage() const
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
        void CHighlightButton::SetHighlightImage(const Rendering::TexturePtr& inpTexture)
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
		const Rendering::TexturePtr& CHighlightButton::GetHighlightImage() const
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
        void CHighlightButton::SetDefaultUVs(Core::Rectangle insUVs)
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
        void CHighlightButton::SetHighlightUVs(Core::Rectangle insUVs)
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
        void CHighlightButton::SetNormalSpriteSheet(const Rendering::SpriteSheetPtr& inpSpriteSheet)
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
		const Rendering::SpriteSheetPtr& CHighlightButton::GetNormalSpriteSheet() const 
		{ 
			return NormalSpriteSheet; 
		}
		//-----------------------------------------------------------
		/// Set Highlight Sprite Sheet
		///
		/// @param Sprite sheet with highlight image
		//-----------------------------------------------------------
		void CHighlightButton::SetHighlightSpriteSheet(const Rendering::SpriteSheetPtr& inpSpriteSheet)
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
		const Rendering::SpriteSheetPtr& CHighlightButton::GetHighlightSpriteSheet() const 
		{ 
			return HighlightSpriteSheet; 
		}
        //-----------------------------------------------------------
        /// Set Normal Sprite Sheet Index
        ///
        /// @param Index of default state on sprite sheet
        //-----------------------------------------------------------
        void CHighlightButton::SetNormalSpriteSheetIndex(u32 inudwIndex)
        {
            NormalSpriteSheetIndex = inudwIndex;
            mpBackgroundImage->SetSpriteSheetIndex(inudwIndex);
        }
        //-----------------------------------------------------------
        /// Set Highlight Sprite Sheet Index
        ///
        /// @param Index of highlight state on sprite sheet
        //-----------------------------------------------------------
        void CHighlightButton::SetHighlightSpriteSheetIndex(u32 inudwIndex)
        {
            HighlightSpriteSheetIndex = inudwIndex;
        }
		//-----------------------------------------------------------
		/// Get Normal Sprite Sheet Index
		///
		/// @return Index of default state on sprite sheet
		//-----------------------------------------------------------
		u32 CHighlightButton::GetNormalSpriteSheetIndex() const
		{
			return NormalSpriteSheetIndex;
		}
		//-----------------------------------------------------------
		/// Get Highlight Sprite Sheet Index
		///
		/// @return Index of highlight state on sprite sheet
		//-----------------------------------------------------------
		u32 CHighlightButton::GetHighlightSpriteSheetIndex() const
		{
			return HighlightSpriteSheetIndex;
		}
		//-----------------------------------------------------------
		/// Set Normal Sprite Sheet Index
		///
		/// @param Index of default state on sprite sheet
		//-----------------------------------------------------------
		void CHighlightButton::SetNormalSpriteSheetIndexID(const std::string& instrID)
		{
			MOFLOW_ASSERT(NormalSpriteSheet, "Cannot set sprite sheet index without setting sprite sheet");
			NormalSpriteSheetIndexID = instrID;
			SetNormalSpriteSheetIndex(NormalSpriteSheet->GetFrameIndexByID(instrID));
		}
		//-----------------------------------------------------------
		/// Set Highlight Sprite Sheet Index ID
		///
		/// @param Index ID of highlight state on sprite sheet
		//-----------------------------------------------------------
		void CHighlightButton::SetHighlightSpriteSheetIndexID(const std::string& instrID)
		{
			MOFLOW_ASSERT(HighlightSpriteSheet, "Cannot set sprite sheet index without setting sprite sheet");
			HighlightSpriteSheetIndexID = instrID;
			SetHighlightSpriteSheetIndex(HighlightSpriteSheet->GetFrameIndexByID(instrID));
		}
		//-----------------------------------------------------------
		/// Get Normal Sprite Sheet Index ID
		///
		/// @return Index ID of default state on sprite sheet
		//-----------------------------------------------------------
		const std::string& CHighlightButton::GetNormalSpriteSheetIndexID() const
		{
			return NormalSpriteSheetIndexID;
		}
		//-----------------------------------------------------------
		/// Get Highlight Sprite Sheet Index ID
		///
		/// @return Index ID of highlight state on sprite sheet
		//-----------------------------------------------------------
		const std::string& CHighlightButton::GetHighlightSpriteSheetIndexID() const
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
        void CHighlightButton::EnableSizeFromImage(bool inbEnable)
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
		bool CHighlightButton::IsSizeFromImageEnabled() const
		{
			return SizeFromImage;
		}
		//--------------------------------------------------------
		/// Set Highlight Colour
		///
		/// @param Tint colour to apply when the button is
		/// selected
		//--------------------------------------------------------
		void CHighlightButton::SetHighlightColour(const Core::CColour & inValue)
		{
			HighlightColour = inValue;
		}
		//--------------------------------------------------------
		/// Get Highlight Colour
		///
		/// @return Tint colour to apply when the button is
		/// selected
		//--------------------------------------------------------
		const Core::CColour & CHighlightButton::GetHighlightColour() const
		{
			return HighlightColour;
		}
        //-----------------------------------------------------------
        /// Set Select Audio Effect
        ///
        /// @param Audio effect played when the button is selected
        //-----------------------------------------------------------
        void CHighlightButton::SetSelectAudioEffect(const Audio::AudioComponentPtr& inpEffect)
        {
            mpSelectAudioEffect = inpEffect;
        }
		//-----------------------------------------------------------
		/// Set De-Select Audio Effect
		///
		/// @param Audio effect played when the button is selected
		//-----------------------------------------------------------
		void CHighlightButton::SetDeselectAudioEffect(const Audio::AudioComponentPtr& inpEffect)
		{
			mpDeselectAudioEffect = inpEffect;
		}
        void CHighlightButton::SetDefaultSelectAudioEffect(Core::STORAGE_LOCATION ineLocation, const std::string instrAudioEffect)
        {
            Audio::IAudioComponentFactory* pAudioFactory = GET_COMPONENT_FACTORY(Audio::IAudioComponentFactory);
            mspDefaultSelectSound = pAudioFactory->CreateAudioComponent(ineLocation, instrAudioEffect, false, false);
        }
        
        void CHighlightButton::SetDefaultDeselectAudioEffect(Core::STORAGE_LOCATION ineLocation, const std::string instrAudioEffect)
        {
            Audio::IAudioComponentFactory* pAudioFactory = GET_COMPONENT_FACTORY(Audio::IAudioComponentFactory);
            mspDefaultDeSelectSound = pAudioFactory->CreateAudioComponent(ineLocation, instrAudioEffect, false, false);
        }
        //-----------------------------------------------------------
        /// On Button Select
        //-----------------------------------------------------------
        void CHighlightButton::OnButtonSelect(CGUIView* inpButton, const Input::TouchInfo & insTouchInfo)
        {
			if(!mbSelected)
			{
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
        /// On Button De-select
        //-----------------------------------------------------------
        void CHighlightButton::OnButtonDeselect(CGUIView* inpButton, const Input::TouchInfo & insTouchInfo)
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
                    mpBackgroundImage->SetColour(Core::CColour::WHITE);
				}
				else if(NormalSpriteSheet)
				{
					mpBackgroundImage->SetColour(Core::CColour::WHITE);
					mpBackgroundImage->SetSpriteSheetIndex(NormalSpriteSheetIndex);
					mpBackgroundImage->SetSpriteSheet(NormalSpriteSheet);
				}
				
				mbSelected = false;
			}
        }
        //-----------------------------------------------------------
        /// On Button De-select Threshold
        //-----------------------------------------------------------
        void CHighlightButton::OnButtonDeselectThreshold(CGUIView* inpButton, const Input::TouchInfo & insTouchInfo)
        {
            const f32 kfThreshold = 5.0f;
			if(std::abs(insTouchInfo.vLocation.x - insTouchInfo.vPreviousLocation.x) > kfThreshold || std::abs(insTouchInfo.vLocation.y - insTouchInfo.vPreviousLocation.y) > kfThreshold)
			{
                OnButtonDeselect(inpButton, insTouchInfo);
			}
        }
        //-----------------------------------------------------------
        /// On Button Activated
        //-----------------------------------------------------------
        void CHighlightButton::OnButtonActivated(CGUIView* inpButton, const Input::TouchInfo & insTouchInfo)
        {
			if(mbSelected)
			{
				OnButtonDeselect(inpButton, insTouchInfo);
				mOnButtonActivatedEvent.Invoke(this);
			}
        }
		//-----------------------------------------------------------
		/// Draw
		///
		/// Overloaded draw method
		///
		/// @param Canvas renderer
		//-----------------------------------------------------------
		void CHighlightButton::Draw(Rendering::CCanvasRenderer* inpCanvasRenderer)
		{
			SetSizeFromImage();
			CGUIView::Draw(inpCanvasRenderer);
		}
		
		void CHighlightButton::SetSizeFromImage()
		{
			if(SizeFromImage)
			{
				Core::CVector2 vImageSize = mpBackgroundImage->GetSizeFromImage();
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
        void CHighlightButton::UpdateSizeMaintainingAspect()
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
        /// Enable Width From Image
        ///
        /// When this is enabled the image view's width will be
        /// based on the width of the image
        ///
        /// @param Enable/disable
        //--------------------------------------------------------
        void CHighlightButton::EnableWidthFromImage(bool inbEnable)
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
        bool CHighlightButton::IsWidthFromImageEnabled() const
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
        void CHighlightButton::SetWidthFromImage()
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
        void CHighlightButton::EnableHeightFromImage(bool inbEnable)
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
        bool CHighlightButton::IsHeightFromImageEnabled() const
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
        void CHighlightButton::SetHeightFromImage()
        {
            SetSize(GetSize().GetRelative().x, 0.0f, GetSize().GetAbsolute().x, mpBackgroundImage->GetSizeFromImage().y);
        }
        //--------------------------------------------------------
        /// Layout Content
        ///
        /// Called when the view is able to retrieve an absolute
        /// value.
        //--------------------------------------------------------
		void CHighlightButton::LayoutContent()
		{
            SetSizeFromImage();
            CGUIView::LayoutContent();
		}
        
		//--------------------------------------------------------
		/// Get Size From Image
		///
		/// @return Absolute size of the image
		//--------------------------------------------------------
		Core::CVector2 CHighlightButton::GetSizeFromImage() const
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
        void CHighlightButton::SetWidthMaintainingAspect(f32 infRelWidth, f32 infAbsWidth)
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
        void CHighlightButton::SetHeightMaintainingAspect(f32 infRelHeight, f32 infAbsHeight)
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
        void CHighlightButton::EnableHeightMaintainingAspect(bool inbEnabled)
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
        void CHighlightButton::EnableWidthMaintainingAspect(bool inbEnabled)
        {
			WidthMaintain = inbEnabled;
        }
		//--------------------------------------------------------
		/// Is Width Maintaining Aspect Enabled
		///
		/// @return Whether auto scaling of the width to maintain the aspect ratio
		//--------------------------------------------------------
		bool CHighlightButton::IsWidthMaintainingAspectEnabled() const
		{
			return WidthMaintain;
		}
		//--------------------------------------------------------
		/// Is Height Maintaining Aspect Enabled
		///
		/// @return Whether auto scaling of the height to maintain the aspect ratio
		//--------------------------------------------------------
		bool CHighlightButton::IsHeightMaintainingAspectEnabled() const
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
        void CHighlightButton::SetFillMaintainingAspect(f32 infRelWidth, f32 infRelHeight, f32 infAbsWidth, f32 infAbsHeight)
        {
            mbFillMaintain = true;
            mvFillMaintainTarget.vRelative.x = infRelWidth;
            mvFillMaintainTarget.vRelative.y = infRelHeight;
            mvFillMaintainTarget.vAbsolute.x = infAbsWidth;
            mvFillMaintainTarget.vAbsolute.y = infAbsHeight;
            
            if(GetParentViewPtr() == NULL)
                return;
            
            Core::CVector2 vParentSize(GetParentViewPtr()->GetAbsoluteSize());
            Core::CVector2 vImageSize(GetSizeFromImage());
            
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
        void CHighlightButton::SetFitMaintainingAspect(f32 infRelWidth, f32 infRelHeight, f32 infAbsWidth, f32 infAbsHeight)
        {
            mbFitMaintain = true;
            mvFitMaintainTarget.vRelative.x = infRelWidth;
            mvFitMaintainTarget.vRelative.y = infRelHeight;
            mvFitMaintainTarget.vAbsolute.x = infAbsWidth;
            mvFitMaintainTarget.vAbsolute.y = infAbsHeight;
            
            if(GetParentViewPtr() == NULL)
                return;
            
            Core::CVector2 vParentSize(GetParentViewPtr()->GetAbsoluteSize());
            Core::CVector2 vImageSize(GetSizeFromImage());
            
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
        CHighlightButton::~CHighlightButton()
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
