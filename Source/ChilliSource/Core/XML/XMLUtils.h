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

#include <rapidxml/rapidxml.hpp>

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
        /// @author I Copland
        //--------------------------------------------------------
        namespace XMLUtils
        {
            //--------------------------------------------------
            /// Convenience typedefs for all of the main rapid xml
            /// types.
            ///
            /// @author I Copland
            //--------------------------------------------------
            using Base = rapidxml::xml_base<>;
            using Node = rapidxml::xml_node<>;
            using Attribute = rapidxml::xml_attribute<>;
            using Document = rapidxml::xml_document<>;
            using DocumentUPtr = std::unique_ptr<rapidxml::xml_document<>>;
            //--------------------------------------------------
            /// Reads an xml document from file.
            ///
            /// @author I Copland
            ///
            /// @param The storage location of the file.
            /// @param The file path.
            ///
            /// @return the new document.
            //--------------------------------------------------
            DocumentUPtr ReadDocument(StorageLocation in_storageLocation, const std::string& in_filePath);
            //--------------------------------------------------
            /// Returns whether or not the given node has the
            /// given name.
            ///
            /// @author I Copland
            ///
            /// @param The node.
            /// @param The name.
            ///
            /// @return Whether or not it has the name.
            //--------------------------------------------------
            bool HasName(const Base* in_base, const std::string& in_name);
            //--------------------------------------------------
            /// Returns the value for the requested attribute name.
            /// If the attribute doesn't exist the default value
            /// will be returned instead.
            ///
            /// @author I Copland
            ///
            /// @param The node.
            /// @param The attribute name.
            /// @param The default value.
            ///
            /// @return The attribute value as a string.
            //--------------------------------------------------
            template <typename TType> TType GetAttribute(const Node* in_node, const std::string& in_attributeName, const TType& in_defaultValue)
            {
                CS_LOG_FATAL("XMLUtils: Unknown attribute type.");
            }
            //--------------------------------------------------
            /// Returns the value for the requested attribute name.
            /// If the attribute doesn't exist the default value
            /// will be returned instead.
            ///
            /// @author I Copland
            ///
            /// @param The node.
            /// @param The attribute name.
            /// @param The default value.
            ///
            /// @return The attribute value as a string.
            //--------------------------------------------------
            template <> std::string GetAttribute(const Node* in_node, const std::string& in_attributeName, const std::string& in_defaultValue);
            //--------------------------------------------------
            /// Returns the value for the requested attribute name.
            /// If the attribute doesn't exist the default value
            /// will be returned instead.
            ///
            /// @author I Copland
            ///
            /// @param The node.
            /// @param The attribute name.
            /// @param The default value.
            ///
            /// @return The attribute value as a boolean.
            //--------------------------------------------------
            bool GetAttribute(const Node* in_node, const std::string& in_attributeName, bool in_defaultValue);
            //--------------------------------------------------
            /// Returns the value for the requested attribute name.
            /// If the attribute doesn't exist the default value
            /// will be returned instead.
            ///
            /// @author I Copland
            ///
            /// @param The node.
            /// @param The attribute name.
            /// @param The default value.
            ///
            /// @return The attribute value as a float.
            //--------------------------------------------------
            f32 GetAttribute(const Node* in_node, const std::string& in_attributeName, f32 in_defaultValue);
            //--------------------------------------------------
            /// Returns the value for the requested attribute name.
            /// If the attribute doesn't exist the default value
            /// will be returned instead.
            ///
            /// @author I Copland
            ///
            /// @param The node.
            /// @param The attribute name.
            /// @param The default value.
            ///
            /// @return The attribute value as a signed 32-bit
            /// integer.
            //--------------------------------------------------
            s32 GetAttribute(const Node* in_node, const std::string& in_attributeName, s32 in_defaultValue);
            //--------------------------------------------------
            /// Returns the value for the requested attribute name.
            /// If the attribute doesn't exist the default value
            /// will be returned instead.
            ///
            /// @author I Copland
            ///
            /// @param The node.
            /// @param The attribute name.
            /// @param The default value.
            ///
            /// @return The attribute value as a signed 64-bit
            /// integer.
            //--------------------------------------------------
            s64 GetAttribute(const Node* in_node, const std::string& in_attributeName, s64 in_defaultValue);
            //--------------------------------------------------
            /// Returns the value for the requested attribute name.
            /// If the attribute doesn't exist the default value
            /// will be returned instead.
            ///
            /// @author I Copland
            ///
            /// @param The node.
            /// @param The attribute name.
            /// @param The default value.
            ///
            /// @return The attribute value as a Vector2.
            //--------------------------------------------------
            Vector2 GetAttribute(const Node* in_node, const std::string& in_attributeName, const Vector2& in_defaultValue);
            //--------------------------------------------------
            /// Returns the value for the requested attribute name.
            /// If the attribute doesn't exist the default value
            /// will be returned instead.
            ///
            /// @author I Copland
            ///
            /// @param The node.
            /// @param The attribute name.
            /// @param The default value.
            ///
            /// @return The attribute value as a Vector3.
            //--------------------------------------------------
            Vector3 GetAttribute(const Node* in_node, const std::string& in_attributeName, const Vector3& in_defaultValue);
            //--------------------------------------------------
            /// Returns the value for the requested attribute name.
            /// If the attribute doesn't exist the default value
            /// will be returned instead.
            ///
            /// @author I Copland
            ///
            /// @param The node.
            /// @param The attribute name.
            /// @param The default value.
            ///
            /// @return The attribute value as a Vector4.
            //--------------------------------------------------
            Vector4 GetAttribute(const Node* in_node, const std::string& in_attributeName, const Vector4& in_defaultValue);
            //--------------------------------------------------
            /// Returns the value for the requested attribute name.
            /// If the attribute doesn't exist the default value
            /// will be returned instead.
            ///
            /// @author I Copland
            ///
            /// @param The node.
            /// @param The attribute name.
            /// @param The default value.
            ///
            /// @return The attribute value as a Matrix3.
            //--------------------------------------------------
            Matrix3 GetAttribute(const Node* in_node, const std::string& in_attributeName, const Matrix3& in_defaultValue);
            //--------------------------------------------------
            /// Returns the value for the requested attribute name.
            /// If the attribute doesn't exist the default value
            /// will be returned instead.
            ///
            /// @author I Copland
            ///
            /// @param The node.
            /// @param The attribute name.
            /// @param The default value.
            ///
            /// @return The attribute value as a Matrix4.
            //--------------------------------------------------
            Matrix4 GetAttribute(const Node* in_node, const std::string& in_attributeName, const Matrix4& in_defaultValue);
            //--------------------------------------------------
            /// Returns the value for the requested attribute name.
            /// If the attribute doesn't exist the default value
            /// will be returned instead.
            ///
            /// @author I Copland
            ///
            /// @param The node.
            /// @param The attribute name.
            /// @param The default value.
            ///
            /// @return The attribute value as a Quaternion.
            //--------------------------------------------------
            Quaternion GetAttribute(const Node* in_node, const std::string& in_attributeName, const Quaternion& in_defaultValue);
            //--------------------------------------------------
            /// Returns the value for the requested attribute name.
            /// If the attribute doesn't exist the default value
            /// will be returned instead.
            ///
            /// @author I Copland
            ///
            /// @param The node.
            /// @param The attribute name.
            /// @param The default value.
            ///
            /// @return The attribute value as a Colour.
            //--------------------------------------------------
            Colour GetAttribute(const Node* in_node, const std::string& in_attributeName, const Colour& in_defaultValue);
            //--------------------------------------------------
            /// @author I Copland
            ///
            /// @param The node.
            ///
            /// @return The node in string form.
            //--------------------------------------------------
            std::string ToString(const Node* in_base);
            //--------------------------------------------------
            /// Reads an xml document from file.
            ///
            /// @author I Copland
            ///
            /// @param The storage location of the file.
            /// @param The file path.
            /// @param The document to write to file.
            ///
            /// @return Whether or not the write succeeded.
            //--------------------------------------------------
            bool WriteDocument(StorageLocation in_storageLocation, const std::string& in_filePath, Document* in_document);
        }
    }
}

#endif
