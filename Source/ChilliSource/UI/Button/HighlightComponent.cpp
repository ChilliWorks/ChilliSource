//
//  HighlightComponent.cpp
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

#include <ChilliSource/UI/Button/HighlightComponent.h>

#include <ChilliSource/Core/Container/VectorUtils.h>
#include <ChilliSource/Core/Container/Property/PropertyMap.h>
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
            const char k_normalDrawableKey[] = "NormalDrawable";
            const char k_highlightDrawableKey[] = "HighlightDrawable";
            const char k_highlightColourKey[] = "HighlightColour";
            
            const std::vector<Core::PropertyMap::PropertyDesc> k_propertyDescs =
            {
                {PropertyTypes::DrawableDef(), k_normalDrawableKey},
                {PropertyTypes::DrawableDef(), k_highlightDrawableKey},
                {Core::PropertyTypes::Colour(), k_highlightColourKey}
            };
        }
        
        CS_DEFINE_NAMEDTYPE(HighlightComponent);
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        const std::vector<Core::PropertyMap::PropertyDesc>& HighlightComponent::GetPropertyDescs()
        {
            return k_propertyDescs;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        HighlightComponent::HighlightComponent(const std::string& in_componentName, const Core::PropertyMap& in_properties)
            : Component(in_componentName)
        {
            RegisterProperty<DrawableDefCSPtr>(PropertyTypes::DrawableDef(), k_normalDrawableKey, Core::MakeDelegate(this, &HighlightComponent::GetNormalDrawableDef), Core::MakeDelegate(this, &HighlightComponent::SetNormalDrawableDef));
            RegisterProperty<DrawableDefCSPtr>(PropertyTypes::DrawableDef(), k_highlightDrawableKey, Core::MakeDelegate(this, &HighlightComponent::GetHighlightDrawableDef), Core::MakeDelegate(this, &HighlightComponent::SetHighlightDrawableDef));
            RegisterProperty<Core::Colour>(Core::PropertyTypes::Colour(), k_highlightColourKey, Core::MakeDelegate(this, &HighlightComponent::GetHighlightColour), Core::MakeDelegate(this, &HighlightComponent::SetHighlightColour));
            ApplyRegisteredProperties(in_properties);
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        bool HighlightComponent::IsA(Core::InterfaceIDType in_interfaceId) const
        {
            return (Component::InterfaceID == in_interfaceId || HighlightComponent::InterfaceID == in_interfaceId);
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        const DrawableDefCSPtr& HighlightComponent::GetNormalDrawableDef() const
        {
            return m_normalDrawableDef;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        const DrawableDefCSPtr& HighlightComponent::GetHighlightDrawableDef() const
        {
            return m_highlightDrawableDef;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        const Core::Colour& HighlightComponent::GetHighlightColour() const
        {
            return m_highlightColour;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void HighlightComponent::SetNormalDrawableDef(const DrawableDefCSPtr& in_drawableDef)
        {
            m_normalDrawableDef = in_drawableDef;
            
            if (m_highlighted == false)
            {
                Unhighlight();
            }
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void HighlightComponent::SetHighlightDrawableDef(const DrawableDefCSPtr& in_drawable)
        {
            m_highlightDrawableDef = in_drawable;
            
            if (m_highlighted == true)
            {
                Highlight();
            }
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void HighlightComponent::SetHighlightColour(const Core::Colour& in_colour)
        {
            m_highlightColour = in_colour;
            
            if (m_highlighted == true)
            {
                Highlight();
            }
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void HighlightComponent::Highlight()
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
        void HighlightComponent::Unhighlight()
        {
            m_highlighted = false;
            
            if (m_drawableComponent != nullptr)
            {
                m_drawableComponent->ApplyDrawableDef(m_normalDrawableDef);
            }
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void HighlightComponent::OnInit()
        {
            m_drawableComponent = GetWidget()->GetComponent<DrawableComponent>();
            CS_ASSERT(m_drawableComponent != nullptr, "Widgets with a Highlight Component must also contain a Drawable Component.");
            
            if (m_highlighted == false)
            {
                Unhighlight();
            }
            else
            {
                Highlight();
            }
            
            m_pressedInsideConnection = GetWidget()->GetPressedInsideEvent().OpenConnection(Core::MakeDelegate(this, &HighlightComponent::OnPressedInside));
            m_moveEnteredConnection = GetWidget()->GetMoveEnteredEvent().OpenConnection(Core::MakeDelegate(this, &HighlightComponent::OnMoveEntered));
            m_moveExitedConnection = GetWidget()->GetMoveExitedEvent().OpenConnection(Core::MakeDelegate(this, &HighlightComponent::OnMoveExited));
            m_releasedInsideConnection = GetWidget()->GetReleasedInsideEvent().OpenConnection(Core::MakeDelegate(this, &HighlightComponent::OnReleasedInside));
            m_releasedOutsideConnection = GetWidget()->GetReleasedOutsideEvent().OpenConnection(Core::MakeDelegate(this, &HighlightComponent::OnReleasedOutside));
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void HighlightComponent::OnPressedInside(Widget* in_widget, const Input::Pointer& in_pointer, Input::Pointer::InputType in_inputType)
        {
            if (in_inputType == Input::Pointer::GetDefaultInputType())
            {
                CS_ASSERT(Core::VectorUtils::Contains(m_activePointerIds, in_pointer.GetId()) == false, "Received pressed event for already pressed Id.");
                CS_ASSERT(Core::VectorUtils::Contains(m_highlightingPointerIds, in_pointer.GetId()) == false, "Received pressed event for already highlighting Id.");
                
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
        void HighlightComponent::OnMoveEntered(Widget* in_widget, const Input::Pointer& in_pointer)
        {
            if (Core::VectorUtils::Contains(m_activePointerIds, in_pointer.GetId()) == true)
            {
                CS_ASSERT(Core::VectorUtils::Contains(m_highlightingPointerIds, in_pointer.GetId()) == false, "Received move entered for already highlighting Id.");
                
                m_highlightingPointerIds.push_back(in_pointer.GetId());
                
                if (m_highlighted == false)
                {
                    Highlight();
                }
            }
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void HighlightComponent::OnMoveExited(Widget* in_widget, const Input::Pointer& in_pointer)
        {
            if (Core::VectorUtils::Contains(m_activePointerIds, in_pointer.GetId()) == true)
            {
                CS_ASSERT(Core::VectorUtils::Contains(m_highlightingPointerIds, in_pointer.GetId()) == true, "Received move exited event for an id that is not in the highlighting list.");
                
                Core::VectorUtils::Remove(m_highlightingPointerIds, in_pointer.GetId());
                
                if (m_highlighted == true && m_highlightingPointerIds.empty() == true)
                {
                    Unhighlight();
                }
            }
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void HighlightComponent::OnReleasedInside(Widget* in_widget, const Input::Pointer& in_pointer, Input::Pointer::InputType in_inputType)
        {
            if (Core::VectorUtils::Contains(m_activePointerIds, in_pointer.GetId()) == true)
            {
                CS_ASSERT(Core::VectorUtils::Contains(m_highlightingPointerIds, in_pointer.GetId()) == true, "Received released inside event for an id that is not in the highlighting list.");
                
                Core::VectorUtils::Remove(m_activePointerIds, in_pointer.GetId());
                Core::VectorUtils::Remove(m_highlightingPointerIds, in_pointer.GetId());
                
                if (m_highlighted == true && m_highlightingPointerIds.empty() == true)
                {
                    Unhighlight();
                }
            }
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void HighlightComponent::OnReleasedOutside(Widget* in_widget, const Input::Pointer& in_pointer, Input::Pointer::InputType in_inputType)
        {
            if (Core::VectorUtils::Contains(m_activePointerIds, in_pointer.GetId()) == true)
            {
                CS_ASSERT(Core::VectorUtils::Contains(m_highlightingPointerIds, in_pointer.GetId()) == false, "Received released inside event for an id that is in the highlighting list.");
                
                Core::VectorUtils::Remove(m_activePointerIds, in_pointer.GetId());
                
                if (m_highlighted == true && m_highlightingPointerIds.empty() == true)
                {
                    Unhighlight();
                }
            }
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void HighlightComponent::OnDestroy()
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
}