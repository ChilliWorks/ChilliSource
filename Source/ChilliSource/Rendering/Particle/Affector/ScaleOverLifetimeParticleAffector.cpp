//
//  ScaleOverLifetimeParticleAffector.cpp
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

#include <ChilliSource/Rendering/Particle/Affector/ScaleOverLifetimeParticleAffector.h>

#include <ChilliSource/Core/Math/MathUtils.h>
#include <ChilliSource/Rendering/Particle/Particle.h>
#include <ChilliSource/Rendering/Particle/ParticleEffect.h>
#include <ChilliSource/Rendering/Particle/Affector/ScaleOverLifetimeParticleAffectorDef.h>

namespace ChilliSource
{
	namespace Rendering
	{
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		ScaleOverLifetimeParticleAffector::ScaleOverLifetimeParticleAffector(const ParticleAffectorDef* in_affectorDef, Core::dynamic_array<Particle>* in_particleArray)
			: ParticleAffector(in_affectorDef, in_particleArray), m_particleScaleData(in_particleArray->size())
		{
			//This can only be created by the ScaleOverLifetimeParticleAffectorDef so this is safe.
			m_scaleOverLifetimeAffectorDef = static_cast<const ScaleOverLifetimeParticleAffectorDef*>(in_affectorDef);
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		void ScaleOverLifetimeParticleAffector::ActivateParticle(u32 in_index, f32 in_effectProgress)
		{
			CS_ASSERT(in_index >= 0 && in_index < m_particleScaleData.size(), "Index out of bounds!");

			ScaleData& scaleData = m_particleScaleData[in_index];
			const Particle& particle = GetParticleArray()->at(in_index);
			
			scaleData.m_initialScale = particle.m_scale;

			scaleData.m_targetScale = scaleData.m_initialScale * m_scaleOverLifetimeAffectorDef->GetScaleProperty()->GenerateValue(in_effectProgress);
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		void ScaleOverLifetimeParticleAffector::AffectParticles(f32 in_deltaTime, f32 in_effectProgress)
		{
			Core::dynamic_array<Particle>* particleArray = GetParticleArray();
			for (u32 i = 0; i < particleArray->size(); ++i)
			{
				Particle& particle = particleArray->at(i);
				ScaleData& scaleData = m_particleScaleData[i];

				f32 normalisedLifeProgress = 1.0f - (particle.m_energy / particle.m_lifetime);
				particle.m_scale = scaleData.m_initialScale + (scaleData.m_targetScale - scaleData.m_initialScale) * normalisedLifeProgress;
			}
		}
	}
}