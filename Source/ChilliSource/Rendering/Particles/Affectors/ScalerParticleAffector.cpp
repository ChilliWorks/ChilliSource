//
//  ScalerParticleAffector.cpp
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

#include <ChilliSource/Rendering/Particles/Affectors/ScalerParticleAffector.h>
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
        ScalerParticleAffector::ScalerParticleAffector(const Core::ParamDictionary& insParams)
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
        ParticleAffectorUPtr ScalerParticleAffector::Create(const Core::ParamDictionary& inParams)
        {
            return ParticleAffectorUPtr(new ScalerParticleAffector(inParams));
        }
        //-----------------------------------------------------
        /// Init
        ///
        /// The affector will initialise the particles to its
        /// liking 
        ///
        /// @param Particle to intialise
        //-----------------------------------------------------
        void ScalerParticleAffector::Init(Particle& in_particle)
        {
            //Doesn't require initing
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
        void ScalerParticleAffector::Apply(Particle& in_particle, f32 infDt)
        {
            f32 fEnergy = in_particle.m_energy;
            
            fEnergy = fEnergy / mfActiveEnergyLevel;
            
            if(!mbInitialSet)
            {
                mbInitialSet = true;
                mvInitialScale = in_particle.m_scale;
            }
            
            Core::Vector2 vDiff = (mvTargetScale - mvInitialScale) * (1.0f - fEnergy);
            in_particle.m_scale = mvInitialScale + vDiff;
        }
    }
}
