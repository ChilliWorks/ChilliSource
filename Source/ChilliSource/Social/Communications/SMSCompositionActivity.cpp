//
//  SMSCompositionActivity.cpp
//  Chilli Source
//
//  Created by Ian Copland on 06/03/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//

#include <ChilliSource/Social/Communications/SMSCompositionActivity.h>

#ifdef CS_TARGETPLATFORM_IOS
#   include <ChilliSource/Backend/Platform/iOS/Social/Communications/SMSCompositionActivity.h>
#elif defined CS_TARGETPLATFORM_ANDROID
#   include <ChilliSource/Backend/Platform/Android/Social/Communications/SMSCompositionActivity.h>
#endif

namespace ChilliSource
{
    namespace Social
    {
        CS_DEFINE_NAMEDTYPE(SMSCompositionActivity);
        //----------------------------------------------------
        //----------------------------------------------------
        SMSCompositionActivityUPtr SMSCompositionActivity::Create()
        {
#ifdef CS_TARGETPLATFORM_IOS
            return SMSCompositionActivityUPtr(new iOS::SMSCompositionActivity());
#elif defined CS_TARGETPLATFORM_ANDROID
            return SMSCompositionActivityUPtr(new Android::SMSCompositionActivity());
#else
            return SMSCompositionActivityUPtr();
#endif
        }
    }
}