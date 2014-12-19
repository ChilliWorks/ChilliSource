//
//  LocalisedTextProvider.cpp
//  Chilli Source
//  Created by Scott Downie on 25/04/2014.
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

#include <ChilliSource/Core/Localisation/LocalisedTextProvider.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/Device.h>
#include <ChilliSource/Core/Json/JsonUtils.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>
#include <ChilliSource/Core/Localisation/LocalisedText.h>

#include <json/json.h>

namespace ChilliSource
{
	namespace Core
	{
		namespace
		{
            const u32 k_version = 1;
			const std::string k_fileExtension("cstext");
            
			//----------------------------------------------------
			/// Performs the heavy lifting for the 2 create methods
			/// by loading the keys and text files into a single resource
			///
			/// @author S Downie
			///
			/// @param The storage location.
			/// @param The filepath.
			/// @param Completion delegate
			/// @param [Out] The output resource.
			//----------------------------------------------------
			void LoadResource(StorageLocation in_storageLocation, const std::string& in_filePath, const ResourceProvider::AsyncLoadDelegate& in_delegate, const ResourceSPtr& out_resource)
			{
                LocalisedText* textResource((LocalisedText*)out_resource.get());
                
                Json::Value jsonRoot;
                if (Core::JsonUtils::ReadJson(in_storageLocation, in_filePath, jsonRoot) == false)
                {
                    CS_LOG_ERROR("Cannot read cstext file: " + in_filePath);
                    textResource->SetLoadState(Resource::LoadState::k_failed);
                    if(in_delegate != nullptr)
                    {
                        Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(std::bind(in_delegate, out_resource));
                    }
                    return;
                }
                
                
                auto jsonVersion = jsonRoot.get("Version", Json::nullValue);
                auto jsonText = jsonRoot.get("Text", Json::nullValue);
                
                if (jsonVersion.isNull() || jsonText.isNull())
                {
                    CS_LOG_ERROR("Cannot read cstext file: " + in_filePath);
                    textResource->SetLoadState(Resource::LoadState::k_failed);
                    if(in_delegate != nullptr)
                    {
                        Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(std::bind(in_delegate, out_resource));
                    }
                    return;
                }
                
                if (jsonVersion.asInt() != k_version)
                {
                    CS_LOG_ERROR("Invalid cstext file version: " + in_filePath);
                    textResource->SetLoadState(Resource::LoadState::k_failed);
                    if(in_delegate != nullptr)
                    {
                        Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(std::bind(in_delegate, out_resource));
                    }
                    return;
                }
                
                std::unordered_map<std::string, std::string> map;
                for (const auto& member : jsonText.getMemberNames())
                {
                    map.emplace(member, jsonText[member].asString());
                }
                
                textResource->Build(map);
                textResource->SetLoadState(Resource::LoadState::k_loaded);
                
                if(in_delegate != nullptr)
                {
                    Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(std::bind(in_delegate, out_resource));
                }
			}
		}

		CS_DEFINE_NAMEDTYPE(LocalisedTextProvider);

		//-------------------------------------------------------
		//-------------------------------------------------------
		LocalisedTextProviderUPtr LocalisedTextProvider::Create()
		{
			return LocalisedTextProviderUPtr(new LocalisedTextProvider());
		}
        //----------------------------------------------------
        //----------------------------------------------------
        bool LocalisedTextProvider::IsA(InterfaceIDType in_interfaceId) const
        {
            return in_interfaceId == ResourceProvider::InterfaceID || in_interfaceId == LocalisedTextProvider::InterfaceID;
        }
		//----------------------------------------------------
		//----------------------------------------------------
		InterfaceIDType LocalisedTextProvider::GetResourceType() const
		{
			return LocalisedText::InterfaceID;
		}
		//----------------------------------------------------
		//----------------------------------------------------
		bool LocalisedTextProvider::CanCreateResourceWithFileExtension(const std::string& in_extension) const
		{
			return in_extension == k_fileExtension;
		}
		//----------------------------------------------------
		//----------------------------------------------------
		void LocalisedTextProvider::CreateResourceFromFile(StorageLocation in_storageLocation, const std::string& in_filePath, const IResourceOptionsBaseCSPtr& in_options, const ResourceSPtr& out_resource)
		{
            LoadResource(in_storageLocation, in_filePath, nullptr, out_resource);
		}
		//----------------------------------------------------
		//----------------------------------------------------
		void LocalisedTextProvider::CreateResourceFromFileAsync(StorageLocation in_storageLocation, const std::string& in_filePath, const IResourceOptionsBaseCSPtr& in_options, const AsyncLoadDelegate& in_delegate, const ResourceSPtr& out_resource)
		{
            Application::Get()->GetTaskScheduler()->ScheduleTask(std::bind(LoadResource, in_storageLocation, in_filePath, in_delegate, out_resource));
		}
	}
}
