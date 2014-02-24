/*
 *  ForwardDeclarations.h
 *  moFlo
 *
 *  Forward declarations for classes in the ChilliSource::Social namespace!
 *
 *  Created by Stuart McGaw on 07/06/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLO_SOCIAL_FORWARDDECLARATIONS_H_
#define _MOFLO_SOCIAL_FORWARDDECLARATIONS_H_

namespace ChilliSource{
	namespace Social{
	
		//Communications
		class ISMSCompositionActivity;
		class IEmailCompositionActivity;
		class IContactInformationProvider;
		
		//Facebook
		class IFacebookAuthenticationSystem;
		class IFacebookPostSystem;
		class IFacebookGraphRequestSystem;
		
		//Twitter
		class ITwitterPostSystem;
		
	}
}

#endif