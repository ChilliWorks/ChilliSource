/*
 *  ContactInformationProvider.cpp
 *  moFlo
 *
 *  Created by Steven Hendrie on 20/01/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */


#include <ChilliSource/Backend/Platform/Android/Social/Communications/ContactInformationProvider.h>
#include <ChilliSource/Backend/Platform/Android/JavaInterface/ContactInformationProviderJavaInterface.h>

namespace ChilliSource
{
	namespace Android
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
