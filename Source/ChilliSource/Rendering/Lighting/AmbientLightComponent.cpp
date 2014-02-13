/*
 *  AmbientLightComponent.cpp
 *  moFlo
 *
 *  Created by Scott Downie on 27/1/2014.
 *  Copyright 2014 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Rendering/Components/AmbientLightComponent.h>

namespace moFlo
{
	namespace Rendering
	{
		DEFINE_NAMED_INTERFACE(CAmbientLightComponent);
        
		//----------------------------------------------------------
		/// Is A
		//----------------------------------------------------------
		bool CAmbientLightComponent::IsA(moFlo::Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == ILightComponent::InterfaceID || inInterfaceID == CAmbientLightComponent::InterfaceID;
		}
        //----------------------------------------------------------
        /// Get Light Matrix
        //----------------------------------------------------------
        const Core::CMatrix4x4& CAmbientLightComponent::GetLightMatrix() const
        {
            return mmatLight;
        }
	}
}

