//
//  TextureProvider.cpp
//  ChilliSource
//  Created by Scott Downie on 15/04/2014.
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

#include <ChilliSource/Rendering/Texture/TextureProvider.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Image/Image.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>
#include <ChilliSource/Rendering/Texture/Texture.h>
#include <ChilliSource/Rendering/Texture/TextureDesc.h>
#include <ChilliSource/Rendering/Texture/TextureResourceOptions.h>

namespace ChilliSource
{
    CS_DEFINE_NAMEDTYPE(TextureProvider);
    
    const IResourceOptionsBaseCSPtr TextureProvider::s_defaultOptions(std::make_shared<TextureResourceOptions>());
    
    //-------------------------------------------------------
    //-------------------------------------------------------
    TextureProviderUPtr TextureProvider::Create()
    {
        return TextureProviderUPtr(new TextureProvider());
    }
    //-------------------------------------------------------------------------
    //-------------------------------------------------------------------------
    bool TextureProvider::IsA(InterfaceIDType in_interfaceId) const
    {
        return in_interfaceId == ResourceProvider::InterfaceID || in_interfaceId == TextureProvider::InterfaceID;
    }
    //----------------------------------------------------------------------------
    //----------------------------------------------------------------------------
    void TextureProvider::PostCreate()
    {
        auto resourceProviders = Application::Get()->GetSystems<ResourceProvider>();
        
        for(u32 i=0; i<resourceProviders.size(); ++i)
        {
            if(resourceProviders[i]->GetResourceType() == Image::InterfaceID)
            {
                m_imageProviders.push_back(resourceProviders[i]);
            }
        }
    }
    //----------------------------------------------------------------------------
    //----------------------------------------------------------------------------
    InterfaceIDType TextureProvider::GetResourceType() const
    {
        return Texture::InterfaceID;
    }
    //----------------------------------------------------------------------------
    //----------------------------------------------------------------------------
    bool TextureProvider::CanCreateResourceWithFileExtension(const std::string& in_extension) const
    {
        for(u32 i=0; i<m_imageProviders.size(); ++i)
        {
            if(m_imageProviders[i]->CanCreateResourceWithFileExtension(in_extension))
            {
                return true;
            }
        }
        
        return false;
    }
    //----------------------------------------------------
    //----------------------------------------------------
    IResourceOptionsBaseCSPtr TextureProvider::GetDefaultOptions() const
    {
        return s_defaultOptions;
    }
    //----------------------------------------------------------------------------
    //----------------------------------------------------------------------------
    void TextureProvider::CreateResourceFromFile(StorageLocation in_location, const std::string& in_filePath, const IResourceOptionsBaseCSPtr& in_options, const ResourceSPtr& out_resource)
    {
        LoadTexture(in_location, in_filePath, in_options, nullptr, out_resource);
    }
    //----------------------------------------------------------------------------
    //----------------------------------------------------------------------------
    void TextureProvider::CreateResourceFromFileAsync(StorageLocation in_location, const std::string& in_filePath, const IResourceOptionsBaseCSPtr& in_options, const ResourceProvider::AsyncLoadDelegate& in_delegate, const ResourceSPtr& out_resource)
    {
        Application::Get()->GetTaskScheduler()->ScheduleTask(TaskType::k_file, [=](const TaskContext&) noexcept
        {
            LoadTexture(in_location, in_filePath, in_options, in_delegate, out_resource);
        });
    }
    //----------------------------------------------------------------------------
    //----------------------------------------------------------------------------
    void TextureProvider::LoadTexture(StorageLocation in_location, const std::string& in_filePath, const IResourceOptionsBaseCSPtr& in_options, const ResourceProvider::AsyncLoadDelegate& in_delegate, const ResourceSPtr& out_resource)
    {
        CS_ASSERT(in_options != nullptr, "Options for texture load cannot be null");
        
        std::string fileName;
        std::string fileExtension;
        StringUtils::SplitBaseFilename(in_filePath, fileName, fileExtension);
        
        ResourceProvider* imageProvider = nullptr;
        for(u32 i=0; i<m_imageProviders.size(); ++i)
        {
            if(m_imageProviders[i]->CanCreateResourceWithFileExtension(fileExtension))
            {
                imageProvider = m_imageProviders[i];
                break;
            }
        }
        
        if(imageProvider == nullptr)
        {
            CS_LOG_ERROR("Cannot find provider for " + in_filePath);
            out_resource->SetLoadState(Resource::LoadState::k_failed);
            if(in_delegate != nullptr)
            {
                Application::Get()->GetTaskScheduler()->ScheduleTask(TaskType::k_mainThread, [=](const TaskContext&) noexcept
                {
                    in_delegate(out_resource);
                });
            }
            return;
        }
        
        ResourceSPtr imageResource(Image::Create());
        imageProvider->CreateResourceFromFile(in_location, in_filePath, nullptr, imageResource);
        ImageSPtr image(std::static_pointer_cast<Image>(imageResource));
        
        if(image->GetLoadState() == Resource::LoadState::k_failed)
        {
            CS_LOG_ERROR("Failed to load image " + in_filePath);
            out_resource->SetLoadState(Resource::LoadState::k_failed);
            if(in_delegate != nullptr)
            {
                Application::Get()->GetTaskScheduler()->ScheduleTask(TaskType::k_mainThread, [=](const TaskContext&) noexcept
                {
                    in_delegate(out_resource);
                });
            }
            return;
        }
        
        if(in_delegate == nullptr)
        {
            auto texture = static_cast<Texture*>(out_resource.get());
            auto options = static_cast<const TextureResourceOptions*>(in_options.get());
            
            TextureDesc desc(Integer2(image->GetWidth(), image->GetHeight()), image->GetFormat(), image->GetCompression(), false);
            desc.SetFilterMode(options->GetFilterMode());
            desc.SetWrapModeS(options->GetWrapModeS());
            desc.SetWrapModeT(options->GetWrapModeT());
            desc.SetMipmappingEnabled(options->IsMipMapsEnabled());

            texture->Build(Texture::DataUPtr(image->MoveData()), image->GetDataSize(), desc);
            texture->SetLoadState(Resource::LoadState::k_loaded);
        }
        else
        {
            Application::Get()->GetTaskScheduler()->ScheduleTask(TaskType::k_mainThread, [=](const TaskContext&) noexcept
            {
                auto texture = static_cast<Texture*>(out_resource.get());
                auto options = static_cast<const TextureResourceOptions*>(in_options.get());

                TextureDesc desc(Integer2(image->GetWidth(), image->GetHeight()), image->GetFormat(), image->GetCompression(), false);
                desc.SetFilterMode(options->GetFilterMode());
                desc.SetWrapModeS(options->GetWrapModeS());
                desc.SetWrapModeT(options->GetWrapModeT());
                desc.SetMipmappingEnabled(options->IsMipMapsEnabled());

                texture->Build(Texture::DataUPtr(image->MoveData()), image->GetDataSize(), desc);
                texture->SetLoadState(Resource::LoadState::k_loaded);
                in_delegate(out_resource);
            });
        }
    }
}

