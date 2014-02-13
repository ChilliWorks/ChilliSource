/*
 *  BulletComponentFactory.h
 *  moFlow
 *
 *  Created by Tag Games on 28/09/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLOW_PHYSICS3D_BULLETPHYSICS_BULLETCOMPONENTFACTORY_H_
#define _MOFLOW_PHYSICS3D_BULLETPHYSICS_BULLETCOMPONENTFACTORY_H_

#include <moFlo/Physics3D/BulletPhysics/ForwardDeclarations.h>
#include <moFlo/Physics3D/ForwardDeclarations.h>
#include <moFlo/Physics3D/3DPhysicsComponentFactory.h>
#include <moFlo/Core/FileIO/FileSystem.h>

namespace moFlo
{
    namespace Physics3D
    {
        //========================================================
        /// Bullet Component Factory
        ///
        /// Implementation of I3DPhysicsComponentFactory producing 
        /// Bullet objects
        //========================================================
        class CBulletComponentFactory : public Physics3D::I3DPhysicsComponentFactory 
        {
        public:	
            CBulletComponentFactory(CBulletPhysicsSystem* inpPhysSystem);
            virtual ~CBulletComponentFactory(){}
            //-----------------------------------------------------------------------
            /// Is A
            ///
            /// @return Whether this implements the given interface.
            //-----------------------------------------------------------------------
            virtual bool IsA(Core::InterfaceIDType inInterfaceID) const;
            //-----------------------------------------------------------------------
            /// Can Produce Components With Interface
            ///
            /// @return Whether the factory can produce the given type
            //------------------------------------------------------------------------
            bool CanProduceComponentWithInterface(Core::InterfaceIDType inInterfaceID) const;
            //-----------------------------------------------------------------------
            /// Can Produce Components With Type Name
            ///
            /// @return Whether the factory can produce the given name
            //------------------------------------------------------------------------
            bool CanProduceComponentWithTypeName(const std::string& inTypeName) const;
            //-----------------------------------------------------------------------
            /// Create Ground Physics Component
            ///
            /// Create a physical plane that acts as the ground
            ///
            /// @param Normal to plane surface
            /// @param Offet from origin along normal
            /// @return Instantiated component
            //------------------------------------------------------------------------
            Physics3D::I3DPhysicsComponentPtr CreateGroundPhysicsComponent(Core::CVector3 vNorm, f32 fOffset) const;
            //-----------------------------------------------------------------------
            /// Create Sphere Physics Component
            ///
            /// Create a physical, spherical bounding volume
            ///
            /// @param Radius
            /// @return Instantiated component
            //------------------------------------------------------------------------
            Physics3D::I3DPhysicsComponentPtr CreateSpherePhysicsComponent(Physics3D::Physics3DComponentDesc& inDesc, f32 infRadius) const;
            //-----------------------------------------------------------------------
            /// Create Box Physics Component
            ///
            /// Create a physical, cubical bounding volume
            ///
            /// @param Half dimensions
            /// @return Instantiated component
            //------------------------------------------------------------------------
            Physics3D::I3DPhysicsComponentPtr CreateBoxPhysicsComponent(Physics3D::Physics3DComponentDesc& inDesc, const Core::CVector3& invHalfExtents) const;
            //-----------------------------------------------------------------------
            /// Create Physics Component From Existing Resources
            ///
            /// Create a Physics Component containing the Collision Shape and Rigid Body passed in
            ///
            /// @param Half dimensions
            /// @return Instantiated component
            //------------------------------------------------------------------------
            Physics3D::I3DPhysicsComponentPtr CreatePhysicsComponent(btCollisionShape* inpShape, btRigidBody* inpBody) const;
            //-----------------------------------------------------------------------
            /// Create Physics Component
            ///
            /// Create a Physics Component from a BulletCollisionMesh
            ///
            /// @param the collision mesh
            /// @return Instantiated component
            //------------------------------------------------------------------------
            Physics3D::I3DPhysicsComponentPtr CreatePhysicsComponent(Physics3D::Physics3DComponentDesc& inDesc, BulletCollisionMeshPtr inpCollisionMesh) const;
            //-----------------------------------------------------------------------
            /// Create Physics Component
            ///
            /// Create a Physics Component from a mocollision file.
            ///
            /// @param the storage location
            /// @param the filename
            /// @return Instantiated component
            //------------------------------------------------------------------------
            Physics3D::I3DPhysicsComponentPtr CreatePhysicsComponent(Physics3D::Physics3DComponentDesc& inDesc, Core::STORAGE_LOCATION ineStorageLocation, const std::string& instrFilename) const;
            
        private:
            
            CBulletPhysicsSystem* mpPhysSystem;
        };
    }
}

#endif