//
//  ParticleSystem.cpp
//  Chilli Source
//
//  Created by Scott Downie on 17/01/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/Rendering/Particles/ParticleSystem.h>

#include <ChilliSource/Core/Base/MakeDelegate.h>
#include <ChilliSource/Rendering/Particles/ParticleComponent.h>
#include <ChilliSource/Rendering/Particles/Emitters/PointParticleEmitter.h>
#include <ChilliSource/Rendering/Particles/Emitters/RingParticleEmitter.h>
#include <ChilliSource/Rendering/Particles/Emitters/ConeParticleEmitter.h>
#include <ChilliSource/Rendering/Particles/Effectors/ColourChangerParticleEffector.h>
#include <ChilliSource/Rendering/Particles/Effectors/LinearForceParticleEffector.h>
#include <ChilliSource/Rendering/Particles/Effectors/SpinnerParticleEffector.h>
#include <ChilliSource/Rendering/Particles/Effectors/ScalerParticleEffector.h>

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
		//------------------------------------------------
		//------------------------------------------------
		ParticleSystem::ParticleSystem()
        : m_particleFactory(this, &m_emitterFactory, &m_effectorFactory), m_timeScaleFactor(1.0f)
		{

		}
		//----------------------------------------------------------
		//----------------------------------------------------------
		bool ParticleSystem::IsA(Core::InterfaceIDType in_interfaceID) const
		{
			return (in_interfaceID == ParticleSystem::InterfaceID) || (in_interfaceID == Core::IComponentProducer::InterfaceID);
		}
        //----------------------------------------------------------
        //----------------------------------------------------------
        void ParticleSystem::OnInit()
        {
            //Register the default emitters and effectors with the factories
            m_emitterFactory.AddCreator(Core::MakeDelegate(&PointParticleEmitter::Create), "Point");
            m_emitterFactory.AddCreator(Core::MakeDelegate(&RingParticleEmitter::Create), "Ring");
            m_emitterFactory.AddCreator(Core::MakeDelegate(&ConeParticleEmitter::Create), "Cone");
            
            m_effectorFactory.AddCreator(Core::MakeDelegate(&ColourChangerParticleEffector::Create), "ColourChanger");
            m_effectorFactory.AddCreator(Core::MakeDelegate(&LinearForceParticleEffector::Create), "LinearForce");
            m_effectorFactory.AddCreator(Core::MakeDelegate(&SpinnerParticleEffector::Create), "Spinner");
            m_effectorFactory.AddCreator(Core::MakeDelegate(&ScalerParticleEffector::Create), "Scaler");
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
                    std::swap(m_particleComponents.back(), m_particleComponents[i]);
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
                if((*it)->GetEntity())
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
		//----------------------------------------------------
		//----------------------------------------------------
		u32 ParticleSystem::GetNumComponentFactories() const
		{
			return 1;
		}
		//-------------------------------------------
		//-------------------------------------------
		Core::ComponentFactory* ParticleSystem::GetComponentFactoryPtr(u32 in_index)
		{
			return &m_particleFactory;
		}
		//-------------------------------------------
		//-------------------------------------------
		Core::ComponentFactory& ParticleSystem::GetComponentFactory(u32 in_index)
		{
			return m_particleFactory;
		}
        //-------------------------------------------
        //-------------------------------------------
        ParticleEmitterFactory& ParticleSystem::GetEmitterFactory()
        {
            return m_emitterFactory;
        }
        //-------------------------------------------
        //-------------------------------------------
        ParticleEffectorFactory& ParticleSystem::GetEffectorFactory()
        {
            return m_effectorFactory;
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
            
            m_emitterFactory.RemoveAllCreators();
            m_effectorFactory.RemoveAllCreators();
        }
	}
}