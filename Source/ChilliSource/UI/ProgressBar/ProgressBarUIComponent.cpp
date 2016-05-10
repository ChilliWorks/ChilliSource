//
//  ProgressBarUIComponent.cpp
//  Chilli Source
//  Created by Ian Copland on 01/12/2014.
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

#include <ChilliSource/UI/ProgressBar/ProgressBarUIComponent.h>

#include <ChilliSource/Core/Delegate/MakeDelegate.h>
#include <ChilliSource/Core/Container/Property/PropertyTypes.h>
#include <ChilliSource/Rendering/Texture/UVs.h>
#include <ChilliSource/UI/Base/PropertyTypes.h>
#include <ChilliSource/UI/Base/Widget.h>
#include <ChilliSource/UI/Drawable/UIDrawable.h>
#include <ChilliSource/UI/Drawable/DrawableUIComponent.h>

namespace ChilliSource
{
    namespace
    {
        const char k_relBarSizeKey[] = "RelBarSize";
        const char k_progressKey[] = "Progress";
        const char k_barTypeKey[] = "BarType";
        const char k_barDirectionKey[] = "BarDirection";
        const char k_barWidgetName[] = "BarWidgetName";
        
        const std::vector<PropertyMap::PropertyDesc> k_propertyDescs =
        {
            {PropertyTypes::Vector2(), k_relBarSizeKey},
            {PropertyTypes::Float(), k_progressKey},
            {PropertyTypes::ProgressBarType(), k_barTypeKey},
            {PropertyTypes::ProgressBarDirection(), k_barDirectionKey},
            {PropertyTypes::String(), k_barWidgetName}
        };
    }
    
