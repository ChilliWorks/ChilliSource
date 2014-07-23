//
//  ColourChangerParticleAffector.cpp
//  Chilli Source
//  Created by Scott Downie on 11/04/2011.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2011 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
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
