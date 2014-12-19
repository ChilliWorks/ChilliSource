//
//  SliderComponent.h
//  Chilli Source
//  Created by Ian Copland on 18/12/2014.
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

#ifndef _CHILLISOURCE_UI_SLIDER_SLIDERCOMPONENT_H_
#define _CHILLISOURCE_UI_SLIDER_SLIDERCOMPONENT_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Container/Property/PropertyMap.h>
#include <ChilliSource/Core/Event/EventConnection.h>
#include <ChilliSource/Input/Pointer/Pointer.h>
#include <ChilliSource/UI/Base/Component.h>
#include <ChilliSource/UI/Slider/SliderDirection.h>

namespace ChilliSource
{
    namespace UI
    {
        //------------------------------------------------------------------------------
        /// A logic component for controlling a slider. This handles both vertical
        /// and horizontal slider types.
        ///
        /// "SliderSize": A float describing the size of the slider relative to the
        /// slider background. The size is dependant on the direction of the slider,
        /// a horzontal slider bases the size of the height of the background, a
        /// vertical slider bases the size of the width of the background. In either
        /// case the slider maintains it aspect ratio based on this size. This
        /// defaults to 1.0.
        ///
        /// "SliderPosition": A float describing the current postition of the slider.
        /// This must be in the range 0.0-1.0. If a value outside this range is provided
        /// the app is considered to be in an irrecoverable state and will terminate.
        /// This defaults to 0.0.
        ///
        /// "SliderDirection": A string describing the direction of the slider. The
        /// possible values are 'Horizontal' or 'Vertical' and defaults to Horizontal'.
        ///
        /// "SliderWidgetName": The name of the internal widget that should be used as
        /// the slider. This cannot be changed after the initial creation of the component.
        /// Attempting to set it using SetProperty will cause the app to be in an
        /// irrecoverable state and terminate. Typically this is not exposed to the user
        /// of the widget.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        class SliderComponent final : public Component
        {
        public:
            CS_DECLARE_NAMEDTYPE(SliderComponent);
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The list of properties supported by a progress bar component.
            //------------------------------------------------------------------------------
            static const std::vector<Core::PropertyMap::PropertyDesc>& GetPropertyDescs();
            //------------------------------------------------------------------------------
            /// Allows querying of whether or not the component implements the interface
            /// associated with the given interface Id. Typically this won't be called
            /// directly, instead the templated version IsA<Interface>() should be used.
            ///
            /// @author Ian Copland
            ///
            /// @param The interface Id.
            ///
            /// @return Whether the object implements the given interface.
            //------------------------------------------------------------------------------
            bool IsA(Core::InterfaceIDType in_interfaceId) const override;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param The size of the slider relative to the slider background. The size is
            /// dependant on the direction of the slider, a horzontal slider bases the size
            /// of the height of the background, a vertical slider bases the size of the
            /// width of the background. In either case the slider maintains it aspect ratio
            /// based on this size.
            //------------------------------------------------------------------------------
            f32 GetSliderSize() const;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param The current position of the slider. This is in the range 0.0 - 1.0.
            //------------------------------------------------------------------------------
            f32 GetSliderPosition() const;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param The direction of the slider: horizontal or vertical.
            //------------------------------------------------------------------------------
            SliderDirection GetSliderDirection() const;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The name of the slider widget.
            //------------------------------------------------------------------------------
            const std::string& GetSliderWidgetName() const;
            //------------------------------------------------------------------------------
            /// Sets the size of the slider relative to the slider background. The size is
            /// dependant on the direction of the slider, a horzontal slider bases the size
            /// of the height of the background, a vertical slider bases the size of the
            /// width of the background. In either case the slider maintains it aspect ratio
            /// based on this size.
            ///
            /// @author Ian Copland
            ///
            /// @param The new relative size.
            //------------------------------------------------------------------------------
            void SetSliderSize(f32 in_size);
            //------------------------------------------------------------------------------
            /// Sets the position of the slider. This has to be in the range 0.0 - 1.0. If
            /// it is out of that range the app is considered to be in an irrecoverable
            /// state and will terminate.
            ///
            /// @author Ian Copland
            ///
            /// @param The slider position.
            //------------------------------------------------------------------------------
            void SetSliderPosition(f32 in_sliderPos);
            //------------------------------------------------------------------------------
            /// Sets the direction of the slider: horizontal or vertical.
            ///
            /// @author Ian Copland
            ///
            /// @param The direction.
            //------------------------------------------------------------------------------
            void SetSliderDirection(SliderDirection in_direction);

        private:
            friend class ComponentFactory;
            //------------------------------------------------------------------------------
            /// Constructor that builds the component from key-value properties. The
            /// properties used to create a slider component are described in the class
            /// documentation.
            ///
            /// @author Ian Copland
            ///
            /// @param The component name.
            /// @param The property map.
            //------------------------------------------------------------------------------
            SliderComponent(const std::string& in_componentName, const Core::PropertyMap& in_properties);
            //------------------------------------------------------------------------------
            /// Sets the name of the slider widget. This can only be called once, on
            /// construction. If this is called a second time the app is considered to be in
            /// an irrecoverable state and will terminate.
            ///
            /// @author Ian Copland
            ///
            /// @return The name of the slider widget.
            //------------------------------------------------------------------------------
            void SetSliderWidgetName(const std::string& in_name);
            //------------------------------------------------------------------------------
            /// Updates the size and position of the slider.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            void UpdateSlider();
            //------------------------------------------------------------------------------
            /// Updates the size and position of the slider from a screen space position.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            void UpdateSliderFromScreenPosition(const Core::Vector2& in_screenSpacePos);
            //------------------------------------------------------------------------------
            /// Called when the component is first added to the owning widget.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            void OnInit();
            //------------------------------------------------------------------------------
            /// This is called when the owning widget is added to the canvas.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            void OnAddedToCanvas() override;
            //------------------------------------------------------------------------------
            /// Called when a pointer is pressed inside the bounds of the owning widget.
            ///
            /// @author Ian Copland
            ///
            /// @param The owning widget.
            /// @param The pointer that was pressed.
            /// @param The type of input.
            //------------------------------------------------------------------------------
            void OnPressedInside(Widget* in_widget, const Input::Pointer& in_pointer, Input::Pointer::InputType in_inputType);
            //------------------------------------------------------------------------------
            /// Called when the owning widget receives an input drag event inside its bounds
            /// having previously received one inside the bounds.
            ///
            /// @author Ian Copland
            ///
            /// @param The owning widget.
            /// @param The pointer that was pressed.
            //------------------------------------------------------------------------------
            void OnDraggedInside(Widget* in_widget, const Input::Pointer& in_pointer);
            //------------------------------------------------------------------------------
            /// Called when the owning widget receives an input move event outside its
            /// bounds having previously received one inside the bounds.
            ///
            /// @author Ian Copland
            ///
            /// @param The owning widget.
            /// @param The pointer that was pressed.
            //------------------------------------------------------------------------------
            void OnDraggedOutside(Widget* in_widget, const Input::Pointer& in_pointer);
            //------------------------------------------------------------------------------
            /// Called when a pointer is released inside the bounds of the owning widget
            /// after having been pressed inside.
            ///
            /// @author Ian Copland
            ///
            /// @param The owning widget.
            /// @param The pointer that was pressed.
            /// @param The type of input.
            //------------------------------------------------------------------------------
            void OnReleasedInside(Widget* in_widget, const Input::Pointer& in_pointer, Input::Pointer::InputType in_inputType);
            //------------------------------------------------------------------------------
            /// Called when a pointer is released outside the bounds of the owning widget
            /// after having been pressed inside.
            ///
            /// @author Ian Copland
            ///
            /// @param The owning widget.
            /// @param The pointer that was pressed.
            /// @param The type of input.
            //------------------------------------------------------------------------------
            void OnReleasedOutside(Widget* in_widget, const Input::Pointer& in_pointer, Input::Pointer::InputType in_inputType);
            //------------------------------------------------------------------------------
            /// Called when the owning widget is being destructed.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            void OnDestroy();
            
            f32 m_sliderSize = 1.0f;
            f32 m_sliderPos = 0.5f;
            SliderDirection m_direction = SliderDirection::k_horizontal;
            std::string m_sliderWidgetName;
            
            Widget* m_sliderWidget = nullptr;
            std::vector<Input::Pointer::Id> m_activePointerIds;
            
            Core::EventConnectionUPtr m_pressedInsideConnection;
            Core::EventConnectionUPtr m_draggedInsideConnection;
            Core::EventConnectionUPtr m_draggedOutsideConnection;
            Core::EventConnectionUPtr m_releasedInsideConnection;
            Core::EventConnectionUPtr m_releasedOutsideConnection;
        };
    }
}

#endif