/*
 *  UDIDManager.cpp
 *  MoFlow
 *
 *  Created by Ian Copland on 01/12/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Platform/Android/UDIDManager.h>
#include <ChilliSource/Backend/Platform/Android/JavaInterface/JavaInterfaceManager.h>
#include <ChilliSource/Platform/Android/JavaInterface/CoreJavaInterface.h>
#include <ChilliSource/Platform/Android/JavaInterface/SharedPreferencesJavaInterface.h>
#include <ChilliSource/Core/File/LocalDataStore.h>
#include <ChilliSource/Core/Cryptographic/HashMD5.h>
#include <ctime>
#include <cstdlib>

namespace ChilliSource
{
	namespace Android
	{
		const std::string kstrUDIDStorageKey = "UDID";
		const std::string kstrSharedPrefsDocName = "MoFlowPreferences";
		//-----------------------------------------
		/// Constructor
		//-----------------------------------------
		CUDIDManager::CUDIDManager() : mstrUDID("NoUDID"), mbInitialised(false)
		{
		}
		//-----------------------------------------
		/// Get UDID
		//-----------------------------------------
		std::string CUDIDManager::GetUDID()
		{
			if (mbInitialised == false)
				Initialise();

			return mstrUDID;
		}
		//-----------------------------------------
		/// Initialise
		//-----------------------------------------
		void CUDIDManager::Initialise()
		{
			if (LoadUDID() == false)
				CalculateUDID();

			DEBUG_LOG("UDID: " + mstrUDID);
			mbInitialised = true;
		}
		//-----------------------------------------
		/// Load UDID
		//-----------------------------------------
		bool CUDIDManager::LoadUDID()
		{
			//load the UDID from the local data store
			std::string strLDSUDID = "";
			bool bLDSUDIDExists = false;
			if (moFlo::Core::CLocalDataStore::GetSingletonPtr()->HasValueForKey(kstrUDIDStorageKey) == true)
			{
				bLDSUDIDExists = moFlo::Core::CLocalDataStore::GetSingletonPtr()->TryGetValue(kstrUDIDStorageKey, strLDSUDID);
			}

			//load the UDID from the android data store
			std::string strASPUDID = "";
			bool bASPUDIDExists = false;
			if (SCSharedPreferencesJavaInterface::KeyExists(kstrSharedPrefsDocName, kstrUDIDStorageKey) == true)
			{
				strASPUDID = SCSharedPreferencesJavaInterface::GetString(kstrSharedPrefsDocName, kstrUDIDStorageKey, "FailedToGetUDID");
				if (strASPUDID != "FailedToGetUDID")
					bASPUDIDExists = true;
			}

			//if neither succeed, or they are different, then return false, otherwise set the UDID.
			if ((bLDSUDIDExists == false && bASPUDIDExists == false) || (bLDSUDIDExists == true && bASPUDIDExists == true && strLDSUDID != strASPUDID))
				return false;

			if (bLDSUDIDExists == true)
				mstrUDID = strLDSUDID;
			else if (bASPUDIDExists == true)
				mstrUDID = strASPUDID;
			else
				FATAL_LOG("Something has gone seriously wrong with the loading of UDIDs :S");

			return true;
		}
		//-----------------------------------------
		/// Save UDID
		//-----------------------------------------
		void CUDIDManager::SaveUDID()
		{
			//store in the local data store
			moFlo::Core::CLocalDataStore::GetSingletonPtr()->SetValueForKey(kstrUDIDStorageKey, mstrUDID);
			moFlo::Core::CLocalDataStore::GetSingletonPtr()->Synchronise();

			//store in shared preferences
			SCSharedPreferencesJavaInterface::SetString(kstrSharedPrefsDocName, kstrUDIDStorageKey, mstrUDID);
		}
		//-----------------------------------------
		/// Calculate UDID
		//-----------------------------------------
		void CUDIDManager::CalculateUDID()
		{
			CoreJavaInterfacePtr pCoreJI = CJavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CCoreJavaInterface>();

			//--try the mac address
			std::string strMacAddress = pCoreJI->GetMacAddress();
			if (strMacAddress != "")
			{
				mstrUDID = "m-" + moFlo::CHashMD5::GenerateHexHashCode(strMacAddress);
				SaveUDID();
				return;
			}

			//--try the Android ID
			std::string strAndroidID = pCoreJI->GetAndroidID();
			if (strAndroidID != "")
			{
				mstrUDID = "a-" + moFlo::CHashMD5::GenerateHexHashCode(strAndroidID);
				SaveUDID();
				return;
			}

			//--try the ID from the telephony manager
			std::string strTelephonyID = pCoreJI->GetTelephonyDeviceID();
			if (strTelephonyID != "")
			{
				mstrUDID = "t-" + moFlo::CHashMD5::GenerateHexHashCode(strTelephonyID);
				SaveUDID();
				return;
			}

			//--if all this fails, fall back on generating a random hash.
			//first of all seed rand with something that is as unique as possible: allocate a random bit of memory and add this to current time.
			u32* pRandomMemory = new u32;
			u32 udwSeed = ((u32)pRandomMemory) + time(NULL);
			SAFE_DELETE(pRandomMemory);
			srand(udwSeed);

			//generate a random number. perform a sequence of rand() calls incase MAX_RAND is really low.
			u32 dwRandomNumber = 0;
			u32 dwMaxUnsignedInt = ((u32)2147483647);
			u32 dwNumberOfRands = (u32)(RAND_MAX / dwMaxUnsignedInt);
			for (u32 i = 0; i < dwNumberOfRands; ++i)
				dwRandomNumber += rand();

			//use this random number to generate a UDID
			mstrUDID = "r-" + moFlo::CHashMD5::GenerateHexHashCode(STRING_CAST(dwRandomNumber));
			SaveUDID();
		}
		//-----------------------------------------
		/// Deconstructor
		//-----------------------------------------
		CUDIDManager::~CUDIDManager()
		{
		}
	}
}
