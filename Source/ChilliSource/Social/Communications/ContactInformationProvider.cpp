//
//  ContactInformationProvider.cpp
//  Chilli Source
//
//  Created by Ian Copland on 06/03/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//

#include <ChilliSource/Social/Communications/ContactInformationProvider.h>

#ifdef CS_TARGETPLATFORM_IOS
#   include <ChilliSource/Backend/Platform/iOS/Social/Communications/ContactInformationProvider.h>
#elif defined CS_TARGETPLATFORM_ANDROID
#   include <ChilliSource/Backend/Platform/Android/Social/Communications/ContactInformationProvider.h>
#endif

namespace ChilliSource
{
    namespace Social
    {
        CS_DEFINE_NAMEDTYPE(ContactInformationProvider);
        //---------------------------------------------------------
        //---------------------------------------------------------
        ContactInformationProviderUPtr ContactInformationProvider::Create()
        {
#ifdef CS_TARGETPLATFORM_IOS
            return ContactInformationProviderUPtr(new iOS::ContactInformationProvider());
#elif defined CS_TARGETPLATFORM_ANDROID
            return ContactInformationProviderUPtr(new Android::ContactInformationProvider());
#else
            return ContactInformationProviderUPtr();
#endif
        }
    }
}
