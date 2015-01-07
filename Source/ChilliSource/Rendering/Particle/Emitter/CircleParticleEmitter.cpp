//
//  CircleParticleEmitter.cpp
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

#include <ChilliSource/Rendering/Particle/Emitter/CircleParticleEmitter.h>

#include <ChilliSource/Core/Math/Random.h>
#include <ChilliSource/Rendering/Particle/ParticleEffect.h>
#include <ChilliSource/Rendering/Particle/Emitter/CircleParticleEmitterDef.h>

namespace ChilliSource
{
	namespace Rendering
	{
		namespace
		{
			//----------------------------------------------------------------
			/// Generates a point within a circle with even distribution. 
			///
			/// @author Ian Copland
			///
			/// @return A random point in a unit circle.
			//----------------------------------------------------------------
			Core::Vector2 GeneratePointInUnitCircle()
			{
				f32 dist = std::sqrt(Core::Random::GenerateNormalised<f32>());
				return Core::Random::GenerateDirection2D<f32>() * dist;
			}
		}

		//----------------------------------------------------------------
		//----------------------------------------------------------------
		CircleParticleEmitter::CircleParticleEmitter(const ParticleEmitterDef* in_particleEmitter, Core::dynamic_array<Particle>* in_particleArray)
			: ParticleEmitter(in_particleEmitter, in_particleArray)
		{
			//Only the circle emitter def can create this, so this is safe.
			m_circleParticleEmitterDef = static_cast<const CircleParticleEmitterDef*>(in_particleEmitter);
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		void CircleParticleEmitter::GenerateEmission(f32 in_normalisedEmissionTime, Core::Vector3& out_position, Core::Vector3& out_direction)
		{
			f32 radius = m_circleParticleEmitterDef->GetRadiusProperty()->GenerateValue(in_normalisedEmissionTime);

			//calculate the position.
			switch (m_circleParticleEmitterDef->GetEmitFromType())
			{
			case CircleParticleEmitterDef::EmitFromType::k_inside:
				out_position = Core::Vector3(GeneratePointInUnitCircle() * radius, 0.0f);
				break;
			case CircleParticleEmitterDef::EmitFromType::k_surface:
				out_position = Core::Vector3(Core::Random::GenerateDirection2D<f32>() * radius, 0.0f);
				break;
			default:
				CS_LOG_FATAL("Invalid 'Emit From' type.");
				break;
			}

			//calculate the direction.
			switch (m_circleParticleEmitterDef->GetEmitDirectionType())
			{
			case CircleParticleEmitterDef::EmitDirectionType::k_random:
				out_direction = Core::Vector3(Core::Random::GenerateDirection2D<f32>(), 0.0f);
				break;
			case CircleParticleEmitterDef::EmitDirectionType::k_awayFromCentre:
				out_direction = Core::Vector3::Normalise(out_position);
				break;
			default:
				CS_LOG_FATAL("Invalid 'Emit Direction' type.");
				break;
			}
		}
	}
}
