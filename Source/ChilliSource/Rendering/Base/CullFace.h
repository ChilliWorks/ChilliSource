//
//  CullFace.h
//  Chilli Source
//
//  Created by Scott Downie on 09/04/2014.
//  Copyright 2014 Tag Games. All rights reserved.
//


#ifndef _CHILLISOURCE_RENDERING_BASE_CULLFACE_H_
#define _CHILLISOURCE_RENDERING_BASE_CULLFACE_H_

namespace ChilliSource
{
	namespace Rendering
	{
        //---------------------------------------------
        /// Used in the face culling function to
        /// describe which faces to remove.
        ///
        /// @author S Downie
        //---------------------------------------------
        enum class CullFace
        {
            k_front,
            k_back
        };
	}
}

#endif
