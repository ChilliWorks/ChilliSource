/*
 *  PointLightComponent.cpp
 *  moFlo
 *
 *  Created by Scott Downie on 31/1/2014.
 *  Copyright 2014 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Rendering/Lighting/PointLightComponent.h>

#include <ChilliSource/Core/Entity/Entity.h>

#include <limits.h>

namespace ChilliSource
{
	namespace Rendering
	{
        DEFINE_NAMED_INTERFACE(CPointLightComponent);

		//----------------------------------------------------------
		/// Constructor
        //----------------------------------------------------------
		CPointLightComponent::CPointLightComponent()
            : mbMatrixCacheValid(false), mfRadius(1.0f), mfMinLightInfluence(0.05f), mfRangeOfInfluence(0.0f), mfConstantAttenuation(0.0f), mfLinearAttenuation(0.0f), mfQuadraticAttenuation(0.0f)
		{
            SetRadius(mfRadius);
            SetMinLightInfluence(mfMinLightInfluence);
		}
		//----------------------------------------------------------
		/// Is A
		//----------------------------------------------------------
		bool CPointLightComponent::IsA(ChilliSource::Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == ILightComponent::InterfaceID || inInterfaceID == CPointLightComponent::InterfaceID;
		}
        //----------------------------------------------------------
        /// Set Radius
        //----------------------------------------------------------
        void CPointLightComponent::SetRadius(f32 infRadius)
        {
            mfRadius = infRadius;
            mbCacheValid = false;
        }
        //----------------------------------------------------------
        /// Set Min Light Influence
        //----------------------------------------------------------
        void CPointLightComponent::SetMinLightInfluence(f32 infMinLightInfluence)
        {
            mfMinLightInfluence = infMinLightInfluence;
            mbCacheValid = false;
        }
        //----------------------------------------------------------
        /// Get Radius
        //----------------------------------------------------------
        f32 CPointLightComponent::GetRadius() const
        {
            return mfRadius;
        }
        //----------------------------------------------------------
        /// Get Min Light Influence
        //----------------------------------------------------------
        f32 CPointLightComponent::GetMinLightInfluence() const
        {
            return mfMinLightInfluence;
        }
        //----------------------------------------------------------
        /// Get Constant Attenuation
        //----------------------------------------------------------
        f32 CPointLightComponent::GetConstantAttenuation() const
        {
            return mfConstantAttenuation;
        }
        //----------------------------------------------------------
        /// Get Linear Attenuation
        //----------------------------------------------------------
        f32 CPointLightComponent::GetLinearAttenuation() const
        {
            return mfLinearAttenuation;
        }
        //----------------------------------------------------------
        /// Get Quadratic Attenuation
        //----------------------------------------------------------
        f32 CPointLightComponent::GetQuadraticAttenuation() const
        {
            return mfQuadraticAttenuation;
        }
        //----------------------------------------------------------
        /// Get Range Of Influence
        //----------------------------------------------------------
        f32 CPointLightComponent::GetRangeOfInfluence() const
        {
            return mfRangeOfInfluence;
        }
        //----------------------------------------------------------
        /// Get Light Matrix
        //----------------------------------------------------------
        const Core::CMatrix4x4& CPointLightComponent::GetLightMatrix() const
        {
            //The point light matrix is simply a light view matrix
            //as the projection is done in the shader
            if(mbMatrixCacheValid == false && GetEntityOwner() != nullptr)
            {
                mmatLight = GetEntityOwner()->Transform().GetWorldTransform().Inverse();
                mbMatrixCacheValid = true;
            }
            
            return mmatLight;
        }
        //----------------------------------------------------
        /// On Attached To Entity
        //----------------------------------------------------
        void CPointLightComponent::OnAttachedToEntity()
        {
            GetEntityOwner()->Transform().GetTransformChangedEvent() += fastdelegate::MakeDelegate(this, &CPointLightComponent::OnEntityTransformChanged);
        }
        //----------------------------------------------------
        /// On Detached From Entity
        //----------------------------------------------------
        void CPointLightComponent::OnDetachedFromEntity()
        {
            GetEntityOwner()->Transform().GetTransformChangedEvent() -= fastdelegate::MakeDelegate(this, &CPointLightComponent::OnEntityTransformChanged);
        }
        //----------------------------------------------------
        /// On Entity Transform Changed
        //----------------------------------------------------
        void CPointLightComponent::OnEntityTransformChanged()
        {
            mbMatrixCacheValid = false;
            mbCacheValid = false;
        }
        //----------------------------------------------------
        /// Calculate Lighting Values
        //----------------------------------------------------
        void CPointLightComponent::CalculateLightingValues()
        {
            if (mfRadius > 0.0f)
            {
                mfConstantAttenuation = 1;
                mfLinearAttenuation = 2 / mfRadius;
                mfQuadraticAttenuation = 1 / (mfRadius * mfRadius);
                
                if (mfMinLightInfluence > 0.0f)
                {
                    mfRangeOfInfluence = mfRadius * (sqrt(GetIntensity() / mfMinLightInfluence) + 1);
                }
                else
                {
                    //if the min light influence is 0 or below this indicates that the range of influence is infinite, so set to max.
                    mfRangeOfInfluence = std::numeric_limits<f32>::max();
                }
            }
            else
            {
                //if the light radius is 0 or below then the attenuation no longer makes sense, so set all to 0. Ensure a divide by 0 wont occur in the shader
                //by leaving the constant attenuation as 1.
                mfConstantAttenuation = 1.0f;
                mfLinearAttenuation = 0.0f;
                mfQuadraticAttenuation = 0.0f;
                mfRangeOfInfluence = 0.0f;
            }
        }
	}
}

