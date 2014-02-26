/*
 *  MeshManager.h
 *  moFlo
 *
 *  Created by Scott Downie on 06/10/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#ifndef _MO_FLO_RENDERING_MESH_MANAGER_H_
#define _MO_FLO_RENDERING_MESH_MANAGER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Resource/ResourceManager.h>
#include <ChilliSource/Rendering/Model/Mesh.h>
#include <ChilliSource/Rendering/Model/MeshDescriptor.h>

namespace ChilliSource
{
	namespace Rendering
	{
		//------------------------------------
		/// Mesh Manager
		//------------------------------------
		class CMeshManager : public Core::IResourceManager
		{
		public:
			DECLARE_NAMED_INTERFACE(CMeshManager);
			
			CMeshManager();
			virtual ~CMeshManager();
			//----------------------------------------------------------------
			/// Is A
			///
			/// Query the interface type
			/// @param The interface to compare
			/// @return Whether the object implements that interface
			//----------------------------------------------------------------
			virtual bool IsA(Core::InterfaceIDType inInterfaceID) const override;
			//----------------------------------------------------------------
			/// Get Resource Type
			///
			/// @return The type of resource this manager handles
			//----------------------------------------------------------------
			virtual Core::InterfaceIDType GetResourceType() const override;
			//----------------------------------------------------------------
			/// Get Provider Type
			///
			/// @return The type of resource it consumes from resource provider
			//----------------------------------------------------------------
			virtual Core::InterfaceIDType GetProviderType() const override;
			//----------------------------------------------------------------
			/// Manages Resource Of Type
			///
			/// @return Whether this object manages the object of type
			//----------------------------------------------------------------
			virtual bool ManagesResourceOfType(Core::InterfaceIDType inInterfaceID) const override;
			//-----------------------------------------------------------------
			/// Get Resource From File
			///
			/// Generic call to get the managers resource
            /// @param The storage location to load from
			/// @param File path to resource
			/// @return Generic pointer to object type
			//-----------------------------------------------------------------
			Core::ResourcePtr GetResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string &instrFilePath) override;
			//-----------------------------------------------------------------
			/// Async Get Resource From File
			///
			/// Generic call to get the managers resource
            /// @param The storage location to load from
			/// @param File path to resource
			/// @return Generic pointer to object type
			//-----------------------------------------------------------------
			Core::ResourcePtr AsyncGetResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string &instrFilePath) override;
			//----------------------------------------------------------------
			/// Get Model From File
			///
			/// Creates (lazily loads) a model instance from file. If the 
			/// model is already loaded it returns a handle to the model
            ///
            /// @param The storage location to load from
            /// @param the filepath
            /// @return the resource pointer to the mesh
			//----------------------------------------------------------------
			MeshPtr GetModelFromFile(Core::StorageLocation ineStorageLocation, const std::string &inFilePath);
			//----------------------------------------------------------------
			/// Async Get Model From File
			///
			/// Creates (lazily loads) a model instance from file. If the 
			/// model is already loaded it returns a handle to the model
            ///
            /// @param The storage location to load from
            /// @param the filepath
            /// @return the resource pointer to the mesh
			//----------------------------------------------------------------
			MeshPtr AsyncGetModelFromFile(Core::StorageLocation ineStorageLocation, const std::string &inFilePath);
			//----------------------------------------------------------------
			/// Create Empty Model
			///
			/// Creates a new empty model which can be later populated with
			/// data using PopulateModelFromFile
			//----------------------------------------------------------------
			MeshPtr CreateEmptyMesh(u32 indwNumSubMeshes, u32 inudwVertexDataSize, u32 indwIndexDataSize);
            //----------------------------------------------------------------
            /// Create Manual Mesh
            ///
            /// @param Mesh descriptor containing the vertex data etx
            /// @return Mesh resource
            //----------------------------------------------------------------
			MeshPtr CreateManualMesh(MeshDescriptor& insMeshDescriptor);
			//----------------------------------------------------------------------------
			/// BuildMesh
			///
			/// Builds the mesh from the MeshDescriptor. This will return failure if any of the submeshes have previously
			/// been prepared and have vertex or index capacities that are too small.
			///
			/// @param The MoModel Declaration used to build the mesh
			/// @param the mesh resource
			/// @param whether or not the mesh needs prepared. Defaults to true.
			/// @return success or failure.
			//----------------------------------------------------------------------------
			static bool BuildMesh(IRenderSystem* inpRenderSystem, MeshDescriptor& inMeshDescriptor, const MeshPtr& outpResource, bool inbNeedsPrepared = true);
            //----------------------------------------------------------------------------
            /// Get Default Mesh
            ///
            /// @return A default cube mesh. This will be returned if no mesh can be found
            //----------------------------------------------------------------------------
            MeshPtr GetDefaultMesh();
            
        private:
            
            static MeshPtr mpDefaultMesh;
		};
	}
}

#endif