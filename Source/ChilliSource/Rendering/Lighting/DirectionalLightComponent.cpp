/*
 *  DirectionalLightComponent.cpp
 *  moFlo
 *
 *  Created by Scott Downie on 31/1/2014.
 *  Copyright 2014 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Rendering/Lighting/DirectionalLightComponent.h>

#include <ChilliSource/Core/Entity/Entity.h>

namespace ChilliSource
{
	namespace Rendering
	{
		DEFINE_NAMED_INTERFACE(CDirectionalLightComponent);
        
        //----------------------------------------------------------
        /// Constructor
        //----------------------------------------------------------
        CDirectionalLightComponent::CDirectionalLightComponent(const TexturePtr& inpShadowMapTarget, const TexturePtr& inpShadowMapDebugTarget)
        : mpShadowMap(inpShadowMapTarget)
        , mpShadowMapDebug(inpShadowMapDebugTarget)
        , mfShadowTolerance(0.0f)
        , mbMatrixCacheValid(false)
		{
		}
		//----------------------------------------------------------
		/// Is A
		//----------------------------------------------------------
		bool CDirectionalLightComponent::IsA(moFlo::Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == ILightComponent::InterfaceID || inInterfaceID == CDirectionalLightComponent::InterfaceID;
		}
        //----------------------------------------------------------
        /// Set Shadow Volume
        //----------------------------------------------------------
        void CDirectionalLightComponent::SetShadowVolume(f32 infWidth, f32 infHeight, f32 infNear, f32 infFar)
        {
            mmatProj = Core::CMatrix4x4::CreateOrthoMatrix(infWidth, infHeight, infNear, infFar);
            
            mbCacheValid = false;
        }
        //----------------------------------------------------------
        /// Get Direction
        //----------------------------------------------------------
        const Core::CVector3& CDirectionalLightComponent::GetDirection() const
        {
            if(GetEntityOwner() != NULL)
            {
                mvDirection = GetEntityOwner()->Transform().GetWorldOrientation() * Core::CVector3::Z_UNIT_NEGATIVE;
            }
            else
            {
                mvDirection = Core::CVector3::Z_UNIT_NEGATIVE;
            }
            
            return mvDirection;
        }
        //----------------------------------------------------------
        /// Get Light Matrix
        //----------------------------------------------------------
        const Core::CMatrix4x4& CDirectionalLightComponent::GetLightMatrix() const
        {
            //The matrix is a view projection
            if(mbMatrixCacheValid == false && GetEntityOwner() != NULL)
            {
                Core::CMatrix4x4 matView = GetEntityOwner()->Transform().GetWorldTransform().Inverse();
                Core::CMatrix4x4::Multiply(&matView, &mmatProj, &mmatLight);
                mbMatrixCacheValid = true;
            }
            
            return mmatLight;
        }
        //----------------------------------------------------------
        /// Get Shadow Tolerance
        //----------------------------------------------------------
        f32 CDirectionalLightComponent::GetShadowTolerance() const
        {
            return mfShadowTolerance;
        }
        //----------------------------------------------------------
        /// Set Shadow Tolerance
        //----------------------------------------------------------
        void CDirectionalLightComponent::SetShadowTolerance(f32 infTolerance)
        {
            mfShadowTolerance = infTolerance;
            
            mbCacheValid = false;
        }
        //----------------------------------------------------------
        /// Get Shadow Map Ptr
        //----------------------------------------------------------
        const TexturePtr& CDirectionalLightComponent::GetShadowMapPtr() const
        {
            return mpShadowMap;
        }
        //----------------------------------------------------------
        /// Get Shadow Map Debug Ptr
        //----------------------------------------------------------
        const TexturePtr& CDirectionalLightComponent::GetShadowMapDebugPtr() const
        {
            return mpShadowMapDebug;
        }
        //----------------------------------------------------
        /// On Attached To Entity
        //----------------------------------------------------
        void CDirectionalLightComponent::OnAttachedToEntity()
        {
            GetEntityOwner()->Transform().GetTransformChangedEvent() += fastdelegate::MakeDelegate(this, &CDirectionalLightComponent::OnEntityTransformChanged);
        }
        //----------------------------------------------------
        /// On Detached From Entity
        //----------------------------------------------------
        void CDirectionalLightComponent::OnDetachedFromEntity()
        {
            GetEntityOwner()->Transform().GetTransformChangedEvent() -= fastdelegate::MakeDelegate(this, &CDirectionalLightComponent::OnEntityTransformChanged);
        }
        //----------------------------------------------------
        /// On Entity Transform Changed
        //----------------------------------------------------
        void CDirectionalLightComponent::OnEntityTransformChanged()
        {
            mbMatrixCacheValid = false;
            mbCacheValid = false;
        }
	}
}

