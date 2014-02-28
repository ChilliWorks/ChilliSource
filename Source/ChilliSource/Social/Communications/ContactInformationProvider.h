/*
 *  ContactInformationProvider.h
 *  moFlow
 *
 *  Created by Stuart McGaw on 08/06/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLO_SOCIAL_COMMUNICATIONS_CONTACTINFORMATIONPROVIDER_H_
#define _MOFLO_SOCIAL_COMMUNICATIONS_CONTACTINFORMATIONPROVIDER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/String/UTF8String.h>
#include <ChilliSource/Core/Base/InformationProvider.h>

namespace ChilliSource{

	namespace Social {
		
		struct ContactRecord {
		
			Core::UTF8String strName;
			bool bHasEmail;
			Core::UTF8String strEmail;
			bool bHasMobileNumber;
			Core::UTF8String strMobileNumber;
			
		};
		
		class ContactInformationProvider : public Core::IInformationProvider{
		public:
			
			CS_DECLARE_NAMEDTYPE(ContactInformationProvider);
			
			virtual void LoadInformation() = 0;
			
			virtual u32 GetNumberContacts() = 0;
			virtual const ContactRecord & GetContactAtIndex(u32 inudwIndex) = 0;
			
		};
		
	}

}

#endif