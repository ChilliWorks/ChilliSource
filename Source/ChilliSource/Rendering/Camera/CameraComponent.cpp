//
//  CameraComponent.cpp
//  Chilli Source
//  Created by Scott Downie on 12/10/2010.
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

#include <ChilliSource/Rendering/Camera/CameraComponent.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/Screen.h>
#include <ChilliSource/Core/Delegate/MakeDelegate.h>
#include <ChilliSource/Core/Entity/Entity.h>
#include <ChilliSource/Core/Event/IConnectableEvent.h>

namespace ChilliSource
{
    CS_DEFINE_NAMEDTYPE(CameraComponent);
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    CameraComponent::CameraComponent(f32 in_nearClip, f32 in_farClip)
    : m_nearClip(in_nearClip), m_farClip(in_farClip), m_screen(Application::Get()->GetScreen())
    {
        
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void CameraComponent::SetNearClipping(f32 in_near)
    {
        m_nearClip = in_near;

        m_isProjCacheValid = false;
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void CameraComponent::SetFarClipping(f32 in_far)
    {
        m_farClip = in_far;
        
        m_isProjCacheValid = false;
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    Ray CameraComponent::Unproject(const Vector2 &invScreenPos)
    {
        Matrix4 matProj = Matrix4::Inverse((GetView() * GetProjection()));
        
        Vector2 vScreenSize = m_screen->GetResolution();
        //Normalise the screen space co-ordinates into clip space
        f32 nx = ((2.0f * (invScreenPos.x/vScreenSize.x)) - 1.0f);
        f32 ny = ((2.0f * (invScreenPos.y/vScreenSize.y)) - 1.0f);
        
        Vector4 vNear(nx, ny, -1.0f, 1.0f);
        Vector4 vFar(nx,ny, 1.0f, 1.0f);
        vNear = vNear * matProj;
        vFar = vFar * matProj;
        
        Ray cRay;
        
        vNear /= vNear.w;
        cRay.vOrigin = Vector3(vNear.x, vNear.y, vNear.z);

        vFar /= vFar.w;
        cRay.vDirection = Vector3(vFar.x, vFar.y, vFar.z) - cRay.vOrigin;
        
        cRay.fLength = cRay.vDirection.Length();
        cRay.vDirection /= cRay.fLength;
        
        return cRay;
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    Vector2 CameraComponent::Project(const Vector3 &invWorldPos)
    {
        //Convert the world space position to clip space
        Matrix4 matToClip = (GetView() * GetProjection());
        Vector4 vScreenPos = Vector4(invWorldPos, 1.0f) * matToClip;
        
        Vector2 vScreenSize = m_screen->GetResolution();
        
        // Normalize co-ordinates
        vScreenPos.x = vScreenPos.x / vScreenPos.w;
        vScreenPos.y = vScreenPos.y / vScreenPos.w;

        //Convert from clip space to screen space
        vScreenPos.x = (vScreenSize.x * 0.5f)* vScreenPos.x + vScreenSize.x * 0.5f;
        vScreenPos.y = (vScreenSize.y * 0.5f)* vScreenPos.y + vScreenSize.y * 0.5f;

        //Return 2D screen space co-ordinates
        return Vector2(vScreenPos.x, vScreenPos.y);
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    const Matrix4& CameraComponent::GetProjection() 
    {
        if(m_isProjCacheValid == false)
        {
            m_projMat = CalculateProjectionMatrix();
            m_isProjCacheValid = true;
        }

        return m_projMat;
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    const Matrix4& CameraComponent::GetView()
    {
        if(GetEntity())
        {
            m_viewMat = Matrix4::Inverse(GetEntity()->GetTransform().GetWorldTransform());
        }

        return m_viewMat;
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    const Frustum& CameraComponent::GetFrustum()
    {
        if(m_isFrustumCacheValid == false)
        {
            UpdateFrustum();
            m_isFrustumCacheValid = true;
        }
        
        return m_frustum;
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    Matrix4 CameraComponent::Billboard(const Matrix4& in_toBillboard)
    {
        const Matrix4& viewMat = GetView();
        Matrix4 result = in_toBillboard * viewMat;
        
        result.m[12] = in_toBillboard.m[12];
        result.m[13] = in_toBillboard.m[13];
        result.m[14] = in_toBillboard.m[14];
        
        return result;
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    const RendererSortPredicateSPtr& CameraComponent::GetOpaqueSortPredicate() const
    {
        return m_opaqueSortPredicate;
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    const RendererSortPredicateSPtr& CameraComponent::GetTransparentSortPredicate() const
    {
        return m_transparentSortPredicate;
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void CameraComponent::SetOpaqueSortPredicate(const RendererSortPredicateSPtr& in_predicate)
    {
        m_opaqueSortPredicate = in_predicate;
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void CameraComponent::SetTransparentSortPredicate(const RendererSortPredicateSPtr& in_predicate)
    {
        m_transparentSortPredicate = in_predicate;
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    const ICullingPredicateSPtr& CameraComponent::GetCullingPredicate() const
    {
        return m_cullPredicate;
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void CameraComponent::SetCullingPredicate(const ICullingPredicateSPtr& in_predicate)
    {
        m_cullPredicate = in_predicate;
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void CameraComponent::OnAddedToEntity()
    {
        m_transformChangedConnection = GetEntity()->GetTransform().GetTransformChangedEvent().OpenConnection([=]()
        {
            m_isFrustumCacheValid = false;
        });
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void CameraComponent::OnAddedToScene()
    {
        m_resolutionChangedConnection = Application::Get()->GetScreen()->GetResolutionChangedEvent().OpenConnection([=](const Vector2& in_resolution)
        {
            m_isFrustumCacheValid = false;
        });
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void CameraComponent::OnRemovedFromScene()
    {
        m_resolutionChangedConnection.reset();
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void CameraComponent::OnRemovedFromEntity()
    {
        m_transformChangedConnection.reset();
    }
}

