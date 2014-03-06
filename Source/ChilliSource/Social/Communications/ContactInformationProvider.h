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

namespace ChilliSource
{
	namespace Social
    {
		struct ContactRecord
        {
			Core::UTF8String m_name;
			bool m_hasEmail;
			Core::UTF8String m_email;
			bool m_hasMobileNumber;
			Core::UTF8String m_mobileNumber;
		};
		
		class ContactInformationProvider : public Core::IInformationProvider
        {
		public:
			CS_DECLARE_NAMEDTYPE(ContactInformationProvider);
            //---------------------------------------------------------
            /// Creates a new instance of the contact information
            /// provider.
            ///
            /// @author I Copland
            ///
            /// @return The new instance.
            //---------------------------------------------------------
            static ContactInformationProviderUPtr Create();
            //---------------------------------------------------------
            /// Loads information about contacts.
            ///
            /// @author I Copland
            //---------------------------------------------------------
			virtual void LoadInformation() = 0;
            //---------------------------------------------------------
            /// Returns the number of contacts loaded by LoadInformation
            ///
            /// @author I Copland
            ///
            /// @return The number of contacts.
            //---------------------------------------------------------
			virtual u32 GetNumberContacts() = 0;
            //---------------------------------------------------------
            /// Returns the contact at the given index.
            ///
            /// @author I Copland
            ///
            /// @param The index
            ///
            /// @return The contact.
            //---------------------------------------------------------
			virtual const ContactRecord & GetContactAtIndex(u32 in_index) = 0;
            //---------------------------------------------------------
            /// Destructor
            ///
            /// @author I Copland
            //---------------------------------------------------------
			virtual ~ContactInformationProvider(){};
        protected:
            //---------------------------------------------------------
            /// Protected constructor to force use of the factory
            /// method.
            ///
            /// @author I Copland
            //---------------------------------------------------------
            ContactInformationProvider(){};
		};
		
	}

}

#endif