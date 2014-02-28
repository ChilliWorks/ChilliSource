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

#include <ChilliSource/Social/Communications/ContactInformationProvider.h>
#include <vector>

namespace ChilliSource
{
	namespace Android
	{
		class CContactInformationProvider: public ChilliSource::Social::ContactInformationProvider
		{
		public:

			bool IsA(Core::InterfaceIDType inID) const override;

			void LoadInformation() override;

			u32 GetNumberContacts() override;
			const Social::ContactRecord & GetContactAtIndex(u32 inudwIndex) override;

			void SetContacts(const std::vector<Social::ContactRecord>& inasContact);

		private:
			std::vector<Social::ContactRecord> masContacts;

		};

	}
}

#endif
