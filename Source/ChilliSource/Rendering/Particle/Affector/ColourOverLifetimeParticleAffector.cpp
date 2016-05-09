//
//  ColourOverLifetimeParticleAffector.cpp
//  Chilli Source
//  Created by Ian Copland on 02/11/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
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

#include <ChilliSource/Rendering/Particle/Affector/ColourOverLifetimeParticleAffector.h>

#include <ChilliSource/Core/Math/MathUtils.h>
#include <ChilliSource/Rendering/Particle/Particle.h>
#include <ChilliSource/Rendering/Particle/ParticleEffect.h>
#include <ChilliSource/Rendering/Particle/Affector/ColourOverLifetimeParticleAffectorDef.h>

namespace ChilliSource
{
    namespace
    {
        //----------------------------------------------------------------
        /// @author Nicolas Tanda
        ///
        /// @param Value to clamp
        ///
        /// @return The value clamped between 0.0f and 1.0f
        //----------------------------------------------------------------
        f32 Clamp(f32 in_value)
        {
            return MathUtils::Clamp(in_value, 0.0f, 1.0f);
        }
    }
    
    //----------------------------------------------------------------
    //----------------------------------------------------------------
    ColourOverLifetimeParticleAffector::ColourOverLifetimeParticleAffector(const ParticleAffectorDef* in_affectorDef, dynamic_array<Particle>* in_particleArray)
    :ParticleAffector(in_affectorDef, in_particleArray)
    ,m_particleColourData(0)
    {
        m_colourOverLifetimeAffectorDef = static_cast<const ColourOverLifetimeParticleAffectorDef*>(in_affectorDef);
        m_intermediateParticles = static_cast<u32>(m_colourOverLifetimeAffectorDef->GetIntermediateColours().size());
        m_particleColourData = std::move(dynamic_array<ColourData>(in_particleArray->size() * (2 + m_intermediateParticles)));
    }
    //----------------------------------------------------------------
    //----------------------------------------------------------------
    void ColourOverLifetimeParticleAffector::ActivateParticle(u32 in_index, f32 in_effectProgress)
    {
        u32 colourDataIndex = in_index * (2 + m_intermediateParticles);
        CS_ASSERT(colourDataIndex >= 0 && colourDataIndex < m_particleColourData.size(), "colourDataIndex out of bounds!");
        
        ColourData& colourDataInitial = m_particleColourData[colourDataIndex];
        const Particle& particle = GetParticleArray()->at(in_index);
        
        colourDataInitial.m_time = 0.0f;
        colourDataInitial.m_colour = particle.m_colour;
        ++colourDataIndex;
        
        // Get the intermediate colours
        std::vector<ColourData> intermediateColours;
        intermediateColours.reserve(m_intermediateParticles);
        for(const auto& intermediateColour : m_colourOverLifetimeAffectorDef->GetIntermediateColours())
        {
            intermediateColours.push_back(ColourData());
            intermediateColours.back().m_colour = intermediateColour.m_colourProperty->GenerateValue(in_effectProgress);
            intermediateColours.back().m_time = intermediateColour.m_timeProperty->GenerateValue(in_effectProgress);
        }
        
        // Sort by time
        std::sort(intermediateColours.begin(), intermediateColours.end(), [](const ColourData& in_r, const ColourData& in_l)-> bool
        {
            return in_r.m_time < in_l.m_time;
        });
        
        // Add to the particles colour data
        for(const auto& intermediateColour : intermediateColours)
        {
            ColourData& colourDataIntermediate = m_particleColourData[colourDataIndex];
            colourDataIntermediate = intermediateColour;
            ++colourDataIndex;
        }
        
        ColourData& colourDataTarget = m_particleColourData[colourDataIndex];
        colourDataTarget.m_time = 1.0f;
        colourDataTarget.m_colour = m_colourOverLifetimeAffectorDef->GetTargetColourProperty()->GenerateValue(in_effectProgress);
    }
    //----------------------------------------------------------------
    //----------------------------------------------------------------
    void ColourOverLifetimeParticleAffector::AffectParticles(f32 in_deltaTime, f32 in_effectProgress)
    {
        const auto& interpolation = m_colourOverLifetimeAffectorDef->GetInterpolation();
        
        dynamic_array<Particle>* particleArray = GetParticleArray();
        for (u32 i = 0; i < particleArray->size(); ++i)
        {
            Particle& particle = particleArray->at(i);
            
            if(particle.m_isActive == false)
            {
                continue;
            }
            
            u32 colourIndex = i * (2 + m_intermediateParticles);
            ColourData& colourDataInitial = m_particleColourData[colourIndex];

            f32 normalisedLifeProgress = 1.0f - (particle.m_energy / particle.m_lifetime);
            f32 progress = interpolation(normalisedLifeProgress);
            
            particle.m_colour = colourDataInitial.m_colour;
            for(u32 offset = 0; offset < m_intermediateParticles + 1; ++offset)
            {
                ColourData& colourData = m_particleColourData[colourIndex + offset];
                ColourData& colourDataNext = m_particleColourData[colourIndex + offset + 1];
                f32 timeProgress = Clamp(Clamp(progress - colourData.m_time) / (colourDataNext.m_time - colourData.m_time));
                particle.m_colour += (colourDataNext.m_colour - colourData.m_colour) * timeProgress;
            }
        }
    }
}
