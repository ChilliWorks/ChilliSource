//
//  Cone2DParticleEmitter.cpp
//  Chilli Source
//  Created by Ian Copland on 03/12/2014.
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

#include <ChilliSource/Rendering/Particle/Emitter/Cone2DParticleEmitter.h>

#include <ChilliSource/Core/Math/Random.h>
#include <ChilliSource/Rendering/Particle/ParticleEffect.h>
#include <ChilliSource/Rendering/Particle/Emitter/Cone2DParticleEmitterDef.h>

#include <cmath>

namespace ChilliSource
{
	namespace Rendering
	{
		namespace
		{
			//----------------------------------------------------------------
			/// Generates a 2D direction within the given angle range with even
			/// distribution.
			///
			/// @author Ian Copland
			///
			/// @param The angle.
			///
			/// @return The direction.
			//----------------------------------------------------------------
			Core::Vector2 GenerateDirectionWithinAngle(f32 in_angle)
			{
				f32 angle = Core::MathUtils::k_pi * 0.5f + Core::Random::GenerateNormalised<f32>() * in_angle - 0.5f * in_angle;
				Core::Vector2 direction(std::cos(angle), std::sin(angle));
				return direction;
			}
			//----------------------------------------------------------------
			/// Generates a direction with the given angle with even
			/// distribution.
			///
			/// @author Ian Copland
			///
			/// @param The angle.
			///
			/// @return The direction.
			//----------------------------------------------------------------
			Core::Vector2 GenerateDirectionWithAngle(f32 in_angle)
			{
				f32 angle = 0.0f;
				if (Core::Random::Generate<u32>(0, 1) == 0)
				{
					angle = Core::MathUtils::k_pi * 0.5f - 0.5f * in_angle;
				}
				else
				{
					angle = Core::MathUtils::k_pi * 0.5f + 0.5f * in_angle;
				}
				Core::Vector2 direction(std::cos(angle), std::sin(angle));
				return direction;
			}
			//----------------------------------------------------------------
			/// Generates a position in a unit 2D cone with the given angle, with
			/// even distribution.
			///
			/// @author Ian Copland
			///
			/// @param The angle.
			///
			/// @return The position.
			//----------------------------------------------------------------
			Core::Vector2 GeneratePositionInUnitCone2D(f32 in_angle)
			{
				f32 dist = std::sqrt(Core::Random::GenerateNormalised<f32>());
				return GenerateDirectionWithinAngle(in_angle) * dist;
			}
			//----------------------------------------------------------------
			/// Generates a position on a the surface of a unit 2D cone with the
			/// given angle, with even distribution.
			///
			/// @author Ian Copland
			///
			/// @param The angle.
			///
			/// @return The direction.
			//----------------------------------------------------------------
			Core::Vector2 GeneratePositionOnUnitCone2D(f32 in_angle)
			{
				f32 dist = std::sqrt(Core::Random::GenerateNormalised<f32>());
				return GenerateDirectionWithAngle(in_angle) * dist;
			}
		}

		//----------------------------------------------------------------
		//----------------------------------------------------------------
		Cone2DParticleEmitter::Cone2DParticleEmitter(const ParticleEmitterDef* in_particleEmitter, Core::dynamic_array<Particle>* in_particleArray)
			: ParticleEmitter(in_particleEmitter, in_particleArray)
		{
			//Only the sphere emitter def can create this, so this is safe.
			m_coneParticleEmitterDef = static_cast<const Cone2DParticleEmitterDef*>(in_particleEmitter);
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		void Cone2DParticleEmitter::GenerateEmission(f32 in_normalisedEmissionTime, Core::Vector3& out_position, Core::Vector3& out_direction)
		{
			f32 radius = m_coneParticleEmitterDef->GetRadiusProperty()->GenerateValue(in_normalisedEmissionTime);
			f32 angle = m_coneParticleEmitterDef->GetAngleProperty()->GenerateValue(in_normalisedEmissionTime);

			//calculate the position.
			switch (m_coneParticleEmitterDef->GetEmitFromType())
			{
			case Cone2DParticleEmitterDef::EmitFromType::k_inside:
				out_position = Core::Vector3(GeneratePositionInUnitCone2D(angle) * radius, 0.0f);
				break;
			case Cone2DParticleEmitterDef::EmitFromType::k_edge:
				out_position = Core::Vector3(GeneratePositionOnUnitCone2D(angle) * radius, 0.0f);
				break;
			case Cone2DParticleEmitterDef::EmitFromType::k_base:
				out_position = Core::Vector3::k_zero;
				break;
			default:
				CS_LOG_FATAL("Invalid 'Emit From' type.");
				break;
			}

			//calculate the direction.
			switch (m_coneParticleEmitterDef->GetEmitDirectionType())
			{
			case Cone2DParticleEmitterDef::EmitDirectionType::k_random:
				out_direction = Core::Vector3(GenerateDirectionWithinAngle(angle), 0.0f);
				break;
			case Cone2DParticleEmitterDef::EmitDirectionType::k_awayFromBase:
				if (out_position != Core::Vector3::k_zero)
				{
					out_direction = Core::Vector3(Core::Vector2::Normalise(out_position.XY()), 0.0f);
				}
				else
				{
					out_direction = Core::Vector3(GenerateDirectionWithinAngle(angle), 0.0f);
				}
				break;
			default:
				CS_LOG_FATAL("Invalid 'Emit Direction' type.");
				break;
			}
		}
	}
}