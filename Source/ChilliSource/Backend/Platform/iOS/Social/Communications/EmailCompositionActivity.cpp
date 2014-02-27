/*
 *  EmailCompositionActivity.cpp
 *  moFlow
 *
 *  Created by Stuart McGaw on 09/06/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Backend/Platform/iOS/Social/Communications/EmailCompositionActivity.h>
#include <ChilliSource/Backend/Platform/iOS/Core/Base/EAGLView.h>
#include <ChilliSource/Core/String/StringUtils.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/File/FileSystem.h>

namespace ChilliSource
{
	namespace iOS
    {
        //-------------------------------------------------------
        /// Constructor
        //-------------------------------------------------------
        CEmailCompositionActivity::CEmailCompositionActivity()
            :mpDelegate(nil)
		{
		}
        //-------------------------------------------------------
        /// Supported By Device
        //-------------------------------------------------------
		bool CEmailCompositionActivity::SupportedByDevice()
        {
			NSString *reqSysVer = @"3.0";
			NSString *currSysVer = [[UIDevice currentDevice] systemVersion];
			bool osVersionSupported = ([currSysVer compare:reqSysVer options:NSNumericSearch] != NSOrderedAscending);
			if (osVersionSupported)
            {
				return [MFMailComposeViewController canSendMail];
			}
			
			return false;
		}
        //-------------------------------------------------------
        /// Is Supported By Device
        ///
        /// @return whether email is supported on the current
        /// device
        //-------------------------------------------------------
        bool CEmailCompositionActivity::IsSupportedByDevice() const
        {
            return SupportedByDevice();
        }
        //------------------------------------------------------
        /// Is A
        //------------------------------------------------------
		bool CEmailCompositionActivity::IsA(Core::InterfaceIDType inID) const
        {
			return inID == CEmailCompositionActivity::InterfaceID;
		}
        //-------------------------------------------------------
        /// Present
        //-------------------------------------------------------
		void CEmailCompositionActivity::Present(const std::vector<Core::UTF8String>& inastrRecipientAddresses, const Core::UTF8String& instrSubject, const Core::UTF8String& instrContents, const SendResultDelegate& inCallback, bool inbFormatAsHtml)
        {
            Attachment emptyAttachment;
            emptyAttachment.mstrFilename = "";
            emptyAttachment.meStorageLocation = Core::StorageLocation::k_none;
            emptyAttachment.mstrMIMEType = "";
			PresentWithAttachment(inastrRecipientAddresses, instrSubject, instrContents, emptyAttachment, inCallback, inbFormatAsHtml);
		}
        //-------------------------------------------------------
        /// Present With Attachments
        //-------------------------------------------------------
		void CEmailCompositionActivity::PresentWithAttachment(const std::vector<Core::UTF8String> & inastrRecipientAddresses, const Core::UTF8String & instrSubject, const Core::UTF8String & instrContents, const Attachment& inAttachment, const SendResultDelegate & inCallback, bool inbFormatAsHtml)
        {
			mpVC = [[MFMailComposeViewController alloc] init];
            if ([MFMailComposeViewController canSendMail] == false)
            {
                [mpVC release];
                inCallback(SendResult::k_failed);
                return;
            }
            
            mpDelegate = [[EmailDelegate alloc] initWithCallback:inCallback];
			
			UIViewController * pRootVC = [EAGLView sharedInstance].viewController;
            
			mpVC.mailComposeDelegate = mpDelegate;
			
			NSMutableArray * pNamesArray = [[NSMutableArray alloc] initWithCapacity:inastrRecipientAddresses.size()];
			
			for (u32 nRecipient = 0; nRecipient < inastrRecipientAddresses.size(); nRecipient++){
				[pNamesArray addObject:Core::CStringUtils::UTF8StringToNSString(inastrRecipientAddresses[nRecipient])];
			}
			[mpVC setToRecipients:pNamesArray];
			[mpVC setMessageBody: Core::CStringUtils::UTF8StringToNSString(instrContents) isHTML:inbFormatAsHtml];
			[mpVC setSubject: Core::CStringUtils::UTF8StringToNSString(instrSubject)];
            
            //add the attachment if one is available.
            if (inAttachment.mstrFilename.size() > 0)
            {
                std::string strFilename;
				if (inAttachment.meStorageLocation == Core::StorageLocation::k_package)
				{
					strFilename = Core::Application::GetFileSystemPtr()->GetDirectoryForPackageFile(inAttachment.mstrFilename);
				}
				else if (inAttachment.meStorageLocation == Core::StorageLocation::k_DLC && Core::Application::GetFileSystemPtr()->DoesFileExistInCachedDLC(inAttachment.mstrFilename) == false)
                {
                    strFilename = Core::Application::GetFileSystemPtr()->GetPackageDLCDirectory() + inAttachment.mstrFilename;
                }
                else
				{
					strFilename = Core::Application::GetFileSystemPtr()->GetStorageLocationDirectory(inAttachment.meStorageLocation) + inAttachment.mstrFilename;
				}
                
                std::string strPath, strBasename;
                Core::CStringUtils::SplitFilename(inAttachment.mstrFilename, strBasename, strPath);
                NSData* pData = [NSData dataWithContentsOfFile: [NSString stringWithUTF8String:strFilename.c_str()]];
                [mpVC addAttachmentData:pData mimeType:[NSString stringWithUTF8String:inAttachment.mstrMIMEType.c_str()] fileName:[NSString stringWithUTF8String:strBasename.c_str()]];
            }
             
			[pRootVC presentModalViewController:mpVC animated:YES];
			
			[mpVC release];
			[pNamesArray release];
        }
        //-------------------------------------------------------
        /// Dismiss
        //-------------------------------------------------------
        void CEmailCompositionActivity::Dismiss()
        {
			[mpDelegate release];
			mpVC.mailComposeDelegate = nil;
            if(mpVC.parentViewController)
            {
                [mpVC.parentViewController dismissModalViewControllerAnimated:YES];
            }
            else if([mpVC presentingViewController])
            {
                [[mpVC presentingViewController] dismissModalViewControllerAnimated:YES];
            }
		}
        //-------------------------------------------------------
        /// Destructor
        //-------------------------------------------------------
        CEmailCompositionActivity::~CEmailCompositionActivity()
        {
			[mpDelegate release];
			mpDelegate = nil;
		}
	}
}

@implementation EmailDelegate
//-------------------------------------------------------
/// Init With Callback
//-------------------------------------------------------
-(EmailDelegate*) initWithCallback:(ChilliSource::Social::EmailCompositionActivity::SendResultDelegate)callback{
	
	if ((self = [super init])){
		
		mCallback = callback;
		
	}
	
	return self;
}
//-------------------------------------------------------
/// Main Compose Controller
///
/// @param The controller.
/// @param The result.
/// @param The error code.
//-------------------------------------------------------
- (void)mailComposeController:(MFMailComposeViewController *)controller didFinishWithResult:(MFMailComposeResult)result  error:(NSError*)error{
	
	using namespace ChilliSource::Social;
	
    if(controller.parentViewController)
    {
        [controller.parentViewController dismissModalViewControllerAnimated:YES];
    }
    else if([controller presentingViewController])
    {
        [[controller presentingViewController] dismissModalViewControllerAnimated:YES];
    }
    
	EmailCompositionActivity::SendResult eResult;
	
    if(error == nil)
    {
		switch (result)
        {
			case MFMailComposeResultSaved:
			case MFMailComposeResultSent:
				eResult = EmailCompositionActivity::SendResult::k_succeed;
				break;
			case MFMailComposeResultCancelled:
				eResult = EmailCompositionActivity::SendResult::k_cancelled;
				break;
			case MFMailComposeResultFailed:
				eResult = EmailCompositionActivity::SendResult::k_failed;
				break;
		}
	
		if (mCallback)
        {
			mCallback(eResult);
		}
	}
}

@end
	

