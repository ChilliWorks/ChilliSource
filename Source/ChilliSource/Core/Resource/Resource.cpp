//
//  Resource.cpp
//  Chilli Source
//  Created by Scott Downie on 30/09/2010.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2010 Tag Games Limited
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

#include <ChilliSource/Core/Resource/Resource.h>

#include <ChilliSource/Core/File/StorageLocation.h>

namespace ChilliSource
{
	namespace Core
	{
		//-------------------------------------------------------
		//-------------------------------------------------------
		Resource::Resource()
        : m_location(StorageLocation::k_none), m_loadState(LoadState::k_loading)
		{
		}
        //-------------------------------------------------------
        //-------------------------------------------------------
        Resource::LoadState Resource::GetLoadState() const
        {
            return m_loadState;
        }
        //-------------------------------------------------------
        /// If we set the load state to loaded or failed
        /// we need to notify any waiting parties that the resource
        /// is now ready
        //-------------------------------------------------------
        void Resource::SetLoadState(LoadState in_loadState)
        {
            m_loadState = in_loadState;
        }
		//-------------------------------------------------------
		//-------------------------------------------------------
		void Resource::SetFilePath(const std::string& in_filePath)
		{
			m_filePath = in_filePath;
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		const std::string& Resource::GetFilePath() const
		{
			return m_filePath;
		}
        //-------------------------------------------------------
        //-------------------------------------------------------
        void Resource::SetName(const std::string& in_name)
        {
            m_name = in_name;
        }
        //-------------------------------------------------------
		//-------------------------------------------------------
		const std::string& Resource::GetName() const
		{
			return m_name;
		}
        //-------------------------------------------------------
        //-------------------------------------------------------
        void Resource::SetId(ResourceId in_id)
        {
            m_id = in_id;
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        Resource::ResourceId Resource::GetId() const
        {
            return m_id;
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        void Resource::SetOptions(const IResourceOptionsBaseCSPtr& in_options)
        {
            m_options = in_options;
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        const IResourceOptionsBaseCSPtr& Resource::GetOptions() const
        {
            return m_options;
        }
		//-------------------------------------------------------
		//-------------------------------------------------------
		void Resource::SetStorageLocation(StorageLocation in_location)
		{
			m_location = in_location;
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
        StorageLocation Resource::GetStorageLocation() const
		{
			return m_location;
		}
	}
}
