/*
 *  MeshManager.cpp
 *  moFlo
 *
 *  Created by Scott Downie on 06/10/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Rendering/Model/MeshManager.h>
#include <ChilliSource/Core/Resource/ResourceProvider.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Rendering/Model/SubMesh.h>

namespace ChilliSource
{
	namespace Rendering
	{
		MeshPtr CMeshManager::mpDefaultMesh;
        
		DEFINE_NAMED_INTERFACE(CMeshManager);
		
		//-----------------------------------------------------------------
		/// Constructor
		///
		/// Default
		//-----------------------------------------------------------------
		CMeshManager::CMeshManager()
		{
		}
		//----------------------------------------------------------------
		/// Is A
		///
		/// Query the interface type
		/// @param The interface to compare
		/// @return Whether the object implements that interface
		//----------------------------------------------------------------
		bool CMeshManager::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == CMeshManager::InterfaceID;
		}
		//----------------------------------------------------------------
		/// Get Resource Type
		///
		/// @return The type of resource this manager handles
		//----------------------------------------------------------------
		Core::InterfaceIDType CMeshManager::GetResourceType() const
		{
			return CMesh::InterfaceID;
		}
		//----------------------------------------------------------------
		/// Manages Resource Of Type
		///
		/// @return Whether this object manages the object of type
		//----------------------------------------------------------------
		bool CMeshManager::ManagesResourceOfType(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == CMesh::InterfaceID;
		}
		//----------------------------------------------------------------
		/// Get Provider Type
		///
		/// @return The type of resource it consumes from resource provider
		//----------------------------------------------------------------
		Core::InterfaceIDType CMeshManager::GetProviderType() const
		{
			return CMesh::InterfaceID;
		}
		//-----------------------------------------------------------------
		/// Get Resource From File
		///
		/// Generic call to get the managers resource
        /// @param The storage location to load from
		/// @param File path to resource
		/// @return Generic pointer to object type
		//-----------------------------------------------------------------
		Core::ResourcePtr CMeshManager::GetResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string &instrFilePath)
		{
			return GetModelFromFile(ineStorageLocation, instrFilePath);
		}
		//-----------------------------------------------------------------
		/// Async Get Resource From File
		///
		/// Generic call to get the managers resource
        /// @param The storage location to load from
		/// @param File path to resource
		/// @return Generic pointer to object type
		//-----------------------------------------------------------------
		Core::ResourcePtr CMeshManager::AsyncGetResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string &instrFilePath)
		{
			return AsyncGetModelFromFile(ineStorageLocation, instrFilePath);
		}
		//----------------------------------------------------------------
		/// Get Model From File
		///
		/// Creates (lazily loads) a model instance from file. If the 
		/// model it already loaded it returns a handle to the model
        /// @param The storage location to load from
        /// @param the filepath
        /// @return the resource pointer to the mesh
		//----------------------------------------------------------------
		MeshPtr CMeshManager::GetModelFromFile(Core::StorageLocation ineStorageLocation, const std::string &inFilePath)
		{
			MapStringToResourcePtr::iterator pExistingResource = mMapFilenameToResource.find(inFilePath);
			
			if(pExistingResource == mMapFilenameToResource.end()) 
			{
				Core::ResourcePtr pResource(new CMesh());
				
				for(u32 nProvider = 0; nProvider < mResourceProviders.size(); nProvider++) 
				{
					//Check if the resource provider can load this extension
					std::string strName, strExt;
					Core::CStringUtils::SplitBaseFilename(inFilePath, strName, strExt);
					
					if(mResourceProviders[nProvider]->CanCreateResourceFromFileWithExtension(strExt))
					{
						if(mResourceProviders[nProvider]->CreateResourceFromFile(ineStorageLocation, inFilePath, pResource)) 
						{
							//Add it to the cache
							CS_DEBUG_LOG("Loading model " + inFilePath);
							mMapFilenameToResource.insert(std::make_pair(inFilePath, pResource));
							pResource->SetName(inFilePath);
							pResource->SetFilename(inFilePath);
							pResource->SetStorageLocation(ineStorageLocation);
							pResource->SetOwningResourceManager(static_cast<Core::IResourceManager*>(this));
							pResource->SetLoaded(true);
							
							return std::static_pointer_cast<CMesh>(pResource);
						}
					}
				}
			} 
			else 
			{
				return std::static_pointer_cast<CMesh>(pExistingResource->second);
			}
			
			//Resource not found
			CS_ERROR_LOG("Cannot find resource for model with path " + inFilePath);
			return GetDefaultMesh();
		}
		//----------------------------------------------------------------
		/// Async Get Model From File
		///
		/// Creates (lazily loads) a model instance from file. If the 
		/// model it already loaded it returns a handle to the model
        ///
        /// @param The storage location to load from
        /// @param the filepath
        /// @return the resource pointer to the mesh
		//----------------------------------------------------------------
		MeshPtr CMeshManager::AsyncGetModelFromFile(Core::StorageLocation ineStorageLocation, const std::string &inFilePath)
		{
			MapStringToResourcePtr::iterator pExistingResource = mMapFilenameToResource.find(inFilePath);
			
			if(pExistingResource == mMapFilenameToResource.end()) 
			{
				Core::ResourcePtr pResource(new CMesh());
				
				for(u32 nProvider = 0; nProvider < mResourceProviders.size(); nProvider++) 
				{
					//Check if the resource provider can load this extension
					std::string strName, strExt;
					Core::CStringUtils::SplitBaseFilename(inFilePath, strName, strExt);
					
					if(mResourceProviders[nProvider]->CanCreateResourceFromFileWithExtension(strExt))
					{
						
						CS_DEBUG_LOG("Loading model " + inFilePath);
						
						pResource->SetName(inFilePath);
						pResource->SetFilename(inFilePath);
						pResource->SetStorageLocation(ineStorageLocation);
						pResource->SetOwningResourceManager(this);
						pResource->SetLoaded(false);
						
						if(mResourceProviders[nProvider]->AsyncCreateResourceFromFile(ineStorageLocation, inFilePath, pResource)) 
						{
							//Add it to the cache
							mMapFilenameToResource.insert(std::make_pair(inFilePath, pResource));
							return std::static_pointer_cast<CMesh>(pResource);
						}
					}
				}
			} 
			else 
			{
				return std::static_pointer_cast<CMesh>(pExistingResource->second);
			}
			
			//Resource not found
			CS_ERROR_LOG("Cannot find resource for model with path " + inFilePath);
			return GetDefaultMesh();
		}
		//----------------------------------------------------------------
		/// Create Manual Mesh
		//----------------------------------------------------------------
		MeshPtr CMeshManager::CreateManualMesh(MeshDescriptor & inMeshDescriptor)
		{
			MeshPtr pMesh(new CMesh());
			
			BuildMesh(mpApplicationOwner->GetRenderSystemPtr(), inMeshDescriptor, pMesh);
			
			return pMesh;
		}
		//----------------------------------------------------------------
		/// Create Empty Model
		//----------------------------------------------------------------
		MeshPtr CMeshManager::CreateEmptyMesh(u32 indwNumSubMeshes, u32 inudwVertexDataSize, u32 indwIndexDataSize)
		{
			CMesh* pMesh = new CMesh();
			
			//create vertex declaration
			s8 dwNumVertexElements = 1;
			VertexElement* apVertElements = new VertexElement[dwNumVertexElements];
			apVertElements[0].eType = VertexDataType::k_float3;
			apVertElements[0].eSemantic = VertexDataSemantic::k_position;
			VertexDeclaration vertDeclaration(dwNumVertexElements, apVertElements);
			
			for (int i = 0; i < indwNumSubMeshes; i++)
			{
				SubMeshPtr newMesh = pMesh->CreateSubMesh("mesh" + Core::ToString(i));
				newMesh->Prepare(mpApplicationOwner->GetRenderSystemPtr(), vertDeclaration, 2, inudwVertexDataSize, indwIndexDataSize);
			}
			
			//cleanup
			delete[] apVertElements;
			
			//return the model pointer
			return MeshPtr(pMesh);
		}
		//----------------------------------------------------------------------------
		/// BuildMesh
		//----------------------------------------------------------------------------
		bool CMeshManager::BuildMesh(RenderSystem* inpRenderSystem, MeshDescriptor& inMeshDescriptor, const MeshPtr& outpResource, bool inbNeedsPrepared)
		{
			bool bSuccess = true;
			
			//set the bounds
			outpResource->SetBounds(inMeshDescriptor.mvMinBounds, inMeshDescriptor.mvMaxBounds);
			
			if (inMeshDescriptor.mFeatures.mbHasAnimationData == true)
			{
				outpResource->mpSkeleton = inMeshDescriptor.mpSkeleton;
			}
			
			//iterate through each mesh
			int count = 0;
			for (std::vector<SubMeshDescriptor>::const_iterator it = inMeshDescriptor.mMeshes.begin(); it != inMeshDescriptor.mMeshes.end(); ++it)
			{
				//caclulate the mesh capacities
				u32 udwVertexDataCapacity = it->mudwNumVertices * inMeshDescriptor.mVertexDeclaration.GetTotalSize(); 
				u32 udwIndexDataCapacity  = it->mudwNumIndices * inMeshDescriptor.mudwIndexSize;
				
				//prepare the mesh if it needs it, otherwise just update the vertex and index declarations.
				SubMeshPtr newSubMesh;
				if (inbNeedsPrepared == true)
				{
					newSubMesh = outpResource->CreateSubMesh(it->mstrName);
					newSubMesh->Prepare(inpRenderSystem, inMeshDescriptor.mVertexDeclaration, inMeshDescriptor.mudwIndexSize, 
										udwVertexDataCapacity, udwIndexDataCapacity, BufferAccess::k_read, it->ePrimitiveType);
				}
				else 
				{
					newSubMesh = outpResource->GetSubMeshAtIndex(count);
					newSubMesh->AlterBufferDeclaration(inMeshDescriptor.mVertexDeclaration, inMeshDescriptor.mudwIndexSize);
				}
				
				//check that the buffers are big enough to hold this data. if not throw an error.
				if (udwVertexDataCapacity <= newSubMesh->GetInternalMeshBuffer()->GetVertexCapacity() &&
					udwIndexDataCapacity <= newSubMesh->GetInternalMeshBuffer()->GetIndexCapacity())
				{
					newSubMesh->Build(it->mpVertexData, it->mpIndexData, it->mudwNumVertices, it->mudwNumIndices, it->mvMinBounds, it->mvMaxBounds);
				}
				else 
				{
					CS_ERROR_LOG("Sub mesh data exceeds its buffer capacity. Mesh will return empty!");
					bSuccess = false;
				}
				
				//set the default material name.
				if (inMeshDescriptor.mFeatures.mbHasMaterial == true)
                {
					newSubMesh->SetDefaultMaterialName(it->mstrMaterialName);
                    newSubMesh->SetDefaultMaterialStorageLocation(it->meMaterialStorageLocation);
                }
				
				//add the skeleton controller
				if (inMeshDescriptor.mFeatures.mbHasAnimationData == true)
				{
					InverseBindPosePtr ibp(new InverseBindPose());
					ibp->mInverseBindPoseMatrices = it->mInverseBindPoseMatrices;
					newSubMesh->SetInverseBindPose(ibp);
				}
				
				count++;
			}
			
			outpResource->CalcVertexAndIndexCounts();
			
			//flag as loaded
			outpResource->SetLoaded(true);
			
			//return success
			return bSuccess;
		}
        //----------------------------------------------------------------------------
        /// Get Default Mesh
        ///
        /// @return A default cube mesh. This will be returned if no mesh can be found
        //----------------------------------------------------------------------------
        MeshPtr CMeshManager::GetDefaultMesh()
        {
            return Core::CApplication::GetDefaultMesh();
        }
		//-----------------------------------------------------------------
		/// Destructor
		///
		//-----------------------------------------------------------------
		CMeshManager::~CMeshManager()
		{
		}
	}
}
