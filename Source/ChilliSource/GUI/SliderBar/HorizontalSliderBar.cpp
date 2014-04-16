//
//  HorizontalSliderBar.cpp
//  moFloTest
//
//  Created by Scott Downie on 09/06/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/GUI/SliderBar/HorizontalSliderBar.h>
#include <ChilliSource/GUI/Image/ImageView.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/MakeDelegate.h>
#include <ChilliSource/Core/Resource/ResourceManagerDispenser.h>
#include <ChilliSource/Core/Resource/ResourcePool.h>
#include <ChilliSource/Core/String/StringParser.h>

#include <ChilliSource/Rendering/Texture/TextureAtlas.h>
#include <ChilliSource/Rendering/Texture/Texture.h>

namespace ChilliSource
{
    namespace GUI
    {
		DEFINE_META_CLASS(HorizontalSliderBar)
        
		DEFINE_PROPERTY(BarTextureAtlas);
		DEFINE_PROPERTY(SliderTextureAtlas);
		DEFINE_PROPERTY(BarTextureAtlasID);
		DEFINE_PROPERTY(SliderTextureAtlasID);
		DEFINE_PROPERTY(UnifiedSliderSize);
        
        //------------------------------------------------------
        /// Constructor
        ///
        /// Create the subviews that make up the container
        //------------------------------------------------------
        HorizontalSliderBar::HorizontalSliderBar() : mpBackgroundImage(new ImageView()), mpSliderImage(new ImageView()), UnifiedSliderSize(0.15f, 1.5f, 0.0f, 0.0f)
        {
			SetSize(0.8f, 0.1f, 0.0f, 0.0f);
            mpBackgroundImage->SetSize(Core::UnifiedVector2(Core::Vector2(1.0f, 1.0f), Core::Vector2(0, 0)));
            mpBackgroundImage->SetPosition(Core::UnifiedVector2(Core::Vector2(0.5f, 0.5f), Core::Vector2(0, 0)));
            mpBackgroundImage->EnableUserInteraction(false);
            AddSubview(mpBackgroundImage);
            
			mpSliderImage->SetSize(UnifiedSliderSize);
            mpSliderImage->SetSize(Core::UnifiedVector2(Core::Vector2(0.15f, 1.5f), Core::Vector2(0, 0)));
            mpSliderImage->SetPosition(Core::UnifiedVector2(Core::Vector2(0.0f, 0.5f), Core::Vector2(0, 0)));
            mpSliderImage->EnableUserInteraction(false);
            mpBackgroundImage->AddSubview(mpSliderImage);
            
            m_movedWithinConnection = mInputEvents.GetMovedWithinEvent().OpenConnection(Core::MakeDelegate(this, &HorizontalSliderBar::OnSliderMoved));
        }
		//------------------------------------------------------
		/// Constructor
		///
		/// From param dictionary
		//------------------------------------------------------
		HorizontalSliderBar::HorizontalSliderBar(const Core::ParamDictionary& insParams)
        : SliderBar(insParams), mpBackgroundImage(new ImageView()), mpSliderImage(new ImageView()), UnifiedSliderSize(0.15f, 1.5f, 0.0f, 0.0f)
		{
			SetSize(0.8f, 0.1f, 0.0f, 0.0f);
			mpBackgroundImage->SetSize(Core::UnifiedVector2(Core::Vector2(1.0f, 1.0f), Core::Vector2(0, 0)));
			mpBackgroundImage->SetPosition(Core::UnifiedVector2(Core::Vector2(0.5f, 0.5f), Core::Vector2(0, 0)));
			mpBackgroundImage->EnableUserInteraction(false);
			AddSubview(mpBackgroundImage);
            
			mpSliderImage->SetSize(UnifiedSliderSize);
			mpSliderImage->SetPosition(Core::UnifiedVector2(Core::Vector2(0.5f, 0.0f), Core::Vector2(0, 0)));
			mpSliderImage->EnableUserInteraction(false);
			mpSliderImage->EnableAcceptTouchesOutsideOfBounds(true);
			mpBackgroundImage->AddSubview(mpSliderImage);
            
			std::string strValue;
            
            //---Bar Texture
            Core::StorageLocation eBarTextureLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("BarTextureLocation", strValue))
            {
                eBarTextureLocation = Core::ParseStorageLocation(strValue);
            }
            if(insParams.TryGetValue("BarTexture", strValue))
            {
                Core::ResourcePool* resourcePool = Core::Application::Get()->GetResourcePool();
				SetBarTexture(resourcePool->LoadResource<Rendering::Texture>(eBarTextureLocation, strValue));
            }
			//---Slider Texture
            Core::StorageLocation eSliderTextureLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("SliderTextureLocation", strValue))
            {
                eSliderTextureLocation = Core::ParseStorageLocation(strValue);
            }
			if(insParams.TryGetValue("SliderTexture", strValue))
			{
                Core::ResourcePool* resourcePool = Core::Application::Get()->GetResourcePool();
				SetSliderTexture(resourcePool->LoadResource<Rendering::Texture>(eSliderTextureLocation, strValue));
			}
            
