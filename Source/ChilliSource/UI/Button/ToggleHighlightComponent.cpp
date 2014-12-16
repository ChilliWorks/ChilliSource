//
//  ToggleHighlightComponent.cpp
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

#include <ChilliSource/UI/Button/ToggleHighlightComponent.h>

#include <ChilliSource/Core/Container/VectorUtils.h>
#include <ChilliSource/Core/Container/Property/PropertyTypes.h>
#include <ChilliSource/Core/Delegate/MakeDelegate.h>
#include <ChilliSource/UI/Base/Widget.h>
#include <ChilliSource/UI/Base/PropertyTypes.h>
#include <ChilliSource/UI/Drawable/DrawableComponent.h>
#include <ChilliSource/UI/Drawable/DrawableDef.h>

namespace ChilliSource
{
    namespace UI
    {
        namespace
        {
            const char k_normalOffDrawableKey[] = "NormalOffDrawable";
            const char k_highlightOffDrawableKey[] = "HighlightOffDrawable";
            const char k_normalOnDrawableKey[] = "NormalOnDrawable";
            const char k_highlightOnDrawableKey[] = "HighlightOnDrawable";
            const char k_toggledOnKey[] = "ToggledOn";
            
            const std::vector<Core::PropertyMap::PropertyDesc> k_propertyDescs =
            {
                {PropertyTypes::DrawableDef(), k_normalOffDrawableKey},
                {PropertyTypes::DrawableDef(), k_highlightOffDrawableKey},
                {PropertyTypes::DrawableDef(), k_normalOnDrawableKey},
                {PropertyTypes::DrawableDef(), k_highlightOnDrawableKey},
                {Core::PropertyTypes::Bool(), k_toggledOnKey}
            };
        }
        
