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
        CS_FORWARD_DECLARE_CLASS(ContactInformationProvider);
        CS_FORWARD_DECLARE_CLASS(EmailCompositionActivity);
        CS_FORWARD_DECLARE_CLASS(SMSCompositionActivity);
        //------------------------------------------------------
        /// Facebook
        //------------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(FacebookAuthenticationSystem);
        CS_FORWARD_DECLARE_CLASS(IFacebookPostSystem);
        //------------------------------------------------------
        /// Twitter
        //------------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(TwitterAuthenticationActivity);
        CS_FORWARD_DECLARE_CLASS(TwitterPostSystem);
	}
}

#endif