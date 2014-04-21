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
#include <ChilliSource/Core/Math/Matrix3x3.h>
#include <ChilliSource/Core/Math/UnifiedCoordinates.h>
#include <ChilliSource/Rendering/Base/AlignmentAnchors.h>
#include <ChilliSource/UI/Drawable/IDrawable.h>
#include <ChilliSource/UI/Layout/ILayout.h>

#include <unordered_map>
#include <vector>

namespace ChilliSource
{
    namespace UI
    {
        //----------------------------------------------------------------------------------------
        /// The Widget class that holds the components for laying out, rendering and manipulating
        /// UI views. A widget can be a single view or a collection of views.
        /// Widgets can be added as subviews or have subviews added to them. Widgets are
        /// layed out using a mixture of absolute and relative coordinates in which relative coordinates
        /// are relative to the parent view
        ///
        /// @author S Downie
        //----------------------------------------------------------------------------------------
        class Widget
        {
        public:

            //----------------------------------------------------------------------------------------
            /// Constructor
            //----------------------------------------------------------------------------------------
            Widget()
            {
                //TODO: Restrict what is allowed to be set on a property
                //TEST STUFF
                std::vector<PropertyDesc> descs = { {"WiggleFactor", "Float"}, {"Visible", "Bool"}, {"SizePolicy", "String"}, {"CustomPointer", "Pointer"} };
                CreateProperties(descs);
            }
            //----------------------------------------------------------------------------------------
            /// Holds the definition of a property that allow it to be set and get from code. The
            /// description contains the name and type of the property
            ///
            /// @author S Downie
            //----------------------------------------------------------------------------------------
            struct PropertyDesc
            {
                std::string m_name;
                std::string m_type;
            };
            //----------------------------------------------------------------------------------------
            /// The supported property types
            ///
            /// @author S Downie
            //----------------------------------------------------------------------------------------
            enum class PropertyType
            {
                k_unknown,
                k_bool,
                k_int,
                k_float,
                k_string,
                k_pointer
            };
            //----------------------------------------------------------------------------------------
            /// Widgets have custom properties that can be changed. These are created from the
            /// widget description and the property type are immutable. Attempting to create them
            /// again will cause an assertion
            ///
            /// @author S Downie
            ///
            /// @param Property descriptors
            //----------------------------------------------------------------------------------------
            void CreateProperties(const std::vector<PropertyDesc>& in_descs);
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
            /// @param Percentage width of parent (0.0 - 1.0)
            /// @param Percentage height of parent (0.0 - 1.0)
            //----------------------------------------------------------------------------------------
            void SetRelativeSize(f32 in_width, f32 in_height);
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
            /// Set the size of the widget in pixels
            ///
            /// @author S Downie
            ///
            /// @param Width in pixels
            /// @param Height in pixels
            //----------------------------------------------------------------------------------------
            void SetAbsoluteSize(f32 in_width, f32 in_height);
            //----------------------------------------------------------------------------------------
            /// Set the size of the widget in pixels
            ///
            /// @author S Downie
            ///
            /// @param Size in pixels
            //----------------------------------------------------------------------------------------
            void SetAbsoluteSize(const Core::Vector2& in_size);
            //----------------------------------------------------------------------------------------
            /// Set the position of the widget relative to its parent size and anchor point i.e.
            /// if the anchor is bottom left then 0.5, 0.5 will place it in the middle of the parent
            ///
            /// @author S Downie
            ///
            /// @param Offset X as percentage width of parent (0.0 - 1.0)
            /// @param Offset Y as percentage height of parent (0.0 - 1.0)
            //----------------------------------------------------------------------------------------
            void SetRelativePosition(f32 in_x, f32 in_y);
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
            /// Set the position of the widget from the parental anchor in pixels
            ///
            /// @author S Downie
            ///
            /// @param X in pixels
            /// @param Y in pixels
            //----------------------------------------------------------------------------------------
            void SetAbsolutePosition(f32 in_x, f32 in_y);
            //----------------------------------------------------------------------------------------
            /// Set the position of the widget from the parental anchor in pixels
            ///
            /// @author S Downie
            ///
            /// @param Position in pixels
            //----------------------------------------------------------------------------------------
            void SetAbsolutePosition(const Core::Vector2& in_pos);
            //----------------------------------------------------------------------------------------
            /// Move the position of the widget from the parental anchor by the given percentages
            /// of the parent height and width
            ///
            /// @author S Downie
            ///
            /// @param Offset X as percentage width of parent (0.0 - 1.0)
            /// @param Offset Y as percentage height of parent (0.0 - 1.0)
            //----------------------------------------------------------------------------------------
            void RelativeMoveBy(f32 in_x, f32 in_y);
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
            /// @param X in pixels
            /// @param Y in pixels
            //----------------------------------------------------------------------------------------
            void AbsoluteMoveBy(f32 in_x, f32 in_y);
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
            /// Scale the widgets current size about its origin by the given scaler
            ///
            /// @author S Downie
            ///
            /// @param Scaler x
            /// @param Scaler y
            //----------------------------------------------------------------------------------------
            void ScaleBy(f32 in_x, f32 in_y);
            //----------------------------------------------------------------------------------------
            /// Scale the widgets current size about its origin to the given scaler
            ///
            /// @author S Downie
            ///
            /// @param Scaler x
            /// @param Scaler y
            //----------------------------------------------------------------------------------------
            void ScaleTo(f32 in_x, f32 in_y);
            //----------------------------------------------------------------------------------------
            /// Set the alignment anchor of the widget to its parent i.e. if the anchor is middle
            /// centre then the origin of the widget will be at the middle centre of the parent
            ///
            /// @author S Downie
            ///
            /// @param Alignment anchor
            //----------------------------------------------------------------------------------------
            void SetAnchorToParent(Rendering::AlignmentAnchor in_anchor);
            //----------------------------------------------------------------------------------------
            /// Set the alignment anchor that is to be the widgets origin i.e. it's pivot point
            ///
            /// @author S Downie
            ///
            /// @param Alignment anchor
            //----------------------------------------------------------------------------------------
            void SetOriginAnchor(Rendering::AlignmentAnchor in_anchor);
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
            /// Adds a widget as a child of this widget. The widget will be rendered as part of this
            /// hierarchy and any relative coordinates will now be in relation to this widget.
            ///
            /// NOTE: Will assert if the widget already has a parent
            ///
            /// @author S Downie
            ///
            /// @param Widget
            //----------------------------------------------------------------------------------------
            void Add(const WidgetSPtr& in_widget);
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
            /// Set the value of the property with the given name. If no property exists
            /// with the name then it will assert.
            ///
            /// @author S Downie
            ///
            /// @param Name
            /// @param Value
            //----------------------------------------------------------------------------------------
            template<typename TType> void SetProperty(const std::string& in_name, TType in_value);
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
            template<typename TType> TType GetProperty(const std::string& in_name) const;
            //----------------------------------------------------------------------------------------
            /// Draw the view using the currently set drawable. Tell any subviews to draw.
            ///
            /// @author S Downie
            ///
            /// @param Canvas renderer
            //----------------------------------------------------------------------------------------
            void Draw(Rendering::CanvasRenderer* in_renderer);
            //----------------------------------------------------------------------------------------
            /// Destructor
            ///
            /// @author S Downie
            //----------------------------------------------------------------------------------------
            ~Widget();
            