    CS_DEFINE_NAMEDTYPE(ProgressBarUIComponent);
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    const std::vector<PropertyMap::PropertyDesc>& ProgressBarUIComponent::GetPropertyDescs()
    {
        return k_propertyDescs;
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    ProgressBarUIComponent::ProgressBarUIComponent(const std::string& in_componentName, const PropertyMap& in_properties)
        : UIComponent(in_componentName), m_relBarSize(1.0f, 1.0f)
    {
        RegisterProperty<Vector2>(PropertyTypes::Vector2(), k_relBarSizeKey, MakeDelegate(this, &ProgressBarUIComponent::GetRelativeBarSize), MakeDelegate(this, &ProgressBarUIComponent::SetRelativeBarSize));
        RegisterProperty<f32>(PropertyTypes::Float(), k_progressKey, MakeDelegate(this, &ProgressBarUIComponent::GetProgress), MakeDelegate(this, &ProgressBarUIComponent::SetProgress));
        RegisterProperty<ProgressBarType>(PropertyTypes::ProgressBarType(), k_barTypeKey, MakeDelegate(this, &ProgressBarUIComponent::GetBarType), MakeDelegate(this, &ProgressBarUIComponent::SetBarType));
        RegisterProperty<ProgressBarDirection>(PropertyTypes::ProgressBarDirection(), k_barDirectionKey, MakeDelegate(this, &ProgressBarUIComponent::GetBarDirection), MakeDelegate(this, &ProgressBarUIComponent::SetBarDirection));
        RegisterProperty<std::string>(PropertyTypes::String(), k_barWidgetName, MakeDelegate(this, &ProgressBarUIComponent::GetBarWidgetName), MakeDelegate(this, &ProgressBarUIComponent::SetBarWidgetName));
        ApplyRegisteredProperties(in_properties);
        
        CS_ASSERT(m_barWidgetName.empty() == false, "Bar widget name must be set on a progress bar component.");
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    bool ProgressBarUIComponent::IsA(InterfaceIDType in_interfaceId) const
    {
        return (UIComponent::InterfaceID == in_interfaceId || ProgressBarUIComponent::InterfaceID == in_interfaceId);
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    const Vector2& ProgressBarUIComponent::GetRelativeBarSize() const
    {
        return m_relBarSize;
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    f32 ProgressBarUIComponent::GetProgress() const
    {
        return m_progress;
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    ProgressBarType ProgressBarUIComponent::GetBarType() const
    {
        return m_barType;
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    ProgressBarDirection ProgressBarUIComponent::GetBarDirection() const
    {
        return m_direction;
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    const std::string& ProgressBarUIComponent::GetBarWidgetName() const
    {
        return m_barWidgetName;
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    void ProgressBarUIComponent::SetRelativeBarSize(const Vector2& in_relSize)
    {
        m_relBarSize = in_relSize;
        
        UpdateBar();
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    void ProgressBarUIComponent::SetProgress(f32 in_progress)
    {
        CS_ASSERT(in_progress >= 0.0f && in_progress <= 1.0f, "Progress must be in the range: 0.0 - 1.0");
        
        m_progress = in_progress;
        
        UpdateBar();
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    void ProgressBarUIComponent::SetBarType(ProgressBarType in_barType)
    {
        m_barType = in_barType;
        
        UpdateBar();
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    void ProgressBarUIComponent::SetBarDirection(ProgressBarDirection in_direction)
    {
        m_direction = in_direction;
        
        UpdateBar();
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    void ProgressBarUIComponent::SetBarWidgetName(const std::string& in_name)
    {
        CS_ASSERT(m_barWidgetName.empty(), "Cannot change the bar widget on a progress bar component.");
        
        m_barWidgetName = in_name;
        
        UpdateBar();
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    void ProgressBarUIComponent::UpdateBar()
    {
        if (m_barWidget != nullptr)
        {
            if (m_direction == ProgressBarDirection::k_horizontal)
            {
                UpdateHorizontalBar();
            }
            else
            {
                UpdateVerticalBar();
            }
        }
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    void ProgressBarUIComponent::UpdateHorizontalBar()
    {
        Vector2 relBarPosition = Vector2(0.5f * (1.0f - m_relBarSize.x), 0.0f);
        Vector2 relBarSize = m_relBarSize;
        relBarSize.x *= m_progress;
        
        m_barWidget->SetParentalAnchor(AlignmentAnchor::k_middleLeft);
        m_barWidget->SetOriginAnchor(AlignmentAnchor::k_middleLeft);
        m_barWidget->SetRelativePosition(relBarPosition);
        m_barWidget->SetRelativeSize(relBarSize);
        
        if (m_barType == ProgressBarType::k_fill)
        {
            CS_ASSERT(m_barDrawableComponent != nullptr, "Fill type progress bar cannot have a bar widget without a drawable component.");
            
            if (m_barDrawableComponent->GetDrawable() != nullptr)
            {
                m_barDrawableComponent->GetDrawable()->SetUVs(UVs(0.0f, 0.0f, m_progress, 1.0f));
            }
        }
        else if (m_barDrawableComponent != nullptr && m_barDrawableComponent->GetDrawable() != nullptr)
        {
            m_barDrawableComponent->GetDrawable()->SetUVs(UVs(0.0f, 0.0f, 1.0f, 1.0f));
        }
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    void ProgressBarUIComponent::UpdateVerticalBar()
    {
        Vector2 relBarPosition = Vector2(0.0f, 0.5f * (1.0f - m_relBarSize.y));
        Vector2 relBarSize = m_relBarSize;
        relBarSize.y *= m_progress;
        
        m_barWidget->SetParentalAnchor(AlignmentAnchor::k_bottomCentre);
        m_barWidget->SetOriginAnchor(AlignmentAnchor::k_bottomCentre);
        m_barWidget->SetRelativePosition(relBarPosition);
        m_barWidget->SetRelativeSize(relBarSize);
        
        if (m_barType == ProgressBarType::k_fill)
        {
            CS_ASSERT(m_barDrawableComponent != nullptr, "Fill type progress bar cannot have a bar widget without a drawable component.");
            
            if (m_barDrawableComponent->GetDrawable() != nullptr)
            {
                m_barDrawableComponent->GetDrawable()->SetUVs(UVs(0.0f, 1.0f - m_progress, 1.0f, m_progress));
            }
        }
        else if (m_barDrawableComponent != nullptr && m_barDrawableComponent->GetDrawable() != nullptr)
        {
            m_barDrawableComponent->GetDrawable()->SetUVs(UVs(0.0f, 0.0f, 1.0f, 1.0f));
        }
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    void ProgressBarUIComponent::OnInit()
    {
        m_barWidget = GetWidget()->GetInternalWidget(m_barWidgetName);
        CS_ASSERT(m_barWidget != nullptr, "Could not find bar widget with name: " + m_barWidgetName);
        
        m_barDrawableComponent = m_barWidget->GetComponent<DrawableUIComponent>();
        
        UpdateBar();
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    void ProgressBarUIComponent::OnDestroy()
    {
    }
}