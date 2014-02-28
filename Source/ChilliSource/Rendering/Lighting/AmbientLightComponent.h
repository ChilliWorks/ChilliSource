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

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Rendering/Lighting/LightComponent.h>

namespace ChilliSource
{
	namespace Rendering
	{
		class AmbientLightComponent : public LightComponent
		{
		public:
			CS_DECLARE_NAMEDTYPE(AmbientLightComponent);
			
			//----------------------------------------------------------
			/// Is A
			///
			/// Returns if it is of the type given
			/// @param Comparison Type
			/// @return Whether the class matches the comparison type
			//----------------------------------------------------------
			bool IsA(Core::InterfaceIDType inInterfaceID) const override;
            //----------------------------------------------------------
            /// Get Light Matrix
            ///
            /// @return Light transform
            //----------------------------------------------------------
            const Core::Matrix4x4& GetLightMatrix() const override;
		};
    }
}

#endif