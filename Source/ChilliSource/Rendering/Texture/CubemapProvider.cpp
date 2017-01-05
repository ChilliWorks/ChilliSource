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

#include <ChilliSource/Rendering/Texture/CubemapProvider.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/Core/Image/Image.h>
#include <ChilliSource/Core/Json/JsonUtils.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>
#include <ChilliSource/Rendering/Texture/Cubemap.h>
#include <ChilliSource/Rendering/Texture/TextureDesc.h>
#include <ChilliSource/Rendering/Texture/CubemapResourceOptions.h>

#include <json/json.h>

namespace ChilliSource
{
    CS_DEFINE_NAMEDTYPE(CubemapProvider);
    
    const IResourceOptionsBaseCSPtr CubemapProvider::s_defaultOptions(std::make_shared<CubemapResourceOptions>());
    
    namespace
    {
        const std::string k_cubemapExtension("cscubemap");
    }
    
    //-------------------------------------------------------
    //-------------------------------------------------------
    CubemapProviderUPtr CubemapProvider::Create()
    {
        return CubemapProviderUPtr(new CubemapProvider());
    }
    //-------------------------------------------------------------------------
    //-------------------------------------------------------------------------
    bool CubemapProvider::IsA(InterfaceIDType in_interfaceId) const
    {
        return in_interfaceId == ResourceProvider::InterfaceID || in_interfaceId == CubemapProvider::InterfaceID;
    }
    //----------------------------------------------------------------------------
    //----------------------------------------------------------------------------
    void CubemapProvider::PostCreate()
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
    InterfaceIDType CubemapProvider::GetResourceType() const
    {
        return Cubemap::InterfaceID;
    }
    //----------------------------------------------------------------------------
    //----------------------------------------------------------------------------
    bool CubemapProvider::CanCreateResourceWithFileExtension(const std::string& in_extension) const
    {
        return in_extension == k_cubemapExtension;
    }
    //----------------------------------------------------
    //----------------------------------------------------
    IResourceOptionsBaseCSPtr CubemapProvider::GetDefaultOptions() const
    {
        return s_defaultOptions;
    }
    //----------------------------------------------------------------------------
    //----------------------------------------------------------------------------
    void CubemapProvider::CreateResourceFromFile(StorageLocation in_location, const std::string& in_filePath, const IResourceOptionsBaseCSPtr& in_options, const ResourceSPtr& out_resource)
    {
        LoadCubemap(in_location, in_filePath, in_options, nullptr, out_resource);
    }
    //----------------------------------------------------------------------------
    //----------------------------------------------------------------------------
    void CubemapProvider::CreateResourceFromFileAsync(StorageLocation in_location, const std::string& in_filePath, const IResourceOptionsBaseCSPtr& in_options, const ResourceProvider::AsyncLoadDelegate& in_delegate, const ResourceSPtr& out_resource)
    {
        Application::Get()->GetTaskScheduler()->ScheduleTask(TaskType::k_file, [=](const TaskContext&) noexcept
        {
            LoadCubemap(in_location, in_filePath, in_options, in_delegate, out_resource);
        });
    }
    //----------------------------------------------------------------------------
    //----------------------------------------------------------------------------
    void CubemapProvider::LoadCubemap(StorageLocation in_location, const std::string& in_filePath, const IResourceOptionsBaseCSPtr& in_options, const ResourceProvider::AsyncLoadDelegate& in_delegate, const ResourceSPtr& out_resource)
    {
        CS_ASSERT(in_options != nullptr, "Options for cubemap load cannot be null");
        
        //Load the JSON description file that holds the names of the 6 textures
        StorageLocation imageLocation = StorageLocation::k_package;
        std::array<std::string, 6> imagePaths;
        Json::Value root;
        if(JsonUtils::ReadJson(in_location, in_filePath, root) == true)
        {
            imageLocation = ParseStorageLocation(root.get("StorageLocation", "Package").asString());
            
            const auto& faces = root["Faces"];
            CS_ASSERT(faces.size() == 6, "Cubemap must have 6 images specified");
            for(auto i=0u; i<faces.size(); ++i)
            {
                imagePaths[i] = faces[i].asString();
            }
        }
        else
        {
            CS_LOG_ERROR("Cannot find cubemap " + in_filePath);
            out_resource->SetLoadState(Resource::LoadState::k_failed);
            if(in_delegate != nullptr)
            {
                Application::Get()->GetTaskScheduler()->ScheduleTask(TaskType::k_mainThread, [=](const TaskContext&) noexcept{ in_delegate(out_resource); });
            }
            return;
        }
        
        //Load the actual images
        std::array<ImageSPtr, 6> images;
        
        for(auto i=0; i<imagePaths.size(); ++i)
        {
            std::string fileName;
            std::string fileExtension;
            StringUtils::SplitBaseFilename(imagePaths[i], fileName, fileExtension);
            
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
                CS_LOG_ERROR("Cannot find provider for " + imagePaths[i]);
                out_resource->SetLoadState(Resource::LoadState::k_failed);
                if(in_delegate != nullptr)
                {
                    Application::Get()->GetTaskScheduler()->ScheduleTask(TaskType::k_mainThread, [=](const TaskContext&) noexcept{ in_delegate(out_resource); });
                }
                return;
            }
            
            ResourceSPtr imageResource(Image::Create());
            imageProvider->CreateResourceFromFile(imageLocation, imagePaths[i], nullptr, imageResource);
            ImageSPtr image(std::static_pointer_cast<Image>(imageResource));
            
            if(image->GetLoadState() == Resource::LoadState::k_failed)
            {
                CS_LOG_ERROR("Failed to load image " + in_filePath);
                out_resource->SetLoadState(Resource::LoadState::k_failed);
                if(in_delegate != nullptr)
                {
                    Application::Get()->GetTaskScheduler()->ScheduleTask(TaskType::k_mainThread, [=](const TaskContext&) noexcept { in_delegate(out_resource); });
                }
                return;
            }
            
            images[i] = image;
        }
        
