/*
 *  DirectionalLightComponent.cpp
 *  moFlo
 *
 *  Created by Scott Downie on 31/1/2014.
 *  Copyright 2014 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Rendering/Lighting/DirectionalLightComponent.h>

#include <ChilliSource/Core/Base/MakeDelegate.h>
#include <ChilliSource/Core/Entity/Entity.h>

namespace ChilliSource
{
	namespace Rendering
	{
		DEFINE_NAMED_INTERFACE(DirectionalLightComponent);
        
        //----------------------------------------------------------
        /// Constructor
        //----------------------------------------------------------
        DirectionalLightComponent::DirectionalLightComponent(const TextureSPtr& inpShadowMapTarget, const TextureSPtr& inpShadowMapDebugTarget)
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
            mmatProj = Core::Matrix4x4::CreateOrthoMatrix(infWidth, infHeight, infNear, infFar);
            
            mbCacheValid = false;
        }
        //----------------------------------------------------------
        /// Get Direction
        //----------------------------------------------------------
        const Core::Vector3& DirectionalLightComponent::GetDirection() const
        {
            if(GetEntityOwner() != nullptr)
            {
                mvDirection = GetEntityOwner()->GetTransform().GetWorldOrientation() * Core::Vector3::Z_UNIT_NEGATIVE;
            }
            else
            {
                mvDirection = Core::Vector3::Z_UNIT_NEGATIVE;
            }
            
            return mvDirection;
        }
        //----------------------------------------------------------
        /// Get Light Matrix
        //----------------------------------------------------------
        const Core::Matrix4x4& DirectionalLightComponent::GetLightMatrix() const
        {
            //The matrix is a view projection
            if(mbMatrixCacheValid == false && GetEntityOwner() != nullptr)
            {
                Core::Matrix4x4 matView = GetEntityOwner()->GetTransform().GetWorldTransform().Inverse();
                Core::Matrix4x4::Multiply(&matView, &mmatProj, &mmatLight);
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
        const TextureSPtr& DirectionalLightComponent::GetShadowMapPtr() const
        {
            return mpShadowMap;
        }
        //----------------------------------------------------------
        /// Get Shadow Map Debug Ptr
        //----------------------------------------------------------
        const TextureSPtr& DirectionalLightComponent::GetShadowMapDebugPtr() const
        {
            return mpShadowMapDebug;
        }
        //----------------------------------------------------
        /// On Attached To Entity
        //----------------------------------------------------
        void DirectionalLightComponent::OnAttachedToEntity()
        {
            GetEntityOwner()->GetTransform().GetTransformChangedEvent().AddListener(Core::MakeDelegate(this, &DirectionalLightComponent::OnEntityTransformChanged));
        }
        //----------------------------------------------------
        /// On Detached From Entity
        //----------------------------------------------------
        void DirectionalLightComponent::OnDetachedFromEntity()
        {
            GetEntityOwner()->GetTransform().GetTransformChangedEvent().RemoveListener(Core::MakeDelegate(this, &DirectionalLightComponent::OnEntityTransformChanged));
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

