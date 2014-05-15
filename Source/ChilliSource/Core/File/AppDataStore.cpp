//
//  AppDataStore.cpp
//  Chilli Source
//  Created by Stuart McGaw on 24/05/2011.
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

#include <ChilliSource/Core/File/AppDataStore.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/Device.h>
#include <ChilliSource/Core/Container/ParamDictionarySerialiser.h>
#include <ChilliSource/Core/Cryptographic/aes.h>
#include <ChilliSource/Core/Cryptographic/AESEncrypt.h>
#include <ChilliSource/Core/Delegate/MakeDelegate.h>
#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/Core/XML/XMLUtils.h>

namespace ChilliSource
{
	namespace Core
    {
        namespace
        {
            const std::string k_filename = "App.ads";
            
            //---------------------------------------------------------
            /// Generates the key that is used to encrypt the ADS. Note
            /// that this is not really crytographically secure as none
            /// of the information used to generate the hash is truely
            /// private.
            ///
            /// @author I Copland
            //---------------------------------------------------------
            std::string GenerateEncryptionKey()
            {
                const std::string k_salt = "aV0r71^jX01}pXMk";
                Device* device = Application::Get()->GetSystem<Device>();
                
                //calculate the SHA1 hash.
                SHA1 hash;
                hash.Reset();
                hash.Update((u8*)device->GetUDID().c_str(), device->GetUDID().size());
                hash.Update((u8*)k_salt.c_str(), k_salt.size());
                hash.Final();
                std::string hexHash = hash.GetHash(SHA1::REPORT_HEX_SHORT);
                CS_ASSERT(hexHash.length() == 40, "Something has gone wrong with the SHA1 hash.");
                
                //truncate into 128 bits.
                std::string output;
                for (u32 i = 0; i < 16; i++)
                {
                    s8 lower = hexHash[i*2 + 0];
                    s8 upper = hexHash[i*2 + 1];
                    s8 combined = (lower + (upper << 4));
                    output += combined;
                    
                    std::string print;
                    print +=combined;
                }
                
                return output;
            }
        }
        
