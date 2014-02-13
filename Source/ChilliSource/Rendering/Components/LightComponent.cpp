/*
 *  LightComponent.cpp
 *  moFloTest
 *
 *  Created by Scott Downie on 08/12/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#include <moFlo/Rendering/Components/LightComponent.h>

#include <moFlo/Core/Entity.h>

#include <limits>

namespace moFlo
{
	namespace Rendering
	{
        DEFINE_NAMED_INTERFACE(ILightComponent);

		//---------------------------------------------
		/// Constructor
		//---------------------------------------------
		ILightComponent::ILightComponent()
        : mfIntensity(1.0f)
        , mbCacheValid(false)
		{
			
		}
        //----------------------------------------------------------
        /// Set Colour
        //----------------------------------------------------------
        void ILightComponent::SetColour(const Core::CColour &inColour)
        {
            mColour = inColour;
            
            mbCacheValid = false;
        }
        //----------------------------------------------------------
        /// Get Colour
        //----------------------------------------------------------
        const Core::CColour& ILightComponent::GetColour() const
        {
            mIntenseColour = mColour * mfIntensity;
            return mIntenseColour;
        }
        //----------------------------------------------------------
        /// Get World Position
        //----------------------------------------------------------
        const Core::CVector3& ILightComponent::GetWorldPosition() const
        {
            if(GetEntityOwner() != NULL)
            {
                return GetEntityOwner()->Transform().GetWorldPosition();
            }
            
            return Core::CVector3::ZERO;
        }
        //----------------------------------------------------------
        /// Set Intensity
        //----------------------------------------------------------
        void ILightComponent::SetIntensity(f32 infIntensity)
        {
            mfIntensity = infIntensity;
            
            mbCacheValid = false;
        }
        //----------------------------------------------------------
        /// Get Intensity
        //----------------------------------------------------------
        f32 ILightComponent::GetIntensity() const
        {
            return mfIntensity;
        }
        //----------------------------------------------------------
        /// Is Cache Valid
        //----------------------------------------------------------
        bool ILightComponent::IsCacheValid() const
        {
            return mbCacheValid;
        }
        //----------------------------------------------------------
        /// Set Cache Valid
        //----------------------------------------------------------
        void ILightComponent::SetCacheValid()
        {
            mbCacheValid = true;
        }
	}
}

