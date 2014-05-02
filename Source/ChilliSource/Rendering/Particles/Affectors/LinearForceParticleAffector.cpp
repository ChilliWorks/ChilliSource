//
//  ColourChangerParticleAffector.cpp
//  moFloTest
//
//  Created by Scott Downie on 11/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/Rendering/Particles/Affectors/LinearForceParticleAffector.h>
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
        LinearForceParticleAffector::LinearForceParticleAffector(const Core::ParamDictionary& insParams)
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
        /// Create a new affector from the given parameters
        ///
        /// @param Param dictionary
        /// @return Ownership of affector
        //-----------------------------------------------------
        ParticleAffectorUPtr LinearForceParticleAffector::Create(const Core::ParamDictionary& inParams)
        {
            return ParticleAffectorUPtr(new LinearForceParticleAffector(inParams));
        }
        //-----------------------------------------------------
        /// Init
        ///
        /// The affector will initialise the particles to its
        /// liking 
        ///
        /// @param Particle to intialise
        //-----------------------------------------------------
        void LinearForceParticleAffector::Init(Particle& in_particle)
        {
            
        }
        //-----------------------------------------------------
        /// Apply
        ///
        /// The affector will apply itself to the given 
        /// particles
        ///
        /// @param Particle to effect
        /// @param Time between frames
        //-----------------------------------------------------
        void LinearForceParticleAffector::Apply(Particle& in_particle, f32 infDt)
        {
			in_particle.m_velocity += (mvForce * infDt);
        }
    }
}
