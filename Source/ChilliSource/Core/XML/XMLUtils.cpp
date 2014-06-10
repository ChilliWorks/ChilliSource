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

namespace ChilliSource
{
    namespace Core
    {
        namespace XMLUtils
        {
            //--------------------------------------------------
            //--------------------------------------------------
            DocumentUPtr ReadDocument(StorageLocation in_storageLocation, const std::string& in_filePath)
            {
                Core::FileStreamSPtr stream = Application::Get()->GetFileSystem()->CreateFileStream(in_storageLocation, in_filePath, Core::FileMode::k_read);
                if (stream != nullptr && stream->IsOpen() == true && stream->IsBad() == false)
                {
                    std::string contents;
                    stream->GetAll(contents);
                    
                    //Load the script
                    DocumentUPtr document(new rapidxml::xml_document<>());
                    document->parse<rapidxml::parse_default>(const_cast<s8*>(contents.c_str()));
                    
                    return document;
                }
                
                return DocumentUPtr();
            }
            //--------------------------------------------------
            //--------------------------------------------------
            bool HasName(const Base* in_base, const std::string& in_name)
            {
                return (std::string(in_base->name()) == in_name);
            }
            //--------------------------------------------------
            //--------------------------------------------------
            std::string GetAttribute(const Node* in_node, const std::string& in_attributeName, const std::string& in_defaultValue)
            {
                for(rapidxml::xml_attribute<> * attribute = in_node->first_attribute(); attribute != nullptr; attribute = attribute->next_attribute())
                {
                    if (HasName(attribute, in_attributeName) == true)
                    {
                        return attribute->value();
                    }
                }
                
                return in_defaultValue;
            }
            //--------------------------------------------------
            //--------------------------------------------------
            bool GetAttribute(const Node* in_node, const std::string& in_attributeName, bool in_defaultValue)
            {
                for(rapidxml::xml_attribute<> * attribute = in_node->first_attribute(); attribute != nullptr; attribute = attribute->next_attribute())
                {
                    if (HasName(attribute, in_attributeName) == true)
                    {
                        return ParseBool(attribute->value());
                    }
                }
                
                return in_defaultValue;
            }
            //--------------------------------------------------
            //--------------------------------------------------
            f32 GetAttribute(const Node* in_node, const std::string& in_attributeName, f32 in_defaultValue)
            {
                for(rapidxml::xml_attribute<> * attribute = in_node->first_attribute(); attribute != nullptr; attribute = attribute->next_attribute())
                {
                    if (HasName(attribute, in_attributeName) == true)
                    {
                        return ParseF32(attribute->value());
                    }
                }
                
                return in_defaultValue;
            }
            //--------------------------------------------------
            //--------------------------------------------------
            s32 GetAttribute(const Node* in_node, const std::string& in_attributeName, s32 in_defaultValue)
            {
                for(rapidxml::xml_attribute<> * attribute = in_node->first_attribute(); attribute != nullptr; attribute = attribute->next_attribute())
                {
                    if (HasName(attribute, in_attributeName) == true)
                    {
                        return ParseS32(attribute->value());
                    }
                }
                
                return in_defaultValue;
            }
            //--------------------------------------------------
            //--------------------------------------------------
            s64 GetAttribute(const Node* in_node, const std::string& in_attributeName, s64 in_defaultValue)
            {
                for(rapidxml::xml_attribute<> * attribute = in_node->first_attribute(); attribute != nullptr; attribute = attribute->next_attribute())
                {
                    if (HasName(attribute, in_attributeName) == true)
                    {
                        return ParseS64(attribute->value());
                    }
                }
                
                return in_defaultValue;
            }
            //--------------------------------------------------
            //--------------------------------------------------
            Vector2 GetAttribute(const Node* in_node, const std::string& in_attributeName, const Vector2& in_defaultValue)
            {
                for(rapidxml::xml_attribute<> * attribute = in_node->first_attribute(); attribute != nullptr; attribute = attribute->next_attribute())
                {
                    if (HasName(attribute, in_attributeName) == true)
                    {
                        return ParseVector2(attribute->value());
                    }
                }
                
                return in_defaultValue;
            }
            //--------------------------------------------------
            //--------------------------------------------------
            Vector3 GetAttribute(const Node* in_node, const std::string& in_attributeName, const Vector3& in_defaultValue)
            {
                for(rapidxml::xml_attribute<> * attribute = in_node->first_attribute(); attribute != nullptr; attribute = attribute->next_attribute())
                {
                    if (HasName(attribute, in_attributeName) == true)
                    {
                        return ParseVector3(attribute->value());
                    }
                }
                
                return in_defaultValue;
            }
            //--------------------------------------------------
            //--------------------------------------------------
            Vector4 GetAttribute(const Node* in_node, const std::string& in_attributeName, const Vector4& in_defaultValue)
            {
                for(rapidxml::xml_attribute<> * attribute = in_node->first_attribute(); attribute != nullptr; attribute = attribute->next_attribute())
                {
                    if (HasName(attribute, in_attributeName) == true)
                    {
                        return ParseVector4(attribute->value());
                    }
                }
                
                return in_defaultValue;
            }
            //--------------------------------------------------
            //--------------------------------------------------
            Matrix3 GetAttribute(const Node* in_node, const std::string& in_attributeName, const Matrix3& in_defaultValue)
            {
                for(rapidxml::xml_attribute<> * attribute = in_node->first_attribute(); attribute != nullptr; attribute = attribute->next_attribute())
                {
                    if (HasName(attribute, in_attributeName) == true)
                    {
                        return ParseMatrix3(attribute->value());
                    }
                }
                
                return in_defaultValue;
            }
            //--------------------------------------------------
            //--------------------------------------------------
            Matrix4 GetAttribute(const Node* in_node, const std::string& in_attributeName, const Matrix4& in_defaultValue)
            {
                for(rapidxml::xml_attribute<> * attribute = in_node->first_attribute(); attribute != nullptr; attribute = attribute->next_attribute())
                {
                    if (HasName(attribute, in_attributeName) == true)
                    {
                        return ParseMatrix4(attribute->value());
                    }
                }
                
                return in_defaultValue;
            }
            //--------------------------------------------------
            //--------------------------------------------------
            Quaternion GetAttribute(const Node* in_node, const std::string& in_attributeName, const Quaternion& in_defaultValue)
            {
                for(rapidxml::xml_attribute<> * attribute = in_node->first_attribute(); attribute != nullptr; attribute = attribute->next_attribute())
                {
                    if (HasName(attribute, in_attributeName) == true)
                    {
                        return ParseQuaternion(attribute->value());
                    }
                }
                
                return in_defaultValue;
            }
            //--------------------------------------------------
            //--------------------------------------------------
            Colour GetAttribute(const Node* in_node, const std::string& in_attributeName, const Colour& in_defaultValue)
            {
                for(rapidxml::xml_attribute<> * attribute = in_node->first_attribute(); attribute != nullptr; attribute = attribute->next_attribute())
                {
                    if (HasName(attribute, in_attributeName) == true)
                    {
                        return ParseColour(attribute->value());
                    }
                }
                
                return in_defaultValue;
            }
            //--------------------------------------------------
            //--------------------------------------------------
            std::string ToString(const Node* in_base)
            {
                std::stringstream stream;
                stream << in_base;
                return stream.str();
            }
            //--------------------------------------------------
            //--------------------------------------------------
            bool WriteDocument(StorageLocation in_storageLocation, const std::string& in_filePath, Document* in_document)
            {
                //TODO:
                return false;
            }
        }
    }
}
