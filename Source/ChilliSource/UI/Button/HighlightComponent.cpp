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

#include <ChilliSource/Core/Delegate/MakeDelegate.h>
#include <ChilliSource/UI/Base/Widget.h>

namespace ChilliSource
{
    namespace UI
    {
        namespace
        {
            const char k_normalDrawableKey[] = "NormalDrawable";
            const char k_highlightDrawableKey[] = "HighlightDrawable";
            
            const std::vector<PropertyMap::PropertyDesc> k_propertyDescs =
            {
                {PropertyType::k_drawableDesc, k_normalDrawableKey},
                {PropertyType::k_drawableDesc, k_highlightDrawableKey},
            };
            
            //-------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param The list of pointer Ids.
            /// @param The pointer Id to find.
            ///
            /// @return Whether or not the given list contains the given id.
            //-------------------------------------------------------------------
            bool Contains(const std::vector<Input::Pointer::Id>& in_ids, Input::Pointer::Id in_id)
            {
                for (auto& id : in_ids)
                {
                    if (id == in_id)
                    {
                        return true;
                    }
                }
                
                return false;
            }
            //-------------------------------------------------------------------
            /// Removes the given pointer id from the given list. If the pointer
            /// Id doesn't exist then the application is considered to be in an
            /// irrecoverable state and will terminate.
            ///
            /// @author Ian Copland
            ///
            /// @param The list of pointer Ids.
            /// @param The pointer Id to remove.
            //-------------------------------------------------------------------
            void Remove(std::vector<Input::Pointer::Id>& in_ids, Input::Pointer::Id in_id)
            {
                for (auto it = in_ids.begin(); it != in_ids.end(); ++it)
                {
                    if ((*it) == in_id)
                    {
                        in_ids.erase(it);
                        return;
                    }
                }
                
                CS_LOG_FATAL("Could not remove Id from Id list as it does not exist.");
            }
        }
        
        CS_DEFINE_NAMEDTYPE(HighlightComponent);
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        const std::vector<PropertyMap::PropertyDesc>& HighlightComponent::GetPropertyDescs()
        {
            return k_propertyDescs;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        HighlightComponent::HighlightComponent(const std::string& in_componentName, const PropertyMap& in_properties)
            : Component(in_componentName)
        {
            RegisterProperty<IDrawableSPtr>(k_normalDrawableKey, Core::MakeDelegate(this, &HighlightComponent::GetNormalDrawable), Core::MakeDelegate(this, &HighlightComponent::SetNormalDrawable));
            RegisterProperty<IDrawableSPtr>(k_highlightDrawableKey, Core::MakeDelegate(this, &HighlightComponent::GetHighlightDrawable), Core::MakeDelegate(this, &HighlightComponent::SetHighlightDrawable));
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
        const IDrawableSPtr& HighlightComponent::GetNormalDrawable() const
        {
            return m_normalDrawable;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        const IDrawableSPtr& HighlightComponent::GetHighlightDrawable() const
        {
            return m_highlightDrawable;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void HighlightComponent::SetNormalDrawable(const IDrawableSPtr& in_drawable)
        {
            m_normalDrawable = in_drawable;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void HighlightComponent::SetHighlightDrawable(const IDrawableSPtr& in_drawable)
        {
            m_highlightDrawable = in_drawable;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void HighlightComponent::Highlight()
        {
            CS_ASSERT(m_highlighted == false, "Cannot highlight when already highlighted.");
            
            m_highlighted = true;
            GetWidget()->SetDrawable(m_highlightDrawable);
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void HighlightComponent::Unhighlight()
        {
            CS_ASSERT(m_highlighted == true, "Cannot unhighlight when already unhighlighted.");
            
            m_highlighted = false;
            GetWidget()->SetDrawable(m_normalDrawable);
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void HighlightComponent::OnInit()
        {
            GetWidget()->SetDrawable(m_normalDrawable);
            
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
                CS_ASSERT(Contains(m_activePointerIds, in_pointer.GetId()) == false, "Received pressed event for already pressed Id.");
                CS_ASSERT(Contains(m_highlightingPointerIds, in_pointer.GetId()) == false, "Received pressed event for already highlighting Id.");
                
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
            if (Contains(m_activePointerIds, in_pointer.GetId()) == true)
            {
                CS_ASSERT(Contains(m_highlightingPointerIds, in_pointer.GetId()) == false, "Received move entered for already highlighting Id.");
                
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
            if (Contains(m_activePointerIds, in_pointer.GetId()) == true)
            {
                CS_ASSERT(Contains(m_highlightingPointerIds, in_pointer.GetId()) == true, "Received move exited event for an id that is not in the highlighting list.");
                
                Remove(m_highlightingPointerIds, in_pointer.GetId());
                
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
            if (Contains(m_activePointerIds, in_pointer.GetId()) == true)
            {
                CS_ASSERT(Contains(m_highlightingPointerIds, in_pointer.GetId()) == true, "Received released inside event for an id that is not in the highlighting list.");
                
                Remove(m_activePointerIds, in_pointer.GetId());
                Remove(m_highlightingPointerIds, in_pointer.GetId());
                
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
            if (Contains(m_activePointerIds, in_pointer.GetId()) == true)
            {
                CS_ASSERT(Contains(m_highlightingPointerIds, in_pointer.GetId()) == false, "Received released inside event for an id that is in the highlighting list.");
                
                Remove(m_activePointerIds, in_pointer.GetId());
                
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
            
            Unhighlight();
            m_activePointerIds.clear();
            m_highlightingPointerIds.clear();
        }
    }
}