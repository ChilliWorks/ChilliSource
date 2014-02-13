/*
 *  ContactInformationProvider.cpp
 *  moFlo
 *
 *  Created by Steven Hendrie on 20/01/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */


#include <moFlo/Platform/Android/Social/Communications/ContactInformationProvider.h>
#include <moFlo/Platform/Android/JavaInterface/ContactInformationProviderJavaInterface.h>

namespace moFlo
{
	namespace AndroidPlatform
	{

		bool CContactInformationProvider::IsA(Core::InterfaceIDType inID) const
		{
			return inID == CContactInformationProvider::InterfaceID;
		}

		void CContactInformationProvider::LoadInformation()
		{
			masContacts.clear();
			SCContactInformationProviderJavaInterface::LoadInformation(this);
		}

		u32 CContactInformationProvider::GetNumberContacts()
		{
			return masContacts.size();
		}
		const Social::ContactRecord & CContactInformationProvider::GetContactAtIndex(u32 inudwIndex)
		{
			return masContacts[inudwIndex];
		}

		void CContactInformationProvider::SetContacts(std::vector<Social::ContactRecord> inasContacts)
		{
			masContacts = inasContacts;
		}
	}
}
