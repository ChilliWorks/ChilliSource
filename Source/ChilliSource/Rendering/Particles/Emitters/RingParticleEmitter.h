//
//  RingParticleEmitter.h
//  moFloTest
//
//  Created by Scott Downie on 11/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _MO_FLO_RENDERING_PARTICLES_EMITTERS_RING_PARTICLE_EMITTER_H_
#define _MO_FLO_RENDERING_PARTICLES_EMITTERS_RING_PARTICLE_EMITTER_H_

#include <ChilliSource/Rendering/Particles/Emitters/ParticleEmitter.h>

namespace ChilliSource
{
    namespace Rendering
    {
        class CRingParticleEmitter : public CParticleEmitter
        {
        public:
            CRingParticleEmitter(const Core::ParamDictionary& inParams, const MaterialPtr &inpMaterial, CParticleComponent* inpComponent);
            
            //-----------------------------------------------------
            /// Create
            ///
            /// Create a new emitter from the given parameters
            ///
            /// @param Param dictionary
            /// @param Material
            /// @param Owning system
            /// @return Ownership of point emitter
            //-----------------------------------------------------
            static CParticleEmitter* Create(const Core::ParamDictionary& inParams, const MaterialPtr &inpMaterial, CParticleComponent* inpComponent);
            
        private:
			//-----------------------------------------------------
			/// Emit
			///
			/// Emit some new particles by pulling from the dead
			/// pool. Emission will depend on the number of
			/// particles emitted and the shape of the emitter
			///
			/// @param Particles
			/// @param Index of current particle
			//-----------------------------------------------------
			void Emit(Particle* inpParticles, u32 udwParticleIndex);
        private:
            
            f32 mfRadius;
        };
    }
}

#endif