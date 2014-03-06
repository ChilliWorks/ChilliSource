/*
 *  SMSCompositionActivity.h
 *  moFlow
 *
 *  Created by Steven Hendrie on 20/01/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */
#ifndef _MOFLO_PLATFORM_ANDROID_SOCIAL_COMMUNICATIONS_SMSCOMPOSITIONSCREEN_H_
#define _MOFLO_PLATFORM_ANDROID_SOCIAL_COMMUNICATIONS_SMSCOMPOSITIONSCREEN_H_

#include <ChilliSource/Social/Communications/SMSCompositionActivity.h>

namespace ChilliSource
{
	namespace Android
	{
		class SMSCompositionActivity: public Social::SMSCompositionActivity
		{
		public:
			virtual bool IsA(Core::InterfaceIDType inID) const override;

			static bool SupportedByDevice();

			void Present(const std::vector<Core::UTF8String> & inastrRecipientNumberss, //Vector of email addresses
								const Core::UTF8String & instrContents, //Content as HTML
								const SendResultDelegate & inCallback) override;

			void Dismiss() override;

			static void OnSMSClosed();

		private:

			friend Social::SMSCompositionActivityUPtr Social::SMSCompositionActivity::Create();
            //----------------------------------------------------
            /// Private constructor to force the use of the
            /// factory method.
            ///
            /// @author I Copland
            //----------------------------------------------------
            SMSCompositionActivity(){}

		private:

			static SendResultDelegate mCallback;
		};
	}
}
#endif