        private:
            friend class Canvas;
            //----------------------------------------------------------------------------------------
            /// Set the pointer to the canvas. This is only called by the canvas when initialising
            /// the canvas widget
            ///
            /// @author S Downie
            ///
            /// @param Canvas
            //----------------------------------------------------------------------------------------
            void SetCanvas(const Widget* in_canvas);
            //----------------------------------------------------------------------------------------
            /// Holds the type of the property and the offset into the blob
            ///
            /// @author S Downie
            //----------------------------------------------------------------------------------------
            struct PropertyLookup
            {
                PropertyType m_type;
                u32 m_offset;
            };
            //----------------------------------------------------------------------------------------
            /// Converts the object type to proprty type and throws compiler error for unsupported
            /// types
            ///
            /// @author S Downie
            ///
            /// @return The internal enum property type of the given object type
            //----------------------------------------------------------------------------------------
            template<typename TType> PropertyType GetType() const;
            //----------------------------------------------------------------------------------------
            /// Calculate the transform matrix of the object based on the local transform and the
            /// parent transform.
            ///
            /// @author S Downie
            ///
            /// @return Screen space transformation matrix
            //----------------------------------------------------------------------------------------
            Core::Matrix3x3 GetFinalTransform() const;
            //----------------------------------------------------------------------------------------
            /// Calculate the screen space position of the object based on the local position, local
            /// alignment to parent, local alignment to origin and the parents final position.
            ///
            /// NOTE: If the position is relative the final position cannot be calculated until
            /// the widget is part of an absolute tree (i.e. one of the widgets up the tree is absolute)
            /// Therefore will assert if the widget is not on the root canvas
            ///
            /// @author S Downie
            ///
            /// @return Screen space position of origin in pixels
            //----------------------------------------------------------------------------------------
            Core::Vector2 GetFinalPosition() const;
            //----------------------------------------------------------------------------------------
            /// Calculate the parent space position of the object based on the local position, local
            /// alignment to parent and local alignment to origin. This method exists to create
            /// a position in pixels that can be used to create the local transform matrix. The local
            /// transform matrix is multiplied with the parent tranform to get the final transform
            /// and this method prevents double transformation by the parent.
            ///
            /// NOTE: If the position is relative the final position cannot be calculated until
            /// the widget is part of an absolute tree (i.e. one of the widgets up the tree is absolute)
            /// Therefore will assert if the widget is not on the root canvas
            ///
            /// @author S Downie
            ///
            /// @return Position of origin in pixels in parent space
            //----------------------------------------------------------------------------------------
            Core::Vector2 GetParentSpacePosition() const;
            //----------------------------------------------------------------------------------------
            /// Calculate the screen space size of the object based on the local size and the
            /// parent size.
            ///
            /// NOTE: If the size is relative the final size cannot be calculated until
            /// the widget is part of an absolute tree (i.e. one of the widgets up the tree is absolute)
            /// Therefore will assert if the widget is not on the root canvas
            ///
            /// @author S Downie
            ///
            /// @return Screen space size in pixels
            //----------------------------------------------------------------------------------------
            Core::Vector2 GetFinalSize() const;
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

