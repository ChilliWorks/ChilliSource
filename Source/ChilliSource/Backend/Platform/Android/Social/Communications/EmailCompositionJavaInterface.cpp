/*
 *  EmailCompositionJavaInterface.cpp
 *  moFlow
 *
 *  Created by Steven Hendrie on 20/01/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Backend/Platform/Android/Social/Communications/EmailCompositionJavaInterface.h>

#include <ChilliSource/Backend/Platform/Android/Core/JNI/JavaInterfaceManager.h>
#include <ChilliSource/Backend/Platform/Android/Core/JNI/JavaInterfaceUtils.h>
#include <ChilliSource/Backend/Platform/Android/Social/Communications/EmailCompositionActivity.h>

//function definitions
extern "C"
{
	void Java_com_taggames_moflow_nativeinterface_CEmailCompositionNativeInterface_OnEmailClosed(JNIEnv* inpEnv, jobject inpThis, s32 indwResultCode);
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
void Java_com_taggames_moflow_nativeinterface_CEmailCompositionNativeInterface_OnEmailClosed(JNIEnv* inpEnv, jobject inpThis, s32 indwResultCode)
{
	ChilliSource::Android::EmailCompositionJavaInterfacePtr pJavaInterface = ChilliSource::Android::CJavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<ChilliSource::Android::CEmailCompositionJavaInterface>();
	if (pJavaInterface != NULL)
	{
		pJavaInterface->OnEmailClosed(indwResultCode);
	}
}

namespace ChilliSource
{
	namespace Android
	{

		CS_DEFINE_NAMEDTYPE(CEmailCompositionJavaInterface);
		//--------------------------------------------------------------
		/// Constructor
		//--------------------------------------------------------------
		CEmailCompositionJavaInterface::CEmailCompositionJavaInterface()
		{
			CreateNativeInterface("com/taggames/moflow/nativeinterface/CEmailCompositionNativeInterface");
			CreateMethodReference("Present", "([Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Z)V");
		}
		//--------------------------------------------------------------
		/// Is A
		//--------------------------------------------------------------
		bool CEmailCompositionJavaInterface::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return (inInterfaceID == CEmailCompositionJavaInterface::InterfaceID);
		}
		//--------------------------------------------------------------
		/// Present
		//--------------------------------------------------------------
		void CEmailCompositionJavaInterface::Present(const std::vector<Core::UTF8String>& inastrRecipientAddresses, const Core::UTF8String& instrSubject, const Core::UTF8String& instrContents,
													const std::string& instrAttachmentFilename, bool inbFormatAsHtml, const ResultDelegate& inDelegate)
		{
			mDelegate = inDelegate;

			JNIEnv* pEnv = CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();

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
		void CEmailCompositionJavaInterface::OnEmailClosed(s32 indwResultCode)
		{
			if (mDelegate != NULL)
			{
				mDelegate(indwResultCode);
			}
			mDelegate = NULL;
		}
	}
}
