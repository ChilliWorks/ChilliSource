//
//  ToString.cpp
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

#include <ChilliSource/Core/String/ToString.h>

#include <ChilliSource/Core/Base/Colour.h>
#include <ChilliSource/Core/File/FileSystem.h>
#include <ChilliSource/Core/Math/Matrix4.h>
#include <ChilliSource/Core/Math/Quaternion.h>
#include <ChilliSource/Core/Math/Vector2.h>
#include <ChilliSource/Core/Math/Vector3.h>
#include <ChilliSource/Core/Math/Vector4.h>

#include <sstream>

namespace ChilliSource
{
    namespace Core
    {
        //---------------------------------------------------------
        //---------------------------------------------------------
        std::string ToString(f32 in_value, u16 in_precision, u16 in_width, s8 in_fill, std::ios::fmtflags in_flags)
        {
            std::stringstream stream;
			stream.precision(in_precision);
			stream.width(in_width);
			stream.fill(in_fill);
			if (in_flags)
				stream.setf(in_flags);
			stream << in_value;
			return stream.str();
        }
        //---------------------------------------------------------
        //---------------------------------------------------------
        std::string ToString(f64 in_value, u16 in_precision, u16 in_width, s8 in_fill, std::ios::fmtflags in_flags)
        {
            std::stringstream stream;
			stream.precision(in_precision);
			stream.width(in_width);
			stream.fill(in_fill);
			if (in_flags)
				stream.setf(in_flags);
			stream << in_value;
			return stream.str();
        }
        //---------------------------------------------------------
        //---------------------------------------------------------
        std::string ToString(s32 in_value, u16 in_width, s8 in_fill, std::ios::fmtflags in_flags)
        {
            std::stringstream stream;
			stream.width(in_width);
			stream.fill(in_fill);
			if (in_flags)
				stream.setf(in_flags);
			stream << in_value;
			return stream.str();
        }
        //---------------------------------------------------------
        //---------------------------------------------------------
        std::string ToString(s32 in_value)
        {
            s8 buffer[16];
			u32 size = CS_SPRINTF(buffer, "%d", in_value);
            return std::string(buffer, buffer + size);
        }
        //---------------------------------------------------------
        //---------------------------------------------------------
        std::string ToString(u32 in_value, u16 in_width, s8 in_fill, std::ios::fmtflags in_flags)
        {
            std::stringstream stream;
			stream.width(in_width);
			stream.fill(in_fill);
			if (in_flags)
				stream.setf(in_flags);
			stream << in_value;
			return stream.str();
        }
        //---------------------------------------------------------
        //---------------------------------------------------------
        std::string ToString(u32 in_value)
        {
            s8 buffer[16];
			u32 size = CS_SPRINTF(buffer, "%d", in_value);
            return std::string(buffer, buffer + size);
        }
        //---------------------------------------------------------
        //---------------------------------------------------------
        std::string ToString(s64 in_value, u16 in_width, s8 in_fill, std::ios::fmtflags in_flags)
        {
            std::stringstream stream;
			stream.width(in_width);
			stream.fill(in_fill);
			if (in_flags)
				stream.setf(in_flags);
			stream << in_value;
			return stream.str();
        }
        //---------------------------------------------------------
        //---------------------------------------------------------
        std::string ToString(u64 in_value, u16 in_width, s8 in_fill, std::ios::fmtflags in_flags)
        {
            std::stringstream stream;
			stream.width(in_width);
			stream.fill(in_fill);
			if (in_flags)
				stream.setf(in_flags);
			stream << in_value;
			return stream.str();
        }
        //---------------------------------------------------------
        //---------------------------------------------------------
        std::string ToString(bool in_value, bool in_yesNo)
        {
            if (in_value)
			{
				if (in_yesNo)
				{
					return "yes";
				}
				else
				{
					return "true";
				}
			}
			else
            {
				if (in_yesNo)
				{
					return "no";
				}
				else
				{
					return "false";
				}
            }
        }
        //---------------------------------------------------------
        //---------------------------------------------------------
        std::string ToString(const Vector2& in_value)
        {
            std::stringstream stream;
			stream << in_value.x << " " << in_value.y;
			return stream.str();
        }
        //---------------------------------------------------------
        //---------------------------------------------------------
        std::string ToString(const Vector3& in_value)
        {
            std::stringstream stream;
			stream << in_value.x << " " << in_value.y << " " << in_value.z;
			return stream.str();
        }
        //---------------------------------------------------------
        //---------------------------------------------------------
        std::string ToString(const Vector4& in_value)
        {
            std::stringstream stream;
			stream << in_value.x << " " << in_value.y << " " << in_value.z << " " << in_value.w;
			return stream.str();
        }
        //---------------------------------------------------------
        //---------------------------------------------------------
        std::string ToString(const Matrix4& in_value)
        {
            std::stringstream stream;
			stream << in_value.m[0] << " "
			<< in_value.m[1] << " "
			<< in_value.m[2] << " "
			<< in_value.m[3] << " "
			<< in_value.m[4] << " "
			<< in_value.m[5] << " "
			<< in_value.m[6] << " "
			<< in_value.m[7] << " "
			<< in_value.m[8] << " "
			<< in_value.m[9] << " "
			<< in_value.m[10] << " "
			<< in_value.m[11] << " "
			<< in_value.m[12] << " "
			<< in_value.m[13] << " "
			<< in_value.m[14] << " "
			<< in_value.m[15];
			return stream.str();
        }
        //---------------------------------------------------------
        //---------------------------------------------------------
        std::string ToString(const Quaternion& in_value)
        {
            std::stringstream stream;
			stream  << in_value.w << " " << in_value.x << " " << in_value.y << " " << in_value.z;
			return stream.str();
        }
        //---------------------------------------------------------
        //---------------------------------------------------------
        std::string ToString(const Colour& in_value)
        {
            std::stringstream stream;
			stream << in_value.r << " " << in_value.g << " " << in_value.b << " " << in_value.a;
			return stream.str();
        }
        //---------------------------------------------------------
        //---------------------------------------------------------
        std::string ToString(const StorageLocation& in_storageLocation)
        {
            switch (in_storageLocation)
            {
                case StorageLocation::k_package:
                    return "Package";
                case StorageLocation::k_saveData:
                    return "SaveData";
                case StorageLocation::k_cache:
                    return "Cache";
                case StorageLocation::k_DLC:
                    return "DLC";
                case StorageLocation::k_chilliSource:
                    return "ChilliSource";
                default:
                    return "";
            }
        }
        
#ifdef CS_TARGETPLATFORM_IOS
        //---------------------------------------------------------
        //---------------------------------------------------------
        std::string ToString(size_t in_value)
        {
            return ToString(static_cast<u32>(in_value));
        }
#endif
    }
}
