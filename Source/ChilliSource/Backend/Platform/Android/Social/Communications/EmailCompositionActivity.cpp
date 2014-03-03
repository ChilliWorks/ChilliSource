/*
 *  EmailCompositionActivity.cpp
 *  moFlow
 *
 *  Created by Steven Hendrie on 20/01/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Backend/Platform/Android/Social/Communications/EmailCompositionActivity.h>

#include <ChilliSource/Backend/Platform/Android/Core/JNI/JavaInterfaceManager.h>
#include <ChilliSource/Backend/Platform/Android/Social/Communications/EmailCompositionJavaInterface.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/MakeDelegate.h>
#include <ChilliSource/Core/File/FileSystem.h>

namespace ChilliSource
{
	namespace Android
	{
		const std::string kstrTempAttachmentDirectory = "_AttachmentTemp/";
		//-------------------------------------------------------
		/// Constructor
		//-------------------------------------------------------
		CEmailCompositionActivity::CEmailCompositionActivity()
		{
			//get the email java interface or create it if it doesn't yet exist.
			mpJavaInterface = CJavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CEmailCompositionJavaInterface>();
			if (mpJavaInterface == NULL)
			{
				mpJavaInterface = EmailCompositionJavaInterfacePtr(new CEmailCompositionJavaInterface());
				CJavaInterfaceManager::GetSingletonPtr()->AddJavaInterface(mpJavaInterface);
			}
		}
		//-------------------------------------------------------
        /// Is Supported By Device
        ///
        /// @return whether email is supported on the current
        /// device
        //-------------------------------------------------------
        bool CEmailCompositionActivity::IsSupportedByDevice() const
        {
			return true;
        }
        //-------------------------------------------------------
		/// Is A
		//-------------------------------------------------------
		bool CEmailCompositionActivity::IsA(Core::InterfaceIDType inID) const
		{
			return inID == CEmailCompositionActivity::InterfaceID;
		}
        //-------------------------------------------------------
        /// Present
        //-------------------------------------------------------
		void CEmailCompositionActivity::Present(const std::vector<Core::UTF8String> & inastrRecipientAddresses, const Core::UTF8String & instrSubject, const Core::UTF8String & instrContents, const SendResultDelegate & inCallback, bool inbFormatAsHtml)
		{
			Attachment emptyAttachment;
			emptyAttachment.mstrFilename = "";
			emptyAttachment.mstrMIMEType = "";
			emptyAttachment.meStorageLocation = Core::StorageLocation::k_none;
			PresentWithAttachment(inastrRecipientAddresses, instrSubject, instrContents, emptyAttachment, inCallback, inbFormatAsHtml);
		}
		//-------------------------------------------------------
		/// Present With Attachment
		//-------------------------------------------------------
		void CEmailCompositionActivity::PresentWithAttachment(const std::vector<Core::UTF8String> & inastrRecipientAddresses, const Core::UTF8String & instrSubject, const Core::UTF8String & instrContents, const Attachment& inAttachment, const SendResultDelegate & inCallback, bool inbFormatAsHtml)
		{
			mCallback = inCallback;

			std::string strFilename;
			if (inAttachment.mstrFilename.size() > 0)
			{
				if (inAttachment.meStorageLocation == Core::StorageLocation::k_package || (inAttachment.meStorageLocation == Core::StorageLocation::k_DLC && Core::Application::GetFileSystemPtr()->DoesFileExistInCachedDLC(inAttachment.mstrFilename) == false))
				{
					Core::Application::GetFileSystemPtr()->CreateDirectory(Core::StorageLocation::k_cache, kstrTempAttachmentDirectory);
					Core::Application::GetFileSystemPtr()->CopyFile(inAttachment.meStorageLocation, inAttachment.mstrFilename, Core::StorageLocation::k_cache, kstrTempAttachmentDirectory + inAttachment.mstrFilename);
					strFilename = Core::Application::GetFileSystemPtr()->GetStorageLocationDirectory(Core::StorageLocation::k_cache) + kstrTempAttachmentDirectory + inAttachment.mstrFilename;
				}
				else
				{
					strFilename = Core::Application::GetFileSystemPtr()->GetStorageLocationDirectory(inAttachment.meStorageLocation) + inAttachment.mstrFilename;
				}
			}

			mpJavaInterface->Present(inastrRecipientAddresses, instrSubject, instrContents, strFilename, inbFormatAsHtml, Core::MakeDelegate(this, &CEmailCompositionActivity::OnEmailClosed));
		}
        //-------------------------------------------------------
        /// Dismiss
        //-------------------------------------------------------
		void CEmailCompositionActivity::Dismiss()
		{
			//We don't have any way of dismissing the activity on Android, so do nothing.
		}
        //-------------------------------------------------------
        /// On Email Closed
        //-------------------------------------------------------
		void CEmailCompositionActivity::OnEmailClosed(s32 indwResultCode)
		{
			if(mCallback != NULL)
			{
				switch (indwResultCode)
				{
					case CEmailCompositionJavaInterface::kdwResultSuccess:
						mCallback(EmailCompositionActivity::SendResult::k_succeed);
						break;
					case CEmailCompositionJavaInterface::kdwResultCancelled:
						mCallback(EmailCompositionActivity::SendResult::k_cancelled);
						break;
					default:
						mCallback(EmailCompositionActivity::SendResult::k_failed);
						break;
				}
			}
			mCallback = NULL;
		}
		//-------------------------------------------------------
		/// Destructor
		//-------------------------------------------------------
		CEmailCompositionActivity::~CEmailCompositionActivity()
		{
			Core::Application::GetFileSystemPtr()->DeleteDirectory(Core::StorageLocation::k_cache, kstrTempAttachmentDirectory);
		}
	}
}
