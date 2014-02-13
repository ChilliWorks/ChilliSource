//
//  BulletCollisionMeshManager.h
//  moFlow
//
//  Created by Ian Copland on 29/05/2012.
//  Copyright (c) 2012 Tag Games Ltd. All rights reserved.
//

#ifndef _MOFLOW_PHYSICS3D_BULLETPHYSICS_BULLETCOLLISIONMESHMANAGER_H_
#define _MOFLOW_PHYSICS3D_BULLETPHYSICS_BULLETCOLLISIONMESHMANAGER_H_

#include <ChilliSource/Physics3D/BulletPhysics/ForwardDeclarations.h>
#include <ChilliSource/Core/ResourceManager.h>

namespace moFlo
{
    namespace Physics3D
    {
        //====================================================================
		/// Bullet Collision Mesh Manager
        ///
        /// Handles all of the bullet collision mesh resources.
		//====================================================================
		class CBulletCollisionMeshManager : public Core::IResourceManager
		{
		public:
			DECLARE_NAMED_INTERFACE(CBulletCollisionMeshManager);
			
            //----------------------------------------------------------------
            /// Constructor
            //----------------------------------------------------------------
			CBulletCollisionMeshManager();
			//----------------------------------------------------------------
			/// Is A
			///
			/// Query the interface type
            ///
			/// @param The interface to compare
			/// @return Whether the object implements that interface
			//----------------------------------------------------------------
			bool IsA(Core::InterfaceIDType inInterfaceID) const;
			//----------------------------------------------------------------
			/// Get Resource Type
			///
			/// @return The type of resource this manager handles
			//----------------------------------------------------------------
			Core::InterfaceIDType GetResourceType() const;
			//----------------------------------------------------------------
			/// Get Provider Type
			///
			/// @return The type of resource it consumes from resource provider
			//----------------------------------------------------------------
			Core::InterfaceIDType GetProviderType() const; 
			//----------------------------------------------------------------
			/// Manages Resource Of Type
			///
			/// @return Whether this object manages the object of type
			//----------------------------------------------------------------
			bool ManagesResourceOfType(Core::InterfaceIDType inInterfaceID) const;;
			//-----------------------------------------------------------------
			/// Get Resource From File
			///
			/// Generic call to get the managers resource
            ///
            /// @param The storage location to load from
			/// @param File path to resource
			/// @return Generic pointer to object type
			//-----------------------------------------------------------------
			Core::ResourcePtr GetResourceFromFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string& instrFilePath);
			//-----------------------------------------------------------------
			/// Async Get Resource From File
			///
			/// Generic call to get the managers resource
            ///
            /// @param The storage location to load from
			/// @param File path to resource
			/// @return Generic pointer to object type
			//-----------------------------------------------------------------
			Core::ResourcePtr AsyncGetResourceFromFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string& instrFilePath);
			//----------------------------------------------------------------
			/// Get Collision Mesh From File
			///
			/// Creates (lazily loads) a bullet collision mesh instance from 
            /// file. If the collision mesh is already loaded it returns a 
            /// handle to the collision mesh.
            ///
            /// @param The storage location to load from
            /// @param the filepath
            /// @return the resource pointer to the collision mesh
			//----------------------------------------------------------------
			BulletCollisionMeshPtr GetCollisionMeshFromFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string& instrFilePath);
			//----------------------------------------------------------------
			/// Async Get Collision Mesh From File
			///
			/// Creates (lazily loads) a bullet collision mesh instance from 
            /// file. If the collision mesh is already loaded it returns a 
            /// handle to the collision mesh.
            ///
            /// @param The storage location to load from
            /// @param the filepath
            /// @return the resource pointer to the collsion mesh
			//----------------------------------------------------------------
			BulletCollisionMeshPtr AsyncGetCollisionMeshFromFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string& instrFilePath);
			//----------------------------------------------------------------
            /// Destructor
            //----------------------------------------------------------------
           virtual ~CBulletCollisionMeshManager();
        };
    }
}

#endif
