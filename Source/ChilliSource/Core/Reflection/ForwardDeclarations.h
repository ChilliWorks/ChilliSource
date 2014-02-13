//
//  ForwardDeclarations.h
//  MoFlowSkeleton
//
//  Created by Robert Henning on 29/11/2011.
//  Copyright (c) 2011 Tag Games. All rights reserved.
//

#ifndef MO_FLOW_CORE_REFLECTION_FORWARD_DECLARATIONS_H_
#define MO_FLOW_CORE_REFLECTION_FORWARD_DECLARATIONS_H_

namespace moFlo 
{
    namespace Reflect
    {
        enum PropAccess
        {
            PROP_READ_ONLY,
            PROP_WRITE_ONLY,
            PROP_READ_WRITE
        };
        
        //---Forward Declarations
        class CInstance;
        class CMetaClass;
        class CProperty;
        template <typename T, typename ReturnType = void> class CMethod;
    }
}

#endif
