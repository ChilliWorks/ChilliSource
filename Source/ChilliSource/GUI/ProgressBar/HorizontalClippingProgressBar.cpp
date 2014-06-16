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
#include <ChilliSource/Rendering/Texture/Texture.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/String/StringParser.h>
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
		DEFINE_PROPERTY(BackgroundTextureAtlasID);
		DEFINE_PROPERTY(ProgressTextureAtlasID);
		DEFINE_PROPERTY(HeightFromImage);
        
        namespace
        {
            //--------------------------------------------------------
            /// Calculate UVs based on the current progress and UVs
            ///
            /// @param UVs
            /// @param UV offsets
            /// @param Out: UVs
            //--------------------------------------------------------
            void CalculateUVs(const Rendering::UVs& insUVs, const Rendering::UVs& insOffsets, Rendering::UVs& outsUVs)
            {
                f32 leftUV = insUVs.m_u;
                f32 rightUV = insUVs.m_u + insUVs.m_s;
                f32 topUV = insUVs.m_v;
                f32 bottomUV = insUVs.m_v + insUVs.m_t;
                
                f32 leftOffset = insOffsets.m_u;
                f32 rightOffset = insOffsets.m_u + insOffsets.m_s;
                f32 topOffset = insOffsets.m_v;
                f32 bottomOffset = insOffsets.m_v + insOffsets.m_t;
                
                f32 fWidthOfArea = rightUV - leftUV;
                f32 fHeightOfArea = bottomUV - topUV;
                Core::Vector2 vTopLeft = Core::Vector2((fWidthOfArea*leftOffset)+leftUV, (fHeightOfArea*topOffset)+topUV);
                Core::Vector2 vBottomRight = Core::Vector2((fWidthOfArea*rightOffset), (fHeightOfArea*bottomOffset));
                outsUVs = Rendering::UVs(vTopLeft.x, vTopLeft.y, vBottomRight.x, vBottomRight.y);
            }
        }

        //------------------------------------------------------
        /// Constructor
        ///
        /// Create the subviews that make up the container
        //------------------------------------------------------
        HorizontalClippingProgressBar::HorizontalClippingProgressBar() 
			: mpBackgroundImage(new ImageView()), mpProgressImage(new ImageView()), mdwSegments(0), HeightFromImage(false), m_progressUVs(0.0f, 0.0f, 1.0f, 1.0f)
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
			: ProgressBar(insParams), mpBackgroundImage(new ImageView()), mpProgressImage(new ImageView()), mdwSegments(0), HeightFromImage(false), m_progressUVs(0.0f, 0.0f, 1.0f, 1.0f)
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
                eBackgroundTextureLocation = CSCore::ParseStorageLocation(strValue);
            }
            if(insParams.TryGetValue("BackgroundTexture", strValue))
            {
                Core::ResourcePool* resourcePool = Core::Application::Get()->GetResourcePool();
                SetBackgroundImage(resourcePool->LoadResource<Rendering::Texture>(eBackgroundTextureLocation, strValue));
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
                SetProgressImage(resourcePool->LoadResource<Rendering::Texture>(eProgressTextureLocation, strValue));
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
		/// Set Background Sprite Sheet Index ID
		///
		/// @param The index ID of the image within the sprite sheet
		//--------------------------------------------------------
		void HorizontalClippingProgressBar::SetBackgroundTextureAtlasID(const std::string& instrID)
		{
			CS_ASSERT(BackgroundTextureAtlas, "Cannot set sprite sheet index without sprite sheet");
			BackgroundTextureAtlasID = instrID;
            mpBackgroundImage->SetTextureAtlasID(instrID);
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
            mpProgressImage->SetTextureAtlasID(instrID);
            m_progressUVs = mpProgressImage->GetUVs();
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
        //------------------------------------------------------
        /// Set Background Image
        ///
        /// @param Texture representing the background
        //------------------------------------------------------
        void HorizontalClippingProgressBar::SetBackgroundImage(const Rendering::TextureCSPtr& inpTexture)
        {
			BackgroundTexture = inpTexture;
            mpBackgroundImage->SetTexture(inpTexture);
        }
        //------------------------------------------------------
        /// Set Progress Image
        ///
        /// @param Texture representing the bar
        //------------------------------------------------------
        void HorizontalClippingProgressBar::SetProgressImage(const Rendering::TextureCSPtr& inpTexture)
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
		const Rendering::TextureCSPtr& HorizontalClippingProgressBar::GetBackgroundImage() const
		{
			return BackgroundTexture;
		}
		//------------------------------------------------------
		/// Get Progress Image
		///
		/// @return Texture representing the bar
		//------------------------------------------------------
		const Rendering::TextureCSPtr& HorizontalClippingProgressBar::GetProgressImage() const
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
					mpBackgroundImage->SetSize(Core::UnifiedVector2(Core::Vector2::k_zero, vAbsSize));
					
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
                    Rendering::UVs rectProgress;
                    rectProgress.m_u = 0.0f;
                    rectProgress.m_v = 0.0f;
                    rectProgress.m_s = mfVisibleProgress;
                    rectProgress.m_t = 1.0f;
                    
                    Rendering::UVs progressUVs;
                    CalculateUVs(m_progressUVs, rectProgress, progressUVs);
                    mpProgressImage->SetUVs(progressUVs);
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
