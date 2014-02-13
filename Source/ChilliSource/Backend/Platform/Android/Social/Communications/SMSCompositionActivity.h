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

namespace moFlo
{
	namespace AndroidPlatform
	{

		class CSMSCompositionActivity: public moFlo::Social::ISMSCompositionActivity
		{
		public:
			virtual bool IsA(Core::InterfaceIDType inID) const;

			static bool SupportedByDevice();

			CSMSCompositionActivity();
			~CSMSCompositionActivity();

			virtual void Present(const std::vector<CUTF8String> & inastrRecipientNumberss, //Vector of email addresses
								const CUTF8String & instrContents, //Content as HTML
								const SendResultDelegate & inCallback);

			virtual void Dismiss();

			static void OnSMSClosed();
		private:
			static SendResultDelegate mCallback;
		};

	}
}
#endif
