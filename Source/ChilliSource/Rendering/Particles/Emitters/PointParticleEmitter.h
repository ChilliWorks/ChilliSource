//
//  PointParticleEmitter.h
//  moFloTest
//
//  Created by Scott Downie on 11/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _MO_FLO_RENDERING_PARTICLES_EMITTERS_POINT_PARTICLE_EMITTER_H_
#define _MO_FLO_RENDERING_PARTICLES_EMITTERS_POINT_PARTICLE_EMITTER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Rendering/Particles/Emitters/ParticleEmitter.h>

namespace ChilliSource
{
    namespace Rendering
    {
        class PointParticleEmitter : public ParticleEmitter
        {
        public:
            PointParticleEmitter(const Core::ParamDictionary& inParams, const MaterialPtr &inpMaterial, ParticleComponent* inpComponent);

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
            static ParticleEmitter* Create(const Core::ParamDictionary& inParams, const MaterialPtr &inpMaterial, ParticleComponent* inpComponent);
            
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
        };
    }
}

#endif