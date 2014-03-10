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
		CS_DEFINE_NAMEDTYPE(DirectionalLightComponent);
        
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
        Core::Vector3 DirectionalLightComponent::GetDirection() const
        {
            if(GetEntity() != nullptr)
            {
                return GetEntity()->GetTransform().GetWorldOrientation() * Core::Vector3::Z_UNIT_NEGATIVE;
            }
            else
            {
                return Core::Vector3::Z_UNIT_NEGATIVE;
            }
        }
        //----------------------------------------------------------
        /// Get Light Matrix
        //----------------------------------------------------------
        const Core::Matrix4x4& DirectionalLightComponent::GetLightMatrix() const
        {
            //The matrix is a view projection
            if(mbMatrixCacheValid == false && GetEntity() != nullptr)
            {
                Core::Matrix4x4 matView = GetEntity()->GetTransform().GetWorldTransform().Inverse();
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
        //----------------------------------------------------
        void DirectionalLightComponent::OnAddedToScene()
        {
            m_transformChangedConnection = GetEntity()->GetTransform().GetTransformChangedEvent().OpenConnection(Core::MakeDelegate(this, &DirectionalLightComponent::OnEntityTransformChanged));
        }
        //----------------------------------------------------
        //----------------------------------------------------
        void DirectionalLightComponent::OnRemovedFromScene()
        {
            m_transformChangedConnection = nullptr;
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

