//
//  SubMesh.h
//  Chilli Source
//  Created by Scott Downie on 08/10/2010.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2010 Tag Games Limited
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

#ifndef _CHILLISOURCE_RENDERING_SUB_MESH_H_
#define _CHILLISOURCE_RENDERING_SUB_MESH_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Rendering/Model/Mesh.h>
#include <ChilliSource/Core/Math/Vector2.h>
#include <ChilliSource/Core/Math/Vector3.h>
#include <ChilliSource/Core/Math/Vector4.h>
#include <ChilliSource/Core/Math/Matrix4.h>
#include <ChilliSource/Core/Math/Geometry/Shapes.h>
#include <ChilliSource/Core/File/FileSystem.h>

namespace ChilliSource
{
    //===============================================
    /// Inverse Bind Pose
    //===============================================
    struct InverseBindPose
    {
        std::vector<Matrix4> mInverseBindPoseMatrices;
    };
    typedef std::shared_ptr<InverseBindPose> InverseBindPosePtr;
    
    class SubMesh
    {
    public:
        
        virtual ~SubMesh();
        
        //-----------------------------------------------------------------
        /// Get AABB
        ///
        /// @return AABB
        //-----------------------------------------------------------------
        const AABB& GetAABB() const;
        //-----------------------------------------------------------------
        /// Get Name
        ///
        /// @return The meshes name
        //-----------------------------------------------------------------
        const std::string& GetName() const;
        //-----------------------------------------------------------------
        /// Get Number of Vertices
        ///
        /// @return Number of verts in this sub-mesh
        //-----------------------------------------------------------------
        u32 GetNumVerts() const;
        //-----------------------------------------------------------------
        /// Get Number of Indices
        ///
        /// @return Number of indices in this sub-mesh
        //-----------------------------------------------------------------
        u32 GetNumIndices() const;
        
    private:
        //Only the mesh can create this
        SubMesh(const std::string& instrName);
        //-----------------------------------------------------------------
        /// Alter Buffer Declaration
        ///
        /// Changes the vertex declaration in the mesh buffer. This should
        /// be called when reusing a sub mesh for different mesh data.
        /// Build should be called following this, with the new mesh data.
        /// 
        /// @param the new vertex declaration.
        /// @param the new size of indices.
        //-----------------------------------------------------------------
        void AlterBufferDeclaration(const VertexDeclaration& inVertexDeclaration, u32 inudwIndexSizeInBytes);
        //-----------------------------------------------------------------
        /// Build
        ///
        /// Fills the sub mesh with vertex and index data. This data will
        /// be passed directly into the Mesh Buffer.
        /// 
        /// @param the pre-formatted vertex data.
        /// @param the pre-formatted index data.
        /// @param the number of vertices.
        /// @param the number of indices.
        /// @param the minimum bounds.
        /// @param the maximum bounds.
        //-----------------------------------------------------------------
        void Build(void* inpVertexData, void*inpIndexData, u32 inudwNumVertices, u32 indwNumIndices, Vector3 invMin, Vector3 invMax);
        //-----------------------------------------------------------------
        /// Set Inverse Bind Pose
        /// 
        /// @param the new InverseBindPose
        //-----------------------------------------------------------------
        void SetInverseBindPose(const InverseBindPosePtr& inpInverseBindPose);
        
        friend class Mesh;
        
    private:
        
        std::string mstrName;
        AABB mBoundingBox;
        
        InverseBindPosePtr mpInverseBindPose;
    };
}

#endif