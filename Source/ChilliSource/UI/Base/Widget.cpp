//
//  Widget.cpp
//  Chilli Source
//  Created by Scott Downie on 17/04/2014.
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

#include <ChilliSource/UI/Base/Widget.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/Screen.h>
#include <ChilliSource/Rendering/Base/AlignmentAnchors.h>
#include <ChilliSource/Rendering/Base/AspectRatioUtils.h>
#include <ChilliSource/Rendering/Base/CanvasRenderer.h>

namespace ChilliSource
{
    namespace UI
    {
        namespace
        {
            std::vector<PropertyMap::PropertyDesc> g_propertyDescs =
            {
                {PropertyType::k_string, "Type", ""},
                {PropertyType::k_string, "Name", ""},
                {PropertyType::k_vec2, "RelPosition", "0 0"},
                {PropertyType::k_vec2, "AbsPosition", "0 0"},
                {PropertyType::k_vec2, "RelSize", "0 0"},
                {PropertyType::k_vec2, "AbsSize", "0 0"},
                {PropertyType::k_vec2, "PreferredSize", "1 1"},
                {PropertyType::k_vec2, "Scale", "1 1"},
                {PropertyType::k_colour, "Colour", "1 1 1 1"},
                {PropertyType::k_float, "Rotation", "0"},
                {PropertyType::k_alignmentAnchor, "OriginAnchor", "MiddleCentre"},
                {PropertyType::k_alignmentAnchor, "ParentalAnchor", "MiddleCentre"},
                {PropertyType::k_bool, "Visible", "true"},
                {PropertyType::k_bool, "ClipChildren", "false"},
                {PropertyType::k_sizePolicy, "SizePolicy", "None"},
                {PropertyType::k_propertyMap, "Layout", "{\"Type\":\"None\"}"},
                {PropertyType::k_propertyMap, "Drawable", "{\"Type\":\"None\"}"}
            };
            
            //----------------------------------------------------------------------------------------
            /// Perform a rough check to see if the widget is offscreen
            ///
            /// @author S Downie
            ///
            /// @param Absolute position
            /// @param Absolute size
            /// @param Canvas size
            ///
            /// @return Whether the widget is considered offscreen and should be culled
            //----------------------------------------------------------------------------------------
            bool ShouldCull(const Core::Vector2& in_absPos, const Core::Vector2& in_absSize, const Core::Vector2& in_canvasSize)
            {
                Core::Vector2 halfSize(in_absSize * 0.5f);
                //Treat it like a square so that we do not need to take rotation into account
                halfSize.x = std::max(halfSize.x, halfSize.y);
                halfSize.y = std::max(halfSize.y, halfSize.y);
                
                Core::Vector2 bottomLeft;
                Rendering::GetAnchorPoint(Rendering::AlignmentAnchor::k_bottomLeft, halfSize, bottomLeft);
                bottomLeft += in_absPos;
                
                Core::Vector2 topRight;
                Rendering::GetAnchorPoint(Rendering::AlignmentAnchor::k_topRight, halfSize, topRight);
                topRight += in_absPos;
                
                return (topRight.y < 0 || bottomLeft.y > in_canvasSize.y || topRight.x < 0 || bottomLeft.x > in_canvasSize.x);
            }
            
