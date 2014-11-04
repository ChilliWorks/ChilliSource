//
//  Widget.h
//  Chilli Source
//  Created by Scott Downie on 17/04/2014.
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

#ifndef _CHILLISOURCE_UI_BASE_WIDGET_H_
#define _CHILLISOURCE_UI_BASE_WIDGET_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/Colour.h>
#include <ChilliSource/Core/Container/concurrent_vector.h>
#include <ChilliSource/Core/Event/Event.h>
#include <ChilliSource/Core/Event/EventConnection.h>
#include <ChilliSource/Core/Math/Matrix3.h>
#include <ChilliSource/Core/Math/UnifiedCoordinates.h>
#include <ChilliSource/Input/Base/Filter.h>
#include <ChilliSource/Input/Pointer/Pointer.h>
#include <ChilliSource/Scripting/Lua/LuaScript.h>
#include <ChilliSource/UI/Base/PropertyAccessor.h>
#include <ChilliSource/UI/Base/PropertyMap.h>
#include <ChilliSource/UI/Base/PropertyType.h>
#include <ChilliSource/UI/Base/SizePolicy.h>
#include <ChilliSource/UI/Base/WidgetDef.h>
#include <ChilliSource/UI/Drawable/IDrawable.h>
#include <ChilliSource/UI/Layout/ILayout.h>

#include <cassert>
#include <functional>
#include <mutex>

namespace ChilliSource
{
    namespace UI
    {
        //----------------------------------------------------------------------------------------
        /// The Widget class that holds the components for laying out, rendering and manipulating
        /// UI widgets. A widget can be a single widget or a collection of widgets.
        /// Widgets can be added to other widgets to create a hierarchy. Widgets are
        /// layed out using a mixture of absolute and relative coordinates in which relative coordinates
        /// are relative to the parent
        ///
        /// Note: Some widgets have private sub-widgets. These are not exposed through the API
        /// and allow the widget to be treated as a solid black box while maintaining the flexibility
        /// of building widgets from smaller blocks.
        ///
        /// @author S Downie
        //----------------------------------------------------------------------------------------
        class Widget final
        {
        public:

            CS_DECLARE_NOCOPY(Widget);
            
            //----------------------------------------------------------------------------------------
            /// Delegate for size policy functions.
            ///
            /// @author S Downie
            ///
            /// @param Widget absolute size
            /// @param Widget absolute preferred size
            ///
            /// @return New size with function applied
            //----------------------------------------------------------------------------------------
            using SizePolicyDelegate = std::function<Core::Vector2(const Core::Vector2&, const Core::Vector2&)>;
            //----------------------------------------------------------------------------------------
            /// Delegate for standard input events such as pressed and released.
            ///
            /// @author S Downie
            ///
            /// @param Widget that the input event occurred on
            /// @param The pointer.
            /// @param The input that triggered the event
            //----------------------------------------------------------------------------------------
            using InputDelegate = std::function<void(Widget*, const Input::Pointer&, Input::Pointer::InputType)>;
            //----------------------------------------------------------------------------------------
            /// Delegate for moving input events.
            ///
            /// @author S Downie
            ///
            /// @param Widget that the input event occurred on
            /// @param The pointer
            //----------------------------------------------------------------------------------------
            using InputMovedDelegate = std::function<void(Widget*, const Input::Pointer&)>;
            //----------------------------------------------------------------------------------------
            /// Constructor that builds the widget from the given definition.
            ///
            /// Default properties exposed to UI files:
            ///
            ///     - Name - String - Identifying name
            ///     - RelPosition - f32 f32 - Position of local anchor relative to parent anchor and parent size
            ///     - AbsPosition - f32 f32 - Absolute position of local anchor relative to parent anchor
            ///     - RelSize - f32 f32 - Size relative to the parent size
            ///     - AbsSize - f32 f32 - Absolute size
            ///     - PreferredSize - f32 f32 - Absolute preferred size
            ///     - Scale - f32 f32 - Scale
            ///     - Rotation - f32 - Rotation in radians
            ///     - Colour - f32 f32 f32 f32 - Colour
            ///     - Visible - "true"/"false" - Visiblity flag
            ///     - ClipChildren - "true"/"false" - Whether children that exceed bounds are clipped
            ///     - OriginAnchor - "TopLeft"/"TopCentre"/"TopRight"/"MiddleLeft"/"MiddleCentre"/"MiddleRight"/"BottomLeft"/"BottomCentre"/"BottomRight" - Origin anchor
            ///     - ParentalAnchor - "TopLeft"/"TopCentre"/"TopRight"/"MiddleLeft"/"MiddleCentre"/"MiddleRight"/"BottomLeft"/"BottomCentre"/"BottomRight" - Parent anchor
            ///     - SizePolicy - "None"/"UsePreferredSize"/"UseWidthMaintainingAspect"/"UseHeightMaintainingAspect"/"FitMaintainingAspect"/"FillMaintainingAspect" - Size policy
            ///     - Drawable - Object - See *Drawable.h
            ///     - Layout - Object - See *Layout.h
            ///
            /// @author S Downie
            ///
            /// @param Default property values
            /// @param Custom property values
            //----------------------------------------------------------------------------------------
            Widget(const PropertyMap& in_defaultProperties, const PropertyMap& in_customProperties);
            //----------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return The list of properties supported by widget
            //----------------------------------------------------------------------------------------
            static std::vector<PropertyMap::PropertyDesc> GetPropertyDescs();
            //----------------------------------------------------------------------------------------
            /// Event triggered when a pointer is pressed inside the widget
            ///
            /// @author S Downie
            ///
            /// @return Connectable event
            //----------------------------------------------------------------------------------------
            Core::IConnectableEvent<InputDelegate>& GetPressedInsideEvent();
            //----------------------------------------------------------------------------------------
            /// Event triggered when a pointer is released inside the widget if it was originally pressed
            /// within the widget.
            ///
            /// NOTE: This event is best to use for buttons.
            ///
            /// @author S Downie
            ///
            /// @return Connectable event
            //----------------------------------------------------------------------------------------
            Core::IConnectableEvent<InputDelegate>& GetReleasedInsideEvent();
            //----------------------------------------------------------------------------------------
            /// Event triggered when a pointer is released outside the widget if it was originally pressed within
            /// the widget
            ///
            /// @author S Downie
            ///
            /// @return Connectable event
            //----------------------------------------------------------------------------------------
            Core::IConnectableEvent<InputDelegate>& GetReleasedOutsideEvent();
            //----------------------------------------------------------------------------------------
            /// Event triggered when a pointer is moved outwith the widget having previously been
            /// within it
            ///
            /// @author S Downie
            ///
            /// @return Connectable event
            //----------------------------------------------------------------------------------------
            Core::IConnectableEvent<InputMovedDelegate>& GetMoveExitedEvent();
            //----------------------------------------------------------------------------------------
            /// Event triggered when a pointer is moved inside the widget have previously been outwith it
            ///
            /// @author S Downie
            ///
            /// @return Connectable event
            //----------------------------------------------------------------------------------------
            Core::IConnectableEvent<InputMovedDelegate>& GetMoveEnteredEvent();
            //----------------------------------------------------------------------------------------
            /// Event triggered when a pointer is moved within the widget if it was originally pressed
            /// within the widget while one or more of the input buttons are down
            ///
            /// @author S Downie
            ///
            /// @return Connectable event
            //----------------------------------------------------------------------------------------
            Core::IConnectableEvent<InputMovedDelegate>& GetDraggedInsideEvent();
            //----------------------------------------------------------------------------------------
            /// Event triggered when a pointer is moved outwith the widget if it was originally pressed
            /// within the widget and while one or more of the input buttons are down
            ///
            /// @author S Downie
            ///
            /// @return Connectable event
            //----------------------------------------------------------------------------------------
            Core::IConnectableEvent<InputMovedDelegate>& GetDraggedOutsideEvent();

