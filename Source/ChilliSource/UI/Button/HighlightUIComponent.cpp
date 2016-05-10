//
//  HighlightUIComponent.cpp
//  Chilli Source
//  Created by Ian Copland on 28/11/2014.
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

#include <ChilliSource/UI/Button/HighlightUIComponent.h>

#include <ChilliSource/Core/Container/VectorUtils.h>
#include <ChilliSource/Core/Container/Property/PropertyMap.h>
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
        const char k_normalDrawableKey[] = "NormalDrawable";
        const char k_highlightDrawableKey[] = "HighlightDrawable";
        const char k_highlightColourKey[] = "HighlightColour";
        
        const std::vector<PropertyMap::PropertyDesc> k_propertyDescs =
        {
            {PropertyTypes::UIDrawableDef(), k_normalDrawableKey},
            {PropertyTypes::UIDrawableDef(), k_highlightDrawableKey},
            {PropertyTypes::Colour(), k_highlightColourKey}
        };
    }
    
    CS_DEFINE_NAMEDTYPE(HighlightUIComponent);
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    const std::vector<PropertyMap::PropertyDesc>& HighlightUIComponent::GetPropertyDescs()
    {
        return k_propertyDescs;
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    HighlightUIComponent::HighlightUIComponent(const std::string& in_componentName, const PropertyMap& in_properties)
        : UIComponent(in_componentName)
    {
        RegisterProperty<UIDrawableDefCSPtr>(PropertyTypes::UIDrawableDef(), k_normalDrawableKey, MakeDelegate(this, &HighlightUIComponent::GetNormalDrawableDef), MakeDelegate(this, &HighlightUIComponent::SetNormalDrawableDef));
        RegisterProperty<UIDrawableDefCSPtr>(PropertyTypes::UIDrawableDef(), k_highlightDrawableKey, MakeDelegate(this, &HighlightUIComponent::GetHighlightDrawableDef), MakeDelegate(this, &HighlightUIComponent::SetHighlightDrawableDef));
        RegisterProperty<Colour>(PropertyTypes::Colour(), k_highlightColourKey, MakeDelegate(this, &HighlightUIComponent::GetHighlightColour), MakeDelegate(this, &HighlightUIComponent::SetHighlightColour));
        ApplyRegisteredProperties(in_properties);
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    bool HighlightUIComponent::IsA(InterfaceIDType in_interfaceId) const
    {
        return (UIComponent::InterfaceID == in_interfaceId || HighlightUIComponent::InterfaceID == in_interfaceId);
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    const UIDrawableDefCSPtr& HighlightUIComponent::GetNormalDrawableDef() const
    {
        return m_normalDrawableDef;
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    const UIDrawableDefCSPtr& HighlightUIComponent::GetHighlightDrawableDef() const
    {
        return m_highlightDrawableDef;
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    const Colour& HighlightUIComponent::GetHighlightColour() const
    {
        return m_highlightColour;
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    void HighlightUIComponent::SetNormalDrawableDef(const UIDrawableDefCSPtr& in_drawableDef)
    {
        m_normalDrawableDef = in_drawableDef;
        
        if (m_highlighted == false)
        {
            Unhighlight();
        }
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    void HighlightUIComponent::SetHighlightDrawableDef(const UIDrawableDefCSPtr& in_drawable)
    {
        m_highlightDrawableDef = in_drawable;
        
        if (m_highlighted == true)
        {
            Highlight();
        }
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    void HighlightUIComponent::SetHighlightColour(const Colour& in_colour)
    {
        m_highlightColour = in_colour;
        
        if (m_highlighted == true)
        {
            Highlight();
        }
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    void HighlightUIComponent::Highlight()
    {
        m_highlighted = true;
        
        if (m_drawableComponent != nullptr)
        {
            if (m_highlightDrawableDef != nullptr)
            {
                m_drawableComponent->ApplyDrawableDef(m_highlightDrawableDef);
                
                auto drawable = m_drawableComponent->GetDrawable();
                drawable->SetColour(m_highlightColour * m_highlightDrawableDef->GetColour());
            }
            else
            {
                m_drawableComponent->ApplyDrawableDef(m_normalDrawableDef);
                
                auto drawable = m_drawableComponent->GetDrawable();
                drawable->SetColour(m_highlightColour * m_normalDrawableDef->GetColour());
            }
        }
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    void HighlightUIComponent::Unhighlight()
    {
        m_highlighted = false;
        
        if (m_drawableComponent != nullptr)
        {
            m_drawableComponent->ApplyDrawableDef(m_normalDrawableDef);
        }
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    void HighlightUIComponent::OnInit()
    {
        m_drawableComponent = GetWidget()->GetComponent<DrawableUIComponent>();
        CS_ASSERT(m_drawableComponent != nullptr, "Widgets with a Highlight UIComponent must also contain a UIDrawable UIComponent.");
        
        if (m_highlighted == false)
        {
            Unhighlight();
        }
        else
        {
            Highlight();
        }
        
        m_pressedInsideConnection = GetWidget()->GetPressedInsideEvent().OpenConnection(MakeDelegate(this, &HighlightUIComponent::OnPressedInside));
        m_moveEnteredConnection = GetWidget()->GetMoveEnteredEvent().OpenConnection(MakeDelegate(this, &HighlightUIComponent::OnMoveEntered));
        m_moveExitedConnection = GetWidget()->GetMoveExitedEvent().OpenConnection(MakeDelegate(this, &HighlightUIComponent::OnMoveExited));
        m_releasedInsideConnection = GetWidget()->GetReleasedInsideEvent().OpenConnection(MakeDelegate(this, &HighlightUIComponent::OnReleasedInside));
        m_releasedOutsideConnection = GetWidget()->GetReleasedOutsideEvent().OpenConnection(MakeDelegate(this, &HighlightUIComponent::OnReleasedOutside));
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    void HighlightUIComponent::OnPressedInside(Widget* in_widget, const Pointer& in_pointer, Pointer::InputType in_inputType)
    {
        if (in_inputType == Pointer::GetDefaultInputType())
        {
            CS_ASSERT(VectorUtils::Contains(m_activePointerIds, in_pointer.GetId()) == false, "Received pressed event for already pressed Id.");
            CS_ASSERT(VectorUtils::Contains(m_highlightingPointerIds, in_pointer.GetId()) == false, "Received pressed event for already highlighting Id.");
            
            m_activePointerIds.push_back(in_pointer.GetId());
            m_highlightingPointerIds.push_back(in_pointer.GetId());
            
            if (m_highlighted == false)
            {
                Highlight();
            }
        }
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    void HighlightUIComponent::OnMoveEntered(Widget* in_widget, const Pointer& in_pointer)
    {
        if (VectorUtils::Contains(m_activePointerIds, in_pointer.GetId()) == true)
        {
            CS_ASSERT(VectorUtils::Contains(m_highlightingPointerIds, in_pointer.GetId()) == false, "Received move entered for already highlighting Id.");
            
            m_highlightingPointerIds.push_back(in_pointer.GetId());
            
            if (m_highlighted == false)
            {
                Highlight();
            }
        }
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    void HighlightUIComponent::OnMoveExited(Widget* in_widget, const Pointer& in_pointer)
    {
        if (VectorUtils::Contains(m_activePointerIds, in_pointer.GetId()) == true)
        {
            CS_ASSERT(VectorUtils::Contains(m_highlightingPointerIds, in_pointer.GetId()) == true, "Received move exited event for an id that is not in the highlighting list.");
            
            VectorUtils::Remove(m_highlightingPointerIds, in_pointer.GetId());
            
            if (m_highlighted == true && m_highlightingPointerIds.empty() == true)
            {
                Unhighlight();
            }
        }
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    void HighlightUIComponent::OnReleasedInside(Widget* in_widget, const Pointer& in_pointer, Pointer::InputType in_inputType)
    {
        if (VectorUtils::Contains(m_activePointerIds, in_pointer.GetId()) == true)
        {
            CS_ASSERT(VectorUtils::Contains(m_highlightingPointerIds, in_pointer.GetId()) == true, "Received released inside event for an id that is not in the highlighting list.");
            
            VectorUtils::Remove(m_activePointerIds, in_pointer.GetId());
            VectorUtils::Remove(m_highlightingPointerIds, in_pointer.GetId());
            
            if (m_highlighted == true && m_highlightingPointerIds.empty() == true)
            {
                Unhighlight();
            }
        }
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    void HighlightUIComponent::OnReleasedOutside(Widget* in_widget, const Pointer& in_pointer, Pointer::InputType in_inputType)
    {
        if (VectorUtils::Contains(m_activePointerIds, in_pointer.GetId()) == true)
        {
            CS_ASSERT(VectorUtils::Contains(m_highlightingPointerIds, in_pointer.GetId()) == false, "Received released inside event for an id that is in the highlighting list.");
            
            VectorUtils::Remove(m_activePointerIds, in_pointer.GetId());
            
            if (m_highlighted == true && m_highlightingPointerIds.empty() == true)
            {
                Unhighlight();
            }
        }
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    void HighlightUIComponent::OnDestroy()
    {
        m_pressedInsideConnection.reset();
        m_moveEnteredConnection.reset();
        m_moveExitedConnection.reset();
        m_releasedInsideConnection.reset();
        m_releasedOutsideConnection.reset();
        
        if (m_highlighted == true)
        {
            Unhighlight();
        }
        
        m_activePointerIds.clear();
        m_highlightingPointerIds.clear();
    }
}