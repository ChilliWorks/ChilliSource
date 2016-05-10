//
//  ToggleHighlightUIComponent.h
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

#ifndef _CHILLISOURCE_UI_BUTTON_TOGGLEHIGHLIGHTUICOMPONENT_H_
#define _CHILLISOURCE_UI_BUTTON_TOGGLEHIGHLIGHTUICOMPONENT_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/Colour.h>
#include <ChilliSource/Core/Container/Property/PropertyMap.h>
#include <ChilliSource/Core/Event/EventConnection.h>
#include <ChilliSource/Input/Pointer/Pointer.h>
#include <ChilliSource/UI/Base/UIComponent.h>

namespace ChilliSource
{
    //--------------------------------------------------------------------------
    /// A logic component for changing the drawable used to render a widget
    /// when it has been pressed down. A highlight component has the following
    /// properties:
    ///
    /// "NormalOffDrawable": A description of the drawable that will be used when
    /// the parent widget is not highlighted and is toggled off.
    ///
    /// "HighlightOffDrawable": A description of the drawable that will be used when
    /// the parent widget is highlighted and is toggled off.
    ///
    /// "HighlightOffColour": The colour set when the button is highlighted while
    /// off.
    ///
    /// "NormalOnDrawable": A description of the drawable that will be used when
    /// the parent widget is not highlighted and is toggled on.
    ///
    /// "HighlightOnDrawable": A description of the drawable that will be used when
    /// the parent widget is highlighted and is toggled on.
    ///
    /// "HighlightOnColour": The colour set when the button is highlighted while
    /// on.
    ///
    /// "ToggledOn": A boolean describing whether or not the toggle is on of
    /// off.
    ///
    /// @author Ian Copland
    //--------------------------------------------------------------------------
    class ToggleHighlightUIComponent final : public UIComponent
    {
    public:
        CS_DECLARE_NAMEDTYPE(ToggleHighlightUIComponent);
        //-------------------------------------------------------------------
        /// @author Ian Copland
        ///
        /// @return The list of properties supported by a highlight component.
        //-------------------------------------------------------------------
        static const std::vector<PropertyMap::PropertyDesc>& GetPropertyDescs();
        //-------------------------------------------------------------------
        /// Allows querying of whether or not the component implements the
        /// interface associated with the given interface Id. Typically
        /// this won't be called directly, instead the templated version
        /// IsA<Interface>() should be used.
        ///
        /// @author Ian Copland
        ///
        /// @param The interface Id.
        ///
        /// @return Whether the object implements the given interface.
        //-------------------------------------------------------------------
        bool IsA(InterfaceIDType in_interfaceId) const override;
        //-------------------------------------------------------------------
        /// @author Ian Copland
        ///
        /// @return the drawable that will be set on the owning widget when
        /// it is not pressed while toggled off.
        //-------------------------------------------------------------------
        const UIDrawableDefCSPtr& GetNormalOffDrawableDef() const;
        //-------------------------------------------------------------------
        /// @author Ian Copland
        ///
        /// @return the drawable that will be set on the owning widget when
        /// it is pressed while toggled off.
        //-------------------------------------------------------------------
        const UIDrawableDefCSPtr& GetHighlightOffDrawableDef() const;
        //-------------------------------------------------------------------
        /// @author Ian Copland
        ///
        /// @return The colour set when the button is highlighted while off.
        //-------------------------------------------------------------------
        const Colour& GetHighlightOffColour() const;
        //-------------------------------------------------------------------
        /// @author Ian Copland
        ///
        /// @return the drawable that will be set on the owning widget when
        /// it is not pressed while toggled on.
        //-------------------------------------------------------------------
        const UIDrawableDefCSPtr& GetNormalOnDrawableDef() const;
        //-------------------------------------------------------------------
        /// @author Ian Copland
        ///
        /// @return the drawable that will be set on the owning widget when
        /// it is pressed while toggled on.
        //-------------------------------------------------------------------
        const UIDrawableDefCSPtr& GetHighlightOnDrawableDef() const;
        //-------------------------------------------------------------------
        /// @author Ian Copland
        ///
        /// @return The colour set when the button is highlighted while on.
        //-------------------------------------------------------------------
        const Colour& GetHighlightOnColour() const;
        //-------------------------------------------------------------------
        /// @author Ian Copland
        ///
        /// @return Whether or not the toggle is currently on or off.
        //-------------------------------------------------------------------
        bool IsToggledOn() const;
        //-------------------------------------------------------------------
        /// Sets the drawable that will be set on the owning widget when
        /// it is not pressed and is toggled off.
        ///
        /// @author Ian Copland
        ///
        /// @param The drawable.
        //-------------------------------------------------------------------
        void SetNormalOffDrawableDef(const UIDrawableDefCSPtr& in_drawableDef);
        //-------------------------------------------------------------------
        /// Sets the drawable that will be set on the owning widget when
        /// it is pressed and is toggled off.
        ///
        /// @author Ian Copland
        ///
        /// @param The drawable.
        //-------------------------------------------------------------------
        void SetHighlightOffDrawableDef(const UIDrawableDefCSPtr& in_drawableDef);
        //-------------------------------------------------------------------
        /// Sets the colour used when the button is highlighted while off.
        ///
        /// @author Ian Copland
        ///
        /// @param The colour.
        //-------------------------------------------------------------------
        void SetHighlightOffColour(const Colour& in_colour);
        //-------------------------------------------------------------------
        /// Sets the drawable that will be set on the owning widget when
        /// it is not pressed and is toggled on.
        ///
        /// @author Ian Copland
        ///
        /// @param The drawable.
        //-------------------------------------------------------------------
        void SetNormalOnDrawableDef(const UIDrawableDefCSPtr& in_drawableDef);
        //-------------------------------------------------------------------
        /// Sets the drawable that will be set on the owning widget when
        /// it is pressed and is toggled on.
        ///
        /// @author Ian Copland
        ///
        /// @param The drawable.
        //-------------------------------------------------------------------
        void SetHighlightOnDrawableDef(const UIDrawableDefCSPtr& in_drawableDef);
        //-------------------------------------------------------------------
        /// Sets the colour used when the button is highlighted while on.
        ///
        /// @author Ian Copland
        ///
        /// @param The colour.
        //-------------------------------------------------------------------
        void SetHighlightOnColour(const Colour& in_colour);
        //-------------------------------------------------------------------
        /// Sets whether or not the toggle is on or off.
        ///
        /// @author Ian Copland
        ///
        /// @param Whether or not it is on or off.
        //-------------------------------------------------------------------
        void SetToggleOn(bool in_toggleOn);
        
