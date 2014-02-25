/*
 *  EmailCompositionActivity.h
 *  moFlow
 *
 *  Created by Stuart McGaw on 08/06/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLO_SOCIAL_COMMUNICATIONS_EMAILCOMPOSITIONACTIVITY_H_
#define _MOFLO_SOCIAL_COMMUNICATIONS_EMAILCOMPOSITIONACTIVITY_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/Activity.h>
#include <ChilliSource/Core/String/UTF8String.h>
#include <ChilliSource/Core/Event/GenericEvent.h>

#include <vector>

namespace ChilliSource 
{
	namespace Social 
	{
        class IEmailCompositionActivity : public Core::IActivity
        {
		public:
			DECLARE_NAMED_INTERFACE(IEmailCompositionActivity);
			//-------------------------------------------------------
            /// Send Result
            ///
            /// An enum describing the result of the email intent.
            //-------------------------------------------------------
			enum class SendResult
            {
                k_succeed,
                k_failed,
                k_cancelled
			};
			typedef fastdelegate::FastDelegate1<SendResult> SendResultDelegate;
            //-------------------------------------------------------
            /// Attachment
            ///
            /// A container for information describing a single
            /// attachment file.
            //-------------------------------------------------------
            struct Attachment
            {
                Core::StorageLocation meStorageLocation;
                std::string mstrFilename;
                std::string mstrMIMEType;
            };
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
			virtual void Present(const DYNAMIC_ARRAY<Core::CUTF8String> & inastrRecipientAddresses, const Core::CUTF8String & instrSubject, const Core::CUTF8String & instrContents, const SendResultDelegate & inCallback, bool inbFormatAsHtml) = 0;
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
            /// @param The attachment
            /// @param The callback describing the result of the email.
            /// @param Whether or not to format as HTML.
            //-------------------------------------------------------
            virtual void PresentWithAttachment(const DYNAMIC_ARRAY<Core::CUTF8String> & inastrRecipientAddresses, const Core::CUTF8String & instrSubject, const Core::CUTF8String & instrContents, const Attachment& inAttachment, const SendResultDelegate & inCallback, bool inbFormatAsHtml) = 0;
            //-------------------------------------------------------
			/// Dismiss
			///
			/// Dismisses the activity if it is currently displayed.
			//-------------------------------------------------------
			virtual void Dismiss() = 0;
            //-------------------------------------------------------
            /// Is Supported By Device
            ///
            /// @return whether email is supported on the current
            /// device
            //-------------------------------------------------------
            virtual bool IsSupportedByDevice() const = 0;
			//-------------------------------------------------------
			/// Destructor
			//-------------------------------------------------------
			virtual ~IEmailCompositionActivity(){};
		};
		
	}
}

#endif
