//
//  ParticleAffector.h
//  Chilli Source
//  Created by Ian Copland on 21/10/2014.
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

#ifndef _CHILLISOURCE_RENDERING_PARTICLE_AFFECTOR_PARTICLEAFFECTOR_H_
#define _CHILLISOURCE_RENDERING_PARTICLE_AFFECTOR_PARTICLEAFFECTOR_H_

#include <ChilliSource/ChilliSource.h>

namespace ChilliSource
{
	namespace Rendering
	{
		//---------------------------------------------------------------------
		/// The base class for all particle affectors. Particle affectors are 
		/// reponsible for applying any sort of effect to particles every update
		/// frame.
		///
		/// Particle affectors will be updated as part of a background task and 
		/// should not be accessed from other threads.
		//---------------------------------------------------------------------
		class ParticleAffector
		{
		public:
			CS_DECLARE_NOCOPY(ParticleAffector);
			//----------------------------------------------------------------
			/// Constructor.
			///
			/// @author Ian Copland
			///
			/// @param The particle affector definition.
			/// @param The particle array.
			//----------------------------------------------------------------
			ParticleAffector(const ParticleAffectorDef* in_affectorDef, Core::dynamic_array<Particle>* in_particleArray);
			//----------------------------------------------------------------
			/// Activates the particle with the given index.
			///
			/// This will be called on a background thread.
			///
			/// @author Ian Copland
			///
			/// @param The index of the particle to activate.
            /// @param The current normalised (0.0 to 1.0) progress through
            /// playback of the particle effect.
			//----------------------------------------------------------------
			virtual void ActivateParticle(u32 in_index, f32 in_effectProgress) = 0;
			//----------------------------------------------------------------
			/// Applies the affect to each of the active particles.
			///
			/// This will be called on a background thread.
			///
			/// @author Ian Copland
			///
			/// @param The delta time.
            /// @param The current normalised (0.0 to 1.0) progress through
            /// playback of the particle effect.
			//----------------------------------------------------------------
			virtual void AffectParticles(f32 in_deltaTime, f32 in_effectProgress) = 0;
			//----------------------------------------------------------------
			/// Destructor
			///
			/// @author Ian Copland
			//----------------------------------------------------------------
			virtual ~ParticleAffector() {};
		protected:
			//----------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @return The particle affector definition
			//----------------------------------------------------------------
			const ParticleAffectorDef* GetAffectorDef() const;
			//----------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @return The particle array.
			//----------------------------------------------------------------
			Core::dynamic_array<Particle>* GetParticleArray() const;
		private:

			const ParticleAffectorDef* m_affectorDef = nullptr;
			Core::dynamic_array<Particle>* m_particleArray = nullptr;
		};
	}
}

#endif