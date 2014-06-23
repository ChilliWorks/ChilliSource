//
//  CSModelProvider.cpp
//  Chilli Source
//  Created by Ian Copland on 25/08/2011.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2011 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#include <ChilliSource/Rendering/Model/CSModelProvider.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>
#include <ChilliSource/Rendering/Model/Mesh.h>
#include <ChilliSource/Rendering/Model/MeshDescriptor.h>

#include <unordered_map>

namespace ChilliSource
{
	namespace Rendering
	{
        namespace
        {
            const std::string k_modelFileExtension("csmodel");
            
            const u32 k_minVersion = 12;
            const u32 k_maxVersion = 12;
            const u32 k_fileCheckValue = 6666;
            
            //---------------------------------------------
            /// Features implemented by the model resource.
            /// Models can opt in and out of features.
            ///
            /// @author Ian Copland.
            //---------------------------------------------
            enum class Feature
            {
                k_none,
                k_hasAnimation,
            };
            //---------------------------------------------
            /// Model resources can have flexible attribute
            /// formats described by mixing and matching the
            /// following.
            ///
            /// @author Ian Copland.
            //---------------------------------------------
            enum class VertexAttribute
            {
                k_none,
                k_position,
                k_normal,
                k_texcoord,
                k_colour,
                k_weights,
                k_jointIndices
            };
            //----------------------------------------------------------------------------
            /// Mesh Data Quantities
            ///
            /// A struct containing the quantities of different pieces of mesh data.
            //----------------------------------------------------------------------------
			struct MeshDataQuantities
            {
                u32 m_numMeshes;
                s32 m_numSkeletonNodes;
                u32 m_numJoints;
            };
            //----------------------------------------------------------------------------
			/// Read value in for given type
            ///
            /// @author Ian Copland
            ///
			/// @param File stream
            ///
			/// @return Value of type T
			//----------------------------------------------------------------------------
			template <typename TType> TType ReadValue(const Core::FileStreamSPtr& in_meshStream)
			{
				TType value;
				in_meshStream->Read(reinterpret_cast<s8*>(&value), sizeof(TType));
				return value;
			}
            //----------------------------------------------------------------------------
			/// Read block of data in for given type
            ///
            /// @author Ian Copland
            ///
			/// @param File stream
            /// @param Num to read
			/// @param [Out] data
			//----------------------------------------------------------------------------
			template <typename TType> void ReadBlock(const Core::FileStreamSPtr& in_meshStream, u32 in_numToRead, TType* out_data)
			{
				in_meshStream->Read(reinterpret_cast<s8*>(out_data), sizeof(TType) * in_numToRead);
			}
            //-----------------------------------------------------------------------------
            /// Read the vertex declaration from the mesh filestream. The declaration
            /// is variable
            ///
            /// @author Ian Copland
            ///
            /// @param Mesh stream
            /// @param [Out] Mesh description
            //-----------------------------------------------------------------------------
            void ReadVertexDeclaration(const Core::FileStreamSPtr& in_meshStream, MeshDescriptor& out_meshDesc)
            {
                //build the vertex declaration from the file
                u8 numVertexElements = ReadValue<u8>(in_meshStream);
                
                VertexElement* vertexElements = new VertexElement[numVertexElements];
                for (int i = 0; i < numVertexElements; ++i)
                {
                    u8 vertexAttrib = ReadValue<u8>(in_meshStream);
                    
                    switch (VertexAttribute(vertexAttrib))
                    {
                        case VertexAttribute::k_none:
                            CS_LOG_ERROR("Unknown vertex type in vertex declaration!");
                            break;
                        case VertexAttribute::k_position:
                            vertexElements[i].eType = VertexDataType::k_float4;
                            vertexElements[i].eSemantic = VertexDataSemantic::k_position;
                            break;
                        case VertexAttribute::k_normal:
                            vertexElements[i].eType = VertexDataType::k_float3;
                            vertexElements[i].eSemantic = VertexDataSemantic::k_normal;
                            break;
                        case VertexAttribute::k_texcoord:
                            vertexElements[i].eType = VertexDataType::k_float2;
                            vertexElements[i].eSemantic = VertexDataSemantic::k_uv;
                            break;
                        case VertexAttribute::k_colour:
                            vertexElements[i].eType = VertexDataType::k_byte4;
                            vertexElements[i].eSemantic = VertexDataSemantic::k_colour;
                            break;
                        case VertexAttribute::k_weights:
                            vertexElements[i].eType = VertexDataType::k_float4;
                            vertexElements[i].eSemantic = VertexDataSemantic::k_weight;
                            break;
                        case VertexAttribute::k_jointIndices:
                            vertexElements[i].eType = VertexDataType::k_byte4;
                            vertexElements[i].eSemantic = VertexDataSemantic::k_jointIndex;
                            break;
                    }
                }
                
                out_meshDesc.mVertexDeclaration = VertexDeclaration(numVertexElements, vertexElements);
                delete[] vertexElements;
            }
            //-----------------------------------------------------------------------------
			/// Read the mesh section of the file
            ///
            /// @author Ian Copland
			///
			/// @param File stream
			/// @param Mesh description
            /// @param [Out] Submesh description
            //-----------------------------------------------------------------------------
            void ReadSubMeshData(const Core::FileStreamSPtr& in_meshStream, const MeshDescriptor& in_meshDesc, SubMeshDescriptor& out_subMeshDesc)
            {
                //read the inverse bind matrices
                if(true == in_meshDesc.mFeatures.mbHasAnimationData)
                {
                    for(u32 i=0; i<in_meshDesc.m_skeletonDesc.m_jointIndices.size(); ++i)
                    {
                        CSCore::Matrix4 IBPMat;
                        ReadBlock<f32>(in_meshStream, 16, IBPMat.m);
                        out_subMeshDesc.mInverseBindPoseMatrices.push_back(IBPMat);
                    }
                }
                
                //read the vertex data
                out_subMeshDesc.mpVertexData = new u8[in_meshDesc.mVertexDeclaration.GetTotalSize() * out_subMeshDesc.mudwNumVertices];
                in_meshStream->Read((s8*)out_subMeshDesc.mpVertexData, in_meshDesc.mVertexDeclaration.GetTotalSize() * out_subMeshDesc.mudwNumVertices);
                
                //read the index data
                out_subMeshDesc.mpIndexData = new u8[in_meshDesc.mudwIndexSize * out_subMeshDesc.mudwNumIndices];
                in_meshStream->Read((s8*)out_subMeshDesc.mpIndexData, in_meshDesc.mudwIndexSize * out_subMeshDesc.mudwNumIndices);
            }
            //-----------------------------------------------------------------------------
			/// Reads the sub-mesh header section of the file
            ///
            /// @author Ian Copland
			///
			/// @param File stream
            /// @param Mesh description
			/// @param [Out] Sube mesh description
            //-----------------------------------------------------------------------------
            void ReadSubMeshHeader(const Core::FileStreamSPtr& in_meshStream, const MeshDescriptor& in_meshDesc, SubMeshDescriptor& out_subMeshDesc)
            {
                //read mesh name
                u8 nextChar = 0;
                do
                {
                    nextChar = ReadValue<u8>(in_meshStream);
                    out_subMeshDesc.mstrName += nextChar;
                    
                } while(nextChar != 0);
                
                //read num verts and triangles
                if (2 == in_meshDesc.mudwIndexSize)
                {
                    out_subMeshDesc.mudwNumVertices = (u32)ReadValue<u16>(in_meshStream);
                    out_subMeshDesc.mudwNumIndices = ((u32)ReadValue<u16>(in_meshStream)) * 3;
                }
                else
                {
                    out_subMeshDesc.mudwNumVertices = ReadValue<u32>(in_meshStream);
                    out_subMeshDesc.mudwNumIndices = ReadValue<u32>(in_meshStream) * 3;
                }
                
                //read bounds
                out_subMeshDesc.mvMinBounds.x = ReadValue<f32>(in_meshStream);
                out_subMeshDesc.mvMinBounds.y = ReadValue<f32>(in_meshStream);
                out_subMeshDesc.mvMinBounds.z = ReadValue<f32>(in_meshStream);
                out_subMeshDesc.mvMaxBounds.x = ReadValue<f32>(in_meshStream);
                out_subMeshDesc.mvMaxBounds.y = ReadValue<f32>(in_meshStream);
                out_subMeshDesc.mvMaxBounds.z = ReadValue<f32>(in_meshStream);
                
                //TODO: Remove texture and material from mesh
                //in the meantime just read and discard.
                if (true == in_meshDesc.mFeatures.mbHasTexture)
                {
                    u8 nextCharacter = 0;
                    do
                    {
                        nextCharacter = ReadValue<u8>(in_meshStream);
                        
                    } while(nextCharacter != 0);
                }
                
                if (true == in_meshDesc.mFeatures.mbHasMaterial)
                {
                    u8 nextCharacter = 0;
                    do
                    {
                        nextCharacter = ReadValue<u8>(in_meshStream);
                        
                    } while(nextCharacter != 0);
                }
            }
            //-----------------------------------------------------------------------------
			/// Reads the skeleton section of the file
            ///
            /// @author Ian Copland
			///
			/// @param File stream
            /// @param Container holding the num of meshes, joints and bones
			/// @param [Out] Skeleton description
            //-----------------------------------------------------------------------------
            void ReadSkeletonData(const Core::FileStreamSPtr& in_meshStream, const MeshDataQuantities& in_quantities, SkeletonDescriptor& out_skeletonDesc)
            {
                //read the skeleton nodes
                out_skeletonDesc.m_nodeNames.reserve(in_quantities.m_numSkeletonNodes);
                out_skeletonDesc.m_parentNodeIndices.reserve(in_quantities.m_numSkeletonNodes);
                
                std::unordered_map<u32, s32> jointToNodeMap;
                for (u32 i = 0; i<(u32)in_quantities.m_numSkeletonNodes; ++i)
                {
                    //get the skeleton node name name
                    std::string nodeName;
                    u8 nextChar = 0;
                    do
                    {
                        nextChar = ReadValue<u8>(in_meshStream);
                        nodeName += nextChar;
                    } while(nextChar != 0);
                    
                    //get the parent index
                    s32 parentIndex = (s32)ReadValue<s16>(in_meshStream);
                    
                    //get the type
                    const u32 k_isJoint = 1;
                    u8 type = ReadValue<u8>(in_meshStream);
                    if (type == k_isJoint)
                    {
                        u32 jointIndex = (u32)ReadValue<u8>(in_meshStream);
                        jointToNodeMap.insert(std::pair<u32, s32>(jointIndex, (s32)i));
                    }
                    
                    out_skeletonDesc.m_nodeNames.push_back(nodeName);
                    out_skeletonDesc.m_parentNodeIndices.push_back(parentIndex);
                }
                
                CS_ASSERT(out_skeletonDesc.m_nodeNames.size() == out_skeletonDesc.m_parentNodeIndices.size(), "Invalid number of node names and indices in skeleton");
                
                for (u32 i=0; i<in_quantities.m_numJoints; ++i)
                {
                    out_skeletonDesc.m_jointIndices.push_back(jointToNodeMap[i]);
                }
            }
            //-----------------------------------------------------------------------------
			/// Reads the header of the file
            ///
            /// @author Ian Copland
			///
			/// @param File stream
			/// @param the file path
            /// @param [Out] Mesh description
            /// @param [Out] A struct containing info on the number of meshes, nodes and joints.
            ///
			/// @return Whether the file is correct
            //-----------------------------------------------------------------------------
            bool ReadGlobalHeader(const Core::FileStreamSPtr& in_meshStream, const std::string& in_filePath, MeshDescriptor& out_meshDesc, MeshDataQuantities& out_meshQuantities)
            {
                u32 fileCheckValue = ReadValue<u32>(in_meshStream);
                if(fileCheckValue != k_fileCheckValue)
                {
                    CS_LOG_ERROR("csmodel file has corruption(incorrect File Check Value): " + in_filePath);
                    return false;
                }
                
                u32 versionNum = ReadValue<u32>(in_meshStream);
                if (versionNum < k_minVersion || versionNum > k_maxVersion)
                {
                    CS_LOG_ERROR("Unsupported csmodel version: " + in_filePath);
                    return false;
                }
                
                //init features
                out_meshDesc.mFeatures.mbHasAnimationData = false;
                out_meshDesc.mFeatures.mbHasMaterial = false;
                out_meshDesc.mFeatures.mbHasTexture = false;
                
                //build the feature declaration from the file
                u32 numFeatures = (u32)ReadValue<u8>(in_meshStream);
                for (u32 i=0; i<numFeatures; ++i)
                {
                    u32 featureType = (u32)ReadValue<u8>(in_meshStream);
                    
                    switch (Feature(featureType))
                    {
                        case Feature::k_hasAnimation:
                            out_meshDesc.mFeatures.mbHasAnimationData = true;
                            break;
                        default:
                            CS_LOG_ERROR("Unknown feature type in csmodel (" + in_filePath + ") feature declaration!");
                            break;
                    }
                }
                
                //read the vertex declaration
                ReadVertexDeclaration(in_meshStream, out_meshDesc);
                
                //read index declaration
                out_meshDesc.mudwIndexSize = ReadValue<u8>(in_meshStream);
                
                //read the min and max bounds
                out_meshDesc.mvMinBounds.x = ReadValue<f32>(in_meshStream);
                out_meshDesc.mvMinBounds.y = ReadValue<f32>(in_meshStream);
                out_meshDesc.mvMinBounds.z = ReadValue<f32>(in_meshStream);
                out_meshDesc.mvMaxBounds.x = ReadValue<f32>(in_meshStream);
                out_meshDesc.mvMaxBounds.y = ReadValue<f32>(in_meshStream);
                out_meshDesc.mvMaxBounds.z = ReadValue<f32>(in_meshStream);
                
                //read the number of meshes
                out_meshQuantities.m_numMeshes = (u32)ReadValue<u16>(in_meshStream);
                out_meshQuantities.m_numSkeletonNodes = 0;
                out_meshQuantities.m_numJoints = 0;
                
                //read num skeleton nodes and joints if used
                if (true == out_meshDesc.mFeatures.mbHasAnimationData)
                {
                    out_meshQuantities.m_numSkeletonNodes = (s32)ReadValue<s16>(in_meshStream);
                    out_meshQuantities.m_numJoints = (u32)ReadValue<u8>(in_meshStream);
                }
                
                return true;
            }
            //----------------------------------------------------------------------------
			/// Read the mesh data from file and creates a mesh descriptor.
            ///
            /// @author Ian Copland
			///
            /// @param The storage location to load from
			/// @param File path
			/// @param [Out] Mesh description
            ///
			/// @return true if successful, false if not
            //----------------------------------------------------------------------------
            bool ReadFile(Core::StorageLocation in_location, const std::string& in_filePath, MeshDescriptor& out_meshDesc)
            {
                Core::FileStreamSPtr meshStream = Core::Application::Get()->GetFileSystem()->CreateFileStream(in_location, in_filePath, Core::FileMode::k_readBinary);
                
                //Check file for corruption
                if(nullptr == meshStream || true == meshStream->IsBad())
                {
                    CS_LOG_ERROR("Cannot open csmodel file: " + in_filePath);
                    return false;
                }
                
                MeshDataQuantities quantities;
                if(ReadGlobalHeader(meshStream, in_filePath, out_meshDesc, quantities) == false)
                {
                    return false;
                }
                
                if (true == out_meshDesc.mFeatures.mbHasAnimationData)
                {
                    ReadSkeletonData(meshStream, quantities, out_meshDesc.m_skeletonDesc);
                }
                
                for(u32 i=0; i<quantities.m_numMeshes; ++i)
                {
                    SubMeshDescriptor subMeshDesc;
                    
                    ReadSubMeshHeader(meshStream, out_meshDesc, subMeshDesc);
                    ReadSubMeshData(meshStream, out_meshDesc, subMeshDesc);
                    
                    out_meshDesc.mMeshes.push_back(subMeshDesc);
                }
                
                meshStream->Close();
                
                return true;
            }
        }
        