            namespace SizePolicyFuncs
            {
                //----------------------------------------------------------
                /// Aspect ratio maintaing function that returns the original
                /// size. This is used despite the fact it doesn't do much to
                /// prevent multiple code paths when calculating size.
                ///
                /// @author S Downie
                ///
                /// @param Original size
                /// @param Preferred size
                ///
                /// @return Original size
                //----------------------------------------------------------
                Core::Vector2 UseOriginalSize(const Core::Vector2& in_originalSize, const Core::Vector2& in_preferredSize)
                {
                    return in_originalSize;
                }
                //----------------------------------------------------------
                /// Aspect ratio maintaing function that returns the preferred
                /// size. This is used despite the fact it doesn't do much to
                /// prevent multiple code paths when calculating size.
                ///
                /// @author S Downie
                ///
                /// @param Original size
                /// @param Preferred size
                ///
                /// @return Preferred size
                //----------------------------------------------------------
                Core::Vector2 UsePreferredSize(const Core::Vector2& in_originalSize, const Core::Vector2& in_preferredSize)
                {
                    return in_preferredSize;
                }
                //----------------------------------------------------------------------------------------
                /// Aspect ratio maintaining function that keeps the original width but adapts
                /// the height to maintain the aspect ratio
                ///
                /// @author S Downie
                ///
                /// @param Original size
                /// @param Preferred size
                ///
                /// @return Size with aspect maintained
                //----------------------------------------------------------------------------------------
                Core::Vector2 KeepOriginalWidthAdaptHeight(const Core::Vector2& in_originalSize, const Core::Vector2& in_preferredSize)
                {
                    return Rendering::AspectRatioUtils::KeepOriginalWidthAdaptHeight(in_originalSize, in_preferredSize.x/in_preferredSize.y);
                }
                //----------------------------------------------------------------------------------------
                /// Aspect ratio maintaining function that keeps the original height but adapts
                /// the width to maintain the aspect ratio
                ///
                /// @author S Downie
                ///
                /// @param Original size
                /// @param Preferred size
                ///
                /// @return Size with aspect maintained
                //----------------------------------------------------------------------------------------
                Core::Vector2 KeepOriginalHeightAdaptWidth(const Core::Vector2& in_originalSize, const Core::Vector2& in_preferredSize)
                {
                    return Rendering::AspectRatioUtils::KeepOriginalHeightAdaptWidth(in_originalSize, in_preferredSize.x/in_preferredSize.y);
                }
                //----------------------------------------------------------------------------------------
                /// Aspect ratio maintaining function that maintains the given target aspect ratio
                /// while ensuring the size does not DROP BELOW the original size
                ///
                /// @author S Downie
                ///
                /// @param Original size
                /// @param Preferred size
                ///
                /// @return Size with aspect maintained
                //----------------------------------------------------------------------------------------
                Core::Vector2 FillOriginal(const Core::Vector2& in_originalSize, const Core::Vector2& in_preferredSize)
                {
                    return Rendering::AspectRatioUtils::FillOriginal(in_originalSize, in_preferredSize.x/in_preferredSize.y);
                }
                //----------------------------------------------------------------------------------------
                /// Aspect ratio maintaining function that maintains the given target aspect ratio
                /// while ensuring the size does not EXCEED the original size
                ///
                /// @author S Downie
                ///
                /// @param Original size
                /// @param Preferred size
                ///
                /// @return Size with aspect maintained
                //----------------------------------------------------------------------------------------
                Core::Vector2 FitOriginal(const Core::Vector2& in_originalSize, const Core::Vector2& in_preferredSize)
                {
                    return Rendering::AspectRatioUtils::FitOriginal(in_originalSize, in_preferredSize.x/in_preferredSize.y);
                }
                
