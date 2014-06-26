//
//  HorizontalStretchableImage.cpp
//  Chilli Source
//  Created by Stuart McGaw on 04/05/2011.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2011 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#include <ChilliSource/GUI/Image/HorizontalStretchableImage.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Resource/ResourcePool.h>
#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/Rendering/Base/CanvasRenderer.h>
#include <ChilliSource/Rendering/Texture/Texture.h>

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
			: WidthMaintain(false), ActAsSpacer(false)
		{
		}
		//---------------------------------------------------------
		/// Constructor 
		///
		/// @param Dictonary of values
		//---------------------------------------------------------
		HorizontalStretchableImage::HorizontalStretchableImage(const Core::ParamDictionary& insParams)
			: GUIView(insParams), WidthMaintain(false), ActAsSpacer(false)
		{
			std::string strValue;

            //---Texture
            Core::StorageLocation eTextureLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("TextureLocation", strValue))
            {
                eTextureLocation = CSCore::ParseStorageLocation(strValue);
            }
            if(insParams.TryGetValue("Texture", strValue))
            {
                Core::ResourcePool* resourcePool = Core::Application::Get()->GetResourcePool();
				SetTexture(resourcePool->LoadResource<Rendering::Texture>(eTextureLocation, strValue));
            }
            
            Core::StorageLocation eTextureAtlasLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("TextureAtlasLocation", strValue))
            {
                eTextureAtlasLocation = CSCore::ParseStorageLocation(strValue);
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
                m_frames[(u32)Patch::k_left] = TextureAtlas->GetFrame(strValue);
            }
            if(insParams.TryGetValue("RightID", strValue))
            {
                m_frames[(u32)Patch::k_right] = TextureAtlas->GetFrame(strValue);
            }
            if(insParams.TryGetValue("CentreID", strValue))
            {
                m_frames[(u32)Patch::k_centre] = TextureAtlas->GetFrame(strValue);
            }
            
			//---Maintain Height
			if(insParams.TryGetValue("WidthMaintain", strValue))
			{
				WidthMaintain = Core::ParseBool(strValue);
			}
            //---Set Maintain Height
			if(insParams.TryGetValue("SetWidthMaintain", strValue))
			{
				Core::Vector2 vSize = Core::ParseVector2(strValue);
				SetWidthMaintainingAspect(vSize.x, vSize.y);
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
        void HorizontalStretchableImage::SetTexture(const Rendering::TextureCSPtr& inpTexture)
        {
            Texture = inpTexture;
        }
        //---------------------------------------------------------
        /// Get Texture
        ///
        /// @return Texture containing the nine patches
        //---------------------------------------------------------
        const Rendering::TextureCSPtr& HorizontalStretchableImage::GetTexture() const
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
		//---------------------------------------------------------
		void HorizontalStretchableImage::SetBaseTextureAtlasID(const std::string& instrID)
		{
            CS_ASSERT(TextureAtlas != nullptr, "Must have texture atlas to set IDs");
            
            BaseTextureAtlasID = instrID;
            
            std::string leftId(instrID + "Left");
			std::string rightId(instrID + "Right");
			std::string centreId(instrID + "Centre");
            
            m_frames[(u32)Patch::k_left] = TextureAtlas->GetFrame(leftId);
            m_frames[(u32)Patch::k_right] = TextureAtlas->GetFrame(rightId);
            m_frames[(u32)Patch::k_centre] = TextureAtlas->GetFrame(centreId);
		}
		//---------------------------------------------------------
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
            
            m_frames[(u32)Patch::k_left] = TextureAtlas->GetFrame(in_left);
            m_frames[(u32)Patch::k_right] = TextureAtlas->GetFrame(in_right);
            m_frames[(u32)Patch::k_centre] = TextureAtlas->GetFrame(in_centre);
		}
		//--------------------------------------------------------
		/// Get Combined Cap Width
		///
		/// @param Sum of the widths of both end caps
		//--------------------------------------------------------
		f32 HorizontalStretchableImage::GetCombinedCapWidth() const
		{
			return m_frames[(u32)Patch::k_left].m_originalSize.x + m_frames[(u32)Patch::k_right].m_originalSize.x;
		}
		//--------------------------------------------------------
		//--------------------------------------------------------
		f32 HorizontalStretchableImage::GetCapHeight() const
		{
			return m_frames[(u32)Patch::k_left].m_originalSize.y;
		}
        //--------------------------------------------------------
		/// Set Width Maintaining Aspect
		///
		/// Change the width of the image and resize the height
		/// to maintain the aspect ratio
		///
		/// @param Unified width
		//--------------------------------------------------------
		void HorizontalStretchableImage::SetWidthMaintainingAspect(f32 infRelWidth, f32 infAbsWidth)
		{
            WidthMaintain = true;
            
            Core::Vector2 vCurrentSize = GetAbsoluteSize();
            
            f32 fAspectRatio = 1.0f;
            
            if(vCurrentSize.x != 0.0f)
            {
                fAspectRatio = vCurrentSize.y / vCurrentSize.x;
            }
            
			SetSize(infRelWidth, 0.0f, infAbsWidth, 0.0f);
			
			f32 fScaleY = GetAbsoluteScale().y;
			if(fScaleY == 0.0f)
				return;
			
			vCurrentSize = GetAbsoluteSize();
            f32 fAbsHeight = (fAspectRatio * vCurrentSize.x) / fScaleY;
			SetSize(infRelWidth, 0.0f, infAbsWidth, fAbsHeight);
		}
        //--------------------------------------------------------
		/// Enable Width Maintaining Aspect
		///
		/// Enables auto scaling of the height to maintain the aspect ratio
		///
		/// @param boolean to disable or enable
		//--------------------------------------------------------
		void HorizontalStretchableImage::EnableWidthMaintainingAspect(bool inbEnabled)
		{
			WidthMaintain = inbEnabled;
		}
		//--------------------------------------------------------
		/// Is Width Maintaining Aspect Enabled
		///
		/// @return auto scaling of the Width to maintain the aspect ratio
		//--------------------------------------------------------
		bool HorizontalStretchableImage::IsWidthMaintainingAspectEnabled() const
		{
			return WidthMaintain;
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
				
                if(WidthMaintain)
                {
                    Core::UnifiedVector2 uvSize = GetSize();
                    SetWidthMaintainingAspect(uvSize.GetRelative().x, uvSize.GetAbsolute().x);
                }
				
                if (ActAsSpacer == false)
                {
                    Core::Colour AbsColour = GetAbsoluteColour();
                    
                    //We need to use a matrix so that we can rotate all the patches with respect
                    //to the view
                    Core::Matrix3 matViewTransform = Core::Matrix3::CreateTransform(GetAbsoluteScreenSpacePosition(), Core::Vector2(1, 1), GetAbsoluteRotation());
                    
                    const Rendering::AlignmentAnchor alignments[(u32)Patch::k_total] =
                    {
                        Rendering::AlignmentAnchor::k_middleLeft,
                        Rendering::AlignmentAnchor::k_middleLeft,
                        Rendering::AlignmentAnchor::k_middleRight
                    };
                    
                    // Calculate dimentions and position for centre based on the panel size and the size of the caps
                    Core::Vector2 leftPatchSize(m_frames[(u32)Patch::k_left].m_originalSize.x, vPanelSize.y);
                    Core::Vector2 rightPatchSize(m_frames[(u32)Patch::k_right].m_originalSize.x, vPanelSize.y);
                    Core::Vector2 centrePatchSize(vPanelSize.x - (leftPatchSize.x + rightPatchSize.x), vPanelSize.y);
                    //If the size of the stretchable centre part is negative this means the caps are overlapping
                    //and need to be shrunk
                    f32 fShrinkX = (centrePatchSize.x < 0 ? centrePatchSize.x * 0.5f : 0);
                    // Clamp the size of the centre
                    centrePatchSize.x = std::max(centrePatchSize.x, 0.0f);
                    
                    const Core::Vector2 sizes[(u32)Patch::k_total] =
                    {
                        Core::Vector2(leftPatchSize.x + fShrinkX, leftPatchSize.y),
                        centrePatchSize,
                        Core::Vector2(rightPatchSize.x + fShrinkX, rightPatchSize.y),
                    };
                    
                    Core::Vector2 vPanelLeftPos = GetAbsoluteAnchorPoint(Rendering::AlignmentAnchor::k_middleLeft);
                    Core::Vector2 vPanelRightPos = GetAbsoluteAnchorPoint(Rendering::AlignmentAnchor::k_middleRight);
                    const Core::Vector2 positions[(u32)Patch::k_total] =
                    {
                        vPanelLeftPos,
                        Core::Vector2(vPanelLeftPos.x + sizes[(u32)Patch::k_left].x, vPanelLeftPos.y),
                        vPanelRightPos
                    };
                    
                    for(u32 i=0; i<(u32)Patch::k_total; ++i)
                    {
                        Core::Vector2 offset;
                        offset.x = (-m_frames[i].m_originalSize.x * 0.5f) + (m_frames[i].m_croppedSize.x * 0.5f) + m_frames[i].m_offset.x;
                        offset.y = (m_frames[i].m_originalSize.y * 0.5f) - (m_frames[i].m_croppedSize.y * 0.5f) - m_frames[i].m_offset.y;
  
                        //Convert from texel space to local sprite space
                        offset = sizes[i]/m_frames[i].m_originalSize * offset;
                        Core::Vector2 size = sizes[i]/m_frames[i].m_originalSize * m_frames[i].m_croppedSize;
                        
                        Core::Matrix3 matPatchTransform = Core::Matrix3::CreateTranslation(positions[i] + offset);
                        Core::Matrix3 matTransform = matPatchTransform * matViewTransform;
                        inpCanvas->DrawBox(matTransform,
                                           size,
                                           offset,
                                           Texture,
                                           m_frames[i].m_uvs,
                                           AbsColour,
                                           alignments[i]);
                    }
				}
                
				//Render subviews
				GUIView::Draw(inpCanvas);
			}
		}
	}
}