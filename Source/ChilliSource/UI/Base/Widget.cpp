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
        void Widget::Draw(Rendering::CanvasRenderer* in_renderer)
        {
            if(m_isLayoutValid == false && m_layout != nullptr)
            {
                //m_layout->Layout();
                m_isLayoutValid = true;
            }
            
            if(m_drawable != nullptr)
            {
                //m_drawable->Draw(in_renderer, , , );
            }
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
