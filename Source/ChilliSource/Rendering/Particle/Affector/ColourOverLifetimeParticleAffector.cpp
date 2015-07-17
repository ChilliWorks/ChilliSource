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

#include <ChilliSource/Core/Math/CurveFunctions.h>
#include <ChilliSource/Core/Math/MathUtils.h>
#include <ChilliSource/Rendering/Particle/Particle.h>
#include <ChilliSource/Rendering/Particle/ParticleEffect.h>
#include <ChilliSource/Rendering/Particle/Affector/ColourOverLifetimeParticleAffectorDef.h>

namespace ChilliSource
{
	namespace Rendering
	{
        namespace
        {
            // TODO: somewhere else?
            const u32 k_maxIntermediateColours = 2;
            
            //----------------------------------------------------------------
            /// @author Nicolas Tanda
            ///
            /// @param Value to clamp
            ///
            /// @return The value clamped between 0.0f and 1.0f
            //----------------------------------------------------------------
            f32 Clamp(f32 in_value)
            {
                return CSCore::MathUtils::Clamp(in_value, 0.0f, 1.0f);
            }
        }
        
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		ColourOverLifetimeParticleAffector::ColourOverLifetimeParticleAffector(const ParticleAffectorDef* in_affectorDef, Core::dynamic_array<Particle>* in_particleArray)
			: ParticleAffector(in_affectorDef, in_particleArray), m_particleColourData(in_particleArray->size())
		{
			//This can only be created by the ColourOverLifetimeParticleAffectorDef so this is safe.
			m_colourOverLifetimeAffectorDef = static_cast<const ColourOverLifetimeParticleAffectorDef*>(in_affectorDef);
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		void ColourOverLifetimeParticleAffector::ActivateParticle(u32 in_index, f32 in_effectProgress)
		{
			CS_ASSERT(in_index >= 0 && in_index < m_particleColourData.size(), "Index out of bounds!");

			ColourData* colourData = m_particleColourData[in_index];
			const Particle& particle = GetParticleArray()->at(in_index);
            
            colourData[0].m_time = 0.0f;
            colourData[0].m_colour = particle.m_colour;
            
            for(u32 index = 1; index < k_maxIntermediateColours + 1; ++index)
            {
                colourData[index].m_colour = m_colourOverLifetimeAffectorDef->GetIntermediateColours()[index - 1].m_colourProperty->GenerateValue(in_effectProgress);
                colourData[index].m_time = m_colourOverLifetimeAffectorDef->GetIntermediateColours()[index - 1].m_timeProperty->GenerateValue(in_effectProgress);
                
                // No data for intermediate colour - take previous data
                if(colourData[index].m_time == 0.0f)
                {
                    colourData[index].m_time = colourData[index - 1].m_time;
                    colourData[index].m_colour = colourData[index - 1].m_colour;
                }
            }
            
            colourData[3].m_time = 1.0f;
            colourData[3].m_colour = m_colourOverLifetimeAffectorDef->GetTargetColourProperty()->GenerateValue(in_effectProgress);
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		void ColourOverLifetimeParticleAffector::AffectParticles(f32 in_deltaTime, f32 in_effectProgress)
        {
            const auto& curveFunction = CSCore::CurveFunctions::GetCurveFunction(m_colourOverLifetimeAffectorDef->GetCurveName());
            
			Core::dynamic_array<Particle>* particleArray = GetParticleArray();
			for (u32 i = 0; i < particleArray->size(); ++i)
			{
				Particle& particle = particleArray->at(i);
				ColourData* colourData = m_particleColourData[i];
                
                if(particle.m_lifetime <= 0.0f)
                {
                    continue;
                }

                f32 normalisedLifeProgress = 1.0f - (particle.m_energy / particle.m_lifetime);
                f32 progress = curveFunction(normalisedLifeProgress);
                
                particle.m_colour = colourData[0].m_colour;
                for(u32 index = 0; index < 3; ++index)
                {
                    f32 timeProgress = Clamp(Clamp(progress - colourData[index].m_time) / (colourData[index + 1].m_time - colourData[index].m_time));
                    particle.m_colour += (colourData[index + 1].m_colour - colourData[index].m_colour) * timeProgress;
                }
			}
		}
	}
}