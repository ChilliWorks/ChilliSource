//
//  ParticlePropertyFactory.h
//  Chilli Source
//  Created by Ian Copland on 04/11/2014.
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

#ifndef _CHILLISOURCE_RENDERING_PARTICLE_PROPERTY_PARTICLEPROPERTYFACTORY_H_
#define _CHILLISOURCE_RENDERING_PARTICLE_PROPERTY_PARTICLEPROPERTYFACTORY_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/String/StringUtils.h>
#include <ChilliSource/Rendering/Particle/Property/RandomParticleProperty.h>
#include <ChilliSource/Rendering/Particle/Property/StaticParticleProperty.h>

#include <json/json.h>

namespace ChilliSource
{
	namespace Rendering
	{
		//------------------------------------------------------------------------
		/// A collection of methods for creating particle properties from json.
		///
		/// @author Ian Copland
		//------------------------------------------------------------------------
		namespace ParticlePropertyFactory
		{
			//----------------------------------------------------------------
			/// Creates a new particle property for the type described in the
			/// template parameter from the given json.
			///
			/// @author Ian Copland
			///
			/// @param The json value.
			///
			/// @return The output particle property.
			//----------------------------------------------------------------
			template <typename TType> ParticlePropertyUPtr<TType> CreateProperty(const Json::Value& in_json);
			//----------------------------------------------------------------
			/// Creates a new static particle property with the value described
			/// in the given string.
			///
			/// @author Ian Copland
			///
			/// @param The string value.
			///
			/// @return The output particle property.
			//----------------------------------------------------------------
			template <typename TType> ParticlePropertyUPtr<TType> CreateStaticProperty(const std::string& in_string);
			//----------------------------------------------------------------
			/// Creates a new random particle property with the value described
			/// in the given json.
			///
			/// @author Ian Copland
			///
			/// @param The json value.
			///
			/// @return The output particle property.
			//----------------------------------------------------------------
			template <typename TType> ParticlePropertyUPtr<TType> CreateRandomProperty(const Json::Value& in_json);
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
			template <typename TType> TType ParseValue(const std::string& in_value);
			//----------------------------------------------------------------
			/// Specialisation for parsing u32 values.
			///
			/// @author Ian Copland
			///
			/// @param The string to parse.
			///
			/// @return The parsed value.
			//----------------------------------------------------------------
			template <> u32 ParseValue(const std::string& in_value);
			//----------------------------------------------------------------
			/// Specialisation for parsing s32 values.
			///
			/// @author Ian Copland
			///
			/// @param The string to parse.
			///
			/// @return The parsed value.
			//----------------------------------------------------------------
			template <> s32 ParseValue(const std::string& in_value);
			//----------------------------------------------------------------
			/// Specialisation for parsing u64 values.
			///
			/// @author Ian Copland
			///
			/// @param The string to parse.
			///
			/// @return The parsed value.
			//----------------------------------------------------------------
			template <> u64 ParseValue(const std::string& in_value);
			//----------------------------------------------------------------
			/// Specialisation for parsing s64 values.
			///
			/// @author Ian Copland
			///
			/// @param The string to parse.
			///
			/// @return The parsed value.
			//----------------------------------------------------------------
			template <> s64 ParseValue(const std::string& in_value);
			//----------------------------------------------------------------
			/// Specialisation for parsing float values.
			///
			/// @author Ian Copland
			///
			/// @param The string to parse.
			///
			/// @return The parsed value.
			//----------------------------------------------------------------
			template <> f32 ParseValue(const std::string& in_value);
			//----------------------------------------------------------------
			/// Specialisation for parsing Vector2 values.
			///
			/// @author Ian Copland
			///
			/// @param The string to parse.
			///
			/// @return The parsed value.
			//----------------------------------------------------------------
			template <> Core::Vector2 ParseValue(const std::string& in_value);
			//----------------------------------------------------------------
			/// Specialisation for parsing Vector3 values.
			///
			/// @author Ian Copland
			///
			/// @param The string to parse.
			///
			/// @return The parsed value.
			//----------------------------------------------------------------
			template <> Core::Vector3 ParseValue(const std::string& in_value);
			//----------------------------------------------------------------
			/// Specialisation for parsing Vector4 values.
			///
			/// @author Ian Copland
			///
			/// @param The string to parse.
			///
			/// @return The parsed value.
			//----------------------------------------------------------------
			template <> Core::Vector4 ParseValue(const std::string& in_value);
			//----------------------------------------------------------------
			/// Specialisation for parsing Matrix3 values.
			///
			/// @author Ian Copland
			///
			/// @param The string to parse.
			///
			/// @return The parsed value.
			//----------------------------------------------------------------
			template <> Core::Matrix3 ParseValue(const std::string& in_value);
			//----------------------------------------------------------------
			/// Specialisation for parsing Matrix4 values.
			///
			/// @author Ian Copland
			///
			/// @param The string to parse.
			///
			/// @return The parsed value.
			//----------------------------------------------------------------
			template <> Core::Matrix4 ParseValue(const std::string& in_value);
			//----------------------------------------------------------------
			/// Specialisation for parsing Quaternion values.
			///
			/// @author Ian Copland
			///
			/// @param The string to parse.
			///
			/// @return The parsed value.
			//----------------------------------------------------------------
			template <> Core::Quaternion ParseValue(const std::string& in_value);
			//----------------------------------------------------------------
			/// Specialisation for parsing Colour values.
			///
			/// @author Ian Copland
			///
			/// @param The string to parse.
			///
			/// @return The parsed value.
			//----------------------------------------------------------------
			template <> Core::Colour ParseValue(const std::string& in_value);

