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
#include <ChilliSource/UI/Drawable/DrawableType.h>

namespace ChilliSource
{
    namespace UI
    {
        namespace
        {
            const std::vector<PropertyMap::PropertyDesc> k_propertyDescs =
            {
                {PropertyType::k_string, "Type", "ThreePatch"},
                {PropertyType::k_string, "Direction", "Horizontal"},
                {PropertyType::k_vec4, "UVs", "0 0 1 1"},
                {PropertyType::k_vec2, "Insets", "0.01 0.01"},
                {PropertyType::k_string, "TextureLocation", "Package"},
                {PropertyType::k_string, "TexturePath", ""},
                {PropertyType::k_string, "AtlasLocation", "Package"},
                {PropertyType::k_string, "AtlasPath", ""},
                {PropertyType::k_string, "AtlasId", ""}
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
            /// @param Image frame
            /// @param Left inset as normalised percentage
            /// @param Right inset as normalised percentage
            ///
            /// @return UVs for the 3 patches
            //----------------------------------------------------------------------------------------
            std::array<Rendering::UVs, ThreePatchDrawable::k_numPatches> CalculateThreePatchUVsHorizontal(const Rendering::TextureAtlas::Frame& in_frame, f32 in_left, f32 in_right)
            {
                std::array<Rendering::UVs, ThreePatchDrawable::k_numPatches> result;
                
                //Fixed
                f32 offsetLeft = -in_frame.m_offset.x/in_frame.m_originalSize.x;
                f32 leftU = std::max(in_frame.m_uvs.m_u + offsetLeft, in_frame.m_uvs.m_u);
                f32 leftS = std::max(offsetLeft + (in_frame.m_uvs.m_s * in_left), 0.0f);
                result[(u32)Patch::k_leftOrBottom].m_u = leftU;
                result[(u32)Patch::k_leftOrBottom].m_s = leftS;
                result[(u32)Patch::k_leftOrBottom].m_v = in_frame.m_uvs.m_v;
                result[(u32)Patch::k_leftOrBottom].m_t = in_frame.m_uvs.m_t;
                
                f32 offsetRight = (in_frame.m_originalSize.x - (in_frame.m_offset.x + in_frame.m_croppedSize.x))/in_frame.m_originalSize.x;
                f32 rightS = std::max(offsetRight + (in_frame.m_uvs.m_s * in_right), 0.0f);
                f32 rightU = std::min(in_frame.m_uvs.m_u + in_frame.m_uvs.m_s - rightS, in_frame.m_uvs.m_u + in_frame.m_uvs.m_s);
                result[(u32)Patch::k_rightOrTop].m_u = rightU;
                result[(u32)Patch::k_rightOrTop].m_s = rightS;
                result[(u32)Patch::k_rightOrTop].m_v = in_frame.m_uvs.m_v;
                result[(u32)Patch::k_rightOrTop].m_t = in_frame.m_uvs.m_t;
                
                //Stretchable
                result[(u32)Patch::k_centre].m_u = leftU + leftS;
                result[(u32)Patch::k_centre].m_s = in_frame.m_uvs.m_s - (leftS + rightS);
                result[(u32)Patch::k_centre].m_v = in_frame.m_uvs.m_v;
                result[(u32)Patch::k_centre].m_t = in_frame.m_uvs.m_t;

                return result;
            }
            //----------------------------------------------------------------------------------------
            /// Based on the insets and the given UVs calculate the UVs for each of the 3 patches
            ///
            /// @author S Downie
            ///
            /// @param Image frame
            /// @param Bottom inset as normalised percentage
            /// @param Top inset as normalised percentage
            ///
            /// @return UVs for the 3 patches
            //----------------------------------------------------------------------------------------
            std::array<Rendering::UVs, ThreePatchDrawable::k_numPatches> CalculateThreePatchUVsVertical(const Rendering::TextureAtlas::Frame& in_frame, f32 in_bottom, f32 in_top)
            {
                std::array<Rendering::UVs, ThreePatchDrawable::k_numPatches> result;
                
                //Fixed
                f32 offsetTop = -in_frame.m_offset.y/in_frame.m_originalSize.y;
                f32 topV = std::max(in_frame.m_uvs.m_v + offsetTop, in_frame.m_uvs.m_v);
                f32 topT = std::max(offsetTop + (in_frame.m_uvs.m_t * in_top), 0.0f);
                result[(u32)Patch::k_rightOrTop].m_v = topV;
                result[(u32)Patch::k_rightOrTop].m_t = topT;
                result[(u32)Patch::k_rightOrTop].m_u = in_frame.m_uvs.m_u;
                result[(u32)Patch::k_rightOrTop].m_s = in_frame.m_uvs.m_s;
                
                f32 offsetBottom = (in_frame.m_originalSize.y - (in_frame.m_offset.y + in_frame.m_croppedSize.y))/in_frame.m_originalSize.y;
                f32 bottomT = std::max(offsetBottom + (in_frame.m_uvs.m_t * in_bottom), 0.0f);
                f32 bottomV = std::min(in_frame.m_uvs.m_v + in_frame.m_uvs.m_t - bottomT, in_frame.m_uvs.m_v + in_frame.m_uvs.m_t);
                result[(u32)Patch::k_leftOrBottom].m_v = bottomV;
                result[(u32)Patch::k_leftOrBottom].m_t = bottomT;
                result[(u32)Patch::k_leftOrBottom].m_u = in_frame.m_uvs.m_u;
                result[(u32)Patch::k_leftOrBottom].m_s = in_frame.m_uvs.m_s;
                
                //Stretchable
                result[(u32)Patch::k_centre].m_v = topV + topT;
                result[(u32)Patch::k_centre].m_t = in_frame.m_uvs.m_t - (topT + bottomT);
                result[(u32)Patch::k_centre].m_u = in_frame.m_uvs.m_u;
                result[(u32)Patch::k_centre].m_s = in_frame.m_uvs.m_s;
                
                return result;
            }
            //----------------------------------------------------------------------------------------
            /// Based on the widget size and the given insets calculate the absolute size of each patch
            ///
            /// @author S Downie
            ///
            /// @param Widget absolute size
            /// @param Image absolute actual size
            /// @param Image absolute preferred size
            /// @param Image offset from top left
            /// @param Left inset as normalised percentage
            /// @param Right inset as normalised percentage
            ///
            /// @return Sizes for the 3 patches
            //----------------------------------------------------------------------------------------
            std::array<Core::Vector2, ThreePatchDrawable::k_numPatches> CalculateThreePatchSizesHorizontal(const Core::Vector2& in_widgetSize, const Rendering::TextureAtlas::Frame& in_frame, f32 in_left, f32 in_right)
            {
                std::array<Core::Vector2, ThreePatchDrawable::k_numPatches> result;
                
                f32 offsetLeft = -in_frame.m_offset.x;
                f32 widthLeft = std::max(offsetLeft + (in_frame.m_originalSize.x * in_left), 0.0f);
                result[(u32)Patch::k_leftOrBottom] = Rendering::AspectRatioUtils::KeepOriginalHeightAdaptWidth(Core::Vector2(widthLeft, in_widgetSize.y), widthLeft/in_frame.m_originalSize.y);
                
                f32 offsetRight = in_frame.m_originalSize.x - (in_frame.m_offset.x + in_frame.m_croppedSize.x);
                f32 widthRight = std::max(offsetRight + (in_frame.m_originalSize.x * in_right), 0.0f);
                result[(u32)Patch::k_rightOrTop] = Rendering::AspectRatioUtils::KeepOriginalHeightAdaptWidth(Core::Vector2(widthRight, in_widgetSize.y), widthRight/in_frame.m_originalSize.y);

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
            /// @param Image absolute actual size
            /// @param Image absolute preferred size
            /// @param Image offset from top left
            /// @param Bottom inset as normalised percentage
            /// @param Top inset as normalised percentage
            ///
            /// @return Sizes for the 3 patches
            //----------------------------------------------------------------------------------------
            std::array<Core::Vector2, ThreePatchDrawable::k_numPatches> CalculateThreePatchSizesVertical(const Core::Vector2& in_widgetSize, const Rendering::TextureAtlas::Frame& in_frame, f32 in_bottom, f32 in_top)
            {
                std::array<Core::Vector2, ThreePatchDrawable::k_numPatches> result;
                
                f32 offsetTop = -in_frame.m_offset.y;
                f32 heightTop = std::max(offsetTop + (in_frame.m_originalSize.y * in_top), 0.0f);
                result[(u32)Patch::k_rightOrTop] = Rendering::AspectRatioUtils::KeepOriginalWidthAdaptHeight(Core::Vector2(in_widgetSize.x, heightTop), in_frame.m_originalSize.x/heightTop);
                
                f32 offsetBottom = in_frame.m_originalSize.y - (in_frame.m_offset.y + in_frame.m_croppedSize.y);
                f32 heightBottom = std::max(offsetBottom + (in_frame.m_originalSize.y * in_bottom), 0.0f);
                result[(u32)Patch::k_leftOrBottom] = Rendering::AspectRatioUtils::KeepOriginalWidthAdaptHeight(Core::Vector2(in_widgetSize.x, heightBottom), in_frame.m_originalSize.x/heightBottom);
                
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
            Core::StringUtils::ToLowerCase(direction);
            
            if(direction == "horizontal")
            {
                m_uvCalculationDelegate = CalculateThreePatchUVsHorizontal;
                m_sizeCalculationDelegate = CalculateThreePatchSizesHorizontal;
                m_positionCalculationDelegate = CalculateThreePatchPositionsHorizontal;
            }
            else if(direction == "vertical")
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
            
            std::string textureLocation(in_properties.GetProperty<std::string>("TextureLocation"));
            std::string texturePath(in_properties.GetProperty<std::string>("TexturePath"));
            
            if(textureLocation.empty() == false && texturePath.empty() == false)
            {
                auto resPool = Core::Application::Get()->GetResourcePool();
                SetTexture(resPool->LoadResource<Rendering::Texture>(Core::ParseStorageLocation(textureLocation), texturePath));
            }
            
            std::string atlasLocation(in_properties.GetProperty<std::string>("AtlasLocation"));
            std::string atlasPath(in_properties.GetProperty<std::string>("AtlasPath"));
            
            if(atlasLocation.empty() == false && atlasPath.empty() == false)
            {
                auto resPool = Core::Application::Get()->GetResourcePool();
                SetTextureAtlas(resPool->LoadResource<Rendering::TextureAtlas>(Core::ParseStorageLocation(atlasLocation), atlasPath));
                SetTextureAtlasId(in_properties.GetProperty<std::string>("AtlasId"));
            }
            
            Core::Vector2 insets(in_properties.GetProperty<Core::Vector2>("Insets"));
            SetInsets(insets.x, insets.y);
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        std::vector<PropertyMap::PropertyDesc> ThreePatchDrawable::GetPropertyDescs()
        {
            return k_propertyDescs;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        DrawableType ThreePatchDrawable::GetType() const
        {
            return DrawableType::k_threePatch;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void ThreePatchDrawable::SetTexture(const Rendering::TextureCSPtr& in_texture)
        {
            m_texture = in_texture;
            Core::Vector2 texSize((f32)m_texture->GetWidth() * m_atlasFrame.m_uvs.m_s, (f32)m_texture->GetHeight() * m_atlasFrame.m_uvs.m_t);
            m_atlasFrame.m_croppedSize = texSize;
            m_atlasFrame.m_originalSize = texSize;
            m_atlasFrame.m_offset = Core::Vector2::k_zero;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void ThreePatchDrawable::SetTextureAtlas(const Rendering::TextureAtlasCSPtr& in_atlas)
        {
            m_atlas = in_atlas;
            
            if(m_atlas == nullptr)
            {
                m_atlasFrame.m_uvs = Rendering::UVs(0.0f, 0.0f, 1.0f, 1.0f);
            }
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void ThreePatchDrawable::SetTextureAtlasId(const std::string& in_atlasId)
        {
            CS_ASSERT(m_texture != nullptr &&  m_atlas != nullptr, "ThreePatchDrawable::SetTextureAtlasId: Atlas Id cannot be set without first setting an atlas and a texture");
            
            m_atlasFrame = m_atlas->GetFrame(in_atlasId);
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void ThreePatchDrawable::SetUVs(const Rendering::UVs& in_UVs)
        {
            m_atlasFrame.m_uvs = in_UVs;
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
            return m_atlasFrame.m_originalSize;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void ThreePatchDrawable::Draw(Rendering::CanvasRenderer* in_renderer, const Core::Matrix3& in_transform, const Core::Vector2& in_absSize, const Core::Colour& in_absColour)
        {
            CS_ASSERT(m_texture != nullptr, "ThreePatchDrawable cannot draw without texture");
            
            //When textures are packed into an atlas their alpha space is cropped. This functionality restores the alpha space by resizing and offseting the patches.
            Core::Vector2 size = in_absSize/m_atlasFrame.m_originalSize * m_atlasFrame.m_croppedSize;
            auto uvs = m_uvCalculationDelegate(m_atlasFrame, m_leftOrBottomInset, m_rightOrTopInset);
            auto sizes = m_sizeCalculationDelegate(size, m_atlasFrame, m_leftOrBottomInset, m_rightOrTopInset);
            auto positions = m_positionCalculationDelegate(sizes);
            
            Core::Vector2 offsetTL
            (
                (-m_atlasFrame.m_originalSize.x * 0.5f) + (m_atlasFrame.m_croppedSize.x * 0.5f) + m_atlasFrame.m_offset.x,
                (m_atlasFrame.m_originalSize.y * 0.5f) - (m_atlasFrame.m_croppedSize.y * 0.5f) - m_atlasFrame.m_offset.y
            );
            offsetTL = in_absSize/m_atlasFrame.m_originalSize * offsetTL;
            
            for(u32 i=0; i<k_numPatches; ++i)
            {
                Core::Matrix3 patchTransform = Core::Matrix3::CreateTranslation(positions[i]);
                in_renderer->DrawBox(patchTransform * in_transform, sizes[i], offsetTL, m_texture, uvs[i], in_absColour, Rendering::AlignmentAnchor::k_middleCentre);
            }
        }
    }
}