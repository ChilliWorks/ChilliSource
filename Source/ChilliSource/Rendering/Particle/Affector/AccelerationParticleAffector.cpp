//
//  AccelerationParticleAffector.cpp
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

#include <ChilliSource/Rendering/Particle/Affector/AccelerationParticleAffector.h>

#include <ChilliSource/Core/Math/MathUtils.h>
#include <ChilliSource/Rendering/Particle/Particle.h>
#include <ChilliSource/Rendering/Particle/ParticleEffect.h>
#include <ChilliSource/Rendering/Particle/Affector/AccelerationParticleAffectorDef.h>

namespace ChilliSource
{
	namespace Rendering
	{
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		AccelerationParticleAffector::AccelerationParticleAffector(const ParticleAffectorDef* in_affectorDef, Core::dynamic_array<Particle>* in_particleArray)
			: ParticleAffector(in_affectorDef, in_particleArray), m_particleAcceleration(in_particleArray->size())
		{
			//This can only be created by the AccelerationParticleAffectorDef so this is safe.
			m_accelerationAffectorDef = static_cast<const AccelerationParticleAffectorDef*>(in_affectorDef);
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		void AccelerationParticleAffector::ActivateParticle(u32 in_index, f32 in_effectProgress)
		{
			CS_ASSERT(in_index >= 0 && in_index < m_particleAcceleration.size(), "Index out of bounds!");

			m_particleAcceleration[in_index] = m_accelerationAffectorDef->GetAccelerationProperty()->GenerateValue(in_effectProgress);
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		void AccelerationParticleAffector::AffectParticles(f32 in_deltaTime, f32 in_effectProgress)
		{
			Core::dynamic_array<Particle>* particleArray = GetParticleArray();
			for (u32 i = 0; i < particleArray->size(); ++i)
			{
				Particle& particle = particleArray->at(i);
				Core::Vector3& acceleration = m_particleAcceleration[i];

				particle.m_velocity += acceleration * in_deltaTime;
			}
		}
	}
}