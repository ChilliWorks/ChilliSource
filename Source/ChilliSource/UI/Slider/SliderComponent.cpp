//
//  SliderComponent.cpp
//  Chilli Source
//  Created by Ian Copland on 18/12/2014.
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

#include <ChilliSource/UI/Slider/SliderComponent.h>

#include <ChilliSource/Core/Delegate/MakeDelegate.h>
#include <ChilliSource/Core/Container/VectorUtils.h>
#include <ChilliSource/Core/Container/Property/PropertyTypes.h>
#include <ChilliSource/Core/Math/MathUtils.h>
#include <ChilliSource/Rendering/Texture/UVs.h>
#include <ChilliSource/UI/Base/PropertyTypes.h>
#include <ChilliSource/UI/Base/Widget.h>
#include <ChilliSource/UI/Drawable/Drawable.h>
#include <ChilliSource/UI/Drawable/DrawableComponent.h>

namespace ChilliSource
{
    namespace UI
    {
        namespace
        {
            const char k_sliderSizeKey[] = "SliderSize";
            const char k_sliderPositionKey[] = "SliderPosition";
            const char k_sliderDirectionKey[] = "SliderDirection";
            const char k_sliderWidgetName[] = "SliderWidgetName";
            
            const std::vector<Core::PropertyMap::PropertyDesc> k_propertyDescs =
            {
                {Core::PropertyTypes::Float(), k_sliderSizeKey},
                {Core::PropertyTypes::Float(), k_sliderPositionKey},
                {PropertyTypes::SliderDirection(), k_sliderDirectionKey},
                {Core::PropertyTypes::String(), k_sliderWidgetName}
            };
        }
        
