//
//  ThreePatchUIDrawable.cpp
//  ChilliSource
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

#include <ChilliSource/UI/Drawable/ThreePatchUIDrawable.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Math/MathUtils.h>
#include <ChilliSource/Core/Resource/ResourcePool.h>
#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/Rendering/Base/AlignmentAnchors.h>
#include <ChilliSource/Rendering/Base/AspectRatioUtils.h>
#include <ChilliSource/Rendering/Base/CanvasRenderer.h>
#include <ChilliSource/Rendering/Texture/Texture.h>
#include <ChilliSource/UI/Drawable/UIDrawableUtils.h>

namespace ChilliSource
{
    namespace
    {
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
        std::array<UVs, ThreePatchUIDrawable::k_numPatches> CalculateThreePatchUVsHorizontal(const TextureAtlas::Frame& in_frame, f32 in_left, f32 in_right)
        {
            std::array<UVs, ThreePatchUIDrawable::k_numPatches> result;
            
            f32 uvWidth = (in_frame.m_originalSize.x/in_frame.m_croppedSize.x) * in_frame.m_uvs.m_s;
            
            f32 leftU = in_frame.m_uvs.m_u - uvWidth + in_frame.m_uvs.m_s;
            f32 middleU = leftU + (uvWidth * in_left);
            f32 rightU = middleU + (uvWidth * (1.0f - in_right - in_left));
            
            f32 leftUS = middleU;
            f32 middleUS = rightU;
            f32 rightUS = leftU + uvWidth;
            
            f32 uvHeight = (in_frame.m_originalSize.y/in_frame.m_croppedSize.y) * in_frame.m_uvs.m_t;
            
            f32 V = in_frame.m_uvs.m_v - uvHeight + in_frame.m_uvs.m_t;
            f32 VT = V + uvHeight;
            
            leftU = std::max(leftU, in_frame.m_uvs.m_u);
            middleU = std::max(middleU, in_frame.m_uvs.m_u);
            rightU = std::max(rightU, in_frame.m_uvs.m_u);
            
            V = std::max(V, in_frame.m_uvs.m_v);
            
            leftUS = std::min(leftUS, in_frame.m_uvs.m_u + in_frame.m_uvs.m_s);
            middleUS = std::min(middleUS, in_frame.m_uvs.m_u + in_frame.m_uvs.m_s);
            rightUS = std::min(rightUS, in_frame.m_uvs.m_u + in_frame.m_uvs.m_s);
            
            VT = std::min(VT, in_frame.m_uvs.m_v + in_frame.m_uvs.m_t);
            
            f32 leftS = leftUS - leftU;
            f32 middleS = middleUS - middleU;
            f32 rightS = rightUS - rightU;
            
            f32 T = VT - V;
            
            result[(u32)Patch::k_leftOrBottom].m_u = leftU;
            result[(u32)Patch::k_leftOrBottom].m_s = leftS;
            
            result[(u32)Patch::k_centre].m_u = middleU;
            result[(u32)Patch::k_centre].m_s = middleS;
            
            result[(u32)Patch::k_rightOrTop].m_u = rightU;
            result[(u32)Patch::k_rightOrTop].m_s = rightS;
            
            result[(u32)Patch::k_rightOrTop].m_v = V;
            result[(u32)Patch::k_rightOrTop].m_t = T;
            
            result[(u32)Patch::k_centre].m_v = V;
            result[(u32)Patch::k_centre].m_t = T;
            
            result[(u32)Patch::k_leftOrBottom].m_v = V;
            result[(u32)Patch::k_leftOrBottom].m_t = T;
            
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
        std::array<UVs, ThreePatchUIDrawable::k_numPatches> CalculateThreePatchUVsVertical(const TextureAtlas::Frame& in_frame, f32 in_bottom, f32 in_top)
        {
            std::array<UVs, ThreePatchUIDrawable::k_numPatches> result;
            
            f32 uvWidth = (in_frame.m_originalSize.x/in_frame.m_croppedSize.x) * in_frame.m_uvs.m_s;
            
            f32 U = in_frame.m_uvs.m_u - uvWidth + in_frame.m_uvs.m_s;
            f32 US = U + uvWidth;
            
            f32 uvHeight = (in_frame.m_originalSize.y/in_frame.m_croppedSize.y) * in_frame.m_uvs.m_t;
            
            f32 topV = in_frame.m_uvs.m_v - uvHeight + in_frame.m_uvs.m_t;
            f32 middleV = topV + (uvHeight * in_top);
            f32 bottomV = middleV + (uvHeight * (1.0f - in_top - in_bottom));
            
            f32 topVT = middleV;
            f32 middleVT = bottomV;
            f32 bottomVT = topV + uvHeight;
            
            U = std::max(U, in_frame.m_uvs.m_u);
            
            topV = std::max(topV, in_frame.m_uvs.m_v);
            middleV = std::max(middleV, in_frame.m_uvs.m_v);
            bottomV = std::max(bottomV, in_frame.m_uvs.m_v);
            
            US = std::min(US, in_frame.m_uvs.m_u + in_frame.m_uvs.m_s);
            
            topVT = std::min(topVT, in_frame.m_uvs.m_v + in_frame.m_uvs.m_t);
            middleVT = std::min(middleVT, in_frame.m_uvs.m_v + in_frame.m_uvs.m_t);
            bottomVT = std::min(bottomVT, in_frame.m_uvs.m_v + in_frame.m_uvs.m_t);
            
            f32 S = US - U;
            
            f32 topT = topVT - topV;
            f32 middleT = middleVT - middleV;
            f32 bottomT = bottomVT - bottomV;
            
            result[(u32)Patch::k_leftOrBottom].m_u = U;
            result[(u32)Patch::k_leftOrBottom].m_s = S;
            
            result[(u32)Patch::k_centre].m_u = U;
            result[(u32)Patch::k_centre].m_s = S;
            
            result[(u32)Patch::k_rightOrTop].m_u = U;
            result[(u32)Patch::k_rightOrTop].m_s = S;
            
            result[(u32)Patch::k_rightOrTop].m_v = topV;
            result[(u32)Patch::k_rightOrTop].m_t = topT;
            
            result[(u32)Patch::k_centre].m_v = middleV;
            result[(u32)Patch::k_centre].m_t = middleT;
            
            result[(u32)Patch::k_leftOrBottom].m_v = bottomV;
            result[(u32)Patch::k_leftOrBottom].m_t = bottomT;
            
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
        std::array<Vector2, ThreePatchUIDrawable::k_numPatches> CalculateThreePatchSizesHorizontal(const Vector2& in_widgetSize, const TextureAtlas::Frame& in_frame, f32 in_left, f32 in_right)
        {
            std::array<Vector2, ThreePatchUIDrawable::k_numPatches> result;
            
            f32 height = in_widgetSize.y/in_frame.m_originalSize.y * in_frame.m_croppedSize.y;
            Vector2 widgetSizeAspectMaintained = AspectRatioUtils::KeepOriginalHeightAdaptWidth(Vector2(in_frame.m_originalSize.x, height), in_frame.m_originalSize.x/in_frame.m_originalSize.y);
            
            f32 widthImageLeft = in_frame.m_originalSize.x * in_left;
            f32 widthVisibleLeft = MathUtils::Clamp(-in_frame.m_offset.x + widthImageLeft, 0.0f, in_frame.m_croppedSize.x);

            f32 offsetRight = -(in_frame.m_originalSize.x - (in_frame.m_offset.x + in_frame.m_croppedSize.x));
            f32 widthImageRight = in_frame.m_originalSize.x * in_right;
            f32 widthVisibleRight = MathUtils::Clamp(offsetRight + widthImageRight, 0.0f, in_frame.m_croppedSize.x);
            
            f32 widthImageCentre = in_frame.m_originalSize.x * (1.0f - in_right - in_left);
            f32 widthVisibleCentre = MathUtils::Clamp(widthImageCentre - (in_frame.m_offset.x + widthVisibleLeft - widthImageLeft), 0.0f, in_frame.m_croppedSize.x);
            f32 widthStretched = in_widgetSize.x - widthVisibleLeft - widthVisibleRight;
            f32 widthWidgetCentre = widthVisibleCentre * (widthStretched/widthImageCentre);
            
            result[(u32)Patch::k_leftOrBottom].x = widthVisibleLeft;
            result[(u32)Patch::k_leftOrBottom].y = widgetSizeAspectMaintained.y;
            result[(u32)Patch::k_centre].x = widthWidgetCentre;
            result[(u32)Patch::k_centre].y = widgetSizeAspectMaintained.y;
            result[(u32)Patch::k_rightOrTop].x = widthVisibleRight;
            result[(u32)Patch::k_rightOrTop].y = widgetSizeAspectMaintained.y;
            
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
        std::array<Vector2, ThreePatchUIDrawable::k_numPatches> CalculateThreePatchSizesVertical(const Vector2& in_widgetSize, const TextureAtlas::Frame& in_frame, f32 in_bottom, f32 in_top)
        {
            std::array<Vector2, ThreePatchUIDrawable::k_numPatches> result;
            
            f32 width = in_widgetSize.x/in_frame.m_originalSize.x * in_frame.m_croppedSize.x;
            Vector2 widgetSizeAspectMaintained = AspectRatioUtils::KeepOriginalWidthAdaptHeight(Vector2(width, in_frame.m_originalSize.y), in_frame.m_originalSize.x/in_frame.m_originalSize.y);
            
            f32 heightImageTop = in_frame.m_originalSize.y * in_top;
            f32 heightVisibleTop = MathUtils::Clamp(-in_frame.m_offset.y + heightImageTop, 0.0f, in_frame.m_croppedSize.y);
            
            f32 offsetBottom = -(in_frame.m_originalSize.y - (in_frame.m_offset.y + in_frame.m_croppedSize.y));
            f32 heightImageBottom = in_frame.m_originalSize.y * in_bottom;
            f32 heightVisibleBottom = MathUtils::Clamp(offsetBottom + heightImageBottom, 0.0f, in_frame.m_croppedSize.y);
            
            f32 heightImageCentre = in_frame.m_originalSize.y * (1.0f - in_bottom - in_top);
            f32 heightVisibleCentre = MathUtils::Clamp(heightImageCentre - (in_frame.m_offset.y + heightVisibleTop - heightImageTop), 0.0f, in_frame.m_croppedSize.y);
            f32 heightStretched = in_widgetSize.y - heightVisibleTop - heightVisibleBottom;
            f32 heightWidgetCentre = heightVisibleCentre * (heightStretched/heightImageCentre);
            
            result[(u32)Patch::k_rightOrTop].y = heightVisibleTop;
            result[(u32)Patch::k_rightOrTop].x = widgetSizeAspectMaintained.x;
            result[(u32)Patch::k_centre].y = heightWidgetCentre;
            result[(u32)Patch::k_centre].x = widgetSizeAspectMaintained.x;
            result[(u32)Patch::k_leftOrBottom].y = heightVisibleBottom;
            result[(u32)Patch::k_leftOrBottom].x = widgetSizeAspectMaintained.x;
            
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
        /// @return Sizes for the 3 patches
        //----------------------------------------------------------------------------------------
        std::array<Vector2, ThreePatchUIDrawable::k_numPatches> CalculateThreePatchPositionsHorizontal(const Vector2& in_widgetSize, const std::array<Vector2, ThreePatchUIDrawable::k_numPatches>& in_sizes)
        {
            std::array<Vector2, ThreePatchUIDrawable::k_numPatches> result;
            
            Vector2 halfWidgetSize = in_widgetSize * 0.5f;
            Vector2 halfLeft = in_sizes[(u32)Patch::k_leftOrBottom] * 0.5f;
            Vector2 halfMiddle = in_sizes[(u32)Patch::k_centre] * 0.5f;
            Vector2 halfRight = in_sizes[(u32)Patch::k_rightOrTop] * 0.5f;
            
            result[(u32)Patch::k_leftOrBottom].x = -halfWidgetSize.x + halfLeft.x;
            result[(u32)Patch::k_leftOrBottom].y = halfWidgetSize.y - halfLeft.y;
            
            result[(u32)Patch::k_centre].x = result[(u32)Patch::k_leftOrBottom].x + halfLeft.x + halfMiddle.x;
            result[(u32)Patch::k_centre].y = halfWidgetSize.y - halfMiddle.y;
            
            result[(u32)Patch::k_rightOrTop].x = result[(u32)Patch::k_centre].x + halfMiddle.x + halfRight.x;
            result[(u32)Patch::k_rightOrTop].y = halfWidgetSize.y - halfRight.y;
            
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
        /// @return Sizes for the 3 patches
        //----------------------------------------------------------------------------------------
        std::array<Vector2, ThreePatchUIDrawable::k_numPatches> CalculateThreePatchPositionsVertical(const Vector2& in_widgetSize, const std::array<Vector2, ThreePatchUIDrawable::k_numPatches>& in_sizes)
        {
            std::array<Vector2, ThreePatchUIDrawable::k_numPatches> result;
            
            Vector2 halfWidgetSize = in_widgetSize * 0.5f;
            Vector2 halfTop = in_sizes[(u32)Patch::k_rightOrTop] * 0.5f;
            Vector2 halfMiddle = in_sizes[(u32)Patch::k_centre] * 0.5f;
            Vector2 halfBottom = in_sizes[(u32)Patch::k_leftOrBottom] * 0.5f;
            
            result[(u32)Patch::k_rightOrTop].x = -halfWidgetSize.x + halfTop.x;
            result[(u32)Patch::k_rightOrTop].y = halfWidgetSize.y - halfTop.y;
            
            result[(u32)Patch::k_centre].x = -halfWidgetSize.x + halfMiddle.x;
            result[(u32)Patch::k_centre].y = result[(u32)Patch::k_rightOrTop].y - halfTop.y - halfMiddle.y;
            
            result[(u32)Patch::k_leftOrBottom].x = -halfWidgetSize.x + halfBottom.x;
            result[(u32)Patch::k_leftOrBottom].y = result[(u32)Patch::k_centre].y - halfMiddle.y - halfBottom.y;
            
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
        ///
        /// @return Offset from top left
        //----------------------------------------------------------------------------------------
        Vector2 CalculateThreePatchOffsetHorizontal(const Vector2& in_widgetSize, const TextureAtlas::Frame& in_frame, f32 in_left, f32 in_right)
        {
            Vector2 result;
            
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
            
            result.y = -(in_frame.m_offset.y/in_frame.m_originalSize.y) * in_widgetSize.y;
            
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
        /// @param Top inset as normalised percentage
        /// @param Bottom inset as normalised percentage
        ///
        /// @return Offset from top left
        //----------------------------------------------------------------------------------------
        Vector2 CalculateThreePatchOffsetVertical(const Vector2& in_widgetSize, const TextureAtlas::Frame& in_frame, f32 in_top, f32 in_bottom)
        {
            Vector2 result;
            
            result.x = (in_frame.m_offset.x/in_frame.m_originalSize.x) * in_widgetSize.x;
            
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
    
    CS_DEFINE_NAMEDTYPE(ThreePatchUIDrawable);
    //----------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------
    ThreePatchUIDrawable::ThreePatchUIDrawable(const TextureCSPtr& in_texture, Direction in_direction, f32 in_leftOrBottom, f32 in_rightOrTop)
    {
        switch (in_direction)
        {
            case Direction::k_horizontal:
                m_uvCalculationDelegate = CalculateThreePatchUVsHorizontal;
                m_sizeCalculationDelegate = CalculateThreePatchSizesHorizontal;
                m_positionCalculationDelegate = CalculateThreePatchPositionsHorizontal;
                m_offsetCalculationDelegate = CalculateThreePatchOffsetHorizontal;
                break;
            case Direction::k_vertical:
                m_uvCalculationDelegate = CalculateThreePatchUVsVertical;
                m_sizeCalculationDelegate = CalculateThreePatchSizesVertical;
                m_positionCalculationDelegate = CalculateThreePatchPositionsVertical;
                m_offsetCalculationDelegate = CalculateThreePatchOffsetVertical;
                break;
        }
        
        SetTexture(in_texture);
        SetInsets(in_leftOrBottom, in_rightOrTop);
    }
    //----------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------
    ThreePatchUIDrawable::ThreePatchUIDrawable(const TextureCSPtr& in_texture, const TextureAtlasCSPtr& in_atlas, const std::string& in_atlasId, Direction in_direction,
                                           f32 in_leftOrBottom, f32 in_rightOrTop)
    {
        CS_ASSERT(in_atlas != nullptr, "The given texture atlas cannot be null.");
        CS_ASSERT(in_atlas->HasFrameWithId(in_atlasId) == true, "The given atlas Id must exist in the texture atlas.");
        
        switch (in_direction)
        {
            case Direction::k_horizontal:
                m_uvCalculationDelegate = CalculateThreePatchUVsHorizontal;
                m_sizeCalculationDelegate = CalculateThreePatchSizesHorizontal;
                m_positionCalculationDelegate = CalculateThreePatchPositionsHorizontal;
                m_offsetCalculationDelegate = CalculateThreePatchOffsetHorizontal;
                break;
            case Direction::k_vertical:
                m_uvCalculationDelegate = CalculateThreePatchUVsVertical;
                m_sizeCalculationDelegate = CalculateThreePatchSizesVertical;
                m_positionCalculationDelegate = CalculateThreePatchPositionsVertical;
                m_offsetCalculationDelegate = CalculateThreePatchOffsetVertical;
                break;
        }
        
        SetTexture(in_texture);
        SetTextureAtlas(in_atlas);
        SetTextureAtlasId(in_atlasId);
        SetInsets(in_leftOrBottom, in_rightOrTop);
    }
    
    //----------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------
    bool ThreePatchUIDrawable::IsA(InterfaceIDType in_interfaceId) const
    {
        return (UIDrawable::InterfaceID == in_interfaceId || ThreePatchUIDrawable::InterfaceID == in_interfaceId);
    }
    //----------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------
    const TextureCSPtr& ThreePatchUIDrawable::GetTexture() const
    {
        return m_texture;
    }
    //----------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------
    const TextureAtlasCSPtr& ThreePatchUIDrawable::GetTextureAtlas() const
    {
        return m_atlas;
    }
    //----------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------
    const std::string& ThreePatchUIDrawable::GetTextureAtlasId() const
    {
        return m_atlasId;
    }
    //----------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------
    const UVs& ThreePatchUIDrawable::GetUVs() const
    {
        return m_uvs;
    }
    //----------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------
    const Colour& ThreePatchUIDrawable::GetColour() const
    {
        return m_colour;
    }
    //----------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------
    void ThreePatchUIDrawable::SetTexture(const TextureCSPtr& in_texture)
    {
        CS_ASSERT(in_texture != nullptr, "Cannot set a null texture on a drawable.");
        
        m_texture = in_texture;
        
        m_atlasFrame = UIDrawableUtils::BuildFrame(m_texture.get(), m_atlas.get(), m_atlasId, m_uvs);
        m_isPatchCatchValid = false;
    }
    //----------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------
    void ThreePatchUIDrawable::SetTextureAtlas(const TextureAtlasCSPtr& in_atlas)
    {
        m_atlas = in_atlas;
        m_atlasId = "";
        
        m_atlasFrame = UIDrawableUtils::BuildFrame(m_texture.get(), m_atlas.get(), m_atlasId, m_uvs);
        m_isPatchCatchValid = false;
    }
    //----------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------
    void ThreePatchUIDrawable::SetTextureAtlasId(const std::string& in_atlasId)
    {
        CS_ASSERT(m_atlas != nullptr, "ThreePatchUIDrawable::SetTextureAtlasId: Atlas Id cannot be set without first setting an atlas");
        
        m_atlasId = in_atlasId;
        
        m_atlasFrame = UIDrawableUtils::BuildFrame(m_texture.get(), m_atlas.get(), m_atlasId, m_uvs);
        m_isPatchCatchValid = false;
    }
    //----------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------
    void ThreePatchUIDrawable::SetUVs(const UVs& in_UVs)
    {
        m_uvs = in_UVs;
        
        m_atlasFrame = UIDrawableUtils::BuildFrame(m_texture.get(), m_atlas.get(), m_atlasId, m_uvs);
        m_isPatchCatchValid = false;
    }
    //----------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------
    void ThreePatchUIDrawable::SetColour(const Colour& in_colour)
    {
        m_colour = in_colour;
    }
    //----------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------
    void ThreePatchUIDrawable::SetInsets(f32 in_leftOrBottom, f32 in_rightOrTop)
    {
        CS_ASSERT(in_leftOrBottom > 0.0f && in_rightOrTop > 0.0f, "Insets must be greater than 0");
        CS_ASSERT(in_leftOrBottom + in_rightOrTop <= 1.0f, "Insets must not overlap i.e. sum to more than 1");
        
        m_leftOrBottomInset = in_leftOrBottom;
        m_rightOrTopInset = in_rightOrTop;
        
        m_isPatchCatchValid = false;
    }
    //----------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------
    Vector2 ThreePatchUIDrawable::GetPreferredSize() const
    {
        return m_atlasFrame.m_originalSize;
    }
    //----------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------
    void ThreePatchUIDrawable::UpdatePatchCache(const Vector2& absSize)
    {
        if(m_cachedWidgetSize != absSize)
        {
            m_isPatchCatchValid = false;
            m_cachedWidgetSize = absSize;
        }
        
        if(m_isPatchCatchValid == false)
        {
            //When textures are packed into an atlas their alpha space is cropped. This functionality restores the alpha space by resizing and offsetting the patches.
            m_cachedUvs = m_uvCalculationDelegate(m_atlasFrame, m_leftOrBottomInset, m_rightOrTopInset);
            m_cachedSizes = m_sizeCalculationDelegate(absSize, m_atlasFrame, m_leftOrBottomInset, m_rightOrTopInset);
            m_cachedPositions = m_positionCalculationDelegate(absSize, m_cachedSizes);
            m_cachedOffsetTL = m_offsetCalculationDelegate(absSize, m_atlasFrame, m_leftOrBottomInset, m_rightOrTopInset);
        }
    }
    //----------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------
    void ThreePatchUIDrawable::Draw(CanvasRenderer* renderer, const Matrix3& transform, const Vector2& absSize, const Colour& absColour, CanvasDrawMode drawMode) noexcept
    {
        CS_ASSERT(m_texture != nullptr, "ThreePatchUIDrawable cannot draw without texture");
        
        UpdatePatchCache(absSize);

        for(u32 i=0; i<k_numPatches; ++i)
        {
            Matrix3 patchTransform = Matrix3::CreateTranslation(m_cachedPositions[i]);
            renderer->DrawBox(drawMode, patchTransform * transform, m_cachedSizes[i], m_cachedOffsetTL, m_texture, m_cachedUvs[i], absColour * m_colour, AlignmentAnchor::k_middleCentre);
        }
    }
}
