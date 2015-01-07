//
//  AngularAccelerationParticleAffectorDef.cpp
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

#include <ChilliSource/Rendering/Particle/Affector/AngularAccelerationParticleAffectorDef.h>

#include <ChilliSource/Rendering/Particle/Affector/AngularAccelerationParticleAffector.h>
#include <ChilliSource/Rendering/Particle/Property/ParticlePropertyFactory.h>

namespace ChilliSource
{
	namespace Rendering
	{
		CS_DEFINE_NAMEDTYPE(AngularAccelerationParticleAffectorDef);
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		AngularAccelerationParticleAffectorDef::AngularAccelerationParticleAffectorDef(ParticlePropertyUPtr<f32> in_angularAccelerationProperty)
			: m_angularAccelerationProperty(std::move(in_angularAccelerationProperty))
		{
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		AngularAccelerationParticleAffectorDef::AngularAccelerationParticleAffectorDef(const Json::Value& in_paramsJson, const LoadedDelegate& in_asyncDelegate)
		{
			//Angular Acceleration
			Json::Value jsonValue = in_paramsJson.get("AngularAccelerationProperty", Json::nullValue);
			CS_ASSERT(jsonValue.isNull() == false, "No angular acceleration property provided.");
			m_angularAccelerationProperty = ParticlePropertyFactory::CreateProperty<f32>(jsonValue);

			//call the loaded delegate if required.
			if (in_asyncDelegate != nullptr)
			{
				in_asyncDelegate(this);
			}
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		bool AngularAccelerationParticleAffectorDef::IsA(Core::InterfaceIDType in_interfaceId) const
		{
			return (ParticleAffectorDef::InterfaceID == in_interfaceId || AngularAccelerationParticleAffectorDef::InterfaceID == in_interfaceId);
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		ParticleAffectorUPtr AngularAccelerationParticleAffectorDef::CreateInstance(Core::dynamic_array<Particle>* in_particleArray) const
		{
			return ParticleAffectorUPtr(new AngularAccelerationParticleAffector(this, in_particleArray));
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		const ParticleProperty<f32>* AngularAccelerationParticleAffectorDef::GetAngularAccelerationProperty() const
		{
			return m_angularAccelerationProperty.get();
		}
	}
}