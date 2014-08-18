//
//  WidgetProxy.h
//  Chilli Source
//  Created by Scott Downie on 18/08/2014.
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

#ifndef _CHILLISOURCE_UI_BASE_WIDGETPROXY_H_
#define _CHILLISOURCE_UI_BASE_WIDGETPROXY_H_

#include <ChilliSource/ChilliSource.h>

namespace ChilliSource
{
    namespace UI
    {
        //----------------------------------------------------------------------------------------
        /// Functions from widget that are exposed to Lua as static functions acting
        /// on the given widget. This allows Lua to manipulate widgets without owning them
        /// as expensive full data.
        ///
        /// @author S Downie
        //----------------------------------------------------------------------------------------
        namespace WidgetProxy
        {
            //----------------------------------------------------------------------------------------
            /// Register all the proxy functions with the given Lua script
            ///
            /// @author S Downie
            ///
            /// @param Lua script
            //----------------------------------------------------------------------------------------
            void RegisterWithLuaScript(Lua::LuaScript* in_script);
            //----------------------------------------------------------------------------------------
            /// Proxy function to allow calling on an instance from Lua script
            ///
            /// @author S Downie
            ///
            /// @param Widget on which to operate
            /// @param Name of widget
            //----------------------------------------------------------------------------------------
            void SetName(Widget* in_widget, const std::string& in_name);
            //----------------------------------------------------------------------------------------
            /// Proxy function to allow calling on an instance from Lua script
            ///
            /// @author S Downie
            ///
            /// @param Widget on which to operate
            ///
            /// @return Name of widget
            //----------------------------------------------------------------------------------------
            const std::string& GetName(Widget* in_widget);
            //----------------------------------------------------------------------------------------
            /// Proxy function to allow calling on an instance from Lua script
            ///
            /// @author S Downie
            ///
            /// @param Widget on which to operate
            ///
            /// @return Drawable or null
            //----------------------------------------------------------------------------------------
            IDrawable* GetDrawable(Widget* in_widget);
            //----------------------------------------------------------------------------------------
            /// Proxy function to allow calling on an instance from Lua script
            ///
            /// Set the percentage size of the widget relative to its parent size i.e. 0.5, 0.5 will
            /// make the widget half the width of the parent and half the height
            ///
            /// @author S Downie
            ///
            /// @param Widget on which to operate
            ///
            /// @param Percentage size of parent (0.0 - 1.0, 0.0 - 1.0)
            //----------------------------------------------------------------------------------------
            void SetRelativeSize(Widget* in_widget, const Core::Vector2& in_size);
            //----------------------------------------------------------------------------------------
            /// Proxy function to allow calling on an instance from Lua script
            ///
            /// @author S Downie
            ///
            /// @param Widget on which to operate
            ///
            /// @return Percentage size of parent (0.0 - 1.0, 0.0 - 1.0)
            //----------------------------------------------------------------------------------------
            Core::Vector2 GetLocalRelativeSize(Widget* in_widget);
            //----------------------------------------------------------------------------------------
            /// Proxy function to allow calling on an instance from Lua script
            ///
            /// Set the size of the widget in pixels
            ///
            /// @author S Downie
            ///
            /// @param Widget on which to operate
            /// @param Size in pixels
            //----------------------------------------------------------------------------------------
            void SetAbsoluteSize(Widget* in_widget, const Core::Vector2& in_size);
            //----------------------------------------------------------------------------------------
            /// Proxy function to allow calling on an instance from Lua script
            ///
            /// @author S Downie
            ///
            /// @param Widget on which to operate
            ///
            /// @return Size in pixels
            //----------------------------------------------------------------------------------------
            Core::Vector2 GetLocalAbsoluteSize(Widget* in_widget);
            //----------------------------------------------------------------------------------------
            /// Proxy function to allow calling on an instance from Lua script
            ///
            /// Set the function that will be used to alter the size in order to maintain the
            /// aspect ratio of the widget. This is usually based on the drawable image size and aspect
            ///
            /// @author S Downie
            ///
            /// @param Widget on which to operate
            /// @param Size policy
            //----------------------------------------------------------------------------------------
            void SetSizePolicy(Widget* in_widget, SizePolicy in_policy);
            //----------------------------------------------------------------------------------------
            /// Proxy function to allow calling on an instance from Lua script
            ///
            /// @author S Downie
            ///
            /// @param Widget on which to operate
            ///
            /// @return Size policy
            //----------------------------------------------------------------------------------------
            SizePolicy GetSizePolicy(Widget* in_widget);
            //----------------------------------------------------------------------------------------
            /// Proxy function to allow calling on an instance from Lua script
            ///
            /// Set the position of the widget relative to its parent size and anchor point i.e.
            /// if the anchor is bottom left then 0.5, 0.5 will place it in the middle of the parent
            ///
            /// @author S Downie
            ///
            /// @param Widget on which to operate
            /// @param Offset as percentage size of parent (0.0 - 1.0, 0.0 - 1.0)
            //----------------------------------------------------------------------------------------
            void SetRelativePosition(Widget* in_widget, const Core::Vector2& in_pos);
            //----------------------------------------------------------------------------------------
            /// Proxy function to allow calling on an instance from Lua script
            ///
            /// @author S Downie
            ///
            /// @param Widget on which to operate
            ///
            /// @return Offset as percentage size of parent (0.0 - 1.0, 0.0 - 1.0)
            //----------------------------------------------------------------------------------------
            Core::Vector2 GetLocalRelativePosition(Widget* in_widget);
            //----------------------------------------------------------------------------------------
            /// Proxy function to allow calling on an instance from Lua script
            ///
            /// Set the position of the widget from the parental anchor in pixels
            ///
            /// @author S Downie
            ///
            /// @param Widget on which to operate
            /// @param Position in pixels from parent anchor
            //----------------------------------------------------------------------------------------
            void SetAbsolutePosition(Widget* in_widget, const Core::Vector2& in_pos);
            //----------------------------------------------------------------------------------------
            /// Proxy function to allow calling on an instance from Lua script
            ///
            /// @author S Downie
            ///
            /// @param Widget on which to operate
            ///
            /// @return Position in pixels from parent anchor
            //----------------------------------------------------------------------------------------
            Core::Vector2 GetLocalAbsolutePosition(Widget* in_widget);
            //----------------------------------------------------------------------------------------
            /// Proxy function to allow calling on an instance from Lua script
            ///
            /// Move the position of the widget from the parental anchor by the given percentages
            /// of the parent height and width
            ///
            /// @author S Downie
            ///
            /// @param Widget on which to operate
            /// @param Offset as percentage size of parent (0.0 - 1.0, 0.0 - 1.0)
            //----------------------------------------------------------------------------------------
            void RelativeMoveBy(Widget* in_widget, const Core::Vector2& in_translate);
            //----------------------------------------------------------------------------------------
            /// Proxy function to allow calling on an instance from Lua script
            ///
            /// Move the position of the widget from the parental anchor by the given pixels
            ///
            /// @author S Downie
            ///
            /// @param Widget on which to operate
            /// @param Translation in pixels
            //----------------------------------------------------------------------------------------
            void AbsoluteMoveBy(Widget* in_widget, const Core::Vector2& in_translate);
            //----------------------------------------------------------------------------------------
            /// Proxy function to allow calling on an instance from Lua script
            ///
            /// Rotate the widget about its origin by the given radians
            ///
            /// @author S Downie
            ///
            /// @param Widget on which to operate
            /// @param Angle or rotation in radians
            //----------------------------------------------------------------------------------------
            void RotateBy(Widget* in_widget, f32 in_angleRads);
            //----------------------------------------------------------------------------------------
            /// Proxy function to allow calling on an instance from Lua script
            ///
            /// Rotate the widget about its origin to the given radians
            ///
            /// @author S Downie
            ///
            /// @param Widget on which to operate
            /// @param Angle or rotation in radians
            //----------------------------------------------------------------------------------------
            void RotateTo(Widget* in_widget, f32 in_angleRads);
            //----------------------------------------------------------------------------------------
            /// Proxy function to allow calling on an instance from Lua script
            ///
            /// @author S Downie
            ///
            /// @param Widget on which to operate
            ///
            /// @return Rotation in radians without any parent rotation applied
            //----------------------------------------------------------------------------------------
            f32 GetLocalRotation(Widget* in_widget);
            //----------------------------------------------------------------------------------------
            /// Proxy function to allow calling on an instance from Lua script
            ///
            /// Scale the widgets current size about its origin by the given scaler
            ///
            /// @author S Downie
            ///
            /// @param Widget on which to operate
            /// @param Scaler width and height
            //----------------------------------------------------------------------------------------
            void ScaleBy(Widget* in_widget, const Core::Vector2& in_scale);
            //----------------------------------------------------------------------------------------
            /// Proxy function to allow calling on an instance from Lua script
            ///
            /// Scale the widgets current size about its origin to the given scaler
            ///
            /// @author S Downie
            ///
            /// @param Widget on which to operate
            /// @param Scaler width and height
            //----------------------------------------------------------------------------------------
            void ScaleTo(Widget* in_widget, const Core::Vector2& in_scale);
            //----------------------------------------------------------------------------------------
            /// Proxy function to allow calling on an instance from Lua script
            ///
            /// @author S Downie
            ///
            /// @param Widget on which to operate
            ///
            /// @return Local scale X Y
            //----------------------------------------------------------------------------------------
            Core::Vector2 GetLocalScale(Widget* in_widget);
            //----------------------------------------------------------------------------------------
            /// Proxy function to allow calling on an instance from Lua script
            ///
            /// Set the alignment anchor of the widget to its parent i.e. if the anchor is middle
            /// centre then the origin of the widget will be at the middle centre of the parent
            ///
            /// @author S Downie
            ///
            /// @param Widget on which to operate
            /// @param Alignment anchor
            //----------------------------------------------------------------------------------------
            void SetParentalAnchor(Widget* in_widget, Rendering::AlignmentAnchor in_anchor);
            //----------------------------------------------------------------------------------------
            /// Proxy function to allow calling on an instance from Lua script
            ///
            /// @author S Downie
            ///
            /// @param Widget on which to operate
            ///
            /// @return Alignment anchor
            //----------------------------------------------------------------------------------------
            Rendering::AlignmentAnchor GetParentalAnchor(Widget* in_widget);
            //----------------------------------------------------------------------------------------
            /// Proxy function to allow calling on an instance from Lua script
            ///
            /// Set the alignment anchor that is to be the widgets origin i.e. it's pivot point
            ///
            /// @author S Downie
            ///
            /// @param Widget on which to operate
            /// @param Alignment anchor
            //----------------------------------------------------------------------------------------
            void SetOriginAnchor(Widget* in_widget, Rendering::AlignmentAnchor in_anchor);
            //----------------------------------------------------------------------------------------
            /// Proxy function to allow calling on an instance from Lua script
            ///
            /// @author S Downie
            ///
            /// @param Widget on which to operate
            ///
            /// @return Alignment anchor
            //----------------------------------------------------------------------------------------
            Rendering::AlignmentAnchor GetOriginAnchor(Widget* in_widget);
            //----------------------------------------------------------------------------------------
            /// Proxy function to allow calling on an instance from Lua script
            ///
            /// Set the colour that is multiplied into the widget. Widgets inherit their parent's
            /// colour.
            ///
            /// @author S Downie
            ///
            /// @param Widget on which to operate
            /// @param Colour
            //----------------------------------------------------------------------------------------
            void SetColour(Widget* in_widget, const Core::Colour& in_colour);
            //----------------------------------------------------------------------------------------
            /// Proxy function to allow calling on an instance from Lua script
            ///
            /// @author S Downie
            ///
            /// @param Widget on which to operate
            ///
            /// @return Local colour
            //----------------------------------------------------------------------------------------
            Core::Colour GetLocalColour(Widget* in_widget);
            //----------------------------------------------------------------------------------------
            /// Proxy function to allow calling on an instance from Lua script
            ///
            /// @author S Downie
            ///
            /// @param Widget on which to operate
            /// @param Set whether the widget hierarchy from here down is visible
            //----------------------------------------------------------------------------------------
            void SetVisible(Widget* in_widget, bool in_visible);
            //----------------------------------------------------------------------------------------
            /// Proxy function to allow calling on an instance from Lua script
            ///
            /// @author S Downie
            ///
            /// @param Widget on which to operate
            ///
            /// @return Whether the widget hierarchy from here down is visible
            //----------------------------------------------------------------------------------------
            bool IsVisible(Widget* in_widget);
            //----------------------------------------------------------------------------------------
            /// Proxy function to allow calling on an instance from Lua script
            ///
            /// NOTE: Clipping does not work well with rotation as it requires an AABB clip region
            ///
            /// @author S Downie
            ///
            /// @param Widget on which to operate
            /// @param Set whether the widget will clip pixels that exceed its bounds
            //----------------------------------------------------------------------------------------
            void SetClippingEnabled(Widget* in_widget, bool in_enabled);
            //----------------------------------------------------------------------------------------
            /// Proxy function to allow calling on an instance from Lua script
            ///
            /// @author S Downie
            ///
            /// @param Widget on which to operate
            ///
            /// @return Set whether the widget will clip pixels that exceed its bounds
            //----------------------------------------------------------------------------------------
            bool IsClippingEnabled(Widget* in_widget);
            //----------------------------------------------------------------------------------------
            /// Proxy function to allow calling on an instance from Lua script
            ///
            /// @author S Downie
            ///
            /// @param Widget to operate on
            ///
            /// @return The first child widget with the given name. Note: this is not recursive
            //----------------------------------------------------------------------------------------
            Widget* GetWidget(Widget* in_widget, const std::string& in_name);
            //----------------------------------------------------------------------------------------
            /// Proxy function to allow calling on an instance from Lua script
            ///
            /// @author S Downie
            ///
            /// @param Widget to operate on
            ///
            /// @return The first internal child widget with the given name. Note: this is not recursive
            //----------------------------------------------------------------------------------------
            Widget* GetInternalWidget(Widget* in_widget, const std::string& in_name);
            //----------------------------------------------------------------------------------------
            /// Proxy function to allow calling on an instance from Lua script
            ///
            /// Bring the subview to the front of its siblings. It will now be rendered on top
            ///
            /// @author S Downie
            ///
            /// @param Widget on which to operate
            //----------------------------------------------------------------------------------------
            void BringToFront(Widget* in_widget);
            //----------------------------------------------------------------------------------------
            /// Proxy function to allow calling on an instance from Lua script
            ///
            /// Bring the subview one place forward to the front of the sibling in front of it.
            ///
            /// @author S Downie
            ///
            /// @param Widget on which to operate
            //----------------------------------------------------------------------------------------
            void BringForward(Widget* in_widget);
            //----------------------------------------------------------------------------------------
            /// Proxy function to allow calling on an instance from Lua script
            ///
            /// Send the subview one place backward to behind the sibling in behind it.
            ///
            /// @author S Downie
            ///
            /// @param Widget on which to operate
            //----------------------------------------------------------------------------------------
            void SendBackward(Widget* in_widget);
            //----------------------------------------------------------------------------------------
            /// Proxy function to allow calling on an instance from Lua script
            ///
            /// Send the subview to the back of its siblings. It will be rendered behind all other
            /// siblings
            ///
            /// @author S Downie
            ///
            /// @param Widget on which to operate
            //----------------------------------------------------------------------------------------
            void SendToBack(Widget* in_widget);
            //----------------------------------------------------------------------------------------
            /// Proxy function to allow calling on an instance from Lua script
            ///
            /// Calculate the screen space position of the object based on the local position, local
            /// alignment to parent, local alignment to origin and the parents final position.
            ///
            /// NOTE: As the relative final position cannot be calculated until
            /// the widget is part of an absolute tree (i.e. one of the widgets up the tree is absolute)
            /// Therefore will assert if the widget is not on the root canvas
            ///
            /// @author S Downie
            ///
            /// @param Widget on which to operate
            ///
            /// @return Screen space position of origin in pixels
            //----------------------------------------------------------------------------------------
            Core::Vector2 GetFinalPosition(Widget* in_widget);
            //----------------------------------------------------------------------------------------
            /// Proxy function to allow calling on an instance from Lua script
            ///
            /// Calculate the screen space size of the object based on the local size and the
            /// parent size.
            ///
            /// NOTE: As the relative the final size cannot be calculated until
            /// the widget is part of an absolute tree (i.e. one of the widgets up the tree is absolute)
            /// Therefore will assert if the widget is not on the root canvas
            ///
            /// @author S Downie
            ///
            /// @param Widget on which to operate
            ///
            /// @return Screen space size in pixels
            //----------------------------------------------------------------------------------------
            Core::Vector2 GetFinalSize(Widget* in_widget);
            //----------------------------------------------------------------------------------------
            /// Proxy function to allow calling on an instance from Lua script
            ///
            /// @author S Downie
            ///
            /// @param Widget on which to operate
            ///
            /// @return The preferred size of the widget based on the current drawable or the
            /// fallback value if there is no drawable
            //----------------------------------------------------------------------------------------
            Core::Vector2 GetPreferredSize(Widget* in_widget);
            //----------------------------------------------------------------------------------------
            /// Proxy function to allow calling on an instance from Lua script
            ///
            /// Calculate the rotation of the object based on the local rotation and the
            /// parent rotation.
            ///
            /// @author S Downie
            ///
            /// @param Widget on which to operate
            ///
            /// @return Final rotation in radians
            //----------------------------------------------------------------------------------------
            f32 GetFinalRotation(Widget* in_widget);
            //----------------------------------------------------------------------------------------
            /// Proxy function to allow calling on an instance from Lua script
            ///
            /// Calculate the scale of the object based on the local scale and the
            /// parent scale.
            ///
            /// @author S Downie
            ///
            /// @param Widget on which to operate
            ///
            /// @return Final scale
            //----------------------------------------------------------------------------------------
            Core::Vector2 GetFinalScale(Widget* in_widget);
            //----------------------------------------------------------------------------------------
            /// Proxy function to allow calling on an instance from Lua script
            ///
            /// Calculate the colour of the object based on the local colour and the
            /// parent colour.
            ///
            /// @author S Downie
            ///
            /// @param Widget on which to operate
            ///
            /// @return Final colour
            //----------------------------------------------------------------------------------------
            Core::Colour GetFinalColour(Widget* in_widget);
        }
    }
}

#endif