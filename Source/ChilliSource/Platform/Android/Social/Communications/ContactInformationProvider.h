/*
 *  ContactInformationProvider.h
 *  moFlo
 *
 *  Created by Steven Hendrie on 20/01/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */


#ifndef _MOFLO_PLATFORM_ANDROID_SOCIAL_COMMUNICATIONS_CCONTACTINFORMATIONSERVICE_H_
#define _MOFLO_PLATFORM_ANDROID_SOCIAL_COMMUNICATIONS_CCONTACTINFORMATIONSERVICE_H_

#include <moFlo/Social/Communications/ContactInformationProvider.h>
#include <vector>

namespace moFlo
{
	namespace AndroidPlatform
	{
		class CContactInformationProvider: public moFlo::Social::IContactInformationProvider
		{
		public:
			virtual bool IsA(Core::InterfaceIDType inID) const;

			virtual void LoadInformation();

			virtual u32 GetNumberContacts();
			virtual const Social::ContactRecord & GetContactAtIndex(u32 inudwIndex);

			void SetContacts(std::vector<Social::ContactRecord> inasContact);

		private:
			std::vector<Social::ContactRecord> masContacts;

		};

	}
}

#endif
