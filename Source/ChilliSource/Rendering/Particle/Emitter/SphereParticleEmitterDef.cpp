//
//  SphereParticleEmitterDef.cpp
//  Chilli Source
//  Created by Ian Copland on 19/10/2014.
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

#include <ChilliSource/Rendering/Particle/Emitter/SphereParticleEmitterDef.h>

#include <ChilliSource/Rendering/Particle/Emitter/SphereParticleEmitter.h>

namespace ChilliSource
{
	namespace Rendering
	{
		CS_DEFINE_NAMEDTYPE(SphereParticleEmitterDef);
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		SphereParticleEmitterDef::SphereParticleEmitterDef(EmissionMode in_emissionMode, ParticlePropertyUPtr<f32> in_emissionRateProperty, ParticlePropertyUPtr<u32> in_particlesPerEmissionProperty,
			ParticlePropertyUPtr<f32> in_emissionChanceProperty, EmitFromType in_emitFromType, EmitDirectionType in_emitDirectionType, ParticlePropertyUPtr<f32> in_radiusProperty)
			: ParticleEmitterDef(in_emissionMode, std::move(in_emissionRateProperty), std::move(in_particlesPerEmissionProperty), std::move(in_emissionChanceProperty)), m_emitFromType(in_emitFromType),
			m_emitDirectionType(in_emitDirectionType), m_radiusProperty(std::move(in_radiusProperty))
		{
			CS_ASSERT(m_radiusProperty != nullptr, "Cannot create a Sphere Particle Emitter Def with a null 'radius' property.");
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		SphereParticleEmitterDef::SphereParticleEmitterDef(const CSCore::ParamDictionary& in_params, const LoadedDelegate& in_loadedDelegate)
			: ParticleEmitterDef(in_params)
		{
			//TODO: !?
			CS_LOG_FATAL("Unimplemented: SphereParticleEmitterDef::SphereParticleEmitterDef(const CSCore::ParamDictionary& in_params, const LoadedDelegate& in_loadedDelegate)");
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		bool SphereParticleEmitterDef::IsA(CSCore::InterfaceIDType in_interfaceId) const
		{
			return (SphereParticleEmitterDef::InterfaceID == in_interfaceId);
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		ParticleEmitterUPtr SphereParticleEmitterDef::CreateInstance(const CSCore::Entity* in_entity, Core::dynamic_array<Particle>* in_particleArray) const
		{
			return ParticleEmitterUPtr(new SphereParticleEmitter(in_entity, this, in_particleArray));
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		SphereParticleEmitterDef::EmitFromType SphereParticleEmitterDef::GetEmitFromType() const
		{
			return m_emitFromType;
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		SphereParticleEmitterDef::EmitDirectionType SphereParticleEmitterDef::GetEmitDirectionType() const
		{
			return m_emitDirectionType;
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		const ParticleProperty<f32>* SphereParticleEmitterDef::GetRadiusProperty() const
		{
			return m_radiusProperty.get();
		}
	}
}