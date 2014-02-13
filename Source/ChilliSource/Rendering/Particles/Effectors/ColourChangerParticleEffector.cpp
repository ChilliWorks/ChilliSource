//
//  ColourChangerParticleEffector.cpp
//  moFloTest
//
//  Created by Scott Downie on 11/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/Rendering/Particles/Effectors/ColourChangerParticleEffector.h>
#include <ChilliSource/Rendering/Particles/Emitters/ParticleEmitter.h>

namespace moFlo
{
    namespace Rendering
    {
        //----------------------------------------------------
        /// Constructor
        ///
        /// @param Param dictionary
        //----------------------------------------------------
        CColourChangerParticleEffector::CColourChangerParticleEffector(const Core::ParamDictionary& insParams)
        :mbInitialColourSet(false)
        {
            mfActiveEnergyLevel = 0.0f;
            
            std::string strTemp;

            //Colour change
            if(insParams.TryGetValue("TargetColour", strTemp))
            {
                mTargetColour = Core::CStringConverter::ParseColourValue(strTemp);
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
        IParticleEffector* CColourChangerParticleEffector::Create(const Core::ParamDictionary& inParams)
        {
            return new CColourChangerParticleEffector(inParams);
        }
        //-----------------------------------------------------
        /// Init
        ///
        /// The effector will initialise the particles to its
        /// liking 
        ///
        /// @param Particle to intialise
        //-----------------------------------------------------
        void CColourChangerParticleEffector::Init(Particle* inpParticle, u32 inudwIndex)
        {
            //Doesn't require initing
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
        void CColourChangerParticleEffector::Apply(Particle* inpParticle, u32 inudwIndex, f32 infDt)
        {
            f32 fEnergy = inpParticle->fEnergy[inudwIndex];
            
            fEnergy = fEnergy / mfActiveEnergyLevel;
            
            if(!mbInitialColourSet)
            {
                mbInitialColourSet = true;
                mInitialColour = inpParticle->Col[inudwIndex];
            }
            
            Core::CColour cColourDiff = (mTargetColour - mInitialColour) * (1.0f - fEnergy);
            Core::CColour cColourNew = mInitialColour + cColourDiff;
            
			inpParticle->Col[inudwIndex] = cColourNew *cColourNew.a;
        }
    }
}
