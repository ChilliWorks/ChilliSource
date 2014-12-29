//
//  ParticlePropertyFactory.cpp
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

#include <ChilliSource/Rendering/Particle/Property/ParticlePropertyFactory.h>

#include <ChilliSource/Rendering/Particle/Property/ParticlePropertyCurveFunctions.h>

#include <ChilliSource/Core/Delegate/MakeDelegate.h>
#include <ChilliSource/Core/String/StringParser.h>

namespace ChilliSource
{
	namespace Rendering
	{
		namespace ParticlePropertyFactory
		{
            namespace Impl
            {
                namespace
                {
                    const char k_linearCurveName[] = "linear";
                    const char k_easeInQuadCurveName[] = "easeinquad";
                    const char k_easeOutQuadCurveName[] = "easeoutquad";
                    const char k_smoothStepCurveName[] = "smoothstep";
                    const char k_linearPingPongCurveName[] = "linearpingpong";
                    const char k_easeInQuadPingPongCurveName[] = "easeinquadpingpong";
                    const char k_easeOutQuadPingPongCurveName[] = "easeoutquadpingpong";
                    const char k_smoothStepPingPongCurveName[] = "smoothsteppingpong";
                }
                
                //------------------------------------------------------------------------------
                //------------------------------------------------------------------------------
                std::function<f32(f32)> ParseCurveFunction(const std::string& in_curveName)
                {
                    std::string nameLower = in_curveName;
                    Core::StringUtils::ToLowerCase(nameLower);
                    
                    if (nameLower == k_linearCurveName)
                    {
                        return Core::MakeDelegate(&ParticlePropertyCurveFunctions::Linear);
                    }
                    else if (nameLower == k_easeInQuadCurveName)
                    {
                        return Core::MakeDelegate(&ParticlePropertyCurveFunctions::EaseInQuad);
                    }
                    else if (nameLower == k_easeOutQuadCurveName)
                    {
                        return Core::MakeDelegate(&ParticlePropertyCurveFunctions::EaseOutQuad);
                    }
                    else if (nameLower == k_smoothStepCurveName)
                    {
                        return Core::MakeDelegate(&ParticlePropertyCurveFunctions::SmoothStep);
                    }
                    else if (nameLower == k_linearPingPongCurveName)
                    {
                        return Core::MakeDelegate(&ParticlePropertyCurveFunctions::LinearPingPong);
                    }
                    else if (nameLower == k_easeInQuadPingPongCurveName)
                    {
                        return Core::MakeDelegate(&ParticlePropertyCurveFunctions::EaseInQuadPingPong);
                    }
                    else if (nameLower == k_easeOutQuadPingPongCurveName)
                    {
                        return Core::MakeDelegate(&ParticlePropertyCurveFunctions::EaseOutQuadPingPong);
                    }
                    else if (nameLower == k_smoothStepPingPongCurveName)
                    {
                        return Core::MakeDelegate(&ParticlePropertyCurveFunctions::SmoothStepPingPong);
                    }
                    
                    CS_LOG_FATAL("Invalid curve function: " + in_curveName);
                    return nullptr;
                }
                //------------------------------------------------------------------------------
                //------------------------------------------------------------------------------
                template <> u32 ParseValue(const std::string& in_value)
                {
                    return Core::ParseU32(in_value);
                }
                //------------------------------------------------------------------------------
                //------------------------------------------------------------------------------
                template <> s32 ParseValue(const std::string& in_value)
                {
                    return Core::ParseS32(in_value);
                }
                //------------------------------------------------------------------------------
                //------------------------------------------------------------------------------
                template <> u64 ParseValue(const std::string& in_value)
                {
                    return Core::ParseU64(in_value);
                }
                //------------------------------------------------------------------------------
                //------------------------------------------------------------------------------
                template <> s64 ParseValue(const std::string& in_value)
                {
                    return Core::ParseS64(in_value);
                }
                //------------------------------------------------------------------------------
                //------------------------------------------------------------------------------
                template <> f32 ParseValue(const std::string& in_value)
                {
                    return Core::ParseF32(in_value);
                }
                //------------------------------------------------------------------------------
                //------------------------------------------------------------------------------
                template <> Core::Vector2 ParseValue(const std::string& in_value)
                {
                    return Core::ParseVector2(in_value);
                }
                //------------------------------------------------------------------------------
                //------------------------------------------------------------------------------
                template <> Core::Vector3 ParseValue(const std::string& in_value)
                {
                    return Core::ParseVector3(in_value);
                }
                //------------------------------------------------------------------------------
                //------------------------------------------------------------------------------
                template <> Core::Vector4 ParseValue(const std::string& in_value)
                {
                    return Core::ParseVector4(in_value);
                }
                //------------------------------------------------------------------------------
                //------------------------------------------------------------------------------
                template <> Core::Matrix3 ParseValue(const std::string& in_value)
                {
                    return Core::ParseMatrix3(in_value);
                }
                //------------------------------------------------------------------------------
                //------------------------------------------------------------------------------
                template <> Core::Matrix4 ParseValue(const std::string& in_value)
                {
                    return Core::ParseMatrix4(in_value);
                }
                //------------------------------------------------------------------------------
                //------------------------------------------------------------------------------
                template <> Core::Quaternion ParseValue(const std::string& in_value)
                {
                    return Core::ParseQuaternion(in_value);
                }
                //------------------------------------------------------------------------------
                //------------------------------------------------------------------------------
                template <> Core::Colour ParseValue(const std::string& in_value)
                {
                    return Core::ParseColour(in_value);
                }
            }
		}
	}
}