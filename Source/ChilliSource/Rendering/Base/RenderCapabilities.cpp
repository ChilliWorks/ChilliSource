//
//  RenderCapabilities.cpp
//  Chilli Source
//
//  Created by Ian Copland on 27/01/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//

#include <ChilliSource/Rendering/Base/RenderCapabilities.h>

#if defined CS_TARGETPLATFORM_IOS || defined CS_TARGETPLATFORM_ANDROID || defined CS_TARGETPLATFORM_WINDOWS
#include <ChilliSource/Backend/Rendering/OpenGL/Base/RenderCapabilities.h>
#endif

namespace ChilliSource
{
    namespace Rendering
    {
        CS_DEFINE_NAMEDTYPE(RenderCapabilities);
        
        //-------------------------------------------------------
        //-------------------------------------------------------
        RenderCapabilitiesUPtr RenderCapabilities::Create()
        {
#if defined CS_TARGETPLATFORM_IOS || defined CS_TARGETPLATFORM_ANDROID || defined CS_TARGETPLATFORM_WINDOWS
            return RenderCapabilitiesUPtr(new OpenGL::RenderCapabilities());
#else
            return nullptr;
#endif
        }
    }
}
