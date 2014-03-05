/*
 *  ContactInformationProviderJavaInterface.cpp
 *  moFlow
 *
 *  Created by Steven Hendrie on 20/01/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Backend/Platform/Android/Social/Communications/ContactInformationProviderJavaInterface.h>

#include <ChilliSource/Backend/Platform/Android/Social/Communications/ContactInformationProvider.h>
#include <ChilliSource/Core/String/StringUtils.h>

//function definitions
extern "C"
{
	void Java_com_chillisource_social_ContactInformationProviderNativeInterface_SetData(JNIEnv* inpEnv, jobject thiz, int inudwSize,jobjectArray instrNames,jobjectArray instrNumbers,jobjectArray instrEmails);
}

//--------------------------------------------------------------------------------------
/// Set Names
///
/// Sets the names of the contacts that are received
/// from the java side
///
/// @param The jni environment.
/// @param the java object calling the function
/// @param number of names
/// @param array of names
/// @param array of numbers
/// @param array of emails
//--------------------------------------------------------------------------------------
void Java_com_chillisource_social_ContactInformationProviderNativeInterface_SetData(JNIEnv* inpEnv, jobject thiz, int inudwSize,jobjectArray instrNames,jobjectArray instrNumbers,jobjectArray instrEmails)
{
	JNIEnv* env = ChilliSource::Android::_IJavaInterface::GetJNIEnvironmentPtr();

	for(int udwName = 0; udwName < inudwSize; ++udwName)
	{
		jstring jstrName = (jstring)inpEnv->GetObjectArrayElement(instrNames,udwName);
		CSCore::UTF8String strName = ChilliSource::Android::_IJavaInterface::CreateUTF8StringFromJString(jstrName);
		ChilliSource::Android::ContactInformationProviderJavaInterface::mastrNames.push_back(strName);
		env->DeleteLocalRef(jstrName);

		jstring jstrNumber =  (jstring)inpEnv->GetObjectArrayElement(instrNumbers,udwName);
		CSCore::UTF8String strNumber = ChilliSource::Android::_IJavaInterface::CreateUTF8StringFromJString(jstrNumber);
		ChilliSource::Android::ContactInformationProviderJavaInterface::mastrNumbers.push_back(strNumber);
		env->DeleteLocalRef(jstrNumber);

		jstring jstrEmail =  (jstring)inpEnv->GetObjectArrayElement(instrEmails,udwName);
		CSCore::UTF8String strEmail = ChilliSource::Android::_IJavaInterface::CreateUTF8StringFromJString(jstrEmail);
		ChilliSource::Android::ContactInformationProviderJavaInterface::mastrEmails.push_back(strEmail);
		env->DeleteLocalRef(jstrEmail);
	}
}


using namespace ChilliSource::Core;
namespace ChilliSource
{
	namespace Android
	{
		std::vector<CSCore::UTF8String> ContactInformationProviderJavaInterface::mastrNames;
		std::vector<CSCore::UTF8String> ContactInformationProviderJavaInterface::mastrNumbers;
		std::vector<CSCore::UTF8String> ContactInformationProviderJavaInterface::mastrEmails;
		ContactInformationProvider * ContactInformationProviderJavaInterface::mpContactProvider = nullptr;

		//--------------------------------------------------------------------------------------
		/// Setup Java Interface
		///
		/// Sets up the java interface: stores the Virtual Machine, and Initialises any java
		/// methods callable from native.
		///
		/// @param a pointer to the virtual machine
		//--------------------------------------------------------------------------------------
		void ContactInformationProviderJavaInterface::SetupJavaInterface(JavaVM* inpJavaVM)
		{
			mspJavaVM = inpJavaVM;

			InitCallableStaticMethod("com/chillisource/social/ContactInformationProviderNativeInterface","LoadInformation", "()V");

		}
		//--------------------------------------------------------------------------------------
		/// Load Information
		///
		/// Loads contact information for player and transfers back to
		/// platform system
		///
		//--------------------------------------------------------------------------------------
		void ContactInformationProviderJavaInterface::LoadInformation(ContactInformationProvider * inpContactProvider)
		{
			mpContactProvider = inpContactProvider;

			MethodReference sdMethodRef = GetStaticMethodReference("LoadInformation");
			if (sdMethodRef.mMethodID != 0 && sdMethodRef.mClassID != 0)
			{
				JNIEnv* env = GetJNIEnvironmentPtr();
				env->CallStaticVoidMethod(sdMethodRef.mClassID, sdMethodRef.mMethodID);
			}
			mpContactProvider->SetContacts(CreateContacts());
		}
		//--------------------------------------------------------------------------------------
		/// Create Contacts
		///
		/// Created array of contact records to be used
		/// by contact information provider
		///
		/// @return array of data for contacts
		//--------------------------------------------------------------------------------------
		std::vector<Social::ContactRecord> ContactInformationProviderJavaInterface::CreateContacts()
		{
			std::vector<Social::ContactRecord> asContactRecords;

			for(u32 udwIndex = 0; udwIndex < mastrNames.size(); ++udwIndex)
			{
				Social::ContactRecord sContactRecord;
				sContactRecord.strName = mastrNames[udwIndex];
				sContactRecord.bHasMobileNumber = (mastrNumbers[udwIndex] != "") ? true : false;
				sContactRecord.strMobileNumber = mastrNumbers[udwIndex];
				sContactRecord.bHasEmail = (mastrEmails[udwIndex] != "") ? true : false;
				sContactRecord.strEmail = mastrEmails[udwIndex];
				asContactRecords.push_back(sContactRecord);
			}
			return asContactRecords;
		}
	}
}
