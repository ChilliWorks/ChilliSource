//
//  EmailComposer.h
//  ChilliSource
//  Created by Stuart McGaw on 09/06/2011.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2011 Tag Games Limited
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

#ifdef CS_TARGETPLATFORM_IOS

#import <ChilliSource/ChilliSource.h>
#import <CSBackend/Platform/iOS/ForwardDeclarations.h>
#import <ChilliSource/Social/Communications/EmailComposer.h>

#import <MessageUI/MessageUI.h>

#import <mutex>

@class EmailComposerDelegate;

namespace CSBackend
{
	namespace iOS
    {
        //-------------------------------------------------------------
        /// The iOS backend for the email composition state system.
        ///
        /// @author S McGaw
        //-------------------------------------------------------------
        class EmailComposer final : public ChilliSource::EmailComposer
		{
		public:
            CS_DECLARE_NAMEDTYPE(EmailComposer);
            //-------------------------------------------------------
			/// Queries whether or not this system implements the
            /// interface with the given Id.
			///
            /// @author S McGaw
            ///
			/// @param The interface Id.
			/// @return Whether system is of given type.
			//-------------------------------------------------------
			bool IsA(ChilliSource::InterfaceIDType in_interfaceId) const override;
            //-------------------------------------------------------
            /// Displays the email activity with the given recipients,
            /// subject and contents.
            ///
            /// @author S Downie
            ///
            /// @param A list of recipients (UTF-8).
            /// @param The subject (UTF-8).
            /// @param The body of the email (UTF-8).
            /// @param The format of the body of the email.
            /// @param The callback describing the result of the email.
            //-------------------------------------------------------
			void Present(const std::vector<std::string>& in_recipientAddresses, const std::string& in_subject, const std::string& in_contents, ContentFormat in_contentFormat,
                         const SendResultDelegate& in_callback) override;
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
            /// @param The attachment.
            /// @param The callback describing the result of the email.
            //-------------------------------------------------------
            void PresentWithAttachment(const std::vector<std::string>& in_recipientAddresses, const std::string& in_subject, const std::string& in_contents, ContentFormat in_contentFormat,
                                       const Attachment& in_attachment, const SendResultDelegate & in_callback) override;
            //-------------------------------------------------------
            /// Determines whether or not the composer is presented.
            /// @author Jordan Brown
            //-------------------------------------------------------
            bool IsPresented() override;
            //-------------------------------------------------------
            /// Called when the result is received from the email
            /// composition view controller. This is for internal
            /// use and should not be called manually by the user.
            ///
            /// @author Ian Copland
            //-------------------------------------------------------
			void OnResult(MFMailComposeViewController* in_viewController, MFMailComposeResult in_result);
		private:
            friend ChilliSource::EmailComposerUPtr ChilliSource::EmailComposer::Create();
            
            //-------------------------------------------------------
            /// This checks whether or not the current iOS device
            /// supports email. This should always be checked before
            /// creating an instance of the class.
            ///
            /// @author S McGaw
            //-------------------------------------------------------
            static bool IsSupportedByDevice();
            //----------------------------------------------------
            /// Private constructor to force the use of the
            /// factory method.
            ///
            /// @author Ian Copland
            //----------------------------------------------------
            EmailComposer();
            //------------------------------------------------------
            /// Called when the owning state is initialised.
            ///
            /// @author Ian Copland
            //------------------------------------------------------
            void OnInit() override;
            //----------------------------------------------------
            /// Cleans up the email composer and sets it back to
            /// a state where it is ready to be presented again.
            ///
            /// @author Ian Copland
            //----------------------------------------------------
            void Cleanup();
            //------------------------------------------------------
            /// Called when the owning state is destroyed.
            ///
            /// @author Ian Copland
            //------------------------------------------------------
            void OnDestroy() override;
            
            SendResultDelegate m_resultDelegate;
			EmailComposerDelegate* m_emailComposerDelegate;
			MFMailComposeViewController* m_viewController;
            UIViewController* m_rootViewController;
            bool m_isPresented = false;
		};
		 
	}
}

#endif
