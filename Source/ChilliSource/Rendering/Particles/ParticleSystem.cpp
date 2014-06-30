//
//  ParticleSystem.cpp
//  Chilli Source
//  Created by S Downie on 17/01/2011.
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

#include <ChilliSource/Rendering/Particles/ParticleSystem.h>

#include <ChilliSource/Core/Entity/Entity.h>
#include <ChilliSource/Rendering/Particles/ParticleComponent.h>


namespace ChilliSource
{
	namespace Rendering
	{
		CS_DEFINE_NAMEDTYPE(ParticleSystem);
        
        //----------------------------------------------------------
        //----------------------------------------------------------
        ParticleSystemUPtr ParticleSystem::Create()
        {
            return ParticleSystemUPtr(new ParticleSystem());
        }
		//----------------------------------------------------------
		//----------------------------------------------------------
		bool ParticleSystem::IsA(Core::InterfaceIDType in_interfaceId) const
		{
			return in_interfaceId == ParticleSystem::InterfaceID;
		}
		//-------------------------------------------
		//-------------------------------------------
		void ParticleSystem::AddParticleComponent(ParticleComponent* in_particle)
		{
            CS_ASSERT(in_particle != nullptr, "Cannot add null particle");
            
			in_particle->SetOwningSystem(this);
			m_particleComponents.push_back(in_particle);
		}
		//-------------------------------------------
		//-------------------------------------------
		void ParticleSystem::RemoveParticleComponent(ParticleComponent* in_particle)
		{
            CS_ASSERT(in_particle != nullptr, "Cannot remove null particle");
            
            u32 numParticles = m_particleComponents.size();
            for(u32 i=0; i<numParticles; ++i)
            {
                if(m_particleComponents[i] == in_particle)
                {
                    //Swap and pop
                    m_particleComponents[i]->SetOwningSystem(nullptr);
                    m_particleComponents[i] = m_particleComponents.back();
                    m_particleComponents.pop_back();
                    return;
                }
            }
		}
		//-------------------------------------------
		//-------------------------------------------
		void ParticleSystem::OnUpdate(f32 in_timeSinceLastUpdate)
		{
            in_timeSinceLastUpdate *= m_timeScaleFactor;
            
			for(auto it = m_particleComponents.begin(); it != m_particleComponents.end(); ++it)
			{
                if((*it)->GetEntity() && (*it)->GetEntity()->GetScene())
                {
                    (*it)->Update(in_timeSinceLastUpdate);
                }
			}
            
            // safe execution of emitter finished delegate
			for(auto it = m_particleComponents.begin(); it != m_particleComponents.end(); ++it)
			{
                if((*it)->GetEmittersFinished())
                {
                    (*it)->EmitterFinishedDelegate();
                    break;
                }
			}
		}
		//-------------------------------------------
		//-------------------------------------------
		void ParticleSystem::SetTimeScaleFactor(f32 in_scale)
		{
			m_timeScaleFactor = in_scale;
		}
        //----------------------------------------------------------
        //----------------------------------------------------------
        void ParticleSystem::OnDestroy()
        {
            u32 numParticles = m_particleComponents.size();
            for(u32 i=0; i<numParticles; ++i)
            {
                m_particleComponents[i]->SetOwningSystem(nullptr);
            }
            
            m_timeScaleFactor = 1.0f;
            
            m_particleComponents.clear();
            m_particleComponents.shrink_to_fit();
        }
	}
}