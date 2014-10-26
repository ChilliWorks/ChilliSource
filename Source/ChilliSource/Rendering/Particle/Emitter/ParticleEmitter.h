//
//  ParticleEmitter.h
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

#ifndef _CHILLISOURCE_RENDERING_PARTICLE_EMITTER_PARTICLEEMITTER_H_
#define _CHILLISOURCE_RENDERING_PARTICLE_EMITTER_PARTICLEEMITTER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Math/Vector3.h>
#include <ChilliSource/Core/Math/Quaternion.h>

#include <random>
#include <vector>

namespace ChilliSource
{
	namespace Rendering
	{
		//-----------------------------------------------------------------------
		/// The base class for all particle emitters. Particle emitters are 
		/// reponsible for decided when, where and in what direction a new 
		/// particle should be spawned. 
		///
		/// Particle emitters will be updated as part of a background task and 
		/// should not be accessed from other threads.
		///
		/// @author Ian Copland
		//-----------------------------------------------------------------------
		class ParticleEmitter
		{
		public:
			CS_DECLARE_NOCOPY(ParticleEmitter);
			//----------------------------------------------------------------
			/// Constructor.
			///
			/// @author Ian Copland
			///
			/// @param The entity the effect is attached to.
			/// @param The particle emitter definition.
			/// @param The particle array.
			//----------------------------------------------------------------
			ParticleEmitter(const Core::Entity* in_entity, const ParticleEmitterDef* in_particleEmitter, Core::dynamic_array<Particle>* in_particleArray);
			//----------------------------------------------------------------
			/// Tries to emit new particles if required. This will be called 
			/// as part of a background task.
			///
			/// @author Ian Copland
			/// 
			/// @param The current playback time of the particle effect.
			//----------------------------------------------------------------
			void TryEmit(f32 in_playbackTime);
			//----------------------------------------------------------------
			/// Destructor.
			///
			/// @author Ian Copland
			//----------------------------------------------------------------
			virtual ~ParticleEmitter() {};
		protected:
			//----------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @return the particle emitter definition.
			//----------------------------------------------------------------
			const ParticleEmitterDef* GetEmitterDef() const;
			//----------------------------------------------------------------
			/// Generates the position and direction of a new emission. These 
			/// values are in local space. This will be called as part of a 
			/// background task.
			///
			/// @author Ian Copland
			///
			/// @param The playback time of the emission.
			/// @param [Out] The generated position in local space.
			/// @param [Out] The generate direction in local space.
			//----------------------------------------------------------------
			virtual void GenerateEmission(f32 in_emissionTime, Core::Vector3& out_position, Core::Vector3& out_direction) = 0;
		private:
			//----------------------------------------------------------------
			/// Tries to emit new particles in stream mode.
			///
			/// @author Ian Copland
			/// 
			/// @param The current playback time of the particle effect.
			//----------------------------------------------------------------
			void TryEmitStream(f32 in_playbackTime);
			//----------------------------------------------------------------
			/// Tries to emit new particles in burst mode.
			///
			/// @author Ian Copland
			/// 
			/// @param The current playback time of the particle effect.
			//----------------------------------------------------------------
			void TryEmitBurst(f32 in_playbackTime);
			//----------------------------------------------------------------
			/// Emits a new particle if the next particle in the list is free
			/// to be emitted. 
			///
			/// @author Ian Copland
			/// 
			/// @param The world space position of the emitter at the time
			/// of emitter.
			/// @param The world space scale of the emitter at the time
			/// of emission.
			/// @param The world orientation of the emitter at the time of
			/// emission.
			/// @param The playback time of emission.
			//----------------------------------------------------------------
			void Emit(const Core::Vector3& in_emitterPosition, const Core::Vector3& in_emitterScale, const Core::Quaternion& in_emitterOrientation, f32 in_emissionTime);

			const Core::Entity* m_entity = nullptr;
			const ParticleEmitterDef* m_emitterDef = nullptr;
			Core::dynamic_array<Particle>* m_particleArray = nullptr;

			bool m_firstEmission = true;
			Core::Vector3 m_emissionPosition;
			Core::Vector3 m_emissionScale;
			Core::Quaternion m_emissionOrientation;
			f32 m_emissionTime = 0.0f;
			u32 m_nextParticleIndex = 0;
		};
	}
}

#endif