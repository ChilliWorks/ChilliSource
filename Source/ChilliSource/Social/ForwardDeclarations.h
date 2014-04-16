//
//  ForwardDeclarations.h
//  Chilli Source
//
//  Created by Stuart McGaw on 07/06/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_SOCIAL_FORWARDDECLARATIONS_H_
#define _CHILLISOURCE_SOCIAL_FORWARDDECLARATIONS_H_

#include <ChilliSource/Core/Base/StandardMacros.h>

#include <memory>

namespace ChilliSource
{
	namespace Social
    {
        //------------------------------------------------------
        /// Communications
        //------------------------------------------------------
        CS_FORWARDDECLARE_CLASS(ContactInformationProvider);
        CS_FORWARDDECLARE_CLASS(EmailComposer);
        //------------------------------------------------------
        /// Facebook
        //------------------------------------------------------
        CS_FORWARDDECLARE_CLASS(FacebookAuthenticationSystem);
        CS_FORWARDDECLARE_CLASS(FacebookPostSystem);
        //------------------------------------------------------
        /// Twitter
        //------------------------------------------------------
        CS_FORWARDDECLARE_CLASS(TwitterAuthenticationActivity);
        CS_FORWARDDECLARE_CLASS(TwitterPostSystem);
	}
}

#endif