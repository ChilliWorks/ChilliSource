//
//  ConeParticleEmitter.h
//  moFlo
//
//  Created by Andrew Glass on 02/11/2012.
//  Copyright (c) 2012 Tag Games. All rights reserved.
//

#ifndef _MO_FLO_RENDERING_PARTICLES_EMITTERS_CONE_PARTICLE_EMITTER_H_
#define _MO_FLO_RENDERING_PARTICLES_EMITTERS_CONE_PARTICLE_EMITTER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Rendering/Particles/Emitters/ParticleEmitter.h>

namespace ChilliSource
{
	namespace Rendering
	{
        class ConeParticleEmitter : public ParticleEmitter
        {
        public:
            ConeParticleEmitter(const Core::ParamDictionary& inParams, const MaterialSPtr &inpMaterial, ParticleComponent* inpComponent);
            
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
            static ParticleEmitter* Create(const Core::ParamDictionary& inParams, const MaterialSPtr &inpMaterial, ParticleComponent* inpComponent);
            
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
