//
//  SliderUIComponent.cpp
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

#include <ChilliSource/UI/Slider/SliderUIComponent.h>

#include <ChilliSource/Core/Delegate/MakeDelegate.h>
#include <ChilliSource/Core/Container/VectorUtils.h>
#include <ChilliSource/Core/Container/Property/PropertyTypes.h>
#include <ChilliSource/Core/Math/MathUtils.h>
#include <ChilliSource/Rendering/Texture/UVs.h>
#include <ChilliSource/UI/Base/PropertyTypes.h>
#include <ChilliSource/UI/Base/Widget.h>
#include <ChilliSource/UI/Drawable/UIDrawable.h>
#include <ChilliSource/UI/Drawable/DrawableUIComponent.h>

namespace ChilliSource
{
    namespace
    {
        const char k_sliderSizeKey[] = "SliderSize";
        const char k_sliderPositionKey[] = "SliderPosition";
        const char k_sliderDirectionKey[] = "SliderDirection";
        const char k_sliderWidgetName[] = "SliderWidgetName";
        
        const std::vector<PropertyMap::PropertyDesc> k_propertyDescs =
        {
            {PropertyTypes::Float(), k_sliderSizeKey},
            {PropertyTypes::Float(), k_sliderPositionKey},
            {PropertyTypes::SliderDirection(), k_sliderDirectionKey},
            {PropertyTypes::String(), k_sliderWidgetName}
        };
    }
    
