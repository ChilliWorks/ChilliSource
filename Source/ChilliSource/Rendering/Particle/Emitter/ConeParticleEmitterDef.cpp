//
//  ConeParticleEmitterDef.cpp
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

#include <ChilliSource/Rendering/Particle/Emitter/ConeParticleEmitterDef.h>

#include <ChilliSource/Rendering/Particle/Emitter/ConeParticleEmitter.h>

namespace ChilliSource
{
	namespace Rendering
	{
		CS_DEFINE_NAMEDTYPE(ConeParticleEmitterDef);
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		ConeParticleEmitterDef::ConeParticleEmitterDef(EmissionMode in_emissionMode, ParticlePropertyUPtr<f32> in_emissionRateProperty, ParticlePropertyUPtr<u32> in_particlesPerEmissionProperty,
			ParticlePropertyUPtr<f32> in_emissionChanceProperty, EmitFromType in_emitFromType, EmitDirectionType in_emitDirectionType, ParticlePropertyUPtr<f32> in_radiusProperty, 
			ParticlePropertyUPtr<f32> in_angleProperty)
			: ParticleEmitterDef(in_emissionMode, std::move(in_emissionRateProperty), std::move(in_particlesPerEmissionProperty), std::move(in_emissionChanceProperty)), m_emitFromType(in_emitFromType),
			m_emitDirectionType(in_emitDirectionType), m_radiusProperty(std::move(in_radiusProperty)), m_angleProperty(std::move(in_angleProperty))
		{
			CS_ASSERT(m_radiusProperty != nullptr, "Cannot create a Sphere Particle Emitter Def with a null 'radius' property.");
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		ConeParticleEmitterDef::ConeParticleEmitterDef(const Core::ParamDictionary& in_params, const LoadedDelegate& in_loadedDelegate)
			: ParticleEmitterDef(in_params)
		{
			//TODO: !?
			CS_LOG_FATAL("Unimplemented: ConeParticleEmitterDef::ConeParticleEmitterDef(const Core::ParamDictionary& in_params, const LoadedDelegate& in_loadedDelegate)");
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		bool ConeParticleEmitterDef::IsA(Core::InterfaceIDType in_interfaceId) const
		{
			return (ParticleEmitterDef::InterfaceID == in_interfaceId || ConeParticleEmitterDef::InterfaceID == in_interfaceId);
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		ParticleEmitterUPtr ConeParticleEmitterDef::CreateInstance(Core::dynamic_array<Particle>* in_particleArray) const
		{
			return ParticleEmitterUPtr(new ConeParticleEmitter(this, in_particleArray));
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		ConeParticleEmitterDef::EmitFromType ConeParticleEmitterDef::GetEmitFromType() const
		{
			return m_emitFromType;
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		ConeParticleEmitterDef::EmitDirectionType ConeParticleEmitterDef::GetEmitDirectionType() const
		{
			return m_emitDirectionType;
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		const ParticleProperty<f32>* ConeParticleEmitterDef::GetRadiusProperty() const
		{
			return m_radiusProperty.get();
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		const ParticleProperty<f32>* ConeParticleEmitterDef::GetAngleProperty() const
		{
			return m_angleProperty.get();
		}
	}
}