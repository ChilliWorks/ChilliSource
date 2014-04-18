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
            
        private:
            
            std::string m_name;
            
            std::unordered_map<std::string, PropertyLookup> m_blobOffsets;
            u8* m_propertyBlob = nullptr;
            
            IDrawableUPtr m_drawable;
            ILayoutUPtr m_layout;
            
            Widget* m_parent = nullptr;
            
            Core::AlignmentAnchor m_originAlignment;
            Core::AlignmentAnchor m_alignmentToParent;
            
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