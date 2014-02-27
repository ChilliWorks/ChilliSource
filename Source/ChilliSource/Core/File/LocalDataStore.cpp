/*
 *  LocalDataStore.cpp
 *  moFlow
 *
 *  Created by Stuart McGaw on 24/05/2011.
 *  Modified by Robert Henning on 24/07/2013
 *   + Updated to use encryption
 *  Copyright 2011 Tag Games. All rights reserved.
 */

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/ApplicationEvents.h>
#include <ChilliSource/Core/File/LocalDataStore.h>
#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/Core/Cryptographic/aes.h>
#include <ChilliSource/Core/Cryptographic/AESEncrypt.h>
#include <ChilliSource/Core/XML/XMLUtils.h>

namespace ChilliSource
{
	namespace Core
    {
        const std::string kstrLocalDataStoreDeprecatedFilename  = "App.lds";
        const std::string kstrLocalDataStoreEncryptedFilename   = "App.elds";
        const std::string kstrLocalDataStoreEncryptionKey       = "aV0r71^jX01}pXMk";
        
        LocalDataStore* LocalDataStore::mpSingletonInstance = nullptr;
		
		//----------------------------------------------------------------
        /// Get Singleton
        //----------------------------------------------------------------
		LocalDataStore& LocalDataStore::GetSingleton()
        {
			return *mpSingletonInstance;
		}
        //----------------------------------------------------------------
        /// Get Singleton Pointer
        //----------------------------------------------------------------
		LocalDataStore* LocalDataStore::GetSingletonPtr()
        {
			return mpSingletonInstance;
		}
		//----------------------------------------------------------------
        /// Has Value For Key
        //----------------------------------------------------------------
		bool LocalDataStore::HasValueForKey(const std::string& instrKey)
        {
			return mBackingDictionary.HasValue(instrKey);
		}
        //----------------------------------------------------------------
        /// Try Get Value
        //----------------------------------------------------------------
		bool LocalDataStore::TryGetValue(const std::string& instrKey, std::string& outstrValue)
        {
			return mBackingDictionary.TryGetValue(instrKey, outstrValue);
		}
        //----------------------------------------------------------------
        /// Try Get Value
        //----------------------------------------------------------------
		bool LocalDataStore::TryGetValue(const std::string& instrKey, bool& outbValue)
        {
			std::string strTempValue;
			bool bSuccess = mBackingDictionary.TryGetValue(instrKey, strTempValue);
			if(bSuccess)
            {
				outbValue = ParseBool(strTempValue);
            }
			return bSuccess;
		}
        //----------------------------------------------------------------
        /// Try Get Value
        //----------------------------------------------------------------
		bool LocalDataStore::TryGetValue(const std::string& instrKey, u16& outuwValue)
        {
			std::string strTempValue;
			bool bSuccess = mBackingDictionary.TryGetValue(instrKey, strTempValue);
			if(bSuccess)
            {
				outuwValue = static_cast<u16>(ParseU32(strTempValue));
            }
			return bSuccess;			
		}
        //----------------------------------------------------------------
        /// Try Get Value
        //----------------------------------------------------------------
		bool LocalDataStore::TryGetValue(const std::string& instrKey, s16& outwValue)
        {
			std::string strTempValue;
			bool bSuccess = mBackingDictionary.TryGetValue(instrKey, strTempValue);
			if(bSuccess)
            {
				outwValue = static_cast<s16>(ParseS32(strTempValue));
			}
			return bSuccess;
		}
        //----------------------------------------------------------------
        /// Try Get Value
        //----------------------------------------------------------------
		bool LocalDataStore::TryGetValue(const std::string& instrKey, u32& outudwValue)
        {
			std::string strTempValue;
			bool bSuccess = mBackingDictionary.TryGetValue(instrKey, strTempValue);
			if(bSuccess)
            {
				outudwValue = static_cast<u32>(ParseU32(strTempValue));
			}
			return bSuccess;			
		}
        //----------------------------------------------------------------
        /// Try Get Value
        //----------------------------------------------------------------
		bool LocalDataStore::TryGetValue(const std::string& instrKey, s32& outdwValue)
        {
			std::string strTempValue;
			bool bSuccess = mBackingDictionary.TryGetValue(instrKey, strTempValue);
			if(bSuccess)
            {
				outdwValue = static_cast<s32>(ParseS32(strTempValue));
			}
			return bSuccess;			
		}
        //----------------------------------------------------------------
        /// Try Get Value
        //----------------------------------------------------------------
		bool LocalDataStore::TryGetValue(const std::string& instrKey, u64& outuddwValue)
        {
			std::string strTempValue;
			bool bSuccess = mBackingDictionary.TryGetValue(instrKey, strTempValue);
			if(bSuccess)
            {
				outuddwValue = static_cast<u64>(ParseU64(strTempValue));
			}
			return bSuccess;			
		}
        //----------------------------------------------------------------
        /// Try Get Value
        //----------------------------------------------------------------
		bool LocalDataStore::TryGetValue(const std::string& instrKey, s64& outddwValue)
        {
			std::string strTempValue;
			bool bSuccess = mBackingDictionary.TryGetValue(instrKey, strTempValue);
			if(bSuccess)
            {
				outddwValue = static_cast<s64>(ParseS64(strTempValue));
			}
			return bSuccess;			
		}
        //----------------------------------------------------------------
        /// Try Get Value
        //----------------------------------------------------------------
		bool LocalDataStore::TryGetValue(const std::string& instrKey, f32& outfValue)
        {
			std::string strTempValue;
			bool bSuccess = mBackingDictionary.TryGetValue(instrKey, strTempValue);
			if(bSuccess)
            {
				outfValue = ParseF32(strTempValue);
			}
			return bSuccess;		
		}		
        //----------------------------------------------------------------
        /// Set Value For Key
        //----------------------------------------------------------------
		void LocalDataStore::SetValueForKey(const std::string& instrKey, const std::string& instrValue)
        {
			mBackingDictionary[instrKey] = instrValue;
			mbBackingValid = false;
		}
        //----------------------------------------------------------------
        /// Set Value For Key
        //----------------------------------------------------------------
		void LocalDataStore::SetValueForKey(const std::string& instrKey, bool inbValue)
        {
			SetValueForKey(instrKey, ToString(inbValue));
		}
        //----------------------------------------------------------------
        /// Set Value For Key
        //----------------------------------------------------------------
		void LocalDataStore::SetValueForKey(const std::string& instrKey, u16 inuwValue)
        {
			SetValueForKey(instrKey, ToString(inuwValue));			
		}
        //----------------------------------------------------------------
        /// Set Value For Key
        //----------------------------------------------------------------
		void LocalDataStore::SetValueForKey(const std::string& instrKey, s16 inwValue)
        {
			SetValueForKey(instrKey, ToString(inwValue));
		}
        //----------------------------------------------------------------
        /// Set Value For Key
        //----------------------------------------------------------------
		void LocalDataStore::SetValueForKey(const std::string& instrKey, u32 inudwValue)
        {
			SetValueForKey(instrKey, ToString(inudwValue));
		}
        //----------------------------------------------------------------
        /// Set Value For Key
        //----------------------------------------------------------------
		void LocalDataStore::SetValueForKey(const std::string& instrKey, s32 indwValue)
        {
			SetValueForKey(instrKey, ToString(indwValue));
		}
        //----------------------------------------------------------------
        /// Set Value For Key
        //----------------------------------------------------------------
		void LocalDataStore::SetValueForKey(const std::string& instrKey, u64 inuddwValue)
        {
			SetValueForKey(instrKey, ToString(inuddwValue));
		}
        //----------------------------------------------------------------
        /// Set Value For Key
        //----------------------------------------------------------------
		void LocalDataStore::SetValueForKey(const std::string& instrKey, s64 inddwValue)
        {
			SetValueForKey(instrKey, ToString(inddwValue));
		}
        //----------------------------------------------------------------
        /// Set Value For Key
        //----------------------------------------------------------------
		void LocalDataStore::SetValueForKey(const std::string& instrKey, f32 infValue)
        {
			SetValueForKey(instrKey, ToString(infValue));
		}
        //----------------------------------------------------------------
        /// Try Erase Key
        //----------------------------------------------------------------
		bool LocalDataStore::TryEraseKey(const std::string& instrKey)
        {
			ParamDictionary::iterator pEntry = mBackingDictionary.find(instrKey);
			if(mBackingDictionary.end() != pEntry)
            {
				mBackingDictionary.erase(pEntry);
				return true;
			}
			
			return false;
		}
        //------------------------
        /// Clear
        //------------------------
        void LocalDataStore::Clear()
        {
            mBackingDictionary.clear();
            mbBackingValid = false;
            Synchronise();
        }
        //----------------------------------------------------------------
        /// Synchronise
        //----------------------------------------------------------------
		void LocalDataStore::Synchronise()
        {
			if(mbBackingValid)
            {
				return;
			}
            
            // Convert to XML
			TiXmlDocument xmlDoc;
			TiXmlElement xmlRootElement("LDS");
			mBackingDictionary.ToXml(&xmlRootElement);
			xmlDoc.InsertEndChild(xmlRootElement);
			
            // Encrypt
            TiXmlPrinter xmlPrinter;
            xmlDoc.Accept(&xmlPrinter);
            std::string strDocToBeEncrypted = xmlPrinter.CStr();
            const u8* pudwDocBinary = reinterpret_cast<const u8*>(strDocToBeEncrypted.c_str());
            u32 udwEncryptedSize = AESEncrypt::CalculateAlignedSize(strDocToBeEncrypted.size());
            s8* pdwDocEncrypted = new s8[udwEncryptedSize];
            AESEncrypt::Encrypt(pudwDocBinary, udwEncryptedSize, kstrLocalDataStoreEncryptionKey, reinterpret_cast<u8*>(pdwDocEncrypted));
            
            // Write to disk
            FileSystem* pFileSystem = Application::GetFileSystemPtr();
            FileStreamSPtr pFileStream = pFileSystem->CreateFileStream(StorageLocation::k_saveData, kstrLocalDataStoreEncryptedFilename, FileMode::k_writeBinary);
            if(pFileStream->IsOpen() && !pFileStream->IsBad())
            {
                pFileStream->Write(pdwDocEncrypted, udwEncryptedSize);
                pFileStream->Close();
            }
            
            CS_SAFE_DELETE_ARRAY(pdwDocEncrypted)
            mbBackingValid = true;
		}
        //----------------------------------------------------------------
        /// Constructor
        //----------------------------------------------------------------
        LocalDataStore::LocalDataStore()
        {
			mpSingletonInstance = this;
			RefreshFromFile();
		}
        //----------------------------------------------------------------
        /// Refresh From File
        //----------------------------------------------------------------
		void LocalDataStore::RefreshFromFile()
        {
            FileSystem* pFileSystem = Application::GetFileSystemPtr();
            if(pFileSystem->DoesFileExist(StorageLocation::k_saveData, kstrLocalDataStoreEncryptedFilename))
            {
                FileStreamSPtr pFileStream = pFileSystem->CreateFileStream(StorageLocation::k_saveData, kstrLocalDataStoreEncryptedFilename, FileMode::k_read);
                if(pFileStream->IsOpen() && !pFileStream->IsBad())
                {
                    std::string strEncryptedXML;
                    pFileStream->GetAll(strEncryptedXML);
                    pFileStream->Close();
                    
                    u32 udwEncryptedSize = strEncryptedXML.size();
                    s8* pbyData = new s8[udwEncryptedSize];
                    const u8* udwDocBinary = reinterpret_cast<const u8*>(strEncryptedXML.c_str());
                    AESEncrypt::Decrypt(udwDocBinary, udwEncryptedSize, kstrLocalDataStoreEncryptionKey, reinterpret_cast<u8*>(pbyData));
                    
                    TiXmlDocument xmlDoc;
                    xmlDoc.Parse(pbyData, 0, TIXML_DEFAULT_ENCODING);
                    TiXmlElement* pRoot = xmlDoc.RootElement();
                    if(nullptr != pRoot)
                    {
                        mBackingDictionary.FromXml(pRoot);
                    }
                    
                    CS_SAFE_DELETE_ARRAY(pbyData);
                }
            }
            else
            if(pFileSystem->DoesFileExist(StorageLocation::k_saveData, kstrLocalDataStoreDeprecatedFilename))
            {
                TiXmlDocument xmlDoc;
                xmlDoc.LoadFile(StorageLocation::k_saveData, kstrLocalDataStoreDeprecatedFilename);
                TiXmlElement* pRoot = xmlDoc.RootElement();
                if(nullptr != pRoot)
                {
                    mBackingDictionary.FromXml(pRoot);
                    pFileSystem->DeleteFile(StorageLocation::k_saveData, kstrLocalDataStoreDeprecatedFilename);
                }
            }
            
            mbBackingValid = true;
        }
        //----------------------------------------------------------------
        /// Subscribe To Application SuspendEvent
        //----------------------------------------------------------------
        void LocalDataStore::SubscribeToApplicationSuspendEvent()
		{
			ApplicationEvents::GetLateSuspendEvent() += ApplicationSystemDelegate(this, &LocalDataStore::OnApplicationSuspended);
		}
        //----------------------------------------------------------------
        /// On Application Suspended
        //----------------------------------------------------------------
		void LocalDataStore::OnApplicationSuspended()
        {
			Synchronise();
		}
    }
}
