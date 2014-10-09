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
#include <ChilliSource/Scripting/Lua/LuaSystem.h>
#include <ChilliSource/Rendering/Base/AlignmentAnchors.h>
#include <ChilliSource/Rendering/Base/AspectRatioUtils.h>
#include <ChilliSource/Rendering/Base/CanvasRenderer.h>
#include <ChilliSource/UI/Base/WidgetProxy.h>
#include <ChilliSource/UI/Drawable/NinePatchDrawableProxy.h>
#include <ChilliSource/UI/Drawable/StandardDrawableProxy.h>
#include <ChilliSource/UI/Drawable/ThreePatchDrawableProxy.h>

namespace ChilliSource
{
    namespace UI
    {
        namespace
        {
            const std::vector<PropertyMap::PropertyDesc> k_propertyDescs =
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
                {PropertyType::k_bool, "InputEnabled", "false"},
                {PropertyType::k_bool, "InputConsumeEnabled", "true"},
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
                
                Core::Vector2 bottomLeft = Rendering::GetAnchorPoint(Rendering::AlignmentAnchor::k_bottomLeft, halfSize);
                bottomLeft += in_absPos;
                
                Core::Vector2 topRight = Rendering::GetAnchorPoint(Rendering::AlignmentAnchor::k_topRight, halfSize);
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
            return k_propertyDescs;
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
            SetInputEnabled(in_defaultProperties.GetProperty<bool>("InputEnabled"));
            SetInputConsumeEnabled(in_defaultProperties.GetProperty<bool>("InputConsumeEnabled"));
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
        void Widget::SetPropertyLinks(std::unordered_map<std::string, IPropertyAccessorUPtr>&& in_defaultLinks, std::unordered_map<std::string, std::pair<Widget*, std::string>>&& in_customLinks)
        {
            m_defaultPropertyLinks = std::move(in_defaultLinks);
            m_customPropertyLinks = std::move(in_customLinks);
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void Widget::SetBehaviourScript(const Scripting::LuaSourceCSPtr& in_behaviourSource)
        {
            if(in_behaviourSource != nullptr)
            {
                auto luaSystem = Core::Application::Get()->GetSystem<Scripting::LuaSystem>();
                m_behaviourScript = luaSystem->CreateScript(in_behaviourSource);
                m_behaviourScript->RegisterVariable("this", this);
                m_behaviourScript->Run();
            }
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        Core::IConnectableEvent<Widget::InputDelegate>& Widget::GetPressedInsideEvent()
        {
            return m_pressedInsideEvent;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        Core::IConnectableEvent<Widget::InputDelegate>& Widget::GetReleasedInsideEvent()
        {
            return m_releasedInsideEvent;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        Core::IConnectableEvent<Widget::InputDelegate>& Widget::GetReleasedOutsideEvent()
        {
            return m_releasedOutsideEvent;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        Core::IConnectableEvent<Widget::InputMovedDelegate>& Widget::GetMoveExitedEvent()
        {
            return m_moveExitedEvent;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        Core::IConnectableEvent<Widget::InputMovedDelegate>& Widget::GetMoveEnteredEvent()
        {
            return m_moveEnteredEvent;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        Core::IConnectableEvent<Widget::InputMovedDelegate>& Widget::GetDraggedInsideEvent()
        {
            return m_draggedInsideEvent;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        Core::IConnectableEvent<Widget::InputMovedDelegate>& Widget::GetDraggedOutsideEvent()
        {
            return m_draggedOutsideEvent;
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
        IDrawable* Widget::GetDrawable() const
        {
            return m_drawable.get();
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
        ILayout* Widget::GetLayout() const
        {
            return m_layout.get();
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
        void Widget::SetRelativeSize(const Core::Vector2& in_size)
        {
            m_localSize.vRelative = in_size;
            
            InvalidateTransformCache();
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        Core::Vector2 Widget::GetLocalRelativeSize() const
        {
            return m_localSize.vRelative;
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
        Core::Vector2 Widget::GetLocalAbsoluteSize() const
        {
            return m_localSize.vAbsolute;
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
            
            m_sizePolicy = in_policy;
            m_sizePolicyDelegate = SizePolicyFuncs::k_sizePolicyFuncs[(u32)m_sizePolicy];
            
            InvalidateTransformCache();
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        SizePolicy Widget::GetSizePolicy() const
        {
            return m_sizePolicy;
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
        Core::Vector2 Widget::GetLocalRelativePosition() const
        {
            return m_localPosition.vRelative;
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
        Core::Vector2 Widget::GetLocalAbsolutePosition() const
        {
            return m_localPosition.vAbsolute;
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
        f32 Widget::GetLocalRotation() const
        {
            return m_localRotation;
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
        void Widget::ScaleTo(const Core::Vector2& in_scale)
        {
            m_localScale = in_scale;
            
            InvalidateTransformCache();
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        Core::Vector2 Widget::GetLocalScale() const
        {
            return m_localScale;
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
        Rendering::AlignmentAnchor Widget::GetParentalAnchor() const
        {
            return m_parentalAnchor;
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
        Rendering::AlignmentAnchor Widget::GetOriginAnchor() const
        {
            return m_originAnchor;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void Widget::SetOriginAbsolutePosition(const Core::Vector2& in_position)
        {
            m_originPosition.vAbsolute = in_position;
            
            InvalidateTransformCache();
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        const Core::Vector2& Widget::GetOriginAbsolutePosition() const
        {
            return m_originPosition.vAbsolute;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void Widget::SetOriginRelativePosition(const Core::Vector2& in_position)
        {
            m_originPosition.vRelative = in_position;
            
            InvalidateTransformCache();
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        const Core::Vector2& Widget::GetOriginRelativePosition() const
        {
            return m_originPosition.vRelative;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void Widget::SetColour(const Core::Colour& in_colour)
        {
            m_localColour = Core::Colour::Clamp(in_colour);
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        Core::Colour Widget::GetLocalColour() const
        {
            return m_localColour;
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
        bool Widget::IsClippingEnabled() const
        {
            return m_isSubviewClippingEnabled;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void Widget::SetInputEnabled(bool in_input)
        {
            m_isInputEnabled = in_input;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        bool Widget::IsInputEnabled() const
        {
            return m_isInputEnabled;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void Widget::SetInputConsumeEnabled(bool in_consume)
        {
            m_isInputConsumeEnabled = in_consume;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        bool Widget::IsInputConsumeEnabled() const
        {
            return m_isInputConsumeEnabled;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void Widget::AddWidget(const WidgetSPtr& in_widget)
        {
            CS_ASSERT(in_widget->GetParent() == nullptr, "Cannot add a widget as a child of more than 1 parent");

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
                    (*it)->SetCanvas(nullptr);
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
        Widget* Widget::GetInternalWidget(const std::string& in_name)
        {
            for(auto& child : m_internalChildren)
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
        bool Widget::IsOnCanvas() const
        {
            return (m_canvas != nullptr);
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
            
            if(m_behaviourScript != nullptr)
            {
                if(m_canvas != nullptr)
                {
                    m_behaviourScript->CallFunction("onAddedToCanvas", Scripting::LuaScript::FunctionNotFoundPolicy::k_failSilent);
                }
                else
                {
                    m_behaviourScript->CallFunction("onRemovedFromCanvas", Scripting::LuaScript::FunctionNotFoundPolicy::k_failSilent);
                }
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
        void Widget::SetCustomProperty(const std::string& in_name, const char* in_value)
        {
            SetCustomProperty<std::string>(in_name, in_value);
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        const char* Widget::GetCustomProperty(const std::string& in_name) const
        {
            return GetCustomProperty<std::string>(in_name).c_str();
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        bool Widget::Contains(const Core::Vector2& in_point) const
        {
            //Convert the point into our local space allowing us to do an AABB check
            Core::Vector2 halfSize = GetFinalSize() * 0.5f;
            
            Core::Vector2 bottLeft(-halfSize.x, -halfSize.y);
            Core::Vector2 topRight(halfSize.x, halfSize.y);
            
            Core::Vector2 localPoint = ToLocalSpace(in_point, Rendering::AlignmentAnchor::k_middleCentre);
			return localPoint.x >= bottLeft.x && localPoint.y >= bottLeft.y && localPoint.x <= topRight.x && localPoint.y <= topRight.y;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        Core::Vector2 Widget::ToLocalSpace(const Core::Vector2& in_point, Rendering::AlignmentAnchor in_alignmentAnchor) const
        {
            Core::Vector2 localPointCentreRelative = in_point * Core::Matrix3::Inverse(GetFinalTransform());
            Core::Vector2 localPointAligned = localPointCentreRelative - Rendering::GetAnchorPoint(in_alignmentAnchor, GetFinalSize());
            return localPointAligned;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        Core::Matrix3 Widget::GetLocalTransform() const
        {
            if(m_isLocalTransformCacheValid == true)
            {
                return m_cachedLocalTransform;
            }
            
            Core::Vector2 anchorPoint = GetLocalOriginCombinedPosition();
            const Core::Vector2 renderSpacePositionCentred = GetLocalRenderSpaceCombinedPosition() - anchorPoint;
            
            Core::Matrix3 pivot(Core::Matrix3::CreateTransform(-anchorPoint, Core::Vector2::k_one, 0.0f));
            Core::Matrix3 rotate(Core::Matrix3::CreateTransform(Core::Vector2::k_zero, Core::Vector2::k_one, -m_localRotation));
            Core::Matrix3 translate(Core::Matrix3::CreateTransform(renderSpacePositionCentred - anchorPoint, Core::Vector2::k_one, 0.0f));
            
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
                m_cachedFinalTransform = Core::Matrix3::CreateTransform(m_localPosition.vAbsolute, Core::Vector2::k_one, -m_localRotation);
                m_cachedFinalPosition = m_localPosition.vAbsolute;
                
                m_isParentTransformCacheValid = true;
                m_isLocalTransformCacheValid = true;
                return m_cachedFinalTransform;
            }
            
            m_cachedFinalTransform = GetLocalTransform() * m_parent->GetFinalTransform();
            m_cachedFinalPosition = GetLocalRenderSpaceCombinedPosition() * m_parent->GetFinalTransform();
            m_isParentTransformCacheValid = true;
            
            return m_cachedFinalTransform;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        Core::Vector2 Widget::GetFinalPosition() const
        {
            //get the final transform to ensure the final position cache is up to date.
            GetFinalTransform();
            return m_cachedFinalPosition;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        Core::Vector2 Widget::GetFinalPositionOfCentre() const
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
        Core::Vector2 Widget::GetLocalRenderSpaceCombinedPosition() const
        {
            CS_ASSERT(m_canvas != nullptr, "Cannot get the local render space combined position of widget without attaching it to the canvas.");
            CS_ASSERT(m_parent != nullptr, "Cannot get the local render space combined position of widget without a parent.");
            
            const Core::Vector2 parentSize(m_parent->GetFinalSize());
            const Core::Vector2 parentHalfSize(parentSize * 0.5f);
            
            auto layoutPair = m_parent->GetLayoutForChild(this);
            
            if(layoutPair.first == nullptr)
            {
                //Get the anchor point to which the widget is aligned
                Core::Vector2 parentAnchorPos = Rendering::GetAnchorPoint(m_parentalAnchor, parentHalfSize);
                
                //Calculate the position relative to the anchor point
                Core::Vector2 localRenderSpacePos = parentAnchorPos + (parentSize * m_localPosition.vRelative) + m_localPosition.vAbsolute;
                return localRenderSpacePos;
            }
            else
            {
                CS_ASSERT(layoutPair.second >= 0, "Cannot find child");
                
                //The parental anchor pertains to the cell when using a layout rather than the parent widget
                Core::Vector2 parentAnchorPos = Rendering::GetAnchorPoint(Rendering::AlignmentAnchor::k_bottomLeft, parentHalfSize);
                Core::Vector2 cellSize(layoutPair.first->GetSizeForIndex((u32)layoutPair.second));
                Core::Vector2 cellAnchorPos = Rendering::GetAnchorPoint(m_parentalAnchor, cellSize * 0.5f);
                
                //Transform into local render space then layout/cell space
                Core::Vector2 localRenderSpacePos = parentAnchorPos;
                Core::Vector2 layoutSpacePos = cellAnchorPos + (cellSize * m_localPosition.vRelative) + m_localPosition.vAbsolute + layoutPair.first->GetPositionForIndex((u32)layoutPair.second);
                
                return localRenderSpacePos + layoutSpacePos;
            }
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        Core::Vector2 Widget::GetLocalOriginCombinedPosition() const
        {
            CSCore::Vector2 originAnchorPos = Rendering::GetAnchorPoint(m_originAnchor, GetFinalSize() * 0.5f);
            CSCore::Vector2 originPos = originAnchorPos + (GetRelativeReferenceSize() * m_originPosition.vRelative) + m_originPosition.vAbsolute;
            return originPos;
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
                finalSize = (m_parent->CalculateRelativeReferenceSizeForChild(this) * m_localSize.vRelative) + m_localSize.vAbsolute;
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
        Core::Vector2 Widget::GetRelativeReferenceSize() const
        {
            CS_ASSERT(m_canvas != nullptr, "Cannot get the absolute size of widget without attaching it to the canvas");
            
            if(m_parent != nullptr)
            {
                return m_parent->CalculateRelativeReferenceSizeForChild(this);
            }
            else
            {
                return m_canvas->GetFinalSize();
            }
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
        Core::Vector2 Widget::CalculateRelativeReferenceSizeForChild(const Widget* in_child)
        {
            auto layoutPair = GetLayoutForChild(in_child);
            
            if(layoutPair.first == nullptr)
            {
                return (GetFinalSize());
            }
            
            CS_ASSERT(layoutPair.second >= 0, "Cannot find child");
            return layoutPair.first->GetSizeForIndex((u32)layoutPair.second);
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        std::pair<ILayout*, s32> Widget::GetLayoutForChild(const Widget* in_child)
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
            
            return std::make_pair(layout, childIndex);
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
        void Widget::Update(f32 in_timeSinceLastUpdate)
        {
            if(m_behaviourScript != nullptr)
            {
                m_behaviourScript->CallFunction("onUpdate", Scripting::LuaScript::FunctionNotFoundPolicy::k_failSilent, in_timeSinceLastUpdate);
            }
            
            m_internalChildren.lock();
            for(auto& child : m_internalChildren)
            {
                child->Update(in_timeSinceLastUpdate);
            }
            m_internalChildren.unlock();
            
            m_children.lock();
            for(auto& child : m_children)
            {
                child->Update(in_timeSinceLastUpdate);
            }
            m_children.unlock();
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
            
            if(m_drawable != nullptr && ShouldCull(GetFinalPositionOfCentre(), finalSize, m_screen->GetResolution()) == false)
            {
                m_drawable->Draw(in_renderer, GetFinalTransform(), finalSize, GetFinalColour());
            }
            
            if(m_isSubviewClippingEnabled == true)
            {
                Core::Vector2 bottomLeftPos = Rendering::GetAnchorPoint(Rendering::AlignmentAnchor::k_bottomLeft, finalSize * 0.5f);
                bottomLeftPos += GetFinalPositionOfCentre();
                
                in_renderer->PushClipBounds(bottomLeftPos, finalSize);
            }
            
            m_internalChildren.lock();
            for(auto& child : m_internalChildren)
            {
                child->Draw(in_renderer);
            }
            m_internalChildren.unlock();
            
            m_children.lock();
            for(auto& child : m_children)
            {
                child->Draw(in_renderer);
            }
            m_children.unlock();
            
            if(m_isSubviewClippingEnabled == true)
            {
                in_renderer->PopClipBounds();
            }
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
        //-----------------------------------------------------------
        /// UI can filter input events to prevent them from being
        /// forwarded to the external app. Input events are
        /// notified from the front most child widget to the back most
        /// and can be consumed.
        //-----------------------------------------------------------
        void Widget::OnPointerDown(const Input::Pointer& in_pointer, f64 in_timestamp, Input::Pointer::InputType in_inputType, Input::Filter& in_filter)
        {
            if(m_isInputEnabled == false)
                return;
            
            m_children.lock();
            for(auto it = m_children.rbegin(); it != m_children.rend(); ++it)
            {
                (*it)->OnPointerDown(in_pointer, in_timestamp, in_inputType, in_filter);
                
                if(in_filter.IsFiltered() == true)
                {
                    m_children.unlock();
                    return;
                }
            }
            m_children.unlock();
            
            m_internalChildren.lock();
            for(auto it = m_internalChildren.rbegin(); it != m_internalChildren.rend(); ++it)
            {
                (*it)->OnPointerDown(in_pointer, in_timestamp, in_inputType, in_filter);
                
                if(in_filter.IsFiltered() == true)
                {
                    m_internalChildren.unlock();
                    return;
                }
            }
            m_internalChildren.unlock();
            
            if(Contains(in_pointer.GetPosition()) == true)
            {
                //Track the input that is down on the widget as
                //this will effect how we trigger the release events
				auto it = m_pressedInput.find(in_pointer.GetId());
				if (it != m_pressedInput.end())
				{
					it->second.insert(in_inputType);
				}
				else
				{
					std::set<Input::Pointer::InputType> inputTypeSet = { in_inputType };
					m_pressedInput.emplace(in_pointer.GetId(), inputTypeSet);
				}
                
                if(m_behaviourScript != nullptr)
                {
                    m_behaviourScript->CallFunction("onPressedInside", Scripting::LuaScript::FunctionNotFoundPolicy::k_failSilent, &in_pointer, in_timestamp, in_inputType);
                }
                
                m_pressedInsideEvent.NotifyConnections(this, in_inputType);
                
                if(m_isInputConsumeEnabled == true)
                {
                    in_filter.SetFiltered();
                }
            }
        }
        //-----------------------------------------------------------
        /// UI can filter input events to prevent them from being
        /// forwarded to the external app.
        //-----------------------------------------------------------
        void Widget::OnPointerMoved(const Input::Pointer& in_pointer, f64 in_timestamp)
        {
            if(m_isInputEnabled == false)
                return;
            
            m_children.lock();
            for(auto it = m_children.rbegin(); it != m_children.rend(); ++it)
            {
                (*it)->OnPointerMoved(in_pointer, in_timestamp);
            }
            m_children.unlock();
            
            m_internalChildren.lock();
            for(auto it = m_internalChildren.rbegin(); it != m_internalChildren.rend(); ++it)
            {
                (*it)->OnPointerMoved(in_pointer, in_timestamp);
            }
            m_internalChildren.unlock();
            
            bool containsPrevious = Contains(in_pointer.GetPreviousPosition());
            bool containsCurrent = Contains(in_pointer.GetPosition());
            
            if(containsPrevious == false && containsCurrent == true)
            {
                if(m_behaviourScript != nullptr)
                {
                    m_behaviourScript->CallFunction("onMoveEntered", Scripting::LuaScript::FunctionNotFoundPolicy::k_failSilent, &in_pointer, in_timestamp);
                }
                
                m_moveEnteredEvent.NotifyConnections(this, in_pointer.GetActiveInputs());
            }
            else if(containsPrevious == true && containsCurrent == false)
            {
                if(m_behaviourScript != nullptr)
                {
                    m_behaviourScript->CallFunction("onMoveExited", Scripting::LuaScript::FunctionNotFoundPolicy::k_failSilent, &in_pointer, in_timestamp);
                }
                
                m_moveExitedEvent.NotifyConnections(this, in_pointer.GetActiveInputs());
            }
            else if(containsPrevious == false && containsCurrent == false)
            {
                auto itPressedInput = m_pressedInput.find(in_pointer.GetId());
				if (itPressedInput != m_pressedInput.end())
                {
                    if(m_behaviourScript != nullptr)
                    {
                        m_behaviourScript->CallFunction("onDraggedOutside", Scripting::LuaScript::FunctionNotFoundPolicy::k_failSilent, &in_pointer, in_timestamp);
                    }
                    
                    m_draggedOutsideEvent.NotifyConnections(this, in_pointer.GetActiveInputs());
                }
            }
            else // Equivalent to if(containsPrevious == true && containsCurrent == true)
            {
				auto itPressedInput = m_pressedInput.find(in_pointer.GetId());
				if (itPressedInput != m_pressedInput.end())
                {
                    if(m_behaviourScript != nullptr)
                    {
                        m_behaviourScript->CallFunction("onDraggedInside", Scripting::LuaScript::FunctionNotFoundPolicy::k_failSilent, &in_pointer, in_timestamp);
                    }
                    
                    m_draggedInsideEvent.NotifyConnections(this, in_pointer.GetActiveInputs());
                }
            }
        }
        //-----------------------------------------------------------
        /// UI can filter input events to prevent them from being
        /// forwarded to the external app.
        //-----------------------------------------------------------
        void Widget::OnPointerUp(const Input::Pointer& in_pointer, f64 in_timestamp, Input::Pointer::InputType in_inputType)
        {
            if(m_isInputEnabled == false)
                return;
            
            m_children.lock();
            for(auto it = m_children.rbegin(); it != m_children.rend(); ++it)
            {
                (*it)->OnPointerUp(in_pointer, in_timestamp, in_inputType);
            }
            m_children.unlock();
            
            m_internalChildren.lock();
            for(auto it = m_internalChildren.rbegin(); it != m_internalChildren.rend(); ++it)
            {
                (*it)->OnPointerUp(in_pointer, in_timestamp, in_inputType);
            }
            m_internalChildren.unlock();
            
            auto itPressedInput = m_pressedInput.find(in_pointer.GetId());
			if (itPressedInput != m_pressedInput.end())
            {
				auto itPressedInputType = itPressedInput->second.find(in_inputType);
				if (itPressedInputType != itPressedInput->second.end())
				{
					itPressedInput->second.erase(itPressedInputType);
					if (itPressedInput->second.empty() == true)
					{
						m_pressedInput.erase(itPressedInput);
					}

					if(Contains(in_pointer.GetPosition()) == true)
					{
						if(m_behaviourScript != nullptr)
						{
							m_behaviourScript->CallFunction("onReleasedInside", Scripting::LuaScript::FunctionNotFoundPolicy::k_failSilent, &in_pointer, in_timestamp, in_inputType);
						}
                    
						m_releasedInsideEvent.NotifyConnections(this, in_inputType);
					}
					else
					{
						if(m_behaviourScript != nullptr)
						{
							m_behaviourScript->CallFunction("onReleasedOutside", Scripting::LuaScript::FunctionNotFoundPolicy::k_failSilent, &in_pointer, in_timestamp, in_inputType);
						}
                    
						m_releasedOutsideEvent.NotifyConnections(this, in_inputType);
					}
				}
            }
        }
    }
}
