//
//  ParticleAffector.h
//  Chilli Source
//  Created by Scott Downie on 17/01/2011.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2011 Tag Games Limited
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

#ifndef _MO_FLO_RENDERING__PARTICLES_PARTICLE_AFFECTOR_H_
#define _MO_FLO_RENDERING__PARTICLES_PARTICLE_AFFECTOR_H_

#include <ChilliSource/ChilliSource.h>

namespace ChilliSource
{
	namespace Rendering
	{
		//================================================================
		/// Particle Affector - Interface
		///
		/// Applied to a particle emitter to effect the particle over time
		//================================================================
		class ParticleAffector
		{
		public:
			virtual ~ParticleAffector(){}
            //-----------------------------------------------------
            /// Init
            ///
            /// The affector will initialise the particles to its
            /// liking 
            ///
            /// @param Particle to intialise
            //-----------------------------------------------------
            virtual void Init(Particle& in_particle) = 0;
            //-----------------------------------------------------
            /// Apply
            ///
            /// The affector will apply itself to the given 
            /// particles
            ///
            /// @param Particle to effect
            /// @param Time between frames
            //-----------------------------------------------------
			virtual void Apply(Particle& in_particle, f32 infDt) = 0;
            //-----------------------------------------------------
            /// Update
            ///
            /// The affector will update itself
            ///
            /// @param Time between frames
            //-----------------------------------------------------
            virtual void Update(f32 infDt) {}
            //-----------------------------------------------------
            /// Set Active Energy Level
            ///
            /// The energy level at which the affector becomes
            /// active
            ///
            /// @param Energy level form 1.0 - 0.0
            //-----------------------------------------------------
            void SetActiveEnergyLevel(f32 infEnergy){mfActiveEnergyLevel = infEnergy;}
			//-----------------------------------------------------
            /// Get Active Energy Level
            ///
            /// The energy level at which the affector becomes
            /// active
            ///
            /// @return Energy level form 1.0 - 0.0
            //-----------------------------------------------------
            f32 GetActiveEnergyLevel() const {return mfActiveEnergyLevel;}
			
        protected:
            f32 mfActiveEnergyLevel;
		};
	}
}

#endif