    CS_DEFINE_NAMEDTYPE(SliderUIComponent);
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    const std::vector<PropertyMap::PropertyDesc>& SliderUIComponent::GetPropertyDescs()
    {
        return k_propertyDescs;
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    SliderUIComponent::SliderUIComponent(const std::string& in_componentName, const PropertyMap& in_properties)
        : UIComponent(in_componentName)
    {
        RegisterProperty<f32>(PropertyTypes::Float(), k_sliderSizeKey, MakeDelegate(this, &SliderUIComponent::GetSliderSize), MakeDelegate(this, &SliderUIComponent::SetSliderSize));
        RegisterProperty<f32>(PropertyTypes::Float(), k_sliderPositionKey, MakeDelegate(this, &SliderUIComponent::GetSliderPosition), MakeDelegate(this, &SliderUIComponent::SetSliderPosition));
        RegisterProperty<SliderDirection>(PropertyTypes::SliderDirection(), k_sliderDirectionKey, MakeDelegate(this, &SliderUIComponent::GetSliderDirection), MakeDelegate(this, &SliderUIComponent::SetSliderDirection));
        RegisterProperty<std::string>(PropertyTypes::String(), k_sliderWidgetName, MakeDelegate(this, &SliderUIComponent::GetSliderWidgetName), MakeDelegate(this, &SliderUIComponent::SetSliderWidgetName));
        ApplyRegisteredProperties(in_properties);
        
        CS_ASSERT(m_sliderWidgetName.empty() == false, "Slider widget name must be set on a Slider component.");
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    bool SliderUIComponent::IsA(InterfaceIDType in_interfaceId) const
    {
        return (UIComponent::InterfaceID == in_interfaceId || SliderUIComponent::InterfaceID == in_interfaceId);
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    f32 SliderUIComponent::GetSliderSize() const
    {
        return m_sliderSize;
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    f32 SliderUIComponent::GetSliderPosition() const
    {
        return m_sliderPos;
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    SliderDirection SliderUIComponent::GetSliderDirection() const
    {
        return m_direction;
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    const std::string& SliderUIComponent::GetSliderWidgetName() const
    {
        return m_sliderWidgetName;
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    void SliderUIComponent::SetSliderSize(f32 in_size)
    {
        m_sliderSize = in_size;
        
        UpdateSlider();
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    void SliderUIComponent::SetSliderPosition(f32 in_sliderPos)
    {
        CS_ASSERT(in_sliderPos >= 0.0f && in_sliderPos <= 1.0f, "Slider must be in the range: 0.0 - 1.0");
        
        m_sliderPos = in_sliderPos;
        
        UpdateSlider();
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    void SliderUIComponent::SetSliderDirection(SliderDirection in_direction)
    {
        m_direction = in_direction;
        
        UpdateSlider();
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    void SliderUIComponent::SetSliderWidgetName(const std::string& in_name)
    {
        CS_ASSERT(m_sliderWidgetName.empty(), "Cannot change the bar widget on a Slider component.");
        
        m_sliderWidgetName = in_name;
        
        UpdateSlider();
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    void SliderUIComponent::UpdateSlider()
    {
        if (m_sliderWidget != nullptr && GetWidget() != nullptr && GetWidget()->IsOnCanvas() == true)
        {
            if (m_direction == SliderDirection::k_horizontal)
            {
                m_sliderWidget->SetParentalAnchor(AlignmentAnchor::k_middleLeft);
                m_sliderWidget->SetOriginAnchor(AlignmentAnchor::k_middleLeft);
                m_sliderWidget->SetSizePolicy(SizePolicy::k_useHeightMaintainingAspect);
                m_sliderWidget->SetRelativeSize(Vector2(0.0f, m_sliderSize));
                
                f32 padding = m_sliderWidget->GetFinalSize().x / GetWidget()->GetFinalSize().x;
                m_sliderWidget->SetRelativePosition(Vector2(m_sliderPos * (1.0f - padding), 0.0f));
            }
            else
            {
                m_sliderWidget->SetParentalAnchor(AlignmentAnchor::k_bottomCentre);
                m_sliderWidget->SetOriginAnchor(AlignmentAnchor::k_bottomCentre);
                m_sliderWidget->SetSizePolicy(SizePolicy::k_useWidthMaintainingAspect);
                m_sliderWidget->SetRelativeSize(Vector2(m_sliderSize, 0.0f));
                
                f32 padding = m_sliderWidget->GetFinalSize().y / GetWidget()->GetFinalSize().y;
                m_sliderWidget->SetRelativePosition(Vector2(0.0f, m_sliderPos * (1.0f - padding)));
            }
        }
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void SliderUIComponent::UpdateSliderFromScreenPosition(const Vector2& in_screenSpacePos)
    {
        CS_ASSERT(GetWidget() != nullptr, "Must have an owning widget to update slider from screen position.");
        CS_ASSERT(GetWidget()->IsOnCanvas() == true, "Owning widget must be on the canvas to update slider from screen position.");
        
        Vector2 localSpacePoint = GetWidget()->ToLocalSpace(in_screenSpacePos, AlignmentAnchor::k_middleCentre);
        Vector2 ownerSize = GetWidget()->GetFinalSize();
        
        if (m_direction == SliderDirection::k_horizontal)
        {
            f32 padding = m_sliderWidget->GetFinalSize().x;
            f32 range = ownerSize.x - padding;
            f32 pointClamped = MathUtils::Clamp(0.5f * range + localSpacePoint.x, 0.0f, range);
            f32 relativePoint = pointClamped / range;
            SetSliderPosition(relativePoint);
        }
        else
        {
            f32 padding = m_sliderWidget->GetFinalSize().y;
            f32 range = ownerSize.y - padding;
            f32 pointClamped = MathUtils::Clamp(0.5f * range + localSpacePoint.y, 0.0f, range);
            f32 relativePoint = pointClamped / range;
            SetSliderPosition(relativePoint);
        }
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    void SliderUIComponent::OnInit()
    {
        m_sliderWidget = GetWidget()->GetInternalWidget(m_sliderWidgetName);
        CS_ASSERT(m_sliderWidget != nullptr, "Could not find bar widget with name: " + m_sliderWidgetName);
        
        m_pressedInsideConnection = GetWidget()->GetPressedInsideEvent().OpenConnection(MakeDelegate(this, &SliderUIComponent::OnPressedInside));
        m_draggedInsideConnection = GetWidget()->GetDraggedInsideEvent().OpenConnection(MakeDelegate(this, &SliderUIComponent::OnDraggedInside));
        m_draggedOutsideConnection = GetWidget()->GetDraggedOutsideEvent().OpenConnection(MakeDelegate(this, &SliderUIComponent::OnDraggedOutside));
        m_releasedInsideConnection = GetWidget()->GetReleasedInsideEvent().OpenConnection(MakeDelegate(this, &SliderUIComponent::OnReleasedInside));
        m_releasedOutsideConnection = GetWidget()->GetReleasedOutsideEvent().OpenConnection(MakeDelegate(this, &SliderUIComponent::OnReleasedOutside));
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void SliderUIComponent::OnAddedToCanvas()
    {
        UpdateSlider();
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void SliderUIComponent::OnPressedInside(Widget* in_widget, const Pointer& in_pointer, Pointer::InputType in_inputType)
    {
        if (in_inputType == Pointer::GetDefaultInputType())
        {
            m_activePointerIds.push_back(in_pointer.GetId());
            UpdateSliderFromScreenPosition(in_pointer.GetPosition());
        }
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void SliderUIComponent::OnDraggedInside(Widget* in_widget, const Pointer& in_pointer)
    {
        if (VectorUtils::Contains(m_activePointerIds, in_pointer.GetId()) == true)
        {
            UpdateSliderFromScreenPosition(in_pointer.GetPosition());
        }
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void SliderUIComponent::OnDraggedOutside(Widget* in_widget, const Pointer& in_pointer)
    {
        if (VectorUtils::Contains(m_activePointerIds, in_pointer.GetId()) == true)
        {
            UpdateSliderFromScreenPosition(in_pointer.GetPosition());
        }
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void SliderUIComponent::OnReleasedInside(Widget* in_widget, const Pointer& in_pointer, Pointer::InputType in_inputType)
    {
        if (VectorUtils::Contains(m_activePointerIds, in_pointer.GetId()) == true)
        {
            UpdateSliderFromScreenPosition(in_pointer.GetPosition());
            VectorUtils::Remove(m_activePointerIds, in_pointer.GetId());
        }
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void SliderUIComponent::OnReleasedOutside(Widget* in_widget, const Pointer& in_pointer, Pointer::InputType in_inputType)
    {
        if (VectorUtils::Contains(m_activePointerIds, in_pointer.GetId()) == true)
        {
            UpdateSliderFromScreenPosition(in_pointer.GetPosition());
            VectorUtils::Remove(m_activePointerIds, in_pointer.GetId());
        }
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    void SliderUIComponent::OnDestroy()
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