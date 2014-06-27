//
//  StretchableImage.cpp
//  Chilli Source
//  Created by Scott Downie on 28/04/2011
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

#include <ChilliSource/GUI/Image/StretchableImage.h>

#include <ChilliSource/Rendering/Texture/Texture.h>
#include <ChilliSource/Rendering/Base/CanvasRenderer.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/Screen.h>
#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/Core/Resource/ResourcePool.h>

namespace ChilliSource
{
    namespace GUI
    {
		DEFINE_META_CLASS(StretchableImage)
		
		DEFINE_PROPERTY(TextureAtlas);
		DEFINE_PROPERTY(BaseTextureAtlasID);
		DEFINE_PROPERTY(CentreTouchConsumption);
		
		//--------------------------------------------------------
        /// Constructor
        /// 
        /// Empty
        //---------------------------------------------------------
        StretchableImage::StretchableImage()
        {
            
        }
        //---------------------------------------------------------
        /// Constructor
        ///
        /// From param dictionary
        //---------------------------------------------------------
        StretchableImage::StretchableImage(const Core::ParamDictionary& insParams) 
		: GUIView(insParams)
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
        }
        //---------------------------------------------------------
        /// Set Texture
        ///
        /// @param Texture containing the nine patches
        //---------------------------------------------------------
        void StretchableImage::SetTexture(const Rendering::TextureCSPtr& inpTexture)
        {
            Texture = inpTexture;
        }
        //---------------------------------------------------------
        /// Get Texture
        ///
        /// @return Texture containing the nine patches
        //---------------------------------------------------------
        const Rendering::TextureCSPtr& StretchableImage::GetTexture() const
        {
            return Texture;
        }
        //---------------------------------------------------------
        /// Set Sprite Sheet
        ///
        /// @param Sprite sheet containing the nine patches
        //---------------------------------------------------------
        void StretchableImage::SetTextureAtlas(const Rendering::TextureAtlasCSPtr& inpTextureAtlas)
        {
            TextureAtlas = inpTextureAtlas;
        }
		//---------------------------------------------------------
		/// Get Sprite Sheet
		///
		/// @return Sprite sheet containing the nine patches
		//---------------------------------------------------------
		const Rendering::TextureAtlasCSPtr& StretchableImage::GetTextureAtlas() const
		{
			return TextureAtlas;
		}
		//---------------------------------------------------------
		//---------------------------------------------------------
		void StretchableImage::SetBaseTextureAtlasID(const std::string& instrID)
		{
            CS_ASSERT(TextureAtlas != nullptr, "Must have texture atlas to set IDs");
                
            BaseTextureAtlasID = instrID;
            
            std::string atlasId = instrID + "TopLeft";
            m_frames[(u32)Patch::k_topLeft] = TextureAtlas->GetFrame(atlasId);
            
            atlasId = instrID + "TopCentre";
            m_frames[(u32)Patch::k_topCentre] = TextureAtlas->GetFrame(atlasId);
            
            atlasId = instrID + "TopRight";
            m_frames[(u32)Patch::k_topRight] = TextureAtlas->GetFrame(atlasId);
            
            atlasId = instrID + "BottomLeft";
            m_frames[(u32)Patch::k_bottomLeft] = TextureAtlas->GetFrame(atlasId);
            
            atlasId = instrID + "BottomCentre";
            m_frames[(u32)Patch::k_bottomCentre] = TextureAtlas->GetFrame(atlasId);
            
            atlasId = instrID + "BottomRight";
            m_frames[(u32)Patch::k_bottomRight] = TextureAtlas->GetFrame(atlasId);
            
            atlasId = instrID + "MiddleLeft";
            m_frames[(u32)Patch::k_middleLeft] = TextureAtlas->GetFrame(atlasId);
            
            atlasId = instrID + "MiddleCentre";
            m_frames[(u32)Patch::k_middleCentre] = TextureAtlas->GetFrame(atlasId);
            
            atlasId = instrID + "MiddleRight";
            m_frames[(u32)Patch::k_middleRight] = TextureAtlas->GetFrame(atlasId);
		}
		//---------------------------------------------------------
		//---------------------------------------------------------
		const std::string& StretchableImage::GetBaseTextureAtlasID() const
		{
			return BaseTextureAtlasID;
		}
        //---------------------------------------------------------
        //---------------------------------------------------------
        void StretchableImage::Draw(Rendering::CanvasRenderer* inpCanvas)
        {
			//Check if this is on screen
			Core::Vector2 vTopRight = GetAbsoluteScreenSpaceAnchorPoint(Rendering::AlignmentAnchor::k_topRight);
			Core::Vector2 vBottomLeft = GetAbsoluteScreenSpaceAnchorPoint(Rendering::AlignmentAnchor::k_bottomLeft);
			
			if(vTopRight.y < 0 || vBottomLeft.y > GetScreen()->GetResolution().y || vTopRight.x < 0 || vBottomLeft.x > GetScreen()->GetResolution().x)
			{
				//Offscreen
				return;
			}
			
            if(Visible && TextureAtlas && Texture)
            {
                Core::Colour AbsColour = GetAbsoluteColour();
                
                //We need to use a matrix so that we can rotate all the patches with respect
                //to the view
                Core::Matrix3 matViewTransform = Core::Matrix3::CreateTransform(GetAbsoluteScreenSpacePosition(), Core::Vector2(1, 1), GetAbsoluteRotation());
				
				// Retrieve each bit's size
				CalculatePatchSizes();
                CalculatePatchPositions();
                
                const Rendering::AlignmentAnchor alignments[(u32)Patch::k_total] =
                {
                    Rendering::AlignmentAnchor::k_topLeft,
                    Rendering::AlignmentAnchor::k_topLeft,
                    Rendering::AlignmentAnchor::k_topRight,
                    Rendering::AlignmentAnchor::k_topLeft,
                    Rendering::AlignmentAnchor::k_topLeft,
                    Rendering::AlignmentAnchor::k_topRight,
                    Rendering::AlignmentAnchor::k_bottomLeft,
                    Rendering::AlignmentAnchor::k_bottomLeft,
                    Rendering::AlignmentAnchor::k_bottomRight
                };
                
                for(u32 i=0; i<(u32)Patch::k_total; ++i)
                {
                    Core::Vector2 offset;
                    offset.x = (-m_frames[i].m_originalSize.x * 0.5f) + (m_frames[i].m_croppedSize.x * 0.5f) + m_frames[i].m_offset.x;
                    offset.y = (m_frames[i].m_originalSize.y * 0.5f) - (m_frames[i].m_croppedSize.y * 0.5f) - m_frames[i].m_offset.y;
                    
                    //Convert from texel space to local sprite space
                    offset = m_patchSizes[i]/m_frames[i].m_originalSize * offset;
                    Core::Vector2 size = m_patchSizes[i]/m_frames[i].m_originalSize * m_frames[i].m_croppedSize;
                    
                    Core::Matrix3 matPatchTransform = Core::Matrix3::CreateTranslation(m_patchPositions[i]);
                    Core::Matrix3 matTransform = matPatchTransform * matViewTransform;
                    inpCanvas->DrawBox(matTransform,
                                       size,
                                       offset,
                                       Texture,
                                       m_frames[i].m_uvs,
                                       AbsColour,
                                       alignments[i]);
                }
                
                // Render subviews
                GUIView::Draw(inpCanvas);
            }
		}
        
