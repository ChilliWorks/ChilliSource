/*
 *  EmailCompositionActivity.h
 *  moFlow
 *
 *  Created by Stuart McGaw on 09/06/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#ifndef MOFLOW_PLATFORM_IOS_SOCIAL_COMMUNICATIONS_EMAILCOMPOSITIONSCREEN_H
#define MOFLOW_PLATFORM_IOS_SOCIAL_COMMUNICATIONS_EMAILCOMPOSITIONSCREEN_H

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Social/Communications/EmailCompositionActivity.h>

#include <MessageUI/MessageUI.h>

@class EmailDelegate;

namespace ChilliSource
{
	namespace iOS
    {
        class CEmailCompositionActivity : public Social::IEmailCompositionActivity
		{
		public:
            //-------------------------------------------------------
            /// Constructor
            //-------------------------------------------------------
            CEmailCompositionActivity();
            //-------------------------------------------------------
            /// Supported By Device
            ///
            /// This checks whether or not the current iOS device
            /// supports email. This should always be checked before
            /// creating an instance of the class.
            //-------------------------------------------------------
            static bool SupportedByDevice();
            //-------------------------------------------------------
            /// Is Supported By Device
            ///
            /// @return whether email is supported on the current
            /// device
            //-------------------------------------------------------
            bool IsSupportedByDevice() const;
            //-------------------------------------------------------
			/// Is A
			///
			/// Is the object of the given interface type
            ///
			/// @param Interface type to query
			/// @return Whether the object is of given type
			//-------------------------------------------------------
			bool IsA(Core::InterfaceIDType inID) const;
            //-------------------------------------------------------
            /// Present
            ///
            /// Displays the email activity with the given recipients,
            /// subject and contents.
            ///
            /// @param A list of recipients.
            /// @param The subject.
            /// @param The body of the email.
            /// @param The callback describing the result of the email.
            /// @param Whether or not to format as HTML.
            //-------------------------------------------------------
			void Present(const DYNAMIC_ARRAY<Core::CUTF8String>& inastrRecipientAddresses, const Core::CUTF8String& instrSubject, const Core::CUTF8String& instrContents, const SendResultDelegate& inCallback, bool inbFormatAsHtml);
            //-------------------------------------------------------
            /// Present With Attachment
            ///
            /// Displays the email activity with the given recipients,
            /// subject and contents, and adds a list of attachments
            /// to the email.
            ///
            /// @param A list of recipients.
            /// @param The subject.
            /// @param The body of the email.
            /// @param The attachment.
            /// @param The callback describing the result of the email.
            /// @param Whether or not to format as HTML.
            //-------------------------------------------------------
            void PresentWithAttachment(const DYNAMIC_ARRAY<Core::CUTF8String> & inastrRecipientAddresses, const Core::CUTF8String & instrSubject, const Core::CUTF8String & instrContents, const Attachment& inAttachment, const SendResultDelegate & inCallback, bool inbFormatAsHtml);
            //-------------------------------------------------------
            /// Dismiss
            ///
            /// Dismisses the activity if it is currently displayed.
            //-------------------------------------------------------
			void Dismiss();
            //-------------------------------------------------------
            /// Destructor
            //-------------------------------------------------------
            virtual ~CEmailCompositionActivity();
		private:
			EmailDelegate* mpDelegate;
			MFMailComposeViewController * mpVC;
		};
		 
	}
}

@interface EmailDelegate : NSObject<MFMailComposeViewControllerDelegate>
{
	ChilliSource::Social::IEmailCompositionActivity::SendResultDelegate mCallback;
}
//-------------------------------------------------------
/// Init With Callback
///
/// @param The send result callback.
//-------------------------------------------------------
-(EmailDelegate*) initWithCallback:(ChilliSource::Social::IEmailCompositionActivity::SendResultDelegate)callback;

@end


#endif