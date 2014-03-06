//
//  EmailCompositionActivity.cpp
//  Chilli Source
//
//  Created by Ian Copland on 06/03/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//

#include <ChilliSource/Social/Communications/EmailCompositionActivity.h>

#ifdef CS_TARGETPLATFORM_IOS
#   include <ChilliSource/Backend/Platform/iOS/Social/Communications/EmailCompositionActivity.h>
#elif defined CS_TARGETPLATFORM_ANDROID
#   include <ChilliSource/Backend/Platform/Android/Social/Communications/EmailCompositionActivity.h>
#endif

namespace ChilliSource
{
    namespace Social
    {
        CS_DEFINE_NAMEDTYPE(EmailCompositionActivity);
        //-------------------------------------------------------
        /// Creates a new instance of the email composition
        /// activity.
        ///
        /// @author I Copland
        ///
        /// @return the new activity instance.
        //-------------------------------------------------------
        EmailCompositionActivityUPtr EmailCompositionActivity::Create()
        {
#ifdef CS_TARGETPLATFORM_IOS
            return EmailCompositionActivityUPtr(new iOS::EmailCompositionActivity());
#elif defined CS_TARGETPLATFORM_ANDROID
            return EmailCompositionActivityUPtr(new Android::EmailCompositionActivity());
#else
            return EmailCompositionActivityUPtr();
#endif
        }
    }
}
