//
//  Widget.cpp
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

#include <ChilliSource/UI/Base/Widget.h>

namespace ChilliSource
{
    namespace UI
    {
        namespace
        {
            //----------------------------------------------------------------------------------------
            /// Convert the string type to internal property type
            ///
            /// @author S Downie
            ///
            /// @param Type string
            ///
            /// @return Property type
            //----------------------------------------------------------------------------------------
            Widget::PropertyType GetTypeFromString(const std::string& in_type)
            {
                if(in_type == "Int") return Widget::PropertyType::k_int;
                if(in_type == "Bool") return Widget::PropertyType::k_bool;
                if(in_type == "Float") return Widget::PropertyType::k_float;
                if(in_type == "String") return Widget::PropertyType::k_string;
                if(in_type == "Pointer") return Widget::PropertyType::k_pointer;
                
                return Widget::PropertyType::k_unknown;
            }
            //----------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Type
            ///
            /// @return Size of type in bytes
            //----------------------------------------------------------------------------------------
            u32 GetTypeSize(Widget::PropertyType in_type)
            {
                switch(in_type)
                {
                    case Widget::PropertyType::k_bool:
                        return sizeof(bool);
                    case Widget::PropertyType::k_int:
                        return sizeof(s32);
                    case Widget::PropertyType::k_float:
                        return sizeof(f32);
                    case Widget::PropertyType::k_string:
                        return sizeof(std::string);
                    case Widget::PropertyType::k_pointer:
                        return sizeof(u8*);
                    case Widget::PropertyType::k_unknown:
                        return 0;
                }
                
                return 0;
            }
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void Widget::CreateProperties(const std::vector<PropertyDesc>& in_descs)
        {
            CS_ASSERT(m_propertyBlob == nullptr, "Cannot create widget properties more than once");
            
            u32 currentOffset = 0;
            for(const auto& desc : in_descs)
            {
                PropertyType type = GetTypeFromString(desc.m_type);
                CS_ASSERT(type != PropertyType::k_unknown, "Unsupported property type: " + desc.m_type);
                
                PropertyLookup lookup = {type, currentOffset};
                m_blobOffsets.insert(std::make_pair(std::move(desc.m_name), std::move(lookup)));
                
                u32 typeSize = GetTypeSize(type);
                currentOffset += typeSize;
            }
            
            //Current offset now also holds the required capacity of the blob
            if(currentOffset > 0)
            {
                m_propertyBlob = new u8[currentOffset];
            }
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void Widget::SetDrawable(IDrawableUPtr in_drawable)
        {
            m_drawable = std::move(in_drawable);
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void Widget::SetLayout(ILayoutUPtr in_layout)
        {
            m_layout = std::move(in_layout);
            m_isLayoutValid = false;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void Widget::SetName(const std::string& in_name)
        {
            m_name = in_name;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        const std::string& Widget::GetName() const
        {
            return m_name;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void Widget::SetRelativeSize(f32 in_width, f32 in_height)
        {
            m_localSize.vRelative.x = in_width;
            m_localSize.vRelative.y = in_height;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void Widget::SetRelativeSize(const Core::Vector2& in_size)
        {
            m_localSize.vRelative = in_size;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void Widget::SetAbsoluteSize(f32 in_width, f32 in_height)
        {
            m_localSize.vAbsolute.x = in_width;
            m_localSize.vAbsolute.y = in_height;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void Widget::SetAbsoluteSize(const Core::Vector2& in_size)
        {
            m_localSize.vAbsolute = in_size;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void Widget::SetRelativePosition(f32 in_x, f32 in_y)
        {
            m_localPosition.vRelative.x = in_x;
            m_localPosition.vRelative.y = in_y;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void Widget::SetRelativePosition(const Core::Vector2& in_pos)
        {
            m_localPosition.vRelative = in_pos;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void Widget::SetAbsolutePosition(f32 in_x, f32 in_y)
        {
            m_localPosition.vAbsolute.x = in_x;
            m_localPosition.vAbsolute.y = in_y;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void Widget::SetAbsolutePosition(const Core::Vector2& in_pos)
        {
            m_localPosition.vAbsolute = in_pos;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void Widget::Add(const WidgetSPtr& in_widget)
        {
            CS_ASSERT(in_widget->GetParent() == nullptr, "Cannot add a widget as a child of more than 1 parent");
            //TODO: Ensure that the vector is not invalidated during iteration
            m_children.push_back(in_widget);
            in_widget->m_parent = this;
            in_widget->m_canvas = m_canvas;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        Widget* Widget::GetParent()
        {
            return m_parent;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        const Widget* Widget::GetParent() const
        {
            return m_parent;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void Widget::SetCanvas(const Widget* in_canvas)
        {
            m_canvas = in_canvas;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void Widget::Draw(Rendering::CanvasRenderer* in_renderer)
        {
            //TODO: Visibility check
            if(m_isLayoutValid == false && m_layout != nullptr)
            {
                //m_layout->Layout();
                m_isLayoutValid = true;
            }
            
            if(m_drawable != nullptr)
            {
                m_drawable->Draw(in_renderer, GetFinalTransform(), GetFinalColour());
            }
            
            //TODO: Draw subview
            for(auto& child : m_children)
            {
                child->Draw(in_renderer);
            }
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        Core::Matrix3x3 Widget::GetFinalTransform() const
        {
            //TODO: Do we add caching here?
            Core::Matrix3x3 localTransform(GetParentSpacePosition(), Core::Vector2::ONE, m_localRotation);
            
            if(m_parent != nullptr)
            {
                Core::Matrix3x3 parentTransform(std::move(m_parent->GetFinalTransform()));

                Core::Matrix3x3 finalTransform;
                Core::Matrix3x3::Multiply(&localTransform, &parentTransform, &finalTransform);
                
                return finalTransform;
            }
            
            return localTransform;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        Core::Vector2 Widget::GetFinalPosition() const
        {
			Core::Matrix3x3 finalTransform(GetFinalTransform());
            return Core::Vector2(finalTransform.m[6], finalTransform.m[7]);
        }
        //----------------------------------------------------------------------------------------
        /// The position of the widget is calculated based on the local absolute and
        /// relative positions as well as the local alignment anchors. The local relative
        /// position is relative to the final parent position and cannot be calculated until
        /// there is an absolute reference point in the widget hierarchy.
        //----------------------------------------------------------------------------------------
        Core::Vector2 Widget::GetParentSpacePosition() const
        {
            CS_ASSERT(m_canvas != nullptr, "Cannot get the absolute position of widget without canvas");
            CS_ASSERT(m_parent != nullptr, "Cannot calculate widget parent space pos without parent");
            
            //Get the anchor point to which the widget is aligned in parent space
            const Core::Vector2 parentSize(m_parent->GetFinalSize());
			const Core::Vector2 parentHalfSize(parentSize * 0.5f);
			Core::Vector2 parentAnchorPos;
			Rendering::GetAnchorPoint(m_alignmentToParent, parentHalfSize, parentAnchorPos);
            
            //Calculate the position relative to the anchor point
            Core::Vector2 parentSpacePos = parentAnchorPos + (parentSize * m_localPosition.vRelative) + m_localPosition.vAbsolute;
            
            //Offset the position by the alignment anchor of the origin
            Core::Vector2 alignmentOffset;
            Rendering::Align(m_originAlignment, GetFinalSize() * 0.5f, alignmentOffset);
            
            return parentSpacePos + alignmentOffset;
        }
        //----------------------------------------------------------------------------------------
        /// The final size of the widget is calculated based on the local absolute and the
        /// local relative size. The local relative size is relative to the final parent size and
        /// cannot be calculated until there is an absolute reference point in the widget
        /// hierarchy
        //----------------------------------------------------------------------------------------
        Core::Vector2 Widget::GetFinalSize() const
        {
            CS_ASSERT(m_canvas != nullptr, "Cannot get the absolute position of widget without canvas");
            
            if(m_parent != nullptr)
            {
                return ((m_parent->GetFinalSize() * m_localSize.vRelative) + m_localSize.vAbsolute) * GetFinalScale();
            }
            
            return m_localSize.vAbsolute * GetFinalScale();
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        f32 Widget::GetFinalRotation() const
        {
            //TODO: Find out why this is minused in the old GUI system
            if(m_parent != nullptr)
            {
                return m_localRotation + m_parent->GetFinalRotation();
            }
            
            return m_localRotation;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        Core::Vector2 Widget::GetFinalScale() const
        {
            if(m_parent != nullptr)
            {
                return m_localScale * m_parent->GetFinalScale();
            }
            
            return m_localScale;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        Core::Colour Widget::GetFinalColour() const
        {
            //TODO: Do we implement the inherit colour option?
            if(m_parent != nullptr)
            {
                return m_localColour * m_parent->GetFinalColour();
            }
            
            return m_localColour;
        }
        //----------------------------------------------------------------------------------------
        /// Set the value of the string property with the given name. If no property exists
        /// with the name then it will assert. This is a specialisation to handle inserting
        /// strings into the blob.
        ///
        /// @author S Downie
        ///
        /// @param Name
        /// @param Value
        //----------------------------------------------------------------------------------------
        template<> void Widget::SetProperty(const std::string& in_name, std::string in_value)
        {
            auto entry = m_blobOffsets.find(in_name);
            CS_ASSERT(entry != m_blobOffsets.end(), "No UI property with name: " + in_name);
            CS_ASSERT(entry->second.m_type == PropertyType::k_string, "Wrong type for property with name " + in_name);
            
            std::string* property = (std::string*)(m_propertyBlob + entry->second.m_offset);
            *property = in_value;
        }
        //----------------------------------------------------------------------------------------
        /// Set the value of the string property with the given name. If no property exists
        /// with the name then it will assert. This is a specialisation to handle inserting
        /// strings into the blob.
        ///
        /// @author S Downie
        ///
        /// @param Name
        /// @param Value
        //----------------------------------------------------------------------------------------
        template<> void Widget::SetProperty(const std::string& in_name, const std::string& in_value)
        {
            SetProperty<std::string>(in_name, in_value);
        }
        //----------------------------------------------------------------------------------------
        /// Set the value of the string property with the given name. If no property exists
        /// with the name then it will assert. This is a specialisation to handle inserting
        /// strings into the blob.
        ///
        /// @author S Downie
        ///
        /// @param Name
        /// @param Value
        //----------------------------------------------------------------------------------------
        template<> void Widget::SetProperty(const std::string& in_name, const char* in_value)
        {
            SetProperty<std::string>(in_name, in_value);
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        template<> Widget::PropertyType Widget::GetType<bool>() const
        {
            return PropertyType::k_bool;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        template<> Widget::PropertyType Widget::GetType<s32>() const
        {
            return PropertyType::k_int;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        template<> Widget::PropertyType Widget::GetType<std::string>() const
        {
            return PropertyType::k_string;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        template<> Widget::PropertyType Widget::GetType<const std::string&>() const
        {
            return PropertyType::k_string;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        template<> Widget::PropertyType Widget::GetType<const char*>() const
        {
            return PropertyType::k_string;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        template<> Widget::PropertyType Widget::GetType<f32>() const
        {
            return PropertyType::k_float;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        Widget::~Widget()
        {
            CS_SAFEDELETE_ARRAY(m_propertyBlob);
        }
    }
}
