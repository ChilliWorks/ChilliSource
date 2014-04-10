//
//  BlendMode.h
//  Chilli Source
//
//  Created by Scott Downie on 09/04/2014.
//  Copyright 2014 Tag Games. All rights reserved.
//


#ifndef _CHILLISOURCE_RENDERING_BASE_BLENDMODE_H_
#define _CHILLISOURCE_RENDERING_BASE_BLENDMODE_H_

namespace ChilliSource
{
	namespace Rendering
	{
        //---------------------------------------------
        /// The blend modes that are used to describe
        /// a blend function for rendering. The blend
        /// function takes 2 modes - the source and
        /// the destination.
        ///
        /// @author S Downie
        //---------------------------------------------
        enum class BlendMode
        {
            k_zero,
            k_one,
            k_sourceCol,
            k_oneMinusSourceCol,
            k_sourceAlpha,
            k_oneMinusSourceAlpha,
            k_destAlpha,
            k_oneMinusDestAlpha
        };
	}
}

#endif
