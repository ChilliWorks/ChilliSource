/*
 *  EmailCompositionActivity.h
 *  moFlow
 *
 *  Created by Steven Hendrie on 20/01/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

#ifndef MOFLOW_PLATFORM_ANDROID_SOCIAL_COMMUNICATIONS_EMAILCOMPOSITIONSCREEN_H
#define MOFLOW_PLATFORM_ANDROID_SOCIAL_COMMUNICATIONS_EMAILCOMPOSITIONSCREEN_H

#include <ChilliSource/Platform/Android/ForwardDeclarations.h>
#include <ChilliSource/Social/Communications/EmailCompositionActivity.h>

namespace moFlo
{
	namespace AndroidPlatform
	{
		class CEmailCompositionActivity: public moFlo::Social::IEmailCompositionActivity
		{
		public:
			//-------------------------------------------------------
			/// Constructor
			//-------------------------------------------------------
			CEmailCompositionActivity();
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
			void Present(const std::vector<CUTF8String> & inastrRecipientAddresses, const CUTF8String & instrSubject, const CUTF8String & instrContents, const SendResultDelegate & inCallback, bool inbFormatAsHtml);
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
			void PresentWithAttachment(const DYNAMIC_ARRAY<CUTF8String> & inastrRecipientAddresses, const CUTF8String & instrSubject, const CUTF8String & instrContents, const Attachment& inAttachment, const SendResultDelegate & inCallback, bool inbFormatAsHtml);
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
            //-------------------------------------------------------
            /// On Email Closed
            ///
            /// Called when the email activity returns with a result.
			///
			/// @param The result code.
            //-------------------------------------------------------
			void OnEmailClosed(s32 indwResultCode);

			EmailCompositionJavaInterfacePtr mpJavaInterface;
			SendResultDelegate mCallback;
		};

	}
}
#endif
