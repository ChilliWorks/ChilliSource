/*
 *  ContactInformationProviderJavaInterface.h
 *  moFlow
 *
 *  Created by Steven Hendrie on 20/01/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLO_PLATFORM_ANDROID_JAVAINTERFACE_CONTACTIONINFOPROVIDER_H_
#define _MOFLO_PLATFORM_ANDROID_JAVAINTERFACE_CONTACTIONINFOPROVIDER_H_

#include <ChilliSource/Backend/Platform/Android/Core/JNI/_JavaInterface.h>
#include <ChilliSource/Backend/Platform/Android/Social/Communications/ContactInformationProvider.h>
#include <ChilliSource/Social/Communications/ContactInformationProvider.h>

#include <vector>

namespace ChilliSource
{
	namespace Android
	{
		class SCContactInformationProviderJavaInterface : public _IJavaInterface
		{
		public:
			//--------------------------------------------------------------------------------------
			/// Setup Java Interface
			///
			/// Sets up the java interface: stores the Virtual Machine, and Initialises any java
			/// methods callable from native.
			///
			/// @param a pointer to the virtual machine
			//--------------------------------------------------------------------------------------
			static void SetupJavaInterface(JavaVM* inpJavaVM);
			//--------------------------------------------------------------------------------------
			/// Load Information
			///
			/// Loads contact information for player
			///
			/// @return array of data for contacts
			//--------------------------------------------------------------------------------------
			static void LoadInformation(CContactInformationProvider * inpContactProvider);

			static std::vector<Core::UTF8String> mastrNames;
			static std::vector<Core::UTF8String> mastrNumbers;
			static std::vector<Core::UTF8String> mastrEmails;

		private:

			static std::vector<Social::ContactRecord> CreateContacts();

			static CContactInformationProvider * mpContactProvider;
		};

	}
}
#endif
