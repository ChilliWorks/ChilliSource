//
//  XMLUtils.h
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

#ifndef _CHILLISOURCE_CORE_XML_XMLUTILS_H_
#define _CHILLISOURCE_CORE_XML_XMLUTILS_H_

#include <ChilliSource/ChilliSource.h>

#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/Core/XML/XML.h>

#include <type_traits>

namespace ChilliSource
{
    namespace Core
    {
        //--------------------------------------------------------
        /// A collection of functions for working with xml
        /// in a Chilli Source project. This provides convenience
        /// functions as well as functions for working within the
        /// Chilli Source file system.
        ///
        /// @author Ian Copland
        //--------------------------------------------------------
        namespace XMLUtils
        {
            //--------------------------------------------------
            /// Reads an xml document from file.
            ///
            /// @author Ian Copland
            ///
            /// @param The storage location of the file.
            /// @param The file path.
            ///
            /// @return the new document stored in an xml container.
            //--------------------------------------------------
            XMLUPtr ReadDocument(StorageLocation in_storageLocation, const std::string& in_filePath);
            //--------------------------------------------------
            /// Parses an xml document from string.
            ///
            /// @author Ian Copland
            ///
            /// @param The xml string.
            ///
            /// @return the new document stored in an xml container.
            //--------------------------------------------------
            XMLUPtr ParseDocument(const std::string& in_xmlString);
            //--------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param The node.
            ///
            /// @return The name of the given node.
            //--------------------------------------------------
            std::string GetName(const XML::Base* in_base);
            //--------------------------------------------------
            /// Returns the value of the given node. What the
            /// value is is determined by the type of node.
            ///
            /// @author Ian Copland
            ///
            /// @param The node.
            ///
            /// @return The value as a string.
            //--------------------------------------------------
            std::string GetValue(const XML::Base* in_base);
            //--------------------------------------------------
            /// Returns the first child with the given name. If
            /// the name is empty, the first child regardless
            /// of name will be returned. If there is no child with
            /// a matching name, null will be returned.
            ///
            /// @author Ian Copland
            ///
            /// @param The node.
            /// @param [optional] The name.
            ///
            /// @return The child node or null.
            //--------------------------------------------------
            XML::Node* GetFirstChildNode(const XML::Node* in_node, const std::string& in_name = "");
            //--------------------------------------------------
            /// Returns the next sibling with the given name. If
            /// the name is empty the next sibling regardless of
            /// name will be returned. If there is no child with
            /// a matching name, null will be returned.
            ///
            /// @author Ian Copland
            ///
            /// @param The node.
            /// @param [Optional] The name.
            ///
            /// @return The sibling node or null.
            //--------------------------------------------------
            XML::Node* GetNextSiblingNode(const XML::Node* in_node, const std::string& in_name = "");
            //--------------------------------------------------
            /// Returns the first child with the given name which
            /// is of element type. If the name is empty, the
            /// first child regardless of name will be returned.
            /// If there is no child with a matching name, null
            /// will be returned.
            ///
            /// @author Ian Copland
            ///
            /// @param The node.
            /// @param [optional] The name.
            ///
            /// @return The child node or null.
            //--------------------------------------------------
            XML::Node* GetFirstChildElement(const XML::Node* in_node, const std::string& in_name = "");
            //--------------------------------------------------
            /// Returns the next sibling with the given name which
            /// is of element type. If the name is empty the next
            /// sibling regardless of name will be returned. If
            /// there is no child with a matching name, null will
            /// be returned.
            ///
            /// @author Ian Copland
            ///
            /// @param The node.
            /// @param [Optional] The name.
            ///
            /// @return The sibling node or null.
            //--------------------------------------------------
            XML::Node* GetNextSiblingElement(const XML::Node* in_node, const std::string& in_name = "");
            //--------------------------------------------------
            /// Returns the first attribute in the node. Will
            /// return null if there is no attributes.
            ///
            /// @author Ian Copland
            ///
            /// @param The node.
            ///
            /// @return The first attribute in the node or null.
            //--------------------------------------------------
            XML::Attribute* GetFirstAttribute(const XML::Node* in_node);
            //--------------------------------------------------
            /// Returns the next attribute in the parent node
            /// if there is one, otherwise returns null.
            ///
            /// @author Ian Copland
            ///
            /// @param The current attribute.
            ///
            /// @return The next attribute or null.
            //--------------------------------------------------
            XML::Attribute* GetNextAttribute(const XML::Attribute* in_attribute);
            //--------------------------------------------------
            /// Returns the value for the requested attribute name.
            /// If the attribute doesn't exist the default value
            /// will be returned instead.
            ///
            /// @author Ian Copland
            ///
            /// @param The node.
            /// @param The attribute name.
            /// @param The default value.
            ///
            /// @return The attribute value as a string.
            //--------------------------------------------------
            template <typename TType> TType GetAttributeValue(const XML::Node* in_node, const std::string& in_attributeName, const TType& in_defaultValue)
            {
                static_assert(std::is_void<TType>() && std::is_void<TType>() == false, "No implementation for the given template type.");
            }
            //--------------------------------------------------
            /// Returns the value for the requested attribute name.
            /// If the attribute doesn't exist the default value
            /// will be returned instead.
            ///
            /// @author Ian Copland
            ///
            /// @param The node.
            /// @param The attribute name.
            /// @param The default value.
            ///
            /// @return The attribute value as a string.
            //--------------------------------------------------
            template <> std::string GetAttributeValue<std::string>(const XML::Node* in_node, const std::string& in_attributeName, const std::string& in_defaultValue);
            //--------------------------------------------------
            /// Returns the value for the requested attribute name.
            /// If the attribute doesn't exist the default value
            /// will be returned instead.
            ///
            /// @author Ian Copland
            ///
            /// @param The node.
            /// @param The attribute name.
            /// @param The default value.
            ///
            /// @return The attribute value as a boolean.
            //--------------------------------------------------
            template <> bool GetAttributeValue<bool>(const XML::Node* in_node, const std::string& in_attributeName, const bool& in_defaultValue);
            //--------------------------------------------------
            /// Returns the value for the requested attribute name.
            /// If the attribute doesn't exist the default value
            /// will be returned instead.
            ///
            /// @author Ian Copland
            ///
            /// @param The node.
            /// @param The attribute name.
            /// @param The default value.
            ///
            /// @return The attribute value as a float.
            //--------------------------------------------------
            template <> f32 GetAttributeValue<f32>(const XML::Node* in_node, const std::string& in_attributeName, const f32& in_defaultValue);
            //--------------------------------------------------
            /// Returns the value for the requested attribute name.
            /// If the attribute doesn't exist the default value
            /// will be returned instead.
            ///
            /// @author Ian Copland
            ///
            /// @param The node.
            /// @param The attribute name.
            /// @param The default value.
            ///
            /// @return The attribute value as a signed 32-bit
            /// integer.
            //--------------------------------------------------
            template <> s32 GetAttributeValue<s32>(const XML::Node* in_node, const std::string& in_attributeName, const s32& in_defaultValue);
            //--------------------------------------------------
            /// Returns the value for the requested attribute name.
            /// If the attribute doesn't exist the default value
            /// will be returned instead.
            ///
            /// @author Ian Copland
            ///
            /// @param The node.
            /// @param The attribute name.
            /// @param The default value.
            ///
            /// @return The attribute value as a signed 64-bit
            /// integer.
            //--------------------------------------------------
            template <> s64 GetAttributeValue<s64>(const XML::Node* in_node, const std::string& in_attributeName, const s64& in_defaultValue);
            //--------------------------------------------------
            /// Returns the value for the requested attribute name.
            /// If the attribute doesn't exist the default value
            /// will be returned instead.
            ///
            /// @author Ian Copland
            ///
            /// @param The node.
            /// @param The attribute name.
            /// @param The default value.
            ///
            /// @return The attribute value as an unsigned 32-bit
            /// integer.
            //--------------------------------------------------
            template <> u32 GetAttributeValue<u32>(const XML::Node* in_node, const std::string& in_attributeName, const u32& in_defaultValue);
            //--------------------------------------------------
            /// Returns the value for the requested attribute name.
            /// If the attribute doesn't exist the default value
            /// will be returned instead.
            ///
            /// @author Ian Copland
            ///
            /// @param The node.
            /// @param The attribute name.
            /// @param The default value.
            ///
            /// @return The attribute value as an unsigned 64-bit
            /// integer.
            //--------------------------------------------------
            template <> u64 GetAttributeValue<u64>(const XML::Node* in_node, const std::string& in_attributeName, const u64& in_defaultValue);
            //--------------------------------------------------
            /// Returns the value for the requested attribute name.
            /// If the attribute doesn't exist the default value
            /// will be returned instead.
            ///
            /// @author Ian Copland
            ///
            /// @param The node.
            /// @param The attribute name.
            /// @param The default value.
            ///
            /// @return The attribute value as a Vector2.
            //--------------------------------------------------
            template <> Vector2 GetAttributeValue<Vector2>(const XML::Node* in_node, const std::string& in_attributeName, const Vector2& in_defaultValue);
            //--------------------------------------------------
            /// Returns the value for the requested attribute name.
            /// If the attribute doesn't exist the default value
            /// will be returned instead.
            ///
            /// @author Ian Copland
            ///
            /// @param The node.
            /// @param The attribute name.
            /// @param The default value.
            ///
            /// @return The attribute value as a Vector3.
            //--------------------------------------------------
            template <> Vector3 GetAttributeValue<Vector3>(const XML::Node* in_node, const std::string& in_attributeName, const Vector3& in_defaultValue);
            //--------------------------------------------------
            /// Returns the value for the requested attribute name.
            /// If the attribute doesn't exist the default value
            /// will be returned instead.
            ///
            /// @author Ian Copland
            ///
            /// @param The node.
            /// @param The attribute name.
            /// @param The default value.
            ///
            /// @return The attribute value as a Vector4.
            //--------------------------------------------------
            template <> Vector4 GetAttributeValue<Vector4>(const XML::Node* in_node, const std::string& in_attributeName, const Vector4& in_defaultValue);
            //--------------------------------------------------
            /// Returns the value for the requested attribute name.
            /// If the attribute doesn't exist the default value
            /// will be returned instead.
            ///
            /// @author Ian Copland
            ///
            /// @param The node.
            /// @param The attribute name.
            /// @param The default value.
            ///
            /// @return The attribute value as a Matrix3.
            //--------------------------------------------------
            template <> Matrix3 GetAttributeValue<Matrix3>(const XML::Node* in_node, const std::string& in_attributeName, const Matrix3& in_defaultValue);
            //--------------------------------------------------
            /// Returns the value for the requested attribute name.
            /// If the attribute doesn't exist the default value
            /// will be returned instead.
            ///
            /// @author Ian Copland
            ///
            /// @param The node.
            /// @param The attribute name.
            /// @param The default value.
            ///
            /// @return The attribute value as a Matrix4.
            //--------------------------------------------------
            template <> Matrix4 GetAttributeValue<Matrix4>(const XML::Node* in_node, const std::string& in_attributeName, const Matrix4& in_defaultValue);
            //--------------------------------------------------
            /// Returns the value for the requested attribute name.
            /// If the attribute doesn't exist the default value
            /// will be returned instead.
            ///
            /// @author Ian Copland
            ///
            /// @param The node.
            /// @param The attribute name.
            /// @param The default value.
            ///
            /// @return The attribute value as a Quaternion.
            //--------------------------------------------------
            template <> Quaternion GetAttributeValue<Quaternion>(const XML::Node* in_node, const std::string& in_attributeName, const Quaternion& in_defaultValue);
            //--------------------------------------------------
            /// Returns the value for the requested attribute name.
            /// If the attribute doesn't exist the default value
            /// will be returned instead.
            ///
            /// @author Ian Copland
            ///
            /// @param The node.
            /// @param The attribute name.
            /// @param The default value.
            ///
            /// @return The attribute value as a Colour.
            //--------------------------------------------------
            template <> Colour GetAttributeValue<Colour>(const XML::Node* in_node, const std::string& in_attributeName, const Colour& in_defaultValue);
            //--------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param The node.
            ///
            /// @return The node in string form.
            //--------------------------------------------------
            std::string ToString(const XML::Node* in_base);
            //--------------------------------------------------
            /// Reads an xml document from file.
            ///
            /// @author Ian Copland
            ///
            /// @param The document to write to file.
            /// @param The storage location of the file.
            /// @param The file path.
            ///
            /// @return Whether or not the write succeeded.
            //--------------------------------------------------
            bool WriteDocument(XML::Document* in_document, StorageLocation in_storageLocation, const std::string& in_filePath);
        }
    }
}

#endif
