//
//  ColourChangerParticleEffector.cpp
//  moFloTest
//
//  Created by Scott Downie on 11/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/Rendering/Particles/Effectors/LinearForceParticleEffector.h>
#include <ChilliSource/Rendering/Particles/Emitters/ParticleEmitter.h>

namespace ChilliSource
{
    namespace Rendering
    {
        //----------------------------------------------------
        /// Constructor
        ///
        /// @param Param dictionary
        //----------------------------------------------------
        CLinearForceParticleEffector::CLinearForceParticleEffector(const Core::ParamDictionary& insParams)
        {
            mfActiveEnergyLevel = 0.0f;
            
            std::string strTemp;

            //Linear force
            if(insParams.TryGetValue("Force", strTemp))
            {
                mvForce = Core::CStringConverter::ParseVector3(strTemp);
            }
            //Energy level
            if(insParams.TryGetValue("AtEnergyLevel", strTemp))
            {
                mfActiveEnergyLevel = Core::CStringConverter::ParseFloat(strTemp);
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
        IParticleEffector* CLinearForceParticleEffector::Create(const Core::ParamDictionary& inParams)
        {
            return new CLinearForceParticleEffector(inParams);
        }
        //-----------------------------------------------------
        /// Init
        ///
        /// The effector will initialise the particles to its
        /// liking 
        ///
        /// @param Particle to intialise
        //-----------------------------------------------------
        void CLinearForceParticleEffector::Init(Particle* inpParticle, u32 inudwIndex)
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
        void CLinearForceParticleEffector::Apply(Particle* inpParticle, u32 inudwIndex, f32 infDt)
        {
			inpParticle->vVelocity[inudwIndex] += (mvForce * infDt);
        }
    }
}
