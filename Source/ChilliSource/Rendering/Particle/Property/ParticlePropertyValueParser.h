//
//  ParticlePropertyValueParser.h
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

#ifndef _CHILLISOURCE_RENDERING_PARTICLE_PROPERTY_PARTICLEPROPERTYVALUEPARSER_H_
#define _CHILLISOURCE_RENDERING_PARTICLE_PROPERTY_PARTICLEPROPERTYVALUEPARSER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Math/Vector2.h>
#include <ChilliSource/Core/Math/Vector3.h>
#include <ChilliSource/Core/Math/Vector4.h>
#include <ChilliSource/Core/Math/Matrix4.h>
#include <ChilliSource/Core/Math/Quaternion.h>
#include <ChilliSource/Core/Base/Colour.h>

namespace ChilliSource
{
	namespace Rendering
	{
		//-----------------------------------------------------------------------
		/// Provides templated methods for parsing values for use
		/// by particle properties.
		///
		/// @author Ian Copland
		//-----------------------------------------------------------------------
		namespace ParticlePropertyValueParser
		{
			//----------------------------------------------------------------
			/// A templated method for parsing values from a 
			/// string.
			///
			/// @author Ian Copland
			///
			/// @param The string to parse.
			///
			/// @return The parsed value.
			//----------------------------------------------------------------
			template <typename TType> TType Parse(const std::string& in_value)
			{
				CS_LOG_FATAL("Cannot parse value of this type.");
			}
			//----------------------------------------------------------------
			/// Specialisation for parsing string values. This 
			/// is needed to handle the case where a templated
			/// property is calling Parse<T>() and T happens to
			/// be string.
			///
			/// @author Ian Copland
			///
			/// @param The string to parse.
			///
			/// @return The un-parsed string.
			//----------------------------------------------------------------
			template <> std::string Parse(const std::string& in_value);
			//----------------------------------------------------------------
			/// Specialisation for parsing bool values.
			///
			/// @author Ian Copland
			///
			/// @param The string to parse.
			///
			/// @return The parsed value.
			//----------------------------------------------------------------
			template <> bool Parse(const std::string& in_value);
			//----------------------------------------------------------------
			/// Specialisation for parsing u32 values.
			///
			/// @author Ian Copland
			///
			/// @param The string to parse.
			///
			/// @return The parsed value.
			//----------------------------------------------------------------
			template <> u32 Parse(const std::string& in_value);
			//----------------------------------------------------------------
			/// Specialisation for parsing s32 values.
			///
			/// @author Ian Copland
			///
			/// @param The string to parse.
			///
			/// @return The parsed value.
			//----------------------------------------------------------------
			template <> s32 Parse(const std::string& in_value);
			//----------------------------------------------------------------
			/// Specialisation for parsing u64 values.
			///
			/// @author Ian Copland
			///
			/// @param The string to parse.
			///
			/// @return The parsed value.
			//----------------------------------------------------------------
			template <> u64 Parse(const std::string& in_value);
			//----------------------------------------------------------------
			/// Specialisation for parsing s64 values.
			///
			/// @author Ian Copland
			///
			/// @param The string to parse.
			///
			/// @return The parsed value.
			//----------------------------------------------------------------
			template <> s64 Parse(const std::string& in_value);
			//----------------------------------------------------------------
			/// Specialisation for parsing float values.
			///
			/// @author Ian Copland
			///
			/// @param The string to parse.
			///
			/// @return The parsed value.
			//----------------------------------------------------------------
			template <> f32 Parse(const std::string& in_value);
			//----------------------------------------------------------------
			/// Specialisation for parsing Vector2 values.
			///
			/// @author Ian Copland
			///
			/// @param The string to parse.
			///
			/// @return The parsed value.
			//----------------------------------------------------------------
			template <> Core::Vector2 Parse(const std::string& in_value);
			//----------------------------------------------------------------
			/// Specialisation for parsing Vector3 values.
			///
			/// @author Ian Copland
			///
			/// @param The string to parse.
			///
			/// @return The parsed value.
			//----------------------------------------------------------------
			template <> Core::Vector3 Parse(const std::string& in_value);
			//----------------------------------------------------------------
			/// Specialisation for parsing Vector4 values.
			///
			/// @author Ian Copland
			///
			/// @param The string to parse.
			///
			/// @return The parsed value.
			//----------------------------------------------------------------
			template <> Core::Vector4 Parse(const std::string& in_value);
			//----------------------------------------------------------------
			/// Specialisation for parsing Matrix3 values.
			///
			/// @author Ian Copland
			///
			/// @param The string to parse.
			///
			/// @return The parsed value.
			//----------------------------------------------------------------
			template <> Core::Matrix3 Parse(const std::string& in_value);
			//----------------------------------------------------------------
			/// Specialisation for parsing Matrix4 values.
			///
			/// @author Ian Copland
			///
			/// @param The string to parse.
			///
			/// @return The parsed value.
			//----------------------------------------------------------------
			template <> Core::Matrix4 Parse(const std::string& in_value);
			//----------------------------------------------------------------
			/// Specialisation for parsing Quaternion values.
			///
			/// @author Ian Copland
			///
			/// @param The string to parse.
			///
			/// @return The parsed value.
			//----------------------------------------------------------------
			template <> Core::Quaternion Parse(const std::string& in_value);
			//----------------------------------------------------------------
			/// Specialisation for parsing Colour values.
			///
			/// @author Ian Copland
			///
			/// @param The string to parse.
			///
			/// @return The parsed value.
			//----------------------------------------------------------------
			template <> Core::Colour Parse(const std::string& in_value);
		};
	}
}

#endif