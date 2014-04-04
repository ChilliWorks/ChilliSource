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
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/File/FileSystem.h>
#include <ChilliSource/Core/String/StringUtils.h>

namespace ChilliSource
{
	namespace iOS
    {
        //-------------------------------------------------------
        /// Constructor
        //-------------------------------------------------------
        EmailCompositionActivity::EmailCompositionActivity()
            :mpDelegate(nil)
		{
		}
        //-------------------------------------------------------
        /// Supported By Device
        //-------------------------------------------------------
		bool EmailCompositionActivity::SupportedByDevice()
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
        bool EmailCompositionActivity::IsSupportedByDevice() const
        {
            return SupportedByDevice();
        }
        //------------------------------------------------------
        /// Is A
        //------------------------------------------------------
		bool EmailCompositionActivity::IsA(Core::InterfaceIDType inID) const
        {
			return inID == EmailCompositionActivity::InterfaceID;
		}
        //-------------------------------------------------------
        /// Present
        //-------------------------------------------------------
		void EmailCompositionActivity::Present(const std::vector<Core::UTF8String>& inastrRecipientAddresses, const Core::UTF8String& instrSubject, const Core::UTF8String& instrContents, const SendResultDelegate& inCallback, bool inbFormatAsHtml)
        {
            Attachment emptyAttachment;
            emptyAttachment.m_filename = "";
            emptyAttachment.m_storageLocation = Core::StorageLocation::k_none;
            emptyAttachment.m_mimeType = "";
			PresentWithAttachment(inastrRecipientAddresses, instrSubject, instrContents, emptyAttachment, inCallback, inbFormatAsHtml);
		}
        //-------------------------------------------------------
        /// Present With Attachments
        //-------------------------------------------------------
		void EmailCompositionActivity::PresentWithAttachment(const std::vector<Core::UTF8String> & inastrRecipientAddresses, const Core::UTF8String & instrSubject, const Core::UTF8String & instrContents, const Attachment& inAttachment, const SendResultDelegate & inCallback, bool inbFormatAsHtml)
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
				[pNamesArray addObject:Core::StringUtils::UTF8StringToNSString(inastrRecipientAddresses[nRecipient])];
			}
			[mpVC setToRecipients:pNamesArray];
			[mpVC setMessageBody: Core::StringUtils::UTF8StringToNSString(instrContents) isHTML:inbFormatAsHtml];
			[mpVC setSubject: Core::StringUtils::UTF8StringToNSString(instrSubject)];
            
            //add the attachment if one is available.
            if (inAttachment.m_filename.size() > 0)
            {
                std::string strFilename;
				if (inAttachment.m_storageLocation == Core::StorageLocation::k_package)
				{
					strFilename = Core::Application::Get()->GetFileSystem()->GetAbsolutePathToFile(Core::StorageLocation::k_package, inAttachment.m_filename);
				}
				else if (inAttachment.m_storageLocation == Core::StorageLocation::k_DLC && Core::Application::Get()->GetFileSystem()->DoesFileExistInCachedDLC(inAttachment.m_filename) == false)
                {
                    strFilename = Core::Application::Get()->GetFileSystem()->GetPackageDLCPath() + inAttachment.m_filename;
                }
                else
				{
					strFilename = Core::Application::Get()->GetFileSystem()->GetAbsolutePathToStorageLocation(inAttachment.m_storageLocation) + inAttachment.m_filename;
				}
                
                std::string strPath, strBasename;
                Core::StringUtils::SplitFilename(inAttachment.m_filename, strBasename, strPath);
                NSData* pData = [NSData dataWithContentsOfFile: [NSString stringWithUTF8String:strFilename.c_str()]];
                [mpVC addAttachmentData:pData mimeType:[NSString stringWithUTF8String:inAttachment.m_mimeType.c_str()] fileName:[NSString stringWithUTF8String:strBasename.c_str()]];
            }
             
			[pRootVC presentModalViewController:mpVC animated:YES];
			
			[mpVC release];
			[pNamesArray release];
        }
        //-------------------------------------------------------
        /// Dismiss
        //-------------------------------------------------------
        void EmailCompositionActivity::Dismiss()
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
        EmailCompositionActivity::~EmailCompositionActivity()
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
	

