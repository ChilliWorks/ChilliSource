//
//  VerticalStretchableImage.cpp
//  Chilli Source
//  Created by Nicolas Tanda on 22/06/2012
//
//  The MIT License (MIT)
//
//  Copyright (c) 2012 Tag Games Limited
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

#include <ChilliSource/GUI/Image/VerticalStretchableImage.h>

#include <ChilliSource/Rendering/Texture/Texture.h>
#include <ChilliSource/Rendering/Base/CanvasRenderer.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Resource/ResourcePool.h>
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
			: HeightMaintain(false), ActAsSpacer(false)
        {
        }
        //---------------------------------------------------------
        /// Constructor
        ///
        /// From param dictionary
        //---------------------------------------------------------
        VerticalStretchableImage::VerticalStretchableImage(const Core::ParamDictionary& insParams) 
			: GUIView(insParams), HeightMaintain(false), ActAsSpacer(false)
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
            
            //---Sprite sheet
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

			//---Sprite sheet base name
			if(insParams.TryGetValue("BaseTextureAtlasID", strValue))
            {
				SetBaseTextureAtlasID(strValue);
			}

            //---Sprite sheet indices
            if(insParams.TryGetValue("TopID", strValue))
            {
                m_frames[(u32)Patch::k_top] = TextureAtlas->GetFrame(strValue);
            }
            if(insParams.TryGetValue("BottomID", strValue))
            {
                m_frames[(u32)Patch::k_bottom] = TextureAtlas->GetFrame(strValue);
            }
            if(insParams.TryGetValue("MiddleID", strValue))
            {
                m_frames[(u32)Patch::k_middle] = TextureAtlas->GetFrame(strValue);
            }
			//---Maintain Width
			if(insParams.TryGetValue("HeightMaintain", strValue))
			{
				HeightMaintain = Core::ParseBool(strValue);
			}
			//---Set Maintain Width
			if(insParams.TryGetValue("SetHeightMaintain", strValue))
			{
				Core::Vector2 vSize = Core::ParseVector2(strValue);
				SetHeightMaintainingAspect(vSize.x, vSize.y);
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
        void VerticalStretchableImage::SetTexture(const Rendering::TextureCSPtr& inpTexture)
        {
            Texture = inpTexture;
        }
        //---------------------------------------------------------
        /// Get Texture
        ///
        /// @return Texture containing the nine patches
        //---------------------------------------------------------
        const Rendering::TextureCSPtr& VerticalStretchableImage::GetTexture() const
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
		//---------------------------------------------------------
		void VerticalStretchableImage::SetBaseTextureAtlasID(const std::string& instrID)
		{
            CS_ASSERT(TextureAtlas != nullptr, "Must have texture atlas to set IDs");
            
            BaseTextureAtlasID = instrID;

            std::string topId(instrID + "Top");
			std::string bottomId(instrID + "Bottom");
			std::string middleId(instrID + "Middle");
            
            m_frames[(u32)Patch::k_top] = TextureAtlas->GetFrame(topId);
            m_frames[(u32)Patch::k_middle] = TextureAtlas->GetFrame(middleId);
            m_frames[(u32)Patch::k_bottom] = TextureAtlas->GetFrame(bottomId);
		}
		//---------------------------------------------------------
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
            
            m_frames[(u32)Patch::k_top] = TextureAtlas->GetFrame(in_top);
            m_frames[(u32)Patch::k_middle] = TextureAtlas->GetFrame(in_middle);
            m_frames[(u32)Patch::k_bottom] = TextureAtlas->GetFrame(in_bottom);
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
			if(Visible && TextureAtlas && Texture)
			{
				Core::Vector2 vPanelSize = GetAbsoluteSize();
				
                if(HeightMaintain)
                {
                    Core::UnifiedVector2 uvSize = GetSize();
                    SetHeightMaintainingAspect(uvSize.GetRelative().y, uvSize.GetAbsolute().y);
                }
				
                if (ActAsSpacer == false)
                {
                    Core::Colour AbsColour = GetAbsoluteColour();
                    
                    //We need to use a matrix so that we can rotate all the patches with respect
                    //to the view
                    Core::Matrix3 matViewTransform = Core::Matrix3::CreateTransform(GetAbsoluteScreenSpacePosition(), Core::Vector2(1, 1), GetAbsoluteRotation());
                    
                    const Rendering::AlignmentAnchor alignments[(u32)Patch::k_total] =
                    {
                        Rendering::AlignmentAnchor::k_topCentre,
                        Rendering::AlignmentAnchor::k_topCentre,
                        Rendering::AlignmentAnchor::k_bottomCentre
                    };
                    
                    // Calculate dimentions and position for centre based on the panel size and the size of the caps
                    Core::Vector2 topPatchSize(vPanelSize.x, m_frames[(u32)Patch::k_top].m_originalSize.y);
                    Core::Vector2 bottomPatchSize(vPanelSize.x, m_frames[(u32)Patch::k_bottom].m_originalSize.y);
                    Core::Vector2 centrePatchSize(vPanelSize.x, vPanelSize.y - (topPatchSize.y + bottomPatchSize.y));
                    //If the size of the stretchable centre part is negative this means the caps are overlapping
                    //and need to be shrunk
                    f32 fShrinkY = (centrePatchSize.y < 0 ? centrePatchSize.y * 0.5f : 0);
                    // Clamp the size of the centre
                    centrePatchSize.y = std::max(centrePatchSize.y, 0.0f);
                    
                    const Core::Vector2 sizes[(u32)Patch::k_total] =
                    {
                        Core::Vector2(topPatchSize.x, topPatchSize.y + fShrinkY),
                        centrePatchSize,
                        Core::Vector2(bottomPatchSize.x, bottomPatchSize.y + fShrinkY)
                    };
                    
                    Core::Vector2 vPanelTopPos = GetAbsoluteAnchorPoint(Rendering::AlignmentAnchor::k_topCentre);
                    Core::Vector2 vPanelBottomPos = GetAbsoluteAnchorPoint(Rendering::AlignmentAnchor::k_bottomCentre);
                    const Core::Vector2 positions[(u32)Patch::k_total] =
                    {
                        vPanelTopPos,
                        Core::Vector2(vPanelTopPos.x, vPanelTopPos.y - sizes[(u32)Patch::k_top].y),
                        vPanelBottomPos
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
            HeightMaintain = true;
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
            return m_frames[(u32)Patch::k_top].m_originalSize.y + m_frames[(u32)Patch::k_bottom].m_originalSize.y;
		}
		//--------------------------------------------------------
		/// Get Combined Cap Width
		///
		/// @param Sum of the heights of both end caps
		//--------------------------------------------------------
		f32 VerticalStretchableImage::GetCapWidth() const
		{
            return m_frames[(u32)Patch::k_top].m_originalSize.x;
		}
    }
}
