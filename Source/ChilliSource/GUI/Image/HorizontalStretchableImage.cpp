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
			: mudwLeftCapIndex(0), mudwCentreIndex(0), mudwRightCapIndex(0), HeightFromImage(false), ActAsSpacer(false)
		{
		}
		//---------------------------------------------------------
		/// Constructor 
		///
		/// @param Dictonary of values
		//---------------------------------------------------------
		HorizontalStretchableImage::HorizontalStretchableImage(const Core::ParamDictionary& insParams)
			: GUIView(insParams), mudwLeftCapIndex(0), mudwCentreIndex(0), mudwRightCapIndex(0), HeightFromImage(false), ActAsSpacer(false)
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
			if(TextureAtlas)
			{
				BaseTextureAtlasID = instrID;

				mudwLeftCapIndex = TextureAtlas->GetFrameIndexById(instrID + "LEFT");
				mudwCentreIndex = TextureAtlas->GetFrameIndexById(instrID + "CENTRE");
				mudwRightCapIndex = TextureAtlas->GetFrameIndexById(instrID + "RIGHT");
			}
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
		/// Left
		/// Centre
		/// Right
		///
		/// @param Array containing the tpage index of each patch
		//---------------------------------------------------------
		void HorizontalStretchableImage::SetTextureAtlasIndices(const u32* inpIndices)
		{
			mudwLeftCapIndex = inpIndices[0];
			mudwCentreIndex = inpIndices[1];
			mudwRightCapIndex = inpIndices[2];
		}
		//---------------------------------------------------------
		/// Set Patch Sprite Sheet Indices
		///
		/// @param Sprite sheet index of left patch
		/// @param Sprite sheet index of centre patch
		/// @param Sprite sheet index of right patch
		//---------------------------------------------------------
		void HorizontalStretchableImage::SetTextureAtlasIndices(u32 inudwLeft, u32 inudwMid, u32 inudwRight)
		{
			mudwLeftCapIndex = inudwLeft;
			mudwCentreIndex = inudwMid;
			mudwRightCapIndex = inudwRight;
		}
		//---------------------------------------------------------
		/// Set Patch Sprite Sheet Left Index
		///
		/// @param tpage index of left patch
		//---------------------------------------------------------
		void HorizontalStretchableImage::SetTextureAtlasLeftIndex(u32 inudwIndex)
		{
			mudwLeftCapIndex = inudwIndex;
		}
		//---------------------------------------------------------
		/// Set Patch Sprite Sheet Centre Index
		///
		/// @param tpage index of centre patch
		//---------------------------------------------------------
		void HorizontalStretchableImage::SetTextureAtlasCentreIndex(u32 inudwIndex)
		{
			mudwCentreIndex = inudwIndex;
		}
		//---------------------------------------------------------
		/// Set Patch Sprite Sheet Right Index
		///
		/// @param tpage index of right patch
		//---------------------------------------------------------
		void HorizontalStretchableImage::SetTextureAtlasRightIndex(u32 inudwIndex)
		{
			mudwRightCapIndex = inudwIndex;
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
			return TextureAtlas->GetFrameSize(mudwLeftCapIndex).x + TextureAtlas->GetFrameSize(mudwRightCapIndex).x;
		}
		//--------------------------------------------------------
		/// Get Combined Cap Height
		///
		/// @param Sum of the heights of both end caps
		//--------------------------------------------------------
		f32 HorizontalStretchableImage::GetCapHeight() const
		{
			return TextureAtlas->GetFrameSize(mudwLeftCapIndex).y;
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
                    
                    // Calculate dimentions and position for middle
                    Core::Vector2 vPatchSize = TextureAtlas->GetFrameSize(mudwCentreIndex);
                    vPatchSize.y = vPanelSize.y;
                    vPatchSize.x = vPanelSize.x - (TextureAtlas->GetFrameSize(mudwLeftCapIndex).x + TextureAtlas->GetFrameSize(mudwRightCapIndex).x);
                    // Record size the caps need to shrink
                    f32 fShrinkX = (vPatchSize.x < 0 ? vPatchSize.x : 0) * 0.5f;
                    vPatchPos.x = vTopLeft.x + TextureAtlas->GetFrameSize(mudwLeftCapIndex).x + fShrinkX;
                    vPatchPos.y = GetAbsoluteAnchorPoint(Rendering::AlignmentAnchor::k_topCentre).y;
                    // Clamp the size of the middle
                    vPatchSize.x = (vPatchSize.x < 0 ? 0 : vPatchSize.x);
                    
                    
                    //Render ourself
                    //Draw the left cap
                    matPatchTransform.Translate(vTopLeft);
                    Core::Matrix3x3::Multiply(&matPatchTransform, &matViewTransform, &matTransform);
                    inpCanvas->DrawBox(matTransform,
                                       Core::Vector2(TextureAtlas->GetFrameSize(mudwLeftCapIndex).x + fShrinkX,vPanelSize.y),
                                       Texture,
                                       TextureAtlas->GetFrameUVs(mudwLeftCapIndex), 
                                       AbsColour, 
                                       Rendering::AlignmentAnchor::k_topLeft);
                    
                    //Draw the right cap
                    matPatchTransform.Translate(GetAbsoluteAnchorPoint(Rendering::AlignmentAnchor::k_topRight));
                    Core::Matrix3x3::Multiply(&matPatchTransform, &matViewTransform, &matTransform);
                    inpCanvas->DrawBox(matTransform, 
                                       Core::Vector2(TextureAtlas->GetFrameSize(mudwRightCapIndex).x + fShrinkX,vPanelSize.y),
                                       Texture,
                                       TextureAtlas->GetFrameUVs(mudwRightCapIndex), 
                                       AbsColour, 
                                       Rendering::AlignmentAnchor::k_topRight);
                    
                    // Draw the centre going from left to right cap
                    matPatchTransform.Translate(vPatchPos);
                    Core::Matrix3x3::Multiply(&matPatchTransform, &matViewTransform, &matTransform);
                    inpCanvas->DrawBox(matTransform,
                                       vPatchSize,
                                       Texture,
                                       TextureAtlas->GetFrameUVs(mudwCentreIndex),
                                       AbsColour,
                                       Rendering::AlignmentAnchor::k_topLeft);
				}
                
				//Render subviews
				GUIView::Draw(inpCanvas);
			}
		}
	}
}