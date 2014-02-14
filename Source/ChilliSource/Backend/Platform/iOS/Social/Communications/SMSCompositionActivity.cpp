/*
 *  SMSCompositionActivity.cpp
 *  moFlow
 *
 *  Created by Stuart McGaw on 09/06/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Platform/iOS/Social/Communications/SMSCompositionActivity.h>
#include <ChilliSource/Core/String/StringUtils.h>
#include <ChilliSource/Backend/Platform/iOS/EAGLView.h>

namespace moFlo{
	namespace iOSPlatform {
		bool CSMSCompositionActivity::IsA(Core::InterfaceIDType inID) const{
			return inID == CSMSCompositionActivity::InterfaceID;
		}
		bool CSMSCompositionActivity::SupportedByDevice(){
			
			NSString *reqSysVer = @"4.0";
			NSString *currSysVer = [[UIDevice currentDevice] systemVersion];
			bool osVersionSupported = ([currSysVer compare:reqSysVer options:NSNumericSearch] != NSOrderedAscending);
			if (osVersionSupported){
				
				return [MFMessageComposeViewController canSendText];
				
			}
			
			return false;
		}
		
		CSMSCompositionActivity::CSMSCompositionActivity()
		:mpDelegate(nil)
		{
			
		}
		CSMSCompositionActivity::~CSMSCompositionActivity(){
			[mpDelegate release];
			mpDelegate = nil;
		}
		void CSMSCompositionActivity::Present(const DYNAMIC_ARRAY<CUTF8String> & inastrRecipientNumbers, const CUTF8String & instrContents, const ISMSCompositionActivity::SendResultDelegate & inCallback){
			
			mpDelegate = [[SMSDelegate alloc] initWithCallback:inCallback];
						
			UIViewController * pRootVC = [EAGLView sharedInstance].viewController;
			
			mpVC = [[MFMessageComposeViewController alloc] init];
			mpVC.messageComposeDelegate = mpDelegate;
			
			NSMutableArray * pNamesArray = [[NSMutableArray alloc] initWithCapacity:inastrRecipientNumbers.size()];
			
			for (u32 nRecipient = 0; nRecipient < inastrRecipientNumbers.size(); nRecipient++){			
				[pNamesArray addObject:Core::CStringUtils::UTF8StringToNSString(inastrRecipientNumbers[nRecipient])];
			}
			mpVC.recipients = pNamesArray;
			
			mpVC.body = Core::CStringUtils::UTF8StringToNSString(instrContents);
			[pRootVC presentModalViewController:mpVC animated:YES];
			
			[mpVC release];
			[pNamesArray release];

		}
		void CSMSCompositionActivity::Dismiss()
        {
			[mpDelegate release];
			mpVC.messageComposeDelegate = nil;
            
            if(mpVC.parentViewController)
            {
                [mpVC.parentViewController dismissModalViewControllerAnimated:YES];
            }
            else if([mpVC presentingViewController])
            {
                [[mpVC presentingViewController] dismissModalViewControllerAnimated:YES];
            }
		}

	}
}

@implementation SMSDelegate

-(SMSDelegate*) initWithCallback:(moFlo::Social::ISMSCompositionActivity::SendResultDelegate)callback{
	
	if ((self = [super init])){
	
		mCallback = callback;
		
	}
	
	return self;
}

- (void)messageComposeViewController:(MFMessageComposeViewController *)controller didFinishWithResult:(MessageComposeResult)result 
{
	using namespace moFlo::Social;
    
    if(controller.parentViewController)
    {
        [controller.parentViewController dismissModalViewControllerAnimated:YES];
    }
    else if([controller presentingViewController])
    {
        [[controller presentingViewController] dismissModalViewControllerAnimated:YES];
    }
    
	ISMSCompositionActivity::SendResult eResult;
	switch (result) {
		case MessageComposeResultSent:
			eResult = ISMSCompositionActivity::SR_SUCCEED;
			break;
		case MessageComposeResultCancelled:
			eResult = ISMSCompositionActivity::SR_CANCELLED;
			break;
		case MessageComposeResultFailed:
			eResult = ISMSCompositionActivity::SR_FAILED;
	}
	
	if (mCallback){
		mCallback(eResult);
	}
}

@end