        void StretchableImage::CalculatePatchPositions()
		{
            Core::Vector2 vTopLeft = GetAbsoluteAnchorPoint(Rendering::AlignmentAnchor::k_topLeft);
            
            m_patchPositions[(u32)Patch::k_topLeft] = vTopLeft;
            m_patchPositions[(u32)Patch::k_topRight] = GetAbsoluteAnchorPoint(Rendering::AlignmentAnchor::k_topRight);
            m_patchPositions[(u32)Patch::k_bottomLeft] = GetAbsoluteAnchorPoint(Rendering::AlignmentAnchor::k_bottomLeft);
            m_patchPositions[(u32)Patch::k_bottomRight] = GetAbsoluteAnchorPoint(Rendering::AlignmentAnchor::k_bottomRight);
            
            m_patchPositions[(u32)Patch::k_topCentre].x = vTopLeft.x + m_patchSizes[(u32)Patch::k_topLeft].x;
            m_patchPositions[(u32)Patch::k_topCentre].y = GetAbsoluteAnchorPoint(Rendering::AlignmentAnchor::k_topCentre).y;
            
            m_patchPositions[(u32)Patch::k_bottomCentre].x = vTopLeft.x + m_patchSizes[(u32)Patch::k_bottomLeft].x;
            m_patchPositions[(u32)Patch::k_bottomCentre].y = GetAbsoluteAnchorPoint(Rendering::AlignmentAnchor::k_bottomCentre).y;
            
            m_patchPositions[(u32)Patch::k_middleLeft].x = GetAbsoluteAnchorPoint(Rendering::AlignmentAnchor::k_middleLeft).x;
            m_patchPositions[(u32)Patch::k_middleLeft].y = vTopLeft.y - m_patchSizes[(u32)Patch::k_topLeft].y;
            
            m_patchPositions[(u32)Patch::k_middleRight].x = GetAbsoluteAnchorPoint(Rendering::AlignmentAnchor::k_middleRight).x;
            m_patchPositions[(u32)Patch::k_middleRight].y = vTopLeft.y - m_patchSizes[(u32)Patch::k_topRight].y;
            
            m_patchPositions[(u32)Patch::k_middleCentre].x = vTopLeft.x + m_patchSizes[(u32)Patch::k_topLeft].x;
            m_patchPositions[(u32)Patch::k_middleCentre].y = vTopLeft.y - m_patchSizes[(u32)Patch::k_topLeft].y;
        }
		
