//
//  ColourChangerParticleEffector.cpp
//  moFloTest
//
//  Created by Scott Downie on 11/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/Rendering/Particles/Effectors/SpinnerParticleEffector.h>
#include <ChilliSource/Rendering/Particles/Emitters/ParticleEmitter.h>
#include <ChilliSource/Core/Math/MathUtils.h>
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
        SpinnerParticleEffector::SpinnerParticleEffector(const Core::ParamDictionary& insParams)
        {
            mfActiveEnergyLevel = 0.0f;
            
            mfRandomOffset = Core::MathUtils::RandomInRange(0.0f, 1.0f);
            
            std::string strTemp;

            //Linear force
            if(insParams.TryGetValue("AngVelocity", strTemp))
            {
                mfAngVelocity = Core::MathUtils::DegToRad( Core::ParseF32(strTemp) );
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
        ParticleEffectorUPtr SpinnerParticleEffector::Create(const Core::ParamDictionary& inParams)
        {
            return ParticleEffectorUPtr(new SpinnerParticleEffector(inParams));
        }
        //-----------------------------------------------------
        /// Init
        ///
        /// The effector will initialise the particles to its
        /// liking 
        ///
        /// @param Particle to intialise
        //-----------------------------------------------------
        void SpinnerParticleEffector::Init(Particle* inpParticle, u32 inudwIndex)
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
        void SpinnerParticleEffector::Apply(Particle* inpParticle, u32 inudwIndex, f32 infDt)
        {
            f32 fRand = Core::MathUtils::GetPseudoRandom(inudwIndex + 1) + mfRandomOffset - 1.0f;
            f32 fAngVelocity = mfAngVelocity * fRand;
            
            inpParticle->fAngularRotation[inudwIndex] +=  fAngVelocity * infDt;
        }
    }
}
