//
//  ConeParticleEmitter.h
//  moFlo
//
//  Created by Andrew Glass on 02/11/2012.
//  Copyright (c) 2012 Tag Games. All rights reserved.
//

#ifndef _MO_FLO_RENDERING_PARTICLES_EMITTERS_CONE_PARTICLE_EMITTER_H_
#define _MO_FLO_RENDERING_PARTICLES_EMITTERS_CONE_PARTICLE_EMITTER_H_

#include <moFlo/Rendering/Particles/Emitters/ParticleEmitter.h>

namespace moFlo
{
	namespace Rendering
	{
        class CConeParticleEmitter : public CParticleEmitter
        {
        public:
            CConeParticleEmitter(const Core::ParamDictionary& inParams, const MaterialPtr &inpMaterial, CParticleComponent* inpComponent);
            
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
            
            f32 mfBoxArea;
            f32 mfBoxHeight;
        };
    }
}

#endif
