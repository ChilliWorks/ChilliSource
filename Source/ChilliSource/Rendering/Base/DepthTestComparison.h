//
//  DepthTestComparison.h
//  Chilli Source
//
//  Created by Scott Downie on 09/04/2014.
//  Copyright 2014 Tag Games. All rights reserved.
//


#ifndef _CHILLISOURCE_RENDERING_BASE_DEPTHTESTCOMPARISON_H_
#define _CHILLISOURCE_RENDERING_BASE_DEPTHTESTCOMPARISON_H_

namespace ChilliSource
{
	namespace Rendering
	{
        //---------------------------------------------
        /// Used in the depth test function to decide
        /// whether a pixel should be rendered or not.
        /// These types describe which comparison operator
        /// should be used
        ///
        /// @author S Downie
        //---------------------------------------------
        enum class DepthTestComparison
        {
            k_less,
            k_equal,
            k_lequal
        };
	}
}

#endif