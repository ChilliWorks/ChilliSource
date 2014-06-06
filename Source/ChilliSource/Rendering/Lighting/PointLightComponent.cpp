//
//  PointLightComponent.cpp
//  Chilli Source
//  Created by Scott Downie on 31/01/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
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

#include <ChilliSource/Rendering/Lighting/PointLightComponent.h>

#include <ChilliSource/Core/Delegate/MakeDelegate.h>
#include <ChilliSource/Core/Entity/Entity.h>

#include <limits>

namespace ChilliSource
{
	namespace Rendering
	{
        CS_DEFINE_NAMEDTYPE(PointLightComponent);

		//----------------------------------------------------------
		/// Constructor
        //----------------------------------------------------------
		PointLightComponent::PointLightComponent()
            : mbMatrixCacheValid(false), mfRadius(1.0f), mfMinLightInfluence(0.05f), mfRangeOfInfluence(0.0f), mfConstantAttenuation(0.0f), mfLinearAttenuation(0.0f), mfQuadraticAttenuation(0.0f)
		{
            SetRadius(mfRadius);
            SetMinLightInfluence(mfMinLightInfluence);
		}
		//----------------------------------------------------------
		/// Is A
		//----------------------------------------------------------
		bool PointLightComponent::IsA(CSCore::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == LightComponent::InterfaceID || inInterfaceID == PointLightComponent::InterfaceID;
		}
        //----------------------------------------------------------
        /// Set Radius
        //----------------------------------------------------------
        void PointLightComponent::SetRadius(f32 infRadius)
        {
            mfRadius = infRadius;
            mbCacheValid = false;
        }
        //----------------------------------------------------------
        /// Set Min Light Influence
        //----------------------------------------------------------
        void PointLightComponent::SetMinLightInfluence(f32 infMinLightInfluence)
        {
            mfMinLightInfluence = infMinLightInfluence;
            mbCacheValid = false;
        }
        //----------------------------------------------------------
        /// Get Radius
        //----------------------------------------------------------
        f32 PointLightComponent::GetRadius() const
        {
            return mfRadius;
        }
        //----------------------------------------------------------
        /// Get Min Light Influence
        //----------------------------------------------------------
        f32 PointLightComponent::GetMinLightInfluence() const
        {
            return mfMinLightInfluence;
        }
        //----------------------------------------------------------
        /// Get Constant Attenuation
        //----------------------------------------------------------
        f32 PointLightComponent::GetConstantAttenuation() const
        {
            return mfConstantAttenuation;
        }
        //----------------------------------------------------------
        /// Get Linear Attenuation
        //----------------------------------------------------------
        f32 PointLightComponent::GetLinearAttenuation() const
        {
            return mfLinearAttenuation;
        }
        //----------------------------------------------------------
        /// Get Quadratic Attenuation
        //----------------------------------------------------------
        f32 PointLightComponent::GetQuadraticAttenuation() const
        {
            return mfQuadraticAttenuation;
        }
        //----------------------------------------------------------
        /// Get Range Of Influence
        //----------------------------------------------------------
        f32 PointLightComponent::GetRangeOfInfluence() const
        {
            return mfRangeOfInfluence;
        }
        //----------------------------------------------------------
        /// Get Light Matrix
        //----------------------------------------------------------
        const Core::Matrix4& PointLightComponent::GetLightMatrix() const
        {
            //The point light matrix is simply a light view matrix
            //as the projection is done in the shader
            if(mbMatrixCacheValid == false && GetEntity() != nullptr)
            {
                mmatLight = Core::Matrix4::Inverse(GetEntity()->GetTransform().GetWorldTransform());
                mbMatrixCacheValid = true;
            }
            
            return mmatLight;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        void PointLightComponent::OnAddedToScene()
        {
            m_transformChangedConnection = GetEntity()->GetTransform().GetTransformChangedEvent().OpenConnection(Core::MakeDelegate(this, &PointLightComponent::OnEntityTransformChanged));
        }
        //----------------------------------------------------
        //----------------------------------------------------
        void PointLightComponent::OnRemovedFromScene()
        {
            m_transformChangedConnection = nullptr;
        }
        //----------------------------------------------------
        /// On Entity Transform Changed
        //----------------------------------------------------
        void PointLightComponent::OnEntityTransformChanged()
        {
            mbMatrixCacheValid = false;
            mbCacheValid = false;
        }
        //----------------------------------------------------
        /// Calculate Lighting Values
        //----------------------------------------------------
        void PointLightComponent::CalculateLightingValues()
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

