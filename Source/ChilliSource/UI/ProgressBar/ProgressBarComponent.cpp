//
//  ProgressBarComponent.cpp
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

#include <ChilliSource/UI/ProgressBar/ProgressBarComponent.h>

#include <ChilliSource/Core/Delegate/MakeDelegate.h>
#include <ChilliSource/Core/Container/Property/PropertyTypes.h>
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
            const char k_relBarSizeKey[] = "RelBarSize";
            const char k_progressKey[] = "Progress";
            const char k_barTypeKey[] = "BarType";
            const char k_barDirectionKey[] = "BarDirection";
            const char k_barWidgetName[] = "BarWidgetName";
            
            const std::vector<Core::PropertyMap::PropertyDesc> k_propertyDescs =
            {
                {Core::PropertyTypes::Vector2(), k_relBarSizeKey},
                {Core::PropertyTypes::Float(), k_progressKey},
                {PropertyTypes::ProgressBarType(), k_barTypeKey},
                {PropertyTypes::ProgressBarDirection(), k_barDirectionKey},
                {Core::PropertyTypes::String(), k_barWidgetName}
            };
        }
        
        CS_DEFINE_NAMEDTYPE(ProgressBarComponent);
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        const std::vector<Core::PropertyMap::PropertyDesc>& ProgressBarComponent::GetPropertyDescs()
        {
            return k_propertyDescs;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        ProgressBarComponent::ProgressBarComponent(const std::string& in_componentName, const Core::PropertyMap& in_properties)
            : Component(in_componentName), m_relBarSize(1.0f, 1.0f)
        {
            RegisterProperty<Core::Vector2>(Core::PropertyTypes::Vector2(), k_relBarSizeKey, CSCore::MakeDelegate(this, &ProgressBarComponent::GetRelativeBarSize), CSCore::MakeDelegate(this, &ProgressBarComponent::SetRelativeBarSize));
            RegisterProperty<f32>(Core::PropertyTypes::Float(), k_progressKey, CSCore::MakeDelegate(this, &ProgressBarComponent::GetProgress), CSCore::MakeDelegate(this, &ProgressBarComponent::SetProgress));
            RegisterProperty<ProgressBarType>(PropertyTypes::ProgressBarType(), k_barTypeKey, CSCore::MakeDelegate(this, &ProgressBarComponent::GetBarType), CSCore::MakeDelegate(this, &ProgressBarComponent::SetBarType));
            RegisterProperty<ProgressBarDirection>(PropertyTypes::ProgressBarDirection(), k_barDirectionKey, CSCore::MakeDelegate(this, &ProgressBarComponent::GetBarDirection), CSCore::MakeDelegate(this, &ProgressBarComponent::SetBarDirection));
            RegisterProperty<std::string>(Core::PropertyTypes::String(), k_barWidgetName, CSCore::MakeDelegate(this, &ProgressBarComponent::GetBarWidgetName), CSCore::MakeDelegate(this, &ProgressBarComponent::SetBarWidgetName));
            ApplyRegisteredProperties(in_properties);
            
            CS_ASSERT(m_barWidgetName.empty() == false, "Bar widget name must be set on a progress bar component.");
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        bool ProgressBarComponent::IsA(Core::InterfaceIDType in_interfaceId) const
        {
            return (Component::InterfaceID == in_interfaceId || ProgressBarComponent::InterfaceID == in_interfaceId);
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        const Core::Vector2& ProgressBarComponent::GetRelativeBarSize() const
        {
            return m_relBarSize;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        f32 ProgressBarComponent::GetProgress() const
        {
            return m_progress;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        ProgressBarType ProgressBarComponent::GetBarType() const
        {
            return m_barType;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        ProgressBarDirection ProgressBarComponent::GetBarDirection() const
        {
            return m_direction;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        const std::string& ProgressBarComponent::GetBarWidgetName() const
        {
            return m_barWidgetName;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void ProgressBarComponent::SetRelativeBarSize(const Core::Vector2& in_relSize)
        {
            m_relBarSize = in_relSize;
            
            UpdateBar();
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void ProgressBarComponent::SetProgress(f32 in_progress)
        {
            CS_ASSERT(in_progress >= 0.0f && in_progress <= 1.0f, "Progress must be in the range: 0.0 - 1.0");
            
            m_progress = in_progress;
            
            UpdateBar();
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void ProgressBarComponent::SetBarType(ProgressBarType in_barType)
        {
            m_barType = in_barType;
            
            UpdateBar();
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void ProgressBarComponent::SetBarDirection(ProgressBarDirection in_direction)
        {
            m_direction = in_direction;
            
            UpdateBar();
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void ProgressBarComponent::SetBarWidgetName(const std::string& in_name)
        {
            CS_ASSERT(m_barWidgetName.empty(), "Cannot change the bar widget on a progress bar component.");
            
            m_barWidgetName = in_name;
            
            UpdateBar();
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void ProgressBarComponent::UpdateBar()
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
        void ProgressBarComponent::UpdateHorizontalBar()
        {
            CSCore::Vector2 relBarPosition = CSCore::Vector2(0.5f * (1.0f - m_relBarSize.x), 0.0f);
            CSCore::Vector2 relBarSize = m_relBarSize;
            relBarSize.x *= m_progress;
            
            m_barWidget->SetParentalAnchor(Rendering::AlignmentAnchor::k_middleLeft);
            m_barWidget->SetOriginAnchor(Rendering::AlignmentAnchor::k_middleLeft);
            m_barWidget->SetRelativePosition(relBarPosition);
            m_barWidget->SetRelativeSize(relBarSize);
            
            if (m_barType == ProgressBarType::k_fill)
            {
                CS_ASSERT(m_barDrawableComponent != nullptr, "Fill type progress bar cannot have a bar widget without a drawable component.");
                
                if (m_barDrawableComponent->GetDrawable() != nullptr)
                {
                    m_barDrawableComponent->GetDrawable()->SetUVs(Rendering::UVs(0.0f, 0.0f, m_progress, 1.0f));
                }
            }
            else if (m_barDrawableComponent != nullptr && m_barDrawableComponent->GetDrawable() != nullptr)
            {
                m_barDrawableComponent->GetDrawable()->SetUVs(Rendering::UVs(0.0f, 0.0f, 1.0f, 1.0f));
            }
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void ProgressBarComponent::UpdateVerticalBar()
        {
            CSCore::Vector2 relBarPosition = CSCore::Vector2(0.0f, 0.5f * (1.0f - m_relBarSize.y));
            CSCore::Vector2 relBarSize = m_relBarSize;
            relBarSize.y *= m_progress;
            
            m_barWidget->SetParentalAnchor(Rendering::AlignmentAnchor::k_bottomCentre);
            m_barWidget->SetOriginAnchor(Rendering::AlignmentAnchor::k_bottomCentre);
            m_barWidget->SetRelativePosition(relBarPosition);
            m_barWidget->SetRelativeSize(relBarSize);
            
            if (m_barType == ProgressBarType::k_fill)
            {
                CS_ASSERT(m_barDrawableComponent != nullptr, "Fill type progress bar cannot have a bar widget without a drawable component.");
                
                if (m_barDrawableComponent->GetDrawable() != nullptr)
                {
                    m_barDrawableComponent->GetDrawable()->SetUVs(Rendering::UVs(0.0f, 1.0f - m_progress, 1.0f, m_progress));
                }
            }
            else if (m_barDrawableComponent != nullptr && m_barDrawableComponent->GetDrawable() != nullptr)
            {
                m_barDrawableComponent->GetDrawable()->SetUVs(Rendering::UVs(0.0f, 0.0f, 1.0f, 1.0f));
            }
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void ProgressBarComponent::OnInit()
        {
            m_barWidget = GetWidget()->GetInternalWidget(m_barWidgetName);
            CS_ASSERT(m_barWidget != nullptr, "Could not find bar widget with name: " + m_barWidgetName);
            
            m_barDrawableComponent = m_barWidget->GetComponent<DrawableComponent>();
            
            UpdateBar();
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void ProgressBarComponent::OnDestroy()
        {
        }
    }
}