/*
 *  ParticleSystem.h
 *  moFloTest
 *
 *  Created by Scott Downie on 17/01/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#ifndef _MO_FLO_RENDERING_PARTICLE_SYSTEM_H_
#define _MO_FLO_RENDERING_PARTICLE_SYSTEM_H_

#include <ChilliSource/Rendering/ForwardDeclarations.h>

#include <ChilliSource/Rendering/Particles/ParticleComponentFactory.h>
#include <ChilliSource/Rendering/Particles/Emitters/ParticleEmitterFactory.h>
#include <ChilliSource/Rendering/Particles/Effectors/ParticleEffectorFactory.h>

#include <ChilliSource/Core/System/System.h>
#include <ChilliSource/Core/System/SystemConcepts.h>

namespace ChilliSource
{
	namespace Rendering
	{
		class CParticleSystem : public Core::ISystem, public Core::IUpdateable, public Core::IComponentProducer
		{
		public:
			DECLARE_NAMED_INTERFACE(CParticleSystem);
			
			CParticleSystem();
			
			//----------------------------------------------------------
			/// Is A
			///
			/// Returns if it is of the type given
			/// @param Comparison Type
			/// @return Whether the class matches the comparison type
			//----------------------------------------------------------
			bool IsA(Core::InterfaceIDType inInterfaceID) const;
			//-------------------------------------------
			/// Add Particle Component
			///
			/// @param Particle Component
			//-------------------------------------------
			void AddParticleComponent(CParticleComponent* inpParticle);
			//-------------------------------------------
			/// Remove Particle Component
			///
			/// @param Particle Component
			//-------------------------------------------
			void RemoveParticleComponent(CParticleComponent* inpParticle);
			//-------------------------------------------
			/// Update 
			///
			/// Update all the particle components
			///
			/// @param Time between frames
			//-------------------------------------------
			void Update(f32 infDT);
			//-------------------------------------------
			/// Sets a factor to scale update delta time by. Useful for pausing all effects when paused etc.
			//-------------------------------------------
			void SetTimeScaleFactor(f32 infValue);			
			//----------------------------------------------------
			/// Get Number Of Component Factories
			///
			/// @return Number of factories in this system
			//----------------------------------------------------
			u32 GetNumComponentFactories() const;
			//-------------------------------------------
			/// Get Component Factory Pointer 
			///
			/// @return Particle component factory
			//-------------------------------------------
			Core::IComponentFactory* GetComponentFactoryPtr(u32 inudwIndex);
			//-------------------------------------------
			/// Get Component Factory  
			///
			/// @return Particle component factory
			//-------------------------------------------
			Core::IComponentFactory& GetComponentFactory(u32 inudwIndex);
            //-------------------------------------------
			/// Get Emitter Factory
			///
			/// @return Particle Emitter factory
			//-------------------------------------------
            CParticleEmitterFactory& GetEmitterFactory();
            //-------------------------------------------
			/// Get Effector Factory  
			///
			/// @return Particle Effector factory
			//-------------------------------------------
            CParticleEffectorFactory& GetEffectorFactory();
			
		private:
			
			CParticleComponentFactory* mpParticleFactory;
            CParticleEmitterFactory mEmitterFactory;
            CParticleEffectorFactory mEffectorFactory;
			f32 mfTimeScaleFactor;
			DYNAMIC_ARRAY<CParticleComponent*> mParticleComponents;
		};
	}
}

#endif