//
//  BulletCollisionMesh.h
//  moFlow
//
//  Created by Ian Copland on 29/05/2012.
//  Copyright (c) 2012 Tag Games Ltd. All rights reserved.
//

#ifndef _MOFLOW_PHYSICS_PHYSICS3D_BULLETCOLLISIONMESH_H_
#define _MOFLOW_PHYSICS_PHYSICS3D_BULLETCOLLISIONMESH_H_

#include <ChilliSource/Physics3D/BulletPhysics/ForwardDeclarations.h>
#include <ChilliSource/Core/Resource.h>
#include <BulletCollision/CollisionShapes/btTriangleIndexVertexArray.h>

namespace moFlo
{
    namespace Physics3D
    {
        //==========================================================
        /// Bullet Collision Mesh
        ///
        /// A resource containing bullet physics collision
        /// mesh data.
        //==========================================================
        class CBulletCollisionMesh : public Core::IResource
        {
        public:
            DECLARE_NAMED_INTERFACE(CBulletCollisionMesh);
            //------------------------------------------------------
			/// Is A
			///
            /// @param the interface ID.
			/// @return Whether this object is of given type
			//------------------------------------------------------
			bool IsA(Core::InterfaceIDType inInterfaceID) const;
            //------------------------------------------------------
			/// Get CollisionShape
			///
			/// @return the bullet mesh data.
			//------------------------------------------------------
            btCollisionShape* GetCollisionShape();
            //------------------------------------------------------
            /// Destructor
            //------------------------------------------------------
            virtual ~CBulletCollisionMesh();
            
        protected:
            //------------------------------------------------------
            /// Constructor
            ///
            /// Delcared protected so only the bullet collision mesh
            /// manager can create this.
            //------------------------------------------------------
            CBulletCollisionMesh();
            //------------------------------------------------------
            /// Prepare
            ///
            /// Sets up the bullet collision shape.
            ///
            /// @param the index data.
            /// @param the index data size.
            /// @param the vertex data.
            /// @param the vertex data size.
            //------------------------------------------------------
            void Prepare(u8* inpIndexData, u32 inudwIndexDataSize, u8* inpVertexData, u32 inudwVertexDataSize);
            //------------------------------------------------------
            /// Cleanup
            ///
            /// Deletes all stored data.
            //------------------------------------------------------
            void Cleanup();
            
            friend class CBulletCollisionMeshManager;
            friend class CBulletCollisionMeshLoader;
        private:
            
            btCollisionShape* mpCollisionShape;
            btTriangleIndexVertexArray* mpTriangleArray;
            u8* mpIndexData;
            u8* mpVertexData;
        };
    }
}

#endif
