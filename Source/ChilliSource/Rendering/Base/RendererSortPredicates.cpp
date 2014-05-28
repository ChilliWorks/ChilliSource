//
//  RendererSortPredicates.cpp
//  Chilli Source
//  Created by Stuart McGaw on 26/04/2011.
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

#include <ChilliSource/Rendering/Base/RendererSortPredicates.h>
#include <ChilliSource/Rendering/Base/Renderer.h>
#include <ChilliSource/Rendering/Model/StaticMeshComponent.h>

#include <ChilliSource/Core/Entity/Entity.h>
#include <ChilliSource/Core/Entity/Transform.h>

using namespace ChilliSource::Core;

namespace ChilliSource
{
	namespace Rendering
    {
		void BackToFrontSortPredicate::PrepareForSort(std::vector<RenderComponent*> * inpRenderable)
        {
			mCameraViewProj = Renderer::matViewProjCache;
            
            Matrix4 matViewTrans;
            Matrix4 matWorld;
            
            for(u32 i = 0; i < inpRenderable->size(); ++i)
            {
				Core::Matrix4 matLocalTrans = Core::Matrix4::CreateTranslation((*inpRenderable)[i]->GetAABB().GetOrigin() - (*inpRenderable)[i]->GetEntity()->GetTransform().GetWorldPosition());
                matWorld = (*inpRenderable)[i]->GetTransformationMatrix() * matLocalTrans;
				matViewTrans = matWorld * mCameraViewProj;
                (*inpRenderable)[i]->SetSortValue(matViewTrans.GetTranslation().z);
            }
		}
        
		bool BackToFrontSortPredicate::SortItem(const RenderComponent* p1, const RenderComponent* p2) const
        {
			return p1->GetSortValue() > p2->GetSortValue();
		}
        
        bool MaterialSortPredicate::SortItem(const RenderComponent* p1, const RenderComponent* p2) const
        {
            const Material* pM1 = p1->GetMaterial().get();
            const Material* pM2 = p2->GetMaterial().get();
            if(pM1 == pM2)
            {
                bool bp1Mesh = p1->IsA(StaticMeshComponent::InterfaceID);
                bool bp2Mesh = p2->IsA(StaticMeshComponent::InterfaceID);
                if(bp1Mesh && bp2Mesh)
                {
                    return ((StaticMeshComponent*)p1)->GetMesh().get() < ((StaticMeshComponent*)p2)->GetMesh().get();
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
