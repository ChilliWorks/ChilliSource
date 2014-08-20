//
//  LuaSourceProvider.cpp
//  Chilli Source
//  Created by Scott Downie on 20/08/2014.
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

#include <ChilliSource/Scripting/Lua/LuaSourceProvider.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/Utils.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>
#include <ChilliSource/Scripting/Lua/LuaSource.h>

namespace ChilliSource
{
	namespace Scripting
	{
        namespace
        {
            const std::string k_fileExtension("lua");
        }
		
        CS_DEFINE_NAMEDTYPE(LuaSourceProvider);
        
        //-------------------------------------------------------
        //-------------------------------------------------------
        LuaSourceProviderUPtr LuaSourceProvider::Create()
        {
            return LuaSourceProviderUPtr(new LuaSourceProvider());
        }
		//-------------------------------------------------------------------------e
		//-------------------------------------------------------------------------
		bool LuaSourceProvider::IsA(Core::InterfaceIDType in_interfaceId) const
		{
			return in_interfaceId == ResourceProvider::InterfaceID || in_interfaceId == LuaSourceProvider::InterfaceID;
		}
        //----------------------------------------------------------------------------
        //----------------------------------------------------------------------------
        Core::InterfaceIDType LuaSourceProvider::GetResourceType() const
        {
            return LuaSource::InterfaceID;
        }
		//----------------------------------------------------------------------------
		//----------------------------------------------------------------------------
		bool LuaSourceProvider::CanCreateResourceWithFileExtension(const std::string& in_extension) const
		{
			return in_extension == k_fileExtension;
		}
		//----------------------------------------------------------------------------
		//----------------------------------------------------------------------------
		void LuaSourceProvider::CreateResourceFromFile(Core::StorageLocation in_location, const std::string& in_filePath, const Core::IResourceOptionsBaseCSPtr& in_options, const Core::ResourceSPtr& out_resource)
		{
            LoadLuaSource(in_location, in_filePath, nullptr, out_resource);
		}
        //----------------------------------------------------------------------------
        //----------------------------------------------------------------------------
		void LuaSourceProvider::CreateResourceFromFileAsync(Core::StorageLocation in_location, const std::string& in_filePath, const Core::IResourceOptionsBaseCSPtr& in_options, const Core::ResourceProvider::AsyncLoadDelegate& in_delegate, const Core::ResourceSPtr& out_resource)
        {
			auto task = std::bind(&LuaSourceProvider::LoadLuaSource, this, in_location, in_filePath, in_delegate, out_resource);
            Core::Application::Get()->GetTaskScheduler()->ScheduleTask(task);
        }
        //----------------------------------------------------------------------------
        //----------------------------------------------------------------------------
		void LuaSourceProvider::LoadLuaSource(Core::StorageLocation in_location, const std::string& in_filePath, const Core::ResourceProvider::AsyncLoadDelegate& in_delegate, const Core::ResourceSPtr& out_resource)
        {
            std::string luaContents;
            bool result = Core::Utils::FileToString(in_location, in_filePath, luaContents);
            
            if(result == true)
            {
                LuaSource* source = (LuaSource*)(out_resource.get());
                source->Build(luaContents);
                out_resource->SetLoadState(Core::Resource::LoadState::k_loaded);
            }
            else
            {
                out_resource->SetLoadState(Core::Resource::LoadState::k_failed);
            }
            
            if(in_delegate != nullptr)
            {
                in_delegate(out_resource);
            }
        }
	}
}
