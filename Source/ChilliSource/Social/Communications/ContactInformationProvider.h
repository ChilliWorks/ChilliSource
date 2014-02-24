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

#include <ChilliSource/Core/String/UTF8String.h>
#include <ChilliSource/Core/Base/InformationProvider.h>

namespace ChilliSource{

	namespace Social {
		
		struct ContactRecord {
		
			CUTF8String strName;
			bool bHasEmail;
			CUTF8String strEmail;
			bool bHasMobileNumber;
			CUTF8String strMobileNumber;
			
		};
		
		class IContactInformationProvider : public IInformationProvider{
		public:
			
			DECLARE_NAMED_INTERFACE(IContactInformationProvider);
			
			virtual void LoadInformation() = 0;
			
			virtual u32 GetNumberContacts() = 0;
			virtual const ContactRecord & GetContactAtIndex(u32 inudwIndex) = 0;
			
		};
		
	}

}

#endif