//
//  ParticleEmitter.cpp
//  Chilli Source
//  Created by Ian Copland on 10/10/2014.
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

#include <ChilliSource/Rendering/Particle/Emitter/ParticleEmitter.h>

#include <ChilliSource/Core/Container/dynamic_array.h>
#include <ChilliSource/Core/Entity/Entity.h>
#include <ChilliSource/Core/Entity/Transform.h>
#include <ChilliSource/Core/Math/Random.h>
#include <ChilliSource/Rendering/Particle/Particle.h>
#include <ChilliSource/Rendering/Particle/ParticleEffect.h>
#include <ChilliSource/Rendering/Particle/Emitter/ParticleEmitterDef.h>

#include <algorithm>

namespace ChilliSource
{
	namespace Rendering
	{
		//----------------------------------------------
		//----------------------------------------------
		ParticleEmitter::ParticleEmitter(const CSCore::Entity* in_entity, const ParticleEmitterDef* in_emitterDef, Core::dynamic_array<Particle>* in_particleArray)
			: m_entity(in_entity), m_emitterDef(in_emitterDef), m_particleArray(in_particleArray)
		{
			CS_ASSERT(m_entity != nullptr, "Cannot create particle emitter with null entity.");
			CS_ASSERT(m_emitterDef != nullptr, "Cannot create particle emitter with null emitter def.");
			CS_ASSERT(m_particleArray != nullptr, "Cannot create particle emitter with null particle array.");
		}
		//----------------------------------------------
		//----------------------------------------------
		void ParticleEmitter::TryEmit(f32 in_playbackTime)
		{
			CS_ASSERT(in_playbackTime >= 0.0f, "Playback time cannot be below zero.");

			//If this is the first emission, then setup the correct entity positions.
			if (m_firstEmission == true)
			{
				m_emissionPosition = m_entity->GetTransform().GetWorldPosition();
				m_emissionScale = m_entity->GetTransform().GetWorldScale();
				m_emissionOrientation = m_entity->GetTransform().GetWorldOrientation();
			}

			//wrap the emission timer if required.
			while (m_emissionTime > in_playbackTime)
			{
				m_emissionTime -= m_emitterDef->GetParticleEffect()->GetDuration();
			}

			//emit based on emission mode.
			switch (m_emitterDef->GetEmissionMode())
			{
			case ParticleEmitterDef::EmissionMode::k_stream:
				TryEmitStream(in_playbackTime);
				break;
			case ParticleEmitterDef::EmissionMode::k_burst:
				TryEmitBurst(in_playbackTime);
				break;
			}
		}
		//----------------------------------------------
		//----------------------------------------------
		const ParticleEmitterDef* ParticleEmitter::GetEmitterDef() const
		{
			return m_emitterDef;
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		void ParticleEmitter::TryEmitStream(f32 in_playbackTime)
		{
			const ParticleEffect* particleEffect = m_emitterDef->GetParticleEffect();

			//Get the time between emissions at this stage in the playback timer. Note that this doesn't take into account
			//the interpolation between the last frame and this, but should be close enough.
			const f32 normalisedPlaybackTime = in_playbackTime / particleEffect->GetDuration();
			const f32 timeBetweenEmissions = 1.0f / m_emitterDef->GetEmissionRateProperty()->GenerateValue(normalisedPlaybackTime);

			f32 prevEmissionTime = m_emissionTime;
			CSCore::Vector3 prevEntityPosition = m_emissionPosition;
			CSCore::Vector3 prevEntityScale = m_emissionScale;
			CSCore::Quaternion prevOrientation = m_emissionOrientation;

			f32 nextEmissionTime = prevEmissionTime + timeBetweenEmissions;
			while (nextEmissionTime <= in_playbackTime)
			{
				m_emissionTime = nextEmissionTime;
				f32 t = (m_emissionTime - prevEmissionTime) / (in_playbackTime - prevEmissionTime);
				m_emissionPosition = CSCore::Vector3::Lerp(prevEntityPosition, m_entity->GetTransform().GetWorldPosition(), t);
				m_emissionScale = CSCore::Vector3::Lerp(prevEntityScale, m_entity->GetTransform().GetWorldScale(), t);
				m_emissionOrientation = CSCore::Quaternion::Slerp(prevOrientation, m_entity->GetTransform().GetWorldOrientation(), t);

				const f32 normalisedPlaybackTime = m_emissionTime / particleEffect->GetDuration();
				u32 particlesPerEmission = m_emitterDef->GetParticlesPerEmissionProperty()->GenerateValue(normalisedPlaybackTime);
				for (u32 i = 0; i < particlesPerEmission; ++i)
				{
					f32 chanceOfEmission = m_emitterDef->GetEmissionChanceProperty()->GenerateValue(normalisedPlaybackTime);

					f32 random = Random::GenerateReal<f32>();
					if (random <= chanceOfEmission)
					{
						Emit(m_emissionPosition, m_emissionScale, m_emissionOrientation, m_emissionTime);
					}
				}

				nextEmissionTime += timeBetweenEmissions;
			}
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		void ParticleEmitter::TryEmitBurst(f32 in_playbackTime)
		{
			//TODO:
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		void ParticleEmitter::Emit(const CSCore::Vector3& in_emitterPosition, const CSCore::Vector3& in_emitterScale, const CSCore::Quaternion& in_emitterOrientation, f32 in_emissionTime)
		{
			const ParticleEffect* particleEffect = m_emitterDef->GetParticleEffect();

			Particle& particle = m_particleArray->at(m_nextParticleIndex++);
			if (m_nextParticleIndex >= particleEffect->GetMaxParticles())
			{
				m_nextParticleIndex = 0;
			}

			if (particle.m_isActive == false)
			{
				//Get the emission position and direction.
				CSCore::Vector3 localPosition;
				CSCore::Vector3 localDirection;
				GenerateEmission(in_emissionTime, localPosition, localDirection);

				const f32 normalisedPlaybackTime = m_emissionTime / particleEffect->GetDuration();

				//calculate the local space properties.
				CSCore::Vector2 localScale = particleEffect->GetInitialScaleProperty()->GenerateValue(normalisedPlaybackTime);
				f32 localRotation = particleEffect->GetInitialRotationProperty()->GenerateValue(normalisedPlaybackTime);
				f32 localSpeed = particleEffect->GetInitialSpeedProperty()->GenerateValue(normalisedPlaybackTime);

				//apply these in the correct simulation space.
				switch (particleEffect->GetSimulationSpace())
				{
				case ParticleEffect::SimulationSpace::k_world:
					//TODO:
					break;
				case ParticleEffect::SimulationSpace::k_local:
					particle.m_position = localPosition;
					particle.m_scale = localScale;
					particle.m_rotation = localRotation;
					particle.m_velocity = localDirection * localSpeed;
					break;
				default:
					CS_LOG_FATAL("Invalid simulation space.");
					break;
				}

				//apply the remaining properties.
				particle.m_lifetime = particleEffect->GetLifetimeProperty()->GenerateValue(normalisedPlaybackTime);
				particle.m_energy = particle.m_lifetime;
				particle.m_colour = particleEffect->GetInitialColourProperty()->GenerateValue(normalisedPlaybackTime);
				particle.m_angularVelocity = particleEffect->GetInitialAngularVelocityProperty()->GenerateValue(normalisedPlaybackTime);
				particle.m_isActive = true;

				//TODO: Initialise affectors.

				//TODO: Handle initialising drawable.
			}
		}
	}
}