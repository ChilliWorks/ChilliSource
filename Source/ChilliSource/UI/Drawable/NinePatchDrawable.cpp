//
//  NinePatchDrawable.cpp
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

#include <ChilliSource/UI/Drawable/NinePatchDrawable.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Math/MathUtils.h>
#include <ChilliSource/Core/Resource/ResourcePool.h>
#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/Rendering/Base/AspectRatioUtils.h>
#include <ChilliSource/Rendering/Base/CanvasRenderer.h>
#include <ChilliSource/Rendering/Texture/Texture.h>
#include <ChilliSource/UI/Drawable/DrawableUtils.h>

namespace ChilliSource
{
    namespace UI
    {
        namespace
        {
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
            /// @param Image frame
            /// @param Left inset as normalised percentage
            /// @param Right inset as normalised percentage
            /// @param Top inset as normalised percentage
            /// @param Bottom inset as normalised percentage
            ///
            /// @return UVs for the 9 patches
            //----------------------------------------------------------------------------------------
            std::array<Rendering::UVs, NinePatchDrawable::k_numPatches> CalculateNinePatchUVs(const Rendering::TextureAtlas::Frame& in_frame, f32 in_left, f32 in_right, f32 in_top, f32 in_bottom)
            {
                std::array<Rendering::UVs, NinePatchDrawable::k_numPatches> result;
                
                f32 uvWidth = (in_frame.m_originalSize.x/in_frame.m_croppedSize.x) * in_frame.m_uvs.m_s;
                
                f32 leftU = in_frame.m_uvs.m_u - uvWidth + in_frame.m_uvs.m_s;
                f32 middleU = leftU + (uvWidth * in_left);
                f32 rightU = middleU + (uvWidth * (1.0f - in_right - in_left));
                
                f32 leftUS = middleU;
                f32 middleUS = rightU;
                f32 rightUS = leftU + uvWidth;

                f32 uvHeight = (in_frame.m_originalSize.y/in_frame.m_croppedSize.y) * in_frame.m_uvs.m_t;
                
                f32 topV = in_frame.m_uvs.m_v - uvHeight + in_frame.m_uvs.m_t;
                f32 middleV = topV + (uvHeight * in_top);
                f32 bottomV = middleV + (uvHeight * (1.0f - in_top - in_bottom));
                
                f32 topVT = middleV;
                f32 middleVT = bottomV;
                f32 bottomVT = topV + uvHeight;
                
                leftU = std::max(leftU, in_frame.m_uvs.m_u);
                middleU = std::max(middleU, in_frame.m_uvs.m_u);
                rightU = std::max(rightU, in_frame.m_uvs.m_u);
                
                topV = std::max(topV, in_frame.m_uvs.m_v);
                middleV = std::max(middleV, in_frame.m_uvs.m_v);
                bottomV = std::max(bottomV, in_frame.m_uvs.m_v);
                
                leftUS = std::min(leftUS, in_frame.m_uvs.m_u + in_frame.m_uvs.m_s);
                middleUS = std::min(middleUS, in_frame.m_uvs.m_u + in_frame.m_uvs.m_s);
                rightUS = std::min(rightUS, in_frame.m_uvs.m_u + in_frame.m_uvs.m_s);
                
                topVT = std::min(topVT, in_frame.m_uvs.m_v + in_frame.m_uvs.m_t);
                middleVT = std::min(middleVT, in_frame.m_uvs.m_v + in_frame.m_uvs.m_t);
                bottomVT = std::min(bottomVT, in_frame.m_uvs.m_v + in_frame.m_uvs.m_t);
                
                f32 leftS = leftUS - leftU;
                f32 middleS = middleUS - middleU;
                f32 rightS = rightUS - rightU;
                
                f32 topT = topVT - topV;
                f32 middleT = middleVT - middleV;
                f32 bottomT = bottomVT - bottomV;
                
                result[(u32)Patch::k_topLeft].m_u = leftU;
                result[(u32)Patch::k_topLeft].m_s = leftS;
                result[(u32)Patch::k_middleLeft].m_u = leftU;
                result[(u32)Patch::k_middleLeft].m_s = leftS;
                result[(u32)Patch::k_bottomLeft].m_u = leftU;
                result[(u32)Patch::k_bottomLeft].m_s = leftS;
                
                result[(u32)Patch::k_topCentre].m_u = middleU;
                result[(u32)Patch::k_topCentre].m_s = middleS;
                result[(u32)Patch::k_middleCentre].m_u = middleU;
                result[(u32)Patch::k_middleCentre].m_s = middleS;
                result[(u32)Patch::k_bottomCentre].m_u = middleU;
                result[(u32)Patch::k_bottomCentre].m_s = middleS;
                
                result[(u32)Patch::k_topRight].m_u = rightU;
                result[(u32)Patch::k_topRight].m_s = rightS;
                result[(u32)Patch::k_middleRight].m_u = rightU;
                result[(u32)Patch::k_middleRight].m_s = rightS;
                result[(u32)Patch::k_bottomRight].m_u = rightU;
                result[(u32)Patch::k_bottomRight].m_s = rightS;
                
                result[(u32)Patch::k_topLeft].m_v = topV;
                result[(u32)Patch::k_topLeft].m_t = topT;
                result[(u32)Patch::k_topCentre].m_v = topV;
                result[(u32)Patch::k_topCentre].m_t = topT;
                result[(u32)Patch::k_topRight].m_v = topV;
                result[(u32)Patch::k_topRight].m_t = topT;
                
                result[(u32)Patch::k_middleLeft].m_v = middleV;
                result[(u32)Patch::k_middleLeft].m_t = middleT;
                result[(u32)Patch::k_middleCentre].m_v = middleV;
                result[(u32)Patch::k_middleCentre].m_t = middleT;
                result[(u32)Patch::k_middleRight].m_v = middleV;
                result[(u32)Patch::k_middleRight].m_t = middleT;
                
                result[(u32)Patch::k_bottomLeft].m_v = bottomV;
                result[(u32)Patch::k_bottomLeft].m_t = bottomT;
                result[(u32)Patch::k_bottomCentre].m_v = bottomV;
                result[(u32)Patch::k_bottomCentre].m_t = bottomT;
                result[(u32)Patch::k_bottomRight].m_v = bottomV;
                result[(u32)Patch::k_bottomRight].m_t = bottomT;
                
                return result;
            }
            //----------------------------------------------------------------------------------------
            /// Based on the widget size and the given insets calculate the absolute size of each patch
            ///
            /// @author S Downie
            ///
            /// @param Widget absolute size
            /// @param Image frame
            /// @param Left inset as normalised percentage
            /// @param Right inset as normalised percentage
            /// @param Top inset as normalised percentage
            /// @param Bottom inset as normalised percentage
            ///
            /// @return Sizes for the 9 patches
            //----------------------------------------------------------------------------------------
            std::array<Core::Vector2, NinePatchDrawable::k_numPatches> CalculateNinePatchSizes(const Core::Vector2& in_widgetSize, const Rendering::TextureAtlas::Frame& in_frame, f32 in_left, f32 in_right, f32 in_top, f32 in_bottom)
            {
                std::array<Core::Vector2, NinePatchDrawable::k_numPatches> result;
                
                f32 widthImageLeft = in_frame.m_originalSize.x * in_left;
                f32 widthVisibleLeft = Core::MathUtils::Clamp(-in_frame.m_offset.x + widthImageLeft, 0.0f, in_frame.m_croppedSize.x);
            
                f32 offsetRight = -(in_frame.m_originalSize.x - (in_frame.m_offset.x + in_frame.m_croppedSize.x));
                f32 widthImageRight = in_frame.m_originalSize.x * in_right;
                f32 widthVisibleRight = Core::MathUtils::Clamp(offsetRight + widthImageRight, 0.0f, in_frame.m_croppedSize.x);
                
                f32 heightImageTop = in_frame.m_originalSize.y * in_top;
                f32 heightVisibleTop = Core::MathUtils::Clamp(-in_frame.m_offset.y + heightImageTop, 0.0f, in_frame.m_croppedSize.y);
                
                f32 offsetBottom = -(in_frame.m_originalSize.y - (in_frame.m_offset.y + in_frame.m_croppedSize.y));
                f32 heightImageBottom = in_frame.m_originalSize.y * in_bottom;
                f32 heightVisibleBottom = Core::MathUtils::Clamp(offsetBottom + heightImageBottom, 0.0f, in_frame.m_croppedSize.y);
                
                //If the fixed dimensions are too big to fit in the widget then we need to downscale them
                f32 heightTotal = heightVisibleTop + heightVisibleBottom;
                if(heightTotal > in_widgetSize.y)
                {
                    f32 scale = in_widgetSize.y/heightTotal;
                    heightVisibleTop *= scale;
                    heightVisibleBottom *= scale;
                }
                f32 widthTotal = widthVisibleLeft + widthVisibleRight;
                if(widthTotal > in_widgetSize.x)
                {
                    f32 scale = in_widgetSize.x/widthTotal;
                    widthVisibleLeft *= scale;
                    widthVisibleRight *= scale;
                }
                
                f32 widthImageCentre = in_frame.m_originalSize.x * (1.0f - in_right - in_left);
                f32 widthVisibleCentre = Core::MathUtils::Clamp(widthImageCentre - (in_frame.m_offset.x + widthVisibleLeft - widthImageLeft), 0.0f, in_frame.m_croppedSize.x);
                f32 widthStretched = in_widgetSize.x - widthVisibleLeft - widthVisibleRight;
                f32 widthWidgetCentre = widthVisibleCentre * (widthStretched/widthImageCentre);
                
                f32 heightImageCentre = in_frame.m_originalSize.y * (1.0f - in_bottom - in_top);
                f32 heightVisibleCentre = Core::MathUtils::Clamp(heightImageCentre - (in_frame.m_offset.y + heightVisibleTop - heightImageTop), 0.0f, in_frame.m_croppedSize.y);
                f32 heightStretched = in_widgetSize.y - heightVisibleTop - heightVisibleBottom;
                f32 heightWidgetCentre = heightVisibleCentre * (heightStretched/heightImageCentre);
                
                result[(u32)Patch::k_topLeft].x = widthVisibleLeft;
                result[(u32)Patch::k_topLeft].y = heightVisibleTop;
                result[(u32)Patch::k_topRight].x = widthVisibleRight;
                result[(u32)Patch::k_topRight].y = heightVisibleTop;
                result[(u32)Patch::k_bottomLeft].x = widthVisibleLeft;
                result[(u32)Patch::k_bottomLeft].y = heightVisibleBottom;
                result[(u32)Patch::k_bottomRight].x = widthVisibleRight;
                result[(u32)Patch::k_bottomRight].y = heightVisibleBottom;
                result[(u32)Patch::k_middleLeft].x = widthVisibleLeft;
                result[(u32)Patch::k_middleRight].x = widthVisibleRight;
                result[(u32)Patch::k_topCentre].y = heightVisibleTop;
                result[(u32)Patch::k_bottomCentre].y = heightVisibleBottom;
                result[(u32)Patch::k_topCentre].x = widthWidgetCentre;
                result[(u32)Patch::k_bottomCentre].x = widthWidgetCentre;
                result[(u32)Patch::k_middleCentre].x = widthWidgetCentre;
                result[(u32)Patch::k_middleCentre].y = heightWidgetCentre;
                result[(u32)Patch::k_middleLeft].y = heightWidgetCentre;
                result[(u32)Patch::k_middleRight].y = heightWidgetCentre;

                return result;
            }
            //----------------------------------------------------------------------------------------
            /// Calculate the local space positions for each patch with the origin of the overall
            /// widget at the top left
            ///
            /// @author S Downie
            ///
            /// @param Widget size
            /// @param Patch sizes
            ///
            /// @return Sizes for the 9 patches
            //----------------------------------------------------------------------------------------
            std::array<Core::Vector2, NinePatchDrawable::k_numPatches> CalculateNinePatchPositions(const Core::Vector2& in_widgetSize, const std::array<Core::Vector2, NinePatchDrawable::k_numPatches>& in_sizes)
            {
                std::array<Core::Vector2, NinePatchDrawable::k_numPatches> result;
                
                Core::Vector2 halfWidgetSize = in_widgetSize * 0.5f;
                Core::Vector2 halfMiddleCentre = in_sizes[(u32)Patch::k_middleCentre] * 0.5f;
                Core::Vector2 halfMiddleLeft = in_sizes[(u32)Patch::k_middleLeft] * 0.5f;
                Core::Vector2 halfMiddleRight = in_sizes[(u32)Patch::k_middleRight] * 0.5f;
                Core::Vector2 halfTopCentre = in_sizes[(u32)Patch::k_topCentre] * 0.5f;
                Core::Vector2 halfTopLeft = in_sizes[(u32)Patch::k_topLeft] * 0.5f;
                Core::Vector2 halfTopRight = in_sizes[(u32)Patch::k_topRight] * 0.5f;
                Core::Vector2 halfBottomCentre = in_sizes[(u32)Patch::k_bottomCentre] * 0.5f;
                Core::Vector2 halfBottomLeft = in_sizes[(u32)Patch::k_bottomLeft] * 0.5f;
                Core::Vector2 halfBottomRight = in_sizes[(u32)Patch::k_bottomRight] * 0.5f;
                
                result[(u32)Patch::k_topLeft].x = -halfWidgetSize.x + halfTopLeft.x;
                result[(u32)Patch::k_topLeft].y = halfWidgetSize.y - halfTopLeft.y;
                
                result[(u32)Patch::k_middleLeft].x = -halfWidgetSize.x + halfMiddleLeft.x;
                result[(u32)Patch::k_middleLeft].y = result[(u32)Patch::k_topLeft].y - halfTopLeft.y - halfMiddleLeft.y;
                
                result[(u32)Patch::k_bottomLeft].x = -halfWidgetSize.x + halfBottomLeft.x;
                result[(u32)Patch::k_bottomLeft].y = result[(u32)Patch::k_middleLeft].y - halfMiddleLeft.y - halfBottomLeft.y;
                
                result[(u32)Patch::k_topCentre].x = result[(u32)Patch::k_topLeft].x + halfTopLeft.x + halfTopCentre.x;
                result[(u32)Patch::k_topCentre].y = halfWidgetSize.y - halfTopCentre.y;
                
                result[(u32)Patch::k_middleCentre].x = result[(u32)Patch::k_middleLeft].x + halfMiddleLeft.x + halfMiddleCentre.x;
                result[(u32)Patch::k_middleCentre].y = result[(u32)Patch::k_topCentre].y - halfTopCentre.y - halfMiddleCentre.y;
                
                result[(u32)Patch::k_bottomCentre].x = result[(u32)Patch::k_bottomLeft].x + halfBottomLeft.x + halfBottomCentre.x;
                result[(u32)Patch::k_bottomCentre].y = result[(u32)Patch::k_middleCentre].y - halfMiddleCentre.y - halfBottomCentre.y;
                
                result[(u32)Patch::k_topRight].x = result[(u32)Patch::k_topCentre].x + halfTopCentre.x + halfTopRight.x;
                result[(u32)Patch::k_topRight].y = halfWidgetSize.y - halfTopRight.y;
                
                result[(u32)Patch::k_middleRight].x = result[(u32)Patch::k_middleCentre].x + halfMiddleCentre.x + halfMiddleRight.x;
                result[(u32)Patch::k_middleRight].y = result[(u32)Patch::k_topRight].y - halfTopRight.y - halfMiddleRight.y;
                
                result[(u32)Patch::k_bottomRight].x = result[(u32)Patch::k_bottomCentre].x + halfBottomCentre.x + halfBottomRight.x;
                result[(u32)Patch::k_bottomRight].y = result[(u32)Patch::k_middleRight].y - halfMiddleRight.y - halfBottomRight.y;
                
                return result;
            }
            //----------------------------------------------------------------------------------------
            /// Calculate the offset for the patches that will position them as if they still had
            /// their cropped space
            ///
            /// @author S Downie
            ///
            /// @param Widget absolute size
            /// @param Image frame
            /// @param Left inset as normalised percentage
            /// @param Right inset as normalised percentage
            /// @param Top inset as normalised percentage
            /// @param Bottom inset as normalised percentage
            ///
            /// @return Offset from top left
            //----------------------------------------------------------------------------------------
            Core::Vector2 CalculatePatchesOffset(const Core::Vector2& in_widgetSize, const Rendering::TextureAtlas::Frame& in_frame, f32 in_left, f32 in_right, f32 in_top, f32 in_bottom)
            {
                Core::Vector2 result;
                
                f32 leftImageEnd = in_frame.m_originalSize.x * in_left;
                f32 leftImageWidth = leftImageEnd;
                f32 rightImageWidth = in_frame.m_originalSize.x * in_right;
                f32 centreImageEnd = in_frame.m_originalSize.x * (1.0f - in_right);
                
                //Left patch
                f32 offsetLeftImage = std::min(in_frame.m_offset.x, leftImageWidth);
                result.x += offsetLeftImage;
                
                if(in_frame.m_offset.x > leftImageEnd)
                {
                    //Centre patch
                    f32 centreWidgetWidthStretched = in_widgetSize.x - leftImageWidth - rightImageWidth;
                    f32 centreImageWidth = centreImageEnd - leftImageEnd;
                    f32 centreOffset = std::min(in_frame.m_offset.x - leftImageWidth, centreImageWidth);
                    f32 centreOffsetWidget = (centreOffset/centreImageWidth) * centreWidgetWidthStretched;
                    
                    result.x += std::min(centreOffsetWidget, centreWidgetWidthStretched);
                }
                if(in_frame.m_offset.x > centreImageEnd)
                {
                    //Right patch
                    f32 rightOffsetImage = in_frame.m_offset.x - centreImageEnd;
                    result.x += rightOffsetImage;
                }
                

                f32 topImageEnd = in_frame.m_originalSize.y * in_top;
                f32 topImageHeight = topImageEnd;
                f32 bottomImageHeight = in_frame.m_originalSize.y * in_bottom;
                f32 middleImageEnd = in_frame.m_originalSize.y * (1.0f - in_bottom);
                
                //Top patch
                f32 offsetTopImage = std::min(in_frame.m_offset.y, topImageHeight);
                result.y -= offsetTopImage;
                
                if(in_frame.m_offset.y > topImageEnd)
                {
                    //Middle patch
                    f32 centreWidgetHeightStretched = in_widgetSize.y - topImageHeight - bottomImageHeight;
                    f32 centreImageHeight = middleImageEnd - topImageEnd;
                    f32 centreOffset = std::min(in_frame.m_offset.y - topImageHeight, centreImageHeight);
                    f32 centreOffsetWidget = (centreOffset/centreImageHeight) * centreWidgetHeightStretched;
                    
                    result.y -= std::min(centreOffsetWidget, centreWidgetHeightStretched);
                }
                if(in_frame.m_offset.y > middleImageEnd)
                {
                    //Bottom patch
                    f32 bottomOffsetImage = in_frame.m_offset.y - middleImageEnd;
                    result.y -= bottomOffsetImage;
                }
                
                return result;
            }
        }
        
