/*
 *  SocialInterfaceDefinitions.cpp
 *  moFlo
 *
 *  Created by Stuart McGaw on 10/06/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#include <moFlo/Social/Communications/ContactInformationProvider.h>
#include <moFlo/Social/Communications/SMSCompositionActivity.h>
#include <moFlo/Social/Communications/EmailCompositionActivity.h>

namespace moFlo{

	namespace Social{
		DEFINE_NAMED_INTERFACE(IContactInformationProvider);
		DEFINE_NAMED_INTERFACE(ISMSCompositionActivity);
		DEFINE_NAMED_INTERFACE(IEmailCompositionActivity);
	}

}