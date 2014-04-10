//
//  StretchableImage.cpp
//  moFlo
//
//  Created by Scott Downie on 28/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/GUI/Image/VerticalStretchableImage.h>

#include <ChilliSource/Rendering/Texture/Texture.h>
#include <ChilliSource/Rendering/Texture/TextureManager.h>
#include <ChilliSource/Rendering/Texture/TextureAtlas.h>
#include <ChilliSource/Rendering/Base/CanvasRenderer.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Resource/ResourcePool.h>
#include <ChilliSource/Core/Resource/ResourceManagerDispenser.h>
#include <ChilliSource/Core/Base/Screen.h>
#include <ChilliSource/Core/String/StringParser.h>

namespace ChilliSource
{
    namespace GUI
    {
		DEFINE_META_CLASS(VerticalStretchableImage)

		DEFINE_PROPERTY(TextureAtlas);
		DEFINE_PROPERTY(HeightMaintain);
		DEFINE_PROPERTY(WidthMaintain);
		DEFINE_PROPERTY(BaseTextureAtlasID);

		//--------------------------------------------------------
        /// Constructor
        /// 
        /// Empty
        //---------------------------------------------------------
        VerticalStretchableImage::VerticalStretchableImage()
			: HeightMaintain(false), WidthMaintain(false)
        {
        }
        //---------------------------------------------------------
        /// Constructor
        ///
        /// From param dictionary
        //---------------------------------------------------------
        VerticalStretchableImage::VerticalStretchableImage(const Core::ParamDictionary& insParams) 
			: GUIView(insParams), HeightMaintain(false), WidthMaintain(false)
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
            
            //---Sprite sheet
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

			//---Sprite sheet base name
			if(insParams.TryGetValue("BaseTextureAtlasID", strValue))
            {
				SetBaseTextureAtlasID(strValue);
			}

