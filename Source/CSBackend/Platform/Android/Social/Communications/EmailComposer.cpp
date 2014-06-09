//
//  EmailComposer.cpp
//  Chilli Source
//  Created by Steven Hendrie on 20/01/2012.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2012 Tag Games Limited
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

#ifdef CS_TARGETPLATFORM_ANDROID

#include <CSBackend/Platform/Android/Social/Communications/EmailComposer.h>

#include <CSBackend/Platform/Android/Core/JNI/JavaInterfaceManager.h>
#include <CSBackend/Platform/Android/Social/Communications/EmailComposerJavaInterface.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Delegate/MakeDelegate.h>
#include <ChilliSource/Core/File/FileSystem.h>

namespace CSBackend
{
	namespace Android
	{
		namespace
		{
			const std::string k_tempAttachmentDirectory = "_AttachmentTemp/";
		}

		CS_DEFINE_NAMEDTYPE(EmailComposer);
		//-------------------------------------------------------
		//-------------------------------------------------------
		EmailComposer::EmailComposer()
			: m_isPresented(false)
		{
		}
        //-------------------------------------------------------
		//-------------------------------------------------------
		bool EmailComposer::IsA(CSCore::InterfaceIDType in_interfaceId) const
		{
			return (in_interfaceId == CSSocial::EmailComposer::InterfaceID || in_interfaceId == EmailComposer::InterfaceID);
		}
        //-------------------------------------------------------
        //-------------------------------------------------------
		void EmailComposer::Present(const std::vector<std::string>& in_recipientAddresses, const std::string& in_subject, const std::string& in_contents, ContentFormat in_contentFormat,
				const SendResultDelegate& in_callback)
		{
			Attachment emptyAttachment;
			emptyAttachment.m_storageLocation = CSCore::StorageLocation::k_none;
			PresentWithAttachment(in_recipientAddresses, in_subject, in_contents, in_contentFormat, emptyAttachment, in_callback);
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		void EmailComposer::PresentWithAttachment(const std::vector<std::string>& in_recipientAddresses, const std::string& in_subject, const std::string& in_contents,
				ContentFormat in_contentFormat, const Attachment& in_attachment, const SendResultDelegate & in_callback)
		{
			CS_ASSERT(m_isPresented == false, "Cannot present email composer while one is already presented.");

			m_isPresented = true;
			m_resultDelegate = in_callback;

			std::string filename;
			CSCore::FileSystem* fileSystem = CSCore::Application::Get()->GetFileSystem();
			if (in_attachment.m_filename.size() > 0)
			{
				if (in_attachment.m_storageLocation == CSCore::StorageLocation::k_package || (in_attachment.m_storageLocation == CSCore::StorageLocation::k_DLC && fileSystem->DoesFileExistInCachedDLC(in_attachment.m_filename) == false))
				{
					fileSystem->CreateDirectoryPath(CSCore::StorageLocation::k_cache, k_tempAttachmentDirectory);
					fileSystem->CopyFile(in_attachment.m_storageLocation, in_attachment.m_filename, CSCore::StorageLocation::k_cache, k_tempAttachmentDirectory + in_attachment.m_filename);
					filename = fileSystem->GetAbsolutePathToStorageLocation(CSCore::StorageLocation::k_cache) + k_tempAttachmentDirectory + in_attachment.m_filename;
				}
				else
				{
					filename = fileSystem->GetAbsolutePathToStorageLocation(in_attachment.m_storageLocation) + in_attachment.m_filename;
				}
			}

			m_javaInterface->Present(in_recipientAddresses, in_subject, in_contents, (in_contentFormat == ContentFormat::k_html), filename, CSCore::MakeDelegate(this, &EmailComposer::OnEmailClosed));
		}
        //-------------------------------------------------------
        //-------------------------------------------------------
		void EmailComposer::Dismiss()
		{
			if (m_isPresented == true)
			{
				m_isPresented = false;

				CS_LOG_FATAL("Cannot dismiss the email composer on Android.");
			}
		}
        //-------------------------------------------------------
        //-------------------------------------------------------
		void EmailComposer::OnEmailClosed(EmailComposerJavaInterface::Result in_result)
		{
			if(m_javaInterface != nullptr)
			{
				EmailComposer::SendResult result = EmailComposer::SendResult::k_failed;
				switch (in_result)
				{
					case EmailComposerJavaInterface::Result::k_success:
						result= EmailComposer::SendResult::k_succeed;
						break;
					case EmailComposerJavaInterface::Result::k_cancelled:
						result = EmailComposer::SendResult::k_cancelled;
						break;
					default:
						break;
				}

				m_isPresented = false;

				SendResultDelegate delegate = m_resultDelegate;
				m_resultDelegate = nullptr;
				delegate(result);
			}
		}
        //------------------------------------------------------
        //------------------------------------------------------
        void EmailComposer::OnInit()
        {
        	m_javaInterface = JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<EmailComposerJavaInterface>();
			if (m_javaInterface == nullptr)
			{
				m_javaInterface = EmailComposerJavaInterfaceSPtr(new EmailComposerJavaInterface());
				JavaInterfaceManager::GetSingletonPtr()->AddJavaInterface(m_javaInterface);
			}
        }
        //------------------------------------------------------
        //------------------------------------------------------
        void EmailComposer::OnDestroy()
        {
        	CSCore::Application::Get()->GetFileSystem()->DeleteDirectory(CSCore::StorageLocation::k_cache, k_tempAttachmentDirectory);
        	m_javaInterface.reset();
        }
	}
}

#endif