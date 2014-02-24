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

#include <ChilliSource/Platform/Android/JavaInterface/_JavaInterface.h>

namespace ChilliSource
{
	namespace Android
	{

		class SCSMSCompositionJavaInterface: public ChilliSource::Android::_IJavaInterface
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
			static void Present(const std::vector<CUTF8String> & inastrRecipientNumbers, //Vector of email addresses
								const CUTF8String & instrContents);
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
