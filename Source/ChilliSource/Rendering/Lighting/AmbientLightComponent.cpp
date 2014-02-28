/*
 *  AmbientLightComponent.cpp
 *  moFlo
 *
 *  Created by Scott Downie on 27/1/2014.
 *  Copyright 2014 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Rendering/Lighting/AmbientLightComponent.h>

namespace ChilliSource
{
	namespace Rendering
	{
		CS_DEFINE_NAMEDTYPE(AmbientLightComponent);
        
		//----------------------------------------------------------
		/// Is A
		//----------------------------------------------------------
		bool AmbientLightComponent::IsA(ChilliSource::Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == LightComponent::InterfaceID || inInterfaceID == AmbientLightComponent::InterfaceID;
		}
        //----------------------------------------------------------
        /// Get Light Matrix
        //----------------------------------------------------------
        const Core::Matrix4x4& AmbientLightComponent::GetLightMatrix() const
        {
            return mmatLight;
        }
	}
}

