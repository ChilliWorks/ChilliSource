//
//  CkBankProvider.cpp
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

#include <ChilliSource/Audio/CricketAudio/CkBankProvider.h>

#include <ChilliSource/Audio/CricketAudio/CkBank.h>
#include <ChilliSource/Audio/CricketAudio/CricketAudioSystem.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>

#include <ck/ck.h>
#include <ck/bank.h>

namespace ChilliSource
{
    namespace
    {
        const char k_bankExtension[] = "ckb";
    }

    CS_DEFINE_NAMEDTYPE(CkBankProvider);
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    CkBankProviderUPtr CkBankProvider::Create()
    {
        return CkBankProviderUPtr(new CkBankProvider());
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    bool CkBankProvider::IsA(InterfaceIDType in_interfaceId) const
    {
        return (ResourceProvider::InterfaceID == in_interfaceId || CkBankProvider::InterfaceID == in_interfaceId);
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    InterfaceIDType CkBankProvider::GetResourceType() const
    {
        return CkBank::InterfaceID;
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    bool CkBankProvider::CanCreateResourceWithFileExtension(const std::string& in_extension) const
    {
        std::string lowerExtension = in_extension;
        StringUtils::ToLowerCase(lowerExtension);

        return (lowerExtension == k_bankExtension);
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void CkBankProvider::CreateResourceFromFile(StorageLocation in_storageLocation, const std::string& in_filePath, const IResourceOptionsBaseCSPtr& in_options, const ResourceSPtr& out_resource)
    {
        auto fileSystem = Application::Get()->GetFileSystem();

        ::CkBank* bank = nullptr;

#ifdef CS_TARGETPLATFORM_ANDROID
        if (in_storageLocation == StorageLocation::k_package || in_storageLocation == StorageLocation::k_chilliSource ||
            (in_storageLocation == StorageLocation::k_DLC && fileSystem->DoesFileExistInCachedDLC(in_filePath) == false))
        {
            m_zippedCkBankLoader.Load(in_storageLocation, in_filePath, out_resource);
            return;
        }
        else
        {
            auto absFilePath = fileSystem->GetAbsolutePathToStorageLocation(in_storageLocation) + in_filePath;
            bank = ::CkBank::newBank(absFilePath.c_str(), kCkPathType_FileSystem);
        }
#else
        if (in_storageLocation == StorageLocation::k_DLC && fileSystem->DoesFileExistInCachedDLC(in_filePath) == false)
        {
            auto absFilePath = fileSystem->GetAbsolutePathToStorageLocation(StorageLocation::k_package) + fileSystem->GetPackageDLCPath() + in_filePath;
            bank = ::CkBank::newBank(absFilePath.c_str(), kCkPathType_FileSystem);
        }
        else
        {
            auto absFilePath = fileSystem->GetAbsolutePathToStorageLocation(in_storageLocation) + in_filePath;
            bank = ::CkBank::newBank(absFilePath.c_str(), kCkPathType_FileSystem);
        }
#endif

        if (bank == nullptr)
        {
            out_resource->SetLoadState(Resource::LoadState::k_failed);
            return;
        }
        
        static_cast<CkBank*>(out_resource.get())->Build(bank);
        out_resource->SetLoadState(Resource::LoadState::k_loaded);
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void CkBankProvider::CreateResourceFromFileAsync(StorageLocation in_storageLocation, const std::string& in_filePath, const IResourceOptionsBaseCSPtr& in_options, const AsyncLoadDelegate& in_delegate, const ResourceSPtr& out_resource)
    {
        auto fileSystem = Application::Get()->GetFileSystem();

        ::CkBank* bank = nullptr;

#ifdef CS_TARGETPLATFORM_ANDROID
        if (in_storageLocation == StorageLocation::k_package || in_storageLocation == StorageLocation::k_chilliSource ||
            (in_storageLocation == StorageLocation::k_DLC && fileSystem->DoesFileExistInCachedDLC(in_filePath) == false))
        {
            m_zippedCkBankLoader.LoadAsync(in_storageLocation, in_filePath, in_delegate, out_resource);
            return;
        }
        else
        {
            auto absFilePath = fileSystem->GetAbsolutePathToStorageLocation(in_storageLocation) + in_filePath;
            bank = ::CkBank::newBankAsync(absFilePath.c_str(), kCkPathType_FileSystem);
        }
#else
        if (in_storageLocation == StorageLocation::k_DLC && fileSystem->DoesFileExistInCachedDLC(in_filePath) == false)
        {
            auto absFilePath = fileSystem->GetAbsolutePathToStorageLocation(StorageLocation::k_package) + fileSystem->GetPackageDLCPath() + in_filePath;
            bank = ::CkBank::newBankAsync(absFilePath.c_str(), kCkPathType_FileSystem);
        }
        else
        {
            auto absFilePath = fileSystem->GetAbsolutePathToStorageLocation(in_storageLocation) + in_filePath;
            bank = ::CkBank::newBankAsync(absFilePath.c_str(), kCkPathType_FileSystem);
        }
#endif

        if (bank != nullptr)
        {
            AsyncRequest request;
            request.m_bankResource = std::static_pointer_cast<CkBank>(out_resource);
            request.m_bank = bank;
            request.m_delegate = in_delegate;
            m_asyncRequests.push_back(request);
        }
        else
        {
            out_resource->SetLoadState(Resource::LoadState::k_failed);
            in_delegate(out_resource);
        }
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void CkBankProvider::OnInit()
    {
        CS_ASSERT(Application::Get()->GetSystem<CricketAudioSystem>() != nullptr, "'CricketAudioSystem' is missing and is required by 'CkBankProvider'!");
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void CkBankProvider::OnUpdate(f32 in_deltaTime)
    {
        if(m_asyncRequests.empty() == false)
        {
            m_asyncRequests.lock();
            for (auto it = m_asyncRequests.begin(); it != m_asyncRequests.end();)
            {
                if (it->m_bank->isLoaded() == true)
                {
                    CS_ASSERT(it->m_bank != nullptr, "Bank cannot be null!");
                    
                    AsyncRequest request = *it;
                    it = m_asyncRequests.erase(it);
                    
                    if (request.m_bank->isFailed() == false)
                    {
                        request.m_bankResource->Build(request.m_bank);
                        request.m_bankResource->SetLoadState(Resource::LoadState::k_loaded);
                        request.m_delegate(request.m_bankResource);
                    }
                    else
                    {
                        request.m_bank->destroy();
                        request.m_bankResource->SetLoadState(Resource::LoadState::k_failed);
                        request.m_delegate(request.m_bankResource);
                    }
                }
                else
                {
                    ++it;
                }
            }
            m_asyncRequests.unlock();
        }
    }
}