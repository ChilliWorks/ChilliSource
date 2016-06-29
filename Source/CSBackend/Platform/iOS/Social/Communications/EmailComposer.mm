//
//  EmailComposer.mm
//  Chilli Source
//  Created by Stuart McGaw on 09/06/2011.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2011 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#ifdef CS_TARGETPLATFORM_IOS

#import <CSBackend/Platform/iOS/Social/Communications/EmailComposer.h>

#import <CSBackend/Platform/iOS/Core/String/NSStringUtils.h>
#import <CSBackend/Platform/iOS/Social/Communications/EmailComposerDelegate.h>
#import <ChilliSource/Core/Base/Application.h>
#import <ChilliSource/Core/File/FileSystem.h>
#import <ChilliSource/Core/String/StringUtils.h>
#import <ChilliSource/Core/Threading/TaskScheduler.h>

namespace CSBackend
{
	namespace iOS
    {
        CS_DEFINE_NAMEDTYPE(EmailComposer);
        //-------------------------------------------------------
        //-------------------------------------------------------
        EmailComposer::EmailComposer()
            : m_currentState(EmailComposerState::k_inactive), m_emailComposerDelegate(nil), m_viewController(nil), m_rootViewController(nil)
		{
		}
        //------------------------------------------------------
        //------------------------------------------------------
		bool EmailComposer::IsA(ChilliSource::InterfaceIDType in_interfaceId) const
        {
			return (in_interfaceId == ChilliSource::EmailComposer::InterfaceID || in_interfaceId == EmailComposer::InterfaceID);
		}
        //-------------------------------------------------------
        //-------------------------------------------------------
		void EmailComposer::Present(const std::vector<std::string>& in_recipientAddresses, const std::string& in_subject, const std::string& in_contents, ContentFormat in_contentFormat,
                                    const SendResultDelegate& in_callback)
        {
            Attachment emptyAttachment;
            emptyAttachment.m_storageLocation = ChilliSource::StorageLocation::k_none;
			PresentWithAttachment(in_recipientAddresses, in_subject, in_contents, in_contentFormat, emptyAttachment, in_callback);
		}
        //-------------------------------------------------------
        //-------------------------------------------------------
		void EmailComposer::PresentWithAttachment(const std::vector<std::string>& in_recipientAddresses, const std::string& in_subject, const std::string& in_contents, ContentFormat in_contentFormat,
                                                  const Attachment& in_attachment, const SendResultDelegate& in_callback)
        {
            @autoreleasepool
            {
                CS_ASSERT(m_currentState == EmailComposerState::k_inactive, "Cannot present the email composer when it is already presented, or while dismissing.");
                CS_ASSERT(ChilliSource::Application::Get()->GetTaskScheduler()->IsMainThread(), "Tried to present Email Composer outside of main thread.");
                
                m_currentState = EmailComposerState::k_presented;
                
                ChilliSource::Application::Get()->GetTaskScheduler()->ScheduleTask(ChilliSource::TaskType::k_system, [=](const ChilliSource::TaskContext& taskContext)
                {
                    m_resultDelegate = in_callback;
                
                    m_viewController = [[MFMailComposeViewController alloc] init];
                    if ([MFMailComposeViewController canSendMail] == false)
                    {
                        [m_viewController release];
                        in_callback(SendResult::k_failed);
                        return;
                    }
                
                    m_viewController.mailComposeDelegate = m_emailComposerDelegate;
                
                    NSMutableArray * pNamesArray = [[NSMutableArray alloc] initWithCapacity:in_recipientAddresses.size()];
                
                    for (u32 nRecipient = 0; nRecipient < in_recipientAddresses.size(); nRecipient++)
                    {
                        NSString* address = [NSStringUtils newNSStringWithUTF8String:in_recipientAddresses[nRecipient]];
                        [pNamesArray addObject:address];
                        [address release];
                    }
                    [m_viewController setToRecipients:pNamesArray];
                
                    NSString* body = [NSStringUtils newNSStringWithUTF8String:in_contents];
                    NSString* subject = [NSStringUtils newNSStringWithUTF8String:in_subject];
                
                    [m_viewController setMessageBody:body isHTML:(in_contentFormat == ContentFormat::k_html)];
                    [m_viewController setSubject:subject];
                
                    [body release];
                    [subject release];
                
                    //add the attachment if one is available.
                    if (in_attachment.m_filename.size() > 0)
                    {
                        auto fileSystem = ChilliSource::Application::Get()->GetFileSystem();

                        std::string strFilename;
                        if (in_attachment.m_storageLocation == ChilliSource::StorageLocation::k_DLC && fileSystem->DoesFileExistInCachedDLC(in_attachment.m_filename) == false)
                        {
                            strFilename = fileSystem->GetAbsolutePathToStorageLocation(ChilliSource::StorageLocation::k_package) + fileSystem->GetPackageDLCPath() + in_attachment.m_filename;
                        }
                        else
                        {
                            strFilename = fileSystem->GetAbsolutePathToStorageLocation(in_attachment.m_storageLocation) + in_attachment.m_filename;
                        }
                    
                        std::string strPath, strBasename;
                        ChilliSource::StringUtils::SplitFilename(in_attachment.m_filename, strBasename, strPath);
                        NSData* pData = [NSData dataWithContentsOfFile: [NSString stringWithUTF8String:strFilename.c_str()]];
                        [m_viewController addAttachmentData:pData mimeType:[NSString stringWithUTF8String:in_attachment.m_mimeType.c_str()] fileName:[NSString stringWithUTF8String:strBasename.c_str()]];
                    }
                    
                    m_rootViewController = [UIApplication sharedApplication].keyWindow.rootViewController;
                    [m_rootViewController presentViewController:m_viewController animated:YES completion:nil];
                
                    [pNamesArray release];
                });
            }
        }
        //----------------------------------------------------
        //----------------------------------------------------
        void EmailComposer::Cleanup()
        {
            m_viewController.mailComposeDelegate = nil;
            
            [m_rootViewController dismissViewControllerAnimated:YES completion:nil];
            m_rootViewController = nil;
            
            [m_viewController release];
            m_viewController = nil;
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        bool EmailComposer::IsPresented()
        {
            return (m_currentState == EmailComposerState::k_presented);
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        void EmailComposer::Dismiss()
        {
            @autoreleasepool
            {
                CS_ASSERT(ChilliSource::Application::Get()->GetTaskScheduler()->IsMainThread(), "Tried to dismiss Email Composer outside of main thread.");
                CS_ASSERT(IsPresented(), "Cannot dismiss non-presented email composer.");
                
                m_currentState = EmailComposerState::k_dismissing;
                
                ChilliSource::Application::Get()->GetTaskScheduler()->ScheduleTask(ChilliSource::TaskType::k_system, [=](const ChilliSource::TaskContext& tastContext)
                {
                    Cleanup();
                });
                
                ChilliSource::Application::Get()->GetTaskScheduler()->ScheduleTask(ChilliSource::TaskType::k_system, [=](const ChilliSource::TaskContext& taskContext)
                {
                    if (m_resultDelegate != nullptr)
                    {
                        SendResultDelegate delegate = m_resultDelegate;
                        m_resultDelegate = nullptr;
                        delegate(SendResult::k_cancelled);
                    }
                    
                    m_currentState = EmailComposerState::k_inactive;
                });
            }
		}
        //-------------------------------------------------------
        //-------------------------------------------------------
        void EmailComposer::OnResult(MFMailComposeViewController* in_viewController, MFMailComposeResult in_result)
        {
            Cleanup();
            
            ChilliSource::Application::Get()->GetTaskScheduler()->ScheduleTask(ChilliSource::TaskType::k_mainThread, [=](const ChilliSource::TaskContext& taskContext)
            {
                CS_ASSERT(IsPresented(), "Received result while email composer is not presented.");
            
                SendResult sendResult = SendResult::k_failed;
                switch (in_result)
                {
                    case MFMailComposeResultSaved:
                    case MFMailComposeResultSent:
                        sendResult = SendResult::k_succeed;
                        break;
                    case MFMailComposeResultCancelled:
                        sendResult = SendResult::k_cancelled;
                        break;
                    default:
                        sendResult = SendResult::k_failed;
                        break;
                }
            
                if (m_resultDelegate != nullptr)
                {
                    SendResultDelegate delegate = m_resultDelegate;
                    m_resultDelegate = nullptr;
                    delegate(SendResult::k_cancelled);
                }
            });
        }
        //------------------------------------------------------
        //------------------------------------------------------
        void EmailComposer::OnInit()
        {
            m_emailComposerDelegate = [[EmailComposerDelegate alloc] initWithSystem:this];
        }
        //------------------------------------------------------
        //------------------------------------------------------
        void EmailComposer::OnDestroy()
        {
            if(IsPresented())
            {
                Dismiss();
            }
            
            [m_emailComposerDelegate release];
            m_emailComposerDelegate = nil;
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
		bool EmailComposer::IsSupportedByDevice()
        {
            @autoreleasepool
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
		}
	}
}

#endif