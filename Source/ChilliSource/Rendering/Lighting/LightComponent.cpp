/*
 *  LightComponent.cpp
 *  moFloTest
 *
 *  Created by Scott Downie on 08/12/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Rendering/Lighting/LightComponent.h>

#include <ChilliSource/Core/Entity/Entity.h>

#include <limits>

namespace ChilliSource
{
	namespace Rendering
	{
        CS_DEFINE_NAMEDTYPE(LightComponent);

		//---------------------------------------------
		/// Constructor
		//---------------------------------------------
		LightComponent::LightComponent()
        : mfIntensity(1.0f)
        , mbCacheValid(false)
		{
			
		}
        //----------------------------------------------------------
        /// Set Colour
        //----------------------------------------------------------
        void LightComponent::SetColour(const Core::Colour &inColour)
        {
            mColour = inColour;
            
            mbCacheValid = false;
        }
        //----------------------------------------------------------
        /// Get Colour
        //----------------------------------------------------------
        Core::Colour LightComponent::GetColour() const
        {
            return mColour * mfIntensity;
        }
        //----------------------------------------------------------
        /// Get World Position
        //----------------------------------------------------------
        const Core::Vector3Old& LightComponent::GetWorldPosition() const
        {
            if(GetEntity() != nullptr)
            {
                return GetEntity()->GetTransform().GetWorldPosition();
            }
            
            return Core::Vector3Old::ZERO;
        }
        //----------------------------------------------------------
        /// Set Intensity
        //----------------------------------------------------------
        void LightComponent::SetIntensity(f32 infIntensity)
        {
            mfIntensity = infIntensity;
            
            mbCacheValid = false;
        }
        //----------------------------------------------------------
        /// Get Intensity
        //----------------------------------------------------------
        f32 LightComponent::GetIntensity() const
        {
            return mfIntensity;
        }
        //----------------------------------------------------------
        /// Is Cache Valid
        //----------------------------------------------------------
        bool LightComponent::IsCacheValid() const
        {
            return mbCacheValid;
        }
        //----------------------------------------------------------
        /// Set Cache Valid
        //----------------------------------------------------------
        void LightComponent::SetCacheValid()
        {
            mbCacheValid = true;
        }
	}
}

