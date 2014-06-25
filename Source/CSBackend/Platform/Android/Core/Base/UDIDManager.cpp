//
//  UDIDManager.cpp
//  Chilli Source
//  Created by Ian Copland on 01/12/2011.
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

#ifdef CS_TARGETPLATFORM_ANDROID

#include <CSBackend/Platform/Android/Core/Base/UDIDManager.h>

#include <CSBackend/Platform/Android/Core/Base/CoreJavaInterface.h>
#include <CSBackend/Platform/Android/Core/File/SharedPreferencesJavaInterface.h>
#include <CSBackend/Platform/Android/Core/JNI/JavaInterfaceManager.h>
#include <ChilliSource/Core/Cryptographic/HashMD5.h>

#include <cstdlib>
#include <ctime>

namespace CSBackend
{
	namespace Android
	{
		const std::string kstrUDIDStorageKey = "UDID";
		const std::string kstrSharedPrefsDocName = "CSPreferences";
		//-----------------------------------------
		/// Constructor
		//-----------------------------------------
		UDIDManager::UDIDManager() : mstrUDID("NoUDID"), mbInitialised(false)
		{
		}
		//-----------------------------------------
		/// Get UDID
		//-----------------------------------------
		const std::string& UDIDManager::GetUDID()
		{
			if (mbInitialised == false)
				Initialise();

			return mstrUDID;
		}
		//-----------------------------------------
		/// Initialise
		//-----------------------------------------
		void UDIDManager::Initialise()
		{
			if (LoadUDID() == false)
				CalculateUDID();

			mbInitialised = true;
		}
		//-----------------------------------------
		/// Load UDID
		//-----------------------------------------
		bool UDIDManager::LoadUDID()
		{
			//load the UDID from the android data store
			std::string strASPUDID = "";
			bool bASPUDIDExists = false;
			if (SharedPreferencesJavaInterface::KeyExists(kstrSharedPrefsDocName, kstrUDIDStorageKey) == true)
			{
				strASPUDID = SharedPreferencesJavaInterface::GetString(kstrSharedPrefsDocName, kstrUDIDStorageKey, "FailedToGetUDID");
				if (strASPUDID != "FailedToGetUDID")
					bASPUDIDExists = true;
			}

			//if neither succeed, or they are different, then return false, otherwise set the UDID.
			if (bASPUDIDExists == false)
				return false;

			mstrUDID = strASPUDID;

			return true;
		}
		//-----------------------------------------
		/// Save UDID
		//-----------------------------------------
		void UDIDManager::SaveUDID()
		{
			//store in shared preferences
			SharedPreferencesJavaInterface::SetString(kstrSharedPrefsDocName, kstrUDIDStorageKey, mstrUDID);
		}
		//-----------------------------------------
		/// Calculate UDID
		//-----------------------------------------
		void UDIDManager::CalculateUDID()
		{
			CoreJavaInterfaceSPtr pCoreJI = JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CoreJavaInterface>();

			//--try the mac address
			std::string strMacAddress = pCoreJI->GetMacAddress();
			if (strMacAddress != "")
			{
				mstrUDID = "m-" + CSCore::HashMD5::GenerateHexHashCode(strMacAddress);
				SaveUDID();
				return;
			}

			//--try the Android ID
			std::string strAndroidID = pCoreJI->GetAndroidID();
			if (strAndroidID != "")
			{
				mstrUDID = "a-" + CSCore::HashMD5::GenerateHexHashCode(strAndroidID);
				SaveUDID();
				return;
			}

			//--try the ID from the telephony manager
			std::string strTelephonyID = pCoreJI->GetTelephonyDeviceID();
			if (strTelephonyID != "")
			{
				mstrUDID = "t-" + CSCore::HashMD5::GenerateHexHashCode(strTelephonyID);
				SaveUDID();
				return;
			}

			//--if all this fails, fall back on generating a random hash.
			//first of all seed rand with something that is as unique as possible: allocate a random bit of memory and add this to current time.
			u32* pRandomMemory = new u32;
			u32 udwSeed = ((u32)pRandomMemory) + time(nullptr);
			CS_SAFEDELETE(pRandomMemory);
			srand(udwSeed);

			//generate a random number. perform a sequence of rand() calls incase MAX_RAND is really low.
			u32 dwRandomNumber = 0;
			u32 dwMaxUnsignedInt = ((u32)2147483647);
			u32 dwNumberOfRands = (u32)(RAND_MAX / dwMaxUnsignedInt);
			for (u32 i = 0; i < dwNumberOfRands; ++i)
				dwRandomNumber += rand();

			//use this random number to generate a UDID
			mstrUDID = "r-" + CSCore::HashMD5::GenerateHexHashCode(CSCore::ToString(dwRandomNumber));
			SaveUDID();
		}
		//-----------------------------------------
		/// Deconstructor
		//-----------------------------------------
		UDIDManager::~UDIDManager()
		{
		}
	}
}

#endif
