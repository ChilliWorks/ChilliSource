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
		ParticleEmitter::ParticleEmitter(const ParticleEmitterDef* in_emitterDef, Core::dynamic_array<Particle>* in_particleArray)
			: m_emitterDef(in_emitterDef), m_particleArray(in_particleArray)
		{
			CS_ASSERT(m_emitterDef != nullptr, "Cannot create particle emitter with null emitter def.");
			CS_ASSERT(m_particleArray != nullptr, "Cannot create particle emitter with null particle array.");
		}
		//----------------------------------------------
		//----------------------------------------------
		std::vector<u32> ParticleEmitter::TryEmit(f32 in_playbackTime, const Core::Vector3& in_emitterPosition, const Core::Vector3& in_emitterScale, const Core::Quaternion& in_emitterOrientation, bool in_interpolateEmission)
		{
			CS_ASSERT(in_playbackTime >= 0.0f, "Playback time cannot be below zero.");

			//If this is the first emission, then setup the correct entity positions.
			if (in_interpolateEmission == false)
			{
				m_emissionPosition = in_emitterPosition;
				m_emissionScale = in_emitterScale;
				m_emissionOrientation = in_emitterOrientation;
			}

			//wrap the emission timer if required.
			while (m_emissionTime > in_playbackTime)
			{
				m_emissionTime -= m_emitterDef->GetParticleEffect()->GetDuration();
                
				m_hasEmitted = false;
			}

			//emit based on emission mode.
			switch (m_emitterDef->GetEmissionMode())
			{
			case ParticleEmitterDef::EmissionMode::k_stream:
				return TryEmitStream(in_playbackTime, in_emitterPosition, in_emitterScale, in_emitterOrientation);
			case ParticleEmitterDef::EmissionMode::k_burst:
				return TryEmitBurst(in_playbackTime, in_emitterPosition, in_emitterScale, in_emitterOrientation);
			default:
				CS_LOG_FATAL("Invalid emission mode.");
				return std::vector<u32>();
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
		std::vector<u32> ParticleEmitter::TryEmitStream(f32 in_playbackTime, const Core::Vector3& in_emitterPosition, const Core::Vector3& in_emitterScale, const Core::Quaternion& in_emitterOrientation)
		{
			std::vector<u32> emittedParticles;

			const ParticleEffect* particleEffect = m_emitterDef->GetParticleEffect();

			//Get the time between emissions at this stage in the playback timer. Note that this doesn't take into account
			//the interpolation between the last frame and this, but should be close enough.
			const f32 normalisedPlaybackTime = in_playbackTime / particleEffect->GetDuration();
			const f32 timeBetweenEmissions = 1.0f / m_emitterDef->GetEmissionRateProperty()->GenerateValue(normalisedPlaybackTime);

			f32 prevEmissionTime = m_emissionTime;
			Core::Vector3 prevEntityPosition = m_emissionPosition;
			Core::Vector3 prevEntityScale = m_emissionScale;
			Core::Quaternion prevOrientation = m_emissionOrientation;

			f32 nextEmissionTime = prevEmissionTime + timeBetweenEmissions;
            
			while (nextEmissionTime <= in_playbackTime)
			{
				m_emissionTime = nextEmissionTime;
				f32 t = (m_emissionTime - prevEmissionTime) / (in_playbackTime - prevEmissionTime);
				m_emissionPosition = Core::Vector3::Lerp(prevEntityPosition, in_emitterPosition, t);
				m_emissionScale = Core::Vector3::Lerp(prevEntityScale, in_emitterScale, t);
				m_emissionOrientation = Core::Quaternion::Slerp(prevOrientation, in_emitterOrientation, t);

				f32 normalisedEmissionTime = m_emissionTime / particleEffect->GetDuration();
                while (normalisedEmissionTime < 0.0f)
                {
                    normalisedEmissionTime += 1.0f;
                }
                CS_ASSERT(normalisedEmissionTime >= 0.0f && normalisedEmissionTime <= 1.0f, "Invalid emission time.");
                
				u32 particlesPerEmission = m_emitterDef->GetParticlesPerEmissionProperty()->GenerateValue(normalisedEmissionTime);
				for (u32 i = 0; i < particlesPerEmission; ++i)
				{
					f32 chanceOfEmission = m_emitterDef->GetEmissionChanceProperty()->GenerateValue(normalisedEmissionTime);
					f32 random = Core::Random::GenerateNormalised<f32>();
					if (random <= chanceOfEmission)
					{
						Emit(normalisedEmissionTime, m_emissionPosition, m_emissionScale, m_emissionOrientation, emittedParticles);
					}
				}

				nextEmissionTime += timeBetweenEmissions;
			}

			return emittedParticles;
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		std::vector<u32> ParticleEmitter::TryEmitBurst(f32 in_playbackTime, const Core::Vector3& in_emitterPosition, const Core::Vector3& in_emitterScale, const Core::Quaternion& in_emitterOrientation)
		{
			std::vector<u32> emittedParticles;

			m_emissionTime = in_playbackTime;

			if (m_hasEmitted == false)
			{
				m_emissionPosition = in_emitterPosition;
				m_emissionScale = in_emitterScale;
				m_emissionOrientation = in_emitterOrientation;

				const f32 normalisedPlaybackTime = 0.0f;
				u32 particlesPerEmission = m_emitterDef->GetParticlesPerEmissionProperty()->GenerateValue(normalisedPlaybackTime);
				for (u32 i = 0; i < particlesPerEmission; ++i)
				{
					f32 chanceOfEmission = m_emitterDef->GetEmissionChanceProperty()->GenerateValue(normalisedPlaybackTime);
					f32 random = Core::Random::GenerateNormalised<f32>();
					if (random <= chanceOfEmission)
					{
						Emit(normalisedPlaybackTime, m_emissionPosition, m_emissionScale, m_emissionOrientation, emittedParticles);
					}
				}

				m_hasEmitted = true;
			}

			return emittedParticles;
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		void ParticleEmitter::Emit(f32 in_normalisedEmissionTime, const Core::Vector3& in_emissionPosition, const Core::Vector3& in_emissionScale, const Core::Quaternion& in_emissionOrientation, std::vector<u32>& inout_emittedParticles)
		{
			const ParticleEffect* particleEffect = m_emitterDef->GetParticleEffect();

			u32 particleIndex = m_nextParticleIndex++;
			Particle& particle = m_particleArray->at(particleIndex);
			if (m_nextParticleIndex >= particleEffect->GetMaxParticles())
			{
				m_nextParticleIndex = 0;
			}

			if (particle.m_isActive == false)
			{
				inout_emittedParticles.push_back(particleIndex);

				//Get the emission position and direction.
				Core::Vector3 localPosition;
				Core::Vector3 localDirection;
				GenerateEmission(in_normalisedEmissionTime, localPosition, localDirection);

				//calculate the local space properties.
				Core::Vector2 localScale = particleEffect->GetInitialScaleProperty()->GenerateValue(in_normalisedEmissionTime);
				f32 localRotation = particleEffect->GetInitialRotationProperty()->GenerateValue(in_normalisedEmissionTime);
				f32 localSpeed = particleEffect->GetInitialSpeedProperty()->GenerateValue(in_normalisedEmissionTime);

				//apply these in the correct simulation space.
				switch (particleEffect->GetSimulationSpace())
				{
					case ParticleEffect::SimulationSpace::k_world:
					{
						//transform the position into world space.
						const Core::Matrix4 worldTransform = Core::Matrix4::CreateTransform(in_emissionPosition, in_emissionScale, in_emissionOrientation);
						particle.m_position = localPosition * worldTransform;

						//we can't directly apply the emission scale to the particles as this would look strange as
						//the camera moved around an emitting entity with a non-uniform scale, so this works out a uniform
						//scale from the average of the components.
						f32 particleScaleFactor = (in_emissionScale.x + in_emissionScale.y + in_emissionScale.z) / 3.0f;
						particle.m_scale = localScale * particleScaleFactor;

						//transform the velocity into world space.
						particle.m_velocity = Core::Vector3::Rotate(((localDirection * localSpeed) * in_emissionScale), in_emissionOrientation);
						break;
					}
					case ParticleEffect::SimulationSpace::k_local:
					{
						particle.m_position = localPosition;
						particle.m_scale = localScale;
						particle.m_velocity = localDirection * localSpeed;
						break;
					}
					default:
					{
						CS_LOG_FATAL("Invalid simulation space.");
						break;
					}
				}

				//apply the remaining properties.
				particle.m_lifetime = particleEffect->GetLifetimeProperty()->GenerateValue(in_normalisedEmissionTime);
				particle.m_energy = particle.m_lifetime;
				particle.m_colour = particleEffect->GetInitialColourProperty()->GenerateValue(in_normalisedEmissionTime);
				particle.m_rotation = localRotation;
				particle.m_angularVelocity = particleEffect->GetInitialAngularVelocityProperty()->GenerateValue(in_normalisedEmissionTime);
				particle.m_isActive = true;
			}
		}
	}
}