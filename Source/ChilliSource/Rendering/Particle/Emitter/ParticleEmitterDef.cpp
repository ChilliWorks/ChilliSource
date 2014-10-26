//
//  ParticleEmitterDef.cpp
//  Chilli Source
//  Created by Ian Copland on 06/10/2014.
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

#include <ChilliSource/Rendering/Particle/Emitter/ParticleEmitterDef.h>

#include <ChilliSource/Core/Container/ParamDictionary.h>

namespace ChilliSource
{
	namespace Rendering
	{
		CS_DEFINE_NAMEDTYPE(ParticleEmitterDef);
		//-----------------------------------------------
		//-----------------------------------------------
		ParticleEmitterDef::ParticleEmitterDef(EmissionMode in_emissionMode, ParticlePropertyUPtr<f32> in_emissionRateProperty, ParticlePropertyUPtr<u32> in_particlesPerEmissionProperty, ParticlePropertyUPtr<f32> in_emissionChanceProperty)
			: m_emissionMode(in_emissionMode), m_emissionRateProperty(std::move(in_emissionRateProperty)), m_particlesPerEmissionProperty(std::move(in_particlesPerEmissionProperty)), m_emissionChanceProperty(std::move(in_emissionChanceProperty))
		{
			CS_ASSERT(m_particlesPerEmissionProperty != nullptr, "Cannot create a Particle Emitter Def with a null 'particles per emission' property.");
			CS_ASSERT(m_emissionChanceProperty != nullptr, "Cannot create a Particle Emitter Def with a null 'emission chance' property.");

#ifdef CS_ENABLE_DEBUG
			if (m_emissionMode == EmissionMode::k_burst)
			{
				CS_ASSERT(m_emissionRateProperty == nullptr, "Cannot create a Particle Emitter Def with an 'emission rate' property if in burst mode.");
			}
			else
			{
				CS_ASSERT(m_emissionRateProperty != nullptr, "Cannot create a Particle Emitter Def with a null 'emission rate' property if in stream mode.");
			}
#endif
		}
		//-----------------------------------------------
		//-----------------------------------------------
		ParticleEmitterDef::ParticleEmitterDef(const CSCore::ParamDictionary& in_params)
		{
			//TODO: !?
			CS_LOG_FATAL("Unimplemented: ParticleEmitterDef::ParticleEmitterDef(const CSCore::ParamDictionary& in_params)");

			if (m_emissionMode == EmissionMode::k_burst)
			{
				m_emissionRateProperty.reset();
			}
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		const ParticleEffect* ParticleEmitterDef::GetParticleEffect() const
		{
			return m_particleEffect;
		}
		//-----------------------------------------------
		//-----------------------------------------------
		ParticleEmitterDef::EmissionMode ParticleEmitterDef::GetEmissionMode() const
		{
			return m_emissionMode;
		}
		//-----------------------------------------------
		//-----------------------------------------------
		const ParticleProperty<f32>* ParticleEmitterDef::GetEmissionRateProperty() const
		{
			return m_emissionRateProperty.get();
		}
		//-----------------------------------------------
		//-----------------------------------------------
		const ParticleProperty<u32>* ParticleEmitterDef::GetParticlesPerEmissionProperty() const
		{
			return m_particlesPerEmissionProperty.get();
		}
		//-----------------------------------------------
		//-----------------------------------------------
		const ParticleProperty<f32>* ParticleEmitterDef::GetEmissionChanceProperty() const
		{
			return m_emissionChanceProperty.get();
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		void ParticleEmitterDef::SetParticleEffect(const ParticleEffect* in_particleEffect)
		{
			m_particleEffect = in_particleEffect;
		}
	}
}