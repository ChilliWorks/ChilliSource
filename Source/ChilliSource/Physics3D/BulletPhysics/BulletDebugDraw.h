//
//  BulletDebugDraw.h
//  moFlow
//
//  Created by Robert Henning on 28/11/2011.
//  Copyright (c) 2011 Tag Games. All rights reserved.
//

#ifndef _MOFLOW_PHYSICS3D_BULLETPHYSICS_BULLETDEBUGDRAW_H_
#define _MOFLOW_PHYSICS3D_BULLETPHYSICS_BULLETDEBUGDRAW_H_

#include <moFlo/Physics3D/BulletPhysics/ForwardDeclarations.h>
#include <LinearMath/btIDebugDraw.h>
#include <moFlo/Rendering/MeshManager.h>
#include <moFlo/Rendering/MeshDescriptor.h>
#include <moFlo/Rendering/Components/StaticMeshComponent.h>
#include <moFlo/Rendering/Renderer.h>
#include <moFlo/Core/Entity.h>

namespace moFlo
{
    namespace Physics3D
    {
        //========================================================
        /// Constants
        //========================================================
        const u32 kudwBulletDebugDrawBufferSize = 1024;
        //========================================================
        /// Debug Draw Vertex
        //========================================================
        struct DebugDrawVertex
        {
            float x,y,z;
            float a,b,g,r;
        };
        //========================================================
        /// Debug Draw Vertex
        //========================================================
        class CBulletDebugDraw : public btIDebugDraw
        {
        public:
            //----------------------------------------------------
            /// Constructor
            //----------------------------------------------------
            CBulletDebugDraw(Core::CApplication * inpApp, Rendering::MaterialPtr inpMaterial);
            //----------------------------------------------------
            /// Draw Line
            ///
            /// Draws a line with the given colour
            ///
            /// @param from.
            /// @param to.
            /// @param the colour.
            //----------------------------------------------------
            void drawLine(const btVector3& invFrom,const btVector3& invTo,const btVector3& invColour);
            //----------------------------------------------------
            /// Draw Triangle
            ///
            /// Draws a triangle.
            ///
            /// @param the first vertex.
            /// @param the second vertex.
            /// @param the third vertex.
            /// @param the colour.
            /// @param the alpha.
            //----------------------------------------------------
            void drawTriangle(const btVector3& invVertex0,const btVector3& invVertex1,const btVector3& invVertex2,const btVector3& invColour, btScalar infAlpha);
            //----------------------------------------------------
            /// Draw Contact Point
            ///
            /// This has not been implemented, this is only a
            /// method stub.
            //----------------------------------------------------
            void drawContactPoint(const btVector3& invPointOnB,const btVector3& invNormalOnB,btScalar infDistance, s32 indwLifeTime,const btVector3& invColour){}
            //----------------------------------------------------
            /// Report Error Warning
            ///
            /// This has not been implemented, this is only a
            /// method stub.
            //----------------------------------------------------
            void reportErrorWarning(const char* inpWarningString){}
            //----------------------------------------------------
            /// Draw 3d Text
            ///
            /// This has not been implemented, this is only a
            /// method stub.
            //----------------------------------------------------
            void draw3dText(const btVector3& invLocation,const char* inpTextString){}
            //----------------------------------------------------
            /// Set Debug Mode
            ///
            /// This has not been implemented, this is only a
            /// method stub.
            //----------------------------------------------------
            void setDebugMode(s32 indwDebugMode){}
            //----------------------------------------------------
            /// Set Debug Mode
            ///
            /// @returns this will always return DBG_DrawWireframe
            //----------------------------------------------------
            s32 getDebugMode() const;
            //----------------------------------------------------
            /// Update and Reset
            ///
            /// Updates and resets the debug draw.
            //----------------------------------------------------
            void UpdateAndReset();
            //----------------------------------------------------
            /// Get Entity
            ///
            /// @return the entity used to draw the debug info.
            //----------------------------------------------------
            Core::EntityPtr GetEntity();
            
        private:
            Core::CVector4 mvFrom[kudwBulletDebugDrawBufferSize];
            Core::CVector4 mvTo[kudwBulletDebugDrawBufferSize];
            Core::CVector4 mvTriangle[kudwBulletDebugDrawBufferSize];
            f32 mfR[kudwBulletDebugDrawBufferSize];
            f32 mfG[kudwBulletDebugDrawBufferSize];
            f32 mfB[kudwBulletDebugDrawBufferSize];
            u16 muwIndices[kudwBulletDebugDrawBufferSize*3];
            DebugDrawVertex mvVertices[kudwBulletDebugDrawBufferSize*3];
            Rendering::VertexElement mVertexElements[2];
            Rendering::CVertexDeclaration* mpVertexDeclaration;
            s32 mdwLines;
            Rendering::MeshDescriptor mMeshDescriptor;
            Rendering::SubMeshDescriptor mSubMeshDescriptor;
            Rendering::StaticMeshComponentPtr pMesh;
            Core::EntityPtr pEntity;
            Core::CApplication * pApp;
            Rendering::MaterialPtr pMaterial;
        };
    }
}

#endif