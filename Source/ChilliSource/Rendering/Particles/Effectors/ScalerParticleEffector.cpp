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
        CScalerParticleEffector::CScalerParticleEffector(const Core::ParamDictionary& insParams)
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
        IParticleEffector* CScalerParticleEffector::Create(const Core::ParamDictionary& inParams)
        {
            return new CScalerParticleEffector(inParams);
        }
        //-----------------------------------------------------
        /// Init
        ///
        /// The effector will initialise the particles to its
        /// liking 
        ///
        /// @param Particle to intialise
        //-----------------------------------------------------
        void CScalerParticleEffector::Init(Particle* inpParticle, u32 inudwIndex)
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
        void CScalerParticleEffector::Apply(Particle* inpParticle, u32 inudwIndex, f32 infDt)
        {
            f32 fEnergy = inpParticle->fEnergy[inudwIndex];
            
            fEnergy = fEnergy / mfActiveEnergyLevel;
            
            if(!mbInitialSet)
            {
                mbInitialSet = true;
                mvInitialScale = inpParticle->vScale[inudwIndex];
            }
            
            Core::CVector3 vDiff = (mvTargetScale - mvInitialScale) * (1.0f - fEnergy);
            inpParticle->vScale[inudwIndex] = mvInitialScale + vDiff;
        }
    }
}
