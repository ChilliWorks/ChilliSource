/*
 *  SocialInterfaceDefinitions.cpp
 *  moFlo
 *
 *  Created by Stuart McGaw on 10/06/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Social/Communications/ContactInformationProvider.h>
#include <ChilliSource/Social/Communications/SMSCompositionActivity.h>
#include <ChilliSource/Social/Communications/EmailCompositionActivity.h>

namespace ChilliSource{

	namespace Social{
		DEFINE_NAMED_INTERFACE(IContactInformationProvider);
		DEFINE_NAMED_INTERFACE(ISMSCompositionActivity);
		DEFINE_NAMED_INTERFACE(IEmailCompositionActivity);
	}

}