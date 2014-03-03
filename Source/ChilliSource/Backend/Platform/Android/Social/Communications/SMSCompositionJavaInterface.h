/*
 *  SMSCompositionJavaInterface.h
 *  moFlow
 *
 *  Created by Steven Hendrie on 20/01/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLO_PLATFORM_ANDROID_JAVAINTERFACE_SMSCOMPOSITIONJAVAINTERFACE_H_
#define _MOFLO_PLATFORM_ANDROID_JAVAINTERFACE_SMSCOMPOSITIONJAVAINTERFACE_H_

#include <ChilliSource/Backend/Platform/Android/Core/JNI/_JavaInterface.h>

namespace ChilliSource
{
	namespace Android
	{
		class SCSMSCompositionJavaInterface: public _IJavaInterface
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
			/// Present
			///
			/// Presents the user with the email interface
			///
			/// @param vector of email addresses
			/// @param subject line
			/// @param content
			/// @param result callback delegate
			/// @param bool if to format as html
			//--------------------------------------------------------------------------------------
			static void Present(const std::vector<Core::UTF8String> & inastrRecipientNumbers, //Vector of email addresses
								const Core::UTF8String & instrContents);
			//--------------------------------------------------------------------------------------
			/// Dismiss
			///
			/// Removes the sms display
			///
			//--------------------------------------------------------------------------------------
			static void Dismiss();
		};

	}
}

#endif
