/*
 *  RendererSortPredicates.cpp
 *  MoFlow
 *
 *  Created by Stuart McGaw on 26/04/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Rendering/RendererSortPredicates.h>
#include <ChilliSource/Rendering/Renderer.h>
#include <ChilliSource/Rendering/Components/StaticMeshComponent.h>

#include <ChilliSource/Core/Entity.h>
#include <ChilliSource/Core/Transform.h>

using namespace moFlo::Core;

namespace moFlo
{
	namespace Rendering
    {
		void CBackToFrontSortPredicate::PrepareForSort(DYNAMIC_ARRAY<IRenderComponent*> * inpRenderable)
        {
			mCameraViewProj = CRenderer::matViewProjCache;
            
            CMatrix4x4 matViewTrans;
            CMatrix4x4 matWorld;
            
            for(u32 i = 0; i < inpRenderable->size(); ++i)
            {
                Core::CMatrix4x4 matLocalTrans;
                matLocalTrans.SetTranslation((*inpRenderable)[i]->GetAABB().GetOrigin() - (*inpRenderable)[i]->GetEntityOwner()->Transform().GetWorldPosition());
                
                Core::CMatrix4x4::Multiply(&((*inpRenderable)[i]->GetTransformationMatrix()), &matLocalTrans, &matWorld);
                
                Core::CMatrix4x4::Multiply(&matWorld, &mCameraViewProj, &matViewTrans);
                (*inpRenderable)[i]->SetSortValue(matViewTrans.GetTranslation().z);
            }
		}
        
		bool CBackToFrontSortPredicate::SortItem(IRenderComponent* p1, IRenderComponent* p2)
        {
			return p1->GetSortValue() > p2->GetSortValue();
		}
        
        bool CMaterialSortPredicate::SortItem(IRenderComponent* p1, IRenderComponent* p2)
        {
            moRendering::CMaterial * pM1= p1->GetMaterial().get();
            moRendering::CMaterial * pM2= p2->GetMaterial().get();
            if(pM1 == pM2)
            {
                bool bp1Mesh = p1->IsA(CStaticMeshComponent::InterfaceID);
                bool bp2Mesh = p2->IsA(CStaticMeshComponent::InterfaceID);
                if(bp1Mesh && bp2Mesh)
                {
                    return ((CStaticMeshComponent*)p1)->GetMesh().get() < ((CStaticMeshComponent*)p2)->GetMesh().get();
                }
                else
                {
                    return bp1Mesh;
                }
            }
            else
            {
                return p1->GetMaterial().get() < p2->GetMaterial().get();
            }
		}
	}
}
