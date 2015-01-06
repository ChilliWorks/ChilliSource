//
//  ParticlePropertyFactoryImpl.h
//  Chilli Source
//  Created by Ian Copland on 29/12/2014.
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

#ifndef _CHILLISOURCE_RENDERING_PARTICLE_PROPERTY_PARTICLEPROPERTYFACTORYIMPL_H_
#define _CHILLISOURCE_RENDERING_PARTICLE_PROPERTY_PARTICLEPROPERTYFACTORYIMPL_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/String/StringUtils.h>
#include <ChilliSource/Rendering/Particle/Property/ComponentwiseRandomConstantParticleProperty.h>
#include <ChilliSource/Rendering/Particle/Property/ComponentwiseRandomCurveParticleProperty.h>
#include <ChilliSource/Rendering/Particle/Property/ConstantParticleProperty.h>
#include <ChilliSource/Rendering/Particle/Property/CurveParticleProperty.h>
#include <ChilliSource/Rendering/Particle/Property/RandomConstantParticleProperty.h>
#include <ChilliSource/Rendering/Particle/Property/RandomCurveParticleProperty.h>

#include <json/json.h>

namespace ChilliSource
{
    namespace Rendering
    {
        //------------------------------------------------------------------------------
        /// A collection of implementation functions for the particle property factory.
        /// These should not be accessed directly, the particle effect factory functions
        /// should be called instead.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        namespace ParticlePropertyFactory
        {
            namespace Impl
            {
                static const char k_constantType[] = "constant";
                static const char k_randomConstantType[] = "randomconstant";
                static const char k_componentwiseRandomConstantType[] = "componentwiserandomconstant";
                static const char k_curveType[] = "curve";
                static const char k_randomCurveType[] = "randomcurve";
                static const char k_componentwiseRandomCurveType[] = "componentwiserandomcurve";
                
                static const char k_typeKey[] = "Type";
                static const char k_valueKey[] = "Value";
                static const char k_lowerValueKey[] = "LowerValue";
                static const char k_upperValueKey[] = "UpperValue";
                static const char k_startValueKey[] = "StartValue";
                static const char k_endValueKey[] = "EndValue";
                static const char k_startLowerValueKey[] = "StartLowerValue";
                static const char k_startUpperValueKey[] = "StartUpperValue";
                static const char k_endLowerValueKey[] = "EndLowerValue";
                static const char k_endUpperValueKey[] = "EndUpperValue";
                static const char k_curveKey[] = "Curve";
                
                //------------------------------------------------------------------------------
                /// Parse the curve function.
                ///
                /// @author Ian Copland
                ///
                /// @param The string name of the curve.
                ///
                /// @return The
                //------------------------------------------------------------------------------
                std::function<f32(f32)> ParseCurveFunction(const std::string& in_curveName);
                //------------------------------------------------------------------------------
                /// A templated method for parsing values from a string.
                ///
                /// @author Ian Copland
                ///
                /// @param The string to parse.
                ///
                /// @return The parsed value.
                //------------------------------------------------------------------------------
                template <typename TType> TType ParseValue(const std::string& in_value)
                {
                    CS_LOG_FATAL("Cannot parse value of this type.");
                }
                //------------------------------------------------------------------------------
                /// Creates a new constant particle property with the value described in the
                /// given string.
                ///
                /// @author Ian Copland
                ///
                /// @param The string value.
                ///
                /// @return The output particle property.
                //------------------------------------------------------------------------------
                template <typename TType> ParticlePropertyUPtr<TType> CreateConstantProperty(const std::string& in_string)
                {
                    return ParticlePropertyUPtr<TType>(new ConstantParticleProperty<TType>(ParseValue<TType>(in_string)));
                }
                //------------------------------------------------------------------------------
                /// Creates a new constant particle property with value described in the given
                /// json.
                ///
                /// @author Ian Copland
                ///
                /// @param The json value.
                ///
                /// @return The output particle property.
                //------------------------------------------------------------------------------
                template <typename TType> ParticlePropertyUPtr<TType> CreateConstantProperty(const Json::Value& in_json)
                {
                    Json::Value value = in_json.get(k_valueKey, Json::nullValue);
                    CS_ASSERT(value != Json::nullValue, "Must supply '" + std::string(k_valueKey) + "' in '" + std::string(k_constantType) + "' property.");
                    CS_ASSERT(value.isString(), "'" + std::string(k_valueKey) + "' in '" + std::string(k_constantType) + "' property must be a string.");
                    
                    return ParticlePropertyUPtr<TType>(new ConstantParticleProperty<TType>(ParseValue<TType>(value.asString())));
                }
                //------------------------------------------------------------------------------
                /// Creates a new random constant particle property with the value described in
                /// the given json.
                ///
                /// @author Ian Copland
                ///
                /// @param The json value.
                ///
                /// @return The output particle property.
                //------------------------------------------------------------------------------
                template <typename TType> ParticlePropertyUPtr<TType> CreateRandomConstantProperty(const Json::Value& in_json)
                {
                    Json::Value lowerValue = in_json.get(k_lowerValueKey, Json::nullValue);
                    CS_ASSERT(lowerValue != Json::nullValue, "Must supply '" + std::string(k_lowerValueKey) + "' in '" + std::string(k_randomConstantType) + "' property.");
                    CS_ASSERT(lowerValue.isString(), "'" + std::string(k_lowerValueKey) + "' in '" + std::string(k_randomConstantType) + "' property must be a string.");
                    
                    Json::Value upperValue = in_json.get(k_upperValueKey, Json::nullValue);
                    CS_ASSERT(upperValue != Json::nullValue, "Must supply '" + std::string(k_upperValueKey) + "' in '" + std::string(k_randomConstantType) + "' property.");
                    CS_ASSERT(upperValue.isString(), "'" + std::string(k_upperValueKey) + "' in '" + std::string(k_randomConstantType) + "' property must be a string.");
                    
                    return ParticlePropertyUPtr<TType>(new RandomConstantParticleProperty<TType>(ParseValue<TType>(lowerValue.asString()), ParseValue<TType>(upperValue.asString())));
                }
                //------------------------------------------------------------------------------
                /// Creates a new componentwise random constant particle property with the value
                /// described in the given json.
                ///
                /// @author Ian Copland
                ///
                /// @param The json value.
                ///
                /// @return The output particle property.
                //------------------------------------------------------------------------------
                template <typename TType> ParticlePropertyUPtr<TType> CreateComponentwiseRandomConstantProperty(const Json::Value& in_json)
                {
                    Json::Value lowerValue = in_json.get(k_lowerValueKey, Json::nullValue);
                    CS_ASSERT(lowerValue != Json::nullValue, "Must supply '" + std::string(k_lowerValueKey) + "' in '" + std::string(k_componentwiseRandomConstantType) + "' property.");
                    CS_ASSERT(lowerValue.isString(), "'" + std::string(k_lowerValueKey) + "' in '" + std::string(k_componentwiseRandomConstantType) + "' property must be a string.");
                    
                    Json::Value upperValue = in_json.get(k_upperValueKey, Json::nullValue);
                    CS_ASSERT(upperValue != Json::nullValue, "Must supply '" + std::string(k_upperValueKey) + "' in '" + std::string(k_componentwiseRandomConstantType) + "' property.");
                    CS_ASSERT(upperValue.isString(), "'" + std::string(k_upperValueKey) + "' in '" + std::string(k_componentwiseRandomConstantType) + "' property must be a string.");
                    
                    return ParticlePropertyUPtr<TType>(new ComponentwiseRandomConstantParticleProperty<TType>(ParseValue<TType>(lowerValue.asString()), ParseValue<TType>(upperValue.asString())));
                }
                //------------------------------------------------------------------------------
                /// Creates a new curve particle property with the value described in the given
                /// json.
                ///
                /// @author Ian Copland
                ///
                /// @param The json value.
                ///
                /// @return The output particle property.
                //------------------------------------------------------------------------------
                template <typename TType> ParticlePropertyUPtr<TType> CreateCurveProperty(const Json::Value& in_json)
                {
                    Json::Value curve = in_json.get(k_curveKey, Json::nullValue);
                    CS_ASSERT(curve != Json::nullValue, "Must supply '" + std::string(k_curveKey) + "' in '" + std::string(k_curveType) + "' property.");
                    CS_ASSERT(curve.isString(), "'" + std::string(k_curveKey) + "' in '" + std::string(k_curveType) + "' property must be a string.");
                    
                    Json::Value startValue = in_json.get(k_startValueKey, Json::nullValue);
                    CS_ASSERT(startValue != Json::nullValue, "Must supply '" + std::string(k_startValueKey) + "' in '" + std::string(k_curveType) + "' property.");
                    CS_ASSERT(startValue.isString(), "'" + std::string(k_startValueKey) + "' in '" + std::string(k_curveType) + "' property must be a string.");
                    
                    Json::Value endValue = in_json.get(k_endValueKey, Json::nullValue);
                    CS_ASSERT(endValue != Json::nullValue, "Must supply '" + std::string(k_endValueKey) + "' in '" + std::string(k_curveType) + "' property.");
                    CS_ASSERT(endValue.isString(), "'" + std::string(k_endValueKey) + "' in '" + std::string(k_curveType) + "' property must be a string.");
                    
                    return ParticlePropertyUPtr<TType>(new CurveParticleProperty<TType>(ParseValue<TType>(startValue.asString()), ParseValue<TType>(endValue.asString()), ParseCurveFunction(curve.asString())));
                }
                //------------------------------------------------------------------------------
                /// Creates a new random curve particle property with the value described in the
                /// given json.
                ///
                /// @author Ian Copland
                ///
                /// @param The json value.
                ///
                /// @return The output particle property.
                //------------------------------------------------------------------------------
                template <typename TType> ParticlePropertyUPtr<TType> CreateRandomCurveProperty(const Json::Value& in_json)
                {
                    Json::Value curve = in_json.get(k_curveKey, Json::nullValue);
                    CS_ASSERT(curve != Json::nullValue, "Must supply '" + std::string(k_curveKey) + "' in '" + std::string(k_randomCurveType) + "' property.");
                    CS_ASSERT(curve.isString(), "'" + std::string(k_curveKey) + "' in '" + std::string(k_randomCurveType) + "' property must be a string.");
                    
                    Json::Value startLowerValue = in_json.get(k_startLowerValueKey, Json::nullValue);
                    CS_ASSERT(startLowerValue != Json::nullValue, "Must supply '" + std::string(k_startLowerValueKey) + "' in '" + std::string(k_randomCurveType) + "' property.");
                    CS_ASSERT(startLowerValue.isString(), "'" + std::string(k_startLowerValueKey) + "' in '" + std::string(k_randomCurveType) + "' property must be a string.");
                    
                    Json::Value startUpperValue = in_json.get(k_startUpperValueKey, Json::nullValue);
                    CS_ASSERT(startUpperValue != Json::nullValue, "Must supply '" + std::string(k_startUpperValueKey) + "' in '" + std::string(k_randomCurveType) + "' property.");
                    CS_ASSERT(startUpperValue.isString(), "'" + std::string(k_startUpperValueKey) + "' in '" + std::string(k_randomCurveType) + "' property must be a string.");
                    
                    Json::Value endLowerValue = in_json.get(k_endLowerValueKey, Json::nullValue);
                    CS_ASSERT(endLowerValue != Json::nullValue, "Must supply '" + std::string(k_endLowerValueKey) + "' in '" + std::string(k_randomCurveType) + "' property.");
                    CS_ASSERT(endLowerValue.isString(), "'" + std::string(k_endLowerValueKey) + "' in '" + std::string(k_randomCurveType) + "' property must be a string.");
                    
                    Json::Value endUpperValue = in_json.get(k_endUpperValueKey, Json::nullValue);
                    CS_ASSERT(endUpperValue != Json::nullValue, "Must supply '" + std::string(k_endUpperValueKey) + "' in '" + std::string(k_randomCurveType) + "' property.");
                    CS_ASSERT(endUpperValue.isString(), "'" + std::string(k_endUpperValueKey) + "' in '" + std::string(k_randomCurveType) + "' property must be a string.");
                    
                    return ParticlePropertyUPtr<TType>(new RandomCurveParticleProperty<TType>(ParseValue<TType>(startLowerValue.asString()), ParseValue<TType>(startUpperValue.asString()),
                        ParseValue<TType>(endLowerValue.asString()), ParseValue<TType>(endUpperValue.asString()), ParseCurveFunction(curve.asString())));
                }
                //------------------------------------------------------------------------------
                /// Creates a new componentwise random curve particle property with the value
                /// described in the given json.
                ///
                /// @author Ian Copland
                ///
                /// @param The json value.
                ///
                /// @return The output particle property.
                //------------------------------------------------------------------------------
                template <typename TType> ParticlePropertyUPtr<TType> CreateComponentwiseRandomCurveProperty(const Json::Value& in_json)
                {
                    Json::Value curve = in_json.get(k_curveKey, Json::nullValue);
                    CS_ASSERT(curve != Json::nullValue, "Must supply '" + std::string(k_curveKey) + "' in '" + std::string(k_componentwiseRandomCurveType) + "' property.");
                    CS_ASSERT(curve.isString(), "'" + std::string(k_curveKey) + "' in '" + std::string(k_componentwiseRandomCurveType) + "' property must be a string.");
                    
                    Json::Value startLowerValue = in_json.get(k_startLowerValueKey, Json::nullValue);
                    CS_ASSERT(startLowerValue != Json::nullValue, "Must supply '" + std::string(k_startLowerValueKey) + "' in '" + std::string(k_componentwiseRandomCurveType) + "' property.");
                    CS_ASSERT(startLowerValue.isString(), "'" + std::string(k_startLowerValueKey) + "' in '" + std::string(k_componentwiseRandomCurveType) + "' property must be a string.");
                    
                    Json::Value startUpperValue = in_json.get(k_startUpperValueKey, Json::nullValue);
                    CS_ASSERT(startUpperValue != Json::nullValue, "Must supply '" + std::string(k_startUpperValueKey) + "' in '" + std::string(k_componentwiseRandomCurveType) + "' property.");
                    CS_ASSERT(startUpperValue.isString(), "'" + std::string(k_startUpperValueKey) + "' in '" + std::string(k_componentwiseRandomCurveType) + "' property must be a string.");
                    
                    Json::Value endLowerValue = in_json.get(k_endLowerValueKey, Json::nullValue);
                    CS_ASSERT(endLowerValue != Json::nullValue, "Must supply '" + std::string(k_endLowerValueKey) + "' in '" + std::string(k_componentwiseRandomCurveType) + "' property.");
                    CS_ASSERT(endLowerValue.isString(), "'" + std::string(k_endLowerValueKey) + "' in '" + std::string(k_componentwiseRandomCurveType) + "' property must be a string.");
                    
                    Json::Value endUpperValue = in_json.get(k_endUpperValueKey, Json::nullValue);
                    CS_ASSERT(endUpperValue != Json::nullValue, "Must supply '" + std::string(k_endUpperValueKey) + "' in '" + std::string(k_componentwiseRandomCurveType) + "' property.");
                    CS_ASSERT(endUpperValue.isString(), "'" + std::string(k_endUpperValueKey) + "' in '" + std::string(k_componentwiseRandomCurveType) + "' property must be a string.");
                    
                    return ParticlePropertyUPtr<TType>(new ComponentwiseRandomCurveParticleProperty<TType>(ParseValue<TType>(startLowerValue.asString()), ParseValue<TType>(startUpperValue.asString()),
                        ParseValue<TType>(endLowerValue.asString()), ParseValue<TType>(endUpperValue.asString()), ParseCurveFunction(curve.asString())));
                }
                //------------------------------------------------------------------------------
                /// Specialisation for parsing u32 values.
                ///
                /// @author Ian Copland
                ///
                /// @param The string to parse.
                ///
                /// @return The parsed value.
                //------------------------------------------------------------------------------
                template <> u32 ParseValue(const std::string& in_value);
                //------------------------------------------------------------------------------
                /// Specialisation for parsing s32 values.
                ///
                /// @author Ian Copland
                ///
                /// @param The string to parse.
                ///
                /// @return The parsed value.
                //------------------------------------------------------------------------------
                template <> s32 ParseValue(const std::string& in_value);
                //------------------------------------------------------------------------------
                /// Specialisation for parsing u64 values.
                ///
                /// @author Ian Copland
                ///
                /// @param The string to parse.
                ///
                /// @return The parsed value.
                //------------------------------------------------------------------------------
                template <> u64 ParseValue(const std::string& in_value);
                //------------------------------------------------------------------------------
                /// Specialisation for parsing s64 values.
                ///
                /// @author Ian Copland
                ///
                /// @param The string to parse.
                ///
                /// @return The parsed value.
                //------------------------------------------------------------------------------
                template <> s64 ParseValue(const std::string& in_value);
                //------------------------------------------------------------------------------
                /// Specialisation for parsing float values.
                ///
                /// @author Ian Copland
                ///
                /// @param The string to parse.
                ///
                /// @return The parsed value.
                //------------------------------------------------------------------------------
                template <> f32 ParseValue(const std::string& in_value);
                //------------------------------------------------------------------------------
                /// Specialisation for parsing Vector2 values.
                ///
                /// @author Ian Copland
                ///
                /// @param The string to parse.
                ///
                /// @return The parsed value.
                //------------------------------------------------------------------------------
                template <> Core::Vector2 ParseValue(const std::string& in_value);
                //------------------------------------------------------------------------------
                /// Specialisation for parsing Vector3 values.
                ///
                /// @author Ian Copland
                ///
                /// @param The string to parse.
                ///
                /// @return The parsed value.
                //------------------------------------------------------------------------------
                template <> Core::Vector3 ParseValue(const std::string& in_value);
                //------------------------------------------------------------------------------
                /// Specialisation for parsing Vector4 values.
                ///
                /// @author Ian Copland
                ///
                /// @param The string to parse.
                ///
                /// @return The parsed value.
                //------------------------------------------------------------------------------
                template <> Core::Vector4 ParseValue(const std::string& in_value);
                //------------------------------------------------------------------------------
                /// Specialisation for parsing Matrix3 values.
                ///
                /// @author Ian Copland
                ///
                /// @param The string to parse.
                ///
                /// @return The parsed value.
                //------------------------------------------------------------------------------
                template <> Core::Matrix3 ParseValue(const std::string& in_value);
                //------------------------------------------------------------------------------
                /// Specialisation for parsing Matrix4 values.
                ///
                /// @author Ian Copland
                ///
                /// @param The string to parse.
                ///
                /// @return The parsed value.
                //------------------------------------------------------------------------------
                template <> Core::Matrix4 ParseValue(const std::string& in_value);
                //------------------------------------------------------------------------------
                /// Specialisation for parsing Quaternion values.
                ///
                /// @author Ian Copland
                ///
                /// @param The string to parse.
                ///
                /// @return The parsed value.
                //------------------------------------------------------------------------------
                template <> Core::Quaternion ParseValue(const std::string& in_value);
                //------------------------------------------------------------------------------
                /// Specialisation for parsing Colour values.
                ///
                /// @author Ian Copland
                ///
                /// @param The string to parse.
                ///
                /// @return The parsed value.
                //------------------------------------------------------------------------------
                template <> Core::Colour ParseValue(const std::string& in_value);
            }
            
            //------------------------------------------------------------------------------
            //------------------------------------------------------------------------------
            template <typename TType> ParticlePropertyUPtr<TType> CreateProperty(const Json::Value& in_json)
            {
                //if it's a string value then default to a constant property.
                if (in_json.isNull() == false && in_json.isString() == true)
                {
                    return Impl::CreateConstantProperty<TType>(in_json.asString());
                }
                
                //otherwise get the type.
                else if (in_json.isNull() == false && in_json.isObject() == true)
                {
                    Json::Value typeValue = in_json.get(Impl::k_typeKey, Json::nullValue);
                    CS_ASSERT(typeValue != Json::nullValue, "A '" + std::string(Impl::k_typeKey) + "' must be supplied for a Particle Property.")
                    CS_ASSERT(typeValue.isString() == true, "'" + std::string(Impl::k_typeKey) + "' in a Particle Property must be a string.")
                    
                    std::string typeString = typeValue.asString();
                    Core::StringUtils::ToLowerCase(typeString);
                        
                    if (typeString == Impl::k_constantType)
                    {
                        return Impl::CreateConstantProperty<TType>(in_json);
                    }
                    else if (typeString == Impl::k_randomConstantType)
                    {
                        return Impl::CreateRandomConstantProperty<TType>(in_json);
                    }
                    else if (typeString == Impl::k_componentwiseRandomConstantType)
                    {
                        return Impl::CreateComponentwiseRandomConstantProperty<TType>(in_json);
                    }
                    else if (typeString == Impl::k_curveType)
                    {
                        return Impl::CreateCurveProperty<TType>(in_json);
                    }
                    else if (typeString == Impl::k_randomCurveType)
                    {
                        return Impl::CreateRandomCurveProperty<TType>(in_json);
                    }
                    else if (typeString == Impl::k_componentwiseRandomCurveType)
                    {
                        return Impl::CreateComponentwiseRandomCurveProperty<TType>(in_json);
                    }
                }
                
                CS_LOG_FATAL("Failed to create particle property.");
                return nullptr;
            }
        }
    }
}

#endif