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

#include <ChilliSource/Core/Entity/ComponentFactory.h>
#include <ChilliSource/Core/File/FileSystem.h>
 
#include <ChilliSource/Rendering/ForwardDeclarations.h>

#include <ChilliSource/Rendering/Particles/Effectors/ParticleEffectorFactory.h>
#include <ChilliSource/Rendering/Particles/Emitters/ParticleEmitterFactory.h>

namespace moFlo
{
	namespace Rendering
	{
		class CParticleComponentFactory : public Core::IComponentFactory
		{
		public: 
			CParticleComponentFactory(CParticleSystem* inpParticleSystem, CParticleEmitterFactory* inpEmitterFactory, CParticleEffectorFactory* inpEffectorFactory);
            
			DECLARE_NAMED_INTERFACE(CParticleComponentFactory);
            
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
			ParticleComponentPtr CreateParticleComponent();
            //--------------------------------------------------------
            /// Create Particle Component From Script
            ///
            /// Creates a particle component using an external script
            ///
            /// @param The storage location to load from
            /// @param The filepath
            /// @return Particle Component
            //--------------------------------------------------------
            ParticleComponentPtr CreateParticleComponentFromScript(Core::STORAGE_LOCATION ineStorageLocation, const std::string& instrScriptFile);
            //--------------------------------------------------------
            /// Register Emitter Type
            ///
            /// Register a custom emitter type
            ///
            /// @param Delegate
            /// @param Emitter type name
            //--------------------------------------------------------
            void RegisterEmitterType(const CParticleEmitterFactory::EmitterCreateDelegate& inDelegate, const std::string& instrName);
            //--------------------------------------------------------
            /// Register Effector Type
            ///
            /// Register a custom effector type
            ///
            /// @param Delegate
            /// @param Effector type name
            //--------------------------------------------------------
            void RegisterEffectorType(const CParticleEffectorFactory::EffectorCreateDelegate& inDelegate, const std::string& instrName);
			
		private:
			
			CParticleSystem* mpParticleSystem;
            IMaterialManager* mpMaterialMgr;
            
            CParticleEffectorFactory* mpEffectorFactory;
            CParticleEmitterFactory* mpEmitterFactory;
		};
	}
}

#endif