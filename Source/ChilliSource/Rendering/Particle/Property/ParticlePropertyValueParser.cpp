//
//  ParticlePropertyValueParser.cpp
//  Chilli Source
//  Created by Ian Copland on 07/10/2014.
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

#include <ChilliSource/Rendering/Particle/Property/ParticlePropertyValueParser.h>

#include <ChilliSource/Core/String/StringParser.h>

namespace ChilliSource
{
	namespace Rendering
	{
		namespace ParticlePropertyValueParser
		{
			//--------------------------------------------------
			//--------------------------------------------------
			template <> std::string Parse(const std::string& in_value)
			{
				return in_value;
			}
			//--------------------------------------------------
			//--------------------------------------------------
			template <> bool Parse(const std::string& in_value)
			{
				return CSCore::ParseBool(in_value);
			}
			//--------------------------------------------------
			//--------------------------------------------------
			template <> u32 Parse(const std::string& in_value)
			{
				return CSCore::ParseU32(in_value);
			}
			//--------------------------------------------------
			//--------------------------------------------------
			template <> s32 Parse(const std::string& in_value)
			{
				return CSCore::ParseS32(in_value);
			}
			//--------------------------------------------------
			//--------------------------------------------------
			template <> u64 Parse(const std::string& in_value)
			{
				return CSCore::ParseU64(in_value);
			}
			//--------------------------------------------------
			//--------------------------------------------------
			template <> s64 Parse(const std::string& in_value)
			{
				return CSCore::ParseS64(in_value);
			}
			//--------------------------------------------------
			//--------------------------------------------------
			template <> f32 Parse(const std::string& in_value)
			{
				return CSCore::ParseF32(in_value);
			}
			//--------------------------------------------------
			//--------------------------------------------------
			template <> CSCore::Vector2 Parse(const std::string& in_value)
			{
				return CSCore::ParseVector2(in_value);
			}
			//--------------------------------------------------
			//--------------------------------------------------
			template <> CSCore::Vector3 Parse(const std::string& in_value)
			{
				return CSCore::ParseVector3(in_value);
			}
			//--------------------------------------------------
			//--------------------------------------------------
			template <> CSCore::Vector4 Parse(const std::string& in_value)
			{
				return CSCore::ParseVector4(in_value);
			}
			//--------------------------------------------------
			//--------------------------------------------------
			template <> CSCore::Matrix3 Parse(const std::string& in_value)
			{
				return CSCore::ParseMatrix3(in_value);
			}
			//--------------------------------------------------
			//--------------------------------------------------
			template <> CSCore::Matrix4 Parse(const std::string& in_value)
			{
				return CSCore::ParseMatrix4(in_value);
			}
			//--------------------------------------------------
			//--------------------------------------------------
			template <> CSCore::Quaternion Parse(const std::string& in_value)
			{
				return CSCore::ParseQuaternion(in_value);
			}
			//--------------------------------------------------
			//--------------------------------------------------
			template <> CSCore::Colour Parse(const std::string& in_value)
			{
				return CSCore::ParseColour(in_value);
			}
		}
	}
}