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
#include <ChilliSource/Backend/Platform/iOS/ForwardDeclarations.h>
#include <ChilliSource/Social/Communications/SMSCompositionActivity.h>

#include <MessageUI/MessageUI.h>

@class SMSDelegate;

namespace ChilliSource{
	namespace iOS {
		
		class SMSCompositionActivity : public Social::SMSCompositionActivity 
		{
		public:
			virtual bool IsA(Core::InterfaceIDType inID) const override;
			
			static bool SupportedByDevice(); //You must always check this method before constructing an instance of the class or doom will ensue.
			
			~SMSCompositionActivity();
			
			virtual void Present(const std::vector<Core::UTF8String> & inastrRecipientNumbers, const Core::UTF8String & instrContents, const SMSCompositionActivity::SendResultDelegate & inCallback) override;

			virtual void Dismiss() override;
		private:
            //----------------------------------------------------
            /// Private constructor to force the use of the
            /// factory method.
            ///
            /// @author I Copland
            //----------------------------------------------------
            SMSCompositionActivity();
            
            friend Social::SMSCompositionActivityUPtr Social::SMSCompositionActivity::Create();
            
			SMSDelegate * mpDelegate;
			MFMessageComposeViewController * mpVC;
		};
		
	}
}

@interface  SMSDelegate : NSObject<MFMessageComposeViewControllerDelegate>
{
	ChilliSource::Social::SMSCompositionActivity::SendResultDelegate mCallback;
}
-(SMSDelegate*) initWithCallback:(ChilliSource::Social::SMSCompositionActivity::SendResultDelegate)callback;

	
@end

#endif