			//---Bar Sprite sheet
            Core::StorageLocation eBarTextureAtlasLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("BarTextureAtlasLocation", strValue))
            {
                eBarTextureAtlasLocation = ChilliSource::Core::ParseStorageLocation(strValue);
            }
			if(insParams.TryGetValue("BarTextureAtlas", strValue))
			{
                Core::ResourcePool* resourcePool = Core::Application::Get()->GetResourcePool();
				SetBarTextureAtlas(resourcePool->LoadResource<Rendering::TextureAtlas>(eBarTextureAtlasLocation, strValue));
			}
			//---Slider Sprite sheet
            Core::StorageLocation eSliderTextureAtlasLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("SliderTextureAtlasLocation", strValue))
            {
                eSliderTextureAtlasLocation = ChilliSource::Core::ParseStorageLocation(strValue);
            }
			if(insParams.TryGetValue("SliderTextureAtlas", strValue))
			{
                Core::ResourcePool* resourcePool = Core::Application::Get()->GetResourcePool();
				SetSliderTextureAtlas(resourcePool->LoadResource<Rendering::TextureAtlas>(eSliderTextureAtlasLocation, strValue));
			}
			//---Background ID
			if(insParams.TryGetValue("BarTextureAtlasID", strValue))
			{
				SetBarTextureAtlasID(strValue);
			}
			//---Slider ID
			if(insParams.TryGetValue("SliderTextureAtlasID", strValue))
			{
				SetSliderTextureAtlasID(strValue);
			}
			//---Slider size
			if(insParams.TryGetValue("UnifiedSliderSize", strValue))
			{
				Core::Vector4 vRawSize = Core::ParseVector4(strValue);
				SetSliderSize(Core::UnifiedVector2(vRawSize.x, vRawSize.y, vRawSize.z, vRawSize.w));
			}
            
			m_movedWithinConnection = mInputEvents.GetMovedWithinEvent().OpenConnection(Core::MakeDelegate(this, &HorizontalSliderBar::OnSliderMoved));
		}
        //--------------------------------------------------------
        /// Set Bar Texture
        ///
        /// @param Texture for the bar
        //--------------------------------------------------------
        void HorizontalSliderBar::SetBarTexture(const Rendering::TextureCSPtr& inpTexture)
        {
            BarTexture = inpTexture;
            mpBackgroundImage->SetTexture(inpTexture);
            
            if(!SliderTexture)
            {
                SetSliderTexture(inpTexture);
            }
        }
        //--------------------------------------------------------
        /// Get Bar Texture
        ///
        /// @return Texture for the bar
        //--------------------------------------------------------
        const Rendering::TextureCSPtr& HorizontalSliderBar::GetBarTexture() const
        {
            return BarTexture;
        }
        //--------------------------------------------------------
        /// Set Slider Texture
        ///
        /// @param Texture for the slider
        //--------------------------------------------------------
        void HorizontalSliderBar::SetSliderTexture(const Rendering::TextureCSPtr& inpTexture)
        {
            SliderTexture = inpTexture;
            mpSliderImage->SetTexture(inpTexture);
            
            if(!BarTexture)
            {
                SetBarTexture(inpTexture);
            }
        }
        //--------------------------------------------------------
        /// Get Slider Texture
        ///
        /// @return Texture for the slider
        //--------------------------------------------------------
        const Rendering::TextureCSPtr& HorizontalSliderBar::GetSliderTexture() const
        {
            return SliderTexture;
        }
		//--------------------------------------------------------
		/// Set Bar Sprite Sheet
		///
		/// @param Sprite sheet for the bar
		//--------------------------------------------------------
		void HorizontalSliderBar::SetBarTextureAtlas(const Rendering::TextureAtlasCSPtr& inpTextureAtlas)
		{
			BarTextureAtlas = inpTextureAtlas;
			mpBackgroundImage->SetTextureAtlas(inpTextureAtlas);
            
            if(!SliderTextureAtlas)
            {
                SetSliderTextureAtlas(inpTextureAtlas);
            }
		}
		//--------------------------------------------------------
		/// Get Bar Sprite Sheet
		///
		/// @return Sprite sheet for the bar
		//--------------------------------------------------------
		const Rendering::TextureAtlasCSPtr& HorizontalSliderBar::GetBarTextureAtlas() const
		{
			return BarTextureAtlas;
		}
		//--------------------------------------------------------
		/// Set Slider Sprite Sheet
		///
		/// @param Sprite sheet for the slider
		//--------------------------------------------------------
		void HorizontalSliderBar::SetSliderTextureAtlas(const Rendering::TextureAtlasCSPtr& inpTextureAtlas)
		{
			SliderTextureAtlas = inpTextureAtlas;
			mpSliderImage->SetTextureAtlas(inpTextureAtlas);
            
            if(!BarTextureAtlas)
            {
                SetBarTextureAtlas(inpTextureAtlas);
            }
		}
		//--------------------------------------------------------
		/// Get Slider Sprite Sheet
		///
		/// @return Sprite sheet for the slider
		//--------------------------------------------------------
		const Rendering::TextureAtlasCSPtr& HorizontalSliderBar::GetSliderTextureAtlas() const
		{
			return SliderTextureAtlas;
		}
		//--------------------------------------------------------
		/// Set Bar Sprite Sheet Index ID
		///
		/// @param The index ID of the image within the sprite sheet
		//--------------------------------------------------------
		void HorizontalSliderBar::SetBarTextureAtlasID(const std::string& instrID)
		{
			CS_ASSERT(BarTextureAtlas, "Cannot set sprite sheet index without setting sprite sheet");
			BarTextureAtlasID = instrID;
            mpBackgroundImage->SetTextureAtlasID(instrID);
		}
		//--------------------------------------------------------
		/// Set Slider Sprite Sheet Index ID
		///
		/// @param The index ID of the image within the sprite sheet
		//--------------------------------------------------------
		void HorizontalSliderBar::SetSliderTextureAtlasID(const std::string& instrID)
		{
			CS_ASSERT(SliderTextureAtlas, "Cannot set sprite sheet index without setting sprite sheet");
			SliderTextureAtlasID = instrID;
            mpSliderImage->SetTextureAtlasID(instrID);
		}
		//--------------------------------------------------------
		/// Get Bar Sprite Sheet Index ID
		///
		/// @return The index ID of the image within the sprite sheet
		//--------------------------------------------------------
		const std::string& HorizontalSliderBar::GetBarTextureAtlasID() const
		{
			return BarTextureAtlasID;
		}
		//--------------------------------------------------------
		/// Get Slider Sprite Sheet Index ID
		///
		/// @return The index ID of the image within the sprite sheet
		//--------------------------------------------------------
		const std::string& HorizontalSliderBar::GetSliderTextureAtlasID() const
		{
			return SliderTextureAtlasID;
		}
		//--------------------------------------------------------
		/// Set Slider Size
		///
		/// @param The unified size of the slider
		//--------------------------------------------------------
		void HorizontalSliderBar::SetSliderSize(const Core::UnifiedVector2& invUnifiedSize)
		{
			UnifiedSliderSize = invUnifiedSize;
			mpSliderImage->SetSize(invUnifiedSize);
		}
		//--------------------------------------------------------
		/// Get Slider Size
		///
		/// @return The unified size of the slider
		//--------------------------------------------------------
		const Core::UnifiedVector2& HorizontalSliderBar::GetSliderSize() const
		{
			return UnifiedSliderSize;
		}
        //---Touch Delegates
        //----------------------------------------------------------
        //-----------------------------------------------------------
        void HorizontalSliderBar::OnSliderMoved(GUIView* in_view, const Input::PointerSystem::Pointer& in_pointer)
        {
            mfSliderValue = ((in_pointer.m_location.x - GetAbsoluteScreenSpacePosition().x)/GetAbsoluteSize().x) + 0.5f;
            mpSliderImage->SetPosition(mfSliderValue, 0.5f, 0.0f, 0.0f);
        }
        //------------------------------------------------------
        /// Draw
        ///
        /// Draws a background box with the progress bar
        /// as subview
        ///
        /// @param Canvas renderer pointer
        //------------------------------------------------------
        void HorizontalSliderBar::Draw(Rendering::CanvasRenderer* inpCanvas)
        {
            if(Visible)
            {
                GUIView::Draw(inpCanvas);
            }
        }
    }
}