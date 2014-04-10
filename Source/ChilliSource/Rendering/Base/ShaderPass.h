//
//  ShaderPass.h
//  Chilli Source
//
//  Created by Scott Downie on 09/04/2014.
//  Copyright 2014 Tag Games. All rights reserved.
//


#ifndef _CHILLISOURCE_RENDERING_BASE_SHADERPASS_H_
#define _CHILLISOURCE_RENDERING_BASE_SHADERPASS_H_

namespace ChilliSource
{
	namespace Rendering
	{
        //---------------------------------------------
        /// The renderer adds to the render buffer
        /// in a series of lighting passes.
        /// * The ambient pass
        /// * The directional pass
        /// * The point light pass
        ///
        /// @author S Downie
        //---------------------------------------------
        enum class ShaderPass
        {
            k_ambient,
            k_directional,
            k_point,
            k_total
        };
	}
}

#endif