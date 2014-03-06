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
#include <ChilliSource/Core/Event/Event.h>
#include <ChilliSource/Core/String/UTF8String.h>

#include <vector>

namespace ChilliSource 
{
	namespace Social 
	{
        class EmailCompositionActivity : public Core::Activity
        {
		public:
			CS_DECLARE_NAMEDTYPE(EmailCompositionActivity);
			//-------------------------------------------------------
            /// An enum describing the result of the email intent.
            //-------------------------------------------------------
			enum class SendResult
            {
                k_succeed,
                k_failed,
                k_cancelled
			};
			typedef std::function<void(SendResult)> SendResultDelegate;
            //-------------------------------------------------------
            /// A container for information describing a single
            /// attachment file.
            //-------------------------------------------------------
            struct Attachment
            {
                Core::StorageLocation m_storageLocation;
                std::string m_filename;
                std::string m_mimeType;
            };
            //-------------------------------------------------------
            /// Creates a new instance of the email composition
            /// activity.
            ///
            /// @author I Copland
            ///
            /// @return the new activity instance.
            //-------------------------------------------------------
            static EmailCompositionActivityUPtr Create();
			//-------------------------------------------------------
            /// Displays the email activity with the given recipients,
            /// subject and contents.
            ///
            /// @param A list of recipients.
            /// @param The subject.
            /// @param The body of the email.
            /// @param The callback describing the result of the email.
            /// @param Whether or not to format as HTML.
            //-------------------------------------------------------
			virtual void Present(const std::vector<Core::UTF8String> & in_recipientAddresses, const Core::UTF8String & in_subject, const Core::UTF8String & in_contents, const SendResultDelegate & in_callback, bool in_formatAsHtml) = 0;
            //-------------------------------------------------------
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
            virtual void PresentWithAttachment(const std::vector<Core::UTF8String> & in_recipientAddresses, const Core::UTF8String & in_subject, const Core::UTF8String & in_contents, const Attachment& in_attachment, const SendResultDelegate & in_callback, bool in_formatAsHtml) = 0;
            //-------------------------------------------------------
			/// Dismisses the activity if it is currently displayed.
			//-------------------------------------------------------
			virtual void Dismiss() = 0;
            //-------------------------------------------------------
            /// @return whether email is supported on the current
            /// device
            //-------------------------------------------------------
            virtual bool IsSupportedByDevice() const = 0;
			//-------------------------------------------------------
			/// Destructor
			//-------------------------------------------------------
			virtual ~EmailCompositionActivity(){};
        protected:
            //-------------------------------------------------------
			/// Protected constructor to force use of the factory
            /// method.
            ///
            /// @author I Copland.
			//-------------------------------------------------------
            EmailCompositionActivity(){};
		};
		
	}
}

#endif
