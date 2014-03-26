//
//  ScalerParticleEffector.cpp
//  moFloTest
//
//  Created by Scott Downie on 11/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/Rendering/Particles/Effectors/ScalerParticleEffector.h>
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
        ScalerParticleEffector::ScalerParticleEffector(const Core::ParamDictionary& insParams)
        :mbInitialSet(false)
        {
            mfActiveEnergyLevel = 0.0f;
            
            std::string strTemp;

            //Colour change
            if(insParams.TryGetValue("TargetScale", strTemp))
            {
                float fScale = Core::ParseF32(strTemp);
                mvTargetScale.x = fScale;
                mvTargetScale.y = fScale;
                mvTargetScale.z = fScale;
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
        ParticleEffectorUPtr ScalerParticleEffector::Create(const Core::ParamDictionary& inParams)
        {
            return ParticleEffectorUPtr(new ScalerParticleEffector(inParams));
        }
        //-----------------------------------------------------
        /// Init
        ///
        /// The effector will initialise the particles to its
        /// liking 
        ///
        /// @param Particle to intialise
        //-----------------------------------------------------
        void ScalerParticleEffector::Init(Particle& in_particle)
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
        void ScalerParticleEffector::Apply(Particle& in_particle, f32 infDt)
        {
            f32 fEnergy = in_particle.m_energy;
            
            fEnergy = fEnergy / mfActiveEnergyLevel;
            
            if(!mbInitialSet)
            {
                mbInitialSet = true;
                mvInitialScale = in_particle.m_scale;
            }
            
            Core::Vector3 vDiff = (mvTargetScale - mvInitialScale) * (1.0f - fEnergy);
            in_particle.m_scale = mvInitialScale + vDiff;
        }
    }
}
