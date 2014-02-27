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
		DEFINE_NAMED_INTERFACE(ParticleSystem);
		//------------------------------------------------
		/// Constructor
		///
		/// Default
		//------------------------------------------------
		ParticleSystem::ParticleSystem() : mfTimeScaleFactor(1.0f)
		{
			mpParticleFactory = new ParticleComponentFactory(this, &mEmitterFactory, &mEffectorFactory);

            //Register the default emitters and effectors
            //with the factories
            mEmitterFactory.Register(ParticleEmitterFactory::EmitterCreateDelegate(&PointParticleEmitter::Create), "Point");
            mEmitterFactory.Register(ParticleEmitterFactory::EmitterCreateDelegate(&RingParticleEmitter::Create), "Ring");
            mEmitterFactory.Register(ParticleEmitterFactory::EmitterCreateDelegate(&ConeParticleEmitter::Create), "Cone");
            
            mEffectorFactory.Register(ParticleEffectorFactory::EffectorCreateDelegate(&ColourChangerParticleEffector::Create), "ColourChanger");
            mEffectorFactory.Register(ParticleEffectorFactory::EffectorCreateDelegate(&LinearForceParticleEffector::Create), "LinearForce");
            mEffectorFactory.Register(ParticleEffectorFactory::EffectorCreateDelegate(&SpinnerParticleEffector::Create), "Spinner");
            mEffectorFactory.Register(ParticleEffectorFactory::EffectorCreateDelegate(&ScalerParticleEffector::Create), "Scaler");
		}
		//----------------------------------------------------------
		/// Is A
		///
		/// Returns if it is of the type given
		/// @param Comparison Type
		/// @return Whether the class matches the comparison type
		//----------------------------------------------------------
		bool ParticleSystem::IsA(ChilliSource::Core::InterfaceIDType inInterfaceID) const
		{
			return (inInterfaceID == ParticleSystem::InterfaceID) || (inInterfaceID == Core::IUpdateable::InterfaceID) || (inInterfaceID == Core::IComponentProducer::InterfaceID);
		}
		//-------------------------------------------
		/// Add Particle Component
		///
		/// @param Particle Component
		//-------------------------------------------
		void ParticleSystem::AddParticleComponent(ParticleComponent* inpParticle)
		{
			inpParticle->SetOwningSystem(this);
			mParticleComponents.push_back(inpParticle);
		}
		//-------------------------------------------
		/// Remove Particle Component
		///
		/// @param Particle Component
		//-------------------------------------------
		void ParticleSystem::RemoveParticleComponent(ParticleComponent* inpParticle)
		{
            for(std::vector<ParticleComponent*>::iterator it = mParticleComponents.begin(); it != mParticleComponents.end(); ++it)
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
		void ParticleSystem::Update(f32 infDT)
		{
            infDT *= mfTimeScaleFactor;
            
			for(std::vector<ParticleComponent*>::iterator it = mParticleComponents.begin(); it != mParticleComponents.end(); ++it)
			{
                if((*it)->GetEntityOwner())
                    (*it)->Update(infDT);
			}
            
            // safe execution of emitter finished delegate
			for(std::vector<ParticleComponent*>::iterator it = mParticleComponents.begin(); it != mParticleComponents.end(); ++it)
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
		void ParticleSystem::SetTimeScaleFactor(f32 infValue)
		{
			mfTimeScaleFactor = infValue;
		}
		//----------------------------------------------------
		/// Get Number Of Component Factories
		///
		/// @return Number of factories in this system
		//----------------------------------------------------
		u32 ParticleSystem::GetNumComponentFactories() const
		{
			return 1;
		}
		//-------------------------------------------
		/// Get Component Factory Pointer 
		///
		/// @return Particle component factory
		//-------------------------------------------
		Core::ComponentFactory* ParticleSystem::GetComponentFactoryPtr(u32 inudwIndex)
		{
			return mpParticleFactory;
		}
		//-------------------------------------------
		/// Get Component Factory  
		///
		/// @return Particle component factory
		//-------------------------------------------
		Core::ComponentFactory& ParticleSystem::GetComponentFactory(u32 inudwIndex)
		{
			return *mpParticleFactory;
		}
        //-------------------------------------------
        /// Get Emitter Factory
        ///
        /// @return Particle Emitter factory
        //-------------------------------------------
        ParticleEmitterFactory& ParticleSystem::GetEmitterFactory()
        {
            return mEmitterFactory;
        }
        //-------------------------------------------
        /// Get Effector Factory  
        ///
        /// @return Particle Effector factory
        //-------------------------------------------
        ParticleEffectorFactory& ParticleSystem::GetEffectorFactory()
        {
            return mEffectorFactory;
        }
	}
}