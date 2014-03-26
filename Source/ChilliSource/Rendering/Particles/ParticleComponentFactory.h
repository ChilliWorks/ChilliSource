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
		class ParticleComponentFactory : public Core::ComponentFactory
		{
		public: 
			ParticleComponentFactory(ParticleSystem* inpParticleSystem, ParticleEmitterFactory* inpEmitterFactory, ParticleEffectorFactory* inpEffectorFactory);
            
			CS_DECLARE_NAMEDTYPE(ParticleComponentFactory);
            
            //-------------------------------------------------------
            /// Is A
            ///
            /// @param Interface ID
            /// @return Whether the object is of given type
            //-------------------------------------------------------
			bool IsA(Core::InterfaceIDType inInterfaceID) const override;
			//--------------------------------------------------------
			/// Can Produce Component With Interface
			///
			/// Used to determine if this factory can produce 
			/// component of given type.
			///
			/// @param The ID of the component to create
			/// @return Whether the object can create component of ID
			//--------------------------------------------------------
			bool CanProduceComponentWithInterface(Core::InterfaceIDType inTypeID) const override;
            //----------------------------------------------------------------------------
            /// Can Produce Component With Type Name
            ///
            /// @param Type name
            /// @return Whether the factory can produce components with the given name
            //----------------------------------------------------------------------------
			bool CanProduceComponentWithTypeName(const std::string & incName) const override;
			//--------------------------------------------------------
			/// Create Particle Component
			///
			/// Creates a default particle effect for customisation
			///
			/// @return Particle Component
			//--------------------------------------------------------
			ParticleComponentUPtr CreateParticleComponent();
            //--------------------------------------------------------
            /// Create Particle Component From Script
            ///
            /// Creates a particle component using an external script
            ///
            /// @param The storage location to load from
            /// @param The filepath
            /// @return Particle Component
            //--------------------------------------------------------
            ParticleComponentUPtr CreateParticleComponentFromScript(Core::StorageLocation ineStorageLocation, const std::string& instrScriptFile);
			
		private:
			
			ParticleSystem* mpParticleSystem;
            MaterialManager* mpMaterialMgr;
            
            ParticleEffectorFactory* mpEffectorFactory;
            ParticleEmitterFactory* mpEmitterFactory;
		};
	}
}

#endif