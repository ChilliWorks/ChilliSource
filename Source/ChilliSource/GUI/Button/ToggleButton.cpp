//
//  ToggleButton.cpp
//  moFlo
//
//  Created by Scott Downie on 14/12/2012.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/GUI/Button/ToggleButton.h>
#include <ChilliSource/GUI/Image/ImageView.h>

#include <ChilliSource/Rendering/Texture/TextureAtlas.h>
#include <ChilliSource/Rendering/Texture/Texture.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/MakeDelegate.h>
#include <ChilliSource/Core/Base/Screen.h>
#include <ChilliSource/Core/Resource/ResourcePool.h>
#include <ChilliSource/Core/String/StringParser.h>

#include <cmath>

namespace ChilliSource
{
    namespace GUI
    {
		DEFINE_META_CLASS(ToggleButton)

		DEFINE_PROPERTY(OnTexture);
		DEFINE_PROPERTY(OffTexture);
		DEFINE_PROPERTY(OnTextureAtlas);
		DEFINE_PROPERTY(OffTextureAtlas);

		DEFINE_PROPERTY(OnTextureAtlasID);
		DEFINE_PROPERTY(OffTextureAtlasID);

		DEFINE_PROPERTY(SizeFromImage);
		DEFINE_PROPERTY(HeightMaintain);
		DEFINE_PROPERTY(WidthMaintain);
        //-----------------------------------------------------------
        /// Constructor
        ///
        /// Create the widget by adding the background image
        //-----------------------------------------------------------
        ToggleButton::ToggleButton() 
        : mpBackgroundImage(new ImageView()),
        msOnUVs(Core::Vector2Old::ZERO, Core::Vector2Old::ONE),
        msOffUVs(Core::Vector2Old::ZERO, Core::Vector2Old::ONE),
        mbSelected(false), SizeFromImage(false), HeightMaintain(false), WidthMaintain(false), mbToggledOn(false)
        {
            mpBackgroundImage->SetSize(Core::UnifiedVector2Old(Core::Vector2Old(1.0f, 1.0f), Core::Vector2Old(0, 0)));
            mpBackgroundImage->SetPosition(Core::UnifiedVector2Old(Core::Vector2Old(0.5f, 0.5f), Core::Vector2Old(0, 0)));
            mpBackgroundImage->EnableUserInteraction(false);
            AddSubview(mpBackgroundImage);
			
            m_pressedInsideConnection = mInputEvents.GetPressedInsideEvent().OpenConnection(Core::MakeDelegate(this, &ToggleButton::OnButtonSelect));
            m_releasedInsideConnection = mInputEvents.GetReleasedInsideEvent().OpenConnection(Core::MakeDelegate(this, &ToggleButton::OnButtonActivated));
            m_movedWithinConnection = mInputEvents.GetMovedWithinEvent().OpenConnection(Core::MakeDelegate(this, &ToggleButton::OnButtonDeselectThreshold));
            m_movedOutsideConnection = mInputEvents.GetMovedOutsideEvent().OpenConnection(Core::MakeDelegate(this, &ToggleButton::OnButtonDeselect));
        }
        //------------------------------------------------------------
        /// Constructor
        ///
        /// From param dictionary
        //------------------------------------------------------------
        ToggleButton::ToggleButton(const Core::ParamDictionary& insParams) 
        : Button(insParams), mpBackgroundImage(new ImageView()),
        msOnUVs(Core::Vector2Old::ZERO, Core::Vector2Old::ONE), msOffUVs(Core::Vector2Old::ZERO, Core::Vector2Old::ONE),
        mbSelected(false), SizeFromImage(false), HeightMaintain(false), WidthMaintain(false), mbToggledOn(false)
        {
            std::string strValue;
            
            //---Default
            Core::StorageLocation eOnTextureLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("OnTextureLocation", strValue))
            {
                eOnTextureLocation = ChilliSource::Core::ParseStorageLocation(strValue);
            }
            if(insParams.TryGetValue("OnTexture", strValue))
            {
                Core::ResourcePool* resourcePool = Core::Application::Get()->GetResourcePool();
                SetOnImage(resourcePool->LoadResource<Rendering::Texture>(eOnTextureLocation, strValue));
            }
            //---Off
            Core::StorageLocation eOffTextureLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("OffTextureLocation", strValue))
            {
                eOffTextureLocation = ChilliSource::Core::ParseStorageLocation(strValue);
            }
            if(insParams.TryGetValue("OffTexture", strValue))
            {
                Core::ResourcePool* resourcePool = Core::Application::Get()->GetResourcePool();
                SetOffImage(resourcePool->LoadResource<Rendering::Texture>(eOffTextureLocation, strValue));
            }
            //---Sprite sheet
            Core::StorageLocation eOnTextureAtlasLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("OnTextureAtlasLocation", strValue))
            {
                eOnTextureAtlasLocation = ChilliSource::Core::ParseStorageLocation(strValue);
            }
            if(insParams.TryGetValue("OnTextureAtlas", strValue))
            {
                Core::ResourcePool* resourcePool = Core::Application::Get()->GetResourcePool();
                SetOnTextureAtlas(resourcePool->LoadResource<Rendering::TextureAtlas>(eOnTextureAtlasLocation, strValue));
            }
            //---Sprite sheet
            Core::StorageLocation eOffTextureAtlasLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("OffTextureAtlasLocation", strValue))
            {
                eOffTextureAtlasLocation = ChilliSource::Core::ParseStorageLocation(strValue);
            }
            if(insParams.TryGetValue("OffTextureAtlas", strValue))
            {
                Core::ResourcePool* resourcePool = Core::Application::Get()->GetResourcePool();
                SetOffTextureAtlas(resourcePool->LoadResource<Rendering::TextureAtlas>(eOffTextureAtlasLocation, strValue));
            }
			//---Default index ID
			if(insParams.TryGetValue("OnTextureAtlasID", strValue))
			{
				SetOnTextureAtlasID(strValue);
			}
			//---Off index ID
			if(insParams.TryGetValue("OffTextureAtlasID", strValue))
			{
				SetOffTextureAtlasID(strValue);
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
				Core::Vector2Old vSize = Core::ParseVector2Old(strValue);
				HeightMaintain = true;
				SetHeightMaintainingAspect(vSize.x, vSize.y);
			}
			//---Set Maintain Width
			if(insParams.TryGetValue("SetWidthMaintain", strValue))
			{
				Core::Vector2Old vSize = Core::ParseVector2Old(strValue);
				WidthMaintain = true;
				SetWidthMaintainingAspect(vSize.x, vSize.y);
			}
            
            mpBackgroundImage->SetSize(Core::UnifiedVector2Old(Core::Vector2Old(1.0f, 1.0f), Core::Vector2Old(0, 0)));
            mpBackgroundImage->SetPosition(Core::UnifiedVector2Old(Core::Vector2Old(0.5f, 0.5f), Core::Vector2Old(0, 0)));
            mpBackgroundImage->EnableUserInteraction(false);
            AddSubview(mpBackgroundImage);
			
            m_pressedInsideConnection = mInputEvents.GetPressedInsideEvent().OpenConnection(Core::MakeDelegate(this, &ToggleButton::OnButtonSelect));
            m_releasedInsideConnection = mInputEvents.GetReleasedInsideEvent().OpenConnection(Core::MakeDelegate(this, &ToggleButton::OnButtonActivated));
            m_movedWithinConnection = mInputEvents.GetMovedWithinEvent().OpenConnection(Core::MakeDelegate(this, &ToggleButton::OnButtonDeselectThreshold));
            m_movedOutsideConnection = mInputEvents.GetMovedOutsideEvent().OpenConnection(Core::MakeDelegate(this, &ToggleButton::OnButtonDeselect));
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
        void ToggleButton::SetOnImage(const Rendering::TextureCSPtr& inpTexture)
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
		const Rendering::TextureCSPtr& ToggleButton::GetOnImage() const
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
        void ToggleButton::SetOffImage(const Rendering::TextureCSPtr& inpTexture)
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
		const Rendering::TextureCSPtr& ToggleButton::GetOffImage() const
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
        void ToggleButton::SetOnTextureAtlas(const Rendering::TextureAtlasCSPtr& inpTextureAtlas)
        {
            OnTextureAtlas = inpTextureAtlas;
            mpBackgroundImage->SetTextureAtlas(inpTextureAtlas);
            
            if(!OffTextureAtlas && OnTextureAtlas)
            {
                SetOffTextureAtlas(inpTextureAtlas);
            }
        }
		//-----------------------------------------------------------
		/// Get On Sprite Sheet
		///
		/// @return Sprite sheet with default image
		//-----------------------------------------------------------
		const Rendering::TextureAtlasCSPtr& ToggleButton::GetOnTextureAtlas() const
		{ 
			return OnTextureAtlas; 
		}
		//-----------------------------------------------------------
		/// Set Off Sprite Sheet
		///
		/// @param Sprite sheet with Off image
		//-----------------------------------------------------------
		void ToggleButton::SetOffTextureAtlas(const Rendering::TextureAtlasCSPtr& inpTextureAtlas)
		{
			OffTextureAtlas = inpTextureAtlas;
			mpBackgroundImage->SetTextureAtlas(inpTextureAtlas);
            
            if(!OnTextureAtlas && OffTextureAtlas)
            {
                SetOnTextureAtlas(inpTextureAtlas);
            }
		}
		//-----------------------------------------------------------
		/// Get Off Sprite Sheet
		///
		/// @return Sprite sheet with Off image
		//-----------------------------------------------------------
		const Rendering::TextureAtlasCSPtr& ToggleButton::GetOffTextureAtlas() const
		{ 
			return OffTextureAtlas; 
		}
		//-----------------------------------------------------------
		/// Set On Sprite Sheet Index
		///
		/// @param Index of default state on sprite sheet
		//-----------------------------------------------------------
		void ToggleButton::SetOnTextureAtlasID(const std::string& instrID)
		{
			OnTextureAtlasID = instrID;
		}
		//-----------------------------------------------------------
		/// Set Off Sprite Sheet Index ID
		///
		/// @param Index ID of Off state on sprite sheet
		//-----------------------------------------------------------
		void ToggleButton::SetOffTextureAtlasID(const std::string& instrID)
		{
			OffTextureAtlasID = instrID;
            mpBackgroundImage->SetTextureAtlasID(instrID);
		}
		//-----------------------------------------------------------
		/// Get On Sprite Sheet Index ID
		///
		/// @return Index ID of default state on sprite sheet
		//-----------------------------------------------------------
		const std::string& ToggleButton::GetOnTextureAtlasID() const
		{
			return OnTextureAtlasID;
		}
		//-----------------------------------------------------------
		/// Get Off Sprite Sheet Index ID
		///
		/// @return Index ID of Off state on sprite sheet
		//-----------------------------------------------------------
		const std::string& ToggleButton::GetOffTextureAtlasID() const
		{
			return OffTextureAtlasID;
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
        //-----------------------------------------------------------
        void ToggleButton::OnButtonSelect(GUIView* in_button, const Input::PointerSystem::Pointer& in_pointer)
        {
			if(!mbSelected)
			{
                mvSelectedPos = in_pointer.m_location;
                
				mbSelected = true;
			}
        }
        //-----------------------------------------------------------
        //-----------------------------------------------------------
        void ToggleButton::OnButtonDeselect(GUIView* in_button, const Input::PointerSystem::Pointer& in_pointer)
        {
			if(mbSelected)
			{
				mbSelected = false;
			}
        }
        //-----------------------------------------------------------
        //-----------------------------------------------------------
        void ToggleButton::OnButtonDeselectThreshold(GUIView* in_button, const Input::PointerSystem::Pointer& in_pointer)
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
        void ToggleButton::OnButtonActivated(GUIView* in_button, const Input::PointerSystem::Pointer& in_pointer)
        {
			if(mbSelected)
			{
				OnButtonDeselect(in_button, in_pointer);
                
                mbToggledOn = !mbToggledOn;
				
                if(mbToggledOn)
                {
                    ToggleOn();
                }
                else
                {
                    ToggleOff();
                }
                
				mOnButtonActivatedEvent.NotifyConnections(this);
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
            
            mpBackgroundImage->SetTexture(OnTexture);
            
            if(OnTextureAtlas)
            {
                mpBackgroundImage->SetTextureAtlas(OnTextureAtlas);
                mpBackgroundImage->SetTextureAtlasID(OnTextureAtlasID);
            }
            else
            {
                mpBackgroundImage->SetUVs(msOnUVs);
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
            
            mpBackgroundImage->SetTexture(OffTexture);
            
            if(OffTextureAtlas)
            {
                mpBackgroundImage->SetTextureAtlas(OffTextureAtlas);
                mpBackgroundImage->SetTextureAtlasID(OffTextureAtlasID);
            }
            else
            {
                mpBackgroundImage->SetUVs(msOffUVs);
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
		void ToggleButton::Draw(Rendering::CanvasRenderer* inpCanvasRenderer)
		{
			SetSizeFromImage();
			GUIView::Draw(inpCanvasRenderer);
		}
		
		void ToggleButton::SetSizeFromImage()
		{
			if(SizeFromImage)
			{
				Core::Vector2Old vImageSize = mpBackgroundImage->GetSizeFromImage();
				SetSize(0.0f, 0.0f, vImageSize.x, vImageSize.y);
			}
			else if(HeightMaintain)
            {
                Core::UnifiedVector2Old uvSize = GetSize();
                SetHeightMaintainingAspect(uvSize.GetRelative().y, uvSize.GetAbsolute().y);
            }
            else if(WidthMaintain)
            {
                Core::UnifiedVector2Old uvSize = GetSize();
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
		Core::Vector2Old ToggleButton::GetSizeFromImage() const
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
        void ToggleButton::SetWidthMaintainingAspect(f32 infRelWidth, f32 infAbsWidth)
        {
            WidthMaintain = true;
            
            SetSize(infRelWidth, 0.0f, infAbsWidth, 0.0f);
			
			f32 fScaleY = GetAbsoluteScale().y;
			if(fScaleY == 0.0f)
				return;
            
            Core::Vector2Old vCurrentSize = GetSizeFromImage();
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
            
            Core::Vector2Old vCurrentSize = GetSizeFromImage();
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

        }
    }
}
