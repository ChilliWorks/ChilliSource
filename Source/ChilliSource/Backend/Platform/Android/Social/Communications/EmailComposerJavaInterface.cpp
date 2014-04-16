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

#include <ChilliSource/Backend/Platform/Android/Social/Communications/EmailComposerJavaInterface.h>

#include <ChilliSource/Backend/Platform/Android/Core/JNI/JavaInterfaceManager.h>
#include <ChilliSource/Backend/Platform/Android/Core/JNI/JavaInterfaceUtils.h>

//function definitions
extern "C"
{
	void Java_com_chillisource_social_EmailComposerNativeInterface_OnEmailClosed(JNIEnv* inpEnv, jobject inpThis, s32 indwResultCode);
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
void Java_com_chillisource_social_EmailComposerNativeInterface_OnEmailClosed(JNIEnv* inpEnv, jobject inpThis, s32 indwResultCode)
{
	ChilliSource::Android::EmailComposerJavaInterfaceSPtr pJavaInterface = ChilliSource::Android::JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<ChilliSource::Android::EmailComposerJavaInterface>();
	if (pJavaInterface != nullptr)
	{
		pJavaInterface->OnEmailClosed(indwResultCode);
	}
}

namespace ChilliSource
{
	namespace Android
	{

		CS_DEFINE_NAMEDTYPE(EmailComposerJavaInterface);
		//--------------------------------------------------------------
		/// Constructor
		//--------------------------------------------------------------
		EmailComposerJavaInterface::EmailComposerJavaInterface()
		{
			CreateNativeInterface("com/chillisource/social/EmailComposerNativeInterface");
			CreateMethodReference("Present", "([Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Z)V");
		}
		//--------------------------------------------------------------
		/// Is A
		//--------------------------------------------------------------
		bool EmailComposerJavaInterface::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return (inInterfaceID == EmailComposerJavaInterface::InterfaceID);
		}
		//--------------------------------------------------------------
		/// Present
		//--------------------------------------------------------------
		void EmailComposerJavaInterface::Present(const std::vector<Core::UTF8String>& inastrRecipientAddresses, const Core::UTF8String& instrSubject, const Core::UTF8String& instrContents,
													const std::string& instrAttachmentFilename, bool inbFormatAsHtml, const ResultDelegate& inDelegate)
		{
			mDelegate = inDelegate;

			JNIEnv* pEnv = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();

			//build the recipient list
			u32 udwCount = 0;
			jobjectArray ajstrRecipients = pEnv->NewObjectArray(inastrRecipientAddresses.size(), pEnv->FindClass("java/lang/String"), pEnv->NewStringUTF(""));
			for(std::vector<Core::UTF8String>::const_iterator it = inastrRecipientAddresses.begin(); it != inastrRecipientAddresses.end(); ++it)
			{
				jstring jstrRecipient = JavaInterfaceUtils::CreateJStringFromUTF8String(*it);
				pEnv->SetObjectArrayElement(ajstrRecipients,udwCount++, jstrRecipient);
				pEnv->DeleteLocalRef(jstrRecipient);
			}

			jstring jstrSubject = JavaInterfaceUtils::CreateJStringFromUTF8String(instrSubject);
			jstring jstrContents = JavaInterfaceUtils::CreateJStringFromUTF8String(instrContents);
			jstring jstrAttachmentFilename = JavaInterfaceUtils::CreateJStringFromUTF8String(instrAttachmentFilename);

			//call method
			pEnv->CallVoidMethod(GetJavaObject(), GetMethodID("Present"), ajstrRecipients, jstrSubject, jstrContents, jstrAttachmentFilename, inbFormatAsHtml);

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
				mDelegate(indwResultCode);
			}
			mDelegate = nullptr;
		}
	}
}