            //----------------------------------------------------------------------------------------
            /// Set the drawable that handles how to render the widget. If this is null then the
            /// widget will not be visible. The widget takes ownership of the drawable.
            ///
            /// @author S Downie
            ///
            /// @param Drawable
            //----------------------------------------------------------------------------------------
            void SetDrawable(IDrawableUPtr in_drawable);
            //----------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Drawable or null
            //----------------------------------------------------------------------------------------
            IDrawable* GetDrawable() const;
            //----------------------------------------------------------------------------------------
            /// Set the layout that handles how to layout the widget's subviews. If this is null then the
            /// subviews will retain their current size and position. Otherwise the size and position may
            /// be manipulatd by the layout
            ///
            /// @author S Downie
            ///
            /// @param Layout
            //----------------------------------------------------------------------------------------
            void SetLayout(ILayoutUPtr in_layout);
            //----------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Layout or null
            //----------------------------------------------------------------------------------------
            ILayout* GetLayout() const;
            //----------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Name of widget
            //----------------------------------------------------------------------------------------
            void SetName(const std::string& in_name);
            //----------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Name of widget
            //----------------------------------------------------------------------------------------
            const std::string& GetName() const;
            //----------------------------------------------------------------------------------------
            /// Set the percentage size of the widget relative to its parent size i.e. 0.5, 0.5 will
            /// make the widget half the width of the parent and half the height
            ///
            /// @author S Downie
            ///
            /// @param Percentage size of parent (0.0 - 1.0, 0.0 - 1.0)
            //----------------------------------------------------------------------------------------
            void SetRelativeSize(const Core::Vector2& in_size);
            //----------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Percentage size of parent (0.0 - 1.0, 0.0 - 1.0)
            //----------------------------------------------------------------------------------------
            Core::Vector2 GetLocalRelativeSize() const;
            //----------------------------------------------------------------------------------------
            /// Set the size of the widget in pixels
            ///
            /// @author S Downie
            ///
            /// @param Size in pixels
            //----------------------------------------------------------------------------------------
            void SetAbsoluteSize(const Core::Vector2& in_size);
            //----------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Size in pixels
            //----------------------------------------------------------------------------------------
            Core::Vector2 GetLocalAbsoluteSize() const;
            //----------------------------------------------------------------------------------------
            /// The default preferred size is used in cases when there is no drawable to query for its
            /// preferred size. The preferred size is used to maintain aspect ratio depending on the
            /// aspect policy.
            ///
            /// @author S Downie
            ///
            /// @param The preferred size of the widget in absolutes
            //----------------------------------------------------------------------------------------
            void SetDefaultPreferredSize(const Core::Vector2& in_size);
            //----------------------------------------------------------------------------------------
            /// Set the function that will be used to alter the size in order to maintain the
            /// aspect ratio of the widget. This is usually based on the drawable image size and aspect
            ///
            /// @author S Downie
            ///
            /// @param Size policy
            //----------------------------------------------------------------------------------------
            void SetSizePolicy(SizePolicy in_policy);
            //----------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Size policy
            //----------------------------------------------------------------------------------------
            SizePolicy GetSizePolicy() const;
            //----------------------------------------------------------------------------------------
            /// Set the position of the widget relative to its parent size and anchor point i.e.
            /// if the anchor is bottom left then 0.5, 0.5 will place it in the middle of the parent
            ///
            /// @author S Downie
            ///
            /// @param Offset as percentage size of parent (0.0 - 1.0, 0.0 - 1.0)
            //----------------------------------------------------------------------------------------
            void SetRelativePosition(const Core::Vector2& in_pos);
            //----------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Offset as percentage size of parent (0.0 - 1.0, 0.0 - 1.0)
            //----------------------------------------------------------------------------------------
            Core::Vector2 GetLocalRelativePosition() const;
            //----------------------------------------------------------------------------------------
            /// Set the position of the widget from the parental anchor in pixels
            ///
            /// @author S Downie
            ///
            /// @param Position in pixels from parent anchor
            //----------------------------------------------------------------------------------------
            void SetAbsolutePosition(const Core::Vector2& in_pos);
            //----------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Position in pixels from parent anchor
            //----------------------------------------------------------------------------------------
            Core::Vector2 GetLocalAbsolutePosition() const;
            //----------------------------------------------------------------------------------------
            /// Move the position of the widget from the parental anchor by the given percentages
            /// of the parent height and width
            ///
            /// @author S Downie
            ///
            /// @param Offset as percentage size of parent (0.0 - 1.0, 0.0 - 1.0)
            //----------------------------------------------------------------------------------------
            void RelativeMoveBy(const Core::Vector2& in_translate);
            //----------------------------------------------------------------------------------------
            /// Move the position of the widget from the parental anchor by the given pixels
            ///
            /// @author S Downie
            ///
            /// @param Translation in pixels
            //----------------------------------------------------------------------------------------
            void AbsoluteMoveBy(const Core::Vector2& in_translate);
            //----------------------------------------------------------------------------------------
            /// Rotate the widget about its origin by the given radians
            ///
            /// @author S Downie
            ///
            /// @param Angle or rotation in radians
            //----------------------------------------------------------------------------------------
            void RotateBy(f32 in_angleRads);
            //----------------------------------------------------------------------------------------
            /// Rotate the widget about its origin to the given radians
            ///
            /// @author S Downie
            ///
            /// @param Angle or rotation in radians
            //----------------------------------------------------------------------------------------
            void RotateTo(f32 in_angleRads);
            //----------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Rotation in radians without any parent rotation applied
            //----------------------------------------------------------------------------------------
            f32 GetLocalRotation() const;
            //----------------------------------------------------------------------------------------
            /// Scale the widgets current size about its origin by the given scaler
            ///
            /// @author S Downie
            ///
            /// @param Scaler width and height
            //----------------------------------------------------------------------------------------
            void ScaleBy(const Core::Vector2& in_scale);
            //----------------------------------------------------------------------------------------
            /// Scale the widgets current size about its origin to the given scaler
            ///
            /// @author S Downie
            ///
            /// @param Scaler width and height
            //----------------------------------------------------------------------------------------
            void ScaleTo(const Core::Vector2& in_scale);
            //----------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Local scale X Y
            //----------------------------------------------------------------------------------------
            Core::Vector2 GetLocalScale() const;
            //----------------------------------------------------------------------------------------
            /// Set the alignment anchor of the widget to its parent i.e. if the anchor is middle
            /// centre then the origin of the widget will be at the middle centre of the parent
            ///
            /// @author S Downie
            ///
            /// @param Alignment anchor
            //----------------------------------------------------------------------------------------
            void SetParentalAnchor(Rendering::AlignmentAnchor in_anchor);
            //----------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Alignment anchor
            //----------------------------------------------------------------------------------------
            Rendering::AlignmentAnchor GetParentalAnchor() const;
            //----------------------------------------------------------------------------------------
            /// Sets the anchor to which the widget's origin position is aligned. The widgets origin
            /// is the position which the object is rendered and at the position arround which it will
            /// be rotated.
            ///
            /// @author S Downie
            ///
            /// @param The alignment anchor
            //----------------------------------------------------------------------------------------
            void SetOriginAnchor(Rendering::AlignmentAnchor in_anchor);
            //----------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return The anchor to which the widget's origin position is aligned. The widgets origin
            /// is the position which the object is rendered and at the position arround which it will
            /// be rotated.
            //----------------------------------------------------------------------------------------
            Rendering::AlignmentAnchor GetOriginAnchor() const;
            //----------------------------------------------------------------------------------------
            /// Sets the widget origin's absolute position. The widgets origin is the position which
            /// the object is rendered and at the position arround which it will  be rotated. This
            /// position is relative to the Origin Anchor.
            ///
            /// @author Ian Copland
            ///
            /// @param The origin's absolute position.
            //----------------------------------------------------------------------------------------
            void SetOriginAbsolutePosition(const Core::Vector2& in_position);
            //----------------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param The widget's origin absolute position. The widgets origin is the position which
            /// the object is rendered and at the position arround which it will  be rotated. This
            /// position is relative to the Origin Anchor.
            //----------------------------------------------------------------------------------------
            const Core::Vector2& GetOriginAbsolutePosition() const;
            //----------------------------------------------------------------------------------------
            /// Sets the widget origin's relative position. The widgets origin is the position which
            /// the object is rendered and at the position arround which it will  be rotated. This
            /// position is relative to the Origin Anchor.
            ///
            /// @author Ian Copland
            ///
            /// @param The origin's relative position.
            //----------------------------------------------------------------------------------------
            void SetOriginRelativePosition(const Core::Vector2& in_position);
            //----------------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param The widget's origin relative position. The widgets origin is the position which
            /// the object is rendered and at the position arround which it will  be rotated. This
            /// position is relative to the Origin Anchor.
            //----------------------------------------------------------------------------------------
            const Core::Vector2& GetOriginRelativePosition() const;
            //----------------------------------------------------------------------------------------
            /// Set the colour that is multiplied into the widget. Widgets inherit their parent's
            /// colour.
            ///
            /// @author S Downie
            ///
            /// @param Colour
            //----------------------------------------------------------------------------------------
            void SetColour(const Core::Colour& in_colour);
            //----------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Local colour
            //----------------------------------------------------------------------------------------
            Core::Colour GetLocalColour() const;
            //----------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Set whether the widget hierarchy from here down is visible
            //----------------------------------------------------------------------------------------
            void SetVisible(bool in_visible);
            //----------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Whether the widget hierarchy from here down is visible
            //----------------------------------------------------------------------------------------
            bool IsVisible() const;
            //----------------------------------------------------------------------------------------
            /// NOTE: Clipping does not work well with rotation as it requires an AABB clip region
            ///
            /// @author S Downie
            ///
            /// @param Set whether the widget will clip pixels that exceed its bounds
            //----------------------------------------------------------------------------------------
            void SetClippingEnabled(bool in_enabled);
            //----------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Set whether the widget will clip pixels that exceed its bounds
            //----------------------------------------------------------------------------------------
            bool IsClippingEnabled() const;
            //----------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Whether the widget should accept and respond to user input
            //----------------------------------------------------------------------------------------
            void SetInputEnabled(bool in_input);
            //----------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Whether the widget accepts and responds to user input
            //----------------------------------------------------------------------------------------
            bool IsInputEnabled() const;
            //----------------------------------------------------------------------------------------
            /// Set whether this widget consumes input events and prevents them being continuing down the
            /// event chain. Only press and release events are consumed.
            ///
            /// @author S Downie
            ///
            /// @param Whether press and release input events are consumed
            //----------------------------------------------------------------------------------------
            void SetInputConsumeEnabled(bool in_consume);
            //----------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Whether press and release input events are consumed
            //----------------------------------------------------------------------------------------
            bool IsInputConsumeEnabled() const;
            //----------------------------------------------------------------------------------------
            /// Adds a widget as a child of this widget. The widget will be rendered as part of this
            /// hierarchy and any relative coordinates will now be in relation to this widget.
            ///
            /// NOTE: Will assert if the widget already has a parent
            ///
            /// @author S Downie
            ///
            /// @param Widget to add
            //----------------------------------------------------------------------------------------
            void AddWidget(const WidgetSPtr& in_widget);
            //----------------------------------------------------------------------------------------
            /// Remove the child widget from this widget. It will no longer be rendered and may
            /// be destroyed if this is the last reference.
            ///
            /// NOTE: Will assert if the parents do not match
            ///
            /// @author S Downie
            ///
            /// @param Widget to remove
            //----------------------------------------------------------------------------------------
            void RemoveWidget(Widget* in_widget);
            //----------------------------------------------------------------------------------------
            /// Remove the widget from the child list of its parent. It will no longer be rendered and may
            /// be destroyed if the parent holds the last reference
            ///
            /// NOTE: Will assert if has no parent
            ///
            /// @author S Downie
            //----------------------------------------------------------------------------------------
            void RemoveFromParent();
            //----------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return The first child widget with the given name. Note: this is not recursive
            //----------------------------------------------------------------------------------------
            WidgetSPtr GetWidget(const std::string& in_name);
            //----------------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return A constant version of the first child widget with the given name. This is not
            /// recursive.
            //----------------------------------------------------------------------------------------
            WidgetCSPtr GetWidget(const std::string& in_name) const;
            //----------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return The first internal child widget with the given name. Note: this is not recursive
            //----------------------------------------------------------------------------------------
            Widget* GetInternalWidget(const std::string& in_name);
            //----------------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return A constant version of the first internal child widget with the given name. This
            /// is not recursive.
            //----------------------------------------------------------------------------------------
            const Widget* GetInternalWidget(const std::string& in_name) const;
            //----------------------------------------------------------------------------------------
            /// Constructs a list of child widgets and returns (Use sparingly)
            ///
            /// @author HMcLaughlin
            ///
            /// @return A vector of all child widgets
            //----------------------------------------------------------------------------------------
            std::vector<WidgetSPtr> GetWidgets();
            //----------------------------------------------------------------------------------------
            /// Returns a const list of the widgets children. A copy of the internal list is created
            /// so this is relatively expensive if the the widget has a lot of children.
            ///
            /// @author Ian Copland
            ///
            /// @return A const list of the child widgets.
            //----------------------------------------------------------------------------------------
            std::vector<WidgetCSPtr> GetWidgets() const;
            //----------------------------------------------------------------------------------------
            /// Constructs a list of internal child widgets and returns (Use sparingly)
            ///
            /// @author HMcLaughlin
            ///
            /// @return A vector of all child widgets
            //----------------------------------------------------------------------------------------
            std::vector<Widget*> GetInternalWidgets();
            //----------------------------------------------------------------------------------------
            /// Returns a const list of the widgets internal children. A copy of the internal list is
            /// created so this is relatively expensive if the the widget has a lot of internal
            /// children.
            ///
            /// @author Ian Copland
            ///
            /// @return A const list of the child widgets.
            //----------------------------------------------------------------------------------------
            std::vector<const Widget*> GetInternalWidgets() const;
            //----------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Parent widget of this widget or null
            //----------------------------------------------------------------------------------------
            Widget* GetParent();
            //----------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Parent widget of this widget or null
            //----------------------------------------------------------------------------------------
            const Widget* GetParent() const;
            //----------------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return Whether or not the widget is currently on a canvas.
            //----------------------------------------------------------------------------------------
            bool IsOnCanvas() const;
            //----------------------------------------------------------------------------------------
            /// Bring the subview to the front of its siblings. It will now be rendered on top
            ///
            /// @author S Downie
            //----------------------------------------------------------------------------------------
            void BringToFront();
            //----------------------------------------------------------------------------------------
            /// Bring the subview one place forward to the front of the sibling in front of it.
            ///
            /// @author S Downie
            //----------------------------------------------------------------------------------------
            void BringForward();
            //----------------------------------------------------------------------------------------
            /// Send the subview one place backward to behind the sibling in behind it.
            ///
            /// @author S Downie
            //----------------------------------------------------------------------------------------
            void SendBackward();
            //----------------------------------------------------------------------------------------
            /// Send the subview to the back of its siblings. It will be rendered behind all other
            /// siblings
            ///
            /// @author S Downie
            //----------------------------------------------------------------------------------------
            void SendToBack();
            //----------------------------------------------------------------------------------------
            /// Calculate the screen space position of the origin of the object object based on the
            /// local position, local alignment to parent and the parents final position.
            ///
            /// NOTE: As the relative final position cannot be calculated until the widget is part of
            /// an absolute tree (i.e. one of the widgets up the tree is absolute) Therefore will assert
            /// if the widget is not on the root canvas
            ///
            /// @author Ian Copland
            ///
            /// @return Screen space position of the origin in pixels
            //----------------------------------------------------------------------------------------
            Core::Vector2 GetFinalPosition() const;
            //----------------------------------------------------------------------------------------
            /// Calculate the screen space position of the centre of the object object based on the
            /// local position, local alignment to parent, local alignment to origin and the parents
            /// final position.
            ///
            /// NOTE: As the relative final position cannot be calculated until the widget is part of
            /// an absolute tree (i.e. one of the widgets up the tree is absolute) Therefore will assert
            /// if the widget is not on the root canvas
            ///
            /// @author S Downie
            ///
            /// @return Screen space position of the centre of the object in pixels
            //----------------------------------------------------------------------------------------
            Core::Vector2 GetFinalPositionOfCentre() const;
            //----------------------------------------------------------------------------------------
            /// Calculate the screen space size of the object based on the local size and the
            /// parent size.
            ///
            /// NOTE: As the relative the final size cannot be calculated until
            /// the widget is part of an absolute tree (i.e. one of the widgets up the tree is absolute)
            /// Therefore will assert if the widget is not on the root canvas
            ///
            /// @author S Downie
            ///
            /// @return Screen space size in pixels
            //----------------------------------------------------------------------------------------
            Core::Vector2 GetFinalSize() const;
            //----------------------------------------------------------------------------------------
            /// Returns the size of the rectange this widget will be positioned and sized relative to.
            /// This will either be the size of the parent or the size of the cell within the parents
            /// layout.
            ///
            /// NOTE: As the relative component of the final size cannot be calculated until the
            /// widget is part of an absolute tree (i.e. one of the widgets up the tree is absolute).
            /// Therefore will assert if the widget is not on the root canvas.
            ///
            /// @author Ian Copland
            ///
            /// @return The screen space size that relative positions and size are relative to.
            /// This is in pixels.
            //----------------------------------------------------------------------------------------
            Core::Vector2 GetRelativeReferenceSize() const;
            //----------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return The preferred size of the widget based on the current drawable or the
            /// fallback value if there is no drawable
            //----------------------------------------------------------------------------------------
            Core::Vector2 GetPreferredSize() const;
            //----------------------------------------------------------------------------------------
            /// Calculate the rotation of the object based on the local rotation and the
            /// parent rotation.
            ///
            /// @author S Downie
            ///
            /// @return Final rotation in radians
            //----------------------------------------------------------------------------------------
            f32 GetFinalRotation() const;
            //----------------------------------------------------------------------------------------
            /// Calculate the scale of the object based on the local scale and the
            /// parent scale.
            ///
            /// @author S Downie
            ///
            /// @return Final scale
            //----------------------------------------------------------------------------------------
            Core::Vector2 GetFinalScale() const;
            //----------------------------------------------------------------------------------------
            /// Calculate the colour of the object based on the local colour and the
            /// parent colour.
            ///
            /// @author S Downie
            ///
            /// @return Final colour
            //----------------------------------------------------------------------------------------
            Core::Colour GetFinalColour() const;
            //----------------------------------------------------------------------------------------
            /// Set the value of the property with the given name. If no property exists
            /// with the name then it will assert.
            ///
            /// @author S Downie
            ///
            /// @param Name
            /// @param Value
            //----------------------------------------------------------------------------------------
            template<typename TType> void SetCustomProperty(const std::string& in_name, TType&& in_value);
            //----------------------------------------------------------------------------------------
            /// Specialisation to store property value for const char* as a std::string
            ///
            /// @author S Downie
            ///
            /// @param Property name
            /// @param Property value
            //----------------------------------------------------------------------------------------
            void SetCustomProperty(const std::string& in_name, const char* in_value);
            //----------------------------------------------------------------------------------------
            /// Get the value of the property with the given name. If no property exists
            /// with the name then it will assert.
            ///
            /// @author S Downie
            ///
            /// @param Name
            ///
            /// @return Value
            //----------------------------------------------------------------------------------------
            template<typename TType> TType GetCustomProperty(const std::string& in_name) const;
            //----------------------------------------------------------------------------------------
            /// Specialisation to return property value for const char* which is stored as a std::string
            ///
            /// @author S Downie
            ///
            /// @param Property name
            ///
            /// @return Property value
            //----------------------------------------------------------------------------------------
            const char* GetCustomProperty(const std::string& in_name) const;
            //----------------------------------------------------------------------------------------
            /// Performs a calculation to check if the given position is within the OOBB
            /// of the widget
            ///
            /// @author S Downie
            ///
            /// @param Position to check
            ///
            /// @return Whther the widget contains the point
            //----------------------------------------------------------------------------------------
            bool Contains(const Core::Vector2& in_point) const;
            //----------------------------------------------------------------------------------------
            /// Converts the input screen space point to local space relative to the given alignment
            /// anchor. As this requires the final transform of the widget this will assert if the
            /// widget is not on the canvas.
            ///
            /// @author Ian Copland
            ///
            /// @param The position to convert to local space. This must be in screen space.
            /// @param The anchor to align it to.
            ///
            /// @return The local space position relative to the given anchor.
            //----------------------------------------------------------------------------------------
            Core::Vector2 ToLocalSpace(const Core::Vector2& in_point, Rendering::AlignmentAnchor in_alignmentAnchor) const;
            //----------------------------------------------------------------------------------------
            /// Called when the layout changes forcing this to update its children
            ///
            /// @author S Downie
            ///
            /// @param The layout that changed
            //----------------------------------------------------------------------------------------
            void OnLayoutChanged(const ILayout* in_layout);
        private:
            friend class Canvas;
            friend class WidgetFactory;
            //----------------------------------------------------------------------------------------
            /// Set the pointer to the canvas
            ///
            /// @author S Downie
            ///
            /// @param Canvas
            //----------------------------------------------------------------------------------------
            void SetCanvas(const Widget* in_canvas);
            //----------------------------------------------------------------------------------------
            /// Set the pointer to the parent.
            ///
            /// @author S Downie
            ///
            /// @param Parent
            //----------------------------------------------------------------------------------------
            void SetParent(Widget* in_parent);
            //----------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Default property values
            //----------------------------------------------------------------------------------------
            void SetDefaultProperties(const PropertyMap& in_defaultProperties);
            //----------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Custom property values
            //----------------------------------------------------------------------------------------
            void SetCustomProperties(const PropertyMap& in_customProperties);
            //----------------------------------------------------------------------------------------
            /// Set up the links so that this widget can access the properties of another via the
            /// SetProperty and GetProperty
            ///
            /// @author S Downie
            ///
            /// @param Links to default properties of the specified widget
            /// @param Links to custom properties of the specified widget
            //----------------------------------------------------------------------------------------
            void SetPropertyLinks(std::unordered_map<std::string, IPropertyAccessorUPtr>&& in_defaultLinks, std::unordered_map<std::string, std::pair<Widget*, std::string>>&& in_customLinks);
            //----------------------------------------------------------------------------------------
            /// Sets the Lua script that controls the behaviour of this widget
            ///
            /// @author S Downie
            ///
            /// @param Lua script source
            //----------------------------------------------------------------------------------------
            void SetBehaviourScript(const Scripting::LuaSourceCSPtr& in_behaviourSource);
            //----------------------------------------------------------------------------------------
            /// Adds a widget as a child of this widget. The widget will be rendered as part of this
            /// hierarchy and any relative coordinates will now be in relation to this widget.
            ///
            /// This widget is effectively a private implementation detail and is not affected by the
            /// layout and is not returned when querying for widgets
            ///
            /// NOTE: Will assert if the widget already has a parent
            ///
            /// @author S Downie
            ///
            /// @param Widget to add
            //----------------------------------------------------------------------------------------
            void AddInternalWidget(const WidgetSPtr& in_widget);
            //----------------------------------------------------------------------------------------
            /// Set the layout that handles how to layout the widget's internal subviews. If this is null then the
            /// subviews will retain their current size and position. Otherwise the size and position may
            /// be manipulatd by the layout
            ///
            /// @author S Downie
            ///
            /// @param Layout
            //----------------------------------------------------------------------------------------
            void SetInternalLayout(ILayoutUPtr in_layout);
            //----------------------------------------------------------------------------------------
            /// Calculate the transform matrix of the object based on the local scale, rotation and
            /// position
            ///
            /// @author S Downie
            ///
            /// @return Local transformation matrix
            //----------------------------------------------------------------------------------------
            Core::Matrix3 GetLocalTransform() const;
            //----------------------------------------------------------------------------------------
            /// Calculate the transform matrix of the object based on the local transform and the
            /// parent transform.
            ///
            /// @author S Downie
            ///
            /// @return Screen space transformation matrix
            //----------------------------------------------------------------------------------------
            Core::Matrix3 GetFinalTransform() const;
            //----------------------------------------------------------------------------------------
            /// Calculates the position on the widget's origin in Local Render space, combining the
            /// relative and absolute components of the local position. Local Render space is relative
            /// to the centre of the parent. This takes into account the parents final size, the
            /// parental anchor and the absolute and relative position.
            ///
            /// NOTE: As the position takes into account the size of the parent, this cannot be
            /// calculated until the widget is part of an absolute tree, i.e. one of the widgets up
            /// the tree is absolute. Therefore this will assert if the widget is not on the root
            /// canvas.
            ///
            /// @author S Downie
            ///
            /// @return The position in local render space. This is in pixels.
            //----------------------------------------------------------------------------------------
            Core::Vector2 GetLocalRenderSpaceCombinedPosition() const;
            //----------------------------------------------------------------------------------------
            /// Calculates the local position of the origin, combining the relative and absolute
            /// components of the origin position. This is relative to the origin anchor point. This
            /// takes into account the final size of the parent and the relative and absolute origin
            /// position.
            ///
            /// NOTE: As part of the origin's position is relative the combined local space position
            /// cannot be calculated until the widget is part of an absolute tree, i.e. one of the
            /// widgets up the tree is absolute. Therefore this will assert if the widget is not on
            /// the rootcanvas.
            ///
            /// @author Ian Copland
            ///
            /// @return Position of the origin anchor in local space. This is in pixels.
            //----------------------------------------------------------------------------------------
            Core::Vector2 GetLocalOriginCombinedPosition() const;
            //----------------------------------------------------------------------------------------
            /// Calculates the reference size of the given child widget. This will be either the size
            /// of the cell in the layout the child is in, or the size of this if there is no layout.
            ///
            /// As this takes into account the final size, this must be part of an absolute hierarchy,
            /// i.e it must be in the canvas widget's hierarchy. If not, this will assert.
            ///
            /// @author Ian Copland
            ///
            /// @param The child widget.
            ///
            /// @return The reference size in absolute screen space pixels.
            //----------------------------------------------------------------------------------------
            Core::Vector2 CalculateRelativeReferenceSizeForChild(const Widget* in_child);
            //----------------------------------------------------------------------------------------
            /// Returns the layout and index into that layout for the given children.
            ///
            /// @author Ian Copland
            ///
            /// @param The child widget.
            ///
            /// @return A pair containing the layout and index. The layout can be null if the the child
            /// is not in a layout.
            //----------------------------------------------------------------------------------------
            std::pair<ILayout*, s32> GetLayoutForChild(const Widget* in_child);
            //----------------------------------------------------------------------------------------
            /// Called when the out transform changes forcing this to update its caches
            ///
            /// @author S Downie
            //----------------------------------------------------------------------------------------
            void InvalidateTransformCache();
            //----------------------------------------------------------------------------------------
            /// Called when the parent transform changes forcing this to update its caches
            ///
            /// @author S Downie
            //----------------------------------------------------------------------------------------
            void OnParentTransformChanged();
            //----------------------------------------------------------------------------------------
            /// Update this widget and any sub widgets
            ///
            /// @author S Downie
            ///
            /// @param Time in seconds since last update
            //----------------------------------------------------------------------------------------
            void Update(f32 in_timeSinceLastUpdate);
            //----------------------------------------------------------------------------------------
            /// Draw the view using the currently set drawable. Tell any subviews to draw.
            ///
            /// @author S Downie
            ///
            /// @param Canvas renderer
            //----------------------------------------------------------------------------------------
            void Draw(Rendering::CanvasRenderer* in_renderer);
            //-----------------------------------------------------------
            /// Called when the canvas receives cursor/touch input
            ///
            /// @author S Downie
            ///
            /// @param The pointer
            /// @param The timestamp.
            /// @param The press type.
            /// @param Filter object to check if the event has been filtered or to filter it
            //-----------------------------------------------------------
            void OnPointerDown(const Input::Pointer& in_pointer, f64 in_timestamp, Input::Pointer::InputType in_inputType, Input::Filter& in_filter);
            //-----------------------------------------------------------
            /// Called when the canvas receives cursor/touch move input
            ///
            /// @author S Downie
            ///
            /// @param The pointer
            /// @param The timestamp.
            //-----------------------------------------------------------
            void OnPointerMoved(const Input::Pointer& in_pointer, f64 in_timestamp);
            //-----------------------------------------------------------
            /// Called when the canvas receiving cursor/touch release input
            ///
            /// @author S Downie
            ///
            /// @param The pointer
            /// @param The timestamp.
            /// @param The press type.
            //-----------------------------------------------------------
            void OnPointerUp(const Input::Pointer& in_pointer, f64 in_timestamp, Input::Pointer::InputType in_inputType);
            
