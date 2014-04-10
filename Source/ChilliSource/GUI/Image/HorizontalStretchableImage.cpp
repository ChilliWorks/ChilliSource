/*
 *  HorizontalStretchableImage.cpp
 *  iOSTemplate
 *
 *  Created by Stuart McGaw on 04/05/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/GUI/Image/HorizontalStretchableImage.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Resource/ResourceManagerDispenser.h>
#include <ChilliSource/Core/Resource/ResourcePool.h>
#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/Rendering/Base/CanvasRenderer.h>
#include <ChilliSource/Rendering/Texture/Texture.h>
#include <ChilliSource/Rendering/Texture/TextureManager.h>
#include <ChilliSource/Rendering/Texture/TextureAtlas.h>

namespace ChilliSource
{
	namespace GUI
	{
		DEFINE_META_CLASS(HorizontalStretchableImage)

		DEFINE_PROPERTY(TextureAtlas);
		DEFINE_PROPERTY(BaseTextureAtlasID);
		DEFINE_PROPERTY(HeightFromImage);
        DEFINE_PROPERTY(ActAsSpacer);
        
		//---------------------------------------------------------
		/// Constructor 
		///
		/// Default
		//---------------------------------------------------------
		HorizontalStretchableImage::HorizontalStretchableImage()
			: HeightFromImage(false), ActAsSpacer(false)
		{
		}
		//---------------------------------------------------------
		/// Constructor 
		///
		/// @param Dictonary of values
		//---------------------------------------------------------
		HorizontalStretchableImage::HorizontalStretchableImage(const Core::ParamDictionary& insParams)
			: GUIView(insParams), HeightFromImage(false), ActAsSpacer(false)
		{
			std::string strValue;

            //---Texture
            Core::StorageLocation eTextureLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("TextureLocation", strValue))
            {
                eTextureLocation = ChilliSource::Core::ParseStorageLocation(strValue);
            }
            if(insParams.TryGetValue("Texture", strValue))
            {
                Texture = LOAD_RESOURCE(Rendering::Texture, eTextureLocation, strValue);
            }
            
            Core::StorageLocation eTextureAtlasLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("TextureAtlasLocation", strValue))
            {
                eTextureAtlasLocation = ChilliSource::Core::ParseStorageLocation(strValue);
            }
            
			if(insParams.TryGetValue("TextureAtlas", strValue))
			{
                Core::ResourcePool* resourcePool = Core::Application::Get()->GetResourcePool();
				SetTextureAtlas(resourcePool->LoadResource<Rendering::TextureAtlas>(eTextureAtlasLocation, strValue));
			}
			
			if(insParams.TryGetValue("BaseTextureAtlasID", strValue))
			{
				SetBaseTextureAtlasID(strValue);
			}
            
            //---Sprite sheet indices
            if(insParams.TryGetValue("LeftID", strValue))
            {
                m_panels.m_leftSize = TextureAtlas->GetFrameSize(strValue);
                m_panels.m_leftUVs = TextureAtlas->GetFrameUVs(strValue);
            }
            if(insParams.TryGetValue("RightID", strValue))
            {
                m_panels.m_rightSize = TextureAtlas->GetFrameSize(strValue);
                m_panels.m_rightUVs = TextureAtlas->GetFrameUVs(strValue);
            }
            if(insParams.TryGetValue("CentreID", strValue))
            {
                m_panels.m_centreSize = TextureAtlas->GetFrameSize(strValue);
                m_panels.m_centreUVs = TextureAtlas->GetFrameUVs(strValue);
            }
			
            //---Height from image
            if(insParams.TryGetValue("HeightFromImage", strValue))
            {
                HeightFromImage = Core::ParseBool(strValue);
            }
            
            if(insParams.TryGetValue("ActAsSpacer", strValue))
            {
                ActAsSpacer = true;
            }
		}
        //---------------------------------------------------------
        /// Set Texture
        ///
        /// @param Texture containing the nine patches
        //---------------------------------------------------------
        void HorizontalStretchableImage::SetTexture(const Rendering::TextureSPtr& inpTexture)
        {
            Texture = inpTexture;
        }
        //---------------------------------------------------------
        /// Get Texture
        ///
        /// @return Texture containing the nine patches
        //---------------------------------------------------------
        const Rendering::TextureSPtr& HorizontalStretchableImage::GetTexture() const
        {
            return Texture;
        }
		//---------------------------------------------------------
		/// Set Sprite Sheet
		///
		/// @param Sprite sheet containing the patches
		//---------------------------------------------------------
		void HorizontalStretchableImage::SetTextureAtlas(const Rendering::TextureAtlasCSPtr& inpTextureAtlas)
		{
			TextureAtlas = inpTextureAtlas;
		}
		//---------------------------------------------------------
		/// Get Sprite Sheet
		///
		/// @return Sprite sheet containing the patches
		//---------------------------------------------------------
		const Rendering::TextureAtlasCSPtr& HorizontalStretchableImage::GetTextureAtlas() const
		{
			return TextureAtlas;
		}
		//---------------------------------------------------------
		/// Set Base Sprite Sheet Index ID
		///
		/// Set the "path" to the sprite sheet index IDs. 
		/// e.g. For the following IDs: 
		///			* "BLUE_PANEL_LEFT"
		///			* "BLUE_PANEL_CENTRE"
		///			* "BLUE_PANEL_RIGHT"
		///
		/// the base ID would be "BLUE_PANEL_"
		//---------------------------------------------------------
		void HorizontalStretchableImage::SetBaseTextureAtlasID(const std::string& instrID)
		{
            CS_ASSERT(TextureAtlas != nullptr, "Must have texture atlas to set IDs");
            
            BaseTextureAtlasID = instrID;
            
            std::string leftId(instrID + "LEFT");
			std::string rightId(instrID + "RIGHT");
			std::string centreId(instrID + "CENTRE");
            
            m_panels.m_leftSize = TextureAtlas->GetFrameSize(leftId);
            m_panels.m_leftUVs = TextureAtlas->GetFrameUVs(leftId);
            
            m_panels.m_rightSize = TextureAtlas->GetFrameSize(rightId);
            m_panels.m_rightUVs = TextureAtlas->GetFrameUVs(rightId);
            
            m_panels.m_centreSize = TextureAtlas->GetFrameSize(centreId);
            m_panels.m_centreUVs = TextureAtlas->GetFrameUVs(centreId);
		}
		//---------------------------------------------------------
		/// Get Base Sprite Sheet Index ID
		///
		/// Get the "path" to the sprite sheet index IDs. 
		/// e.g. For the following IDs: 
		///			* "BLUE_PANEL_LEFT"
		///			* "BLUE_PANEL_CENTRE"
		///			* "BLUE_PANEL_RIGHT"
		///
		/// the base ID would be "BLUE_PANEL_"
		//---------------------------------------------------------
		const std::string& HorizontalStretchableImage::GetBaseTextureAtlasID() const
		{
			return BaseTextureAtlasID;
		}
		//---------------------------------------------------------
		/// Set Patch Sprite Sheet Indices
		///
		/// @param Sprite sheet id of left patch
		/// @param Sprite sheet id of centre patch
		/// @param Sprite sheet id of right patch
		//---------------------------------------------------------
		void HorizontalStretchableImage::SetTextureAtlasIds(const std::string& in_left, const std::string& in_centre, const std::string& in_right)
		{
            CS_ASSERT(TextureAtlas != nullptr, "Must have texture atlas to set IDs");
            
            m_panels.m_leftSize = TextureAtlas->GetFrameSize(in_left);
            m_panels.m_leftUVs = TextureAtlas->GetFrameUVs(in_left);
            
            m_panels.m_rightSize = TextureAtlas->GetFrameSize(in_right);
            m_panels.m_rightUVs = TextureAtlas->GetFrameUVs(in_right);
            
            m_panels.m_centreSize = TextureAtlas->GetFrameSize(in_centre);
            m_panels.m_centreUVs = TextureAtlas->GetFrameUVs(in_centre);
		}
        //--------------------------------------------------------
        /// Enable Height From Image
        ///
        /// When this is enabled the image view's height will be 
        /// based on the size of the image
        ///
        /// @param Enable/disable
        //--------------------------------------------------------
        void HorizontalStretchableImage::EnableHeightFromImage(bool inbEnable)
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
		bool HorizontalStretchableImage::IsHeightFromImageEnabled() const
		{
			return HeightFromImage;
		}
		//--------------------------------------------------------
		/// Get Combined Cap Width
		///
		/// @param Sum of the widths of both end caps
		//--------------------------------------------------------
		f32 HorizontalStretchableImage::GetCombinedCapWidth() const
		{
			return m_panels.m_leftSize.x + m_panels.m_rightSize.x;
		}
		//--------------------------------------------------------
		/// Get Combined Cap Height
		///
		/// @param Sum of the heights of both end caps
		//--------------------------------------------------------
		f32 HorizontalStretchableImage::GetCapHeight() const
		{
			return m_panels.m_leftSize.y;
		}
        //--------------------------------------------------------
        /// Layout Content
        ///
        /// Called when the view is able to retrieve an absolute
        /// value.
        //--------------------------------------------------------
		void HorizontalStretchableImage::LayoutContent()
		{
            SetSizeFromImage();
            GUIView::LayoutContent();
		}
		
		void HorizontalStretchableImage::SetSizeFromImage()
		{
			if(HeightFromImage)
            {
				Core::Vector2 vPanelSize = GetAbsoluteSize();
				vPanelSize.y = GetCapHeight();
				SetSize(0.0f, 0.0f, vPanelSize.x, vPanelSize.y);
            }
		}
		//---------------------------------------------------------
		/// Draw
		/// 
		/// Draw the image constructed from the nine patches
		///
		/// @param Canvas renderer pointer
		//---------------------------------------------------------
		void HorizontalStretchableImage::Draw(Rendering::CanvasRenderer* inpCanvas)
		{
			if(Visible && TextureAtlas && Texture)
			{
				Core::Vector2 vPanelSize = GetAbsoluteSize();
				
				if(HeightFromImage)
				{
					vPanelSize.y = GetCapHeight();
					SetSize(0.0f, 0.0f, vPanelSize.x, vPanelSize.y);
				}
				
                if (ActAsSpacer == false)
                {
                    Core::Vector2 vPanelPos = GetAbsoluteScreenSpacePosition();
                    Core::Vector2 vTopLeft = GetAbsoluteAnchorPoint(Rendering::AlignmentAnchor::k_topLeft);
                    Core::Vector2 vPatchPos;
                    
                    Core::Colour AbsColour = GetAbsoluteColour();
                    
                    //We need to use a matrix so that we can rotate all the patches with respect
                    //to the view
                    Core::Matrix3x3 matTransform;
                    Core::Matrix3x3 matPatchTransform;
                    Core::Matrix3x3 matViewTransform;
                    
                    matViewTransform.SetTransform(vPanelPos, Core::Vector2(1, 1), GetAbsoluteRotation());
                    
                    // Calculate dimentions and position for centre
                    Core::Vector2 vPatchSize = m_panels.m_centreSize;
                    vPatchSize.y = vPanelSize.y;
                    vPatchSize.x = vPanelSize.x - (m_panels.m_leftSize.x + m_panels.m_rightSize.x);
                    // Record size the caps need to shrink
                    f32 fShrinkX = (vPatchSize.x < 0 ? vPatchSize.x : 0) * 0.5f;
                    vPatchPos.x = vTopLeft.x + m_panels.m_leftSize.x + fShrinkX;
                    vPatchPos.y = GetAbsoluteAnchorPoint(Rendering::AlignmentAnchor::k_topCentre).y;
                    // Clamp the size of the centre
                    vPatchSize.x = (vPatchSize.x < 0 ? 0 : vPatchSize.x);
                    
                    
                    //Render ourself
                    //Draw the left cap
                    matPatchTransform.Translate(vTopLeft);
                    Core::Matrix3x3::Multiply(&matPatchTransform, &matViewTransform, &matTransform);
                    inpCanvas->DrawBox(matTransform,
                                       Core::Vector2(m_panels.m_leftSize.x + fShrinkX,vPanelSize.y),
                                       Texture,
                                       m_panels.m_leftUVs,
                                       AbsColour, 
                                       Rendering::AlignmentAnchor::k_topLeft);
                    
                    //Draw the right cap
                    matPatchTransform.Translate(GetAbsoluteAnchorPoint(Rendering::AlignmentAnchor::k_topRight));
                    Core::Matrix3x3::Multiply(&matPatchTransform, &matViewTransform, &matTransform);
                    inpCanvas->DrawBox(matTransform, 
                                       Core::Vector2(m_panels.m_rightSize.x + fShrinkX,vPanelSize.y),
                                       Texture,
                                       m_panels.m_rightUVs,
                                       AbsColour, 
                                       Rendering::AlignmentAnchor::k_topRight);
                    
                    // Draw the centre going from left to right cap
                    matPatchTransform.Translate(vPatchPos);
                    Core::Matrix3x3::Multiply(&matPatchTransform, &matViewTransform, &matTransform);
                    inpCanvas->DrawBox(matTransform,
                                       vPatchSize,
                                       Texture,
                                       m_panels.m_centreUVs,
                                       AbsColour,
                                       Rendering::AlignmentAnchor::k_topLeft);
				}
                
				//Render subviews
				GUIView::Draw(inpCanvas);
			}
		}
	}
}