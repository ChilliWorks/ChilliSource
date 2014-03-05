/*
 *  ContactInformationProvider.cpp
 *  moFlo
 *
 *  Created by Steven Hendrie on 20/01/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */


#include <ChilliSource/Backend/Platform/Android/Social/Communications/ContactInformationProvider.h>

#include <ChilliSource/Backend/Platform/Android/Social/Communications/ContactInformationProviderJavaInterface.h>

namespace ChilliSource
{
	namespace Android
	{
		bool ContactInformationProvider::IsA(Core::InterfaceIDType inID) const
		{
			return inID == ContactInformationProvider::InterfaceID;
		}

		void ContactInformationProvider::LoadInformation()
		{
			masContacts.clear();
			ContactInformationProviderJavaInterface::LoadInformation(this);
		}

		u32 ContactInformationProvider::GetNumberContacts()
		{
			return masContacts.size();
		}
		const Social::ContactRecord & ContactInformationProvider::GetContactAtIndex(u32 inudwIndex)
		{
			return masContacts[inudwIndex];
		}

		void ContactInformationProvider::SetContacts(const std::vector<Social::ContactRecord>& inasContacts)
		{
			masContacts = inasContacts;
		}
	}
}