        private:
            
            PropertyMap m_customProperties;
            std::unordered_map<std::string, IPropertyAccessorUPtr> m_defaultPropertyLinks;
            std::unordered_map<std::string, std::pair<Widget*, std::string>> m_customPropertyLinks;
            
            std::unordered_map<Input::Pointer::Id, std::set<Input::Pointer::InputType>> m_pressedInput;
            
            Core::Event<InputDelegate> m_pressedInsideEvent;
            Core::Event<InputDelegate> m_releasedInsideEvent;
            Core::Event<InputDelegate> m_releasedOutsideEvent;
            Core::Event<InputMovedDelegate> m_moveExitedEvent;
            Core::Event<InputMovedDelegate> m_moveEnteredEvent;
            Core::Event<InputMovedDelegate> m_draggedInsideEvent;
            Core::Event<InputMovedDelegate> m_draggedOutsideEvent;
            
            Core::UnifiedVector2 m_localPosition;
            Core::UnifiedVector2 m_localSize;
            Core::Vector2 m_preferredSize;
            Core::Vector2 m_localScale;
            Core::Colour m_localColour;
            f32 m_localRotation;
            
            mutable Core::Matrix3 m_cachedLocalTransform;
            mutable Core::Matrix3 m_cachedFinalTransform;
            mutable Core::Vector2 m_cachedFinalPosition;
            mutable Core::Vector2 m_cachedFinalSize;
            
