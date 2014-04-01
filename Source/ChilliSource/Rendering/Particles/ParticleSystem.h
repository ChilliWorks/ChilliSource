//
//  ParticleSystem.h
//  Chilli Source
//
//  Created by Scott Downie on 17/01/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_RENDERING_PARTICLES_PARTICLESYSTEM_H_
#define _CHILLISOURCE_RENDERING_PARTICLES_PARTICLESYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Rendering/Particles/ParticleComponentFactory.h>
#include <ChilliSource/Rendering/Particles/Emitters/ParticleEmitterFactory.h>
#include <ChilliSource/Rendering/Particles/Effectors/ParticleEffectorFactory.h>
#include <ChilliSource/Core/System/AppSystem.h>
#include <ChilliSource/Core/System/SystemConcepts.h>

namespace ChilliSource
{
	namespace Rendering
	{
        //----------------------------------------------------------
        /// The particle system manages the updating of all
        /// existing particle components
        ///
        /// @author S Downie
        //----------------------------------------------------------
		class ParticleSystem final : public Core::AppSystem, public Core::IComponentProducer
		{
		public:
			CS_DECLARE_NAMEDTYPE(ParticleSystem);
			
            //----------------------------------------------------------
            /// Factory method to create new particle system
            ///
            /// @author S Downie
            ///
            /// @return Ownership of new particle system
            //----------------------------------------------------------
            static ParticleSystemUPtr Create();
			//----------------------------------------------------------
			/// @author S Downie
			///
			/// @param Comparison Type
            ///
			/// @return Whether the class matches the comparison type
			//----------------------------------------------------------
			bool IsA(Core::InterfaceIDType in_interfaceId) const override;
			//-------------------------------------------
			/// @author S Downie
			///
			/// @param Particle Component
			//-------------------------------------------
			void AddParticleComponent(ParticleComponent* in_particle);
			//-------------------------------------------
			/// @author S Downie
			///
			/// @param Particle Component
			//-------------------------------------------
			void RemoveParticleComponent(ParticleComponent* in_particle);
			//-------------------------------------------
			/// Sets a factor to scale update delta time by.
            /// Useful for pausing, slowing down and speeding
            /// up all effects.
            ///
            /// @author S Downie
            ///
            /// @param Time scaler
			//-------------------------------------------
			void SetTimeScaleFactor(f32 in_scale);
			//----------------------------------------------------
			/// @author S Downie
			///
			/// @return Number of factories in this system
			//----------------------------------------------------
			u32 GetNumComponentFactories() const override;
			//-------------------------------------------
			/// @author S Downie
			///
			/// @return Particle component factory
			//-------------------------------------------
			Core::ComponentFactory* GetComponentFactoryPtr(u32 in_index) override;
			//-------------------------------------------
			/// @author S Downie
			///
			/// @return Particle component factory
			//-------------------------------------------
			Core::ComponentFactory& GetComponentFactory(u32 in_index) override;
            //-------------------------------------------
			/// @author S Downie
			///
			/// @return Particle Emitter factory
			//-------------------------------------------
            ParticleEmitterFactory& GetEmitterFactory();
            //-------------------------------------------
			/// @author S Downie
			///
			/// @return Particle Effector factory
			//-------------------------------------------
            ParticleEffectorFactory& GetEffectorFactory();
            
        private:
            
            //----------------------------------------------------------
            /// Private constructor to force use of factory method
            ///
            /// @author S Downie
            //----------------------------------------------------------
			ParticleSystem();
            //----------------------------------------------------------
			/// @author S Downie
			///
            /// Called when the system is created
			//----------------------------------------------------------
			void OnInit() override;
            //-------------------------------------------
			/// Update all the particle components
			///
            /// @author S Downie
            ///
			/// @param Time since last update in seconds
			//-------------------------------------------
			void OnUpdate(f32 in_timeSinceLastUpdate) override;
            //----------------------------------------------------------
			/// @author S Downie
			///
            /// Called when the system is destroyed
			//----------------------------------------------------------
			void OnDestroy() override;
			
		private:
			
            ParticleEmitterFactory m_emitterFactory;
            ParticleEffectorFactory m_effectorFactory;
            ParticleComponentFactory m_particleFactory;
            
			std::vector<ParticleComponent*> m_particleComponents;
            
            f32 m_timeScaleFactor;
		};
	}
}

#endif