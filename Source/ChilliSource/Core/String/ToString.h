//
//  ToString.h
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

#ifndef _CHILLISOURCE_CORE_STRING_TOSTRING_H_
#define _CHILLISOURCE_CORE_STRING_TOSTRING_H_

#include <ChilliSource/ChilliSource.h>

#include <string>
#include <iostream>

namespace ChilliSource
{
    namespace Core
    {
        //---------------------------------------------------------
        /// Converts a 32 bit float to a string.
        ///
        /// @author Ian Copland
        ///
        /// @param The float value.
        /// @param The precision to use.
        /// @param The minimum width of the string.
        /// @param The char used to pad strings smaller than the
        /// width.
        /// @param The format flags.
        ///
        /// @return The value as a string.
        //---------------------------------------------------------
        std::string ToString(f32 in_value, u16 in_precision = 6, u16 in_width = 0, s8 in_fill = ' ', std::ios::fmtflags in_flags = std::ios::fmtflags(0));
        //---------------------------------------------------------
        /// Converts a 64 bit float to a string.
        ///
        /// @author Ian Copland
        ///
        /// @param The float value.
        /// @param The precision to use.
        /// @param The minimum width of the string.
        /// @param The char used to pad strings smaller than the
        /// width.
        /// @param The format flags.
        ///
        /// @return The value as a string.
        //---------------------------------------------------------
        std::string ToString(f64 in_value, u16 in_precision = 6, u16 in_width = 0, s8 in_fill = ' ', std::ios::fmtflags in_flags = std::ios::fmtflags(0));
        //---------------------------------------------------------
        /// Converts a 32 bit signed integer to a string.
        ///
        /// @author Ian Copland
        ///
        /// @param The integer value.
        /// @param The minimum width of the string.
        /// @param The char used to pad strings smaller than the
        /// width.
        /// @param The format flags.
        
        /// @return The value as a string.
        //---------------------------------------------------------
        std::string ToString(s32 in_value, u16 in_width, s8 in_fill = ' ', std::ios::fmtflags in_flags = std::ios::fmtflags(0));
        //---------------------------------------------------------
        /// Converts a 32 bit signed integer to a string.
        ///
        /// @author Ian Copland
        ///
        /// @param The integer value.
        ///
        /// @return The value as a string.
        //---------------------------------------------------------
        std::string ToString(s32 in_val);
        //---------------------------------------------------------
        /// Converts a 32 bit unsigned integer to a string.
        ///
        /// @author Ian Copland
        ///
        /// @param The integer value.
        /// @param The minimum width of the string.
        /// @param The char used to pad strings smaller than the
        /// width.
        /// @param The format flags.
        ///
        /// @return The value as a string.
        //---------------------------------------------------------
        std::string ToString(u32 in_value, u16 in_width, s8 in_fill = ' ', std::ios::fmtflags in_flags = std::ios::fmtflags(0));
        //---------------------------------------------------------
        /// Converts a 32 bit unsigned integer to a string.
        ///
        /// @author Ian Copland
        ///
        /// @param The integer value.
        ///
        /// @return The value as a string.
        //---------------------------------------------------------
        std::string ToString(u32 in_value);
        //---------------------------------------------------------
        /// Converts a 64 bit signed integer to a string.
        ///
        /// @author Ian Copland
        ///
        /// @param The integer value.
        /// @param The minimum width of the string.
        /// @param The char used to pad strings smaller than the
        /// width.
        /// @param The format flags.
        ///
        /// @return The value as a string.
        //---------------------------------------------------------
        std::string ToString(s64 in_value, u16 in_width = 0, s8 in_fill = ' ', std::ios::fmtflags in_flags = std::ios::fmtflags(0));
        //---------------------------------------------------------
        /// Converts a 64 bit signed integer to a string.
        ///
        /// @author Ian Copland
        ///
        /// @param The integer value.
        /// @param The minimum width of the string.
        /// @param The char used to pad strings smaller than the
        /// width.
        /// @param The format flags.
        ///
        /// @return The value as a string.
        //---------------------------------------------------------
        std::string ToString(u64 in_value, u16 in_width = 0, s8 in_fill = ' ', std::ios::fmtflags in_flags = std::ios::fmtflags(0));
        //---------------------------------------------------------
        /// Converts a boolean to a string.
        ///
        /// @author Ian Copland
        ///
        /// @param The boolean value.
        /// @param If this is true the result given will be "yes"
        /// and "no" instead of "true" and "false".
        ///
        /// @return The value as a string.
        //---------------------------------------------------------
        std::string ToString(bool in_value, bool in_yesNo = false);
        //---------------------------------------------------------
        /// Converts a 2 component vector to a string.
        ///
        /// @author Ian Copland
        ///
        /// @param The vector value.
        ///
        /// @return The value as a string.
        //---------------------------------------------------------
        std::string ToString(const Vector2& in_value);
        //---------------------------------------------------------
        /// Converts a 3 component vector to a string.
        ///
        /// @author Ian Copland
        ///
        /// @param The vector value.
        ///
        /// @return The value as a string.
        //---------------------------------------------------------
        std::string ToString(const Vector3& in_value);
        //---------------------------------------------------------
        /// Converts a 4 component vector to a string.
        ///
        /// @author Ian Copland
        ///
        /// @param The vector value.
        ///
        /// @return The value as a string.
        //---------------------------------------------------------
        std::string ToString(const Vector4& in_value);
        //---------------------------------------------------------
        /// Converts a 4x4 matrix to a string.
        ///
        /// @author Ian Copland
        ///
        /// @param The matrix value.
        ///
        /// @return The value as a string.
        //---------------------------------------------------------
        std::string ToString(const Matrix4& in_value);
        //---------------------------------------------------------
        /// Converts a quaternion to a string.
        ///
        /// @author Ian Copland
        ///
        /// @param The quaternion value.
        ///
        /// @return The value as a string.
        //---------------------------------------------------------
        std::string ToString(const Quaternion& in_value);
        //---------------------------------------------------------
        /// Converts a colour to a string.
        ///
        /// @author Ian Copland
        ///
        /// @param The colour value.
        ///
        /// @return The value as a string.
        //---------------------------------------------------------
        std::string ToString(const Colour& in_value);
        //---------------------------------------------------------
        /// Converts a storage location value to a string.
        ///
        /// @author Ian Copland
        ///
        /// @param The storage location value.
        ///
        /// @return The value as a string.
        //---------------------------------------------------------
        std::string ToString(const StorageLocation& in_storageLocation);
        
#ifdef CS_TARGETPLATFORM_IOS
        //---------------------------------------------------------
        /// Converts a size_t to a string.
        ///
        /// @author Ian Copland
        ///
        /// @param The size_t value.
        ///
        /// @return The value as a string.
        //---------------------------------------------------------
        std::string ToString(size_t in_value);
#endif
    }
}

#endif
