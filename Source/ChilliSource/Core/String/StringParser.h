//
//  StringParser.h
//  Chilli Source
//  Created by Ian Copland on 25/02/2014.
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

#ifndef _CHILLISOURCE_CORE_STRING_STRINGPARSER_H_
#define _CHILLISOURCE_CORE_STRING_STRINGPARSER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Math/Vector2.h>
#include <ChilliSource/Core/Math/Vector3.h>
#include <ChilliSource/Core/Math/Vector4.h>
#include <ChilliSource/Core/Math/Matrix4.h>
#include <ChilliSource/Core/Math/Quaternion.h>
#include <ChilliSource/Core/Base/Colour.h>
#include <ChilliSource/Core/File/FileSystem.h>

namespace ChilliSource
{
    namespace Core
    {
        //------------------------------------------------------------
        /// Parse a string to a 32 bit float. If the string cannot be
        /// be parsed this will return 0.
        ///
        /// @author Ian Copland
        ///
        /// @param The string.
        ///
        /// @return The float value.
        //------------------------------------------------------------
        f32 ParseF32(const std::string& in_string);
        //------------------------------------------------------------
        /// Parse a string to a 32 bit signed integer. If the string
        /// cannot be parsed this will return 0.
        ///
        /// @author Ian Copland
        ///
        /// @param The string.
        ///
        /// @return The integer value.
        //------------------------------------------------------------
        s32 ParseS32(const std::string& in_string);
        //------------------------------------------------------------
        /// Parse a string to a 32 bit unsigned integer. If the string
        /// cannot be parsed this will return 0.
        ///
        /// @author Ian Copland
        ///
        /// @param The string.
        ///
        /// @return The integer value.
        //------------------------------------------------------------
        u32 ParseU32(const std::string& in_string);
        //------------------------------------------------------------
        /// Parse a string to a 64 bit signed integer. If the string
        /// cannot be parsed this will return 0.
        ///
        /// @author Ian Copland
        ///
        /// @param The string.
        ///
        /// @return The integer value.
        //------------------------------------------------------------
        s64 ParseS64(const std::string& in_string);
        //------------------------------------------------------------
        /// Parse a string to a 32 bit unsigned integer. If the string
        /// cannot be parsed this will return 0.
        ///
        /// @author Ian Copland
        ///
        /// @param The string.
        ///
        /// @return The integer value.
        //------------------------------------------------------------
        u64 ParseU64(const std::string& in_string);
        //------------------------------------------------------------
        /// Parse a string to a 32 bit unsigned integer. True will be
        /// returned if the string is "true", "yes" or "1" and false
        /// will be returned in all other cases.
        ///
        /// @author Ian Copland
        ///
        /// @param The string.
        ///
        /// @return The boolean value.
        //------------------------------------------------------------
        bool ParseBool(const std::string& in_string);
        //------------------------------------------------------------
        /// Parse a string to a 2 component vector. The vector must be
        /// in the format "x y", i.e F32 values, space delimited. Failure
        /// to parse will result in a zero vector.
        ///
        /// @author Ian Copland
        ///
        /// @param The string.
        ///
        /// @return The vector value.
        //------------------------------------------------------------
        Vector2 ParseVector2(const std::string& in_string);
        //------------------------------------------------------------
        /// Parse a string to a 3 component vector. The vector must be
        /// in the format "x y z", i.e F32 values, space delimited. Failure
        /// to parse will result in a zero vector.
        ///
        /// @author Ian Copland
        ///
        /// @param The string.
        ///
        /// @return The vector value.
        //------------------------------------------------------------
        Vector3 ParseVector3(const std::string& in_string);
        //------------------------------------------------------------
        /// Parse a string to a 4 component vector. The vector must be
        /// in the format "x y z w", i.e F32 values, space delimited.
        /// Failure to parse will result in a zero vector.
        ///
        /// @author Ian Copland
        ///
        /// @param The string.
        ///
        /// @return The vector value.
        //------------------------------------------------------------
        Vector4 ParseVector4(const std::string& in_string);
        //------------------------------------------------------------
        /// Parse a string to a matrix. The matrix should be provided
        /// in row major format, space delimited. failure to parse will
        /// result in an identity matrix.
        ///
        /// @author Ian Copland
        ///
        /// @param The string.
        ///
        /// @return The matrix value.
        //------------------------------------------------------------
        Matrix3 ParseMatrix3(const std::string& in_string);
        //------------------------------------------------------------
        /// Parse a string to a matrix. The matrix should be provided
        /// in row major format, space delimited. failure to parse will
        /// result in an identity matrix.
        ///
        /// @author Ian Copland
        ///
        /// @param The string.
        ///
        /// @return The matrix value.
        //------------------------------------------------------------
        Matrix4 ParseMatrix4(const std::string& in_string);
        //------------------------------------------------------------
        /// Parse a string to a quaternion. The quaternion must be
        /// in the format "w x y z", i.e F32 values, space delimited.
        /// Failure to parse will result in an identity quaternion.
        ///
        /// @author Ian Copland
        ///
        /// @param The string.
        ///
        /// @return The quaternion value.
        //------------------------------------------------------------
        Quaternion ParseQuaternion(const std::string& in_string);
        //------------------------------------------------------------
        /// Parse a string to a colour. The colour must be
        /// in the format "r g b a", i.e F32 values, space delimited.
        /// "r g b" will also be accepted. Failure to parse will result
        /// in a black colour.
        ///
        /// @author Ian Copland
        ///
        /// @param The string.
        ///
        /// @return The colour value.
        //------------------------------------------------------------
        Colour ParseColour(const std::string& in_string);
        //------------------------------------------------------------
        /// Parse a storage location. Acceptable storage locations are:
        ///
        ///  - Package
        ///  - DLC
        ///  - SaveData
        ///  - Cache
        ///  - Root
        ///
        /// Note that the location strings are not case sensitive.
        ///
        /// @author Ian Copland
        ///
        /// @param The string.
        ///
        /// @return The colour value.
        //------------------------------------------------------------
        StorageLocation ParseStorageLocation(const std::string &in_string);
        //------------------------------------------------------------
        /// Converts from a string to a surface format type. Valid
        /// formats are:
        ///
        ///  - RGB565_DEPTH24
        ///  - RGB565_DEPTH32
        ///  - RGB888_DEPTH24
        ///  - RGB888_DEPTH32
        ///
        /// Note that the format strings are not case sensitive.
        ///
        /// @author Ian Copland
        ///
        /// @param The string.
        ///
        /// @return The surface format.
        //------------------------------------------------------------
        Rendering::SurfaceFormat ParseSurfaceFormat(const std::string& in_surfaceFormat);
    }
}

#endif
