//
//  ColourChangerParticleEffector.cpp
//  moFloTest
//
//  Created by Scott Downie on 11/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/Rendering/Particles/Effectors/LinearForceParticleEffector.h>
#include <ChilliSource/Rendering/Particles/Emitters/ParticleEmitter.h>
#include <ChilliSource/Core/String/StringParser.h>

namespace ChilliSource
{
    namespace Rendering
    {
        //----------------------------------------------------
        /// Constructor
        ///
        /// @param Param dictionary
        //----------------------------------------------------
        LinearForceParticleEffector::LinearForceParticleEffector(const Core::ParamDictionary& insParams)
        {
            mfActiveEnergyLevel = 0.0f;
            
            std::string strTemp;

            //Linear force
            if(insParams.TryGetValue("Force", strTemp))
            {
                mvForce = Core::ParseVector3(strTemp);
            }
            //Energy level
            if(insParams.TryGetValue("AtEnergyLevel", strTemp))
            {
                mfActiveEnergyLevel = Core::ParseF32(strTemp);
            }
        }
        //-----------------------------------------------------
        /// Create
        ///
        /// Create a new effector from the given parameters
        ///
        /// @param Param dictionary
        /// @return Ownership of effector
        //-----------------------------------------------------
        ParticleEffectorUPtr LinearForceParticleEffector::Create(const Core::ParamDictionary& inParams)
        {
            return ParticleEffectorUPtr(new LinearForceParticleEffector(inParams));
        }
        //-----------------------------------------------------
        /// Init
        ///
        /// The effector will initialise the particles to its
        /// liking 
        ///
        /// @param Particle to intialise
        //-----------------------------------------------------
        void LinearForceParticleEffector::Init(Particle* inpParticle, u32 inudwIndex)
        {
            
        }
        //-----------------------------------------------------
        /// Apply
        ///
        /// The effector will apply itself to the given 
        /// particles
        ///
        /// @param Particle to effect
        /// @param Time between frames
        //-----------------------------------------------------
        void LinearForceParticleEffector::Apply(Particle* inpParticle, u32 inudwIndex, f32 infDt)
        {
			inpParticle->vVelocity[inudwIndex] += (mvForce * infDt);
        }
    }
}