        CS_DEFINE_NAMEDTYPE(ToggleHighlightComponent);
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        const std::vector<Core::PropertyMap::PropertyDesc>& ToggleHighlightComponent::GetPropertyDescs()
        {
            return k_propertyDescs;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        ToggleHighlightComponent::ToggleHighlightComponent(const std::string& in_componentName, const Core::PropertyMap& in_properties)
            : Component(in_componentName)
        {
            RegisterProperty<DrawableDefCSPtr>(PropertyTypes::DrawableDef(), k_normalOffDrawableKey, Core::MakeDelegate(this, &ToggleHighlightComponent::GetNormalOffDrawableDef), Core::MakeDelegate(this, &ToggleHighlightComponent::SetNormalOffDrawableDef));
            RegisterProperty<DrawableDefCSPtr>(PropertyTypes::DrawableDef(), k_highlightOffDrawableKey, Core::MakeDelegate(this, &ToggleHighlightComponent::GetHighlightOffDrawableDef), Core::MakeDelegate(this, &ToggleHighlightComponent::SetHighlightOffDrawableDef));
            RegisterProperty<DrawableDefCSPtr>(PropertyTypes::DrawableDef(), k_normalOnDrawableKey, Core::MakeDelegate(this, &ToggleHighlightComponent::GetNormalOnDrawableDef), Core::MakeDelegate(this, &ToggleHighlightComponent::SetNormalOnDrawableDef));
            RegisterProperty<DrawableDefCSPtr>(PropertyTypes::DrawableDef(), k_highlightOnDrawableKey, Core::MakeDelegate(this, &ToggleHighlightComponent::GetHighlightOnDrawableDef), Core::MakeDelegate(this, &ToggleHighlightComponent::SetHighlightOnDrawableDef));
            RegisterProperty<bool>(Core::PropertyTypes::Bool(), k_toggledOnKey, Core::MakeDelegate(this, &ToggleHighlightComponent::IsToggledOn), Core::MakeDelegate(this, &ToggleHighlightComponent::SetToggleOn));
            ApplyRegisteredProperties(in_properties);
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        bool ToggleHighlightComponent::IsA(Core::InterfaceIDType in_interfaceId) const
        {
            return (Component::InterfaceID == in_interfaceId || ToggleHighlightComponent::InterfaceID == in_interfaceId);
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        const DrawableDefCSPtr& ToggleHighlightComponent::GetNormalOffDrawableDef() const
        {
            return m_normalOffDrawableDef;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        const DrawableDefCSPtr& ToggleHighlightComponent::GetHighlightOffDrawableDef() const
        {
            return m_highlightOffDrawableDef;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        const DrawableDefCSPtr& ToggleHighlightComponent::GetNormalOnDrawableDef() const
        {
            return m_normalOnDrawableDef;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        const DrawableDefCSPtr& ToggleHighlightComponent::GetHighlightOnDrawableDef() const
        {
            return m_normalOffDrawableDef;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        bool ToggleHighlightComponent::IsToggledOn() const
        {
            return (m_state == ToggleHighlightState::k_normalOn || m_state == ToggleHighlightState::k_highlightedOn);
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void ToggleHighlightComponent::SetNormalOffDrawableDef(const DrawableDefCSPtr& in_drawableDef)
        {
            m_normalOffDrawableDef = in_drawableDef;
            
            if (m_drawableComponent != nullptr && m_state == ToggleHighlightState::k_normalOff)
            {
                m_drawableComponent->SetDrawableDef(m_normalOffDrawableDef);
            }
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void ToggleHighlightComponent::SetHighlightOffDrawableDef(const DrawableDefCSPtr& in_drawableDef)
        {
            m_highlightOffDrawableDef = in_drawableDef;
            
            if (m_drawableComponent != nullptr && m_state == ToggleHighlightState::k_highlightedOff)
            {
                m_drawableComponent->SetDrawableDef(m_highlightOffDrawableDef);
            }
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void ToggleHighlightComponent::SetNormalOnDrawableDef(const DrawableDefCSPtr& in_drawableDef)
        {
            m_normalOnDrawableDef = in_drawableDef;
            
            if (m_drawableComponent != nullptr && m_state == ToggleHighlightState::k_normalOn)
            {
                m_drawableComponent->SetDrawableDef(m_normalOnDrawableDef);
            }
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void ToggleHighlightComponent::SetHighlightOnDrawableDef(const DrawableDefCSPtr& in_drawableDef)
        {
            m_highlightOnDrawableDef = in_drawableDef;
            
            if (m_drawableComponent != nullptr && m_state == ToggleHighlightState::k_highlightedOn)
            {
                m_drawableComponent->SetDrawableDef(m_highlightOnDrawableDef);
            }
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void ToggleHighlightComponent::SetToggleOn(bool in_toggleOn)
        {
            if (in_toggleOn == true && m_state == ToggleHighlightState::k_normalOff)
            {
                ApplyOnNormal();
            }
            else if (in_toggleOn == true && m_state == ToggleHighlightState::k_highlightedOff)
            {
                ApplyOnHightlighted();
            }
            else if (in_toggleOn == false && m_state == ToggleHighlightState::k_normalOn)
            {
                ApplyOffNormal();
            }
            else if (in_toggleOn == false && m_state == ToggleHighlightState::k_highlightedOn)
            {
                ApplyOffHightlighted();
            }
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        bool ToggleHighlightComponent::IsHighlighted() const
        {
            return (m_state == ToggleHighlightState::k_highlightedOff || m_state == ToggleHighlightState::k_highlightedOn);
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void ToggleHighlightComponent::Toggle()
        {
            if (IsToggledOn() == true)
            {
                SetToggleOn(false);
            }
            else
            {
                SetToggleOn(true);
            }
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void ToggleHighlightComponent::Highlight()
        {
            CS_ASSERT(IsHighlighted() == false, "Cannot highlight when already highlighted.");
            
            if (m_state == ToggleHighlightState::k_normalOff)
            {
                ApplyOffHightlighted();
            }
            else if (m_state == ToggleHighlightState::k_normalOn)
            {
                ApplyOnHightlighted();
            }
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void ToggleHighlightComponent::Unhighlight()
        {
            CS_ASSERT(IsHighlighted() == true, "Cannot highlight when already highlighted.");
            
            if (m_state == ToggleHighlightState::k_highlightedOff)
            {
                ApplyOffNormal();
            }
            else if (m_state == ToggleHighlightState::k_highlightedOn)
            {
                ApplyOnNormal();
            }
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void ToggleHighlightComponent::ApplyOffNormal()
        {
            CS_ASSERT(m_state != ToggleHighlightState::k_normalOff, "Trying to apply state that is already active.");
            
            m_state = ToggleHighlightState::k_normalOff;
            m_drawableComponent->SetDrawableDef(m_normalOffDrawableDef);
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void ToggleHighlightComponent::ApplyOffHightlighted()
        {
            CS_ASSERT(m_state != ToggleHighlightState::k_highlightedOff, "Trying to apply state that is already active.");
            
            m_state = ToggleHighlightState::k_highlightedOff;
            m_drawableComponent->SetDrawableDef(m_highlightOffDrawableDef);
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void ToggleHighlightComponent::ApplyOnNormal()
        {
            CS_ASSERT(m_state != ToggleHighlightState::k_normalOn, "Trying to apply state that is already active.");
            
            m_state = ToggleHighlightState::k_normalOn;
            m_drawableComponent->SetDrawableDef(m_normalOnDrawableDef);
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void ToggleHighlightComponent::ApplyOnHightlighted()
        {
            CS_ASSERT(m_state != ToggleHighlightState::k_highlightedOn, "Trying to apply state that is already active.");
            
            m_state = ToggleHighlightState::k_highlightedOn;
            m_drawableComponent->SetDrawableDef(m_highlightOnDrawableDef);
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void ToggleHighlightComponent::OnInit()
        {
            m_drawableComponent = GetWidget()->GetComponent<DrawableComponent>();
            CS_ASSERT(m_drawableComponent != nullptr, "Widgets with a Toggle Highlight Component must also contain a Drawable Component.");
            
            if (IsToggledOn() == false)
            {
                m_drawableComponent->SetDrawableDef(m_normalOffDrawableDef);
            }
            else
            {
                m_drawableComponent->SetDrawableDef(m_normalOnDrawableDef);
            }
            
            m_pressedInsideConnection = GetWidget()->GetPressedInsideEvent().OpenConnection(Core::MakeDelegate(this, &ToggleHighlightComponent::OnPressedInside));
            m_moveEnteredConnection = GetWidget()->GetMoveEnteredEvent().OpenConnection(Core::MakeDelegate(this, &ToggleHighlightComponent::OnMoveEntered));
            m_moveExitedConnection = GetWidget()->GetMoveExitedEvent().OpenConnection(Core::MakeDelegate(this, &ToggleHighlightComponent::OnMoveExited));
            m_releasedInsideConnection = GetWidget()->GetReleasedInsideEvent().OpenConnection(Core::MakeDelegate(this, &ToggleHighlightComponent::OnReleasedInside));
            m_releasedOutsideConnection = GetWidget()->GetReleasedOutsideEvent().OpenConnection(Core::MakeDelegate(this, &ToggleHighlightComponent::OnReleasedOutside));
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void ToggleHighlightComponent::OnPressedInside(Widget* in_widget, const Input::Pointer& in_pointer, Input::Pointer::InputType in_inputType)
        {
            if (in_inputType == Input::Pointer::GetDefaultInputType())
            {
                CS_ASSERT(Core::VectorUtils::Contains(m_activePointerIds, in_pointer.GetId()) == false, "Received pressed event for already pressed Id.");
                CS_ASSERT(Core::VectorUtils::Contains(m_highlightingPointerIds, in_pointer.GetId()) == false, "Received pressed event for already highlighting Id.");
                
                m_activePointerIds.push_back(in_pointer.GetId());
                m_highlightingPointerIds.push_back(in_pointer.GetId());
                
                if (IsHighlighted() == false)
                {
                    Highlight();
                }
            }
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void ToggleHighlightComponent::OnMoveEntered(Widget* in_widget, const Input::Pointer& in_pointer)
        {
            if (Core::VectorUtils::Contains(m_activePointerIds, in_pointer.GetId()) == true)
            {
                CS_ASSERT(Core::VectorUtils::Contains(m_highlightingPointerIds, in_pointer.GetId()) == false, "Received move entered for already highlighting Id.");
                
                m_highlightingPointerIds.push_back(in_pointer.GetId());
                
                if (IsHighlighted() == false)
                {
                    Highlight();
                }
            }
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void ToggleHighlightComponent::OnMoveExited(Widget* in_widget, const Input::Pointer& in_pointer)
        {
            if (Core::VectorUtils::Contains(m_activePointerIds, in_pointer.GetId()) == true)
            {
                CS_ASSERT(Core::VectorUtils::Contains(m_highlightingPointerIds, in_pointer.GetId()) == true, "Received move exited event for an id that is not in the highlighting list.");
                
                Core::VectorUtils::Remove(m_highlightingPointerIds, in_pointer.GetId());
                
                if (IsHighlighted() == true && m_highlightingPointerIds.empty() == true)
                {
                    Unhighlight();
                }
            }
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void ToggleHighlightComponent::OnReleasedInside(Widget* in_widget, const Input::Pointer& in_pointer, Input::Pointer::InputType in_inputType)
        {
            if (Core::VectorUtils::Contains(m_activePointerIds, in_pointer.GetId()) == true)
            {
                CS_ASSERT(Core::VectorUtils::Contains(m_highlightingPointerIds, in_pointer.GetId()) == true, "Received released inside event for an id that is not in the highlighting list.");
                
                Core::VectorUtils::Remove(m_activePointerIds, in_pointer.GetId());
                Core::VectorUtils::Remove(m_highlightingPointerIds, in_pointer.GetId());
                
                if (IsHighlighted() == true && m_highlightingPointerIds.empty() == true)
                {
                    Unhighlight();
                    Toggle();
                }
            }
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void ToggleHighlightComponent::OnReleasedOutside(Widget* in_widget, const Input::Pointer& in_pointer, Input::Pointer::InputType in_inputType)
        {
            if (Core::VectorUtils::Contains(m_activePointerIds, in_pointer.GetId()) == true)
            {
                CS_ASSERT(Core::VectorUtils::Contains(m_highlightingPointerIds, in_pointer.GetId()) == false, "Received released inside event for an id that is in the highlighting list.");
                
                Core::VectorUtils::Remove(m_activePointerIds, in_pointer.GetId());
                
                if (IsHighlighted() == true && m_highlightingPointerIds.empty() == true)
                {
                    Unhighlight();
                }
            }
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void ToggleHighlightComponent::OnDestroy()
        {
            m_pressedInsideConnection.reset();
            m_moveEnteredConnection.reset();
            m_moveExitedConnection.reset();
            m_releasedInsideConnection.reset();
            m_releasedOutsideConnection.reset();
            
            if (IsToggledOn() == false)
            {
                m_drawableComponent->SetDrawableDef(m_normalOffDrawableDef);
            }
            else
            {
                m_drawableComponent->SetDrawableDef(m_normalOnDrawableDef);
            }
            
            m_activePointerIds.clear();
            m_highlightingPointerIds.clear();
        }
    }
}