		void StretchableImage::CalculatePatchSizes()
		{
			Core::Vector2 vPanelSize = GetAbsoluteSize();

            m_patchSizes[(u32)Patch::k_topLeft] = m_frames[(u32)Patch::k_topLeft].m_originalSize;
            m_patchSizes[(u32)Patch::k_topRight] = m_frames[(u32)Patch::k_topRight].m_originalSize;
            m_patchSizes[(u32)Patch::k_bottomLeft] = m_frames[(u32)Patch::k_bottomLeft].m_originalSize;
            m_patchSizes[(u32)Patch::k_bottomRight] = m_frames[(u32)Patch::k_bottomRight].m_originalSize;
            
			//Check to see if they are going to fit in the bounds of the view
			f32 fTotal = m_patchSizes[(u32)Patch::k_topLeft].y + m_patchSizes[(u32)Patch::k_bottomLeft].y;
			if(fTotal > vPanelSize.y)
			{
				//The corners are too tall let's
				//squish them
				f32 fScale = vPanelSize.y/fTotal;
				m_patchSizes[(u32)Patch::k_topLeft].y *= fScale;
				m_patchSizes[(u32)Patch::k_bottomLeft].y *= fScale;
			}
			fTotal = m_patchSizes[(u32)Patch::k_topRight].y + m_patchSizes[(u32)Patch::k_bottomRight].y;
			if(fTotal > vPanelSize.y)
			{
				//The corners are too tall let's
				//squish them
				f32 fScale = vPanelSize.y/fTotal;
				m_patchSizes[(u32)Patch::k_topRight].y *= fScale;
				m_patchSizes[(u32)Patch::k_bottomRight].y *= fScale;
			}
			fTotal = m_patchSizes[(u32)Patch::k_topLeft].x + m_patchSizes[(u32)Patch::k_topRight].x;
			if(fTotal > vPanelSize.x)
			{
				//The corners are too tall let's
				//squish them
				f32 fScale = vPanelSize.x/fTotal;
				m_patchSizes[(u32)Patch::k_topLeft].x *= fScale;
				m_patchSizes[(u32)Patch::k_topRight].x *= fScale;
			}
			fTotal = m_patchSizes[(u32)Patch::k_bottomLeft].x + m_patchSizes[(u32)Patch::k_bottomRight].x;
			if(fTotal > vPanelSize.x)
			{
				//The corners are too tall let's
				//squish them
				f32 fScale = vPanelSize.x/fTotal;
				m_patchSizes[(u32)Patch::k_bottomLeft].x *= fScale;
				m_patchSizes[(u32)Patch::k_bottomRight].x *= fScale;
			}
			
			// Calculate the top
			m_patchSizes[(u32)Patch::k_topCentre].x = vPanelSize.x - (m_patchSizes[(u32)Patch::k_topLeft].x + m_patchSizes[(u32)Patch::k_topRight].x);
			m_patchSizes[(u32)Patch::k_topCentre].y = m_patchSizes[(u32)Patch::k_topLeft].y;
			
            // Calculate the bottom
			m_patchSizes[(u32)Patch::k_bottomCentre].x = vPanelSize.x - (m_patchSizes[(u32)Patch::k_bottomLeft].x + m_patchSizes[(u32)Patch::k_bottomRight].x);
			m_patchSizes[(u32)Patch::k_bottomCentre].y = m_patchSizes[(u32)Patch::k_bottomLeft].y;
			
			// Calculate the left
			m_patchSizes[(u32)Patch::k_middleLeft].y = vPanelSize.y - (m_patchSizes[(u32)Patch::k_topLeft].y + m_patchSizes[(u32)Patch::k_bottomLeft].y);
			m_patchSizes[(u32)Patch::k_middleLeft].x = m_patchSizes[(u32)Patch::k_topLeft].x;
			
			// Calculate the right
			m_patchSizes[(u32)Patch::k_middleRight].y = vPanelSize.y - (m_patchSizes[(u32)Patch::k_topRight].y + m_patchSizes[(u32)Patch::k_bottomRight].y);
			m_patchSizes[(u32)Patch::k_middleRight].x = m_patchSizes[(u32)Patch::k_topRight].x;
			
			// Calculate the centre
			m_patchSizes[(u32)Patch::k_middleCentre].x = vPanelSize.x - (m_patchSizes[(u32)Patch::k_middleLeft].x + m_patchSizes[(u32)Patch::k_middleRight].x);
			m_patchSizes[(u32)Patch::k_middleCentre].y = vPanelSize.y - (m_patchSizes[(u32)Patch::k_topCentre].y + m_patchSizes[(u32)Patch::k_bottomCentre].y);
		}
    }
}
