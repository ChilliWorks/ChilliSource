/*
 *  AmbientLightComponent.h
 *  moFlo
 *
 *  Created by Scott Downie on 28/1/2014.
 *  Copyright 2014 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLOW_RENDERING_AMBIENTLIGHTCOMPONENT_H_
#define _MOFLOW_RENDERING_AMBIENTLIGHTCOMPONENT_H_

#include <ChilliSource/Rendering/Lighting/LightComponent.h>

namespace ChilliSource
{
	namespace Rendering
	{
		class CAmbientLightComponent : public ILightComponent
		{
		public:
			DECLARE_NAMED_INTERFACE(CAmbientLightComponent);
			
			//----------------------------------------------------------
			/// Is A
			///
			/// Returns if it is of the type given
			/// @param Comparison Type
			/// @return Whether the class matches the comparison type
			//----------------------------------------------------------
			bool IsA(Core::InterfaceIDType inInterfaceID) const;
            //----------------------------------------------------------
            /// Get Light Matrix
            ///
            /// @return Light transform
            //----------------------------------------------------------
            const Core::CMatrix4x4& GetLightMatrix() const;
		};
    }
}

#endif