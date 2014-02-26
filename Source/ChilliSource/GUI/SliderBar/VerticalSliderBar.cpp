//
//  VerticalSliderBar.cpp
//  moFloTest
//
//  Created by Scott Downie on 09/06/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/GUI/SliderBar/VerticalSliderBar.h>

#include <ChilliSource/GUI/Image/ImageView.h>
#include <ChilliSource/Core/Resource/ResourceManagerDispenser.h>
#include <ChilliSource/Core/Math/UnifiedCoordinates.h>
#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/Rendering/Sprite/SpriteSheet.h>
#include <ChilliSource/Rendering/Sprite/SpriteSheetManager.h>
#include <ChilliSource/Rendering/Texture/TextureManager.h>
#include <ChilliSource/Rendering/Texture/Texture.h>

namespace ChilliSource
{
    namespace GUI
    {
		DEFINE_META_CLASS(CVerticalSliderBar)

		DEFINE_PROPERTY(BarSpriteSheet);
		DEFINE_PROPERTY(SliderSpriteSheet);
		DEFINE_PROPERTY(BarSpriteSheetIndex);
		DEFINE_PROPERTY(SliderSpriteSheetIndex);
		DEFINE_PROPERTY(BarSpriteSheetIndexID);
		DEFINE_PROPERTY(SliderSpriteSheetIndexID);
		DEFINE_PROPERTY(UnifiedSliderSize);

        //------------------------------------------------------
        /// Constructor
        ///
        /// Create the subviews that make up the container
        //------------------------------------------------------
        CVerticalSliderBar::CVerticalSliderBar() 
		: mpBackgroundImage(new CImageView()), mpSliderImage(new CImageView()), BarSpriteSheetIndex(0), SliderSpriteSheetIndex(0), UnifiedSliderSize(1.5f, 0.15f, 0.0f, 0.0f)
        {
			SetSize(0.1f, 0.8f, 0.0f, 0.0f);
            mpBackgroundImage->SetSize(Core::UnifiedVector2(Core::CVector2(1.0f, 1.0f), Core::CVector2(0, 0)));
            mpBackgroundImage->SetPosition(Core::UnifiedVector2(Core::CVector2(0.5f, 0.5f), Core::CVector2(0, 0)));
            mpBackgroundImage->EnableUserInteraction(false);
            AddSubview(mpBackgroundImage);
            
            mpSliderImage->SetSize(UnifiedSliderSize);
            mpSliderImage->SetPosition(Core::UnifiedVector2(Core::CVector2(0.5f, 0.0f), Core::CVector2(0, 0)));
            mpSliderImage->EnableUserInteraction(false);
            mpBackgroundImage->AddSubview(mpSliderImage);
            
            mInputEvents.GetMovedWithinEvent() += GUIEventDelegate(this, &CVerticalSliderBar::OnSliderMoved);
        }
        //------------------------------------------------------
        /// Constructor
        ///
        /// From param dictionary
        //------------------------------------------------------
        CVerticalSliderBar::CVerticalSliderBar(const Core::ParamDictionary& insParams) 
		: CSliderBar(insParams), mpBackgroundImage(new CImageView()), mpSliderImage(new CImageView()),
			BarSpriteSheetIndex(0), SliderSpriteSheetIndex(0), UnifiedSliderSize(1.5f, 0.15f, 0.0f, 0.0f)
        {
			SetSize(0.1f, 0.8f, 0.0f, 0.0f);
            mpBackgroundImage->SetSize(Core::UnifiedVector2(Core::CVector2(1.0f, 1.0f), Core::CVector2(0, 0)));
            mpBackgroundImage->SetPosition(Core::UnifiedVector2(Core::CVector2(0.5f, 0.5f), Core::CVector2(0, 0)));
            mpBackgroundImage->EnableUserInteraction(false);
            AddSubview(mpBackgroundImage);
            
            mpSliderImage->SetSize(UnifiedSliderSize);
            mpSliderImage->SetPosition(Core::UnifiedVector2(Core::CVector2(0.5f, 0.0f), Core::CVector2(0, 0)));
            mpSliderImage->EnableUserInteraction(false);
			mpSliderImage->EnableAcceptTouchesOutsideOfBounds(true);
            mpBackgroundImage->AddSubview(mpSliderImage);
            
            std::string strValue;
            
            //---Bar Sprite sheet
            Core::StorageLocation eBarSpriteSheetLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("BarSpriteSheetLocation", strValue))
            {
                eBarSpriteSheetLocation = Core::ParseStorageLocation(strValue);
            }
            if(insParams.TryGetValue("BarSpriteSheet", strValue))
            {
                SetBarSpriteSheet(LOAD_RESOURCE(Rendering::CSpriteSheet, eBarSpriteSheetLocation, strValue));
            }
			//---Slider Sprite sheet
            Core::StorageLocation eSliderSpriteSheetLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("SliderSpriteSheetLocation", strValue))
            {
                eSliderSpriteSheetLocation = Core::ParseStorageLocation(strValue);
            }
			if(insParams.TryGetValue("SliderSpriteSheet", strValue))
			{
				SetSliderSpriteSheet(LOAD_RESOURCE(Rendering::CSpriteSheet, eSliderSpriteSheetLocation, strValue));
			}
            //---Background index
            if(insParams.TryGetValue("BarSpriteSheetIndex", strValue))
            {
				MOFLOW_ASSERT(BarSpriteSheet, "Cannot set sprite sheet index without setting sprite sheet");
				SetBarSpriteSheetIndex(Core::ParseU32(strValue));
            }
			//---Slider index
			if(insParams.TryGetValue("SliderSpriteSheetIndex", strValue))
			{
				MOFLOW_ASSERT(SliderSpriteSheet, "Cannot set sprite sheet index without setting sprite sheet");
				SetSliderSpriteSheetIndex(Core::ParseU32(strValue));
			}
			//---Background ID
			if(insParams.TryGetValue("BarSpriteSheetIndexID", strValue))
			{
				SetBarSpriteSheetIndexID(strValue);
			}
			//---Slider ID
			if(insParams.TryGetValue("SliderSpriteSheetIndexID", strValue))
			{
				SetSliderSpriteSheetIndexID(strValue);
			}
            //---Slider size
            if(insParams.TryGetValue("UnifiedSliderSize", strValue))
            {
                Core::CVector4 vRawSize = Core::ParseVector4(strValue);
                SetSliderSize(Core::UnifiedVector2(vRawSize.x, vRawSize.y, vRawSize.z, vRawSize.w));
            }
            
