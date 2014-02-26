/*
 *  SMSCompositionActivity.h
 *  moFlow
 *
 *  Created by Stuart McGaw on 09/06/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLO_PLATFORM_IOS_SOCIAL_COMMUNICATIONS_SMSCOMPOSITIONSCREEN_H_
#define _MOFLO_PLATFORM_IOS_SOCIAL_COMMUNICATIONS_SMSCOMPOSITIONSCREEN_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Social/Communications/SMSCompositionActivity.h>

#include <MessageUI/MessageUI.h>

@class SMSDelegate;

namespace ChilliSource{
	namespace iOS {
		
		class CSMSCompositionActivity : public Social::ISMSCompositionActivity 
		{
		public:
			virtual bool IsA(Core::InterfaceIDType inID) const;
			
			static bool SupportedByDevice(); //You must always check this method before constructing an instance of the class or doom will ensue.
			
			CSMSCompositionActivity();
			~CSMSCompositionActivity();
			
			virtual void Present(const std::vector<Core::CUTF8String> & inastrRecipientNumbers, const Core::CUTF8String & instrContents, const ISMSCompositionActivity::SendResultDelegate & inCallback);

			virtual void Dismiss();
		private:
			
			SMSDelegate * mpDelegate;
			MFMessageComposeViewController * mpVC;
		};
		
	}
}

@interface  SMSDelegate : NSObject<MFMessageComposeViewControllerDelegate>
{
	ChilliSource::Social::ISMSCompositionActivity::SendResultDelegate mCallback;
}
-(SMSDelegate*) initWithCallback:(ChilliSource::Social::ISMSCompositionActivity::SendResultDelegate)callback;

	
@end

#endif