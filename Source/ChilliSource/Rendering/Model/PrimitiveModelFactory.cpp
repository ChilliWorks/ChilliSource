//
//  PrimitiveModelFactory.cpp
//  ChilliSource
//  Created by Ian Copland on 03/08/2015.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2015 Tag Games Limited
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

#include <ChilliSource/Rendering/Model/PrimitiveModelFactory.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Resource/ResourcePool.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>
#include <ChilliSource/Rendering/Model/Model.h>
#include <ChilliSource/Rendering/Model/ModelDesc.h>

namespace ChilliSource
{
    namespace
    {
        //------------------------------------------------------------------------------
        /// Creates a new model description describing a plane model.
        ///
        /// @author Ian Copland
        ///
        /// @param in_size - The size of the model.
        /// @param in_textureRepeat - The number of times the texture is repeated.
        ///
        /// @return The new model description.
        //------------------------------------------------------------------------------
        ModelDesc CreatePlaneDesc(const Vector2& in_size, const Vector2& in_textureRepeat)
        {
            constexpr u32 k_numVertices = 4;
            constexpr u32 k_numIndices = 6;
            const Vector2 halfSize = in_size * 0.5f;

            // Vertices
            auto vertices = new StaticMeshVertex[k_numVertices];
            vertices[0] = { Vector4(-halfSize.x, 0.0f, -halfSize.y, 1.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector2(0.0f, 0.0f) };
            vertices[1] = { Vector4(-halfSize.x, 0.0f, halfSize.y, 1.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector2(0.0f, in_textureRepeat.y) };
            vertices[2] = { Vector4(halfSize.x, 0.0f, -halfSize.y, 1.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector2(in_textureRepeat.x, 0.0f) };
            vertices[3] = { Vector4(halfSize.x, 0.0f, halfSize.y, 1.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector2(in_textureRepeat.x, in_textureRepeat.y) };
            
            // Indices
            u16* indices(new u16[k_numIndices]);
            indices[0] = 0;
            indices[1] = 2;
            indices[2] = 1;
            indices[3] = 1;
            indices[4] = 2;
            indices[5] = 3;
            
            auto name = "PlaneMesh";
            auto polygonType = PolygonType::k_triangle;
            auto vertexFormat = VertexFormat::k_staticMesh;
            auto indexFormat = IndexFormat::k_short;
            AABB aabb(Vector3::k_zero, Vector3(in_size.x, 0.0f, in_size.y));
            Sphere boundingSphere(Vector3::k_zero, in_size.Length() * 0.5f);
            std::unique_ptr<const u8[]> vertexData(reinterpret_cast<const u8*>(vertices));
            std::unique_ptr<const u8[]> indexData(reinterpret_cast<const u8*>(indices));
            
            std::vector<MeshDesc> meshDescs;
            meshDescs.push_back(MeshDesc(name, polygonType, vertexFormat, indexFormat, aabb, boundingSphere, k_numVertices, k_numIndices, std::move(vertexData), std::move(indexData)));
            
            return ModelDesc(std::move(meshDescs), aabb, boundingSphere);
        }
        //------------------------------------------------------------------------------
        /// Creates a new model description describing a box model.
        ///
        /// @author Ian Copland
        ///
        /// @param in_size - The size of the model.
        /// @param in_textureRepeat - The number of times the texture is repeated on
        /// each face.
        /// @param in_flipNormals - Whether or not to flip the box inside out.
        /// This is useful for creating rooms
        ///
        /// @return The new model description.
        //------------------------------------------------------------------------------
        ModelDesc CreateBoxDesc(const Vector3& in_size, const Vector2& in_textureRepeat, bool in_flipNormals)
        {
            constexpr u32 k_numVertices = 24;
            constexpr u32 k_numIndices = 36;
            const Vector3 halfSize = in_size * 0.5f;
            
            //Vertices
            auto vertices = new StaticMeshVertex[k_numVertices];
            float normalDir = (in_flipNormals == false) ? 1.0f : -1.0f;
            
            //bottom
            vertices[0] = { Vector4(-halfSize.x, -halfSize.y, -halfSize.z, 1.0f), Vector3(0.0f, -normalDir, 0.0f), Vector3(-normalDir, 0.0f, 0.0f), Vector3(0.0f, 0.0f, normalDir), Vector2(0.0f, 0.0f) };
            vertices[1] = { Vector4(-halfSize.x, -halfSize.y, halfSize.z, 1.0f), Vector3(0.0f, -normalDir, 0.0f), Vector3(-normalDir, 0.0f, 0.0f), Vector3(0.0f, 0.0f, normalDir), Vector2(0.0f, in_textureRepeat.y) };
            vertices[2] = { Vector4(halfSize.x, -halfSize.y, -halfSize.z, 1.0f), Vector3(0.0f, -normalDir, 0.0f), Vector3(-normalDir, 0.0f, 0.0f), Vector3(0.0f, 0.0f, normalDir), Vector2(in_textureRepeat.x, 0.0f) };
            vertices[3] = { Vector4(halfSize.x, -halfSize.y, halfSize.z, 1.0f), Vector3(0.0f, -normalDir, 0.0f), Vector3(-normalDir, 0.0f, 0.0f), Vector3(0.0f, 0.0f, normalDir), Vector2(in_textureRepeat.x, in_textureRepeat.y) };
            
            //top
            vertices[4] = { Vector4(halfSize.x, halfSize.y, -halfSize.z, 1.0f), Vector3(0.0f, normalDir, 0.0f), Vector3(normalDir, 0.0f, 0.0f), Vector3(0.0f, 0.0f, -normalDir), Vector2(0.0f, 0.0f) };
            vertices[5] = { Vector4(halfSize.x, halfSize.y, halfSize.z, 1.0f), Vector3(0.0f, normalDir, 0.0f), Vector3(normalDir, 0.0f, 0.0f), Vector3(0.0f, 0.0f, -normalDir), Vector2(0.0f, in_textureRepeat.y) };
            vertices[6] = { Vector4(-halfSize.x, halfSize.y, -halfSize.z, 1.0f), Vector3(0.0f, normalDir, 0.0f), Vector3(normalDir, 0.0f, 0.0f), Vector3(0.0f, 0.0f, -normalDir), Vector2(in_textureRepeat.x, 0.0f) };
            vertices[7] = { Vector4(-halfSize.x, halfSize.y, halfSize.z, 1.0f), Vector3(0.0f, normalDir, 0.0f), Vector3(normalDir, 0.0f, 0.0f), Vector3(0.0f, 0.0f, -normalDir), Vector2(in_textureRepeat.x, in_textureRepeat.y) };
            
            //left
            vertices[8] = { Vector4(-halfSize.x, halfSize.y, -halfSize.z, 1.0f), Vector3(-normalDir, 0.0f, 0.0f), Vector3(0.0f, -normalDir, 0.0f), Vector3(0.0f, 0.0f, -normalDir), Vector2(0.0f, 0.0f) };
            vertices[9] = { Vector4(-halfSize.x, halfSize.y, halfSize.z, 1.0f), Vector3(-normalDir, 0.0f, 0.0f), Vector3(0.0f, -normalDir, 0.0f), Vector3(0.0f, 0.0f, -normalDir), Vector2(0.0f, in_textureRepeat.y) };
            vertices[10] = { Vector4(-halfSize.x, -halfSize.y, -halfSize.z, 1.0f), Vector3(-normalDir, 0.0f, 0.0f), Vector3(0.0f, -normalDir, 0.0f), Vector3(0.0f, 0.0f, -normalDir), Vector2(in_textureRepeat.x, 0.0f) };
            vertices[11] = { Vector4(-halfSize.x, -halfSize.y, halfSize.z, 1.0f), Vector3(-normalDir, 0.0f, 0.0f), Vector3(0.0f, -normalDir, 0.0f), Vector3(0.0f, 0.0f, -normalDir), Vector2(in_textureRepeat.x, in_textureRepeat.y) };
            
            //right
            vertices[12] = { Vector4(halfSize.x, -halfSize.y, -halfSize.z, 1.0f), Vector3(normalDir, 0.0f, 0.0f), Vector3(0.0f, normalDir, 0.0f), Vector3(0.0f, 0.0f, -normalDir), Vector2(0.0f, 0.0f) };
            vertices[13] = { Vector4(halfSize.x, -halfSize.y, halfSize.z, 1.0f), Vector3(normalDir, 0.0f, 0.0f), Vector3(0.0f, normalDir, 0.0f), Vector3(0.0f, 0.0f, -normalDir), Vector2(0.0f, in_textureRepeat.y) };
            vertices[14] = { Vector4(halfSize.x, halfSize.y, -halfSize.z, 1.0f), Vector3(normalDir, 0.0f, 0.0f), Vector3(0.0f, normalDir, 0.0f), Vector3(0.0f, 0.0f, -normalDir), Vector2(in_textureRepeat.x, 0.0f) };
            vertices[15] = { Vector4(halfSize.x, halfSize.y, halfSize.z, 1.0f), Vector3(normalDir, 0.0f, 0.0f), Vector3(0.0f, normalDir, 0.0f), Vector3(0.0f, 0.0f, -normalDir), Vector2(in_textureRepeat.x, in_textureRepeat.y) };
            
            //front
            vertices[16] = { Vector4(-halfSize.x, halfSize.y, -halfSize.z, 1.0f), Vector3(0.0f, 0.0f, -normalDir), Vector3(normalDir, 0.0f, 0.0f), Vector3(0.0f, normalDir, 0.0f), Vector2(0.0f, 0.0f) };
            vertices[17] = { Vector4(-halfSize.x, -halfSize.y, -halfSize.z, 1.0f), Vector3(0.0f, 0.0f, -normalDir), Vector3(normalDir, 0.0f, 0.0f), Vector3(0.0f, normalDir, 0.0f), Vector2(0.0f, in_textureRepeat.y) };
            vertices[18] = { Vector4(halfSize.x, halfSize.y, -halfSize.z, 1.0f), Vector3(0.0f, 0.0f, -normalDir), Vector3(normalDir, 0.0f, 0.0f), Vector3(0.0f, normalDir, 0.0f), Vector2(in_textureRepeat.x, 0.0f) };
            vertices[19] = { Vector4(halfSize.x, -halfSize.y, -halfSize.z, 1.0f), Vector3(0.0f, 0.0f, -normalDir), Vector3(normalDir, 0.0f, 0.0f), Vector3(0.0f, normalDir, 0.0f), Vector2(in_textureRepeat.x, in_textureRepeat.y) };
            
            //back
            vertices[20] = { Vector4(-halfSize.x, -halfSize.y, halfSize.z, 1.0f), Vector3(0.0f, 0.0f, normalDir), Vector3(normalDir, 0.0f, 0.0f), Vector3(0.0f, -normalDir, 0.0f), Vector2(0.0f, 0.0f) };
            vertices[21] = { Vector4(-halfSize.x, halfSize.y, halfSize.z, 1.0f), Vector3(0.0f, 0.0f, normalDir), Vector3(normalDir, 0.0f, 0.0f), Vector3(0.0f, -normalDir, 0.0f), Vector2(0.0f, in_textureRepeat.y) };
            vertices[22] = { Vector4(halfSize.x, -halfSize.y, halfSize.z, 1.0f), Vector3(0.0f, 0.0f, normalDir), Vector3(normalDir, 0.0f, 0.0f), Vector3(0.0f, -normalDir, 0.0f), Vector2(in_textureRepeat.x, 0.0f) };
            vertices[23] = { Vector4(halfSize.x, halfSize.y, halfSize.z, 1.0f), Vector3(0.0f, 0.0f, normalDir), Vector3(normalDir, 0.0f, 0.0f), Vector3(0.0f, -normalDir, 0.0f), Vector2(in_textureRepeat.x, in_textureRepeat.y) };
            
            //Indices
            u16 *indices(new u16[k_numIndices]);
            
            //bottom
            indices[0] = 0;
            indices[1] = 1;
            indices[2] = 2;
            indices[3] = 1;
            indices[4] = 3;
            indices[5] = 2;
            
            //top
            indices[6] = 4;
            indices[7] = 5;
            indices[8] = 6;
            indices[9] = 5;
            indices[10] = 7;
            indices[11] = 6;
            
            //left
            indices[12] = 8;
            indices[13] = 9;
            indices[14] = 10;
            indices[15] = 9;
            indices[16] = 11;
            indices[17] = 10;
            
            //right
            indices[18] = 12;
            indices[19] = 13;
            indices[20] = 14;
            indices[21] = 13;
            indices[22] = 15;
            indices[23] = 14;
            
            //front
            indices[24] = 16;
            indices[25] = 17;
            indices[26] = 18;
            indices[27] = 17;
            indices[28] = 19;
            indices[29] = 18;
            
            //back
            indices[30] = 20;
            indices[31] = 21;
            indices[32] = 22;
            indices[33] = 21;
            indices[34] = 23;
            indices[35] = 22;
            
            if (in_flipNormals == true)
            {
                for (u32 i = 0; i < k_numIndices; i += 3)
                {
                    u16 temp = indices[i + 1];
                    indices[i + 1] = indices[i + 2];
                    indices[i + 2] = temp;
                }
            }
        
            auto name = "BoxMesh";
            auto polygonType = PolygonType::k_triangle;
            auto vertexFormat = VertexFormat::k_staticMesh;
            auto indexFormat = IndexFormat::k_short;
            AABB aabb(Vector3::k_zero, in_size);
            Sphere boundingSphere(Vector3::k_zero, in_size.Length() / 0.5f);
            std::unique_ptr<const u8[]> vertexData(reinterpret_cast<const u8*>(vertices));
            std::unique_ptr<const u8[]> indexData(reinterpret_cast<const u8*>(indices));
            
            std::vector<MeshDesc> meshDescs;
            meshDescs.push_back(MeshDesc(name, polygonType, vertexFormat, indexFormat, aabb, boundingSphere, k_numVertices, k_numIndices, std::move(vertexData), std::move(indexData)));
            
            return ModelDesc(std::move(meshDescs), aabb, boundingSphere);
        }
    }
    
    CS_DEFINE_NAMEDTYPE(PrimitiveModelFactory);
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    PrimitiveModelFactoryUPtr PrimitiveModelFactory::Create()
    {
        return PrimitiveModelFactoryUPtr(new PrimitiveModelFactory());
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    bool PrimitiveModelFactory::IsA(InterfaceIDType in_interfaceId) const
    {
        return (PrimitiveModelFactory::InterfaceID == in_interfaceId);
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    ModelCSPtr PrimitiveModelFactory::CreatePlane(const Vector2& in_size, const Vector2& in_textureRepeat) const
    {
        CS_RELEASE_ASSERT(Application::Get()->GetTaskScheduler()->IsMainThread(), "Cannot create models on a background thread.");
        
        auto resourcePool = Application::Get()->GetResourcePool();
        auto meshName = "_PrimitivePlane(" + ToString(in_size) + ")";
        auto mesh = resourcePool->GetResource<Model>(meshName);
        
        if (mesh == nullptr)
        {
            auto mutableMesh = resourcePool->CreateResource<Model>(meshName);
            
            auto desc = CreatePlaneDesc(in_size, in_textureRepeat);
            mutableMesh->Build(std::move(desc));
            mutableMesh->SetLoadState(Resource::LoadState::k_loaded);
            
            mesh = mutableMesh;
        }
        
        return mesh;
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    ModelCSPtr PrimitiveModelFactory::CreateBox(const Vector3& in_size, const Vector2& in_textureRepeat, bool in_flipNormals) const
    {
        CS_RELEASE_ASSERT(Application::Get()->GetTaskScheduler()->IsMainThread(), "Cannot create models on a background thread.");
        
        auto resourcePool = Application::Get()->GetResourcePool();
        auto meshName = "_PrimitiveBox(" + ToString(in_size) + "|" + ToString(in_textureRepeat) + "|" + ToString(in_flipNormals) + ")";
        auto mesh = resourcePool->GetResource<Model>(meshName);
        
        if (mesh == nullptr)
        {
            auto mutableMesh = resourcePool->CreateResource<Model>(meshName);
            
            auto desc = CreateBoxDesc(in_size, in_textureRepeat, in_flipNormals);
            mutableMesh->Build(std::move(desc));
            mutableMesh->SetLoadState(Resource::LoadState::k_loaded);
            
            mesh = mutableMesh;
        }
        
        return mesh;
    }
}