            mInputEvents.GetMovedWithinEvent() += GUIEventDelegate(this, &CVerticalSliderBar::OnSliderMoved);
        }
        //--------------------------------------------------------
        /// Set Bar Sprite Sheet
        ///
        /// @param Sprite sheet for the bar
        //--------------------------------------------------------
        void CVerticalSliderBar::SetBarSpriteSheet(const Rendering::SpriteSheetPtr& inpSpriteSheet)
        {
			BarSpriteSheet = inpSpriteSheet;
            mpBackgroundImage->SetSpriteSheet(inpSpriteSheet);
            
            if(!SliderSpriteSheet)
            {
                SetSliderSpriteSheet(inpSpriteSheet);
            }
        }
		//--------------------------------------------------------
		/// Get Bar Sprite Sheet
		///
		/// @return Sprite sheet for the bar
		//--------------------------------------------------------
		const Rendering::SpriteSheetPtr& CVerticalSliderBar::GetBarSpriteSheet() const
		{
			return BarSpriteSheet;
		}
		//--------------------------------------------------------
		/// Set Slider Sprite Sheet
		///
		/// @param Sprite sheet for the slider
		//--------------------------------------------------------
		void CVerticalSliderBar::SetSliderSpriteSheet(const Rendering::SpriteSheetPtr& inpSpriteSheet)
		{
			SliderSpriteSheet = inpSpriteSheet;
			mpSliderImage->SetSpriteSheet(inpSpriteSheet);
            
            if(!BarSpriteSheet)
            {
                SetBarSpriteSheet(inpSpriteSheet);
            }
		}
		//--------------------------------------------------------
		/// Get Slider Sprite Sheet
		///
		/// @return Sprite sheet for the slider
		//--------------------------------------------------------
		const Rendering::SpriteSheetPtr& CVerticalSliderBar::GetSliderSpriteSheet() const
		{
			return SliderSpriteSheet;
		}
        //--------------------------------------------------------
        /// Set Bar Sprite Sheet Index
        ///
        /// @param The index of the image within the sprite sheet
        //--------------------------------------------------------
        void CVerticalSliderBar::SetBarSpriteSheetIndex(u32 inudwIndex)
        {
			BarSpriteSheetIndex = inudwIndex;
            mpBackgroundImage->SetSpriteSheetIndex(inudwIndex);
        }
        //--------------------------------------------------------
        /// Set Slider Sprite Sheet Index
        ///
        /// @param The index of the image within the sprite sheet
        //--------------------------------------------------------
        void CVerticalSliderBar::SetSliderSpriteSheetIndex(u32 inudwIndex)
        {
			SliderSpriteSheetIndex = inudwIndex;
            mpSliderImage->SetSpriteSheetIndex(inudwIndex);
        }
		//--------------------------------------------------------
		/// Get Bar Sprite Sheet Index
		///
		/// @return The index of the image within the sprite sheet
		//--------------------------------------------------------
		u32 CVerticalSliderBar::GetBarSpriteSheetIndex() const
		{
			return BarSpriteSheetIndex;
		}
		//--------------------------------------------------------
		/// Get Slider Sprite Sheet Index
		///
		/// @return The index of the image within the sprite sheet
		//--------------------------------------------------------
		u32 CVerticalSliderBar::GetSliderSpriteSheetIndex() const
		{
			return SliderSpriteSheetIndex;
		}
		//--------------------------------------------------------
		/// Set Bar Sprite Sheet Index ID
		///
		/// @param The index ID of the image within the sprite sheet
		//--------------------------------------------------------
		void CVerticalSliderBar::SetBarSpriteSheetIndexID(const std::string& instrID)
		{
			MOFLOW_ASSERT(BarSpriteSheet, "Cannot set sprite sheet index without setting sprite sheet");
			BarSpriteSheetIndexID = instrID;
			SetBarSpriteSheetIndex(BarSpriteSheet->GetFrameIndexByID(instrID));
		}
		//--------------------------------------------------------
		/// Set Slider Sprite Sheet Index ID
		///
		/// @param The index ID of the image within the sprite sheet
		//--------------------------------------------------------
		void CVerticalSliderBar::SetSliderSpriteSheetIndexID(const std::string& instrID)
		{
			MOFLOW_ASSERT(SliderSpriteSheet, "Cannot set sprite sheet index without setting sprite sheet");
			SliderSpriteSheetIndexID = instrID;
			SetSliderSpriteSheetIndex(SliderSpriteSheet->GetFrameIndexByID(instrID));
		}
		//--------------------------------------------------------
		/// Get Bar Sprite Sheet Index ID
		///
		/// @return The index ID of the image within the sprite sheet
		//--------------------------------------------------------
		const std::string& CVerticalSliderBar::GetBarSpriteSheetIndexID() const
		{
			return BarSpriteSheetIndexID;
		}
		//--------------------------------------------------------
		/// Get Slider Sprite Sheet Index ID
		///
		/// @return The index ID of the image within the sprite sheet
		//--------------------------------------------------------
		const std::string& CVerticalSliderBar::GetSliderSpriteSheetIndexID() const
		{
			return SliderSpriteSheetIndexID;
		}
        //--------------------------------------------------------
        /// Set Slider Size
        ///
        /// @param The unified size of the slider
        //--------------------------------------------------------
        void CVerticalSliderBar::SetSliderSize(const Core::UnifiedVector2& invUnifiedSize)
        {
			UnifiedSliderSize = invUnifiedSize;
            mpSliderImage->SetSize(invUnifiedSize);
        }
		//--------------------------------------------------------
		/// Get Slider Size
		///
		/// @return The unified size of the slider
		//--------------------------------------------------------
		const Core::UnifiedVector2& CVerticalSliderBar::GetSliderSize() const
		{
			return UnifiedSliderSize;
		}
        //---Touch Delegates
        //-----------------------------------------------------------
        /// On Slider Moved
        ///
        /// Called when the user drags the slider
        ///
        /// @param Touch data
        //-----------------------------------------------------------
        void CVerticalSliderBar::OnSliderMoved(CGUIView* inpView, const Input::TouchInfo & insTouchInfo)
        {
            mfSliderValue = ((insTouchInfo.vLocation.y - GetAbsoluteScreenSpacePosition().y)/GetAbsoluteSize().y) + 0.5f;
            mpSliderImage->SetPosition(0.5f, mfSliderValue, 0.0f, 0.0f);
        }
		//-----------------------------------------------------------
		/// Update Slider Position
		/// 
		/// Reposition the slider image
		//-----------------------------------------------------------
		void CVerticalSliderBar::UpdateSliderPosition()
		{
			mpSliderImage->SetPosition(0.5f, mfSliderValue, 0.0f, 0.0f);
		}
        //------------------------------------------------------
        /// Draw
        ///
        /// Draws a background box with the progress bar
        /// as subview
        ///
        /// @param Canvas renderer pointer
        //------------------------------------------------------
        void CVerticalSliderBar::Draw(Rendering::CCanvasRenderer* inpCanvas)
        {
            if(Visible)
            {
                CGUIView::Draw(inpCanvas);
            }
        }
		//------------------------------------------------------
		/// Destructor
		//------------------------------------------------------
		CVerticalSliderBar::~CVerticalSliderBar()
		{
			mInputEvents.GetMovedWithinEvent() -= GUIEventDelegate(this, &CVerticalSliderBar::OnSliderMoved);
		}
    }
}
