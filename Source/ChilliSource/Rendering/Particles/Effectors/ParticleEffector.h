/*
 *  ParticleEffector.h
 *  moFloTest
 *
 *  Created by Scott Downie on 17/01/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#ifndef _MO_FLO_RENDERING__PARTICLES_PARTICLE_EFFECTOR_H_
#define _MO_FLO_RENDERING__PARTICLES_PARTICLE_EFFECTOR_H_

#include <ChilliSource/ChilliSource.h>

namespace ChilliSource
{
	namespace Rendering
	{
		//================================================================
		/// Particle Effector - Interface
		///
		/// Applied to a particle emitter to effect the particle over time
		//================================================================
		class ParticleEffector
		{
		public:
			virtual ~ParticleEffector(){}
            //-----------------------------------------------------
            /// Init
            ///
            /// The effector will initialise the particles to its
            /// liking 
            ///
            /// @param Particle to intialise
            //-----------------------------------------------------
            virtual void Init(Particle& in_particle) = 0;
            //-----------------------------------------------------
            /// Apply
            ///
            /// The effector will apply itself to the given 
            /// particles
            ///
            /// @param Particle to effect
            /// @param Time between frames
            //-----------------------------------------------------
			virtual void Apply(Particle& in_particle, f32 infDt) = 0;
            //-----------------------------------------------------
            /// Update
            ///
            /// The effector will update itself
            ///
            /// @param Time between frames
            //-----------------------------------------------------
            virtual void Update(f32 infDt) {}
            //-----------------------------------------------------
            /// Set Active Energy Level
            ///
            /// The energy level at which the effector becomes
            /// active
            ///
            /// @param Energy level form 1.0 - 0.0
            //-----------------------------------------------------
            void SetActiveEnergyLevel(f32 infEnergy){mfActiveEnergyLevel = infEnergy;}
			//-----------------------------------------------------
            /// Get Active Energy Level
            ///
            /// The energy level at which the effector becomes
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