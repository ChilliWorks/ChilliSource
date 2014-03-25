//
//  ColourChangerParticleEffector.cpp
//  moFloTest
//
//  Created by Scott Downie on 11/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/Rendering/Particles/Effectors/ColourChangerParticleEffector.h>
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
        ColourChangerParticleEffector::ColourChangerParticleEffector(const Core::ParamDictionary& insParams)
        :mbInitialColourSet(false)
        {
            mfActiveEnergyLevel = 0.0f;
            
            std::string strTemp;

            //Colour change
            if(insParams.TryGetValue("TargetColour", strTemp))
            {
                mTargetColour = Core::ParseColour(strTemp);
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
        ParticleEffectorUPtr ColourChangerParticleEffector::Create(const Core::ParamDictionary& inParams)
        {
            return ParticleEffectorUPtr(new ColourChangerParticleEffector(inParams));
        }
        //-----------------------------------------------------
        /// Init
        ///
        /// The effector will initialise the particles to its
        /// liking 
        ///
        /// @param Particle to intialise
        //-----------------------------------------------------
        void ColourChangerParticleEffector::Init(Particle& in_particle)
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
        void ColourChangerParticleEffector::Apply(Particle& in_particle, f32 infDt)
        {
            f32 fEnergy = in_particle.m_energy;
            
            fEnergy = fEnergy / mfActiveEnergyLevel;
            
            if(!mbInitialColourSet)
            {
                mbInitialColourSet = true;
                mInitialColour = in_particle.m_colour;
            }
            
            Core::Colour cColourDiff = (mTargetColour - mInitialColour) * (1.0f - fEnergy);
            Core::Colour cColourNew = mInitialColour + cColourDiff;
            
			in_particle.m_colour = cColourNew *cColourNew.a;
        }
    }
}
