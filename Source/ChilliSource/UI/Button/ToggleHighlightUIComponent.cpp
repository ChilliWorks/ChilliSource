//
//  ToggleHighlightUIComponent.cpp
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

#include <ChilliSource/UI/Button/ToggleHighlightUIComponent.h>

#include <ChilliSource/Core/Container/VectorUtils.h>
#include <ChilliSource/Core/Container/Property/PropertyTypes.h>
#include <ChilliSource/Core/Delegate/MakeDelegate.h>
#include <ChilliSource/UI/Base/Widget.h>
#include <ChilliSource/UI/Base/PropertyTypes.h>
#include <ChilliSource/UI/Drawable/DrawableUIComponent.h>
#include <ChilliSource/UI/Drawable/UIDrawableDef.h>

namespace ChilliSource
{
    namespace
    {
        const char k_normalOffDrawableKey[] = "NormalOffDrawable";
        const char k_highlightOffDrawableKey[] = "HighlightOffDrawable";
        const char k_highlightOffColourKey[] = "HighlightOffColour";
        const char k_normalOnDrawableKey[] = "NormalOnDrawable";
        const char k_highlightOnDrawableKey[] = "HighlightOnDrawable";
        const char k_highlightOnColourKey[] = "HighlightOnColour";
        const char k_toggledOnKey[] = "ToggledOn";
        
        const std::vector<PropertyMap::PropertyDesc> k_propertyDescs =
        {
            {PropertyTypes::UIDrawableDef(), k_normalOffDrawableKey},
            {PropertyTypes::UIDrawableDef(), k_highlightOffDrawableKey},
            {PropertyTypes::Colour(), k_highlightOffColourKey},
            {PropertyTypes::UIDrawableDef(), k_normalOnDrawableKey},
            {PropertyTypes::UIDrawableDef(), k_highlightOnDrawableKey},
            {PropertyTypes::Colour(), k_highlightOnColourKey},
            {PropertyTypes::Bool(), k_toggledOnKey}
        };
    }
    
