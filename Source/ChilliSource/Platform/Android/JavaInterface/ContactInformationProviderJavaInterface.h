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

#include <ChilliSource/Social/Communications/ContactInformationProvider.h>
#include <Platform/Android/Social/Communications/ContactInformationProvider.h>
#include <vector>
#include <ChilliSource/Platform/Android/JavaInterface/_JavaInterface.h>

namespace moFlo
{
	namespace AndroidPlatform
	{

		class SCContactInformationProviderJavaInterface : public moFlo::AndroidPlatform::_IJavaInterface
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

			static std::vector<UTF8String> mastrNames;
			static std::vector<UTF8String> mastrNumbers;
			static std::vector<UTF8String> mastrEmails;
		private:
			static std::vector<Social::ContactRecord> CreateContacts();


			static CContactInformationProvider * mpContactProvider;


		};

	}
}
#endif
