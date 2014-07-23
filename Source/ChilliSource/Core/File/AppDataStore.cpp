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
#include <ChilliSource/Core/Container/ParamDictionarySerialiser.h>
#include <ChilliSource/Core/Cryptographic/AESEncrypt.h>
#include <ChilliSource/Core/Cryptographic/HashSHA1.h>
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
            const std::string k_privateKey = "aV0r71^jX01}pXMk";
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
                XML::Document doc;
                XML::Node* rootNode = doc.allocate_node(rapidxml::node_type::node_element);
                doc.append_node(rootNode);
                ParamDictionarySerialiser::ToXml(m_dictionary, rootNode);
                
                // Encrypt
                std::string strDocToBeEncrypted = XMLUtils::ToString(&doc);
                AESEncrypt::Data encryptedData = AESEncrypt::EncryptString(strDocToBeEncrypted, k_privateKey);

                // Write to disk
                FileSystem* pFileSystem = Application::Get()->GetFileSystem();
                FileStreamSPtr pFileStream = pFileSystem->CreateFileStream(StorageLocation::k_saveData, k_filename, FileMode::k_writeBinary);
                if(pFileStream->IsOpen() && !pFileStream->IsBad())
                {
                    pFileStream->Write(reinterpret_cast<const s8*>(encryptedData.m_data.get()), encryptedData.m_size);
                    pFileStream->Close();
                }
                
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
				FileStreamSPtr fileStream = pFileSystem->CreateFileStream(StorageLocation::k_saveData, k_filename, FileMode::k_readBinary);
				if (fileStream->IsOpen() == true && fileStream->IsBad() == false)
                {
					fileStream->SeekG(0, SeekDir::k_end);
					u32 encryptedDataSize = fileStream->TellG();
					fileStream->SeekG(0, SeekDir::k_beginning);
                    
                    std::unique_ptr<s8[]> encryptedData(new s8[encryptedDataSize]);
					fileStream->Read(encryptedData.get(), encryptedDataSize);
					fileStream->Close();
                    
                    std::string decrypted = AESEncrypt::DecryptString(reinterpret_cast<const u8*>(encryptedData.get()), encryptedDataSize, k_privateKey);

                    XMLUPtr xml = XMLUtils::ParseDocument(decrypted);
                    XML::Node* root = XMLUtils::GetFirstChildElement(xml->GetDocument());
                    if(nullptr != root)
                    {
                        m_dictionary = ParamDictionarySerialiser::FromXml(root);
                    }
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
