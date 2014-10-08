//
//  EmailComposerJavaInterface.cpp
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

#include <CSBackend/Platform/Android/Social/Communications/EmailComposerJavaInterface.h>

#include <CSBackend/Platform/Android/Core/JNI/JavaInterfaceManager.h>
#include <CSBackend/Platform/Android/Core/JNI/JavaInterfaceUtils.h>

//function definitions
extern "C"
{
	void Java_com_chilliworks_chillisource_social_EmailComposerNativeInterface_OnEmailClosed(JNIEnv* inpEnv, jobject inpThis, s32 indwResultCode);
}
//--------------------------------------------------------------------------------------
/// On Email Closed
///
/// Interface function called from java. This is called whenever the email activity
/// returns a result.
///
/// @param The jni environment.
/// @param the java object calling the function.
/// @param The result code.
//--------------------------------------------------------------------------------------
void Java_com_chilliworks_chillisource_social_EmailComposerNativeInterface_OnEmailClosed(JNIEnv* inpEnv, jobject inpThis, s32 indwResultCode)
{
	CSBackend::Android::EmailComposerJavaInterfaceSPtr pJavaInterface = CSBackend::Android::JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CSBackend::Android::EmailComposerJavaInterface>();
	if (pJavaInterface != nullptr)
	{
		pJavaInterface->OnEmailClosed(indwResultCode);
	}
}

namespace CSBackend
{
	namespace Android
	{
		namespace
		{
			//--------------------------------------------------------------
			/// Converts the integer result returned from java to the
			/// Result enum.
			///
			/// @author Ian Copland
			///
			/// @param The result integer.
			///
			/// @return The result enum.
			//--------------------------------------------------------------
			EmailComposerJavaInterface::Result IntegerToResult(s32 in_result)
			{
				const s32 k_resultSuccess = -1;
				const s32 k_resultCancelled = 0;

				switch (in_result)
				{
				case k_resultSuccess:
					return EmailComposerJavaInterface::Result::k_success;
				case k_resultCancelled:
					return EmailComposerJavaInterface::Result::k_cancelled;
				default:
					return EmailComposerJavaInterface::Result::k_failed;
				}
			}
		}

		CS_DEFINE_NAMEDTYPE(EmailComposerJavaInterface);
		//--------------------------------------------------------------
		/// Constructor
		//--------------------------------------------------------------
		EmailComposerJavaInterface::EmailComposerJavaInterface()
		{
			CreateNativeInterface("com/chilliworks/chillisource/social/EmailComposerNativeInterface");
			CreateMethodReference("Present", "([Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;ZLjava/lang/String;)V");
		}
		//--------------------------------------------------------------
		/// Is A
		//--------------------------------------------------------------
		bool EmailComposerJavaInterface::IsA(CSCore::InterfaceIDType inInterfaceID) const
		{
			return (inInterfaceID == EmailComposerJavaInterface::InterfaceID);
		}
		//--------------------------------------------------------------
		/// Present
		//--------------------------------------------------------------
		void EmailComposerJavaInterface::Present(const std::vector<std::string>& inastrRecipientAddresses, const std::string& instrSubject, const std::string& instrContents,
				bool inbFormatAsHtml, const std::string& instrAttachmentFilename, const ResultDelegate& inDelegate)
		{
			mDelegate = inDelegate;

			JNIEnv* pEnv = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();

			//build the recipient list
			u32 udwCount = 0;
			jobjectArray ajstrRecipients = pEnv->NewObjectArray(inastrRecipientAddresses.size(), pEnv->FindClass("java/lang/String"), pEnv->NewStringUTF(""));
			for(auto it = inastrRecipientAddresses.begin(); it != inastrRecipientAddresses.end(); ++it)
			{
				jstring jstrRecipient = JavaInterfaceUtils::CreateJStringFromSTDString(*it);
				pEnv->SetObjectArrayElement(ajstrRecipients,udwCount++, jstrRecipient);
				pEnv->DeleteLocalRef(jstrRecipient);
			}

			jstring jstrSubject = JavaInterfaceUtils::CreateJStringFromSTDString(instrSubject);
			jstring jstrContents = JavaInterfaceUtils::CreateJStringFromSTDString(instrContents);
			jstring jstrAttachmentFilename = JavaInterfaceUtils::CreateJStringFromSTDString(instrAttachmentFilename);

			//call method
			pEnv->CallVoidMethod(GetJavaObject(), GetMethodID("Present"), ajstrRecipients, jstrSubject, jstrContents, inbFormatAsHtml, jstrAttachmentFilename);

			//clean up
			pEnv->DeleteLocalRef(ajstrRecipients);
			pEnv->DeleteLocalRef(jstrSubject);
			pEnv->DeleteLocalRef(jstrContents);
			pEnv->DeleteLocalRef(jstrAttachmentFilename);
		}
		//--------------------------------------------------------------
		/// On Email Closed
		//--------------------------------------------------------------
		void EmailComposerJavaInterface::OnEmailClosed(s32 indwResultCode)
		{
			if (mDelegate != nullptr)
			{
				ResultDelegate delegate = mDelegate;
				mDelegate = nullptr;
				delegate(IntegerToResult(indwResultCode));
			}
		}
	}
}

#endif