    private:
        friend class UIComponentFactory;
        //-------------------------------------------------------------------
        /// An enum describing the current state of the toggle highlight.
        ///
        /// @author Ian Copland
        //-------------------------------------------------------------------
        enum class ToggleHighlightState
        {
            k_normalOff,
            k_highlightedOff,
            k_normalOn,
            k_highlightedOn
        };
        //-------------------------------------------------------------------
        /// Constructor that builds the component from key-value properties.
        /// The properties used to create a toggle highlight component are
        /// described in the class documentation.
        ///
        /// @author Ian Copland
        ///
        /// @param The component name.
        /// @param The property map.
        //-------------------------------------------------------------------
        ToggleHighlightUIComponent(const std::string& in_componentName, const PropertyMap& in_properties);
        //-------------------------------------------------------------------
        /// @author Ian Copland
        ///
        /// @return Whether or not the component is highlighted.
        //-------------------------------------------------------------------
        bool IsHighlighted() const;
        //-------------------------------------------------------------------
        /// Toggles the current state of the toggle button.
        ///
        /// @author Ian Copland
        //-------------------------------------------------------------------
        void Toggle();
        //-------------------------------------------------------------------
        /// Highlights the toggle button depending on the current toggle
        /// state.
        ///
        /// @author Ian Copland
        //-------------------------------------------------------------------
        void Highlight();
        //-------------------------------------------------------------------
        /// Unhighlights the toggle button depending on the current toggle
        /// state.
        ///
        /// @author Ian Copland
        //-------------------------------------------------------------------
        void Unhighlight();
        //-------------------------------------------------------------------
        /// Puts the toggle highlight component in the normal 'Off' state.
        ///
        /// @author Ian Copland
        //-------------------------------------------------------------------
        void ApplyOffNormal();
        //-------------------------------------------------------------------
        /// Puts the toggle highlight component in the highlighted 'Off' state.
        ///
        /// @author Ian Copland
        //-------------------------------------------------------------------
        void ApplyOffHighlighted();
        //-------------------------------------------------------------------
        /// Puts the toggle highlight component in the normal 'On' state.
        ///
        /// @author Ian Copland
        //-------------------------------------------------------------------
        void ApplyOnNormal();
        //-------------------------------------------------------------------
        /// Puts the toggle highlight component in the highlighted 'On' state.
        ///
        /// @author Ian Copland
        //-------------------------------------------------------------------
        void ApplyOnHightlighted();
        //-------------------------------------------------------------------
        /// Called when the component is first added to the owning widget.
        ///
        /// @author Ian Copland
        //-------------------------------------------------------------------
        void OnInit() override;
        //-------------------------------------------------------------------
        /// Called when a pointer is pressed inside the bounds of the owning
        /// widget. This will enable the highlight.
        ///
        /// @author Ian Copland
        ///
        /// @param The owning widget.
        /// @param The pointer that was pressed.
        /// @param The type of input.
        //-------------------------------------------------------------------
        void OnPressedInside(Widget* in_widget, const Pointer& in_pointer, Pointer::InputType in_inputType);
        //-------------------------------------------------------------------
        /// Called when the owning widget receives an input move event within
        /// its bounds having previously received one outside the bounds.
        /// This will enable the highlight if the pointer was previously
        /// pressed inside the widget.
        ///
        /// @author Ian Copland
        ///
        /// @param The owning widget.
        /// @param The pointer that was pressed.
        //-------------------------------------------------------------------
        void OnMoveEntered(Widget* in_widget, const Pointer& in_pointer);
        //-------------------------------------------------------------------
        /// Called when the owning widget receives an input move event outside
        /// its bounds having previously received one inside the bounds.
        /// This will disable the highlight if there are no other pointers
        /// down on the widget.
        ///
        /// @author Ian Copland
        ///
        /// @param The owning widget.
        /// @param The pointer that was pressed.
        //-------------------------------------------------------------------
        void OnMoveExited(Widget* in_widget, const Pointer& in_pointer);
        //-------------------------------------------------------------------
        /// Called when a pointer is released inside the bounds of the owning
        /// widget after having been pressed inside.
        ///
        /// @author Ian Copland
        ///
        /// @param The owning widget.
        /// @param The pointer that was pressed.
        /// @param The type of input.
        //-------------------------------------------------------------------
        void OnReleasedInside(Widget* in_widget, const Pointer& in_pointer, Pointer::InputType in_inputType);
        //-------------------------------------------------------------------
        /// Called when a pointer is released outside the bounds of the owning
        /// widget after having been pressed inside.
        ///
        /// @author Ian Copland
        ///
        /// @param The owning widget.
        /// @param The pointer that was pressed.
        /// @param The type of input.
        //-------------------------------------------------------------------
        void OnReleasedOutside(Widget* in_widget, const Pointer& in_pointer, Pointer::InputType in_inputType);
        //-------------------------------------------------------------------
        /// Called when the owning widget is being destructed.
        ///
        /// @author Ian Copland
        //-------------------------------------------------------------------
        void OnDestroy() override;
        
        UIDrawableDefCSPtr m_normalOffDrawableDef;
        UIDrawableDefCSPtr m_highlightOffDrawableDef;
        Colour m_highlightOffColour;
        UIDrawableDefCSPtr m_normalOnDrawableDef;
        UIDrawableDefCSPtr m_highlightOnDrawableDef;
        Colour m_highlightOnColour;

        DrawableUIComponent* m_drawableComponent = nullptr;
        
        ToggleHighlightState m_state = ToggleHighlightState::k_normalOff;
        std::vector<Pointer::Id> m_activePointerIds;
        std::vector<Pointer::Id> m_highlightingPointerIds;
        
        EventConnectionUPtr m_pressedInsideConnection;
        EventConnectionUPtr m_moveEnteredConnection;
        EventConnectionUPtr m_moveExitedConnection;
        EventConnectionUPtr m_releasedInsideConnection;
        EventConnectionUPtr m_releasedOutsideConnection;
    };
}

#endif