        private:
            
            Core::UnifiedVector2 m_localPosition;
            Core::UnifiedVector2 m_localSize;
            Core::Vector2 m_localScale = Core::Vector2::ONE;
            Core::Colour m_localColour;
            f32 m_localRotation = 0.0f;
            
            std::vector<WidgetSPtr> m_children;
            
            std::string m_name;
            
            std::unordered_map<std::string, PropertyLookup> m_blobOffsets;
            u8* m_propertyBlob = nullptr;
            
            IDrawableUPtr m_drawable;
            ILayoutUPtr m_layout;
            
            Widget* m_parent = nullptr;
            const Widget* m_canvas = nullptr;
            
            Rendering::AlignmentAnchor m_originAnchor = Rendering::AlignmentAnchor::k_middleCentre;
            Rendering::AlignmentAnchor m_anchorToParent = Rendering::AlignmentAnchor::k_bottomLeft;
            
            bool m_isLayoutValid = false;
            bool m_isVisible = true;
            bool m_isSubviewClippingEnabled = false;
        };
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        template<typename TType> void Widget::SetProperty(const std::string& in_name, TType in_value)
        {
            auto entry = m_blobOffsets.find(in_name);
            CS_ASSERT(entry != m_blobOffsets.end(), "No UI property with name: " + in_name);
            CS_ASSERT(entry->second.m_type == GetType<TType>(), "Wrong type for property with name " + in_name);
            
            TType* property = (TType*)(m_propertyBlob + entry->second.m_offset);
            *property = std::move(in_value);
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        template<typename TType> TType Widget::GetProperty(const std::string& in_name) const
        {
            auto entry = m_blobOffsets.find(in_name);
            CS_ASSERT(entry != m_blobOffsets.end(), "No UI property with name: " + in_name);
            CS_ASSERT(entry->second.m_type == GetType<TType>(), "Wrong type for property with name " + in_name);
            
            TType* property = (TType*)(m_propertyBlob + entry->second.m_offset);
            return *property;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        template<typename TType> Widget::PropertyType Widget::GetType() const
        {
            static_assert(std::is_pointer<TType>::value, "Property type not supported");
            return PropertyType::k_pointer;
        }
        //----------------------------------------------------------------------------------------
        /// Specialisation to return property type for bool
        ///
        /// @author S Downie
        ///
        /// @return Bool prop type
        //----------------------------------------------------------------------------------------
        template<> Widget::PropertyType Widget::GetType<bool>() const;
        //----------------------------------------------------------------------------------------
        /// Specialisation to return property type for int
        ///
        /// @author S Downie
        ///
        /// @return Int prop type
        //----------------------------------------------------------------------------------------
        template<> Widget::PropertyType Widget::GetType<s32>() const;
        //----------------------------------------------------------------------------------------
        /// Specialisation to return property type for string
        ///
        /// @author S Downie
        ///
        /// @return String prop type
        //----------------------------------------------------------------------------------------
        template<> Widget::PropertyType Widget::GetType<std::string>() const;
        //----------------------------------------------------------------------------------------
        /// Specialisation to return property type for string
        ///
        /// @author S Downie
        ///
        /// @return String prop type
        //----------------------------------------------------------------------------------------
        template<> Widget::PropertyType Widget::GetType<const std::string&>() const;
        //----------------------------------------------------------------------------------------
        /// Specialisation to return property type for string
        ///
        /// @author S Downie
        ///
        /// @return String prop type
        //----------------------------------------------------------------------------------------
        template<> Widget::PropertyType Widget::GetType<const char*>() const;
        //----------------------------------------------------------------------------------------
        /// Specialisation to return property type for float
        ///
        /// @author S Downie
        ///
        /// @return Float prop type
        //----------------------------------------------------------------------------------------
        template<> Widget::PropertyType Widget::GetType<f32>() const;
    }
}

#endif