        if(in_delegate == nullptr)
        {
            auto cubemap = static_cast<Cubemap*>(out_resource.get());
            auto options = static_cast<const CubemapResourceOptions*>(in_options.get());
            
            std::array<std::unique_ptr<const u8[]>, 6> textureData;
            for(auto i=0; i<images.size(); ++i)
            {
                textureData[i] = std::move(images[i]->MoveData());
            }
            
            TextureDesc desc(Integer2(images[0]->GetWidth(), images[0]->GetHeight()), images[0]->GetFormat(), images[0]->GetCompression(), false);
            desc.SetFilterMode(options->GetFilterMode());
            desc.SetWrapModeS(options->GetWrapModeS());
            desc.SetWrapModeT(options->GetWrapModeT());
            desc.SetMipmappingEnabled(options->IsMipMapsEnabled());
            
            cubemap->Build(std::move(textureData), images[0]->GetDataSize(), desc);
            cubemap->SetLoadState(Resource::LoadState::k_loaded);
        }
        else
        {
            Application::Get()->GetTaskScheduler()->ScheduleTask(TaskType::k_mainThread, [=](const TaskContext&) noexcept
             {
                 auto cubemap = static_cast<Cubemap*>(out_resource.get());
                 auto options = static_cast<const CubemapResourceOptions*>(in_options.get());
                 
                 std::array<std::unique_ptr<const u8[]>, 6> textureData;
                 for(auto i=0; i<images.size(); ++i)
                 {
                     textureData[i] = std::move(images[i]->MoveData());
                 }
                 
                 TextureDesc desc(Integer2(images[0]->GetWidth(), images[0]->GetHeight()), images[0]->GetFormat(), images[0]->GetCompression(), false);
                 desc.SetFilterMode(options->GetFilterMode());
                 desc.SetWrapModeS(options->GetWrapModeS());
                 desc.SetWrapModeT(options->GetWrapModeT());
                 desc.SetMipmappingEnabled(options->IsMipMapsEnabled());
                 
                 cubemap->Build(std::move(textureData), images[0]->GetDataSize(), desc);
                 cubemap->SetLoadState(Resource::LoadState::k_loaded);
                 in_delegate(out_resource);
             });
        }
    }
}

