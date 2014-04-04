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
		class MeshManager : public Core::ResourceManager
		{
		public:
			CS_DECLARE_NAMEDTYPE(MeshManager);
			
			MeshManager();
			virtual ~MeshManager();
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
			Core::ResourceOldSPtr GetResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string &instrFilePath) override;
			//-----------------------------------------------------------------
			/// Async Get Resource From File
			///
			/// Generic call to get the managers resource
            /// @param The storage location to load from
			/// @param File path to resource
			/// @return Generic pointer to object type
			//-----------------------------------------------------------------
			Core::ResourceOldSPtr AsyncGetResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string &instrFilePath) override;
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
			MeshSPtr GetModelFromFile(Core::StorageLocation ineStorageLocation, const std::string &inFilePath);
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
			MeshSPtr AsyncGetModelFromFile(Core::StorageLocation ineStorageLocation, const std::string &inFilePath);
			//----------------------------------------------------------------
			/// Create Empty Model
			///
			/// Creates a new empty model which can be later populated with
			/// data using PopulateModelFromFile
			//----------------------------------------------------------------
			MeshUPtr CreateEmptyMesh(u32 indwNumSubMeshes, u32 inudwVertexDataSize, u32 indwIndexDataSize);
            //----------------------------------------------------------------
            /// Create Manual Mesh
            ///
            /// @param Mesh descriptor containing the vertex data etx
            /// @return Mesh resource
            //----------------------------------------------------------------
			MeshUPtr CreateManualMesh(const MeshDescriptor& insMeshDescriptor);
			//----------------------------------------------------------------------------
			/// BuildMesh
			///
			/// Builds the mesh from the MeshDescriptor. This will return failure if any of the submeshes have previously
			/// been prepared and have vertex or index capacities that are too small.
			///
			/// @param The MoModel Declaration used to build the mesh
			/// @param the mesh resource
			/// @return success or failure.
			//----------------------------------------------------------------------------
			static bool BuildMesh(RenderSystem* inpRenderSystem, const MeshDescriptor& inMeshDescriptor, Mesh* outpResource);
            //----------------------------------------------------------------------------
            /// Get Default Mesh
            ///
            /// @return A default cube mesh. This will be returned if no mesh can be found
            //----------------------------------------------------------------------------
            MeshSPtr GetDefaultMesh();
            
        private:
            
            static MeshSPtr mpDefaultMesh;
		};
	}
}

#endif