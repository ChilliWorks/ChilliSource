//
//  ForwardDeclarations.h
//  SocialCat
//
//  Created by Robert Henning on 25/03/2013.
//  Copyright (c) 2013 Tag Games Ltd. All rights reserved.
//

#ifndef __CORE__SCENEANIMATION__FORWARD_DECLARATIONS_H__
#define __CORE__SCENEANIMATION__FORWARD_DECLARATIONS_H__

// Shortcut macro for defining full gamut of shared/weak ptrs
#define FORWARD_DECLARE_GENERIC(x,prefix)  \
class                                    prefix##x;\
typedef std::shared_ptr<prefix##x>            x##Ptr;\
typedef std::shared_ptr<const prefix##x>      x##CPtr;\
typedef std::weak_ptr<prefix##x>              x##WPtr;\
typedef std::weak_ptr<const prefix##x>        x##CWPtr;

#define FORWARD_DECLARE_CLASS(x)        FORWARD_DECLARE_GENERIC(x,C)
#define FORWARD_DECLARE_INTERFACE(x)    FORWARD_DECLARE_GENERIC(x,I)

namespace ChilliSource
{
    namespace Core
    {        
        FORWARD_DECLARE_CLASS(SceneDesc)
        FORWARD_DECLARE_CLASS(SceneAnimation);
    }
}

#endif
