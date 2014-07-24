//
//  StretchableDrawable.cpp
//  Chilli Source
//  Created by Scott Downie on 24/07/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
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

#include <ChilliSource/UI/Drawable/StretchableDrawable.h>

#include <ChilliSource/Rendering/Base/CanvasRenderer.h>
#include <ChilliSource/Rendering/Texture/Texture.h>

#include <array>

namespace ChilliSource
{
    namespace UI
    {
        namespace
        {
            const u32 k_numPatches = 9;
            
            //----------------------------------------------------------------------------------------
            /// Identifier for each patch in the 9 patch. Can be used as index look-ups into
            /// arrays of UVs, positions, etc.
            ///
            /// @author S Downie
            //----------------------------------------------------------------------------------------
            enum class Patch
            {
                k_topLeft,
                k_topCentre,
                k_topRight,
                k_middleLeft,
                k_middleCentre,
                k_middleRight,
                k_bottomLeft,
                k_bottomCentre,
                k_bottomRight
            };
            //----------------------------------------------------------------------------------------
            /// Based on the insets and the given UVs calculate the UVs for each of the 9 patches
            ///
            /// @author S Downie
            ///
            /// @param UVs
            /// @param Left inset as normalised percentage
            /// @param Right inset as normalised percentage
            /// @param Top inset as normalised percentage
            /// @param Bottom inset as normalised percentage
            ///
            /// @return UVs for the 9 patches
            //----------------------------------------------------------------------------------------
            std::array<Rendering::UVs, k_numPatches> CalculateNinePatchUVs(const Rendering::UVs& in_UVs, f32 in_left, f32 in_right, f32 in_top, f32 in_bottom)
            {
                std::array<Rendering::UVs, k_numPatches> result;
                
                //Fixed
                f32 leftU = in_UVs.m_u;
                f32 leftS = in_UVs.m_s * in_left;
                result[(u32)Patch::k_topLeft].m_u = leftU;
                result[(u32)Patch::k_topLeft].m_s = leftS;
                result[(u32)Patch::k_middleLeft].m_u = leftU;
                result[(u32)Patch::k_middleLeft].m_s = leftS;
                result[(u32)Patch::k_bottomLeft].m_u = leftU;
                result[(u32)Patch::k_bottomLeft].m_s = leftS;
                
                f32 rightS = in_UVs.m_s * in_right;
                f32 rightU = in_UVs.m_u + in_UVs.m_s - rightS;
                result[(u32)Patch::k_topRight].m_u = rightU;
                result[(u32)Patch::k_topRight].m_s = rightS;
                result[(u32)Patch::k_middleRight].m_u = rightU;
                result[(u32)Patch::k_middleRight].m_s = rightS;
                result[(u32)Patch::k_bottomRight].m_u = rightU;
                result[(u32)Patch::k_bottomRight].m_s = rightS;
                
                f32 topV = in_UVs.m_v;
                f32 topT = in_UVs.m_t * in_top;
                result[(u32)Patch::k_topLeft].m_v = topV;
                result[(u32)Patch::k_topLeft].m_t = topT;
                result[(u32)Patch::k_topCentre].m_v = topV;
                result[(u32)Patch::k_topCentre].m_t = topT;
                result[(u32)Patch::k_topRight].m_v = topV;
                result[(u32)Patch::k_topRight].m_t = topT;
                
                f32 bottomT = in_UVs.m_t * in_bottom;
                f32 bottomV = in_UVs.m_v + in_UVs.m_t - bottomT;
                result[(u32)Patch::k_bottomLeft].m_v = bottomV;
                result[(u32)Patch::k_bottomLeft].m_t = bottomT;
                result[(u32)Patch::k_bottomCentre].m_v = bottomV;
                result[(u32)Patch::k_bottomCentre].m_t = bottomT;
                result[(u32)Patch::k_bottomRight].m_v = bottomV;
                result[(u32)Patch::k_bottomRight].m_t = bottomT;
                
                //Stretchable
                result[(u32)Patch::k_middleLeft].m_v = topV + topT;
                result[(u32)Patch::k_middleLeft].m_t = result[(u32)Patch::k_bottomLeft].m_v - (result[(u32)Patch::k_topLeft].m_v + result[(u32)Patch::k_topLeft].m_t);
                result[(u32)Patch::k_middleRight].m_v = result[(u32)Patch::k_middleLeft].m_v;
                result[(u32)Patch::k_middleRight].m_t = result[(u32)Patch::k_bottomRight].m_v - (result[(u32)Patch::k_topRight].m_v + result[(u32)Patch::k_topRight].m_t);
                
                result[(u32)Patch::k_topCentre].m_u = leftU + leftS;
                result[(u32)Patch::k_topCentre].m_s = result[(u32)Patch::k_topRight].m_u - (result[(u32)Patch::k_topLeft].m_u + result[(u32)Patch::k_topLeft].m_s);
                result[(u32)Patch::k_bottomCentre].m_u = result[(u32)Patch::k_topCentre].m_u;
                result[(u32)Patch::k_bottomCentre].m_s = result[(u32)Patch::k_bottomRight].m_u - (result[(u32)Patch::k_bottomLeft].m_u + result[(u32)Patch::k_bottomLeft].m_s);
                
                result[(u32)Patch::k_middleCentre].m_u = result[(u32)Patch::k_topCentre].m_u;
                result[(u32)Patch::k_middleCentre].m_s = result[(u32)Patch::k_topCentre].m_s;
                result[(u32)Patch::k_middleCentre].m_v = result[(u32)Patch::k_middleLeft].m_v;
                result[(u32)Patch::k_middleCentre].m_t = result[(u32)Patch::k_middleLeft].m_t;
                
                return result;
            }
            //----------------------------------------------------------------------------------------
            /// Based on the widget size and the given insets calculate the absolute size of each patch
            ///
            /// @author S Downie
            ///
            /// @param Widget absolute size
            /// @param Image absolute size
            /// @param Left inset as normalised percentage
            /// @param Right inset as normalised percentage
            /// @param Top inset as normalised percentage
            /// @param Bottom inset as normalised percentage
            ///
            /// @return Sizes for the 9 patches
            //----------------------------------------------------------------------------------------
            std::array<Core::Vector2, k_numPatches> CalculateNinePatchSizes(const Core::Vector2& in_widgetSize, const Core::Vector2& in_imageSize, f32 in_left, f32 in_right, f32 in_top, f32 in_bottom)
            {
                std::array<Core::Vector2, k_numPatches> result;
                
                //If the corners at their preferred size are larger than the widget then they need to be scaled down
                f32 totalPatchSizeX = in_imageSize.x * in_left + in_imageSize.x * in_left;
                f32 totalPatchSizeY = in_imageSize.y * in_top + in_imageSize.y * in_bottom;
                f32 scaleX = totalPatchSizeX <= in_widgetSize.x ? 1.0f : in_widgetSize.x/totalPatchSizeX;
                f32 scaleY = totalPatchSizeY <= in_widgetSize.y ? 1.0f : in_widgetSize.y/totalPatchSizeY;
                
                //Fixed
                result[(u32)Patch::k_topLeft].x = in_imageSize.x * in_left * scaleX;
                result[(u32)Patch::k_topLeft].y = in_imageSize.y * in_top * scaleY;
                result[(u32)Patch::k_topRight].x = in_imageSize.x * in_right * scaleX;
                result[(u32)Patch::k_topRight].y = in_imageSize.y * in_top * scaleY;
                result[(u32)Patch::k_bottomLeft].x = in_imageSize.x * in_left * scaleX;
                result[(u32)Patch::k_bottomLeft].y = in_imageSize.y * in_bottom * scaleY;
                result[(u32)Patch::k_bottomRight].x = in_imageSize.x * in_right * scaleX;
                result[(u32)Patch::k_bottomRight].y = in_imageSize.y * in_bottom * scaleY;
                result[(u32)Patch::k_middleLeft].x = in_imageSize.x * in_left * scaleX;
                result[(u32)Patch::k_middleRight].x = in_imageSize.x * in_right * scaleX;
                result[(u32)Patch::k_topCentre].y = in_imageSize.y * in_top * scaleY;
                result[(u32)Patch::k_bottomCentre].y = in_imageSize.y * in_bottom * scaleY;
                
                //Stretchable
                result[(u32)Patch::k_topCentre].x = std::max(in_widgetSize.x - result[(u32)Patch::k_topLeft].x - result[(u32)Patch::k_topRight].x, 0.0f);
                result[(u32)Patch::k_bottomCentre].x = std::max(in_widgetSize.x - result[(u32)Patch::k_bottomLeft].x - result[(u32)Patch::k_bottomRight].x, 0.0f);
                result[(u32)Patch::k_middleLeft].y = std::max(in_widgetSize.y - result[(u32)Patch::k_topLeft].y - result[(u32)Patch::k_bottomLeft].y, 0.0f);
                result[(u32)Patch::k_middleRight].y = std::max(in_widgetSize.y - result[(u32)Patch::k_topRight].y - result[(u32)Patch::k_bottomRight].y, 0.0f);
                result[(u32)Patch::k_middleCentre].x = result[(u32)Patch::k_topCentre].x;
                result[(u32)Patch::k_middleCentre].y = result[(u32)Patch::k_middleLeft].y;
                
                return result;
            }
            //----------------------------------------------------------------------------------------
            /// Calculate the local space positions for each patch with the origin of the overall
            /// widget at the centre (0, 0)
            ///
            /// @author S Downie
            ///
            /// @param Patch sizes
            ///
            /// @return Sizes for the 9 patches
            //----------------------------------------------------------------------------------------
            std::array<Core::Vector2, k_numPatches> CalculateNinePatchPositions(const std::array<Core::Vector2, k_numPatches>& in_sizes)
            {
                std::array<Core::Vector2, k_numPatches> result;
                
                result[(u32)Patch::k_middleCentre].x = 0.0f;
                result[(u32)Patch::k_middleCentre].y = 0.0f;
                
                Core::Vector2 halfMiddleCentre = in_sizes[(u32)Patch::k_middleCentre] * 0.5f;
                Core::Vector2 halfMiddleLeft = in_sizes[(u32)Patch::k_middleLeft] * 0.5f;
                Core::Vector2 halfMiddleRight = in_sizes[(u32)Patch::k_middleRight] * 0.5f;
                Core::Vector2 halfTopCentre = in_sizes[(u32)Patch::k_topCentre] * 0.5f;
                Core::Vector2 halfTopLeft = in_sizes[(u32)Patch::k_topLeft] * 0.5f;
                Core::Vector2 halfTopRight = in_sizes[(u32)Patch::k_topRight] * 0.5f;
                Core::Vector2 halfBottomCentre = in_sizes[(u32)Patch::k_bottomCentre] * 0.5f;
                Core::Vector2 halfBottomLeft = in_sizes[(u32)Patch::k_bottomLeft] * 0.5f;
                Core::Vector2 halfBottomRight = in_sizes[(u32)Patch::k_bottomRight] * 0.5f;
                
                result[(u32)Patch::k_topCentre].x = 0.0f;
                result[(u32)Patch::k_topCentre].y = result[(u32)Patch::k_middleCentre].y + halfMiddleCentre.y + halfTopCentre.y;
                
                result[(u32)Patch::k_bottomCentre].x = 0.0f;
                result[(u32)Patch::k_bottomCentre].y = result[(u32)Patch::k_middleCentre].y - halfMiddleCentre.y - halfBottomCentre.y;
                
                result[(u32)Patch::k_middleLeft].x = result[(u32)Patch::k_middleCentre].x - halfMiddleCentre.x - halfMiddleLeft.x;
                result[(u32)Patch::k_middleLeft].y = 0.0f;
                
                result[(u32)Patch::k_middleRight].x = result[(u32)Patch::k_middleCentre].x + halfMiddleCentre.x + halfMiddleRight.x;
                result[(u32)Patch::k_middleRight].y = 0.0f;
                
                result[(u32)Patch::k_topLeft].x = result[(u32)Patch::k_topCentre].x - halfTopCentre.x - halfTopLeft.x;
                result[(u32)Patch::k_topLeft].y = result[(u32)Patch::k_middleLeft].y + halfMiddleLeft.y + halfTopLeft.y;
                
                result[(u32)Patch::k_topRight].x = result[(u32)Patch::k_topCentre].x + halfTopCentre.x + halfTopRight.x;
                result[(u32)Patch::k_topRight].y = result[(u32)Patch::k_middleRight].y + halfMiddleRight.y + halfTopRight.y;
                
                result[(u32)Patch::k_bottomLeft].x = result[(u32)Patch::k_bottomCentre].x - halfBottomCentre.x - halfBottomLeft.x;
                result[(u32)Patch::k_bottomLeft].y = result[(u32)Patch::k_middleLeft].y - halfMiddleLeft.y - halfBottomLeft.y;
                
                result[(u32)Patch::k_bottomRight].x = result[(u32)Patch::k_bottomCentre].x + halfBottomCentre.x + halfBottomRight.x;
                result[(u32)Patch::k_bottomRight].y = result[(u32)Patch::k_middleRight].y - halfMiddleRight.y - halfBottomRight.y;
                
                return result;
            }
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void StretchableDrawable::SetTexture(const Rendering::TextureCSPtr& in_texture)
        {
            m_texture = in_texture;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void StretchableDrawable::SetUVs(const Rendering::UVs& in_UVs)
        {
            m_UVs = in_UVs;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void StretchableDrawable::SetInsets(f32 in_left, f32 in_right, f32 in_top, f32 in_bottom)
        {
            m_leftInset = in_left;
            m_rightInset = in_right;
            m_topInset = in_top;
            m_bottomInset = in_bottom;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        Core::Vector2 StretchableDrawable::GetPreferredSize() const
        {
            CS_ASSERT(m_texture != nullptr, "StretchableDrawable cannot get preferred size without texture");
            return Core::Vector2((f32)m_texture->GetWidth(), (f32)m_texture->GetHeight());
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void StretchableDrawable::Draw(Rendering::CanvasRenderer* in_renderer, const Core::Matrix3& in_transform, const Core::Vector2& in_absSize, const Core::Colour& in_absColour)
        {
            CS_ASSERT(m_texture != nullptr, "StretchableDrawable cannot draw without texture");
            
            auto uvs = CalculateNinePatchUVs(m_UVs, m_leftInset, m_rightInset, m_topInset, m_bottomInset);
            auto sizes = CalculateNinePatchSizes(in_absSize, GetPreferredSize(), m_leftInset, m_rightInset, m_topInset, m_bottomInset);
            auto positions = CalculateNinePatchPositions(sizes);
            
            for(u32 i=0; i<k_numPatches; ++i)
            {
                Core::Matrix3 patchTransform = Core::Matrix3::CreateTranslation(positions[i]);
                in_renderer->DrawBox(patchTransform * in_transform, sizes[i], Core::Vector2::k_zero, m_texture, uvs[i], in_absColour, Rendering::AlignmentAnchor::k_middleCentre);
            }
        }
    }
}