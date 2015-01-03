//
//  CkAudioBankProvider.cpp
//  Chilli Source
//  Created by Ian Copland on 30/12/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
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

#include <ChilliSource/Audio/CricketAudio/CkAudioBankProvider.h>

#include <ChilliSource/Audio/CricketAudio/CkAudioBank.h>
#include <ChilliSource/Audio/CricketAudio/CkSystem.h>
#include <ChilliSource/Core/Base/Application.h>

#include <ck/ck.h>
#include <ck/bank.h>

namespace ChilliSource
{
	namespace Audio
	{
		namespace
		{
			const char k_bankExtension[] = "ckb";
		}

		CS_DEFINE_NAMEDTYPE(CkAudioBankProvider);
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		CkAudioBankProviderUPtr CkAudioBankProvider::Create()
		{
			return CkAudioBankProviderUPtr(new CkAudioBankProvider());
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		bool CkAudioBankProvider::IsA(Core::InterfaceIDType in_interfaceId) const
		{
			return (Core::ResourceProvider::InterfaceID == in_interfaceId || CkAudioBankProvider::InterfaceID == in_interfaceId);
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		Core::InterfaceIDType CkAudioBankProvider::GetResourceType() const
		{
			return CkAudioBank::InterfaceID;
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		bool CkAudioBankProvider::CanCreateResourceWithFileExtension(const std::string& in_extension) const
		{
			std::string lowerExtension = in_extension;
			Core::StringUtils::ToLowerCase(lowerExtension);

			return (lowerExtension == k_bankExtension);
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		void CkAudioBankProvider::CreateResourceFromFile(Core::StorageLocation in_storageLocation, const std::string& in_filePath, const Core::IResourceOptionsBaseCSPtr& in_options, const Core::ResourceSPtr& out_resource)
		{
            CkBank* bank = nullptr;
            
#if CS_TARGETPLATFORM_ANDROID
            //TODO:
#else 
            std::string packageLocation = CSCore::StringUtils::StandardiseDirectoryPath(CSCore::Application::Get()->GetFileSystem()->GetAbsolutePathToStorageLocation(in_storageLocation));
            bank = CkBank::newBank((packageLocation + in_filePath).c_str(), kCkPathType_FileSystem);
#endif
            if (bank == nullptr)
            {
                out_resource->SetLoadState(Core::Resource::LoadState::k_failed);
                return;
            }
            
            static_cast<CkAudioBank*>(out_resource.get())->Build(bank);
            out_resource->SetLoadState(Core::Resource::LoadState::k_loaded);
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		void CkAudioBankProvider::CreateResourceFromFileAsync(Core::StorageLocation in_storageLocation, const std::string& in_filePath, const Core::IResourceOptionsBaseCSPtr& in_options, const AsyncLoadDelegate& in_delegate, const Core::ResourceSPtr& out_resource)
		{
			//TODO: !? handle async loading.
			CS_LOG_FATAL("Async currently not supported.");
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		void CkAudioBankProvider::OnInit()
		{
			CS_ASSERT(Core::Application::Get()->GetSystem<CkSystem>() != nullptr, "'CkSystem' is missing and is required by 'CkAudioBankProvider'!");
		}
	}
}