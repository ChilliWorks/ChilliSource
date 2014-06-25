//
//  EmailCompositionJavaInterface.h
//  Chilli Source
//  Created by Steven Hendrie on 20/01/2012.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2012 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#ifdef CS_TARGETPLATFORM_ANDROID

#ifndef _CHILLISOURCE_PLATFORM_ANDROID_JAVAINTERFACE_EMAILCOMPOSITION_H
#define _CHILLISOURCE_PLATFORM_ANDROID_JAVAINTERFACE_EMAILCOMPOSITION_H

#include <CSBackend/Platform/Android/Core/JNI/JavaInterface.h>

#include <CSBackend/Platform/Android/ForwardDeclarations.h>

namespace CSBackend
{
	namespace Android
	{
		class EmailComposerJavaInterface: public CSBackend::Android::IJavaInterface
		{
		public:
			CS_DECLARE_NAMEDTYPE(EmailCompositionJavaInterface);

			//--------------------------------------------------------------
			/// An enum describing the possible results from presenting the
			/// email composer.
			///
			/// @author Ian Copland
			//--------------------------------------------------------------
			enum class Result
			{
				k_success,
				k_cancelled,
				k_failed
			};
			//--------------------------------------------------------------
			/// Delegates
			//--------------------------------------------------------------
			typedef std::function<void(Result)> ResultDelegate;
			//--------------------------------------------------------------
			/// Constructor
			//--------------------------------------------------------------
			EmailComposerJavaInterface();
			//--------------------------------------------------------------
			/// Is A
			///
			/// @return whether or not this object implements the given interface.
			//--------------------------------------------------------------
			bool IsA(CSCore::InterfaceIDType inInterfaceID) const;
			//--------------------------------------------------------------
			/// Present
			///
			/// Presents the user with the email interface.
			///
			/// @param vector of email addresses (UTF-8)
			/// @param subject line (UTF-8)
			/// @param content (UTF-8)
			/// @param bool if to format as html
			/// @param The attachment filename.
			/// @param result callback delegate
			//--------------------------------------------------------------
			void Present(const std::vector<std::string>& inastrRecipientAddresses, const std::string& instrSubject, const std::string& instrContents,
					bool inbFormatAsHtml, const std::string& instrAttachmentFilename, const ResultDelegate& inDelegate);
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

#endif