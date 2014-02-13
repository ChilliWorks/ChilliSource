//
//  BulletCollisionMeshLoader.h
//  moFlow
//
//  Created by Ian Copland on 29/05/2012.
//  Copyright (c) 2012 Tag Games Ltd. All rights reserved.
//

#ifndef _MOFLOW_PHYSICS3D_BULLETPHYSICS_BULLETCOLLISIONMESHLOADER_H_
#define _MOFLOW_PHYSICS3D_BULLETPHYSICS_BULLETCOLLISIONMESHLOADER_H_

#include <ChilliSource/Physics3D/BulletPhysics/ForwardDeclarations.h>
#include <ChilliSource/Core/ResourceProvider.h>

namespace moFlo
{
    namespace Physics3D
    {
        //==============================================================
        /// Bullet Collision Mesh Loader
        ///
        /// Loads bullet collision meshes from a mocollision file.
        //==============================================================
        class CBulletCollisionMeshLoader : public Core::IResourceProvider
        {
        public:
            //----------------------------------------------------------
            /// Constructor
            //----------------------------------------------------------
            CBulletCollisionMeshLoader();
            //----------------------------------------------------------
			/// Is A
			///
			/// @param Interface to compare
			/// @return Whether the object implements the given interface
			//----------------------------------------------------------
			bool IsA(Core::InterfaceIDType inInterfaceID) const;
            //----------------------------------------------------------
			/// Can Create Resource of Kind
			///
			/// @param Type to compare
			/// @return Whether the object can create a resource of 
            ///         given type
			//----------------------------------------------------------
			bool CanCreateResourceOfKind(Core::InterfaceIDType inInterfaceID) const;
			//----------------------------------------------------------
			/// Can Create Resource From File With Extension
			///
			/// @param Extension to compare
			/// @return Whether the object can create a resource with the 
            ///         given extension
			//----------------------------------------------------------
			bool CanCreateResourceFromFileWithExtension(const std::string & inExtension) const;
            //----------------------------------------------------------
            /// Destructor
            //----------------------------------------------------------
            virtual ~CBulletCollisionMeshLoader();
            
        protected:
            //----------------------------------------------------------
			/// Create Resource From File
			///
            /// @param The storage location to load from
			/// @param Filename
			/// @param the output resource pointer
			/// @return whether or not this was successful
			//----------------------------------------------------------
			bool CreateResourceFromFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string& instrFilePath, Core::ResourcePtr& outpResource);
			//----------------------------------------------------------
			/// Async Create Resource From File
			///
            /// @param The storage location to load from
			/// @param Filename
			/// @param the output resource pointer
			/// @return whether or not this was successful
			//----------------------------------------------------------
			bool AsyncCreateResourceFromFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string& instrFilePath, Core::ResourcePtr& outpResource);
			//----------------------------------------------------------
			/// Create Collision Mesh From File
			///
            /// @param The storage location to load from
			/// @param Filename
			/// @param the output resource pointer
			/// @return whether or not this was successful
			//----------------------------------------------------------
			bool CreateCollisionMeshFromFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string &instrFilePath, BulletCollisionMeshPtr& outpResource);
			//----------------------------------------------------------
			/// Async Create Collision Mesh From File
			///
            /// @param The storage location to load from
			/// @param Filename
			/// @param the output resource pointer
			/// @return whether or not this was successful
			//----------------------------------------------------------
			bool AsyncCreateCollisionMeshFromFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string &instrFilePath, BulletCollisionMeshPtr& outpResource);
            //----------------------------------------------------------
			/// Read File Task
			///
			/// Reads the mesh data of a MoCollision file as a background
            /// task.
			///
			/// @param The storage location to load from
			/// @param Filename
			/// @param the output resource pointer
			//----------------------------------------------------------
			void ReadFileTask(Core::STORAGE_LOCATION ineStorageLocation, const std::string &instrFilePath, BulletCollisionMeshPtr& outpResource);
            //----------------------------------------------------------
			/// Read File
			///
			/// Reads the mesh data of a MoCollision file
			///
			/// @param The storage location to load from
			/// @param Filename
			/// @param the output resource pointer
			//----------------------------------------------------------
			bool ReadFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string &instrFilePath, BulletCollisionMeshPtr& outpResource);
            //----------------------------------------------------------
			/// Read Value (Templated)
			///
			/// Read value in for given type
            ///
			/// @param File stream
			/// @return Value of type T
			//----------------------------------------------------------
			template <typename T> T ReadValue(moFlo::Core::FileStreamPtr inpFileStream)
			{
				T Value;
				inpFileStream->Read(reinterpret_cast<s8*>(&Value), sizeof(T));
				return Value;
			}
            
            friend class CBulletCollisionMeshManager;
        };
    }
}

#endif

