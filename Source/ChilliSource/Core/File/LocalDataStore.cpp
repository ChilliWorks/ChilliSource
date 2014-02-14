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
#include <ChilliSource/Core/String/StringConverter.h>
#include <ChilliSource/Core/Cryptographic/aes.h>
#include <ChilliSource/Core/Cryptographic/AESEncrypt.h>
#include <ChilliSource/Core/XML/XMLUtils.h>

namespace moFlo
{
	namespace Core
    {
        const std::string kstrLocalDataStoreDeprecatedFilename  = "App.lds";
        const std::string kstrLocalDataStoreEncryptedFilename   = "App.elds";
        const std::string kstrLocalDataStoreEncryptionKey       = "aV0r71^jX01}pXMk";
        
        CLocalDataStore* CLocalDataStore::mpSingletonInstance = NULL;
		
		//----------------------------------------------------------------
        /// Get Singleton
        //----------------------------------------------------------------
		CLocalDataStore& CLocalDataStore::GetSingleton()
        {
			return *mpSingletonInstance;
		}
        //----------------------------------------------------------------
        /// Get Singleton Pointer
        //----------------------------------------------------------------
		CLocalDataStore* CLocalDataStore::GetSingletonPtr()
        {
			return mpSingletonInstance;
		}
		//----------------------------------------------------------------
        /// Has Value For Key
        //----------------------------------------------------------------
		bool CLocalDataStore::HasValueForKey(const std::string& instrKey)
        {
			return mBackingDictionary.HasValue(instrKey);
		}
        //----------------------------------------------------------------
        /// Try Get Value
        //----------------------------------------------------------------
		bool CLocalDataStore::TryGetValue(const std::string& instrKey, std::string& outstrValue)
        {
			return mBackingDictionary.TryGetValue(instrKey, outstrValue);
		}
        //----------------------------------------------------------------
        /// Try Get Value
        //----------------------------------------------------------------
		bool CLocalDataStore::TryGetValue(const std::string& instrKey, bool& outbValue)
        {
			std::string strTempValue;
			bool bSuccess = mBackingDictionary.TryGetValue(instrKey, strTempValue);
			if(bSuccess)
            {
				outbValue = CStringConverter::ParseBool(strTempValue);
            }
			return bSuccess;
		}
        //----------------------------------------------------------------
        /// Try Get Value
        //----------------------------------------------------------------
		bool CLocalDataStore::TryGetValue(const std::string& instrKey, u16& outuwValue)
        {
			std::string strTempValue;
			bool bSuccess = mBackingDictionary.TryGetValue(instrKey, strTempValue);
			if(bSuccess)
            {
				outuwValue = static_cast<u16>(CStringConverter::ParseUnsignedInt(strTempValue));
            }
			return bSuccess;			
		}
        //----------------------------------------------------------------
        /// Try Get Value
        //----------------------------------------------------------------
		bool CLocalDataStore::TryGetValue(const std::string& instrKey, s16& outwValue)
        {
			std::string strTempValue;
			bool bSuccess = mBackingDictionary.TryGetValue(instrKey, strTempValue);
			if(bSuccess)
            {
				outwValue = static_cast<s16>(CStringConverter::ParseInt(strTempValue));
			}
			return bSuccess;
		}
        //----------------------------------------------------------------
        /// Try Get Value
        //----------------------------------------------------------------
		bool CLocalDataStore::TryGetValue(const std::string& instrKey, u32& outudwValue)
        {
			std::string strTempValue;
			bool bSuccess = mBackingDictionary.TryGetValue(instrKey, strTempValue);
			if(bSuccess)
            {
				outudwValue = static_cast<u32>(CStringConverter::ParseUnsignedInt(strTempValue));
			}
			return bSuccess;			
		}
        //----------------------------------------------------------------
        /// Try Get Value
        //----------------------------------------------------------------
		bool CLocalDataStore::TryGetValue(const std::string& instrKey, s32& outdwValue)
        {
			std::string strTempValue;
			bool bSuccess = mBackingDictionary.TryGetValue(instrKey, strTempValue);
			if(bSuccess)
            {
				outdwValue = static_cast<s32>(CStringConverter::ParseInt(strTempValue));
			}
			return bSuccess;			
		}
        //----------------------------------------------------------------
        /// Try Get Value
        //----------------------------------------------------------------
		bool CLocalDataStore::TryGetValue(const std::string& instrKey, u64& outuddwValue)
        {
			std::string strTempValue;
			bool bSuccess = mBackingDictionary.TryGetValue(instrKey, strTempValue);
			if(bSuccess)
            {
				outuddwValue = static_cast<u64>(CStringConverter::ParseUnsignedLong(strTempValue));
			}
			return bSuccess;			
		}
        //----------------------------------------------------------------
        /// Try Get Value
        //----------------------------------------------------------------
		bool CLocalDataStore::TryGetValue(const std::string& instrKey, s64& outddwValue)
        {
			std::string strTempValue;
			bool bSuccess = mBackingDictionary.TryGetValue(instrKey, strTempValue);
			if(bSuccess)
            {
				outddwValue = static_cast<s64>(CStringConverter::ParseLong(strTempValue));
			}
			return bSuccess;			
		}
        //----------------------------------------------------------------
        /// Try Get Value
        //----------------------------------------------------------------
		bool CLocalDataStore::TryGetValue(const std::string& instrKey, f32& outfValue)
        {
			std::string strTempValue;
			bool bSuccess = mBackingDictionary.TryGetValue(instrKey, strTempValue);
			if(bSuccess)
            {
				outfValue = CStringConverter::ParseFloat(strTempValue);
			}
			return bSuccess;		
		}		
        //----------------------------------------------------------------
        /// Set Value For Key
        //----------------------------------------------------------------
		void CLocalDataStore::SetValueForKey(const std::string& instrKey, const std::string& instrValue)
        {
			mBackingDictionary[instrKey] = instrValue;
			mbBackingValid = false;
		}
        //----------------------------------------------------------------
        /// Set Value For Key
        //----------------------------------------------------------------
		void CLocalDataStore::SetValueForKey(const std::string& instrKey, bool inbValue)
        {
			SetValueForKey(instrKey, CStringConverter::ToString(inbValue));
		}
        //----------------------------------------------------------------
        /// Set Value For Key
        //----------------------------------------------------------------
		void CLocalDataStore::SetValueForKey(const std::string& instrKey, u16 inuwValue)
        {
			SetValueForKey(instrKey, CStringConverter::ToString(inuwValue));			
		}
        //----------------------------------------------------------------
        /// Set Value For Key
        //----------------------------------------------------------------
		void CLocalDataStore::SetValueForKey(const std::string& instrKey, s16 inwValue)
        {
			SetValueForKey(instrKey, CStringConverter::ToString(inwValue));
		}
        //----------------------------------------------------------------
        /// Set Value For Key
        //----------------------------------------------------------------
		void CLocalDataStore::SetValueForKey(const std::string& instrKey, u32 inudwValue)
        {
			SetValueForKey(instrKey, CStringConverter::ToString(inudwValue));
		}
        //----------------------------------------------------------------
        /// Set Value For Key
        //----------------------------------------------------------------
		void CLocalDataStore::SetValueForKey(const std::string& instrKey, s32 indwValue)
        {
			SetValueForKey(instrKey, CStringConverter::ToString(indwValue));
		}
        //----------------------------------------------------------------
        /// Set Value For Key
        //----------------------------------------------------------------
		void CLocalDataStore::SetValueForKey(const std::string& instrKey, u64 inuddwValue)
        {
			SetValueForKey(instrKey, CStringConverter::ToString(inuddwValue));
		}
        //----------------------------------------------------------------
        /// Set Value For Key
        //----------------------------------------------------------------
		void CLocalDataStore::SetValueForKey(const std::string& instrKey, s64 inddwValue)
        {
			SetValueForKey(instrKey, CStringConverter::ToString(inddwValue));
		}
        //----------------------------------------------------------------
        /// Set Value For Key
        //----------------------------------------------------------------
		void CLocalDataStore::SetValueForKey(const std::string& instrKey, f32 infValue)
        {
			SetValueForKey(instrKey, CStringConverter::ToString(infValue));
		}
        //----------------------------------------------------------------
        /// Try Erase Key
        //----------------------------------------------------------------
		bool CLocalDataStore::TryEraseKey(const std::string& instrKey)
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
        void CLocalDataStore::Clear()
        {
            mBackingDictionary.clear();
            mbBackingValid = false;
            Synchronise();
        }
        //----------------------------------------------------------------
        /// Synchronise
        //----------------------------------------------------------------
		void CLocalDataStore::Synchronise()
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
            u32 udwEncryptedSize = CAESEncrypt::CalculateAlignedSize(strDocToBeEncrypted.size());
            s8* pdwDocEncrypted = new s8[udwEncryptedSize];
            CAESEncrypt::Encrypt(pudwDocBinary, udwEncryptedSize, kstrLocalDataStoreEncryptionKey, reinterpret_cast<u8*>(pdwDocEncrypted));
            