			//----------------------------------------------------------------
			//----------------------------------------------------------------
			template <typename TType> ParticlePropertyUPtr<TType> CreateProperty(const Json::Value& in_json)
			{
				if (in_json.isNull() == false && in_json.isString() == true)
				{
					return CreateStaticProperty<TType>(in_json.asString());
				}
				else if (in_json.isNull() == false && in_json.isObject() == true)
				{
					Json::Value typeValue = in_json.get("Type", Json::nullValue);
					if (typeValue.isNull() == false && typeValue.isString() == true)
					{
						std::string typeString = typeValue.asString();
						Core::StringUtils::ToLowerCase(typeString);

						if (typeString == "static")
						{
							Json::Value value = in_json.get("ValueA", Json::nullValue);
							if (value.isNull() == false && value.isString() == true)
							{
								return CreateStaticProperty<TType>(value.asString());
							}
						}
						else if (typeString == "random")
						{
							return CreateRandomProperty<TType>(in_json);
						}
					}
				}

				CS_LOG_FATAL("Failed to create particle property.");
				return nullptr;
			}
			//----------------------------------------------------------------
			//----------------------------------------------------------------
			template <typename TType> ParticlePropertyUPtr<TType> CreateStaticProperty(const std::string& in_string)
			{
				return ParticlePropertyUPtr<TType>(new StaticParticleProperty<TType>(ParseValue<TType>(in_string)));
			}
			//----------------------------------------------------------------
			//----------------------------------------------------------------
			template <typename TType> ParticlePropertyUPtr<TType> CreateRandomProperty(const Json::Value& in_json)
			{
				Json::Value valueA = in_json.get("ValueA", Json::nullValue);
				Json::Value valueB = in_json.get("ValueB", Json::nullValue);

				if (valueA.isNull() == false && valueA.isString() == true && valueB.isNull() == false && valueB.isString() == true)
				{
					return ParticlePropertyUPtr<TType>(new RandomParticleProperty<TType>(ParseValue<TType>(valueA.asString()), ParseValue<TType>(valueB.asString())));
				}

				return nullptr;
			}
			//----------------------------------------------------------------
			//----------------------------------------------------------------
			template <typename TType> TType ParseValue(const std::string& in_value)
			{
				CS_LOG_FATAL("Cannot parse value of this type.");
			}
		}
	}
}

#endif