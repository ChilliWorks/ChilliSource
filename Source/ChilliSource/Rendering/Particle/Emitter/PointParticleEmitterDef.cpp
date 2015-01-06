//
//  PointParticleEmitterDef.cpp
//  Chilli Source
//  Created by Ian Copland on 02/11/2014.
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

#include <ChilliSource/Rendering/Particle/Emitter/PointParticleEmitterDef.h>

#include <ChilliSource/Rendering/Particle/Emitter/PointParticleEmitter.h>

namespace ChilliSource
{
	namespace Rendering
	{
		CS_DEFINE_NAMEDTYPE(PointParticleEmitterDef);
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		PointParticleEmitterDef::PointParticleEmitterDef(EmissionMode in_emissionMode, ParticlePropertyUPtr<f32> in_emissionRateProperty, ParticlePropertyUPtr<u32> in_particlesPerEmissionProperty,
			ParticlePropertyUPtr<f32> in_emissionChanceProperty)
			: ParticleEmitterDef(in_emissionMode, std::move(in_emissionRateProperty), std::move(in_particlesPerEmissionProperty), std::move(in_emissionChanceProperty))
		{
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		PointParticleEmitterDef::PointParticleEmitterDef(const Json::Value& in_paramsJson, const LoadedDelegate& in_loadedDelegate)
			: ParticleEmitterDef(in_paramsJson)
		{
			//call the loaded delegate if required.
			if (in_loadedDelegate != nullptr)
			{
				in_loadedDelegate(this);
			}
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		bool PointParticleEmitterDef::IsA(Core::InterfaceIDType in_interfaceId) const
		{
			return (ParticleEmitterDef::InterfaceID == in_interfaceId || PointParticleEmitterDef::InterfaceID == in_interfaceId);
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		ParticleEmitterUPtr PointParticleEmitterDef::CreateInstance(Core::dynamic_array<Particle>* in_particleArray) const
		{
			return ParticleEmitterUPtr(new PointParticleEmitter(this, in_particleArray));
		}
	}
}