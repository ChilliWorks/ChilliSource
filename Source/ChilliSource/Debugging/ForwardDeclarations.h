//
//  ForwardDeclarations.h
//  Chilli Source
//
//  Created by Ian Copland on 25/02/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//

#ifndef _CHILLISOURCE_DEBUGGING_FORWARDDECLARATIONS_H_
#define _CHILLISOURCE_DEBUGGING_FORWARDDECLARATIONS_H_

#include <ChilliSource/Core/Base/StandardMacros.h>

#include <memory>

namespace ChilliSource
{
    namespace Debugging
    {
        //--------------------------------------------------
        /// Base
        //--------------------------------------------------
#ifdef CS_ENABLE_DEBUGSTATS
        CS_FORWARDDECLARE_CLASS(DebugStats);
        CS_FORWARDDECLARE_CLASS(DebugStatsView);
#endif
    }
}
#endif
