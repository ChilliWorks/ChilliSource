//
//  JsonUtils.h
//  Chilli Source
//  Created by Ian Copland on 24/11/2014.
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

#ifndef _CHILLISOURCE_CORE_JSON_JSONUTILS_H_
#define _CHILLISOURCE_CORE_JSON_JSONUTILS_H_

#include <ChilliSource/ChilliSource.h>

#include <json/forwards.h>

namespace ChilliSource
{
    namespace Core
    {
        //----------------------------------------------------------------------
        /// A collection of convenience methods for working with json. This
        /// includes a method for creating a new Json object from a string
        /// and for creating a new Json object from a file.
        ///
        /// @author Ian Copland
        //----------------------------------------------------------------------
        namespace JsonUtils
        {
            //---------------------------------------------------------------
            /// Creates a new Json object from the given string. If there
            /// are any errors in the given Json the app is considered to be
            /// in an irrecoverable state and will terminate.
            ///
            /// @author Ian Copland
            ///
            /// @param The input json string.
            ///
            /// @return The new json object.
            //---------------------------------------------------------------
            Json::Value ParseJson(const std::string& in_jsonString);
            //---------------------------------------------------------------
            /// Creates a new Json object from the contents of the file
            /// at the given path. If the file cannot be read this will return
            /// false, but if the file can be read but it is not valid json
            /// the app is considered to be in an irrecoverable state and
            /// will terminate.
            ///
            /// @author Ian Copland
            ///
            /// @param The storage location of the file.
            /// @param The file path.
            ///
            /// @return The new json object.
            //---------------------------------------------------------------
            bool ReadJson(Core::StorageLocation in_storageLocation, const std::string& in_filePath, Json::Value& out_jsonValue);
        }
    }
}

#endif