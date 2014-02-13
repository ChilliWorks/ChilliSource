//
//  BulletDebugDraw.cpp
//  moFlow
//
//  Created by Robert Henning on 28/11/2011.
//  Copyright (c) 2011 Tag Games. All rights reserved.
//

#include <iostream>
#include <ChilliSource/Physics3D/BulletPhysics/BulletDebugDraw.h>
#include <ChilliSource/Core/ResourceManagerDispenser.h>
#include <ChilliSource/Core/Application.h>
#include <ChilliSource/Rendering/RenderComponentFactory.h>
#include <ChilliSource/Rendering/RenderSystem.h>

namespace moFlo
{
    namespace Physics3D
    {
        //----------------------------------------------------
        /// Constructor
        //----------------------------------------------------
        CBulletDebugDraw::CBulletDebugDraw(Core::CApplication * inpApp, Rendering::MaterialPtr inpMaterial)
        {
            pApp=inpApp;
            pMaterial=inpMaterial;
            for(s32 i=0;i<kudwBulletDebugDrawBufferSize;i++)
            {
                muwIndices[(i*2)]=(i*2);
                muwIndices[(i*2)+1]=(i*2)+1;
            }
            
            mVertexElements[0].eSemantic=Rendering::POSITION;
            mVertexElements[0].eType=Rendering::FLOAT3;
            
            mVertexElements[1].eSemantic=Rendering::COLOUR;
            mVertexElements[1].eType=Rendering::FLOAT4;
            
            mMeshDescriptor.mVertexDeclaration=Rendering::CVertexDeclaration(2,mVertexElements);
            mMeshDescriptor.mudwIndexSize=2;
            mSubMeshDescriptor.mpVertexData=(u8*)mvVertices;
            mSubMeshDescriptor.mpIndexData=(u8*)muwIndices;    
            mSubMeshDescriptor.ePrimitiveType=Rendering::LINE;
            
            mMeshDescriptor.mMeshes.push_back(mSubMeshDescriptor);
            
            mdwLines=0;
        }
        //----------------------------------------------------
        /// Draw Line
        //----------------------------------------------------
        void CBulletDebugDraw::drawLine(const btVector3& invFrom,const btVector3& invTo,const btVector3& invColour)
        {
            if(mdwLines<kudwBulletDebugDrawBufferSize)
            {
                mvFrom[mdwLines].x=invFrom.x();
                mvFrom[mdwLines].y=invFrom.y();
                mvFrom[mdwLines].z=invFrom.z(); 
                mvTo[mdwLines].x=invTo.x();
                mvTo[mdwLines].y=invTo.y();
                mvTo[mdwLines].z=invTo.z(); 
                mfR[mdwLines]=invColour.x();
                mfG[mdwLines]=invColour.y();       
                mfB[mdwLines]=invColour.z(); 
                mdwLines++;
            }
        }
        //----------------------------------------------------
        /// Set Debug Mode
        //----------------------------------------------------
        void CBulletDebugDraw::drawTriangle(const btVector3& invVertex0,const btVector3& invVertex1,const btVector3& invVertex2,const btVector3& invColour, btScalar invAlpha)
        {
            if(mdwLines<kudwBulletDebugDrawBufferSize-2)
            {
                mvFrom[mdwLines].x=invVertex0.x();
                mvFrom[mdwLines].y=invVertex0.y();
                mvFrom[mdwLines].z=invVertex0.z(); 
                mvTo[mdwLines].x=invVertex1.x();
                mvTo[mdwLines].y=invVertex1.y();
                mvTo[mdwLines].z=invVertex1.z();       
                mdwLines++;
            }
        }
        //----------------------------------------------------
        /// Set Debug Mode
        //----------------------------------------------------
        s32 CBulletDebugDraw::getDebugMode() const
        {
            return DBG_DrawWireframe;
        }
        //----------------------------------------------------
        /// Set Debug Mode
        //----------------------------------------------------
        void CBulletDebugDraw::UpdateAndReset()
        {
            if(!pEntity)
            {
                pEntity=Core::CreateEntity();
            }
            else
            {
                pEntity-=pMesh;
                pMesh.reset();
            }
            
            for(s32 i=0;i<mdwLines;i++)
            {
                mvVertices[(i*2)].x = mvFrom[i].x;
                mvVertices[(i*2)].y = mvFrom[i].y;
                mvVertices[(i*2)].z = -mvFrom[i].z;
                mvVertices[(i*2)].r = mfR[i];
                mvVertices[(i*2)].g = mfG[i];
                mvVertices[(i*2)].b = mfB[i];
                mvVertices[(i*2)].a = 1.0;
                mvVertices[(i*2)+1].x = mvTo[i].x;
                mvVertices[(i*2)+1].y = mvTo[i].y;
                mvVertices[(i*2)+1].z = -mvTo[i].z;
                mvVertices[(i*2)+1].r = mfR[i];
                mvVertices[(i*2)+1].g = mfG[i];
                mvVertices[(i*2)+1].b = mfB[i];
                mvVertices[(i*2)+1].a = 1.0;
            }
            
            mMeshDescriptor.mMeshes[0].mudwNumVertices=mdwLines*2;
            mMeshDescriptor.mMeshes[0].mudwNumIndices=mdwLines*2;    
            Rendering::CMeshManager* pModelMgr = (Rendering::CMeshManager*)Core::CResourceManagerDispenser::GetSingletonPtr()->GetResourceManagerForType(Rendering::CMesh::InterfaceID);
            Rendering::MeshPtr pModel = pModelMgr->CreateManualMesh(mMeshDescriptor);
            Rendering::CRenderComponentFactory* pRenderFactory = (moFlo::Rendering::CRenderComponentFactory*) pApp->GetRenderSystemPtr()->GetComponentFactoryPtr(0);
            pMesh = pRenderFactory->CreateStaticMeshComponent(pModel,pMaterial);
            pEntity+=pMesh;
            
            mdwLines=0;
        }
        //----------------------------------------------------
        /// Set Debug Mode
        //----------------------------------------------------
        Core::EntityPtr CBulletDebugDraw::GetEntity()
        {
            if(!pEntity)
            {
                pEntity=Core::CreateEntity();
                pEntity->SetName("DebugDraw");
            } 
            return pEntity;
        }
    }
}