        CS_DEFINE_NAMEDTYPE(SliderComponent);
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        const std::vector<Core::PropertyMap::PropertyDesc>& SliderComponent::GetPropertyDescs()
        {
            return k_propertyDescs;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        SliderComponent::SliderComponent(const std::string& in_componentName, const Core::PropertyMap& in_properties)
            : Component(in_componentName)
        {
            RegisterProperty<f32>(Core::PropertyTypes::Float(), k_sliderSizeKey, CSCore::MakeDelegate(this, &SliderComponent::GetSliderSize), CSCore::MakeDelegate(this, &SliderComponent::SetSliderSize));
            RegisterProperty<f32>(Core::PropertyTypes::Float(), k_sliderPositionKey, CSCore::MakeDelegate(this, &SliderComponent::GetSliderPosition), CSCore::MakeDelegate(this, &SliderComponent::SetSliderPosition));
            RegisterProperty<SliderDirection>(PropertyTypes::SliderDirection(), k_sliderDirectionKey, CSCore::MakeDelegate(this, &SliderComponent::GetSliderDirection), CSCore::MakeDelegate(this, &SliderComponent::SetSliderDirection));
            RegisterProperty<std::string>(Core::PropertyTypes::String(), k_sliderWidgetName, CSCore::MakeDelegate(this, &SliderComponent::GetSliderWidgetName), CSCore::MakeDelegate(this, &SliderComponent::SetSliderWidgetName));
            ApplyRegisteredProperties(in_properties);
            
            CS_ASSERT(m_sliderWidgetName.empty() == false, "Slider widget name must be set on a Slider component.");
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        bool SliderComponent::IsA(Core::InterfaceIDType in_interfaceId) const
        {
            return (Component::InterfaceID == in_interfaceId || SliderComponent::InterfaceID == in_interfaceId);
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        f32 SliderComponent::GetSliderSize() const
        {
            return m_sliderSize;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        f32 SliderComponent::GetSliderPosition() const
        {
            return m_sliderPos;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        SliderDirection SliderComponent::GetSliderDirection() const
        {
            return m_direction;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        const std::string& SliderComponent::GetSliderWidgetName() const
        {
            return m_sliderWidgetName;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void SliderComponent::SetSliderSize(f32 in_size)
        {
            m_sliderSize = in_size;
            
            UpdateSlider();
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void SliderComponent::SetSliderPosition(f32 in_sliderPos)
        {
            CS_ASSERT(in_sliderPos >= 0.0f && in_sliderPos <= 1.0f, "Slider must be in the range: 0.0 - 1.0");
            
            m_sliderPos = in_sliderPos;
            
            UpdateSlider();
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void SliderComponent::SetSliderDirection(SliderDirection in_direction)
        {
            m_direction = in_direction;
            
            UpdateSlider();
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void SliderComponent::SetSliderWidgetName(const std::string& in_name)
        {
            CS_ASSERT(m_sliderWidgetName.empty(), "Cannot change the bar widget on a Slider component.");
            
            m_sliderWidgetName = in_name;
            
            UpdateSlider();
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void SliderComponent::UpdateSlider()
        {
            if (m_sliderWidget != nullptr && GetWidget() != nullptr && GetWidget()->IsOnCanvas() == true)
            {
                if (m_direction == SliderDirection::k_horizontal)
                {
                    m_sliderWidget->SetParentalAnchor(Rendering::AlignmentAnchor::k_middleLeft);
                    m_sliderWidget->SetOriginAnchor(Rendering::AlignmentAnchor::k_middleLeft);
                    m_sliderWidget->SetSizePolicy(CSUI::SizePolicy::k_useHeightMaintainingAspect);
                    m_sliderWidget->SetRelativeSize(Core::Vector2(0.0f, m_sliderSize));
                    
                    f32 padding = m_sliderWidget->GetFinalSize().x / GetWidget()->GetFinalSize().x;
                    m_sliderWidget->SetRelativePosition(Core::Vector2(m_sliderPos * (1.0f - padding), 0.0f));
                }
                else
                {
                    m_sliderWidget->SetParentalAnchor(Rendering::AlignmentAnchor::k_bottomCentre);
                    m_sliderWidget->SetOriginAnchor(Rendering::AlignmentAnchor::k_bottomCentre);
                    m_sliderWidget->SetSizePolicy(CSUI::SizePolicy::k_useWidthMaintainingAspect);
                    m_sliderWidget->SetRelativeSize(Core::Vector2(m_sliderSize, 0.0f));
                    
                    f32 padding = m_sliderWidget->GetFinalSize().y / GetWidget()->GetFinalSize().y;
                    m_sliderWidget->SetRelativePosition(Core::Vector2(0.0f, m_sliderPos * (1.0f - padding)));
                }
            }
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void SliderComponent::UpdateSliderFromScreenPosition(const Core::Vector2& in_screenSpacePos)
        {
            CS_ASSERT(GetWidget() != nullptr, "Must have an owning widget to update slider from screen position.");
            CS_ASSERT(GetWidget()->IsOnCanvas() == true, "Owning widget must be on the canvas to update slider from screen position.");
            
            Core::Vector2 localSpacePoint = GetWidget()->ToLocalSpace(in_screenSpacePos, Rendering::AlignmentAnchor::k_middleCentre);
            Core::Vector2 ownerSize = GetWidget()->GetFinalSize();
            
            if (m_direction == SliderDirection::k_horizontal)
            {
                f32 padding = m_sliderWidget->GetFinalSize().x;
                f32 range = ownerSize.x - padding;
                f32 pointClamped = Core::MathUtils::Clamp(0.5f * range + localSpacePoint.x, 0.0f, range);
                f32 relativePoint = pointClamped / range;
                SetSliderPosition(relativePoint);
            }
            else
            {
                f32 padding = m_sliderWidget->GetFinalSize().y;
                f32 range = ownerSize.y - padding;
                f32 pointClamped = Core::MathUtils::Clamp(0.5f * range + localSpacePoint.y, 0.0f, range);
                f32 relativePoint = pointClamped / range;
                SetSliderPosition(relativePoint);
            }
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void SliderComponent::OnInit()
        {
            m_sliderWidget = GetWidget()->GetInternalWidget(m_sliderWidgetName);
            CS_ASSERT(m_sliderWidget != nullptr, "Could not find bar widget with name: " + m_sliderWidgetName);
            
            m_pressedInsideConnection = GetWidget()->GetPressedInsideEvent().OpenConnection(Core::MakeDelegate(this, &SliderComponent::OnPressedInside));
            m_draggedInsideConnection = GetWidget()->GetDraggedInsideEvent().OpenConnection(Core::MakeDelegate(this, &SliderComponent::OnDraggedInside));
            m_draggedOutsideConnection = GetWidget()->GetDraggedOutsideEvent().OpenConnection(Core::MakeDelegate(this, &SliderComponent::OnDraggedOutside));
            m_releasedInsideConnection = GetWidget()->GetReleasedInsideEvent().OpenConnection(Core::MakeDelegate(this, &SliderComponent::OnReleasedInside));
            m_releasedOutsideConnection = GetWidget()->GetReleasedOutsideEvent().OpenConnection(Core::MakeDelegate(this, &SliderComponent::OnReleasedOutside));
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void SliderComponent::OnAddedToCanvas()
        {
            UpdateSlider();
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void SliderComponent::OnPressedInside(Widget* in_widget, const Input::Pointer& in_pointer, Input::Pointer::InputType in_inputType)
        {
            if (in_inputType == Input::Pointer::GetDefaultInputType())
            {
                m_activePointerIds.push_back(in_pointer.GetId());
                UpdateSliderFromScreenPosition(in_pointer.GetPosition());
            }
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void SliderComponent::OnDraggedInside(Widget* in_widget, const Input::Pointer& in_pointer)
        {
            if (Core::VectorUtils::Contains(m_activePointerIds, in_pointer.GetId()) == true)
            {
                UpdateSliderFromScreenPosition(in_pointer.GetPosition());
            }
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void SliderComponent::OnDraggedOutside(Widget* in_widget, const Input::Pointer& in_pointer)
        {
            if (Core::VectorUtils::Contains(m_activePointerIds, in_pointer.GetId()) == true)
            {
                UpdateSliderFromScreenPosition(in_pointer.GetPosition());
            }
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void SliderComponent::OnReleasedInside(Widget* in_widget, const Input::Pointer& in_pointer, Input::Pointer::InputType in_inputType)
        {
            if (Core::VectorUtils::Contains(m_activePointerIds, in_pointer.GetId()) == true)
            {
                UpdateSliderFromScreenPosition(in_pointer.GetPosition());
                Core::VectorUtils::Remove(m_activePointerIds, in_pointer.GetId());
            }
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void SliderComponent::OnReleasedOutside(Widget* in_widget, const Input::Pointer& in_pointer, Input::Pointer::InputType in_inputType)
        {
            if (Core::VectorUtils::Contains(m_activePointerIds, in_pointer.GetId()) == true)
            {
                UpdateSliderFromScreenPosition(in_pointer.GetPosition());
                Core::VectorUtils::Remove(m_activePointerIds, in_pointer.GetId());
            }
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void SliderComponent::OnDestroy()
        {
            m_pressedInsideConnection.reset();
            m_draggedInsideConnection.reset();
            m_draggedOutsideConnection.reset();
            m_releasedInsideConnection.reset();
            m_releasedOutsideConnection.reset();
            
            m_activePointerIds.clear();
            m_sliderWidget = nullptr;
        }
    }
}