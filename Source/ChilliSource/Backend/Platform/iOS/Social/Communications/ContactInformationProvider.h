/*
 *  ContactInformationProvider.h
 *  moFlo
 *
 *  Created by Stuart McGaw on 08/06/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLO_PLATFORM_IOS_SOCIAL_COMMUNICATIONS_CCONTACTINFORMATIONSERVICE_H_
#define _MOFLO_PLATFORM_IOS_SOCIAL_COMMUNICATIONS_CCONTACTINFORMATIONSERVICE_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Social/Communications/ContactInformationProvider.h>

#include <vector>

namespace ChilliSource{

	namespace iOS {
		
		class CContactInformationProvider : public Social::IContactInformationProvider
		{
		public:
			virtual bool IsA(Core::InterfaceIDType inID) const;
			
			virtual void LoadInformation();
			
			virtual u32 GetNumberContacts();
			virtual const Social::ContactRecord & GetContactAtIndex(u32 inudwIndex);
		private:
			DYNAMIC_ARRAY<Social::ContactRecord> masContacts;
		};
		
	}
	
}

#endif