//
//  ConeParticleEmitter.cpp
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

#include <ChilliSource/Rendering/Particle/Emitter/ConeParticleEmitter.h>

#include <ChilliSource/Core/Math/Random.h>
#include <ChilliSource/Rendering/Particle/ParticleEffect.h>
#include <ChilliSource/Rendering/Particle/Emitter/ConeParticleEmitterDef.h>

#include <cmath>

namespace ChilliSource
{
	namespace Rendering
	{
		namespace
		{
			//----------------------------------------------------------------
			/// Generates a direction within the given angle range with even
			/// distribution.
			///
			/// @author Ian Copland
			///
			/// @param The angle.
			///
			/// @return The direction.
			//----------------------------------------------------------------
			Core::Vector3 GenerateDirectionWithinAngle(f32 in_angle)
			{
				//get the y value that would ensure the top of the cone is a circle of unit radius.
				f32 y = 1.0f / tan(in_angle * 0.5f);

				//get a random point within the circle at the top of the cone. the square root of the
				//random distance is used to acheive even distribution.
				Core::Vector2 topDirection = Core::Random::GenerateDirection2D<f32>();
				f32 dist = std::sqrt(Core::Random::GenerateNormalised<f32>());

				//normalise this to get a direction vector.
				Core::Vector3 output(topDirection.x * dist, y, topDirection.y * dist);
				output.Normalise();
				return output;
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
			Core::Vector3 GenerateDirectionWithAngle(f32 in_angle)
			{
				//get the y value that would ensure the top of the cone is a circle of unit radius.
				f32 y = 1.0f / tan(in_angle * 0.5f);

				//get a random point on the surface the circle at the top of the cone.
				Core::Vector2 topDirection = Core::Random::GenerateDirection2D<f32>();

				//normalise this to get a direction vector.
				Core::Vector3 output(topDirection.x, y, topDirection.y);
				output.Normalise();
				return output;
			}
			//----------------------------------------------------------------
			/// Generates a position in a unit cone with the given angle, with
			/// even distribution.
			///
			/// @author Ian Copland
			///
			/// @param The angle.
			///
			/// @return The position.
			//----------------------------------------------------------------
			Core::Vector3 GeneratePositionInUnitCone(f32 in_angle)
			{
				const f32 oneOverThree = 1.0f / 3.0f;

				f32 dist = std::pow(Core::Random::GenerateNormalised<f32>(), oneOverThree);
				return GenerateDirectionWithinAngle(in_angle) * dist;
			}
			//----------------------------------------------------------------
			/// Generates a position on a the surface of a unit cone with the
			/// given angle, with even distribution.
			///
			/// @author Ian Copland
			///
			/// @param The angle.
			///
			/// @return The direction.
			//----------------------------------------------------------------
			Core::Vector3 GeneratePositionOnUnitCone(f32 in_angle)
			{
				const f32 oneOverThree = 1.0f / 3.0f;

				f32 dist = std::pow(Core::Random::GenerateNormalised<f32>(), oneOverThree);
				return GenerateDirectionWithAngle(in_angle) * dist;
			}
		}

		//----------------------------------------------------------------
		//----------------------------------------------------------------
		ConeParticleEmitter::ConeParticleEmitter(const ParticleEmitterDef* in_particleEmitter, Core::dynamic_array<Particle>* in_particleArray)
			: ParticleEmitter(in_particleEmitter, in_particleArray)
		{
			//Only the sphere emitter def can create this, so this is safe.
			m_coneParticleEmitterDef = static_cast<const ConeParticleEmitterDef*>(in_particleEmitter);
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		void ConeParticleEmitter::GenerateEmission(f32 in_normalisedEmissionTime, Core::Vector3& out_position, Core::Vector3& out_direction)
		{
			f32 radius = m_coneParticleEmitterDef->GetRadiusProperty()->GenerateValue(in_normalisedEmissionTime);
			f32 angle = m_coneParticleEmitterDef->GetAngleProperty()->GenerateValue(in_normalisedEmissionTime);

			//calculate the position.
			switch (m_coneParticleEmitterDef->GetEmitFromType())
			{
			case ConeParticleEmitterDef::EmitFromType::k_inside:
				out_position = GeneratePositionInUnitCone(angle) * radius;
				break;
			case ConeParticleEmitterDef::EmitFromType::k_surface:
				out_position = GeneratePositionOnUnitCone(angle) * radius;
				break;
			case ConeParticleEmitterDef::EmitFromType::k_base:
				out_position = Core::Vector3::k_zero;
				break;
			default:
				CS_LOG_FATAL("Invalid 'Emit From' type.");
				break;
			}

			//calculate the direction.
			switch (m_coneParticleEmitterDef->GetEmitDirectionType())
			{
			case ConeParticleEmitterDef::EmitDirectionType::k_random:
				out_direction = GenerateDirectionWithinAngle(angle);
				break;
			case ConeParticleEmitterDef::EmitDirectionType::k_awayFromBase:
				if (out_position != Core::Vector3::k_zero)
				{
					out_direction = Core::Vector3::Normalise(out_position);
				}
				else
				{
					out_direction = GenerateDirectionWithinAngle(angle);
				}
				break;
			default:
				CS_LOG_FATAL("Invalid 'Emit Direction' type.");
				break;
			}
		}
	}
}