    CS_DEFINE_NAMEDTYPE(ToggleHighlightUIComponent);
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    const std::vector<PropertyMap::PropertyDesc>& ToggleHighlightUIComponent::GetPropertyDescs()
    {
        return k_propertyDescs;
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    ToggleHighlightUIComponent::ToggleHighlightUIComponent(const std::string& in_componentName, const PropertyMap& in_properties)
        : UIComponent(in_componentName)
    {
        RegisterProperty<UIDrawableDefCSPtr>(PropertyTypes::UIDrawableDef(), k_normalOffDrawableKey, MakeDelegate(this, &ToggleHighlightUIComponent::GetNormalOffDrawableDef), MakeDelegate(this, &ToggleHighlightUIComponent::SetNormalOffDrawableDef));
        RegisterProperty<UIDrawableDefCSPtr>(PropertyTypes::UIDrawableDef(), k_highlightOffDrawableKey, MakeDelegate(this, &ToggleHighlightUIComponent::GetHighlightOffDrawableDef), MakeDelegate(this, &ToggleHighlightUIComponent::SetHighlightOffDrawableDef));
        RegisterProperty<Colour>(PropertyTypes::Colour(), k_highlightOffColourKey, MakeDelegate(this, &ToggleHighlightUIComponent::GetHighlightOffColour), MakeDelegate(this, &ToggleHighlightUIComponent::SetHighlightOffColour));
        RegisterProperty<UIDrawableDefCSPtr>(PropertyTypes::UIDrawableDef(), k_normalOnDrawableKey, MakeDelegate(this, &ToggleHighlightUIComponent::GetNormalOnDrawableDef), MakeDelegate(this, &ToggleHighlightUIComponent::SetNormalOnDrawableDef));
        RegisterProperty<UIDrawableDefCSPtr>(PropertyTypes::UIDrawableDef(), k_highlightOnDrawableKey, MakeDelegate(this, &ToggleHighlightUIComponent::GetHighlightOnDrawableDef), MakeDelegate(this, &ToggleHighlightUIComponent::SetHighlightOnDrawableDef));
        RegisterProperty<Colour>(PropertyTypes::Colour(), k_highlightOnColourKey, MakeDelegate(this, &ToggleHighlightUIComponent::GetHighlightOnColour), MakeDelegate(this, &ToggleHighlightUIComponent::SetHighlightOnColour));
        RegisterProperty<bool>(PropertyTypes::Bool(), k_toggledOnKey, MakeDelegate(this, &ToggleHighlightUIComponent::IsToggledOn), MakeDelegate(this, &ToggleHighlightUIComponent::SetToggleOn));
        ApplyRegisteredProperties(in_properties);
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    bool ToggleHighlightUIComponent::IsA(InterfaceIDType in_interfaceId) const
    {
        return (UIComponent::InterfaceID == in_interfaceId || ToggleHighlightUIComponent::InterfaceID == in_interfaceId);
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    const UIDrawableDefCSPtr& ToggleHighlightUIComponent::GetNormalOffDrawableDef() const
    {
        return m_normalOffDrawableDef;
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    const UIDrawableDefCSPtr& ToggleHighlightUIComponent::GetHighlightOffDrawableDef() const
    {
        return m_highlightOffDrawableDef;
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    const Colour& ToggleHighlightUIComponent::GetHighlightOffColour() const
    {
        return m_highlightOffColour;
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    const UIDrawableDefCSPtr& ToggleHighlightUIComponent::GetNormalOnDrawableDef() const
    {
        return m_normalOnDrawableDef;
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    const UIDrawableDefCSPtr& ToggleHighlightUIComponent::GetHighlightOnDrawableDef() const
    {
        return m_normalOffDrawableDef;
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    const Colour& ToggleHighlightUIComponent::GetHighlightOnColour() const
    {
        return m_highlightOnColour;
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    bool ToggleHighlightUIComponent::IsToggledOn() const
    {
        return (m_state == ToggleHighlightState::k_normalOn || m_state == ToggleHighlightState::k_highlightedOn);
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    void ToggleHighlightUIComponent::SetNormalOffDrawableDef(const UIDrawableDefCSPtr& in_drawableDef)
    {
        m_normalOffDrawableDef = in_drawableDef;
        
        if (m_state == ToggleHighlightState::k_normalOff)
        {
            ApplyOffNormal();
        }
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    void ToggleHighlightUIComponent::SetHighlightOffDrawableDef(const UIDrawableDefCSPtr& in_drawableDef)
    {
        m_highlightOffDrawableDef = in_drawableDef;
        
        if (m_state == ToggleHighlightState::k_highlightedOff)
        {
            ApplyOffHighlighted();
        }
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    void ToggleHighlightUIComponent::SetHighlightOffColour(const Colour& in_colour)
    {
        m_highlightOffColour = in_colour;
        
        if (m_state == ToggleHighlightState::k_highlightedOff)
        {
            ApplyOffHighlighted();
        }
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    void ToggleHighlightUIComponent::SetNormalOnDrawableDef(const UIDrawableDefCSPtr& in_drawableDef)
    {
        m_normalOnDrawableDef = in_drawableDef;
        
        if (m_state == ToggleHighlightState::k_normalOn)
        {
            ApplyOnNormal();
        }
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    void ToggleHighlightUIComponent::SetHighlightOnDrawableDef(const UIDrawableDefCSPtr& in_drawableDef)
    {
        m_highlightOnDrawableDef = in_drawableDef;
        
        if (m_state == ToggleHighlightState::k_highlightedOn)
        {
            ApplyOnHightlighted();
        }
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    void ToggleHighlightUIComponent::SetHighlightOnColour(const Colour& in_colour)
    {
        m_highlightOnColour = in_colour;
        
        if (m_state == ToggleHighlightState::k_highlightedOn)
        {
            ApplyOnHightlighted();
        }
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    void ToggleHighlightUIComponent::SetToggleOn(bool in_toggleOn)
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
            ApplyOffHighlighted();
        }
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    bool ToggleHighlightUIComponent::IsHighlighted() const
    {
        return (m_state == ToggleHighlightState::k_highlightedOff || m_state == ToggleHighlightState::k_highlightedOn);
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    void ToggleHighlightUIComponent::Toggle()
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
    void ToggleHighlightUIComponent::Highlight()
    {
        switch (m_state)
        {
            case ToggleHighlightState::k_normalOff:
            case ToggleHighlightState::k_highlightedOff:
                ApplyOffHighlighted();
                break;
            case ToggleHighlightState::k_normalOn:
            case ToggleHighlightState::k_highlightedOn:
                ApplyOnHightlighted();
                break;
            default:
                CS_LOG_FATAL("Invalid state.");
                break;
        }
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    void ToggleHighlightUIComponent::Unhighlight()
    {
        switch (m_state)
        {
            case ToggleHighlightState::k_normalOff:
            case ToggleHighlightState::k_highlightedOff:
                ApplyOffNormal();
                break;
            case ToggleHighlightState::k_normalOn:
            case ToggleHighlightState::k_highlightedOn:
                ApplyOnNormal();
                break;
            default:
                CS_LOG_FATAL("Invalid state.");
                break;
        }
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    void ToggleHighlightUIComponent::ApplyOffNormal()
    {
        m_state = ToggleHighlightState::k_normalOff;
        
        if (m_drawableComponent != nullptr)
        {
            m_drawableComponent->ApplyDrawableDef(m_normalOffDrawableDef);
        }
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    void ToggleHighlightUIComponent::ApplyOffHighlighted()
    {
        m_state = ToggleHighlightState::k_highlightedOff;
        
        if (m_drawableComponent != nullptr)
        {
            if (m_highlightOffDrawableDef != nullptr)
            {
                m_drawableComponent->ApplyDrawableDef(m_highlightOffDrawableDef);
                
                auto drawable = m_drawableComponent->GetDrawable();
                drawable->SetColour(m_highlightOffColour * m_highlightOffDrawableDef->GetColour());
            }
            else
            {
                m_drawableComponent->ApplyDrawableDef(m_normalOffDrawableDef);
                
                auto drawable = m_drawableComponent->GetDrawable();
                drawable->SetColour(m_highlightOffColour * m_normalOffDrawableDef->GetColour());
            }
        }
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    void ToggleHighlightUIComponent::ApplyOnNormal()
    {
        m_state = ToggleHighlightState::k_normalOn;
        
        if (m_drawableComponent != nullptr)
        {
            m_drawableComponent->ApplyDrawableDef(m_normalOnDrawableDef);
        }
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    void ToggleHighlightUIComponent::ApplyOnHightlighted()
    {
        m_state = ToggleHighlightState::k_highlightedOn;
        
        if (m_drawableComponent != nullptr)
        {
            if (m_highlightOnDrawableDef != nullptr)
            {
                m_drawableComponent->ApplyDrawableDef(m_highlightOnDrawableDef);
                
                auto drawable = m_drawableComponent->GetDrawable();
                drawable->SetColour(m_highlightOnColour * m_highlightOnDrawableDef->GetColour());
            }
            else
            {
                m_drawableComponent->ApplyDrawableDef(m_normalOnDrawableDef);
                
                auto drawable = m_drawableComponent->GetDrawable();
                drawable->SetColour(m_highlightOnColour * m_normalOnDrawableDef->GetColour());
            }
        }
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    void ToggleHighlightUIComponent::OnInit()
    {
        m_drawableComponent = GetWidget()->GetComponent<DrawableUIComponent>();
        CS_ASSERT(m_drawableComponent != nullptr, "Widgets with a Toggle Highlight UIComponent must also contain a UIDrawable UIComponent.");
        
        if (IsToggledOn() == false)
        {
            Unhighlight();
        }
        else
        {
            Highlight();
        }
        
        m_pressedInsideConnection = GetWidget()->GetPressedInsideEvent().OpenConnection(MakeDelegate(this, &ToggleHighlightUIComponent::OnPressedInside));
        m_moveEnteredConnection = GetWidget()->GetMoveEnteredEvent().OpenConnection(MakeDelegate(this, &ToggleHighlightUIComponent::OnMoveEntered));
        m_moveExitedConnection = GetWidget()->GetMoveExitedEvent().OpenConnection(MakeDelegate(this, &ToggleHighlightUIComponent::OnMoveExited));
        m_releasedInsideConnection = GetWidget()->GetReleasedInsideEvent().OpenConnection(MakeDelegate(this, &ToggleHighlightUIComponent::OnReleasedInside));
        m_releasedOutsideConnection = GetWidget()->GetReleasedOutsideEvent().OpenConnection(MakeDelegate(this, &ToggleHighlightUIComponent::OnReleasedOutside));
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    void ToggleHighlightUIComponent::OnPressedInside(Widget* in_widget, const Pointer& in_pointer, Pointer::InputType in_inputType)
    {
        if (in_inputType == Pointer::GetDefaultInputType())
        {
            CS_ASSERT(VectorUtils::Contains(m_activePointerIds, in_pointer.GetId()) == false, "Received pressed event for already pressed Id.");
            CS_ASSERT(VectorUtils::Contains(m_highlightingPointerIds, in_pointer.GetId()) == false, "Received pressed event for already highlighting Id.");
            
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
    void ToggleHighlightUIComponent::OnMoveEntered(Widget* in_widget, const Pointer& in_pointer)
    {
        if (VectorUtils::Contains(m_activePointerIds, in_pointer.GetId()) == true)
        {
            CS_ASSERT(VectorUtils::Contains(m_highlightingPointerIds, in_pointer.GetId()) == false, "Received move entered for already highlighting Id.");
            
            m_highlightingPointerIds.push_back(in_pointer.GetId());
            
            if (IsHighlighted() == false)
            {
                Highlight();
            }
        }
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    void ToggleHighlightUIComponent::OnMoveExited(Widget* in_widget, const Pointer& in_pointer)
    {
        if (VectorUtils::Contains(m_activePointerIds, in_pointer.GetId()) == true)
        {
            CS_ASSERT(VectorUtils::Contains(m_highlightingPointerIds, in_pointer.GetId()) == true, "Received move exited event for an id that is not in the highlighting list.");
            
            VectorUtils::Remove(m_highlightingPointerIds, in_pointer.GetId());
            
            if (IsHighlighted() == true && m_highlightingPointerIds.empty() == true)
            {
                Unhighlight();
            }
        }
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    void ToggleHighlightUIComponent::OnReleasedInside(Widget* in_widget, const Pointer& in_pointer, Pointer::InputType in_inputType)
    {
        if (VectorUtils::Contains(m_activePointerIds, in_pointer.GetId()) == true)
        {
            CS_ASSERT(VectorUtils::Contains(m_highlightingPointerIds, in_pointer.GetId()) == true, "Received released inside event for an id that is not in the highlighting list.");
            
            VectorUtils::Remove(m_activePointerIds, in_pointer.GetId());
            VectorUtils::Remove(m_highlightingPointerIds, in_pointer.GetId());
            
            if (IsHighlighted() == true && m_highlightingPointerIds.empty() == true)
            {
                Unhighlight();
                Toggle();
            }
        }
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    void ToggleHighlightUIComponent::OnReleasedOutside(Widget* in_widget, const Pointer& in_pointer, Pointer::InputType in_inputType)
    {
        if (VectorUtils::Contains(m_activePointerIds, in_pointer.GetId()) == true)
        {
            CS_ASSERT(VectorUtils::Contains(m_highlightingPointerIds, in_pointer.GetId()) == false, "Received released inside event for an id that is in the highlighting list.");
            
            VectorUtils::Remove(m_activePointerIds, in_pointer.GetId());
            
            if (IsHighlighted() == true && m_highlightingPointerIds.empty() == true)
            {
                Unhighlight();
            }
        }
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    void ToggleHighlightUIComponent::OnDestroy()
    {
        m_pressedInsideConnection.reset();
        m_moveEnteredConnection.reset();
        m_moveExitedConnection.reset();
        m_releasedInsideConnection.reset();
        m_releasedOutsideConnection.reset();
        
        if (IsToggledOn() == false)
        {
            m_drawableComponent->ApplyDrawableDef(m_normalOffDrawableDef);
        }
        else
        {
            m_drawableComponent->ApplyDrawableDef(m_normalOnDrawableDef);
        }
        
        m_activePointerIds.clear();
        m_highlightingPointerIds.clear();
    }
}