        CS_DEFINE_NAMEDTYPE(CSModelProvider);
        
        //-------------------------------------------------------------------------
		//-------------------------------------------------------------------------
        CSModelProviderUPtr CSModelProvider::Create()
        {
            return CSModelProviderUPtr(new CSModelProvider());
        }
		//-------------------------------------------------------------------------
		//-------------------------------------------------------------------------
		bool CSModelProvider::IsA(Core::InterfaceIDType in_interfaceId) const
		{
			return in_interfaceId == ResourceProvider::InterfaceID || in_interfaceId == CSModelProvider::InterfaceID;
		}
        //----------------------------------------------------
        //----------------------------------------------------
        Core::InterfaceIDType CSModelProvider::GetResourceType() const
        {
            return Mesh::InterfaceID;
        }
		//----------------------------------------------------------------------------
		//----------------------------------------------------------------------------
		bool CSModelProvider::CanCreateResourceWithFileExtension(const std::string& in_extension) const
		{
			return in_extension == k_modelFileExtension;
		}
		//----------------------------------------------------------------------------
		//----------------------------------------------------------------------------
		void CSModelProvider::CreateResourceFromFile(Core::StorageLocation in_location, const std::string& in_filePath, const Core::IResourceOptionsBaseCSPtr& in_options, const Core::ResourceSPtr& out_resource)
		{
			MeshSPtr meshResource = std::static_pointer_cast<Mesh>(out_resource);
			
            MeshDescriptor descriptor;
			
			if (ReadFile(in_location, in_filePath, descriptor) == false)
			{
                meshResource->SetLoadState(Core::Resource::LoadState::k_failed);
				return;
			}
			
            BuildMesh(nullptr, descriptor, meshResource);
		}
		//----------------------------------------------------------------------------
		//----------------------------------------------------------------------------
		void CSModelProvider::CreateResourceFromFileAsync(Core::StorageLocation in_location, const std::string& in_filePath, const Core::IResourceOptionsBaseCSPtr& in_options, const AsyncLoadDelegate& in_delegate, const Core::ResourceSPtr& out_resource)
		{
            CS_ASSERT(in_delegate != nullptr, "Cannot load mesh async with null delegate");
            
			MeshSPtr meshResource = std::static_pointer_cast<Mesh>(out_resource);
			
            //Load model as task
			auto task = std::bind(&CSModelProvider::LoadMeshDataTask, this, in_location, in_filePath, in_delegate, meshResource);
			Core::Application::Get()->GetTaskScheduler()->ScheduleTask(task);
		}
		//----------------------------------------------------------------------------
		//----------------------------------------------------------------------------
		void CSModelProvider::LoadMeshDataTask(Core::StorageLocation in_location, const std::string& in_filePath, const AsyncLoadDelegate& in_delegate, const MeshSPtr& out_resource)
		{
			//read the mesh data into a MoStaticDeclaration
			MeshDescriptor descriptor;
			if (false == ReadFile(in_location, in_filePath, descriptor))
			{
                out_resource->SetLoadState(Core::Resource::LoadState::k_failed);
				Core::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(std::bind(in_delegate, out_resource));
				return;
			}
			
			//start a main thread task for loading the data into a mesh
			Core::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(std::bind(&CSModelProvider::BuildMesh, this, in_delegate, descriptor, out_resource));
		}
		//----------------------------------------------------------------------------
		//----------------------------------------------------------------------------
		void CSModelProvider::BuildMesh(const AsyncLoadDelegate& in_delegate, MeshDescriptor& out_meshDesc, const MeshSPtr& out_resource)
		{
			bool success = out_resource->Build(out_meshDesc);

			//cleanup
			for (auto it = out_meshDesc.mMeshes.begin(); it != out_meshDesc.mMeshes.end(); ++it)
			{
				delete[] it->mpVertexData;
				delete[] it->mpIndexData;
			}
			
            Core::Resource::LoadState loadState = success ? Core::Resource::LoadState::k_loaded : Core::Resource::LoadState::k_failed;
            out_resource->SetLoadState(loadState);
            
            if(in_delegate != nullptr)
            {
                in_delegate(out_resource);
            }
		}
	}
}

