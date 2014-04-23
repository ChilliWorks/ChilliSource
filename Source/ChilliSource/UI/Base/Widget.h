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

#include <array>
#include <functional>
#include <mutex>
#include <unordered_map>
#include <vector>

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
                k_pointer,
                k_vec2,
                k_vec3
            };
            //----------------------------------------------------------------------------------------
            /// Identifiers for functions that maintain the aspect ratio of the widget based on
            /// current size and preferred size
            ///
            /// @author S Downie
            //----------------------------------------------------------------------------------------
            enum class SizePolicy
            {
                k_none,
                k_usePreferredSize,
                k_useWidthMaintainingAspect,
                k_useHeightMaintainingAspect,
                k_fitMaintainingAspect,
                k_fillMaintainingAspect,
                k_totalNum
            };
            //----------------------------------------------------------------------------------------
            /// Constructor
            ///
            /// @author S Downie
            //----------------------------------------------------------------------------------------
            Widget() = default;
            //----------------------------------------------------------------------------------------
            /// Build the widget from the given defintion. This includes creating the custom properties.
            /// Build should only be called once per instance.
            ///
            /// @author S Downie
            ///
            /// @param Property descriptors
            //----------------------------------------------------------------------------------------
            void Build(const std::vector<PropertyDesc>& in_descs);
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
            void SetParentalAnchor(Rendering::AlignmentAnchor in_anchor);
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
            Widget* GetWidget(const std::string& in_name);
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
            /// Calculate the screen space position of the object based on the local position, local
            /// alignment to parent, local alignment to origin and the parents final position.
            ///
            /// NOTE: As the relative final position cannot be calculated until
            /// the widget is part of an absolute tree (i.e. one of the widgets up the tree is absolute)
            /// Therefore will assert if the widget is not on the root canvas
            ///
            /// @author S Downie
            ///
            /// @return Screen space position of origin in pixels
            //----------------------------------------------------------------------------------------
            Core::Vector2 GetFinalPosition() const;
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
            /// Calculate the transform matrix of the object based on the local scale, rotation and
            /// position
            ///
            /// @author S Downie
            ///
            /// @return Local transformation matrix
            //----------------------------------------------------------------------------------------
            Core::Matrix3x3 GetLocalTransform() const;
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
            /// Calculate the parent space position of the object based on the local position, local
            /// alignment to parent and local alignment to origin. This method exists to create
            /// a position in pixels that can be used to create the local transform matrix. The local
            /// transform matrix is multiplied with the parent tranform to get the final transform
            /// and this method prevents double transformation by the parent.
            ///
            /// NOTE: As the position is relative the final position cannot be calculated until
            /// the widget is part of an absolute tree (i.e. one of the widgets up the tree is absolute)
            /// Therefore will assert if the widget is not on the root canvas
            ///
            /// @author S Downie
            ///
            /// @return Position of origin in pixels in parent space
            //----------------------------------------------------------------------------------------
            Core::Vector2 GetParentSpacePosition() const;
            //----------------------------------------------------------------------------------------
            /// Called when the out transform changes forcing this to update its caches
            ///
            /// @author S Downie
            //----------------------------------------------------------------------------------------
            void OnTransformChanged();
            //----------------------------------------------------------------------------------------
            /// Called when the parent transform changes forcing this to update its caches
            ///
            /// @author S Downie
            //----------------------------------------------------------------------------------------
            void OnParentTransformChanged();
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
        private:
            
            Core::UnifiedVector2 m_localPosition;
            Core::UnifiedVector2 m_localSize;
            Core::Vector2 m_preferredSize = Core::Vector2::ONE;
            Core::Vector2 m_localScale = Core::Vector2::ONE;
            Core::Colour m_localColour;
            f32 m_localRotation = 0.0f;
            
            mutable Core::Matrix3x3 m_cachedLocalTransform;
            mutable Core::Matrix3x3 m_cachedFinalTransform;
            mutable Core::Vector2 m_cachedFinalSize;
            
            std::array<SizePolicyDelegate, (u32)SizePolicy::k_totalNum> m_sizePolicyFuncs;
            SizePolicyDelegate m_sizePolicyDelegate;
            
            std::vector<WidgetSPtr> m_internalChildren;
            std::vector<WidgetSPtr> m_children;
            
            std::string m_name;
            
            std::unordered_map<std::string, PropertyLookup> m_blobOffsets;
            u8* m_propertyBlob = nullptr;
            
            IDrawableUPtr m_drawable;
            ILayoutUPtr m_layout;
            
            Widget* m_parent = nullptr;
            const Widget* m_canvas = nullptr;
            
            Rendering::AlignmentAnchor m_originAnchor = Rendering::AlignmentAnchor::k_middleCentre;
            Rendering::AlignmentAnchor m_parentalAnchor = Rendering::AlignmentAnchor::k_middleCentre;
            
            bool m_isVisible = true;
            bool m_isSubviewClippingEnabled = false;
            
            mutable bool m_isParentTransformCacheValid = false;
            mutable bool m_isLocalTransformCacheValid = false;
            mutable bool m_isLocalSizeCacheValid = false;
            mutable bool m_isParentSizeCacheValid = false;
            
            mutable std::mutex m_sizeMutex;
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
        //----------------------------------------------------------------------------------------
        /// Specialisation to return property type for string
        ///
        /// @author S Downie
        ///
        /// @return Vec2 prop type
        //----------------------------------------------------------------------------------------
        template<> Widget::PropertyType Widget::GetType<Core::Vector2>() const;
        //----------------------------------------------------------------------------------------
        /// Specialisation to return property type for string
        ///
        /// @author S Downie
        ///
        /// @return Vec2 prop type
        //----------------------------------------------------------------------------------------
        template<> Widget::PropertyType Widget::GetType<const Core::Vector2&>() const;
        //----------------------------------------------------------------------------------------
        /// Specialisation to return property type for string
        ///
        /// @author S Downie
        ///
        /// @return Vec3 prop type
        //----------------------------------------------------------------------------------------
        template<> Widget::PropertyType Widget::GetType<Core::Vector3>() const;
        //----------------------------------------------------------------------------------------
        /// Specialisation to return property type for string
        ///
        /// @author S Downie
        ///
        /// @return Vec3 prop type
        //----------------------------------------------------------------------------------------
        template<> Widget::PropertyType Widget::GetType<const Core::Vector3&>() const;
    }
}

#endif