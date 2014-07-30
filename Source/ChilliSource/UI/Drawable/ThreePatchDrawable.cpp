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

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Resource/ResourcePool.h>
#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/Rendering/Base/AspectRatioUtils.h>
#include <ChilliSource/Rendering/Base/CanvasRenderer.h>
#include <ChilliSource/Rendering/Texture/Texture.h>

namespace ChilliSource
{
    namespace UI
    {
        namespace
        {
            std::vector<PropertyMap::PropertyDesc> g_propertyDescs =
            {
                {PropertyType::k_string, "Type", "ThreePatch"},
                {PropertyType::k_string, "Direction", "Horizontal"},
                {PropertyType::k_vec4, "UVs", "0 0 1 1"},
                {PropertyType::k_vec2, "Insets", "0.01 0.01"},
                {PropertyType::k_string, "TextureLocation", "Package"},
                {PropertyType::k_string, "TexturePath", ""}
            };
        
            //----------------------------------------------------------------------------------------
            /// Identifier for each patch in the 3 patch. Can be used as index look-ups into
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
            /// Based on the insets and the given UVs calculate the UVs for each of the 3 patches
            ///
            /// @author S Downie
            ///
            /// @param UVs
            /// @param Left inset as normalised percentage
            /// @param Right inset as normalised percentage
            ///
            /// @return UVs for the 3 patches
            //----------------------------------------------------------------------------------------
            std::array<Rendering::UVs, ThreePatchDrawable::k_numPatches> CalculateThreePatchUVsHorizontal(const Rendering::UVs& in_UVs, f32 in_left, f32 in_right)
            {
                std::array<Rendering::UVs, ThreePatchDrawable::k_numPatches> result;
                
                //Fixed
                f32 leftU = in_UVs.m_u;
                f32 leftS = in_UVs.m_s * in_left;
                result[(u32)Patch::k_leftOrBottom].m_u = leftU;
                result[(u32)Patch::k_leftOrBottom].m_s = leftS;
                
                f32 rightS = in_UVs.m_s * in_right;
                f32 rightU = in_UVs.m_u + in_UVs.m_s - rightS;
                result[(u32)Patch::k_rightOrTop].m_u = rightU;
                result[(u32)Patch::k_rightOrTop].m_s = rightS;
                
                //Stretchable
                result[(u32)Patch::k_centre].m_u = leftU + leftS;
                result[(u32)Patch::k_centre].m_s = in_UVs.m_s - (leftS + rightS);

                return result;
            }
            //----------------------------------------------------------------------------------------
            /// Based on the insets and the given UVs calculate the UVs for each of the 3 patches
            ///
            /// @author S Downie
            ///
            /// @param UVs
            /// @param Bottom inset as normalised percentage
            /// @param Top inset as normalised percentage
            ///
            /// @return UVs for the 3 patches
            //----------------------------------------------------------------------------------------
            std::array<Rendering::UVs, ThreePatchDrawable::k_numPatches> CalculateThreePatchUVsVertical(const Rendering::UVs& in_UVs, f32 in_bottom, f32 in_top)
            {
                std::array<Rendering::UVs, ThreePatchDrawable::k_numPatches> result;
                
                //Fixed
                f32 topV = in_UVs.m_v;
                f32 topT = in_UVs.m_t * in_top;
                result[(u32)Patch::k_rightOrTop].m_v = topV;
                result[(u32)Patch::k_rightOrTop].m_t = topT;
                
                f32 bottomT = in_UVs.m_t * in_bottom;
                f32 bottomV = in_UVs.m_v + in_UVs.m_t - bottomT;
                result[(u32)Patch::k_leftOrBottom].m_v = bottomV;
                result[(u32)Patch::k_leftOrBottom].m_t = bottomT;
                
                //Stretchable
                result[(u32)Patch::k_centre].m_v = topV + topT;
                result[(u32)Patch::k_centre].m_t = in_UVs.m_t - (topT + bottomT);
                
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
            ///
            /// @return Sizes for the 3 patches
            //----------------------------------------------------------------------------------------
            std::array<Core::Vector2, ThreePatchDrawable::k_numPatches> CalculateThreePatchSizesHorizontal(const Core::Vector2& in_widgetSize, const Core::Vector2& in_imageSize, f32 in_left, f32 in_right)
            {
                std::array<Core::Vector2, ThreePatchDrawable::k_numPatches> result;
                
                result[(u32)Patch::k_leftOrBottom] = Rendering::AspectRatioUtils::KeepOriginalHeightAdaptWidth(Core::Vector2(in_imageSize.x * in_left, in_widgetSize.y), (in_imageSize.x * in_left)/in_imageSize.y);
                result[(u32)Patch::k_rightOrTop] = Rendering::AspectRatioUtils::KeepOriginalHeightAdaptWidth(Core::Vector2(in_imageSize.x * in_right, in_widgetSize.y), (in_imageSize.x * in_right)/in_imageSize.y);
                
                result[(u32)Patch::k_centre].x = std::max(in_widgetSize.x - result[(u32)Patch::k_leftOrBottom].x - result[(u32)Patch::k_rightOrTop].x, 0.0f);
                result[(u32)Patch::k_centre].y = result[(u32)Patch::k_leftOrBottom].y;
                
                return result;
            }
            //----------------------------------------------------------------------------------------
            /// Based on the widget size and the given insets calculate the absolute size of each patch
            ///
            /// @author S Downie
            ///
            /// @param Widget absolute size
            /// @param Image absolute size
            /// @param Bottom inset as normalised percentage
            /// @param Top inset as normalised percentage
            ///
            /// @return Sizes for the 3 patches
            //----------------------------------------------------------------------------------------
            std::array<Core::Vector2, ThreePatchDrawable::k_numPatches> CalculateThreePatchSizesVertical(const Core::Vector2& in_widgetSize, const Core::Vector2& in_imageSize, f32 in_bottom, f32 in_top)
            {
                std::array<Core::Vector2, ThreePatchDrawable::k_numPatches> result;
                
                result[(u32)Patch::k_leftOrBottom] = Rendering::AspectRatioUtils::KeepOriginalWidthAdaptHeight(Core::Vector2(in_widgetSize.x, in_imageSize.y * in_bottom), in_imageSize.x/(in_imageSize.y * in_bottom));
                result[(u32)Patch::k_rightOrTop] = Rendering::AspectRatioUtils::KeepOriginalWidthAdaptHeight(Core::Vector2(in_widgetSize.x, in_imageSize.y * in_top), in_imageSize.x/(in_imageSize.y * in_top));
                
                result[(u32)Patch::k_centre].x = result[(u32)Patch::k_leftOrBottom].x;
                result[(u32)Patch::k_centre].y = std::max(in_widgetSize.y - result[(u32)Patch::k_leftOrBottom].y - result[(u32)Patch::k_rightOrTop].y, 0.0f);
                
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
            std::array<Core::Vector2, ThreePatchDrawable::k_numPatches> CalculateThreePatchPositionsHorizontal(const std::array<Core::Vector2, ThreePatchDrawable::k_numPatches>& in_sizes)
            {
                std::array<Core::Vector2, ThreePatchDrawable::k_numPatches> result;
                
                Core::Vector2 halfCentre = in_sizes[(u32)Patch::k_centre] * 0.5f;
                Core::Vector2 halfLeft = in_sizes[(u32)Patch::k_leftOrBottom] * 0.5f;
                Core::Vector2 halfRight = in_sizes[(u32)Patch::k_rightOrTop] * 0.5f;
                
                result[(u32)Patch::k_centre] = Core::Vector2(halfLeft.x - halfRight.x, 0.0f);
                result[(u32)Patch::k_leftOrBottom].x = result[(u32)Patch::k_centre].x - halfCentre.x - halfLeft.x;
                result[(u32)Patch::k_rightOrTop].x = result[(u32)Patch::k_centre].x + halfCentre.x + halfRight.x;
                
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
            std::array<Core::Vector2, ThreePatchDrawable::k_numPatches> CalculateThreePatchPositionsVertical(const std::array<Core::Vector2, ThreePatchDrawable::k_numPatches>& in_sizes)
            {
                std::array<Core::Vector2, ThreePatchDrawable::k_numPatches> result;
                
                Core::Vector2 halfCentre = in_sizes[(u32)Patch::k_centre] * 0.5f;
                Core::Vector2 halfBottom  = in_sizes[(u32)Patch::k_leftOrBottom] * 0.5f;
                Core::Vector2 halfTop = in_sizes[(u32)Patch::k_rightOrTop] * 0.5f;
                
                result[(u32)Patch::k_centre] = Core::Vector2(0.0f, halfBottom.y - halfTop.y);
                result[(u32)Patch::k_leftOrBottom].y = result[(u32)Patch::k_centre].y - halfCentre.y - halfBottom.y;
                result[(u32)Patch::k_rightOrTop].y = result[(u32)Patch::k_centre].y + halfCentre.y + halfTop.y;
                
                return result;
            }
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        ThreePatchDrawable::ThreePatchDrawable(Direction in_direction)
        {
            switch (in_direction)
            {
                case Direction::k_horizontal:
                    m_uvCalculationDelegate = CalculateThreePatchUVsHorizontal;
                    m_sizeCalculationDelegate = CalculateThreePatchSizesHorizontal;
                    m_positionCalculationDelegate = CalculateThreePatchPositionsHorizontal;
                    break;
                case Direction::k_vertical:
                    m_uvCalculationDelegate = CalculateThreePatchUVsVertical;
                    m_sizeCalculationDelegate = CalculateThreePatchSizesVertical;
                    m_positionCalculationDelegate = CalculateThreePatchPositionsVertical;
                    break;
            }
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        ThreePatchDrawable::ThreePatchDrawable(const PropertyMap& in_properties)
        {
            std::string direction(in_properties.GetProperty<std::string>("Direction"));
            
            if(direction == "Horizontal")
            {
                m_uvCalculationDelegate = CalculateThreePatchUVsHorizontal;
                m_sizeCalculationDelegate = CalculateThreePatchSizesHorizontal;
                m_positionCalculationDelegate = CalculateThreePatchPositionsHorizontal;
            }
            else if(direction == "Vertical")
            {
                m_uvCalculationDelegate = CalculateThreePatchUVsVertical;
                m_sizeCalculationDelegate = CalculateThreePatchSizesVertical;
                m_positionCalculationDelegate = CalculateThreePatchPositionsVertical;
            }
            else
            {
                CS_LOG_FATAL("ThreePatchDrawable: Unknown type: " + direction);
            }
            
            Core::Vector4 uvs(in_properties.GetProperty<Core::Vector4>("UVs"));
            SetUVs(Rendering::UVs(uvs.x, uvs.y, uvs.z, uvs.w));
            
            Core::Vector2 insets(in_properties.GetProperty<Core::Vector2>("Insets"));
            SetInsets(insets.x, insets.y);
            
            std::string location(in_properties.GetProperty<std::string>("TextureLocation"));
            std::string path(in_properties.GetProperty<std::string>("TexturePath"));
            
            if(location.empty() == false && path.empty() == false)
            {
                auto resPool = Core::Application::Get()->GetResourcePool();
                m_texture = resPool->LoadResource<Rendering::Texture>(Core::ParseStorageLocation(location), path);
            }
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        std::vector<PropertyMap::PropertyDesc> ThreePatchDrawable::GetPropertyDescs()
        {
            return g_propertyDescs;
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
            CS_ASSERT(in_leftOrBottom > 0.0f && in_rightOrTop > 0.0f, "Insets must be greater than 0");
            CS_ASSERT(in_leftOrBottom + in_rightOrTop <= 1.0f, "Insets must not overlap i.e. sum to more than 1");
            
            m_leftOrBottomInset = in_leftOrBottom;
            m_rightOrTopInset = in_rightOrTop;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        Core::Vector2 ThreePatchDrawable::GetPreferredSize() const
        {
            CS_ASSERT(m_texture != nullptr, "ThreePatchDrawable cannot get preferred size without texture");
            return Core::Vector2((f32)m_texture->GetWidth() * m_UVs.m_s, (f32)m_texture->GetHeight() * m_UVs.m_t);
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void ThreePatchDrawable::Draw(Rendering::CanvasRenderer* in_renderer, const Core::Matrix3& in_transform, const Core::Vector2& in_absSize, const Core::Colour& in_absColour)
        {
            CS_ASSERT(m_texture != nullptr, "ThreePatchDrawable cannot draw without texture");
            
            auto uvs = m_uvCalculationDelegate(m_UVs, m_leftOrBottomInset, m_rightOrTopInset);
            auto sizes = m_sizeCalculationDelegate(in_absSize, GetPreferredSize(), m_leftOrBottomInset, m_rightOrTopInset);
            auto positions = m_positionCalculationDelegate(sizes);
            
            for(u32 i=0; i<k_numPatches; ++i)
            {
                Core::Matrix3 patchTransform = Core::Matrix3::CreateTranslation(positions[i]);
                in_renderer->DrawBox(patchTransform * in_transform, sizes[i], Core::Vector2::k_zero, m_texture, uvs[i], in_absColour, Rendering::AlignmentAnchor::k_middleCentre);
            }
        }
    }
}