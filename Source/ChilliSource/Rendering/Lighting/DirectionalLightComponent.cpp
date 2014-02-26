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
		DEFINE_NAMED_INTERFACE(DirectionalLightComponent);
        
        //----------------------------------------------------------
        /// Constructor
        //----------------------------------------------------------
        DirectionalLightComponent::DirectionalLightComponent(const TexturePtr& inpShadowMapTarget, const TexturePtr& inpShadowMapDebugTarget)
        : mpShadowMap(inpShadowMapTarget)
        , mpShadowMapDebug(inpShadowMapDebugTarget)
        , mfShadowTolerance(0.0f)
        , mbMatrixCacheValid(false)
		{
		}
		//----------------------------------------------------------
		/// Is A
		//----------------------------------------------------------
		bool DirectionalLightComponent::IsA(ChilliSource::Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == LightComponent::InterfaceID || inInterfaceID == DirectionalLightComponent::InterfaceID;
		}
        //----------------------------------------------------------
        /// Set Shadow Volume
        //----------------------------------------------------------
        void DirectionalLightComponent::SetShadowVolume(f32 infWidth, f32 infHeight, f32 infNear, f32 infFar)
        {
            mmatProj = Core::CMatrix4x4::CreateOrthoMatrix(infWidth, infHeight, infNear, infFar);
            
            mbCacheValid = false;
        }
        //----------------------------------------------------------
        /// Get Direction
        //----------------------------------------------------------
        const Core::CVector3& DirectionalLightComponent::GetDirection() const
        {
            if(GetEntityOwner() != nullptr)
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
        const Core::CMatrix4x4& DirectionalLightComponent::GetLightMatrix() const
        {
            //The matrix is a view projection
            if(mbMatrixCacheValid == false && GetEntityOwner() != nullptr)
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
        f32 DirectionalLightComponent::GetShadowTolerance() const
        {
            return mfShadowTolerance;
        }
        //----------------------------------------------------------
        /// Set Shadow Tolerance
        //----------------------------------------------------------
        void DirectionalLightComponent::SetShadowTolerance(f32 infTolerance)
        {
            mfShadowTolerance = infTolerance;
            
            mbCacheValid = false;
        }
        //----------------------------------------------------------
        /// Get Shadow Map Ptr
        //----------------------------------------------------------
        const TexturePtr& DirectionalLightComponent::GetShadowMapPtr() const
        {
            return mpShadowMap;
        }
        //----------------------------------------------------------
        /// Get Shadow Map Debug Ptr
        //----------------------------------------------------------
        const TexturePtr& DirectionalLightComponent::GetShadowMapDebugPtr() const
        {
            return mpShadowMapDebug;
        }
        //----------------------------------------------------
        /// On Attached To Entity
        //----------------------------------------------------
        void DirectionalLightComponent::OnAttachedToEntity()
        {
            GetEntityOwner()->Transform().GetTransformChangedEvent() += fastdelegate::MakeDelegate(this, &DirectionalLightComponent::OnEntityTransformChanged);
        }
        //----------------------------------------------------
        /// On Detached From Entity
        //----------------------------------------------------
        void DirectionalLightComponent::OnDetachedFromEntity()
        {
            GetEntityOwner()->Transform().GetTransformChangedEvent() -= fastdelegate::MakeDelegate(this, &DirectionalLightComponent::OnEntityTransformChanged);
        }
        //----------------------------------------------------
        /// On Entity Transform Changed
        //----------------------------------------------------
        void DirectionalLightComponent::OnEntityTransformChanged()
        {
            mbMatrixCacheValid = false;
            mbCacheValid = false;
        }
	}
}