            SizePolicy m_sizePolicy;
            SizePolicyDelegate m_sizePolicyDelegate;
            
            Core::concurrent_vector<WidgetSPtr> m_internalChildren;
            Core::concurrent_vector<WidgetSPtr> m_children;
            
            std::string m_name;
            
            IDrawableUPtr m_drawable;
            ILayoutUPtr m_layout;
            ILayoutUPtr m_internalLayout;
            
            Scripting::LuaScriptUPtr m_behaviourScript;
            
            Widget* m_parent = nullptr;
            const Widget* m_canvas = nullptr;
            
            Rendering::AlignmentAnchor m_parentalAnchor;
            Rendering::AlignmentAnchor m_originAnchor;
            Core::UnifiedVector2 m_originPosition;
            
            bool m_isVisible;
            bool m_isSubviewClippingEnabled;
            bool m_isInputEnabled;
            bool m_isInputConsumeEnabled;
            
            mutable bool m_isParentTransformCacheValid = false;
            mutable bool m_isLocalTransformCacheValid = false;
            mutable bool m_isLocalSizeCacheValid = false;
            mutable bool m_isParentSizeCacheValid = false;
            
            mutable std::mutex m_sizeMutex;
    
            Core::Screen* m_screen;
        };
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        template<typename TType> void Widget::SetCustomProperty(const std::string& in_name, TType&& in_value)
        {
            auto itDefault = m_defaultPropertyLinks.find(in_name);
            if(itDefault != m_defaultPropertyLinks.end())
            {
                auto accessor = CS_SMARTCAST(PropertyAccessor<TType>*, itDefault->second.get());
                accessor->Set(std::forward<TType>(in_value));
                return;
            }
            
            auto itCustom = m_customPropertyLinks.find(in_name);
            if(itCustom != m_customPropertyLinks.end())
            {
                itCustom->second.first->SetCustomProperty<TType>(itCustom->second.second, std::forward<TType>(in_value));
                return;
            }
            
            m_customProperties.SetProperty(in_name, std::forward<TType>(in_value));
            
            if(m_behaviourScript != nullptr)
            {
                m_behaviourScript->CallFunction("onCustomPropertyChanged", Scripting::LuaScript::FunctionNotFoundPolicy::k_failSilent, in_name, std::forward<TType>(in_value));
            }
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        template<typename TType> TType Widget::GetCustomProperty(const std::string& in_name) const
        {
            auto itDefault = m_defaultPropertyLinks.find(in_name);
            if(itDefault != m_defaultPropertyLinks.end())
            {
                auto accessor = CS_SMARTCAST(PropertyAccessor<TType>*, itDefault->second.get());
                return accessor->Get();
            }
            
            auto itCustom = m_customPropertyLinks.find(in_name);
            if(itCustom != m_customPropertyLinks.end())
            {
                return itCustom->second.first->GetCustomProperty<TType>(itCustom->second.second);
            }
            
            return m_customProperties.GetProperty<TType>(in_name);
        }
    }
}

#endif