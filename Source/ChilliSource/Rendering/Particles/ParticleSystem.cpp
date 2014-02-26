/*
 *  ParticleSystem.cpp
 *  moFloTest
 *
 *  Created by Scott Downie on 17/01/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Rendering/Particles/ParticleSystem.h>

#include <ChilliSource/Rendering/Particles/Emitters/PointParticleEmitter.h>
#include <ChilliSource/Rendering/Particles/Emitters/RingParticleEmitter.h>
#include <ChilliSource/Rendering/Particles/Emitters/ConeParticleEmitter.h>

#include <ChilliSource/Rendering/Particles/Effectors/ColourChangerParticleEffector.h>
#include <ChilliSource/Rendering/Particles/Effectors/LinearForceParticleEffector.h>
#include <ChilliSource/Rendering/Particles/Effectors/SpinnerParticleEffector.h>
#include <ChilliSource/Rendering/Particles/Effectors/ScalerParticleEffector.h>

#include <ChilliSource/Rendering/Particles/ParticleComponent.h>

namespace ChilliSource
{
	namespace Rendering
	{
		DEFINE_NAMED_INTERFACE(CParticleSystem);
		//------------------------------------------------
		/// Constructor
		///
		/// Default
		//------------------------------------------------
		CParticleSystem::CParticleSystem() : mfTimeScaleFactor(1.0f)
		{
			mpParticleFactory = new CParticleComponentFactory(this, &mEmitterFactory, &mEffectorFactory);

            //Register the default emitters and effectors
            //with the factories
            mEmitterFactory.Register(CParticleEmitterFactory::EmitterCreateDelegate(&CPointParticleEmitter::Create), "Point");
            mEmitterFactory.Register(CParticleEmitterFactory::EmitterCreateDelegate(&CRingParticleEmitter::Create), "Ring");
            mEmitterFactory.Register(CParticleEmitterFactory::EmitterCreateDelegate(&CConeParticleEmitter::Create), "Cone");
            
            mEffectorFactory.Register(CParticleEffectorFactory::EffectorCreateDelegate(&CColourChangerParticleEffector::Create), "ColourChanger");
            mEffectorFactory.Register(CParticleEffectorFactory::EffectorCreateDelegate(&CLinearForceParticleEffector::Create), "LinearForce");
            mEffectorFactory.Register(CParticleEffectorFactory::EffectorCreateDelegate(&CSpinnerParticleEffector::Create), "Spinner");
            mEffectorFactory.Register(CParticleEffectorFactory::EffectorCreateDelegate(&CScalerParticleEffector::Create), "Scaler");
		}
		//----------------------------------------------------------
		/// Is A
		///
		/// Returns if it is of the type given
		/// @param Comparison Type
		/// @return Whether the class matches the comparison type
		//----------------------------------------------------------
		bool CParticleSystem::IsA(ChilliSource::Core::InterfaceIDType inInterfaceID) const
		{
			return (inInterfaceID == CParticleSystem::InterfaceID) || (inInterfaceID == Core::IUpdateable::InterfaceID) || (inInterfaceID == Core::IComponentProducer::InterfaceID);
		}
		//-------------------------------------------
		/// Add Particle Component
		///
		/// @param Particle Component
		//-------------------------------------------
		void CParticleSystem::AddParticleComponent(CParticleComponent* inpParticle)
		{
			inpParticle->SetOwningSystem(this);
			mParticleComponents.push_back(inpParticle);
		}
		//-------------------------------------------
		/// Remove Particle Component
		///
		/// @param Particle Component
		//-------------------------------------------
		void CParticleSystem::RemoveParticleComponent(CParticleComponent* inpParticle)
		{
            for(std::vector<CParticleComponent*>::iterator it = mParticleComponents.begin(); it != mParticleComponents.end(); ++it)
            {
                if((*it) == inpParticle)
                {
                    mParticleComponents.erase(it);
                    return;
                }
            }
		}
		//-------------------------------------------
		/// Update 
		///
		/// Update all the particle components
		///
		/// @param Time between frames
		//-------------------------------------------
		void CParticleSystem::Update(f32 infDT)
		{
            infDT *= mfTimeScaleFactor;
            
			for(std::vector<CParticleComponent*>::iterator it = mParticleComponents.begin(); it != mParticleComponents.end(); ++it)
			{
                if((*it)->GetEntityOwner())
                    (*it)->Update(infDT);
			}
            
            // safe execution of emitter finished delegate
			for(std::vector<CParticleComponent*>::iterator it = mParticleComponents.begin(); it != mParticleComponents.end(); ++it)
			{
                if((*it)->GetEmittersFinished())
                {
                    (*it)->EmitterFinishedDelegate();
                    break;
                }
			}
		}
		//-------------------------------------------
		/// Sets a factor to scale update delta time by. Useful for pausing all effects when paused etc.
		//-------------------------------------------
		void CParticleSystem::SetTimeScaleFactor(f32 infValue)
		{
			mfTimeScaleFactor = infValue;
		}
		//----------------------------------------------------
		/// Get Number Of Component Factories
		///
		/// @return Number of factories in this system
		//----------------------------------------------------
		u32 CParticleSystem::GetNumComponentFactories() const
		{
			return 1;
		}
		//-------------------------------------------
		/// Get Component Factory Pointer 
		///
		/// @return Particle component factory
		//-------------------------------------------
		Core::IComponentFactory* CParticleSystem::GetComponentFactoryPtr(u32 inudwIndex)
		{
			return mpParticleFactory;
		}
		//-------------------------------------------
		/// Get Component Factory  
		///
		/// @return Particle component factory
		//-------------------------------------------
		Core::IComponentFactory& CParticleSystem::GetComponentFactory(u32 inudwIndex)
		{
			return *mpParticleFactory;
		}
        //-------------------------------------------
        /// Get Emitter Factory
        ///
        /// @return Particle Emitter factory
        //-------------------------------------------
        CParticleEmitterFactory& CParticleSystem::GetEmitterFactory()
        {
            return mEmitterFactory;
        }
        //-------------------------------------------
        /// Get Effector Factory  
        ///
        /// @return Particle Effector factory
        //-------------------------------------------
        CParticleEffectorFactory& CParticleSystem::GetEffectorFactory()
        {
            return mEffectorFactory;
        }
	}
}