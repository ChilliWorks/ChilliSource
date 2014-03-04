/*
 *  MoStaticModelLoader.cpp
 *  moFlo
 *
 *  Created by Ian Copland on 25/08/2011.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Rendering/Model/MoModelLoader.h>
#include <ChilliSource/Rendering/Model/Mesh.h>
#include <ChilliSource/Rendering/Model/SubMesh.h>

#include <ChilliSource/Core/Base/Application.h>

#include <stdexcept>
#include <ChilliSource/Rendering/Model/MeshManager.h>

namespace ChilliSource
{
	namespace Rendering
	{
		const std::string kstrMoModelExtension("momodel");
		
		const u32 kgMoModelMinVersion = 9;
		const u32 kgMoModelMaxVersion = 11;
		const u32 kgMoModelFileCheckValue = 6666;
		
		enum class MoModelFeature
		{
            k_none,
            k_hasTexture,
            k_hasMaterial,
            k_hasAnimation
		};
		
		enum class MoModelVertexElement
		{
            k_none,
            k_position,
            k_normal,
            k_texcoord,
            k_colour,
            k_weights,
            k_jointIndices
		};
		
		//-------------------------------------------------------------------------
		/// Constructor
		//-------------------------------------------------------------------------
		MoModelLoader::MoModelLoader(Core::Application* inpApp) : mpApp(inpApp)
		{
		}
		//-------------------------------------------------------------------------
		/// Is A
		//-------------------------------------------------------------------------
		bool MoModelLoader::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == ResourceProvider::InterfaceID;
		}
		//----------------------------------------------------------------------------
		/// Can Create Resource of Kind
		//----------------------------------------------------------------------------
		bool MoModelLoader::CanCreateResourceOfKind(Core::InterfaceIDType inInterfaceID) const
		{
			return (inInterfaceID == Mesh::InterfaceID);
		}
		//----------------------------------------------------------------------------
		/// Can Create Resource From File With Extension
		//----------------------------------------------------------------------------
		bool MoModelLoader::CanCreateResourceFromFileWithExtension(const std::string & inExtension) const
		{
			return (inExtension == kstrMoModelExtension);
		}
		//----------------------------------------------------------------------------
		/// Create Resource From File
		//----------------------------------------------------------------------------
		bool MoModelLoader::CreateResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string & inFilePath, Core::ResourceSPtr& outpResource)  
		{
			MeshSPtr pMesh = std::static_pointer_cast<Mesh>(outpResource);
			
			//get the path to the curent file
			std::string filename, filepath;
			ChilliSource::Core::StringUtils::SplitFilename(inFilePath, filename, filepath);
			
			return CreateMeshFromFile(ineStorageLocation, inFilePath, filepath, pMesh);
		}
		//----------------------------------------------------------------------------
		/// Create Mesh From File
		//----------------------------------------------------------------------------
		bool MoModelLoader::CreateMeshFromFile(Core::StorageLocation ineStorageLocation, const std::string & inFilePath, const std::string& instrMaterialPath, const MeshSPtr& outpResource)  
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
		bool MoModelLoader::AsyncCreateResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string & inFilePath, Core::ResourceSPtr& outpResource)
		{
			MeshSPtr pMesh = std::static_pointer_cast<Mesh>(outpResource);
			
			//get the path to the curent file
			std::string filename, filepath;
			ChilliSource::Core::StringUtils::SplitFilename(inFilePath, filename, filepath);
			
			return AsyncCreateMeshFromFile(ineStorageLocation, inFilePath, filepath, pMesh);
		}
		//----------------------------------------------------------------------------
		/// Async Create Mesh From File
		//----------------------------------------------------------------------------
		bool MoModelLoader::AsyncCreateMeshFromFile(Core::StorageLocation ineStorageLocation, const std::string &inFilePath, const std::string& instrMaterialPath, const MeshSPtr& outpResource)
		{
			//Load model as task
			Core::Task<Core::StorageLocation, const std::string&, const std::string&, const MeshSPtr&> MeshTask(this, &MoModelLoader::LoadMeshDataTask, ineStorageLocation, inFilePath, instrMaterialPath, outpResource);
			Core::TaskScheduler::ScheduleTask(MeshTask);
			
			return true;
		}
		//----------------------------------------------------------------------------
		/// LoadMeshDataTask
		//----------------------------------------------------------------------------
		void MoModelLoader::LoadMeshDataTask(Core::StorageLocation ineStorageLocation, const std::string &inFilePath, const std::string &instrMaterialPath, const MeshSPtr& outpResource)
		{
			//read the mesh data into a MoStaticDeclaration
			MeshDescriptor descriptor;
			if (false == ReadFile(ineStorageLocation, inFilePath, instrMaterialPath, descriptor))
			{
				return;
			}
			
			//start a main thread task for loading the data into a mesh
			Core::TaskScheduler::ScheduleMainThreadTask(Core::Task<MeshDescriptor&,const MeshSPtr&>(this, &MoModelLoader::BuildMeshTask, descriptor, outpResource));
		}
		//----------------------------------------------------------------------------
		/// Build Mesh Task
		//----------------------------------------------------------------------------
		void MoModelLoader::BuildMeshTask(MeshDescriptor& inMeshDescriptor, const MeshSPtr& outpResource)
		{
			BuildMesh(inMeshDescriptor, outpResource);				   
		}
		//----------------------------------------------------------------------------
		/// Populate Existing Mesh From File
		//----------------------------------------------------------------------------
		void MoModelLoader::PopulateExistingMeshFromFile(const MeshSPtr& inpMesh, Core::StorageLocation ineStorageLocation, const std::string &inFilePath, const std::string& inMaterialPath)
		{
			MeshDescriptor descriptor;
			
			std::string strMatPath = inMaterialPath;
			if (inMaterialPath.length() == 0)
			{
				std::string filename;
				ChilliSource::Core::StringUtils::SplitFilename(inFilePath, filename, strMatPath);
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
		void MoModelLoader::AsyncPopulateExistingMeshFromFile(const MeshSPtr& inpMesh, Core::StorageLocation ineStorageLocation, const std::string &inFilePath, const std::string& inMaterialPath)
		{
			inpMesh->SetLoaded(false);
			
			std::string strMatPath = inMaterialPath;
			if (inMaterialPath.length() == 0)
			{
				std::string filename;
				ChilliSource::Core::StringUtils::SplitFilename(inFilePath, filename, strMatPath);
			}
			
			//Load image as a task. Once it completed we can load the texture as it should be done on the main thread
			Core::Task<Core::StorageLocation, const std::string&, const std::string&, const MeshSPtr&> MeshTask(this, &MoModelLoader::LoadDataForExistingMeshTask, ineStorageLocation, inFilePath, strMatPath, inpMesh);
			Core::TaskScheduler::ScheduleTask(MeshTask);
		}
		//----------------------------------------------------------------------------
		/// LoadDataForExistingMeshTask
		//----------------------------------------------------------------------------
		void MoModelLoader::LoadDataForExistingMeshTask(Core::StorageLocation ineStorageLocation, const std::string &inFilePath, const std::string& inMaterialPath, const MeshSPtr& outpResource)
		{
			//read the mesh data into a MoStaticDeclaration
			MeshDescriptor descriptor;
			if (false == ReadFile(ineStorageLocation, inFilePath, inMaterialPath, descriptor))
			{
				return;
			}
			
			//start a main thread task for loading the data into a mesh
			Core::TaskScheduler::ScheduleMainThreadTask(Core::Task<MeshDescriptor&, const MeshSPtr&>(this, &MoModelLoader::BuildExistingMeshTask, descriptor, outpResource));
		}
		//----------------------------------------------------------------------------
		/// Build Existing Mesh Task
		//----------------------------------------------------------------------------
		void MoModelLoader::BuildExistingMeshTask(MeshDescriptor& inMeshDescriptor, const MeshSPtr& outpResource)
		{			
			BuildMesh(inMeshDescriptor, outpResource, false);
		}
		//----------------------------------------------------------------------------
		/// BuildMesh
		//----------------------------------------------------------------------------
		bool MoModelLoader::BuildMesh(MeshDescriptor& inMeshDescriptor, const MeshSPtr& outpResource, bool inbNeedsPrepared)
		{
			bool bSuccess = MeshManager::BuildMesh(mpApp->GetRenderSystemPtr(), inMeshDescriptor, outpResource, inbNeedsPrepared);
			
			//cleanup
			for (std::vector<SubMeshDescriptor>::const_iterator it = inMeshDescriptor.mMeshes.begin(); it != inMeshDescriptor.mMeshes.end(); ++it)
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
		bool MoModelLoader::ReadFile(Core::StorageLocation ineStorageLocation, const std::string &inFilePath, const std::string &inMaterialPath, MeshDescriptor& inMeshDescriptor)
		{
			bool mbSuccess = true;
			
			Core::FileStreamSPtr stream = Core::Application::Get()->GetFileSystemPtr()->CreateFileStream(ineStorageLocation, inFilePath, Core::FileMode::k_readBinary);
	
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
		bool MoModelLoader::ReadGlobalHeader(const ChilliSource::Core::FileStreamSPtr& inpStream, MeshDescriptor& inMeshDescriptor, const std::string &inFilePath, MeshDataQuantities& outMeshDataQuantities)
		{
			//Check file for corruption
			if(nullptr == inpStream || true == inpStream->IsBad())
			{
				CS_LOG_ERROR("Cannot open MoModel file: " + inFilePath);
				return false;
			}
			
			u32 udwFileCheckValue = ReadValue<u32>(inpStream);
			if(udwFileCheckValue != kgMoModelFileCheckValue)
			{
				CS_LOG_ERROR("MoModel file has corruption(incorrect File Check Value): " + inFilePath);
				return false;
			}
			
			u32 udwVersionNum = ReadValue<u32>(inpStream);
			if (udwVersionNum < kgMoModelMinVersion || udwVersionNum > kgMoModelMaxVersion)
			{
				CS_LOG_ERROR("Unsupported MoModel version: " + inFilePath);
				return false;
			}
			
			//init features
			inMeshDescriptor.mFeatures.mbHasTexture = false;
			inMeshDescriptor.mFeatures.mbHasMaterial = false;
			inMeshDescriptor.mFeatures.mbHasAnimationData = false;
			
			//build the feature declaration from the file
			u32 dwNumFeatures = (u32)ReadValue<u8>(inpStream);
			for (u32 i = 0; i < dwNumFeatures; i++)
			{
				u32 dwFeatureType = (u32)ReadValue<u8>(inpStream);
				
				switch (MoModelFeature(dwFeatureType))
				{
					case MoModelFeature::k_hasTexture:
						inMeshDescriptor.mFeatures.mbHasTexture = true;
						break;
					case MoModelFeature::k_hasMaterial:
						inMeshDescriptor.mFeatures.mbHasMaterial = true;
						break;
					case MoModelFeature::k_hasAnimation:
						inMeshDescriptor.mFeatures.mbHasAnimationData = true;
                        
                        //A breaking change was made to animated models in version 11.
                        if (udwVersionNum < 11)
                        {
                            CS_LOG_ERROR("Model contains old format animation data, please update to momodel version 11: " + inFilePath);
                            return false;
                        }
						break;
					default:
						CS_LOG_ERROR("Unknown feature type in MoModel(" + inFilePath + ")feature declaration!");
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
		bool MoModelLoader::ReadSkeletonData(const ChilliSource::Core::FileStreamSPtr& inpStream, const MeshDataQuantities& inQuantities, MeshDescriptor& inMeshDescriptor)
		{
			if (true == inMeshDescriptor.mFeatures.mbHasAnimationData)
			{
                inMeshDescriptor.mpSkeleton = SkeletonSPtr(new Skeleton());
                
				//read the skeleton nodes
                std::unordered_map<u32, s32> jointToNodeMap;
				for (s32 i = 0; i < inQuantities.mdwNumSkeletonNodes; i++)
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
                for (u32 i = 0; i < inQuantities.mudwNumJoints; ++i)
                {
                    inMeshDescriptor.mpSkeleton->AddJointIndex(jointToNodeMap[i]);
                }
			}
			
			return true;
		}
		//-----------------------------------------------------------------------------
		/// Read Mesh Header
		//-----------------------------------------------------------------------------
		bool MoModelLoader::ReadMeshHeader(const ChilliSource::Core::FileStreamSPtr& inpStream, MeshDescriptor& inMeshDescriptor, SubMeshDescriptor& inSubMeshDescriptor, 
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
				inSubMeshDescriptor.mstrMaterialName = ChilliSource::Core::StringUtils::StandardisePath(instrMaterialPath) + strMaterialFilename;
			}
			
			return true;
		}
		//-----------------------------------------------------------------------------
		/// Read Mesh Data
		//-----------------------------------------------------------------------------
		bool MoModelLoader::ReadMeshData(const ChilliSource::Core::FileStreamSPtr& inpStream, MeshDescriptor& inMeshDescriptor, SubMeshDescriptor& inSubMeshDescriptor)
		{
			//read the inverse bind matrices
            if (true == inMeshDescriptor.mFeatures.mbHasAnimationData)
			{
                for (u32 i = 0; i < inMeshDescriptor.mpSkeleton->GetNumJoints(); i++)
                {
                    ChilliSource::Core::Matrix4x4 IBPMat;
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
		void MoModelLoader::ReadVertexDeclaration(const ChilliSource::Core::FileStreamSPtr& inpStream, MeshDescriptor& inMeshDescriptor)
		{
			//build the vertex declaration from the file
			u8 dwNumVertexElements = ReadValue<u8>(inpStream);
			VertexElement* apVertElements = new VertexElement[dwNumVertexElements];
			for (int i = 0; i < dwNumVertexElements; i++)
			{
				u8 dwVertexElement = ReadValue<u8>(inpStream);
				
				switch (MoModelVertexElement(dwVertexElement))
				{
					case MoModelVertexElement::k_none:
						CS_LOG_ERROR("Unknown vertex type in vertex declaration!");
						break;
					case MoModelVertexElement::k_position:
						apVertElements[i].eType = VertexDataType::k_float4;
						apVertElements[i].eSemantic = VertexDataSemantic::k_position;
						break;
					case MoModelVertexElement::k_normal:
						apVertElements[i].eType = VertexDataType::k_float3;
						apVertElements[i].eSemantic = VertexDataSemantic::k_normal;
						break;
					case MoModelVertexElement::k_texcoord:
						apVertElements[i].eType = VertexDataType::k_float2;
						apVertElements[i].eSemantic = VertexDataSemantic::k_uv;
						break;
					case MoModelVertexElement::k_colour:
						apVertElements[i].eType = VertexDataType::k_byte4;
						apVertElements[i].eSemantic = VertexDataSemantic::k_colour;
						break;
					case MoModelVertexElement::k_weights:
						apVertElements[i].eType = VertexDataType::k_float4;
						apVertElements[i].eSemantic = VertexDataSemantic::k_weight;
						break;
					case MoModelVertexElement::k_jointIndices:
						apVertElements[i].eType = VertexDataType::k_byte4;
						apVertElements[i].eSemantic = VertexDataSemantic::k_jointIndex;
						break;
				}
			}
			inMeshDescriptor.mVertexDeclaration = VertexDeclaration(dwNumVertexElements, apVertElements);
			delete[] apVertElements;
		}
		//---------------------------------------------------
		/// Destructor
		//---------------------------------------------------
		MoModelLoader::~MoModelLoader()
		{
			
		}
	}
}

