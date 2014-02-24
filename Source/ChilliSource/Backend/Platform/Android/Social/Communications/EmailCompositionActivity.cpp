/*
 *  EmailCompositionActivity.cpp
 *  moFlow
 *
 *  Created by Steven Hendrie on 20/01/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Platform/Android/Social/Communications/EmailCompositionActivity.h>
#include <ChilliSource/Platform/Android/JavaInterface/EmailCompositionJavaInterface.h>
#include <ChilliSource/Backend/Platform/Android/JavaInterface/JavaInterfaceManager.h>
#include <ChilliSource/Core/Main/Application.h>
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
		void CEmailCompositionActivity::Present(const std::vector<CUTF8String> & inastrRecipientAddresses, const CUTF8String & instrSubject, const CUTF8String & instrContents, const SendResultDelegate & inCallback, bool inbFormatAsHtml)
		{
			Attachment emptyAttachment;
			emptyAttachment.mstrFilename = "";
			emptyAttachment.mstrMIMEType = "";
			emptyAttachment.meStorageLocation = Core::SL_NONE;
			PresentWithAttachment(inastrRecipientAddresses, instrSubject, instrContents, emptyAttachment, inCallback, inbFormatAsHtml);
		}
		//-------------------------------------------------------
		/// Present With Attachment
		//-------------------------------------------------------
		void CEmailCompositionActivity::PresentWithAttachment(const DYNAMIC_ARRAY<CUTF8String> & inastrRecipientAddresses, const CUTF8String & instrSubject, const CUTF8String & instrContents, const Attachment& inAttachment, const SendResultDelegate & inCallback, bool inbFormatAsHtml)
		{
			mCallback = inCallback;

			std::string strFilename;
			if (inAttachment.mstrFilename.size() > 0)
			{
				if (inAttachment.meStorageLocation == Core::SL_PACKAGE || (inAttachment.meStorageLocation == Core::SL_DLC && Core::CApplication::GetFileSystemPtr()->DoesFileExistInCachedDLC(inAttachment.mstrFilename) == false))
				{
					Core::CApplication::GetFileSystemPtr()->CreateDirectory(Core::SL_CACHE, kstrTempAttachmentDirectory);
					Core::CApplication::GetFileSystemPtr()->CopyFile(inAttachment.meStorageLocation, inAttachment.mstrFilename, Core::SL_CACHE, kstrTempAttachmentDirectory + inAttachment.mstrFilename);
					strFilename = Core::CApplication::GetFileSystemPtr()->GetStorageLocationDirectory(Core::SL_CACHE) + kstrTempAttachmentDirectory + inAttachment.mstrFilename;
				}
				else
				{
					strFilename = Core::CApplication::GetFileSystemPtr()->GetStorageLocationDirectory(inAttachment.meStorageLocation) + inAttachment.mstrFilename;
				}
			}

			mpJavaInterface->Present(inastrRecipientAddresses, instrSubject, instrContents, strFilename, inbFormatAsHtml, CEmailCompositionJavaInterface::ResultDelegate(this, &CEmailCompositionActivity::OnEmailClosed));
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
						mCallback(IEmailCompositionActivity::SR_SUCCEED);
						break;
					case CEmailCompositionJavaInterface::kdwResultCancelled:
						mCallback(IEmailCompositionActivity::SR_CANCELLED);
						break;
					default:
						mCallback(IEmailCompositionActivity::SR_FAILED);
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
			DEBUG_LOG("Removing Temporary attachment files.");
			Core::CApplication::GetFileSystemPtr()->DeleteDirectory(Core::SL_CACHE, kstrTempAttachmentDirectory);
		}
	}
}