            // Write to disk
            IFileSystem* pFileSystem = CApplication::GetFileSystemPtr();
            FileStreamPtr pFileStream = pFileSystem->CreateFileStream(SL_SAVEDATA, kstrLocalDataStoreEncryptedFilename, FM_WRITE_BINARY);
            if(pFileStream->IsOpen() && !pFileStream->IsBad())
            {
                pFileStream->Write(pdwDocEncrypted, udwEncryptedSize);
                pFileStream->Close();
            }
            
            SAFE_DELETE_ARRAY(pdwDocEncrypted)
            mbBackingValid = true;
		}
        //----------------------------------------------------------------
        /// Constructor
        //----------------------------------------------------------------
        CLocalDataStore::CLocalDataStore()
        {
			mpSingletonInstance = this;
			RefreshFromFile();
		}
        //----------------------------------------------------------------
        /// Refresh From File
        //----------------------------------------------------------------
		void CLocalDataStore::RefreshFromFile()
        {
            IFileSystem* pFileSystem = CApplication::GetFileSystemPtr();
            if(pFileSystem->DoesFileExist(SL_SAVEDATA, kstrLocalDataStoreEncryptedFilename))
            {
                FileStreamPtr pFileStream = pFileSystem->CreateFileStream(SL_SAVEDATA, kstrLocalDataStoreEncryptedFilename, FM_READ);
                if(pFileStream->IsOpen() && !pFileStream->IsBad())
                {
                    std::string strEncryptedXML;
                    pFileStream->GetAll(strEncryptedXML);
                    pFileStream->Close();
                    
                    u32 udwEncryptedSize = strEncryptedXML.size();
                    s8* pbyData = new s8[udwEncryptedSize];
                    const u8* udwDocBinary = reinterpret_cast<const u8*>(strEncryptedXML.c_str());
                    CAESEncrypt::Decrypt(udwDocBinary, udwEncryptedSize, kstrLocalDataStoreEncryptionKey, reinterpret_cast<u8*>(pbyData));
                    
                    TiXmlDocument xmlDoc;
                    xmlDoc.Parse(pbyData, 0, TIXML_DEFAULT_ENCODING);
                    TiXmlElement* pRoot = xmlDoc.RootElement();
                    if(NULL != pRoot)
                    {
                        mBackingDictionary.FromXml(pRoot);
                    }
                    
                    SAFE_DELETE_ARRAY(pbyData);
                }
            }
            else
            if(pFileSystem->DoesFileExist(SL_SAVEDATA, kstrLocalDataStoreDeprecatedFilename))
            {
                TiXmlDocument xmlDoc;
                xmlDoc.LoadFile(SL_SAVEDATA, kstrLocalDataStoreDeprecatedFilename);
                TiXmlElement* pRoot = xmlDoc.RootElement();
                if(NULL != pRoot)
                {
                    mBackingDictionary.FromXml(pRoot);
                    pFileSystem->DeleteFile(SL_SAVEDATA, kstrLocalDataStoreDeprecatedFilename);
                }
            }
            
            mbBackingValid = true;
        }
        //----------------------------------------------------------------
        /// Subscribe To Application SuspendEvent
        //----------------------------------------------------------------
        void CLocalDataStore::SubscribeToApplicationSuspendEvent()
		{
			CApplicationEvents::GetLateSuspendEvent() += ApplicationSystemDelegate(this, &CLocalDataStore::OnApplicationSuspended);
		}
        //----------------------------------------------------------------
        /// On Application Suspended
        //----------------------------------------------------------------
		void CLocalDataStore::OnApplicationSuspended()
        {
			Synchronise();
		}
    }
}
