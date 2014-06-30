//
//  EmailComposer.h
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

#ifndef _CSBACKEND_PLATFORM_ANDROID_SOCIAL_COMMUNICATIONS_EMAILCOMPOSER_H_
#define _CSBACKEND_PLATFORM_ANDROID_SOCIAL_COMMUNICATIONS_EMAILCOMPOSER_H_

#include <ChilliSource/ChilliSource.h>
#include <CSBackend/Platform/Android/ForwardDeclarations.h>
#include <CSBackend/Platform/Android/Social/Communications/EmailComposerJavaInterface.h>
#include <ChilliSource/Social/Communications/EmailComposer.h>

namespace CSBackend
{
	namespace Android
	{
		//---------------------------------------------------------------
		/// The Android backend for the Email Composer.
		///
		/// @author S Hendrie
		//---------------------------------------------------------------
		class EmailComposer final : public CSSocial::EmailComposer
		{
		public:
			CS_DECLARE_NAMEDTYPE(EmailComposer);
            //-------------------------------------------------------
			/// Queries whether or not this system implements the
            /// interface with the given Id.
			///
            /// @author S Hendrie
            ///
			/// @param The interface Id.
			/// @param Whether system is of given type.
			//-------------------------------------------------------
			bool IsA(CSCore::InterfaceIDType in_interfaceId) const override;
            //-------------------------------------------------------
            /// Displays the email activity with the given recipients,
            /// subject and contents.
			///
			/// @author S Hendrie
            ///
            /// @param A list of recipients (UTF-8).
            /// @param The subject (UTF-8).
            /// @param The body of the email (UTF-8).
            /// @param The format of the body of the email.
            /// @param The callback describing the result of the email.
            //-------------------------------------------------------
			void Present(const std::vector<std::string>& in_recipientAddresses, const std::string& in_subject, const std::string& in_contents, ContentFormat in_contentFormat, const SendResultDelegate& in_callback) override;
			//-------------------------------------------------------
			/// Displays the email activity with the given recipients,
			/// subject and contents, and adds a list of attachments
			/// to the email.
			///
			/// @author Ian Copland
			///
			/// @param A list of recipients (UTF-8).
			/// @param The subject (UTF-8).
			/// @param The body of the email (UTF-8).
            /// @param The format of the body of the email.
			/// @param The attachment
			/// @param The callback describing the result of the email.
			//-------------------------------------------------------
			void PresentWithAttachment(const std::vector<std::string>& in_recipientAddresses, const std::string& in_subject, const std::string& in_contents, ContentFormat in_contentFormat,
					const Attachment& in_attachment, const SendResultDelegate& in_callback) override;
            //-------------------------------------------------------
            /// Dismisses the activity if it is currently displayed.
			///
			/// @author S Hendrie
            //-------------------------------------------------------
			void Dismiss() override;
		private:
			friend CSSocial::EmailComposerUPtr CSSocial::EmailComposer::Create();
            //----------------------------------------------------
            /// Private constructor to force the use of the
            /// factory method.
            ///
            /// @author Ian Copland
            //----------------------------------------------------
			EmailComposer();
            //------------------------------------------------------
            /// Called when the the owning state is initialised.
            ///
            /// @author Ian Copland
            //------------------------------------------------------
            void OnInit() override;
            //-------------------------------------------------------
            /// Called when the email activity returns with a result.
            ///
            /// @author Ian Copland
			///
			/// @param The result.
            //-------------------------------------------------------
			void OnEmailClosed(EmailComposerJavaInterface::Result in_result);
            //------------------------------------------------------
            /// Called when the the owning state is destroyed.
            ///
            /// @author Ian Copland
            //------------------------------------------------------
            void OnDestroy() override;

            bool m_isPresented;
			EmailComposerJavaInterfaceSPtr m_javaInterface;
			SendResultDelegate m_resultDelegate;
		};

	}
}

#endif

#endif