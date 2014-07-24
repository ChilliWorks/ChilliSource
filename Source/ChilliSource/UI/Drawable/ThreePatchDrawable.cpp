//
//  ThreePatchDrawable.cpp
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

#include <ChilliSource/UI/Drawable/ThreePatchDrawable.h>

#include <ChilliSource/Rendering/Base/AspectRatioUtils.h>
#include <ChilliSource/Rendering/Base/CanvasRenderer.h>
#include <ChilliSource/Rendering/Texture/Texture.h>

#include <array>

namespace ChilliSource
{
    namespace UI
    {
        namespace
        {
            const u32 k_numPatches = 3;
            
            //----------------------------------------------------------------------------------------
            /// Identifier for each patch in the 9 patch. Can be used as index look-ups into
            /// arrays of UVs, positions, etc.
            ///
            /// @author S Downie
            //----------------------------------------------------------------------------------------
            enum class Patch
            {
                k_leftOrBottom,
                k_centre,
                k_rightOrTop
            };
            //----------------------------------------------------------------------------------------
            /// Based on the insets and the given UVs calculate the UVs for each of the 9 patches
            ///
            /// @author S Downie
            ///
            /// @param UVs
            /// @param Left or bottom inset as normalised percentage
            /// @param Right or top inset as normalised percentage
            ///
            /// @return UVs for the 3 patches
            //----------------------------------------------------------------------------------------
            std::array<Rendering::UVs, k_numPatches> CalculateThreePatchUVs(const Rendering::UVs& in_UVs, f32 in_leftOrBottom, f32 in_rightOrTop)
            {
                std::array<Rendering::UVs, k_numPatches> result;
                
                //Fixed
                f32 leftU = in_UVs.m_u;
                f32 leftS = in_UVs.m_s * in_leftOrBottom;
                result[(u32)Patch::k_leftOrBottom].m_u = leftU;
                result[(u32)Patch::k_leftOrBottom].m_s = leftS;
                
                f32 rightS = in_UVs.m_s * in_rightOrTop;
                f32 rightU = in_UVs.m_u + in_UVs.m_s - rightS;
                result[(u32)Patch::k_rightOrTop].m_u = rightU;
                result[(u32)Patch::k_rightOrTop].m_s = rightS;
                
                //Stretchable
                result[(u32)Patch::k_centre].m_u = leftU + leftS;
                result[(u32)Patch::k_centre].m_s = result[(u32)Patch::k_rightOrTop].m_u - (result[(u32)Patch::k_leftOrBottom].m_u + result[(u32)Patch::k_leftOrBottom].m_s);

                return result;
            }
            //----------------------------------------------------------------------------------------
            /// Based on the widget size and the given insets calculate the absolute size of each patch
            ///
            /// @author S Downie
            ///
            /// @param Widget absolute size
            /// @param Image absolute size
            /// @param Left or bottom inset as normalised percentage
            /// @param Right or top inset as normalised percentage
            ///
            /// @return Sizes for the 3 patches
            //----------------------------------------------------------------------------------------
            std::array<Core::Vector2, k_numPatches> CalculateThreePatchSizes(const Core::Vector2& in_widgetSize, const Core::Vector2& in_imageSize, f32 in_leftOrBottom, f32 in_rightOrTop)
            {
                std::array<Core::Vector2, k_numPatches> result;
                
                result[(u32)Patch::k_leftOrBottom] = Rendering::AspectRatioUtils::KeepOriginalHeightAdaptWidth(Core::Vector2(in_imageSize.x * in_leftOrBottom, in_widgetSize.y), in_imageSize.x * in_leftOrBottom/in_imageSize.y);
                result[(u32)Patch::k_rightOrTop] = Rendering::AspectRatioUtils::KeepOriginalHeightAdaptWidth(Core::Vector2(in_imageSize.x * in_rightOrTop, in_widgetSize.y), in_imageSize.x * in_rightOrTop/in_imageSize.y);
                
                result[(u32)Patch::k_centre].x = std::max(in_widgetSize.x - result[(u32)Patch::k_leftOrBottom].x - result[(u32)Patch::k_rightOrTop].x, 0.0f);
                result[(u32)Patch::k_centre].y = result[(u32)Patch::k_leftOrBottom].y;
                
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
            /// @return Sizes for the 3 patches
            //----------------------------------------------------------------------------------------
            std::array<Core::Vector2, k_numPatches> CalculateThreePatchPositions(const std::array<Core::Vector2, k_numPatches>& in_sizes)
            {
                std::array<Core::Vector2, k_numPatches> result;
                
                Core::Vector2 halfCentre = in_sizes[(u32)Patch::k_centre] * 0.5f;
                Core::Vector2 halfLeftOrBottom  = in_sizes[(u32)Patch::k_leftOrBottom] * 0.5f;
                Core::Vector2 halfRightOrTop = in_sizes[(u32)Patch::k_rightOrTop] * 0.5f;
                
                result[(u32)Patch::k_centre] = Core::Vector2::k_zero;
                result[(u32)Patch::k_leftOrBottom].x = -halfCentre.x - halfLeftOrBottom.x;
                result[(u32)Patch::k_rightOrTop].x = halfCentre.x + halfRightOrTop.x;
                
                return result;
            }
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        ThreePatchDrawable::ThreePatchDrawable(Type in_type)
        {
            
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void ThreePatchDrawable::SetTexture(const Rendering::TextureCSPtr& in_texture)
        {
            m_texture = in_texture;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void ThreePatchDrawable::SetUVs(const Rendering::UVs& in_UVs)
        {
            m_UVs = in_UVs;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void ThreePatchDrawable::SetInsets(f32 in_leftOrBottom, f32 in_rightOrTop)
        {
            CS_ASSERT(in_leftOrBottom + in_rightOrTop <= 1.0f, "Insets must not overlap i.e. sum to more than 1");
            
            m_leftOrBottomInset = in_leftOrBottom;
            m_rightOrTopInset = in_rightOrTop;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        Core::Vector2 ThreePatchDrawable::GetPreferredSize() const
        {
            CS_ASSERT(m_texture != nullptr, "ThreePatchDrawable cannot get preferred size without texture");
            return Core::Vector2((f32)m_texture->GetWidth(), (f32)m_texture->GetHeight());
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void ThreePatchDrawable::Draw(Rendering::CanvasRenderer* in_renderer, const Core::Matrix3& in_transform, const Core::Vector2& in_absSize, const Core::Colour& in_absColour)
        {
            CS_ASSERT(m_texture != nullptr, "ThreePatchDrawable cannot draw without texture");
            
            auto uvs = CalculateThreePatchUVs(m_UVs, m_leftOrBottomInset, m_rightOrTopInset);
            auto sizes = CalculateThreePatchSizes(in_absSize, GetPreferredSize(), m_leftOrBottomInset, m_rightOrTopInset);
            auto positions = CalculateThreePatchPositions(sizes);
            
            for(u32 i=0; i<k_numPatches; ++i)
            {
                Core::Matrix3 patchTransform = Core::Matrix3::CreateTranslation(positions[i]);
                in_renderer->DrawBox(patchTransform * in_transform, sizes[i], Core::Vector2::k_zero, m_texture, uvs[i], in_absColour, Rendering::AlignmentAnchor::k_middleCentre);
            }
        }
    }
}