                const Widget::SizePolicyDelegate k_sizePolicyFuncs[(u32)SizePolicy::k_totalNum] =
                {
                    UseOriginalSize,
                    UsePreferredSize,
                    KeepOriginalWidthAdaptHeight,
                    KeepOriginalHeightAdaptWidth,
                    FitOriginal,
                    FillOriginal
                };
            }
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        Widget::Widget(const PropertyMap& in_defaultProperties, const PropertyMap& in_customProperties)
        {
            SetDefaultProperties(in_defaultProperties);
            SetCustomProperties(in_customProperties);
            
            m_screen = Core::Application::Get()->GetSystem<Core::Screen>();
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        std::vector<PropertyMap::PropertyDesc> Widget::GetPropertyDescs()
        {
            return g_propertyDescs;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void Widget::SetDefaultProperties(const PropertyMap& in_defaultProperties)
        {
            SetName(in_defaultProperties.GetProperty<std::string>("Name"));
            SetRelativePosition(in_defaultProperties.GetProperty<Core::Vector2>("RelPosition"));
            SetAbsolutePosition(in_defaultProperties.GetProperty<Core::Vector2>("AbsPosition"));
            SetRelativeSize(in_defaultProperties.GetProperty<Core::Vector2>("RelSize"));
            SetAbsoluteSize(in_defaultProperties.GetProperty<Core::Vector2>("AbsSize"));
            SetDefaultPreferredSize(in_defaultProperties.GetProperty<Core::Vector2>("PreferredSize"));
            ScaleTo(in_defaultProperties.GetProperty<Core::Vector2>("Scale"));
            SetColour(in_defaultProperties.GetProperty<Core::Colour>("Colour"));
            RotateTo(in_defaultProperties.GetProperty<f32>("Rotation"));
            SetParentalAnchor(in_defaultProperties.GetProperty<Rendering::AlignmentAnchor>("ParentalAnchor"));
            SetOriginAnchor(in_defaultProperties.GetProperty<Rendering::AlignmentAnchor>("OriginAnchor"));
            SetVisible(in_defaultProperties.GetProperty<bool>("Visible"));
            SetClippingEnabled(in_defaultProperties.GetProperty<bool>("ClipChildren"));
            SetSizePolicy(in_defaultProperties.GetProperty<SizePolicy>("SizePolicy"));
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void Widget::SetCustomProperties(const PropertyMap& in_customProperties)
        {
            m_customProperties = in_customProperties;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void Widget::SetDrawable(IDrawableUPtr in_drawable)
        {
            m_drawable = std::move(in_drawable);
            
            InvalidateTransformCache();
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void Widget::SetLayout(ILayoutUPtr in_layout)
        {
            m_layout = std::move(in_layout);
            
            if(m_layout != nullptr)
            {
                m_layout->SetWidget(this);
            }
            
            OnLayoutChanged(m_layout.get());
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void Widget::SetInternalLayout(ILayoutUPtr in_layout)
        {
            m_internalLayout = std::move(in_layout);
            
            if(m_internalLayout != nullptr)
            {
                m_internalLayout->SetWidget(this);
            }
            
            OnLayoutChanged(m_internalLayout.get());
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void Widget::SetName(const std::string& in_name)
        {
            m_name = in_name;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        const std::string& Widget::GetName() const
        {
            return m_name;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void Widget::SetRelativeSize(f32 in_width, f32 in_height)
        {
            m_localSize.vRelative.x = in_width;
            m_localSize.vRelative.y = in_height;
            
            InvalidateTransformCache();
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void Widget::SetRelativeSize(const Core::Vector2& in_size)
        {
            m_localSize.vRelative = in_size;
            
            InvalidateTransformCache();
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void Widget::SetAbsoluteSize(f32 in_width, f32 in_height)
        {
            m_localSize.vAbsolute.x = in_width;
            m_localSize.vAbsolute.y = in_height;
            
            InvalidateTransformCache();
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void Widget::SetAbsoluteSize(const Core::Vector2& in_size)
        {
            m_localSize.vAbsolute = in_size;
            
            InvalidateTransformCache();
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void Widget::SetDefaultPreferredSize(const Core::Vector2& in_size)
        {
            m_preferredSize = in_size;
            
            InvalidateTransformCache();
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void Widget::SetSizePolicy(SizePolicy in_policy)
        {
            CS_ASSERT(in_policy != SizePolicy::k_totalNum, "k_totalNum is not a size policy");
            
            InvalidateTransformCache();
            
            m_sizePolicyDelegate = SizePolicyFuncs::k_sizePolicyFuncs[(u32)in_policy];
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void Widget::SetRelativePosition(f32 in_x, f32 in_y)
        {
            m_localPosition.vRelative.x = in_x;
            m_localPosition.vRelative.y = in_y;
            
            InvalidateTransformCache();
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void Widget::SetRelativePosition(const Core::Vector2& in_pos)
        {
            m_localPosition.vRelative = in_pos;
            
            InvalidateTransformCache();
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void Widget::SetAbsolutePosition(f32 in_x, f32 in_y)
        {
            m_localPosition.vAbsolute.x = in_x;
            m_localPosition.vAbsolute.y = in_y;
            
            InvalidateTransformCache();
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void Widget::SetAbsolutePosition(const Core::Vector2& in_pos)
        {
            m_localPosition.vAbsolute = in_pos;
            
            InvalidateTransformCache();
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void Widget::RelativeMoveBy(f32 in_x, f32 in_y)
        {
            m_localPosition.vRelative.x = in_x;
            m_localPosition.vRelative.y = in_y;
            
            InvalidateTransformCache();
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void Widget::RelativeMoveBy(const Core::Vector2& in_translate)
        {
            m_localPosition.vRelative += in_translate;
            
            InvalidateTransformCache();
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void Widget::AbsoluteMoveBy(f32 in_x, f32 in_y)
        {
            m_localPosition.vAbsolute.x += in_x;
            m_localPosition.vAbsolute.y += in_y;
            
            InvalidateTransformCache();
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void Widget::AbsoluteMoveBy(const Core::Vector2& in_translate)
        {
            m_localPosition.vAbsolute += in_translate;
            
            InvalidateTransformCache();
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void Widget::RotateBy(f32 in_angleRads)
        {
            m_localRotation += in_angleRads;
            
            InvalidateTransformCache();
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void Widget::RotateTo(f32 in_angleRads)
        {
            m_localRotation = in_angleRads;
            
            InvalidateTransformCache();
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void Widget::ScaleBy(const Core::Vector2& in_scale)
        {
            m_localScale *= in_scale;
            
            InvalidateTransformCache();
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void Widget::ScaleBy(f32 in_x, f32 in_y)
        {
            m_localScale.x *= in_x;
            m_localScale.y *= in_y;
            
            InvalidateTransformCache();
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void Widget::ScaleTo(const Core::Vector2& in_scale)
        {
            m_localScale = in_scale;
            
            InvalidateTransformCache();
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void Widget::ScaleTo(f32 in_x, f32 in_y)
        {
            m_localScale.x = in_x;
            m_localScale.y = in_y;
            
            InvalidateTransformCache();
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void Widget::SetParentalAnchor(Rendering::AlignmentAnchor in_anchor)
        {
            m_parentalAnchor = in_anchor;
            
            InvalidateTransformCache();
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void Widget::SetOriginAnchor(Rendering::AlignmentAnchor in_anchor)
        {
            m_originAnchor = in_anchor;
            
            InvalidateTransformCache();
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void Widget::SetColour(const Core::Colour& in_colour)
        {
            m_localColour = in_colour;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void Widget::SetVisible(bool in_visible)
        {
            m_isVisible = in_visible;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        bool Widget::IsVisible() const
        {
            return m_isVisible;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void Widget::SetClippingEnabled(bool in_enabled)
        {
            m_isSubviewClippingEnabled = in_enabled;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void Widget::AddWidget(const WidgetSPtr& in_widget)
        {
            CS_ASSERT(in_widget->GetParent() == nullptr, "Cannot add a widget as a child of more than 1 parent");
            //TODO: Ensure that the vector is not invalidated during iteration
            m_children.push_back(in_widget);
            in_widget->m_parent = this;
            in_widget->SetCanvas(m_canvas);
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void Widget::RemoveWidget(Widget* in_widget)
        {
            CS_ASSERT(in_widget->GetParent() == this, "Widget is a child of a different parent");
            
            for(auto it = std::begin(m_children); it != std::end(m_children); ++it)
            {
                if(it->get() == in_widget)
                {
                    m_children.erase(it);
                    return;
                }
            }
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void Widget::AddInternalWidget(const WidgetSPtr& in_widget)
        {
            CS_ASSERT(in_widget->GetParent() == nullptr, "Cannot add a widget as a child of more than 1 parent");
            //TODO: Ensure that the vector is not invalidated during iteration
            m_internalChildren.push_back(in_widget);
            in_widget->m_parent = this;
            in_widget->SetCanvas(m_canvas);
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void Widget::RemoveFromParent()
        {
            CS_ASSERT(m_parent != nullptr, "Widget has no parent to remove from");
            
            m_parent->RemoveWidget(this);
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        Widget* Widget::GetWidget(const std::string& in_name)
        {
            for(auto& child : m_children)
            {
                if(child->m_name == in_name)
                {
                    return child.get();
                }
            }
            
            return nullptr;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        Widget* Widget::GetParent()
        {
            return m_parent;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        const Widget* Widget::GetParent() const
        {
            return m_parent;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void Widget::BringToFront()
        {
            CS_ASSERT(m_parent != nullptr, "Widget has no parent to rearrange from");
            
            s32 length = m_parent->m_children.size() - 1;
            for(s32 i=0; i<length; ++i)
            {
                if(m_parent->m_children[i].get() == this)
                {
                    std::swap(m_parent->m_children[i], m_parent->m_children[i+1]);
                }
            }
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void Widget::BringForward()
        {
            CS_ASSERT(m_parent != nullptr, "Widget has no parent to rearrange from");
            
            s32 length = m_parent->m_children.size() - 1;
            for(s32 i=0; i<length; ++i)
            {
                if(m_parent->m_children[i].get() == this)
                {
                    std::swap(m_parent->m_children[i], m_parent->m_children[i+1]);
                    return;
                }
            }
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void Widget::SendBackward()
        {
            CS_ASSERT(m_parent != nullptr, "Widget has no parent to rearrange from");
            
            u32 length = m_parent->m_children.size();
            for(u32 i=1; i<length; ++i)
            {
                if(m_parent->m_children[i].get() == this)
                {
                    std::swap(m_parent->m_children[i], m_parent->m_children[i-1]);
                    return;
                }
            }
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void Widget::SendToBack()
        {
            CS_ASSERT(m_parent != nullptr, "Widget has no parent to rearrange from");
            
            u32 length = m_parent->m_children.size();
            for(u32 i=1; i<length; ++i)
            {
                if(m_parent->m_children[i].get() == this)
                {
                    std::swap(m_parent->m_children[i], m_parent->m_children[i-1]);
                }
            }
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void Widget::SetCanvas(const Widget* in_canvas)
        {
            m_canvas = in_canvas;
            
            InvalidateTransformCache();
            
            for(auto& child : m_internalChildren)
            {
                child->SetCanvas(m_canvas);
            }
            
            for(auto& child : m_children)
            {
                child->SetCanvas(m_canvas);
            }
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void Widget::SetParent(Widget* in_parent)
        {
            m_parent = in_parent;
            
            InvalidateTransformCache();
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void Widget::Draw(Rendering::CanvasRenderer* in_renderer)
        {
            if(m_isVisible == false)
            {
                return;
            }
            
            Core::Vector2 finalSize(GetFinalSize());
            
            if(m_drawable != nullptr && ShouldCull(GetFinalPosition(), finalSize, m_screen->GetResolution()) == false)
            {
                m_drawable->Draw(in_renderer, GetFinalTransform(), finalSize, GetFinalColour());
            }
            
            if(m_isSubviewClippingEnabled == true)
            {
                Core::Vector2 halfSize(finalSize * 0.5f);
                Core::Vector2 bottomLeftPos;
                Rendering::GetAnchorPoint(Rendering::AlignmentAnchor::k_bottomLeft, halfSize, bottomLeftPos);
                bottomLeftPos += GetFinalPosition();
                
                in_renderer->PushClipBounds(bottomLeftPos, finalSize);
            }
            
            for(auto& child : m_internalChildren)
            {
                child->Draw(in_renderer);
            }
            
            for(auto& child : m_children)
            {
                child->Draw(in_renderer);
            }
            
            if(m_isSubviewClippingEnabled == true)
            {
                in_renderer->PopClipBounds();
            }
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        Core::Matrix3 Widget::GetLocalTransform() const
        {
            if(m_isLocalTransformCacheValid == true)
            {
                return m_cachedLocalTransform;
            }
            
            Core::Vector2 halfSize(GetFinalSize() * 0.5f);
            Core::Vector2 pivotPos;
            Rendering::GetAnchorPoint(m_originAnchor, halfSize, pivotPos);
            
            Core::Matrix3 pivot(Core::Matrix3::CreateTransform(-pivotPos, Core::Vector2::k_one, 0.0f));
            Core::Matrix3 rotate(Core::Matrix3::CreateTransform(Core::Vector2::k_zero, Core::Vector2::k_one, m_localRotation));
            Core::Matrix3 translate(Core::Matrix3::CreateTransform(GetParentSpacePosition() - pivotPos, Core::Vector2::k_one, 0.0f));
            
            m_cachedLocalTransform = pivot * rotate * translate;
            
            m_isLocalTransformCacheValid = true;
            return m_cachedLocalTransform;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        Core::Matrix3 Widget::GetFinalTransform() const
        {
            if(m_isParentTransformCacheValid == true && m_isLocalTransformCacheValid == true)
            {
                return m_cachedFinalTransform;
            }
            
            if(m_canvas == this)
            {
                m_cachedFinalTransform = Core::Matrix3::CreateTransform(m_localPosition.vAbsolute, Core::Vector2::k_one, m_localRotation);
                
                m_isParentTransformCacheValid = true;
                m_isLocalTransformCacheValid = true;
                return m_cachedFinalTransform;
            }
            
            m_cachedFinalTransform = GetLocalTransform() * m_parent->GetFinalTransform();
            m_isParentTransformCacheValid = true;
            
            return m_cachedFinalTransform;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        Core::Vector2 Widget::GetFinalPosition() const
        {
			Core::Matrix3 finalTransform(GetFinalTransform());
            return Core::Vector2(finalTransform.m[6], finalTransform.m[7]);
        }
        //----------------------------------------------------------------------------------------
        /// The position of the widget is calculated based on the local absolute and
        /// relative positions as well as the local alignment anchors and layout. The local relative
        /// position is relative to the final parent position and cannot be calculated until
        /// there is an absolute reference point in the widget hierarchy.
        //----------------------------------------------------------------------------------------
        Core::Vector2 Widget::GetParentSpacePosition() const
        {
            CS_ASSERT(m_canvas != nullptr, "Cannot get the absolute position of widget without attaching it to the canvas");
            CS_ASSERT(m_parent != nullptr, "Cannot get the absolute position of widget without a parent");
            
            const Core::Vector2 parentSize(m_parent->GetFinalSize());
            const Core::Vector2 parentHalfSize(parentSize * 0.5f);
            
            //Offset the position by the alignment anchor of the origin
            Core::Vector2 alignmentOffset;
            Rendering::Align(m_originAnchor, GetFinalSize() * 0.5f, alignmentOffset);
            
            ILayout* layout = nullptr;
            s32 childIndex = -1;
            
            for(u32 i=0; i<m_parent->m_children.size(); ++i)
            {
                if(m_parent->m_children[i].get() == this)
                {
                    childIndex = (s32)i;
                    layout = m_parent->m_layout.get();
                    break;
                }
            }
            
            if(layout == nullptr)
            {
                for(u32 i=0; i<m_parent->m_internalChildren.size(); ++i)
                {
                    if(m_parent->m_internalChildren[i].get() == this)
                    {
                        childIndex = (s32)i;
                        layout = m_parent->m_internalLayout.get();
                        break;
                    }
                }
            }
            
            if(layout == nullptr)
            {
                //Get the anchor point to which the widget is aligned in parent space
                Core::Vector2 parentAnchorPos;
                Rendering::GetAnchorPoint(m_parentalAnchor, parentHalfSize, parentAnchorPos);
                
                //Calculate the position relative to the anchor point
                Core::Vector2 parentSpacePos = parentAnchorPos + (parentSize * m_localPosition.vRelative) + m_localPosition.vAbsolute;
                
                return parentSpacePos + alignmentOffset;
            }
            else
            {
                CS_ASSERT(childIndex >= 0, "Cannot find child");
                
                //The parental anchor pertains to the cell when using a layout rather than the parent widget
                Core::Vector2 parentAnchorPos;
                Rendering::GetAnchorPoint(Rendering::AlignmentAnchor::k_bottomLeft, parentHalfSize, parentAnchorPos);
                
                Core::Vector2 cellSize(layout->GetSizeForIndex((u32)childIndex));
                Core::Vector2 cellHalfSize(cellSize * 0.5f);
                Core::Vector2 cellAnchorPos;
                Rendering::GetAnchorPoint(m_parentalAnchor, cellHalfSize, cellAnchorPos);
                
                //Transform into parent space then layout/cell space
                Core::Vector2 parentSpacePos = parentAnchorPos;
                Core::Vector2 layoutSpacePos = cellAnchorPos + (cellSize * m_localPosition.vRelative) + m_localPosition.vAbsolute + layout->GetPositionForIndex((u32)childIndex);
                
                return parentSpacePos + layoutSpacePos + alignmentOffset;
            }
        }
        //----------------------------------------------------------------------------------------
        /// The final size of the widget is calculated based on the local absolute and the
        /// local relative size. The local relative size is relative to the final parent size and
        /// cannot be calculated until there is an absolute reference point in the widget
        /// hierarchy
        //----------------------------------------------------------------------------------------
        Core::Vector2 Widget::GetFinalSize() const
        {
            CS_ASSERT(m_canvas != nullptr, "Cannot get the absolute size of widget without attaching it to the canvas");
            
            if(m_isParentSizeCacheValid == true && m_isLocalSizeCacheValid == true)
            {
                return m_cachedFinalSize;
            }
            
            Core::Vector2 finalSize;
            
            if(m_parent != nullptr)
            {
                finalSize = m_parent->CalculateChildFinalSize(this);
            }
            else
            {
                finalSize = m_localSize.vAbsolute;
            }
            
            finalSize = m_sizePolicyDelegate(finalSize, GetPreferredSize()) * m_localScale;
            
            std::unique_lock<std::mutex> lock(m_sizeMutex);
            m_cachedFinalSize = finalSize;
            lock.unlock();
            
            m_isLocalSizeCacheValid = true;
            m_isParentSizeCacheValid = true;
            
            return finalSize;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        Core::Vector2 Widget::CalculateChildFinalSize(const Widget* in_child)
        {
            ILayout* layout = nullptr;
            s32 childIndex = -1;
            
            for(u32 i=0; i<m_children.size(); ++i)
            {
                if(m_children[i].get() == in_child)
                {
                    childIndex = (s32)i;
                    layout = m_layout.get();
                    break;
                }
            }
            
            if(layout == nullptr)
            {
                for(u32 i=0; i<m_internalChildren.size(); ++i)
                {
                    if(m_internalChildren[i].get() == in_child)
                    {
                        childIndex = (s32)i;
                        layout = m_internalLayout.get();
                        break;
                    }
                }
            }
            
            if(layout == nullptr)
            {
                return ((GetFinalSize() * in_child->m_localSize.vRelative) + in_child->m_localSize.vAbsolute);
            }
            
            CS_ASSERT(childIndex >= 0, "Cannot find child");
            
            return ((layout->GetSizeForIndex((u32)childIndex) * in_child->m_localSize.vRelative) + in_child->m_localSize.vAbsolute);
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        Core::Vector2 Widget::GetPreferredSize() const
        {
            if(m_drawable != nullptr)
            {
                return m_drawable->GetPreferredSize();
            }
            
            return m_preferredSize;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        f32 Widget::GetFinalRotation() const
        {
            if(m_parent != nullptr)
            {
                return m_localRotation + m_parent->GetFinalRotation();
            }
            
            return m_localRotation;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        Core::Vector2 Widget::GetFinalScale() const
        {
            if(m_parent != nullptr)
            {
                return m_localScale * m_parent->GetFinalScale();
            }
            
            return m_localScale;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        Core::Colour Widget::GetFinalColour() const
        {
            //TODO: Do we implement the inherit colour option?
            if(m_parent != nullptr)
            {
                return m_localColour * m_parent->GetFinalColour();
            }
            
            return m_localColour;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void Widget::InvalidateTransformCache()
        {
            m_isLocalTransformCacheValid = false;
            m_isLocalSizeCacheValid = false;
            
            if(m_canvas != nullptr)
            {
                if(m_layout != nullptr)
                {
                    m_layout->BuildLayout();
                }
                
                if(m_internalLayout != nullptr)
                {
                    m_internalLayout->BuildLayout();
                }
            }
            
            for(auto& child : m_internalChildren)
            {
                child->OnParentTransformChanged();
            }
            
            for(auto& child : m_children)
            {
                child->OnParentTransformChanged();
            }
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void Widget::OnParentTransformChanged()
        {
            m_isParentTransformCacheValid = false;
            m_isParentSizeCacheValid = false;
            
            InvalidateTransformCache();
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void Widget::OnLayoutChanged(const ILayout* in_layout)
        {
            if(in_layout == m_layout.get())
            {
                for(auto& child : m_children)
                {
                    child->OnParentTransformChanged();
                }
            }
            else if(in_layout == m_internalLayout.get())
            {
                for(auto& child : m_internalChildren)
                {
                    child->OnParentTransformChanged();
                }
            }
        }
    }
}