        CS_DEFINE_NAMEDTYPE(NinePatchDrawable);
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        NinePatchDrawable::NinePatchDrawable(const Rendering::TextureCSPtr& in_texture, f32 in_leftInset, f32 in_rightInset, f32 in_topInset, f32 in_bottomInset)
        {
            SetTexture(in_texture);
            SetInsets(in_leftInset, in_rightInset, in_topInset, in_bottomInset);
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        NinePatchDrawable::NinePatchDrawable(const Rendering::TextureCSPtr& in_texture, const Rendering::TextureAtlasCSPtr& in_atlas, const std::string& in_atlasId, f32 in_leftInset, f32 in_rightInset,
                                             f32 in_topInset, f32 in_bottomInset)
        {
            CS_ASSERT(in_atlas != nullptr, "Texture atlas cannot be null.");
            CS_ASSERT(in_atlas->HasFrameWithId(in_atlasId) == true, "Texture atlas must contain provided Id.");
            
            SetTexture(in_texture);
            SetTextureAtlas(in_atlas);
            SetTextureAtlasId(in_atlasId);
            SetInsets(in_leftInset, in_rightInset, in_topInset, in_bottomInset);
        }
                //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        bool NinePatchDrawable::IsA(Core::InterfaceIDType in_interfaceId) const
        {
            return (Drawable::InterfaceID == in_interfaceId || NinePatchDrawable::InterfaceID == in_interfaceId);
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void NinePatchDrawable::SetTexture(const Rendering::TextureCSPtr& in_texture)
        {
            CS_ASSERT(in_texture != nullptr, "Texture cannot be null in a nine-patch drawable.");
            
            m_texture = in_texture;
            
            m_atlasFrame = DrawableUtils::BuildFrame(m_texture.get(), m_atlas.get(), m_atlasId, m_uvs);
            m_isPatchCatchValid = false;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void NinePatchDrawable::SetTextureAtlas(const Rendering::TextureAtlasCSPtr& in_atlas)
        {
            m_atlas = in_atlas;
            m_atlasId = "";
            
            m_atlasFrame = DrawableUtils::BuildFrame(m_texture.get(), m_atlas.get(), m_atlasId, m_uvs);
            m_isPatchCatchValid = false;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void NinePatchDrawable::SetTextureAtlasId(const std::string& in_atlasId)
        {
            CS_ASSERT(m_atlas != nullptr, "NinePatchDrawable::SetTextureAtlasId: Atlas Id cannot be set without first setting an atlas");
            
            m_atlasId = in_atlasId;
            
            m_atlasFrame = DrawableUtils::BuildFrame(m_texture.get(), m_atlas.get(), m_atlasId, m_uvs);
            m_isPatchCatchValid = false;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void NinePatchDrawable::SetUVs(const Rendering::UVs& in_UVs)
        {
            m_uvs = in_UVs;
            
            m_atlasFrame = DrawableUtils::BuildFrame(m_texture.get(), m_atlas.get(), m_atlasId, m_uvs);
            m_isPatchCatchValid = false;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void NinePatchDrawable::SetColour(const Core::Colour& in_colour)
        {
            m_colour = in_colour;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void NinePatchDrawable::SetInsets(f32 in_left, f32 in_right, f32 in_top, f32 in_bottom)
        {
            CS_ASSERT(in_left > 0.0f && in_right > 0.0f && in_top > 0.0f && in_bottom > 0.0f, "Insets must be greater than 0");
            CS_ASSERT(in_left + in_right <= 1.0f, "Insets must not overlap i.e. sum to more than 1");
            CS_ASSERT(in_top + in_bottom <= 1.0f, "Insets must not overlap i.e. sum to more than 1");
            
            m_leftInset = in_left;
            m_rightInset = in_right;
            m_topInset = in_top;
            m_bottomInset = in_bottom;
            
            m_isPatchCatchValid = false;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        Core::Vector2 NinePatchDrawable::GetPreferredSize() const
        {
            return m_atlasFrame.m_originalSize;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void NinePatchDrawable::Draw(Rendering::CanvasRenderer* in_renderer, const Core::Matrix3& in_transform, const Core::Vector2& in_absSize, const Core::Colour& in_absColour)
        {
            CS_ASSERT(m_texture != nullptr, "NinePatchDrawable cannot draw without texture");
            
            if(m_cachedWidgetSize != in_absSize)
            {
                m_isPatchCatchValid = false;
                m_cachedWidgetSize = in_absSize;
            }
            
            if(m_isPatchCatchValid == false)
            {
                //When textures are packed into an atlas their alpha space is cropped. This functionality restores the alpha space by resizing and offsetting the patches.
                m_cachedUvs = CalculateNinePatchUVs(m_atlasFrame, m_leftInset, m_rightInset, m_topInset, m_bottomInset);
                m_cachedSizes = CalculateNinePatchSizes(in_absSize, m_atlasFrame, m_leftInset, m_rightInset, m_topInset, m_bottomInset);
                m_cachedPositions = CalculateNinePatchPositions(in_absSize, m_cachedSizes);
                m_cachedOffsetTL = CalculatePatchesOffset(in_absSize, m_atlasFrame, m_leftInset, m_rightInset, m_topInset, m_bottomInset);
                
                m_isPatchCatchValid = true;
            }
            
            for(u32 i=0; i<k_numPatches; ++i)
            {
                Core::Matrix3 patchTransform = Core::Matrix3::CreateTranslation(m_cachedPositions[i]);
                in_renderer->DrawBox(patchTransform * in_transform, m_cachedSizes[i], m_cachedOffsetTL, m_texture, m_cachedUvs[i], in_absColour * m_colour, Rendering::AlignmentAnchor::k_middleCentre);
            }
        }
    }
}