        CS_DEFINE_NAMEDTYPE(AppDataStore);
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        AppDataStoreUPtr AppDataStore::Create()
        {
            return AppDataStoreUPtr(new AppDataStore());
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        AppDataStore::AppDataStore()
        : m_needsSynchonised(false)
        {
            RefreshFromFile();
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        bool AppDataStore::IsA(InterfaceIDType in_interfaceId) const
        {
            return (AppDataStore::InterfaceID == in_interfaceId);
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
		bool AppDataStore::Contains(const std::string& in_key)
        {
            std::unique_lock<std::mutex> lock(m_mutex);
			return m_dictionary.HasKey(in_key);
		}
        //--------------------------------------------------------------
        //--------------------------------------------------------------
		bool AppDataStore::TryGetValue(const std::string& in_key, std::string& out_value) const
        {
            std::unique_lock<std::mutex> lock(m_mutex);
			return m_dictionary.TryGetValue(in_key, out_value);
		}
        //--------------------------------------------------------------
        //--------------------------------------------------------------
		bool AppDataStore::TryGetValue(const std::string& in_key, bool& out_value) const
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            
			std::string strTempValue;
			bool bSuccess = m_dictionary.TryGetValue(in_key, strTempValue);
			if(bSuccess)
            {
				out_value = ParseBool(strTempValue);
            }
			return bSuccess;
		}
        //--------------------------------------------------------------
        //--------------------------------------------------------------
		bool AppDataStore::TryGetValue(const std::string& in_key, u16& out_value) const
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            
			std::string strTempValue;
			bool bSuccess = m_dictionary.TryGetValue(in_key, strTempValue);
			if(bSuccess)
            {
				out_value = static_cast<u16>(ParseU32(strTempValue));
            }
			return bSuccess;			
		}
        //--------------------------------------------------------------
        //--------------------------------------------------------------
		bool AppDataStore::TryGetValue(const std::string& in_key, s16& out_value) const
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            
			std::string strTempValue;
			bool bSuccess = m_dictionary.TryGetValue(in_key, strTempValue);
			if(bSuccess)
            {
				out_value = static_cast<s16>(ParseS32(strTempValue));
			}
			return bSuccess;
		}
        //--------------------------------------------------------------
        //--------------------------------------------------------------
		bool AppDataStore::TryGetValue(const std::string& in_key, u32& out_value) const
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            
			std::string strTempValue;
			bool bSuccess = m_dictionary.TryGetValue(in_key, strTempValue);
			if(bSuccess)
            {
				out_value = static_cast<u32>(ParseU32(strTempValue));
			}
			return bSuccess;			
		}
        //--------------------------------------------------------------
        //--------------------------------------------------------------
		bool AppDataStore::TryGetValue(const std::string& in_key, s32& out_value) const
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            
			std::string strTempValue;
			bool bSuccess = m_dictionary.TryGetValue(in_key, strTempValue);
			if(bSuccess)
            {
				out_value = static_cast<s32>(ParseS32(strTempValue));
			}
			return bSuccess;			
		}
        //--------------------------------------------------------------
        //--------------------------------------------------------------
		bool AppDataStore::TryGetValue(const std::string& in_key, u64& out_value) const
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            
			std::string strTempValue;
			bool bSuccess = m_dictionary.TryGetValue(in_key, strTempValue);
			if(bSuccess)
            {
				out_value = static_cast<u64>(ParseU64(strTempValue));
			}
			return bSuccess;			
		}
        //--------------------------------------------------------------
        //--------------------------------------------------------------
		bool AppDataStore::TryGetValue(const std::string& in_key, s64& out_value) const
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            
			std::string strTempValue;
			bool bSuccess = m_dictionary.TryGetValue(in_key, strTempValue);
			if(bSuccess)
            {
				out_value = static_cast<s64>(ParseS64(strTempValue));
			}
			return bSuccess;			
		}
        //--------------------------------------------------------------
        //--------------------------------------------------------------
		bool AppDataStore::TryGetValue(const std::string& in_key, f32& out_value) const
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            
			std::string strTempValue;
			bool bSuccess = m_dictionary.TryGetValue(in_key, strTempValue);
			if(bSuccess)
            {
				out_value = ParseF32(strTempValue);
			}
			return bSuccess;		
		}		
        //--------------------------------------------------------------
        //--------------------------------------------------------------
		void AppDataStore::SetValue(const std::string& in_key, const std::string& in_value)
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            
			m_dictionary[in_key] = in_value;
			m_needsSynchonised = true;
		}
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        void AppDataStore::SetValue(const std::string& in_key, const s8* in_value)
        {
            SetValue(in_key, std::string(in_value));
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
		void AppDataStore::SetValue(const std::string& in_key, bool in_value)
        {
			SetValue(in_key, ToString(in_value));
		}
        //--------------------------------------------------------------
        //--------------------------------------------------------------
		void AppDataStore::SetValue(const std::string& in_key, u16 in_value)
        {
			SetValue(in_key, ToString(in_value));
		}
        //--------------------------------------------------------------
        //--------------------------------------------------------------
		void AppDataStore::SetValue(const std::string& in_key, s16 in_value)
        {
			SetValue(in_key, ToString(in_value));
		}
        //--------------------------------------------------------------
        //--------------------------------------------------------------
		void AppDataStore::SetValue(const std::string& in_key, u32 in_value)
        {
			SetValue(in_key, ToString(in_value));
		}
        //--------------------------------------------------------------
        //--------------------------------------------------------------
		void AppDataStore::SetValue(const std::string& in_key, s32 in_value)
        {
			SetValue(in_key, ToString(in_value));
		}
        //--------------------------------------------------------------
        //--------------------------------------------------------------
		void AppDataStore::SetValue(const std::string& in_key, u64 in_value)
        {
			SetValue(in_key, ToString(in_value));
		}
        //--------------------------------------------------------------
        //--------------------------------------------------------------
		void AppDataStore::SetValue(const std::string& in_key, s64 in_value)
        {
			SetValue(in_key, ToString(in_value));
		}
        //--------------------------------------------------------------
        //--------------------------------------------------------------
		void AppDataStore::SetValue(const std::string& in_key, f32 infValue)
        {
			SetValue(in_key, ToString(infValue));
		}
        //--------------------------------------------------------------
        //--------------------------------------------------------------
		bool AppDataStore::Erase(const std::string& in_key)
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            
			ParamDictionary::iterator pEntry = m_dictionary.find(in_key);
			if(m_dictionary.end() != pEntry)
            {
				m_dictionary.erase(pEntry);
				return true;
			}
			
			return false;
		}
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        void AppDataStore::Clear()
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            
            m_dictionary.clear();
            m_needsSynchonised = true;
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
		void AppDataStore::Save()
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            
			if(m_needsSynchonised == true)
            {
                // Convert to XML
                TiXmlDocument xmlDoc;
                TiXmlElement xmlRootElement("ADS");
                ParamDictionarySerialiser::ToXml(m_dictionary, &xmlRootElement);
                xmlDoc.InsertEndChild(xmlRootElement);
                
                // Encrypt
                TiXmlPrinter xmlPrinter;
                xmlDoc.Accept(&xmlPrinter);
                std::string strDocToBeEncrypted = xmlPrinter.CStr();
                const u8* pudwDocBinary = reinterpret_cast<const u8*>(strDocToBeEncrypted.c_str());
                u32 udwEncryptedSize = AESEncrypt::CalculateAlignedSize(strDocToBeEncrypted.size());
                s8* pdwDocEncrypted = new s8[udwEncryptedSize];
                AESEncrypt::Encrypt(pudwDocBinary, udwEncryptedSize, GenerateEncryptionKey(), reinterpret_cast<u8*>(pdwDocEncrypted));
                
                // Write to disk
                FileSystem* pFileSystem = Application::Get()->GetFileSystem();
                FileStreamSPtr pFileStream = pFileSystem->CreateFileStream(StorageLocation::k_saveData, k_filename, FileMode::k_writeBinary);
                if(pFileStream->IsOpen() && !pFileStream->IsBad())
                {
                    pFileStream->Write(pdwDocEncrypted, udwEncryptedSize);
                    pFileStream->Close();
                }
                
                CS_SAFEDELETE_ARRAY(pdwDocEncrypted)
                m_needsSynchonised = false;
            }
		}
        //--------------------------------------------------------------
        //--------------------------------------------------------------
		void AppDataStore::RefreshFromFile()
        {
            FileSystem* pFileSystem = Application::Get()->GetFileSystem();
            if(pFileSystem->DoesFileExist(StorageLocation::k_saveData, k_filename) == true)
            {
                FileStreamSPtr pFileStream = pFileSystem->CreateFileStream(StorageLocation::k_saveData, k_filename, FileMode::k_read);
                if(pFileStream->IsOpen() && !pFileStream->IsBad())
                {
                    std::string strEncryptedXML;
                    pFileStream->GetAll(strEncryptedXML);
                    pFileStream->Close();
                    
                    u32 udwEncryptedSize = strEncryptedXML.size();
                    s8* pbyData = new s8[udwEncryptedSize];
                    const u8* udwDocBinary = reinterpret_cast<const u8*>(strEncryptedXML.c_str());
                    AESEncrypt::Decrypt(udwDocBinary, udwEncryptedSize, GenerateEncryptionKey(), reinterpret_cast<u8*>(pbyData));
                    
                    TiXmlDocument xmlDoc;
                    xmlDoc.Parse(pbyData, 0, TIXML_DEFAULT_ENCODING);
                    TiXmlElement* pRoot = xmlDoc.RootElement();
                    if(nullptr != pRoot)
                    {
                        m_dictionary = ParamDictionarySerialiser::FromXml(pRoot);
                    }
                    
                    CS_SAFEDELETE_ARRAY(pbyData);
                }
            }
            
            m_needsSynchonised = false;
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
		void AppDataStore::OnSuspend()
        {
			Save();
		}
    }
}
