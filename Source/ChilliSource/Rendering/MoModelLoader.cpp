/*
 *  MoStaticModelLoader.cpp
 *  moFlo
 *
 *  Created by Ian Copland on 25/08/2011.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#include <moFlo/Rendering/MoModelLoader.h>
#include <moFlo/Rendering/Mesh.h>
#include <moFlo/Rendering/SubMesh.h>

#include <moFlo/Core/Application.h>

#include <stdexcept>
#include <moFlo/Rendering/MeshManager.h>

namespace moFlo
{
	namespace Rendering
	{
		const std::string kstrMoModelExtension("momodel");
		
		const u32 kgMoModelMinVersion = 9;
		const u32 kgMoModelMaxVersion = 11;
		const u32 kgMoModelFileCheckValue = 6666;
		
		enum MO_MODEL_FEATURE
		{
			MMF_NONE,
			MMF_HAS_TEXTURE,
			MMF_HAS_MATERIAL,
			MMF_HAS_ANIMATION_DATA
		};
		
		enum MO_MODEL_VERTEX_ELEMENT
		{
			MMVE_NONE,
			MMVE_POSITION,
			MMVE_NORMAL,
			MMVE_TEXCOORD,
			MMVE_COLOUR,
			MMVE_WEIGHTS,
			MMVE_JOINT_INDICES
		};
		
		//-------------------------------------------------------------------------
		/// Constructor
		//-------------------------------------------------------------------------
		CMoModelLoader::CMoModelLoader(Core::CApplication* inpApp) : mpApp(inpApp)
		{
		}
		//-------------------------------------------------------------------------
		/// Is A
		//-------------------------------------------------------------------------
		bool CMoModelLoader::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == IResourceProvider::InterfaceID;
		}
		//----------------------------------------------------------------------------
		/// Can Create Resource of Kind
		//----------------------------------------------------------------------------
		bool CMoModelLoader::CanCreateResourceOfKind(Core::InterfaceIDType inInterfaceID) const
		{
			return (inInterfaceID == Rendering::CMesh::InterfaceID);
		}
		//----------------------------------------------------------------------------
		/// Can Create Resource From File With Extension
		//----------------------------------------------------------------------------
		bool CMoModelLoader::CanCreateResourceFromFileWithExtension(const std::string & inExtension) const
		{
			return (inExtension == kstrMoModelExtension);
		}
		//----------------------------------------------------------------------------
		/// Create Resource From File
		//----------------------------------------------------------------------------
		bool CMoModelLoader::CreateResourceFromFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string & inFilePath, Core::ResourcePtr& outpResource)  
		{
			MeshPtr pMesh = SHARED_PTR_CAST<CMesh>(outpResource);
			
			//get the path to the curent file
			std::string filename, filepath;
			moFlo::Core::CStringUtils::SplitFilename(inFilePath, filename, filepath);
			
			return CreateMeshFromFile(ineStorageLocation, inFilePath, filepath, pMesh);
		}
		//----------------------------------------------------------------------------
		/// Create Mesh From File
		//----------------------------------------------------------------------------
		bool CMoModelLoader::CreateMeshFromFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string & inFilePath, const std::string& instrMaterialPath, const MeshPtr& outpResource)  
		{
            MeshDescriptor descriptor;
			
			if (ReadFile(ineStorageLocation, inFilePath, instrMaterialPath, descriptor) == false)
			{
				return false;
			}
			
			//build the mesh
			return BuildMesh(descriptor, outpResource);
		}
		//----------------------------------------------------------------------------
		/// Async Create Resource From File
		//----------------------------------------------------------------------------
		bool CMoModelLoader::AsyncCreateResourceFromFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string & inFilePath, Core::ResourcePtr& outpResource)
		{
			MeshPtr pMesh = SHARED_PTR_CAST<CMesh>(outpResource);
			
			//get the path to the curent file
			std::string filename, filepath;
			moFlo::Core::CStringUtils::SplitFilename(inFilePath, filename, filepath);
			
			return AsyncCreateMeshFromFile(ineStorageLocation, inFilePath, filepath, pMesh);
		}
		//----------------------------------------------------------------------------
		/// Async Create Mesh From File
		//----------------------------------------------------------------------------
		bool CMoModelLoader::AsyncCreateMeshFromFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string &inFilePath, const std::string& instrMaterialPath, const MeshPtr& outpResource)
		{
			//Load model as task
			Task4<Core::STORAGE_LOCATION, const std::string&, const std::string&, const MeshPtr&> MeshTask(this, &CMoModelLoader::LoadMeshDataTask, ineStorageLocation, inFilePath, instrMaterialPath, outpResource);
			CTaskScheduler::ScheduleTask(MeshTask);
			
			return true;
		}
		//----------------------------------------------------------------------------
		/// LoadMeshDataTask
		//----------------------------------------------------------------------------
		void CMoModelLoader::LoadMeshDataTask(Core::STORAGE_LOCATION ineStorageLocation, const std::string &inFilePath, const std::string &instrMaterialPath, const MeshPtr& outpResource)
		{
			//read the mesh data into a MoStaticDeclaration
			MeshDescriptor descriptor;
			if (false == ReadFile(ineStorageLocation, inFilePath, instrMaterialPath, descriptor))
			{
				return;
			}
			
			//start a main thread task for loading the data into a mesh
			CTaskScheduler::ScheduleMainThreadTask(Task2<MeshDescriptor&,const MeshPtr&>(this, &CMoModelLoader::BuildMeshTask, descriptor, outpResource));
		}
		//----------------------------------------------------------------------------
		/// Build Mesh Task
		//----------------------------------------------------------------------------
		void CMoModelLoader::BuildMeshTask(MeshDescriptor& inMeshDescriptor, const MeshPtr& outpResource)
		{
			BuildMesh(inMeshDescriptor, outpResource);				   
		}
		//----------------------------------------------------------------------------
		/// Populate Existing Mesh From File
		//----------------------------------------------------------------------------
		void CMoModelLoader::PopulateExistingMeshFromFile(const MeshPtr& inpMesh, Core::STORAGE_LOCATION ineStorageLocation, const std::string &inFilePath, const std::string& inMaterialPath)
		{
			MeshDescriptor descriptor;
			
			std::string strMatPath = inMaterialPath;
			if (inMaterialPath.length() == 0)
			{
				std::string filename;
				moFlo::Core::CStringUtils::SplitFilename(inFilePath, filename, strMatPath);
			}
			
			if (ReadFile(ineStorageLocation, inFilePath, strMatPath, descriptor) == false)
			{
				return;
			}
			
			BuildMesh(descriptor, inpMesh, false);
		}
		//----------------------------------------------------------------------------
		/// Async Populate Existing Mesh From File
		//----------------------------------------------------------------------------
		void CMoModelLoader::AsyncPopulateExistingMeshFromFile(const MeshPtr& inpMesh, Core::STORAGE_LOCATION ineStorageLocation, const std::string &inFilePath, const std::string& inMaterialPath)
		{
			inpMesh->SetLoaded(false);
			
			std::string strMatPath = inMaterialPath;
			if (inMaterialPath.length() == 0)
			{
				std::string filename;
				moFlo::Core::CStringUtils::SplitFilename(inFilePath, filename, strMatPath);
			}
			
			//Load image as a task. Once it completed we can load the texture as it should be done on the main thread
			Task4<Core::STORAGE_LOCATION, const std::string&, const std::string&, const MeshPtr&> MeshTask(this, &CMoModelLoader::LoadDataForExistingMeshTask, ineStorageLocation, inFilePath, strMatPath, inpMesh);
			CTaskScheduler::ScheduleTask(MeshTask);
		}
		//----------------------------------------------------------------------------
		/// LoadDataForExistingMeshTask
		//----------------------------------------------------------------------------
		void CMoModelLoader::LoadDataForExistingMeshTask(Core::STORAGE_LOCATION ineStorageLocation, const std::string &inFilePath, const std::string& inMaterialPath, const MeshPtr& outpResource)
		{
			//read the mesh data into a MoStaticDeclaration
			MeshDescriptor descriptor;
			if (false == ReadFile(ineStorageLocation, inFilePath, inMaterialPath, descriptor))
			{
				return;
			}
			
			//start a main thread task for loading the data into a mesh
			CTaskScheduler::ScheduleMainThreadTask(Task2<MeshDescriptor&, const MeshPtr&>(this, &CMoModelLoader::BuildExistingMeshTask, descriptor, outpResource));
		}
		//----------------------------------------------------------------------------
		/// Build Existing Mesh Task
		//----------------------------------------------------------------------------
		void CMoModelLoader::BuildExistingMeshTask(MeshDescriptor& inMeshDescriptor, const MeshPtr& outpResource)
		{			
			BuildMesh(inMeshDescriptor, outpResource, false);
		}
		//----------------------------------------------------------------------------
		/// BuildMesh
		//----------------------------------------------------------------------------
		bool CMoModelLoader::BuildMesh(MeshDescriptor& inMeshDescriptor, const MeshPtr& outpResource, bool inbNeedsPrepared)
		{
			bool bSuccess = moFlo::Rendering::CMeshManager::BuildMesh(mpApp->GetRenderSystemPtr(), inMeshDescriptor, outpResource, inbNeedsPrepared);
			
			//cleanup
			for (DYNAMIC_ARRAY<SubMeshDescriptor>::const_iterator it = inMeshDescriptor.mMeshes.begin(); it != inMeshDescriptor.mMeshes.end(); ++it)
			{
				delete[] it->mpVertexData;
				delete[] it->mpIndexData;
			}
			
			//return success
			return bSuccess;
		}
		//----------------------------------------------------------------------------
		/// Read File
		//----------------------------------------------------------------------------
		bool CMoModelLoader::ReadFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string &inFilePath, const std::string &inMaterialPath, MeshDescriptor& inMeshDescriptor)
		{
			bool mbSuccess = true;
			
			moFlo::Core::FileStreamPtr stream = Core::CApplication::GetFileSystemPtr()->CreateFileStream(ineStorageLocation, inFilePath, moFlo::Core::FM_READ_BINARY);
	
			MeshDataQuantities quantities;
			mbSuccess = ReadGlobalHeader(stream, inMeshDescriptor, inFilePath, quantities);
			
			if (true == mbSuccess)
            {
				mbSuccess = ReadSkeletonData(stream, quantities, inMeshDescriptor);
            }
			
			if (true == mbSuccess)
			{	
				for (u32 i = 0; i < quantities.mudwNumMeshes; i++)
				{
					SubMeshDescriptor meshDescriptor;
					
					mbSuccess = ReadMeshHeader(stream, inMeshDescriptor, meshDescriptor, inMaterialPath);
					if (false == mbSuccess)
						break;
					
                    meshDescriptor.meMaterialStorageLocation = ineStorageLocation;
					mbSuccess = ReadMeshData(stream, inMeshDescriptor, meshDescriptor);
					if (false == mbSuccess)
						break;
					
					inMeshDescriptor.mMeshes.push_back(meshDescriptor);
				}
			}
			
			//cleanup
			stream->Close();
			
			//return true
			return mbSuccess;
		}
		//-----------------------------------------------------------------------------
		/// Read Global Header
		//-----------------------------------------------------------------------------
		bool CMoModelLoader::ReadGlobalHeader(const moFlo::Core::FileStreamPtr& inpStream, MeshDescriptor& inMeshDescriptor, const std::string &inFilePath, MeshDataQuantities& outMeshDataQuantities)
		{
			//Check file for corruption
			if(NULL == inpStream || true == inpStream->IsBad())
			{
				ERROR_LOG("Cannot open MoModel file: " + inFilePath);
				return false;
			}
			
			u32 udwFileCheckValue = ReadValue<u32>(inpStream);
			if(udwFileCheckValue != kgMoModelFileCheckValue)
			{
				ERROR_LOG("MoModel file has corruption(incorrect File Check Value): " + inFilePath);
				return false;
			}
			
			u32 udwVersionNum = ReadValue<u32>(inpStream);
			if (udwVersionNum < kgMoModelMinVersion || udwVersionNum > kgMoModelMaxVersion)
			{
				ERROR_LOG("Unsupported MoModel version: " + inFilePath);
				return false;
			}
			
			//init features
			inMeshDescriptor.mFeatures.mbHasTexture = false;
			inMeshDescriptor.mFeatures.mbHasMaterial = false;
			inMeshDescriptor.mFeatures.mbHasAnimationData = false;
			
			//build the feature declaration from the file
			u32 dwNumFeatures = (u32)ReadValue<u8>(inpStream);
			for (int i = 0; i < dwNumFeatures; i++)
			{
				u32 dwFeatureType = (u32)ReadValue<u8>(inpStream);
				
				switch (dwFeatureType)
				{
					case MMF_HAS_TEXTURE:
						inMeshDescriptor.mFeatures.mbHasTexture = true;
						break;
					case MMF_HAS_MATERIAL:
						inMeshDescriptor.mFeatures.mbHasMaterial = true;
						break;
					case MMF_HAS_ANIMATION_DATA:
						inMeshDescriptor.mFeatures.mbHasAnimationData = true;
                        
                        //A breaking change was made to animated models in version 11.
                        if (udwVersionNum < 11)
                        {
                            ERROR_LOG("Model contains old format animation data, please update to momodel version 11: " + inFilePath);
                            return false;
                        }
						break;
					default:
						ERROR_LOG("Unknown feature type in MoModel(" + inFilePath + ")feature declaration!");
						break;
				}
			}
			
			//read the vertex declaration
			ReadVertexDeclaration(inpStream, inMeshDescriptor);
			
			//read index declaration
			inMeshDescriptor.mudwIndexSize = ReadValue<u8>(inpStream);
			
			//read the min and max bounds
			inMeshDescriptor.mvMinBounds.x = ReadValue<f32>(inpStream);
			inMeshDescriptor.mvMinBounds.y = ReadValue<f32>(inpStream);
			inMeshDescriptor.mvMinBounds.z = ReadValue<f32>(inpStream);
			inMeshDescriptor.mvMaxBounds.x = ReadValue<f32>(inpStream);
			inMeshDescriptor.mvMaxBounds.y = ReadValue<f32>(inpStream);
			inMeshDescriptor.mvMaxBounds.z = ReadValue<f32>(inpStream);
			
			//read the number of meshes
			outMeshDataQuantities.mudwNumMeshes = (u32)ReadValue<u16>(inpStream);
			
			//read num skeleton nodes and joints if used
			outMeshDataQuantities.mdwNumSkeletonNodes = 0;
            outMeshDataQuantities.mudwNumJoints = 0;
			if (true == inMeshDescriptor.mFeatures.mbHasAnimationData)
			{
                outMeshDataQuantities.mdwNumSkeletonNodes = (s32)ReadValue<s16>(inpStream);
                outMeshDataQuantities.mudwNumJoints = (u32)ReadValue<u8>(inpStream);
			}
			
			return true;
		}
		//-----------------------------------------------------------------------------
		/// Read Skeleton
		//-----------------------------------------------------------------------------
		bool CMoModelLoader::ReadSkeletonData(const moFlo::Core::FileStreamPtr& inpStream, const MeshDataQuantities& inQuantities, MeshDescriptor& inMeshDescriptor)
		{
			if (true == inMeshDescriptor.mFeatures.mbHasAnimationData)
			{
                inMeshDescriptor.mpSkeleton = SkeletonPtr(new CSkeleton());
                
				//read the skeleton nodes
                HASH_MAP<u32, s32> jointToNodeMap;
				for (u32 i = 0; i < inQuantities.mdwNumSkeletonNodes; i++)
				{
                    //get the skeleton node name name
                    std::string strName;
					bool bKeepGoing = true;
					while (true == bKeepGoing)
					{
						u8 bySingleChar = ReadValue<u8>(inpStream);
						if (bySingleChar == 0)
							bKeepGoing = false;
						else 
							strName += bySingleChar;
					}
					
					//get the parent index
					s32 dwParent = (s32)ReadValue<s16>(inpStream);
                    
                    //get the type
                    u8 byType = ReadValue<u8>(inpStream);
                    if (byType == 1)
                    {
                        u32 udwJointIndex = (u32)ReadValue<u8>(inpStream);
                        jointToNodeMap.insert(std::pair<u32, s32>(udwJointIndex, (s32)i));
                    }
					
					//add joint
					inMeshDescriptor.mpSkeleton->AddNode(strName, dwParent);
				}
                
                //build the list of joints
                for (int i = 0; i < inQuantities.mudwNumJoints; ++i)
                {
                    inMeshDescriptor.mpSkeleton->AddJointIndex(jointToNodeMap[i]);
                }
			}
			
			return true;
		}
		//-----------------------------------------------------------------------------
		/// Read Mesh Header
		//-----------------------------------------------------------------------------
		bool CMoModelLoader::ReadMeshHeader(const moFlo::Core::FileStreamPtr& inpStream, MeshDescriptor& inMeshDescriptor, SubMeshDescriptor& inSubMeshDescriptor, 
											const std::string& instrMaterialPath)
		{
			//read mesh name
			std::string strMeshName;
			bool bKeepGoing = true;
			while (true == bKeepGoing)
			{
				u8 bySingleChar = ReadValue<u8>(inpStream);
				if (bySingleChar == 0)
					bKeepGoing = false;
				else 
					strMeshName += bySingleChar;
			}
			inSubMeshDescriptor.mstrName = strMeshName;
			
			//read num verts and triangles
			if (2 == inMeshDescriptor.mudwIndexSize)
			{
				inSubMeshDescriptor.mudwNumVertices = (u32)ReadValue<u16>(inpStream);
				inSubMeshDescriptor.mudwNumIndices = ((u32)ReadValue<u16>(inpStream)) * (u32)3;
			}
			else
			{
				inSubMeshDescriptor.mudwNumVertices = ReadValue<u32>(inpStream);
				inSubMeshDescriptor.mudwNumIndices = ReadValue<u32>(inpStream) * (u32)3;
			}

			//read bounds
			inSubMeshDescriptor.mvMinBounds.x = ReadValue<f32>(inpStream);
			inSubMeshDescriptor.mvMinBounds.y = ReadValue<f32>(inpStream);
			inSubMeshDescriptor.mvMinBounds.z = ReadValue<f32>(inpStream);
			inSubMeshDescriptor.mvMaxBounds.x = ReadValue<f32>(inpStream);
			inSubMeshDescriptor.mvMaxBounds.y = ReadValue<f32>(inpStream);
			inSubMeshDescriptor.mvMaxBounds.z = ReadValue<f32>(inpStream);
			
			//if this has a texture, read it
			if (true == inMeshDescriptor.mFeatures.mbHasTexture)
			{
				std::string strTextureFilename;
				bKeepGoing = true;
				while (true == bKeepGoing)
				{
					u8 bySingleChar = ReadValue<u8>(inpStream);
					if (0 == bySingleChar)
						bKeepGoing = false;
					else 
						strTextureFilename += bySingleChar;
				}
				inSubMeshDescriptor.mstrTextureName = strTextureFilename;
			}
			
			//if this has a material, read it
			if (true == inMeshDescriptor.mFeatures.mbHasMaterial)
			{
				std::string strMaterialFilename;
				bKeepGoing = true;
				while (true == bKeepGoing)
				{
					u8 bySingleChar = ReadValue<u8>(inpStream);
					if (0 == bySingleChar)
						bKeepGoing = false;
					else 
						strMaterialFilename += bySingleChar;
				}
				inSubMeshDescriptor.mstrMaterialName = moFlo::Core::CStringUtils::StandardisePath(instrMaterialPath) + strMaterialFilename;
			}
			
			return true;
		}
		//-----------------------------------------------------------------------------
		/// Read Mesh Data
		//-----------------------------------------------------------------------------
		bool CMoModelLoader::ReadMeshData(const moFlo::Core::FileStreamPtr& inpStream, MeshDescriptor& inMeshDescriptor, SubMeshDescriptor& inSubMeshDescriptor)
		{
			//read the inverse bind matrices
            if (true == inMeshDescriptor.mFeatures.mbHasAnimationData)
			{
                for (u32 i = 0; i < inMeshDescriptor.mpSkeleton->GetNumJoints(); i++)
                {
                    moFlo::Core::CMatrix4x4 IBPMat;
                    for (u32 j = 0; j < 16; j++)
                    {
                        IBPMat.m[j] = ReadValue<f32>(inpStream);
                    }
                    inSubMeshDescriptor.mInverseBindPoseMatrices.push_back(IBPMat);
                }
            }
			
			//read the vertex data
			inSubMeshDescriptor.mpVertexData = new u8[inMeshDescriptor.mVertexDeclaration.GetTotalSize() * inSubMeshDescriptor.mudwNumVertices];
			inpStream->Read((s8*)inSubMeshDescriptor.mpVertexData, inMeshDescriptor.mVertexDeclaration.GetTotalSize() * inSubMeshDescriptor.mudwNumVertices);
			
			//read the index data
			inSubMeshDescriptor.mpIndexData = new u8[inMeshDescriptor.mudwIndexSize * inSubMeshDescriptor.mudwNumIndices];
			inpStream->Read((s8*)inSubMeshDescriptor.mpIndexData, inMeshDescriptor.mudwIndexSize * inSubMeshDescriptor.mudwNumIndices);
			
			return true;
		}
		//-----------------------------------------------------------------------------
		/// Read Vertex Declaration
		//-----------------------------------------------------------------------------
		void CMoModelLoader::ReadVertexDeclaration(const moFlo::Core::FileStreamPtr& inpStream, MeshDescriptor& inMeshDescriptor)
		{
			//build the vertex declaration from the file
			u8 dwNumVertexElements = ReadValue<u8>(inpStream);
			VertexElement* apVertElements = new VertexElement[dwNumVertexElements];
			for (int i = 0; i < dwNumVertexElements; i++)
			{
				u8 dwVertexElement = ReadValue<u8>(inpStream);
				
				switch (dwVertexElement)
				{
					case MMVE_NONE:
						ERROR_LOG("Unknown vertex type in vertex declaration!");
						break;
					case MMVE_POSITION:
						apVertElements[i].eType = FLOAT4;
						apVertElements[i].eSemantic = POSITION;
						break;
					case MMVE_NORMAL:
						apVertElements[i].eType = FLOAT3;
						apVertElements[i].eSemantic = NORMAL;
						break;
					case MMVE_TEXCOORD:
						apVertElements[i].eType = FLOAT2;
						apVertElements[i].eSemantic = UV;
						break;
					case MMVE_COLOUR:
						apVertElements[i].eType = BYTE4;
						apVertElements[i].eSemantic = COLOUR;
						break;
					case MMVE_WEIGHTS:
						apVertElements[i].eType = FLOAT4;
						apVertElements[i].eSemantic = WEIGHTS;
						break;
					case MMVE_JOINT_INDICES:
						apVertElements[i].eType = BYTE4;
						apVertElements[i].eSemantic = JOINT_INDICES;
						break;
				}
			}
			inMeshDescriptor.mVertexDeclaration = CVertexDeclaration(dwNumVertexElements, apVertElements);
			delete[] apVertElements;
		}
		//---------------------------------------------------
		/// Destructor
		//---------------------------------------------------
		CMoModelLoader::~CMoModelLoader()
		{
			
		}
	}
}

