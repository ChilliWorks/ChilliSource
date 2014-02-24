/*
 *  EmailCompositionJavaInterface.h
 *  moFlow
 *
 *  Created by Steven Hendrie on 20/01/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

#ifndef MOFLOW_PLATFORM_ANDROID_JAVAINTERFACE_EMAILCOMPOSITION_H
#define MOFLOW_PLATFORM_ANDROID_JAVAINTERFACE_EMAILCOMPOSITION_H

#include <ChilliSource/Backend/Platform/Android/ForwardDeclarations.h>
#include <ChilliSource/Platform/Android/JavaInterface/JavaInterface.h>
#include <ChilliSource/Social/Communications/EmailCompositionActivity.h>

namespace ChilliSource
{
	namespace Android
	{
		class CEmailCompositionJavaInterface: public ChilliSource::Android::IJavaInterface
		{
		public:
			DECLARE_NAMED_INTERFACE(SCEmailCompositionJavaInterface);

			//--------------------------------------------------------------
			/// Result Codes
			//--------------------------------------------------------------
			static const s32 kdwResultSuccess = -1;
			static const s32 kdwResultCancelled = 0;
			//--------------------------------------------------------------
			/// Delegates
			//--------------------------------------------------------------
			typedef fastdelegate::FastDelegate1<s32> ResultDelegate;
			//--------------------------------------------------------------
			/// Constructor
			//--------------------------------------------------------------
			CEmailCompositionJavaInterface();
			//--------------------------------------------------------------
			/// Is A
			///
			/// @return whether or not this object implements the given interface.
			//--------------------------------------------------------------
			bool IsA(Core::InterfaceIDType inInterfaceID) const;
			//--------------------------------------------------------------
			/// Present
			///
			/// Presents the user with the email interface.
			///
			/// @param vector of email addresses
			/// @param subject line
			/// @param content
			/// @param The attachment filename.
			/// @param bool if to format as html
			/// @param result callback delegate
			//--------------------------------------------------------------
			void Present(const DYNAMIC_ARRAY<CUTF8String>& inastrRecipientAddresses, const CUTF8String& instrSubject, const CUTF8String& instrContents,
						const std::string& instrAttachmentFilename, bool inbFormatAsHtml, const ResultDelegate& inDelegate);
			//--------------------------------------------------------------
			/// On Email Closed
			///
			/// Called when a result is returned by the activity.
			///
			/// @param The result code.
			//--------------------------------------------------------------
			void OnEmailClosed(s32 indwResultCode);
		private:
			ResultDelegate mDelegate;
		};
	}
}
#endif
