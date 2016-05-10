//
//  HighlightUIComponent.h
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

#ifndef _CHILLISOURCE_UI_BUTTON_HIGHLIGHTUICOMPONENT_H_
#define _CHILLISOURCE_UI_BUTTON_HIGHLIGHTUICOMPONENT_H_

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
    /// "NormalDrawable": A description of the drawable that will be used when
    /// the parent widget is not highlighted.
    ///
    /// "HighlightDrawable": A description of the drawable that will be used when
    /// the parent widget is highlighted. If no highlight drawable is set the
    /// normal drawable will be used instead.
    ///
    /// "HighlightColour": The colour that the widget will be set to while
    /// highlighted. This defaults to white.
    ///
    /// @author Ian Copland
    //--------------------------------------------------------------------------
    class HighlightUIComponent final : public UIComponent
    {
    public:
        CS_DECLARE_NAMEDTYPE(HighlightUIComponent);
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
        /// it is not pressed.
        //-------------------------------------------------------------------
        const UIDrawableDefCSPtr& GetNormalDrawableDef() const;
        //-------------------------------------------------------------------
        /// @author Ian Copland
        ///
        /// @return the drawable that will be set on the owning widget when
        /// it is pressed.
        //-------------------------------------------------------------------
        const UIDrawableDefCSPtr& GetHighlightDrawableDef() const;
        //-------------------------------------------------------------------
        /// @author Ian Copland
        ///
        /// @return The highlight colour
        //-------------------------------------------------------------------
        const Colour& GetHighlightColour() const;
        //-------------------------------------------------------------------
        /// Sets the drawable that will be set on the owning widget when
        /// it is not pressed.
        ///
        /// @author Ian Copland
        ///
        /// @param The drawable.
        //-------------------------------------------------------------------
        void SetNormalDrawableDef(const UIDrawableDefCSPtr& in_drawableDef);
        //-------------------------------------------------------------------
        /// Sets the drawable that will be set on the owning widget when
        /// it is pressed.
        ///
        /// @author Ian Copland
        ///
        /// @param The drawable.
        //-------------------------------------------------------------------
        void SetHighlightDrawableDef(const UIDrawableDefCSPtr& in_drawableDef);
        //-------------------------------------------------------------------
        /// Sets the highlight colour.
        ///
        /// @author Ian Copland
        ///
        /// @param The colour.
        //-------------------------------------------------------------------
        void SetHighlightColour(const Colour& in_colour);
        
    private:
        friend class UIComponentFactory;
        //-------------------------------------------------------------------
        /// Constructor that builds the component from key-value properties.
        /// The properties used to create a highlight component are described
        /// in the class documentation.
        ///
        /// @author Ian Copland
        ///
        /// @param The component name.
        /// @param The property map.
        //-------------------------------------------------------------------
        HighlightUIComponent(const std::string& in_componentName, const PropertyMap& in_properties);
        //-------------------------------------------------------------------
        /// Enables the highlight on the owning widget.
        ///
        /// @author Ian Copland
        //-------------------------------------------------------------------
        void Highlight();
        //-------------------------------------------------------------------
        /// Enables the highlight on the owning widget.
        ///
        /// @author Ian Copland
        //-------------------------------------------------------------------
        void Unhighlight();
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
        
        UIDrawableDefCSPtr m_normalDrawableDef;
        UIDrawableDefCSPtr m_highlightDrawableDef;
        Colour m_highlightColour;
        
        DrawableUIComponent* m_drawableComponent = nullptr;
        
        bool m_highlighted = false;
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