//
//  XMLUtils.cpp
//  Chilli Source
//  Created by Ian Copland on 10/06/2014
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

#include <ChilliSource/Core/XML/XMLUtils.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/File/FileSystem.h>

#include <rapidxml/rapidxml_print.hpp>

#include <cstring>

namespace ChilliSource
{
    namespace Core
    {
        namespace XMLUtils
        {
            //--------------------------------------------------
            //--------------------------------------------------
            XMLUPtr ReadDocument(StorageLocation in_storageLocation, const std::string& in_filePath)
            {
                Core::FileStreamSPtr stream = Application::Get()->GetFileSystem()->CreateFileStream(in_storageLocation, in_filePath, Core::FileMode::k_read);
                if (stream != nullptr && stream->IsOpen() == true && stream->IsBad() == false)
                {
                    std::string contents;
                    stream->GetAll(contents);
                    
                    XMLUPtr document = ParseDocument(contents);
                    return document;
                }
                
                return XMLUPtr();
            }
            //--------------------------------------------------
            //--------------------------------------------------
            XMLUPtr ParseDocument(const std::string& in_xmlString)
            {
                std::unique_ptr<s8[]> data(new s8[in_xmlString.length() + 1]);
                memcpy(data.get(), in_xmlString.c_str(), in_xmlString.length());
                data[in_xmlString.length()] = '\0';
                
                std::unique_ptr<XML::Document> document(new XML::Document());
                document->parse<rapidxml::parse_default>(data.get());
                return XMLUPtr(new XML(std::move(document), std::move(data)));
            }
            //--------------------------------------------------
            //--------------------------------------------------
            std::string GetName(const XML::Base* in_base)
            {
                return std::string(in_base->name(), in_base->name_size());
            }
            //--------------------------------------------------
            //--------------------------------------------------
            std::string GetValue(const XML::Base* in_base)
            {
                return std::string(in_base->value(), in_base->value_size());
            }
            //--------------------------------------------------
            //--------------------------------------------------
            XML::Node* GetFirstChildNode(const XML::Node* in_node, const std::string& in_name)
            {
                const s8* nameData = nullptr;
                u32 nameSize = in_name.length();
                if (nameSize > 0)
                {
                    nameData = in_name.c_str();
                }
                
                return in_node->first_node(nameData, nameSize);
            }
            //--------------------------------------------------
            //--------------------------------------------------
            XML::Node* GetNextSiblingNode(const XML::Node* in_node, const std::string& in_name)
            {
                const s8* nameData = nullptr;
                u32 nameSize = in_name.length();
                if (nameSize > 0)
                {
                    nameData = in_name.c_str();
                }
                
                return in_node->next_sibling(nameData, nameSize);
            }
            //--------------------------------------------------
            //--------------------------------------------------
            XML::Node* GetFirstChildElement(const XML::Node* in_node, const std::string& in_name)
            {
                const s8* nameData = nullptr;
                u32 nameSize = in_name.length();
                if (nameSize > 0)
                {
                    nameData = in_name.c_str();
                }
                
                XML::Node* child = in_node->first_node(nameData, nameSize);
                if (child != nullptr && child->type() == rapidxml::node_type::node_element)
                {
                    return child;
                }
                else if (child != nullptr)
                {
                    XML::Node* sibling = child;
                    do
                    {
                        sibling = sibling->next_sibling(nameData, nameSize);
                    }
                    while(sibling != nullptr && sibling->type() != rapidxml::node_type::node_element);
                    return sibling;
                }
                
                return nullptr;
            }
            //--------------------------------------------------
            //--------------------------------------------------
            XML::Node* GetNextSiblingElement(const XML::Node* in_node, const std::string& in_name)
            {
                const s8* nameData = nullptr;
                u32 nameSize = in_name.length();
                if (nameSize > 0)
                {
                    nameData = in_name.c_str();
                }
                
                XML::Node* sibling = in_node->next_sibling(nameData, nameSize);
                while (sibling != nullptr && sibling->type() != rapidxml::node_type::node_element)
                {
                    sibling = sibling->next_sibling(nameData, nameSize);
                }
                return sibling;
            }
            //--------------------------------------------------
            //--------------------------------------------------
            XML::Attribute* GetFirstAttribute(const XML::Node* in_node)
            {
                return in_node->first_attribute();
            }
            //--------------------------------------------------
            //--------------------------------------------------
            XML::Attribute* GetNextAttribute(const XML::Attribute* in_attribute)
            {
                return in_attribute->next_attribute();
            }
            //--------------------------------------------------
            //--------------------------------------------------
            template <> std::string GetAttributeValue<std::string>(const XML::Node* in_node, const std::string& in_attributeName, const std::string& in_defaultValue)
            {
                for(rapidxml::xml_attribute<>* attribute = in_node->first_attribute(); attribute != nullptr; attribute = attribute->next_attribute())
                {
                    if (GetName(attribute) == in_attributeName)
                    {
                        return GetValue(attribute);
                    }
                }
                
                return in_defaultValue;
            }
            //--------------------------------------------------
            //--------------------------------------------------
            template <> bool GetAttributeValue<bool>(const XML::Node* in_node, const std::string& in_attributeName, const bool& in_defaultValue)
            {
                for(rapidxml::xml_attribute<>* attribute = in_node->first_attribute(); attribute != nullptr; attribute = attribute->next_attribute())
                {
                    if (GetName(attribute) == in_attributeName)
                    {
                        return ParseBool(GetValue(attribute));
                    }
                }
                
                return in_defaultValue;
            }
            //--------------------------------------------------
            //--------------------------------------------------
            template <> f32 GetAttributeValue<f32>(const XML::Node* in_node, const std::string& in_attributeName, const f32& in_defaultValue)
            {
                for(rapidxml::xml_attribute<>* attribute = in_node->first_attribute(); attribute != nullptr; attribute = attribute->next_attribute())
                {
                    if (GetName(attribute) == in_attributeName)
                    {
                        return ParseF32(GetValue(attribute));
                    }
                }
                
                return in_defaultValue;
            }
            //--------------------------------------------------
            //--------------------------------------------------
            template <> s32 GetAttributeValue<s32>(const XML::Node* in_node, const std::string& in_attributeName, const s32& in_defaultValue)
            {
                for(rapidxml::xml_attribute<>* attribute = in_node->first_attribute(); attribute != nullptr; attribute = attribute->next_attribute())
                {
                    if (GetName(attribute) == in_attributeName)
                    {
                        return ParseS32(GetValue(attribute));
                    }
                }
                
                return in_defaultValue;
            }
            //--------------------------------------------------
            //--------------------------------------------------
            template <> s64 GetAttributeValue<s64>(const XML::Node* in_node, const std::string& in_attributeName, const s64& in_defaultValue)
            {
                for(rapidxml::xml_attribute<>* attribute = in_node->first_attribute(); attribute != nullptr; attribute = attribute->next_attribute())
                {
                    if (GetName(attribute) == in_attributeName)
                    {
                        return ParseS64(GetValue(attribute));
                    }
                }
                
                return in_defaultValue;
            }
            //--------------------------------------------------
            //--------------------------------------------------
            template <> u32 GetAttributeValue<u32>(const XML::Node* in_node, const std::string& in_attributeName, const u32& in_defaultValue)
            {
                for(rapidxml::xml_attribute<>* attribute = in_node->first_attribute(); attribute != nullptr; attribute = attribute->next_attribute())
                {
                    if (GetName(attribute) == in_attributeName)
                    {
                        return ParseU32(GetValue(attribute));
                    }
                }
                
                return in_defaultValue;
            }
            //--------------------------------------------------
            //--------------------------------------------------
            template <> u64 GetAttributeValue<u64>(const XML::Node* in_node, const std::string& in_attributeName, const u64& in_defaultValue)
            {
                for(rapidxml::xml_attribute<>* attribute = in_node->first_attribute(); attribute != nullptr; attribute = attribute->next_attribute())
                {
                    if (GetName(attribute) == in_attributeName)
                    {
                        return ParseU64(GetValue(attribute));
                    }
                }
                
                return in_defaultValue;
            }
            //--------------------------------------------------
            //--------------------------------------------------
            template <> Vector2 GetAttributeValue<Vector2>(const XML::Node* in_node, const std::string& in_attributeName, const Vector2& in_defaultValue)
            {
                for(rapidxml::xml_attribute<>* attribute = in_node->first_attribute(); attribute != nullptr; attribute = attribute->next_attribute())
                {
                    if (GetName(attribute) == in_attributeName)
                    {
                        return ParseVector2(GetValue(attribute));
                    }
                }
                
                return in_defaultValue;
            }
            //--------------------------------------------------
            //--------------------------------------------------
            template <> Vector3 GetAttributeValue<Vector3>(const XML::Node* in_node, const std::string& in_attributeName, const Vector3& in_defaultValue)
            {
                for(rapidxml::xml_attribute<>* attribute = in_node->first_attribute(); attribute != nullptr; attribute = attribute->next_attribute())
                {
                    if (GetName(attribute) == in_attributeName)
                    {
                        return ParseVector3(GetValue(attribute));
                    }
                }
                
                return in_defaultValue;
            }
            //--------------------------------------------------
            //--------------------------------------------------
            template <> Vector4 GetAttributeValue<Vector4>(const XML::Node* in_node, const std::string& in_attributeName, const Vector4& in_defaultValue)
            {
                for(rapidxml::xml_attribute<>* attribute = in_node->first_attribute(); attribute != nullptr; attribute = attribute->next_attribute())
                {
                    if (GetName(attribute) == in_attributeName)
                    {
                        return ParseVector4(GetValue(attribute));
                    }
                }
                
                return in_defaultValue;
            }
            //--------------------------------------------------
            //--------------------------------------------------
            template <> Matrix3 GetAttributeValue<Matrix3>(const XML::Node* in_node, const std::string& in_attributeName, const Matrix3& in_defaultValue)
            {
                for(rapidxml::xml_attribute<>* attribute = in_node->first_attribute(); attribute != nullptr; attribute = attribute->next_attribute())
                {
                    if (GetName(attribute) == in_attributeName)
                    {
                        return ParseMatrix3(GetValue(attribute));
                    }
                }
                
                return in_defaultValue;
            }
            //--------------------------------------------------
            //--------------------------------------------------
            template <> Matrix4 GetAttributeValue<Matrix4>(const XML::Node* in_node, const std::string& in_attributeName, const Matrix4& in_defaultValue)
            {
                for(rapidxml::xml_attribute<>* attribute = in_node->first_attribute(); attribute != nullptr; attribute = attribute->next_attribute())
                {
                    if (GetName(attribute) == in_attributeName)
                    {
                        return ParseMatrix4(GetValue(attribute));
                    }
                }
                
                return in_defaultValue;
            }
            //--------------------------------------------------
            //--------------------------------------------------
            template <> Quaternion GetAttributeValue<Quaternion>(const XML::Node* in_node, const std::string& in_attributeName, const Quaternion& in_defaultValue)
            {
                for(rapidxml::xml_attribute<>* attribute = in_node->first_attribute(); attribute != nullptr; attribute = attribute->next_attribute())
                {
                    if (GetName(attribute) == in_attributeName)
                    {
                        return ParseQuaternion(GetValue(attribute));
                    }
                }
                
                return in_defaultValue;
            }
            //--------------------------------------------------
            //--------------------------------------------------
            template <> Colour GetAttributeValue<Colour>(const XML::Node* in_node, const std::string& in_attributeName, const Colour& in_defaultValue)
            {
                for(rapidxml::xml_attribute<>* attribute = in_node->first_attribute(); attribute != nullptr; attribute = attribute->next_attribute())
                {
                    if (GetName(attribute) == in_attributeName)
                    {
                        return ParseColour(GetValue(attribute));
                    }
                }
                
                return in_defaultValue;
            }
            //--------------------------------------------------
            //--------------------------------------------------
            std::string ToString(const XML::Node* in_base)
            {
                std::ostringstream stream;
                stream << *in_base;
                return stream.str();
            }
            //--------------------------------------------------
            //--------------------------------------------------
            bool WriteDocument(XML::Document* in_document, StorageLocation in_storageLocation, const std::string& in_filePath)
            {
                Core::FileStreamSPtr stream = Application::Get()->GetFileSystem()->CreateFileStream(in_storageLocation, in_filePath, Core::FileMode::k_write);
                if (stream != nullptr && stream->IsOpen() == true && stream->IsBad() == false)
                {
                    std::string contents = XMLUtils::ToString(in_document);
                    stream->Write(contents);
                    stream->Close();
                    return true;
                }
                
                return false;
            }
        }
    }
}
