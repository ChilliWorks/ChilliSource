/*
 *  MoModelLoader.h
 *  moFlo
 *
 *  Created by Ian Copland on 25/08/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#ifndef _MO_FLO_RENDERING_MOMODEL_LOADER_H_
#define _MO_FLO_RENDERING_MOMODEL_LOADER_H_

#include <ChilliSource/Core/Resource/ResourceProvider.h>
#include <ChilliSource/Core/File/FileSystem.h>
#include <ChilliSource/Core/Math/Quaternion.h>
#include <ChilliSource/Rendering/Model/MeshManager.h>
#include <ChilliSource/Core/Math/Vector3.h>
#include <ChilliSource/Rendering/Base/VertexDeclaration.h>
#include <ChilliSource/Rendering/Model/MeshDescriptor.h>

namespace ChilliSource
{
	namespace Rendering
	{
		//-------------------------------------------------------------------------
		/// CMoModelLoader class.
		/// Loads mo model files into a CMesh resource.
		//-------------------------------------------------------------------------
		class CMoModelLoader : public Core::IResourceProvider
		{
		public:
			CMoModelLoader(Core::CApplication* inpApp);
			virtual ~CMoModelLoader();
			
			//-------------------------------------------------------------------------
			/// Is A
			///
			/// @param Interface to compare
			/// @return Whether the object implements the given interface
			//-------------------------------------------------------------------------
			bool IsA(Core::InterfaceIDType inInterfaceID) const;
			//----------------------------------------------------------------------------
			/// Can Create Resource of Kind
			///
			/// @param Type to compare
			/// @return Whether the object can create a resource of given type
			//----------------------------------------------------------------------------
			bool CanCreateResourceOfKind(Core::InterfaceIDType inInterfaceID) const;
			//----------------------------------------------------------------------------
			/// Can Create Resource From File With Extension
			///
			/// @param Extension to compare
			/// @return Whether the object can create a resource with the given extension
			//----------------------------------------------------------------------------
			bool CanCreateResourceFromFileWithExtension(const std::string & inExtension) const;
			//----------------------------------------------------------------------------
			/// Populate Existing Mesh From File
			///
			/// Fills an existing CMesh with data from a file. This will overwrite any
			/// Previous data in the CMesh. The sub meshes in the mesh must already have been
			/// initialised and must be big enough to fit the new data. If they are too small,
			/// an error will be thrown and the output mesh will be empty.
			///
			/// @param the existing model pointer
            /// @param The storage location to load from
			/// @param Filename
			/// @param The path to this files materials. Assumed to be the same directory as the file if left blank.
			//----------------------------------------------------------------------------
			void PopulateExistingMeshFromFile(const MeshPtr& inpMesh, Core::StorageLocation ineStorageLocation, const std::string &inFilePath, const std::string& inMaterialPath = "");
			//----------------------------------------------------------------------------
			/// Async Populate Existing Mesh From File
			///
			/// Fills an existing CMesh with data from a file. This will overwrite any
			/// Previous data in the CMesh. The sub meshes in the mesh must already have been
			/// initialised and must be big enough to fit the new data. If they are too small,
			/// an error will be thrown and the output mesh will be empty. This is all handled
			/// in the background, asynchonously.
			///
			/// @param the existing model pointer
            /// @param The storage location to load from
			/// @param Filename
			/// @param The path to this files materials. Assumed to be the same directory as the file if left blank.
			//----------------------------------------------------------------------------
			void AsyncPopulateExistingMeshFromFile(const MeshPtr& inpMesh, Core::StorageLocation ineStorageLocation, const std::string &inFilePath, const std::string& inMaterialPath = "");
		private:
            //----------------------------------------------------------------------------
            /// Mesh Data Quantities
            ///
            /// A struct containing the quantities of different peices of mesh data.
            //----------------------------------------------------------------------------
			struct MeshDataQuantities
            {
                u32 mudwNumMeshes;
                s32 mdwNumSkeletonNodes;
                u32 mudwNumJoints;
            };
			//----------------------------------------------------------------------------
			/// Create Resource From File
			///
            /// @param The storage location to load from
			/// @param Filename
			/// @param the output resource pointer
			/// @return whether or not this was successful
			//----------------------------------------------------------------------------
			bool CreateResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string & inFilePath, Core::ResourcePtr& outpResource);
			//----------------------------------------------------------------------------
			/// Async Create Resource From File
			///
            /// @param The storage location to load from
			/// @param Filename
			/// @param the output resource pointer
			/// @return whether or not this was successful
			//----------------------------------------------------------------------------
			bool AsyncCreateResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string & inFilePath, Core::ResourcePtr& outpResource);
			//----------------------------------------------------------------------------
			/// Create Mesh From File
			///
            /// @param The storage location to load from
			/// @param Filename
			/// @param The path to this files materials. Assumed to be the same directory as the file if left blank.
			/// @param the output resource pointer
			/// @return whether or not this was successful
			//----------------------------------------------------------------------------
			bool CreateMeshFromFile(Core::StorageLocation ineStorageLocation, const std::string &inFilePath, const std::string& instrMaterialPath, const MeshPtr& outpResource);
			//----------------------------------------------------------------------------
			/// Async Create Mesh From File
			///
            /// @param The storage location to load from
			/// @param Filename
			/// @param The path to this files materials. Assumed to be the same directory as the file if left blank.
			/// @param the output resource pointer
			/// @return whether or not this was successful
			//----------------------------------------------------------------------------
			bool AsyncCreateMeshFromFile(Core::StorageLocation ineStorageLocation, const std::string &inFilePath, const std::string& instrMaterialPath, const MeshPtr& outpResource);
			//----------------------------------------------------------------------------
			/// LoadMeshDataTask
			///
            /// @param The storage location to load from
			/// @param Filename
			/// @param The path to this files materials. Assumed to be the same directory as the file if left blank.
			/// @param the output resource pointer
			//----------------------------------------------------------------------------
			void LoadMeshDataTask(Core::StorageLocation ineStorageLocation, const std::string &inFilePath, const std::string& instrMaterialPath, const MeshPtr& outpResource);
			//----------------------------------------------------------------------------
			/// Build Mesh Task
			///
			/// @param the mesh declaration
			/// @param the output resource pointer
			//----------------------------------------------------------------------------
			void BuildMeshTask(MeshDescriptor& inMeshDescriptor, const MeshPtr& outpResource);
			//----------------------------------------------------------------------------
			/// LoadDataForExistingMeshTask
			///
            /// @param The storage location to load from
			/// @param Filename
			/// @param The path to this files materials. Assumed to be the same directory as the file if left blank.
			/// @param the output resource pointer
			//----------------------------------------------------------------------------
			void LoadDataForExistingMeshTask(Core::StorageLocation ineStorageLocation, const std::string &inFilePath,  const std::string& instrMaterialPath, const MeshPtr& outpResource);
			//----------------------------------------------------------------------------
			/// Build Existing Mesh Task
			///
			/// @param the mesh declaration
			/// @param the output resource pointer
			//----------------------------------------------------------------------------
			void BuildExistingMeshTask(MeshDescriptor& inMeshDescriptor, const MeshPtr& outpResource);
			//----------------------------------------------------------------------------
			/// BuildMesh
			///
			/// Calls BuildMesh in the mesh manager, then cleans up the MeshDescriptor.
			///
			/// @param The MeshDescriptor used to build the mesh
			/// @param the mesh resource
			/// @param whether or not the mesh needs prepared. Defaults to true.
			/// @return success or failure.
			//----------------------------------------------------------------------------
			bool BuildMesh(MeshDescriptor& inMeshDescriptor, const MeshPtr& outpResource, bool inbNeedsPrepared = true);
			//----------------------------------------------------------------------------
			/// Read File
			///
			/// Read the mesh data from file and creates a MoStaticMeshDeclaration.
			///
            /// @param The storage location to load from
			/// @param Filename
			/// @param The path to this files materials. Assumed to be the same directory as the file if left blank.
			/// @param The output MoModelDeclaration
			/// @return true if successful, false if not
			//----------------------------------------------------------------------------
			bool ReadFile(Core::StorageLocation ineStorageLocation, const std::string &inFilePath, const std::string& instrMaterialPath, MeshDescriptor& outDeclaration);
			//-----------------------------------------------------------------------------
			/// Read Global Header
			///
			/// Reads the header of a MoModel file
			///
			/// @param File stream
			/// @param the output MoModel Declaration
			/// @param the file path
            /// @param OUT: A struct containing info on the number of meshes, nodes and joints.
			/// @return Whether the file is correct
			//-----------------------------------------------------------------------------
			bool ReadGlobalHeader(const ChilliSource::Core::FileStreamPtr& inpStream, MeshDescriptor& inMeshDescriptor, const std::string &inFilePath, MeshDataQuantities& outMeshDataQuantities);
			//-----------------------------------------------------------------------------
			/// Read Skeleton
			///
			/// Reads the skeleton section of a MoModel file
			///
			/// @param File stream
			/// @param the output MoModel Declaration
			/// @return Whether the file is correct
			//-----------------------------------------------------------------------------
			bool ReadSkeletonData(const ChilliSource::Core::FileStreamPtr& inpStream, const MeshDataQuantities& inQuantities, MeshDescriptor& inMeshDescriptor);
			//-----------------------------------------------------------------------------
			/// Read Mesh Header
			///
			/// Reads the mesh header section of a MoModel file
			///
			/// @param File stream
			/// @param the output MoModel Declaration
			/// @return Whether the file is correct
			//-----------------------------------------------------------------------------
			bool ReadMeshHeader(const ChilliSource::Core::FileStreamPtr& inpStream, MeshDescriptor& inMeshDescriptor, SubMeshDescriptor& outSubMeshDescriptor,
								const std::string& instrMaterialPath);
			//-----------------------------------------------------------------------------
			/// Read Mesh Data
			///
			/// Reads the mesh data of a MoModel file
			///
			/// @param File stream
			/// @param the output MoModel Declaration
			/// @return Whether the file is correct
			//-----------------------------------------------------------------------------
			bool ReadMeshData(const ChilliSource::Core::FileStreamPtr& inpStream, MeshDescriptor& inMeshDescriptor, SubMeshDescriptor& outSubMeshDescriptor);
			//-----------------------------------------------------------------------------
			/// Read Vertex Declaration
			///
			/// Reads the vertex declaration from the file header and stores in in the
			/// MoModel declaration.
			///
			/// @param File stream
			/// @param MoModel declaration
			//-----------------------------------------------------------------------------
			void ReadVertexDeclaration(const ChilliSource::Core::FileStreamPtr& inpStream, MeshDescriptor& inMeshDescriptor);
			//----------------------------------------------------------------------------
			/// Read Value (Templated)
			///
			/// Read value in for given type
			/// @param File stream
			/// @return Value of type T
			//----------------------------------------------------------------------------
			template <typename T> T ReadValue(const ChilliSource::Core::FileStreamPtr& inpFileStream)
			{
				T Value;
				inpFileStream->Read(reinterpret_cast<s8*>(&Value), sizeof(T));
				return Value;
			}
		private:	
			
			Core::CApplication* mpApp;
			friend class CMeshManager;
		};
	}
}

#endif
