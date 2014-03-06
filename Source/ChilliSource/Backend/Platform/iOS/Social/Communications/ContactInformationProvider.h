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
#include <ChilliSource/Backend/Platform/iOS/ForwardDeclarations.h>
#include <ChilliSource/Social/Communications/ContactInformationProvider.h>

#include <vector>

namespace ChilliSource
{
	namespace iOS
    {
		class ContactInformationProvider : public Social::ContactInformationProvider
		{
		public:
			virtual bool IsA(Core::InterfaceIDType inID) const override;
			
			virtual void LoadInformation() override;
			
			virtual u32 GetNumberContacts() override;
			virtual const Social::ContactRecord & GetContactAtIndex(u32 inudwIndex) override;
		private:
			std::vector<Social::ContactRecord> masContacts;
		};
		
	}
	
}

#endif