            //---Sprite sheet indices
            if(insParams.TryGetValue("TopID", strValue))
            {
                m_panels.m_topSize = TextureAtlas->GetFrameSize(strValue);
                m_panels.m_topUVs = TextureAtlas->GetFrameUVs(strValue);
            }
            if(insParams.TryGetValue("BottomID", strValue))
            {
                m_panels.m_bottomSize = TextureAtlas->GetFrameSize(strValue);
                m_panels.m_bottomUVs = TextureAtlas->GetFrameUVs(strValue);
            }
            if(insParams.TryGetValue("MiddleID", strValue))
            {
                m_panels.m_middleSize = TextureAtlas->GetFrameSize(strValue);
                m_panels.m_middleUVs = TextureAtlas->GetFrameUVs(strValue);
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
			//---Set Maintain Width
			if(insParams.TryGetValue("SetHeightMaintain", strValue))
			{
				Core::Vector2 vSize = Core::ParseVector2(strValue);
				SetHeightMaintainingAspect(vSize.x, vSize.y);
			}
			//---Set Maintain Height
			if(insParams.TryGetValue("SetWidthMaintain", strValue))
			{
				Core::Vector2 vSize = Core::ParseVector2(strValue);
				SetWidthMaintainingAspect(vSize.x, vSize.y);
			}
        }
        //---------------------------------------------------------
        /// Set Texture
        ///
        /// @param Texture containing the nine patches
        //---------------------------------------------------------
        void VerticalStretchableImage::SetTexture(const Rendering::TextureSPtr& inpTexture)
        {
            Texture = inpTexture;
        }
        //---------------------------------------------------------
        /// Get Texture
        ///
        /// @return Texture containing the nine patches
        //---------------------------------------------------------
        const Rendering::TextureSPtr& VerticalStretchableImage::GetTexture() const
        {
            return Texture;
        }
        //---------------------------------------------------------
        /// Set Sprite Sheet
        ///
        /// @param Sprite sheet containing the nine patches
        //---------------------------------------------------------
        void VerticalStretchableImage::SetTextureAtlas(const Rendering::TextureAtlasCSPtr& inpTextureAtlas)
        {
            TextureAtlas = inpTextureAtlas;
        }
		//---------------------------------------------------------
		/// Get Sprite Sheet
		///
		/// @return Sprite sheet containing the nine patches
		//---------------------------------------------------------
		const Rendering::TextureAtlasCSPtr& VerticalStretchableImage::GetTextureAtlas() const
		{
			return TextureAtlas;
		}
		//---------------------------------------------------------
		/// Set Base Sprite Sheet Index ID
		///
		/// Set the "path" to the sprite sheet index IDs. 
		/// e.g. For the following IDs: 
		///			* "BLUE_PANEL_TOP_LEFT"
		///			* "BLUE_PANEL_TOP_RIGHT"
		///			* "BLUE_PANEL_BOTTOM_LEFT"
		///			* "BLUE_PANEL_BOTTOM_RIGHT"
		///			* "BLUE_PANEL_TOP_CENTRE"
		///			* "BLUE_PANEL_BOTTOM_CENTRE"
		///			* "BLUE_PANEL_MIDDLE_CENTRE"
		///			* "BLUE_PANEL_MIDDLE_RIGHT"
		///			* "BLUE_PANEL_MIDDLE_LEFT"
		///
		/// the base ID would be "BLUE_PANEL_"
		//---------------------------------------------------------
		void VerticalStretchableImage::SetBaseTextureAtlasID(const std::string& instrID)
		{
            CS_ASSERT(TextureAtlas != nullptr, "Must have texture atlas to set IDs");
            
            BaseTextureAtlasID = instrID;

            std::string topId(instrID + "TOP");
			std::string bottomId(instrID + "BOTTOM");
			std::string middleId(instrID + "MIDDLE");
                
            m_panels.m_topSize = TextureAtlas->GetFrameSize(topId);
            m_panels.m_topUVs = TextureAtlas->GetFrameUVs(topId);

            m_panels.m_bottomSize = TextureAtlas->GetFrameSize(bottomId);
            m_panels.m_bottomUVs = TextureAtlas->GetFrameUVs(bottomId);

            m_panels.m_middleSize = TextureAtlas->GetFrameSize(middleId);
            m_panels.m_middleUVs = TextureAtlas->GetFrameUVs(middleId);
		}
		//---------------------------------------------------------
		/// Get Base Sprite Sheet Index ID
		///
		/// Get the "path" to the sprite sheet index IDs. 
		/// e.g. For the following IDs: 
		///			* "BLUE_PANEL_TOP_LEFT"
		///			* "BLUE_PANEL_TOP_RIGHT"
		///			* "BLUE_PANEL_BOTTOM_LEFT"
		///			* "BLUE_PANEL_BOTTOM_RIGHT"
		///			* "BLUE_PANEL_TOP_CENTRE"
		///			* "BLUE_PANEL_BOTTOM_CENTRE"
		///			* "BLUE_PANEL_MIDDLE_CENTRE"
		///			* "BLUE_PANEL_MIDDLE_RIGHT"
		///			* "BLUE_PANEL_MIDDLE_LEFT"
		///
		/// the base ID would be "BLUE_PANEL_"
		//---------------------------------------------------------
		const std::string& VerticalStretchableImage::GetBaseTextureAtlasID() const
		{
			return BaseTextureAtlasID;
		}
		//---------------------------------------------------------
		/// Set Patch Sprite Sheet Indices
		///
		/// @param Sprite sheet id of top patch
		/// @param Sprite sheet id of middle patch
		/// @param Sprite sheet id of bottom patch
		//---------------------------------------------------------
		void VerticalStretchableImage::SetTextureAtlasIds(const std::string& in_top, const std::string& in_middle, const std::string& in_bottom)
		{
            CS_ASSERT(TextureAtlas != nullptr, "Must have texture atlas to set IDs");
            
            m_panels.m_topSize = TextureAtlas->GetFrameSize(in_top);
            m_panels.m_topUVs = TextureAtlas->GetFrameUVs(in_top);
            
            m_panels.m_bottomSize = TextureAtlas->GetFrameSize(in_bottom);
            m_panels.m_bottomUVs = TextureAtlas->GetFrameUVs(in_bottom);
            
            m_panels.m_middleSize = TextureAtlas->GetFrameSize(in_middle);
            m_panels.m_middleUVs = TextureAtlas->GetFrameUVs(in_middle);
		}
        //---------------------------------------------------------
        /// Draw
        /// 
        /// Draw the image constructed from the nine patches
        ///
        /// @param Canvas renderer pointer
        //---------------------------------------------------------
        void VerticalStretchableImage::Draw(Rendering::CanvasRenderer* inpCanvas)
        {
			//Check if this is on screen
			Core::Vector2 vTopRight = GetAbsoluteScreenSpaceAnchorPoint(Rendering::AlignmentAnchor::k_topRight);
			Core::Vector2 vBottomLeft = GetAbsoluteScreenSpaceAnchorPoint(Rendering::AlignmentAnchor::k_bottomLeft);
			
			if(vTopRight.y < 0 || vBottomLeft.y > Core::Screen::GetOrientedHeight() || vTopRight.x < 0 || vBottomLeft.x > Core::Screen::GetOrientedWidth())
			{
				//Offscreen
				return;
			}
			
            if(Visible && TextureAtlas && Texture)
            {			
                Core::Vector2 vPanelSize = GetAbsoluteSize();
                Core::Vector2 vPanelPos = GetAbsoluteScreenSpacePosition();
                Core::Vector2 vTopLeft = GetAbsoluteAnchorPoint(Rendering::AlignmentAnchor::k_topLeft);
                Core::Vector2 vPatchPos;
                
                Core::Colour AbsColour = GetAbsoluteColour();
                
                //We need to use a matrix so that we can rotate all the patches with respect
                //to the view
                Core::Matrix3x3 matTransform;
                Core::Matrix3x3 matPatchTransform;
                Core::Matrix3x3 matViewTransform;
                
                matViewTransform.SetTransform(vPanelPos, Core::Vector2(1, 1), GetRotation());
                
                //Get the patch sizes
                Core::Vector2 vTPatchSize = m_panels.m_topSize;
                Core::Vector2 vBPatchSize = m_panels.m_bottomSize;
                
                vTPatchSize.x = vPanelSize.x;
                vBPatchSize.x = vPanelSize.x;

                Core::Vector2 vTCPatchSize;
                Core::Vector2 vBCPatchSize;
                Core::Vector2 vMCPatchSize;
                
                //Check to see if they are going to fit in the bounds of the view
                f32 fTotal = vTPatchSize.y + vBPatchSize.y;
                if(fTotal > vPanelSize.y)
                {
                    //The corners are too tall let's
                    //squish them
                    f32 fScale = vPanelSize.y/fTotal;
                    vTPatchSize.y *= fScale;
                    vBPatchSize.y *= fScale;
                }

                fTotal = vTPatchSize.y;
                if(fTotal > vPanelSize.y)
                {
                    //The corners are too tall let's
                    //squish them
                    f32 fScale = vPanelSize.y/fTotal;
                    vTPatchSize.y *= fScale;
                }
                fTotal = vBPatchSize.y;
                if(fTotal > vPanelSize.y)
                {
                    //The corners are too tall let's
                    //squish them
                    f32 fScale = vPanelSize.y/fTotal;
                    vBPatchSize.y *= fScale;
                }
                
                //Render ourself
                //Draw the corners first
                matPatchTransform.Translate(vTopLeft);
                
                Core::Matrix3x3::Multiply(&matPatchTransform, &matViewTransform, &matTransform);
                inpCanvas->DrawBox(matTransform,
                                   vTPatchSize, 
								   Texture,
                                   m_panels.m_topUVs,
                                   AbsColour, 
                                   Rendering::AlignmentAnchor::k_topLeft);
                
                
                matPatchTransform.Translate(GetAbsoluteAnchorPoint(Rendering::AlignmentAnchor::k_bottomLeft));
                Core::Matrix3x3::Multiply(&matPatchTransform, &matViewTransform, &matTransform);
                inpCanvas->DrawBox(matTransform, 
                                   vBPatchSize, 
								   Texture,
                                   m_panels.m_bottomUVs,
                                   AbsColour, 
                                   Rendering::AlignmentAnchor::k_bottomLeft);

 
                //Draw the centre
                vMCPatchSize.x = vTPatchSize.x;
                vMCPatchSize.y = vPanelSize.y - (vTPatchSize.y + vBPatchSize.y);
                vPatchPos.x = vTopLeft.x;
                vPatchPos.y = vTopLeft.y - vTPatchSize.y;
               
                matPatchTransform.Translate(vPatchPos);
                Core::Matrix3x3::Multiply(&matPatchTransform, &matViewTransform, &matTransform);
                inpCanvas->DrawBox(matTransform,
                                   vMCPatchSize, 
								   Texture,
                                   m_panels.m_middleUVs,
                                   AbsColour, 
                                   Rendering::AlignmentAnchor::k_topLeft);
                
                //Render subviews
                GUIView::Draw(inpCanvas);
            }
		}
		//--------------------------------------------------------
		/// Set Width Maintaining Aspect
		///
		/// Change the width of the image and resize the height
		/// to maintain the aspect ratio
		///
		/// @param Unified width
		//--------------------------------------------------------
		void VerticalStretchableImage::SetWidthMaintainingAspect(f32 infRelWidth, f32 infAbsWidth)
		{
            Core::Vector2 vCurrentSize = GetAbsoluteSize();
			f32 fAspectRatio = vCurrentSize.y / vCurrentSize.x;
			SetSize(infRelWidth, 0.0f, infAbsWidth, 0.0f);
			
			f32 fScaleY = GetAbsoluteScale().y;
			if(fScaleY == 0.0f)
				return;
			
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
		void VerticalStretchableImage::SetHeightMaintainingAspect(f32 infRelHeight, f32 infAbsHeight)
		{
            Core::Vector2 vCurrentSize = GetAbsoluteSize();
			f32 fAspectRatio = vCurrentSize.x / vCurrentSize.y;
			SetSize(0.0f, infRelHeight, 0.0f, infAbsHeight);
			
			f32 fScaleX = GetAbsoluteScale().x;
			if(fScaleX == 0.0f)
				return;
			
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
		void VerticalStretchableImage::EnableHeightMaintainingAspect(bool inbEnabled)
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
		void VerticalStretchableImage::EnableWidthMaintainingAspect(bool inbEnabled)
		{
			WidthMaintain = inbEnabled;            
		}
		//--------------------------------------------------------
		/// Is Width Maintaining Aspect Enabled
		///
		/// @return auto scaling of the Width to maintain the aspect ratio
		//--------------------------------------------------------
		bool VerticalStretchableImage::IsWidthMaintainingAspectEnabled() const
		{
			return WidthMaintain;
		}
		//--------------------------------------------------------
		/// Is Height Maintaining Aspect Enabled
		///
		/// @return auto scaling of the height to maintain the aspect ratio
		//--------------------------------------------------------
		bool VerticalStretchableImage::IsHeightMaintainingAspectEnabled() const
		{
			return HeightMaintain;
		}
		//--------------------------------------------------------
		/// Get Combined Cap Height
		///
		/// @param Sum of the widths of both end caps
		//--------------------------------------------------------
		f32 VerticalStretchableImage::GetCombinedCapHeight() const
		{
			return m_panels.m_topSize.y + m_panels.m_bottomSize.y;
		}
		//--------------------------------------------------------
		/// Get Combined Cap Width
		///
		/// @param Sum of the heights of both end caps
		//--------------------------------------------------------
		f32 VerticalStretchableImage::GetCapWidth() const
		{
			return m_panels.m_topSize.x;
		}
    }
}
