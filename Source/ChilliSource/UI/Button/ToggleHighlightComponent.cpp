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
            {PropertyTypes::DrawableDef(), k_normalOffDrawableKey},
            {PropertyTypes::DrawableDef(), k_highlightOffDrawableKey},
            {PropertyTypes::Colour(), k_highlightOffColourKey},
            {PropertyTypes::DrawableDef(), k_normalOnDrawableKey},
            {PropertyTypes::DrawableDef(), k_highlightOnDrawableKey},
            {PropertyTypes::Colour(), k_highlightOnColourKey},
            {PropertyTypes::Bool(), k_toggledOnKey}
        };
    }
    
    CS_DEFINE_NAMEDTYPE(ToggleHighlightComponent);
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    const std::vector<PropertyMap::PropertyDesc>& ToggleHighlightComponent::GetPropertyDescs()
    {
        return k_propertyDescs;
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    ToggleHighlightComponent::ToggleHighlightComponent(const std::string& in_componentName, const PropertyMap& in_properties)
        : UIComponent(in_componentName)
    {
        RegisterProperty<DrawableDefCSPtr>(PropertyTypes::DrawableDef(), k_normalOffDrawableKey, MakeDelegate(this, &ToggleHighlightComponent::GetNormalOffDrawableDef), MakeDelegate(this, &ToggleHighlightComponent::SetNormalOffDrawableDef));
        RegisterProperty<DrawableDefCSPtr>(PropertyTypes::DrawableDef(), k_highlightOffDrawableKey, MakeDelegate(this, &ToggleHighlightComponent::GetHighlightOffDrawableDef), MakeDelegate(this, &ToggleHighlightComponent::SetHighlightOffDrawableDef));
        RegisterProperty<Colour>(PropertyTypes::Colour(), k_highlightOffColourKey, MakeDelegate(this, &ToggleHighlightComponent::GetHighlightOffColour), MakeDelegate(this, &ToggleHighlightComponent::SetHighlightOffColour));
        RegisterProperty<DrawableDefCSPtr>(PropertyTypes::DrawableDef(), k_normalOnDrawableKey, MakeDelegate(this, &ToggleHighlightComponent::GetNormalOnDrawableDef), MakeDelegate(this, &ToggleHighlightComponent::SetNormalOnDrawableDef));
        RegisterProperty<DrawableDefCSPtr>(PropertyTypes::DrawableDef(), k_highlightOnDrawableKey, MakeDelegate(this, &ToggleHighlightComponent::GetHighlightOnDrawableDef), MakeDelegate(this, &ToggleHighlightComponent::SetHighlightOnDrawableDef));
        RegisterProperty<Colour>(PropertyTypes::Colour(), k_highlightOnColourKey, MakeDelegate(this, &ToggleHighlightComponent::GetHighlightOnColour), MakeDelegate(this, &ToggleHighlightComponent::SetHighlightOnColour));
        RegisterProperty<bool>(PropertyTypes::Bool(), k_toggledOnKey, MakeDelegate(this, &ToggleHighlightComponent::IsToggledOn), MakeDelegate(this, &ToggleHighlightComponent::SetToggleOn));
        ApplyRegisteredProperties(in_properties);
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    bool ToggleHighlightComponent::IsA(InterfaceIDType in_interfaceId) const
    {
        return (UIComponent::InterfaceID == in_interfaceId || ToggleHighlightComponent::InterfaceID == in_interfaceId);
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
    const Colour& ToggleHighlightComponent::GetHighlightOffColour() const
    {
        return m_highlightOffColour;
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
    const Colour& ToggleHighlightComponent::GetHighlightOnColour() const
    {
        return m_highlightOnColour;
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
        
        if (m_state == ToggleHighlightState::k_normalOff)
        {
            ApplyOffNormal();
        }
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    void ToggleHighlightComponent::SetHighlightOffDrawableDef(const DrawableDefCSPtr& in_drawableDef)
    {
        m_highlightOffDrawableDef = in_drawableDef;
        
        if (m_state == ToggleHighlightState::k_highlightedOff)
        {
            ApplyOffHighlighted();
        }
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    void ToggleHighlightComponent::SetHighlightOffColour(const Colour& in_colour)
    {
        m_highlightOffColour = in_colour;
        
        if (m_state == ToggleHighlightState::k_highlightedOff)
        {
            ApplyOffHighlighted();
        }
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    void ToggleHighlightComponent::SetNormalOnDrawableDef(const DrawableDefCSPtr& in_drawableDef)
    {
        m_normalOnDrawableDef = in_drawableDef;
        
        if (m_state == ToggleHighlightState::k_normalOn)
        {
            ApplyOnNormal();
        }
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    void ToggleHighlightComponent::SetHighlightOnDrawableDef(const DrawableDefCSPtr& in_drawableDef)
    {
        m_highlightOnDrawableDef = in_drawableDef;
        
        if (m_state == ToggleHighlightState::k_highlightedOn)
        {
            ApplyOnHightlighted();
        }
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    void ToggleHighlightComponent::SetHighlightOnColour(const Colour& in_colour)
    {
        m_highlightOnColour = in_colour;
        
        if (m_state == ToggleHighlightState::k_highlightedOn)
        {
            ApplyOnHightlighted();
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
            ApplyOffHighlighted();
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
    void ToggleHighlightComponent::Unhighlight()
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
    void ToggleHighlightComponent::ApplyOffNormal()
    {
        m_state = ToggleHighlightState::k_normalOff;
        
        if (m_drawableComponent != nullptr)
        {
            m_drawableComponent->ApplyDrawableDef(m_normalOffDrawableDef);
        }
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    void ToggleHighlightComponent::ApplyOffHighlighted()
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
    void ToggleHighlightComponent::ApplyOnNormal()
    {
        m_state = ToggleHighlightState::k_normalOn;
        
        if (m_drawableComponent != nullptr)
        {
            m_drawableComponent->ApplyDrawableDef(m_normalOnDrawableDef);
        }
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    void ToggleHighlightComponent::ApplyOnHightlighted()
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
    void ToggleHighlightComponent::OnInit()
    {
        m_drawableComponent = GetWidget()->GetComponent<DrawableComponent>();
        CS_ASSERT(m_drawableComponent != nullptr, "Widgets with a Toggle Highlight UIComponent must also contain a Drawable UIComponent.");
        
        if (IsToggledOn() == false)
        {
            Unhighlight();
        }
        else
        {
            Highlight();
        }
        
        m_pressedInsideConnection = GetWidget()->GetPressedInsideEvent().OpenConnection(MakeDelegate(this, &ToggleHighlightComponent::OnPressedInside));
        m_moveEnteredConnection = GetWidget()->GetMoveEnteredEvent().OpenConnection(MakeDelegate(this, &ToggleHighlightComponent::OnMoveEntered));
        m_moveExitedConnection = GetWidget()->GetMoveExitedEvent().OpenConnection(MakeDelegate(this, &ToggleHighlightComponent::OnMoveExited));
        m_releasedInsideConnection = GetWidget()->GetReleasedInsideEvent().OpenConnection(MakeDelegate(this, &ToggleHighlightComponent::OnReleasedInside));
        m_releasedOutsideConnection = GetWidget()->GetReleasedOutsideEvent().OpenConnection(MakeDelegate(this, &ToggleHighlightComponent::OnReleasedOutside));
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    void ToggleHighlightComponent::OnPressedInside(Widget* in_widget, const Pointer& in_pointer, Pointer::InputType in_inputType)
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
    void ToggleHighlightComponent::OnMoveEntered(Widget* in_widget, const Pointer& in_pointer)
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
    void ToggleHighlightComponent::OnMoveExited(Widget* in_widget, const Pointer& in_pointer)
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
    void ToggleHighlightComponent::OnReleasedInside(Widget* in_widget, const Pointer& in_pointer, Pointer::InputType in_inputType)
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
    void ToggleHighlightComponent::OnReleasedOutside(Widget* in_widget, const Pointer& in_pointer, Pointer::InputType in_inputType)
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
    void ToggleHighlightComponent::OnDestroy()
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