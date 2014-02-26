/*
 *  ParticleComponentFactory.h
 *  moFloTest
 *
 *  Created by Scott Downie on 17/01/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#ifndef _MO_FLO_RENDERING_PARTICLE_COMPONENT_FACTORY_H_
#define _MO_FLO_RENDERING_PARTICLE_COMPONENT_FACTORY_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Entity/ComponentFactory.h>
#include <ChilliSource/Core/File/FileSystem.h>
#include <ChilliSource/Rendering/Particles/Effectors/ParticleEffectorFactory.h>
#include <ChilliSource/Rendering/Particles/Emitters/ParticleEmitterFactory.h>

namespace ChilliSource
{
	namespace Rendering
	{
		class ParticleComponentFactory : public Core::IComponentFactory
		{
		public: 
			ParticleComponentFactory(ParticleSystem* inpParticleSystem, ParticleEmitterFactory* inpEmitterFactory, ParticleEffectorFactory* inpEffectorFactory);
            
			DECLARE_NAMED_INTERFACE(ParticleComponentFactory);
            
            //-------------------------------------------------------
            /// Is A
            ///
            /// @param Interface ID
            /// @return Whether the object is of given type
            //-------------------------------------------------------
			bool IsA(Core::InterfaceIDType inInterfaceID) const;
			//--------------------------------------------------------
			/// Can Produce Component With Interface
			///
			/// Used to determine if this factory can produce 
			/// component of given type.
			///
			/// @param The ID of the component to create
			/// @return Whether the object can create component of ID
			//--------------------------------------------------------
			bool CanProduceComponentWithInterface(Core::InterfaceIDType inTypeID) const;
            //----------------------------------------------------------------------------
            /// Can Produce Component With Type Name
            ///
            /// @param Type name
            /// @return Whether the factory can produce components with the given name
            //----------------------------------------------------------------------------
			bool CanProduceComponentWithTypeName(const std::string & incName) const;
			//--------------------------------------------------------
			/// Create Particle Component
			///
			/// Creates a default particle effect for customisation
			///
			/// @return Particle Component
			//--------------------------------------------------------
			ParticleComponentSPtr CreateParticleComponent();
            //--------------------------------------------------------
            /// Create Particle Component From Script
            ///
            /// Creates a particle component using an external script
            ///
            /// @param The storage location to load from
            /// @param The filepath
            /// @return Particle Component
            //--------------------------------------------------------
            ParticleComponentSPtr CreateParticleComponentFromScript(Core::StorageLocation ineStorageLocation, const std::string& instrScriptFile);
            //--------------------------------------------------------
            /// Register Emitter Type
            ///
            /// Register a custom emitter type
            ///
            /// @param Delegate
            /// @param Emitter type name
            //--------------------------------------------------------
            void RegisterEmitterType(const ParticleEmitterFactory::EmitterCreateDelegate& inDelegate, const std::string& instrName);
            //--------------------------------------------------------
            /// Register Effector Type
            ///
            /// Register a custom effector type
            ///
            /// @param Delegate
            /// @param Effector type name
            //--------------------------------------------------------
            void RegisterEffectorType(const ParticleEffectorFactory::EffectorCreateDelegate& inDelegate, const std::string& instrName);
			
		private:
			
			ParticleSystem* mpParticleSystem;
            MaterialManager* mpMaterialMgr;
            
            ParticleEffectorFactory* mpEffectorFactory;
            ParticleEmitterFactory* mpEmitterFactory;
		};
	}
}

#endif