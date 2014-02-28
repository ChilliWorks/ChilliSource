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

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Rendering/Particles/ParticleComponentFactory.h>
#include <ChilliSource/Rendering/Particles/Emitters/ParticleEmitterFactory.h>
#include <ChilliSource/Rendering/Particles/Effectors/ParticleEffectorFactory.h>
#include <ChilliSource/Core/System/System.h>
#include <ChilliSource/Core/System/SystemConcepts.h>

namespace ChilliSource
{
	namespace Rendering
	{
		class ParticleSystem : public Core::System, public Core::IUpdateable, public Core::IComponentProducer
		{
		public:
			CS_DECLARE_NAMEDTYPE(ParticleSystem);
			
			ParticleSystem();
			
			//----------------------------------------------------------
			/// Is A
			///
			/// Returns if it is of the type given
			/// @param Comparison Type
			/// @return Whether the class matches the comparison type
			//----------------------------------------------------------
			bool IsA(Core::InterfaceIDType inInterfaceID) const override;
			//-------------------------------------------
			/// Add Particle Component
			///
			/// @param Particle Component
			//-------------------------------------------
			void AddParticleComponent(ParticleComponent* inpParticle);
			//-------------------------------------------
			/// Remove Particle Component
			///
			/// @param Particle Component
			//-------------------------------------------
			void RemoveParticleComponent(ParticleComponent* inpParticle);
			//-------------------------------------------
			/// Update 
			///
			/// Update all the particle components
			///
			/// @param Time between frames
			//-------------------------------------------
			void Update(f32 infDT) override;
			//-------------------------------------------
			/// Sets a factor to scale update delta time by. Useful for pausing all effects when paused etc.
			//-------------------------------------------
			void SetTimeScaleFactor(f32 infValue);			
			//----------------------------------------------------
			/// Get Number Of Component Factories
			///
			/// @return Number of factories in this system
			//----------------------------------------------------
			u32 GetNumComponentFactories() const override;
			//-------------------------------------------
			/// Get Component Factory Pointer 
			///
			/// @return Particle component factory
			//-------------------------------------------
			Core::ComponentFactory* GetComponentFactoryPtr(u32 inudwIndex) override;
			//-------------------------------------------
			/// Get Component Factory  
			///
			/// @return Particle component factory
			//-------------------------------------------
			Core::ComponentFactory& GetComponentFactory(u32 inudwIndex) override;
            //-------------------------------------------
			/// Get Emitter Factory
			///
			/// @return Particle Emitter factory
			//-------------------------------------------
            ParticleEmitterFactory& GetEmitterFactory();
            //-------------------------------------------
			/// Get Effector Factory  
			///
			/// @return Particle Effector factory
			//-------------------------------------------
            ParticleEffectorFactory& GetEffectorFactory();
			
		private:
			
			ParticleComponentFactory* mpParticleFactory;
            ParticleEmitterFactory mEmitterFactory;
            ParticleEffectorFactory mEffectorFactory;
			f32 mfTimeScaleFactor;
			std::vector<ParticleComponent*> mParticleComponents;
		};
	}
}

#endif