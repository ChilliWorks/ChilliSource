//
//  AccelerationParticleAffectorDef.cpp
//  Chilli Source
//  Created by Ian Copland on 03/11/2014.
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

#include <ChilliSource/Rendering/Particle/Affector/AccelerationParticleAffectorDef.h>

#include <ChilliSource/Rendering/Particle/Affector/AccelerationParticleAffector.h>
#include <ChilliSource/Rendering/Particle/Property/ParticlePropertyFactory.h>

namespace ChilliSource
{
	namespace Rendering
	{
		CS_DEFINE_NAMEDTYPE(AccelerationParticleAffectorDef);
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		AccelerationParticleAffectorDef::AccelerationParticleAffectorDef(ParticlePropertyUPtr<Core::Vector3> in_accelerationProperty)
			: m_accelerationProperty(std::move(in_accelerationProperty))
		{
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		AccelerationParticleAffectorDef::AccelerationParticleAffectorDef(const Json::Value& in_paramsJson, const LoadedDelegate& in_asyncDelegate)
		{
			//Acceleration
			Json::Value jsonValue = in_paramsJson.get("AccelerationProperty", Json::nullValue);
			CS_ASSERT(jsonValue.isNull() == false, "No acceleration property provided.");
			m_accelerationProperty = ParticlePropertyFactory::CreateProperty<Core::Vector3>(jsonValue);

			//call the loaded delegate if required.
			if (in_asyncDelegate != nullptr)
			{
				in_asyncDelegate(this);
			}
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		bool AccelerationParticleAffectorDef::IsA(Core::InterfaceIDType in_interfaceId) const
		{
			return (ParticleAffectorDef::InterfaceID == in_interfaceId || AccelerationParticleAffectorDef::InterfaceID == in_interfaceId);
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		ParticleAffectorUPtr AccelerationParticleAffectorDef::CreateInstance(Core::dynamic_array<Particle>* in_particleArray) const
		{
			return ParticleAffectorUPtr(new AccelerationParticleAffector(this, in_particleArray));
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		const ParticleProperty<Core::Vector3>* AccelerationParticleAffectorDef::GetAccelerationProperty() const
		{
			return m_accelerationProperty.get();
		}
	}
}