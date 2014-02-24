//
//  ScalerParticleEffector.h
//  moFlow
//
//  Created by Scott Downie on 11/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//


#ifndef _MO_FLO_RENDERING_PARTICLES_EFFECTORS_SCALER_PARTICLE_EFFECTOR_H_
#define _MO_FLO_RENDERING_PARTICLES_EFFECTORS_SCALER_PARTICLE_EFFECTOR_H_

#include <ChilliSource/Rendering/Particles/Effectors/ParticleEffector.h>

#include <ChilliSource/Core/Container/ParamDictionary.h>

namespace ChilliSource
{
    namespace Rendering
    {
        class CScalerParticleEffector : public IParticleEffector
        {
        public:
            CScalerParticleEffector(const Core::ParamDictionary& insParams);
            //-----------------------------------------------------
            /// Create
            ///
            /// Create a new effector from the given parameters
            ///
            /// @param Param dictionary
            /// @return Ownership of effector
            //-----------------------------------------------------
            static IParticleEffector* Create(const Core::ParamDictionary& inParams);
            //-----------------------------------------------------
            /// Init
            ///
            /// The effector will initialise the particles to its
            /// liking 
            ///
            /// @param Particle to intialise
            //-----------------------------------------------------
            void Init(Particle* inpParticle, u32 inudwIndex);
            //-----------------------------------------------------
            /// Apply
            ///
            /// The effector will apply itself to the given 
            /// particles
            ///
            /// @param Particle to effect
            /// @param Time between frames
            //-----------------------------------------------------
            void Apply(Particle* inpParticle, u32 inudwIndex, f32 infDt);
            
        private:
            
            bool mbInitialSet;
            Core::CVector3 mvInitialScale;
            Core::CVector3 mvTargetScale;
        };
    }
}

#endif