//
//  CSModelProvider.cpp
//  ChilliSource
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
#include <ChilliSource/Rendering/Model/Model.h>
#include <ChilliSource/Rendering/Model/ModelDesc.h>
#include <ChilliSource/Rendering/Model/IndexFormat.h>
#include <ChilliSource/Rendering/Model/PolygonType.h>

#include <unordered_map>

namespace ChilliSource
{
    namespace
    {
        const std::string k_modelFileExtension("csmodel");
        
        constexpr u32 k_minVersion = 13;
        constexpr u32 k_maxVersion = 13;
        constexpr u32 k_fileCheckValue = 6666;
        
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
            k_tangent,
            k_bitangent,
            k_texcoord,
            k_colour,
            k_weights,
            k_jointIndices
        };
        //----------------------------------------------------------------------------
        /// Model Data Quantities
        ///
        /// A struct containing the quantities of different pieces of mesh data.
        //----------------------------------------------------------------------------
        struct MeshDataQuantities
        {
            u32 m_numMeshes = 0;
            s32 m_numSkeletonNodes = 0;
            u32 m_numJoints = 0;
        };
        //----------------------------------------------------------------------------
        /// A container for mesh header data.
        ///
        /// @author Ian Copland.
        //----------------------------------------------------------------------------
        struct MeshHeader final
        {
            std::string m_name;
            u32 m_numVertices = 0;
            u32 m_numIndices = 0;
            AABB m_aabb;
        };
        //----------------------------------------------------------------------------
        /// A container for mesh data.
        ///
        /// @author Ian Copland.
        //----------------------------------------------------------------------------
        struct MeshData final
        {
            std::unique_ptr<const u8[]> m_vertexData;
            std::unique_ptr<const u8[]> m_indexData;
            std::vector<Matrix4> m_inverseBindPoses;
        };
        //----------------------------------------------------------------------------
        /// A container for model header data.
        ///
        /// @author Ian Copland.
        //----------------------------------------------------------------------------
        struct ModelHeader final
        {
            bool m_hasAnimationData = false;
            VertexFormat m_vertexFormat;
            IndexFormat m_indexFormat = IndexFormat::k_short;
            AABB m_aabb;
        };
        //----------------------------------------------------------------------------
        /// Read block of data in for given type
        ///
        /// @author Ian Copland
        ///
        /// @param File stream
        /// @param Num to read
        /// @param [Out] data
        //----------------------------------------------------------------------------
        template <typename TType> void ReadBlock(IBinaryInputStream* in_meshStream, u32 in_numToRead, TType* out_data)
        {
            in_meshStream->Read(reinterpret_cast<u8*>(out_data), sizeof(TType) * in_numToRead);
        }
        //----------------------------------------------------------------------------
        /// Reads a null terminated string from the stream.
        ///
        /// @author Ian Copland
        ///
        /// @param File stream
        ///
        /// @return The string.
        //----------------------------------------------------------------------------
        std::string ReadString(IBinaryInputStream* in_meshStream) noexcept
        {
            std::string output;
            
            u8 nextChar = 0;
            do
            {
                nextChar = in_meshStream->Read<u8>();
                output += nextChar;
                
            } while(nextChar != 0);
            
            return output;
        }
        //-----------------------------------------------------------------------------
        /// Read the vertex format from the mesh filestream.
        ///
        /// @author Ian Copland
        ///
        /// @param Model stream
        ///
        /// @return the vertex description.
        //-----------------------------------------------------------------------------
        VertexFormat ReadVertexFormat(IBinaryInputStream* in_meshStream)
        {
            //build the vertex declaration from the file
            u8 numVertexElements = in_meshStream->Read<u8>();
            
            std::vector<VertexFormat::ElementType> elements;
            for (int i = 0; i < numVertexElements; ++i)
            {
                u8 vertexAttrib = in_meshStream->Read<u8>();
                
                switch (VertexAttribute(vertexAttrib))
                {
                    case VertexAttribute::k_position:
                        elements.push_back(VertexFormat::ElementType::k_position4);
                        break;
                    case VertexAttribute::k_normal:
                        elements.push_back(VertexFormat::ElementType::k_normal3);
                        break;
                    case VertexAttribute::k_tangent:
                        elements.push_back(VertexFormat::ElementType::k_tangent3);
                        break;
                    case VertexAttribute::k_bitangent:
                        elements.push_back(VertexFormat::ElementType::k_bitangent3);
                        break;
                    case VertexAttribute::k_texcoord:
                        elements.push_back(VertexFormat::ElementType::k_uv2);
                        break;
                    case VertexAttribute::k_colour:
                        elements.push_back(VertexFormat::ElementType::k_colour4);
                        break;
                    case VertexAttribute::k_weights:
                        elements.push_back(VertexFormat::ElementType::k_weight4);
                        break;
                    case VertexAttribute::k_jointIndices:
                        elements.push_back(VertexFormat::ElementType::k_jointIndex4);
                        break;
                    default:
                        CS_LOG_ERROR("Unknown vertex type in vertex declaration!");
                        break;
                }
            }
            
            return VertexFormat(elements);
        }
        //-----------------------------------------------------------------------------
        /// Calculates a bounding sphere from the given AABB.
        ///
        /// @author Ian Copland
        ///
        /// @param in_aabb - The AABB from which to generate a bounding sphere.
        ///
        /// @return The bounding sphere generated.
        //-----------------------------------------------------------------------------
        Sphere CalcBoundingSphere(const AABB& in_aabb) noexcept
        {
            return Sphere(in_aabb.Centre(), in_aabb.GetSize().Length() * 0.5f);
        }
        //-----------------------------------------------------------------------------
        /// Reads the sub-mesh header section of the file
        ///
        /// @author Ian Copland
        ///
        /// @param File stream
        /// @param Model description
        /// @param [Out] Sube mesh description
        //-----------------------------------------------------------------------------
        MeshHeader ReadMeshHeader(IBinaryInputStream* in_meshStream, IndexFormat in_indexFormat)
        {
            MeshHeader meshHeader;
            
            meshHeader.m_name = ReadString(in_meshStream);
            
            CS_ASSERT(in_indexFormat == IndexFormat::k_short, "Only short indices are currently supported.");
            
            constexpr u32 k_indicesPerTriangle = 3;
            meshHeader.m_numVertices = (u32)in_meshStream->Read<u16>();
            meshHeader.m_numIndices = ((u32)in_meshStream->Read<u16>()) * k_indicesPerTriangle;
            
            Vector3 minBound, maxBound;
            minBound.x = in_meshStream->Read<f32>();
            minBound.y = in_meshStream->Read<f32>();
            minBound.z = in_meshStream->Read<f32>();
            maxBound.x = in_meshStream->Read<f32>();
            maxBound.y = in_meshStream->Read<f32>();
            maxBound.z = in_meshStream->Read<f32>();
            
            meshHeader.m_aabb = AABB((maxBound + minBound) * 0.5f, maxBound - minBound);
            
            return meshHeader;
        }
        //-----------------------------------------------------------------------------
        /// Read the mesh section of the file
        ///
        /// @author Ian Copland
        ///
        /// @param in_meshStream - File stream
        /// @param in_vertexDataSize - The vertex data size.
        /// @param in_indexDataSize - The index data size.
        /// @param in_numJoints - The number of joints in the mesh. Defaults to 0.
        ///
        /// @return The mesh data
        //-----------------------------------------------------------------------------
        MeshData ReadMeshData(IBinaryInputStream*in_meshStream, u32 in_vertexDataSize, u32 in_indexDataSize, u32 in_numJoints = 0)
        {
            MeshData meshData;
            
            for(u32 i = 0; i < in_numJoints; ++i)
            {
                Matrix4 IBPMat;
                ReadBlock<f32>(in_meshStream, 16, IBPMat.m);
                
                meshData.m_inverseBindPoses.push_back(IBPMat);
            }
            
            auto vertexData = new u8[in_vertexDataSize];
            in_meshStream->Read(vertexData, in_vertexDataSize);
            meshData.m_vertexData = std::unique_ptr<const u8[]>(vertexData);
            
            auto indexData = new u8[in_indexDataSize];
            in_meshStream->Read(indexData, in_indexDataSize);
            meshData.m_indexData = std::unique_ptr<const u8[]>(indexData);
            
            return meshData;
        }
        //-----------------------------------------------------------------------------
        /// Reads the skeleton section of the file
        ///
        /// @author Ian Copland
        ///
        /// @param File stream
        /// @param Container holding the num of meshes, joints and bones
        ///
        /// @return The skeleton description.
        //-----------------------------------------------------------------------------
        SkeletonDesc ReadSkeletonData(IBinaryInputStream* in_meshStream, const MeshDataQuantities& in_quantities)
        {
            std::vector<std::string> names;
            std::vector<s32> parentNodeIndices;
            
            names.reserve(in_quantities.m_numSkeletonNodes);
            parentNodeIndices.reserve(in_quantities.m_numSkeletonNodes);
            
            std::unordered_map<u32, s32> jointToNodeMap;
            for (u32 i = 0; i<(u32)in_quantities.m_numSkeletonNodes; ++i)
            {
                names.push_back(ReadString(in_meshStream));
                
                //get the parent index
                parentNodeIndices.push_back(s32(in_meshStream->Read<s16>()));
                
                //get the type
                constexpr u32 k_isJoint = 1;
                u8 type = in_meshStream->Read<u8>();
                if (type == k_isJoint)
                {
                    u32 jointIndex = (u32)in_meshStream->Read<u8>();
                    jointToNodeMap.insert(std::pair<u32, s32>(jointIndex, (s32)i));
                }
            }
            
            CS_ASSERT(names.size() == parentNodeIndices.size(), "Invalid number of node names and indices in skeleton");
            
            
            std::vector<s32> jointIndices;
            for (u32 i = 0; i < in_quantities.m_numJoints; ++i)
            {
                jointIndices.push_back(jointToNodeMap[i]);
            }
            
            return SkeletonDesc(names, parentNodeIndices, jointIndices);
        }
        //-----------------------------------------------------------------------------
        /// Reads the header of the file
        ///
        /// @author Ian Copland
        ///
        /// @param File stream
        /// @param the file path
        /// @param [Out] Model description
        /// @param [Out] A struct containing info on the number of meshes, nodes and joints.
        ///
        /// @return Whether the file is correct
        //-----------------------------------------------------------------------------
        void ReadGlobalHeader(IBinaryInputStream* in_meshStream, const std::string& in_filePath, ModelHeader& out_modelHeader, MeshDataQuantities& out_meshQuantities)
        {
            u32 fileCheckValue = in_meshStream->Read<u32>();
            CS_RELEASE_ASSERT(fileCheckValue == k_fileCheckValue, "csmodel file is corrupt (incorrect File Check Value): " + in_filePath);
            
            u32 versionNum = in_meshStream->Read<u32>();
            CS_RELEASE_ASSERT(versionNum >= k_minVersion && versionNum <= k_maxVersion, "Unsupported csmodel version: " + in_filePath);
            
            u32 numFeatures = (u32)in_meshStream->Read<u8>();
            for (u32 i=0; i<numFeatures; ++i)
            {
                u32 featureType = (u32)in_meshStream->Read<u8>();
                
                switch (Feature(featureType))
                {
                    case Feature::k_hasAnimation:
                        out_modelHeader.m_hasAnimationData = true;
                        break;
                    default:
                        CS_LOG_ERROR("Unknown feature type in csmodel (" + in_filePath + ") feature declaration!");
                        break;
                }
            }
            
            out_modelHeader.m_vertexFormat = ReadVertexFormat(in_meshStream);
            
            constexpr u8 k_shortIndexFormatSize = 2;
            auto indexSize = in_meshStream->Read<u8>();
            CS_RELEASE_ASSERT(indexSize == k_shortIndexFormatSize, "Invalid index size.");
            
            Vector3 minBounds, maxBounds;
            minBounds.x = in_meshStream->Read<f32>();
            minBounds.y = in_meshStream->Read<f32>();
            minBounds.z = in_meshStream->Read<f32>();
            maxBounds.x = in_meshStream->Read<f32>();
            maxBounds.y = in_meshStream->Read<f32>();
            maxBounds.z = in_meshStream->Read<f32>();
            out_modelHeader.m_aabb = AABB((maxBounds + minBounds) * 0.5f, maxBounds - minBounds);
            
            out_meshQuantities.m_numMeshes = (u32)in_meshStream->Read<u16>();
            if (out_modelHeader.m_hasAnimationData)
            {
                out_meshQuantities.m_numSkeletonNodes = (s32)in_meshStream->Read<s16>();
                out_meshQuantities.m_numJoints = (u32)in_meshStream->Read<u8>();
            }
        }
        //----------------------------------------------------------------------------
        /// Read the mesh data from file and creates a mesh descriptor.
        ///
        /// @author Ian Copland
        ///
        /// @param The storage location to load from
        /// @param File path
        /// @param [Out] Model description
        ///
        /// @return true if successful, false if not
        //----------------------------------------------------------------------------
        bool ReadFile(StorageLocation in_location, const std::string& in_filePath, ModelDesc& out_modelDesc)
        {
            auto meshStream = Application::Get()->GetFileSystem()->CreateBinaryInputStream(in_location, in_filePath);
            
            //Check file for corruption
            if(nullptr == meshStream)
            {
                CS_LOG_ERROR("Cannot open csmodel file: " + in_filePath);
                return false;
            }
            
            ModelHeader modelHeader;
            MeshDataQuantities quantities;
            ReadGlobalHeader(meshStream.get(), in_filePath, modelHeader, quantities);
            
            SkeletonDesc skeletonDesc;
            if (modelHeader.m_hasAnimationData)
            {
                skeletonDesc = ReadSkeletonData(meshStream.get(), quantities);
            }
            
            std::vector<MeshDesc> meshDescs;
            for(u32 i = 0; i < quantities.m_numMeshes; ++i)
            {
                auto meshHeader = ReadMeshHeader(meshStream.get(), modelHeader.m_indexFormat);
                
                CS_ASSERT(modelHeader.m_indexFormat == IndexFormat::k_short, "Invalid index format.");
                constexpr u32 k_indexSize = 2;
                auto meshData = ReadMeshData(meshStream.get(), meshHeader.m_numVertices * modelHeader.m_vertexFormat.GetSize(), meshHeader.m_numIndices * k_indexSize, quantities.m_numJoints);
                auto meshBoundingSphere = CalcBoundingSphere(meshHeader.m_aabb);
                
                meshDescs.push_back(MeshDesc(meshHeader.m_name, PolygonType::k_triangle, modelHeader.m_vertexFormat, modelHeader.m_indexFormat, meshHeader.m_aabb, meshBoundingSphere, meshHeader.m_numVertices,
                                             meshHeader.m_numIndices, std::move(meshData.m_vertexData), std::move(meshData.m_indexData), std::move(meshData.m_inverseBindPoses)));
                
            }
            
            auto modelBoundingSphere = CalcBoundingSphere(modelHeader.m_aabb);
            out_modelDesc = ModelDesc(std::move(meshDescs), modelHeader.m_aabb, modelBoundingSphere, skeletonDesc, false);

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
    bool CSModelProvider::IsA(InterfaceIDType in_interfaceId) const
    {
        return in_interfaceId == ResourceProvider::InterfaceID || in_interfaceId == CSModelProvider::InterfaceID;
    }
    //----------------------------------------------------
    //----------------------------------------------------
    InterfaceIDType CSModelProvider::GetResourceType() const
    {
        return Model::InterfaceID;
    }
    //----------------------------------------------------------------------------
    //----------------------------------------------------------------------------
    bool CSModelProvider::CanCreateResourceWithFileExtension(const std::string& in_extension) const
    {
        return in_extension == k_modelFileExtension;
    }
    //----------------------------------------------------------------------------
    //----------------------------------------------------------------------------
    void CSModelProvider::CreateResourceFromFile(StorageLocation in_location, const std::string& in_filePath, const IResourceOptionsBaseCSPtr& in_options, const ResourceSPtr& out_resource)
    {
        auto modelResource = static_cast<Model*>(out_resource.get());
        
        ModelDesc modelDesc;
        
        if (ReadFile(in_location, in_filePath, modelDesc) == false)
        {
            modelResource->SetLoadState(Resource::LoadState::k_failed);
            return;
        }
        
        modelResource->Build(std::move(modelDesc));
        modelResource->SetLoadState(Resource::LoadState::k_loaded);
    }
    //----------------------------------------------------------------------------
    //----------------------------------------------------------------------------
    void CSModelProvider::CreateResourceFromFileAsync(StorageLocation in_location, const std::string& in_filePath, const IResourceOptionsBaseCSPtr& in_options, const AsyncLoadDelegate& in_delegate, const ResourceSPtr& out_resource)
    {
        CS_ASSERT(in_delegate != nullptr, "Cannot load mesh async with null delegate");
        
        ModelSPtr meshResource = std::static_pointer_cast<Model>(out_resource);
        
        //Load model as task
        Application::Get()->GetTaskScheduler()->ScheduleTask(TaskType::k_file, [=](const TaskContext&) noexcept
        {
            LoadMeshDataTask(in_location, in_filePath, in_delegate, meshResource);
        });
    }
    //----------------------------------------------------------------------------
    //----------------------------------------------------------------------------
    void CSModelProvider::LoadMeshDataTask(StorageLocation in_location, const std::string& in_filePath, const AsyncLoadDelegate& in_delegate, const ModelSPtr& out_resource)
    {
        //read the mesh data into a MoStaticDeclaration
        ModelDescSPtr modelDesc(new ModelDesc());
        if (false == ReadFile(in_location, in_filePath, *modelDesc))
        {
            out_resource->SetLoadState(Resource::LoadState::k_failed);
            Application::Get()->GetTaskScheduler()->ScheduleTask(TaskType::k_mainThread, [=](const TaskContext&) noexcept
            {
                in_delegate(out_resource);
            });
        }
        
        //start a main thread task for loading the data into a mesh
        Application::Get()->GetTaskScheduler()->ScheduleTask(TaskType::k_large, [=](const TaskContext&) noexcept
        {
            out_resource->Build(std::move(*modelDesc));
            out_resource->SetLoadState(Resource::LoadState::k_loaded);
            
           in_delegate(out_resource);
        });
    }
}
