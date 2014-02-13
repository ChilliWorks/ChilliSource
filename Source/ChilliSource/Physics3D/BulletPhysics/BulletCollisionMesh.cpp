//
//  BulletCollisionMesh.cpp
//  moFlow
//
//  Created by Ian Copland on 29/05/2012.
//  Copyright (c) 2012 Tag Games Ltd. All rights reserved.
//

#include <moFlo/Physics3D/BulletPhysics/BulletCollisionMesh.h>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

namespace moFlo
{
    namespace Physics3D
    {
        DEFINE_NAMED_INTERFACE(CBulletCollisionMesh);
        //------------------------------------------------------
        /// Constructor
        //------------------------------------------------------
        CBulletCollisionMesh::CBulletCollisionMesh()
        : mpCollisionShape(NULL), mpTriangleArray(NULL), mpIndexData(NULL), mpVertexData(NULL)
        {
        }
        //------------------------------------------------------
        /// Is A
        //------------------------------------------------------
        bool CBulletCollisionMesh::IsA(Core::InterfaceIDType inInterfaceID) const
        {
            return (inInterfaceID == CBulletCollisionMesh::InterfaceID);
        }
        //------------------------------------------------------
        /// Get Data
        //------------------------------------------------------
        btCollisionShape* CBulletCollisionMesh::GetCollisionShape()
        {
            return mpCollisionShape;
        }
        //------------------------------------------------------
        /// Destructor
        //------------------------------------------------------
        CBulletCollisionMesh::~CBulletCollisionMesh()
        {
            Cleanup();
        }
        //------------------------------------------------------
        /// Prepare
        //------------------------------------------------------
        void CBulletCollisionMesh::Prepare(u8* inpIndexData, u32 inudwIndexDataSize, u8* inpVertexData, u32 inudwVertexDataSize)
        {
            Cleanup();
            
            //get the number of triangles and vertices
            u32 udwNumTriangles = (inudwIndexDataSize / 3) / 4;
            u32 udwNumVertices = (inudwIndexDataSize / 3) / 4;
            
            mpTriangleArray = new btTriangleIndexVertexArray(udwNumTriangles, (int*)inpIndexData, 3*sizeof(u32), udwNumVertices, (float*)inpVertexData, 3 * sizeof(f32));
            mpCollisionShape = new btBvhTriangleMeshShape(mpTriangleArray, true, true);
        }
        //------------------------------------------------------
        /// Cleanup
        //------------------------------------------------------
        void CBulletCollisionMesh::Cleanup()
        {
            SAFE_DELETE(mpCollisionShape);
            SAFE_DELETE(mpTriangleArray);
            SAFE_DELETE_ARRAY(mpIndexData);
            SAFE_DELETE_ARRAY(mpVertexData);
        }
    }
}
