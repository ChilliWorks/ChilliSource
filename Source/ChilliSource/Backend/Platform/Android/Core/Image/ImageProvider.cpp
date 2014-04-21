//
//  ImageProvider.cpp
//  Chilli Source
//  Created by I Copland on 05/02/2011.
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

#include <ChilliSource/Backend/Platform/Android/Core/Image/ImageProvider.h>

#include <ChilliSource/Backend/Platform/Android/Core/Image/PngImage.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Image/Image.h>
#include <ChilliSource/Core/Image/ImageCompression.h>
#include <ChilliSource/Core/Image/ImageFormat.h>
#include <ChilliSource/Core/String/StringUtils.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>

namespace ChilliSource
{
	namespace Android
	{
		namespace
		{
			const std::string k_pngExtension("png");

            //-----------------------------------------------------------
            /// Performs the heavy lifting for the 2 create methods
            ///
            /// @author I Copland
            ///
            /// @param The storage location.
            /// @param The filepath.
            /// @param Completion delegate
            /// @param [Out] The output resource
            //-----------------------------------------------------------
			void CreatePNGImageFromFile(Core::StorageLocation in_storageLocation, const std::string& in_filepath, const Core::ResourceProvider::AsyncLoadDelegate& in_delegate, const Core::ResourceSPtr& out_resource)
			{
				Core::Image* imageResource = (Core::Image*)(out_resource.get());

				//load the png image
				PngImage image;
				image.Load(in_storageLocation, in_filepath);

				//check the image has loaded
				if(image.IsLoaded() == false)
				{
					image.Release();
					CS_LOG_ERROR("Failed to load image: " + in_filepath);
					imageResource->SetLoadState(Core::Resource::LoadState::k_failed);
	                if(in_delegate != nullptr)
	                {
	                    Core::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(std::bind(in_delegate, out_resource));
	                }
					return;
				}

                Core::Image::Descriptor desc;
                desc.m_compression = Core::ImageCompression::k_none;
                desc.m_format = image.GetImageFormat();
                desc.m_width = image.GetWidth();
                desc.m_height = image.GetHeight();
                desc.m_dataSize = image.GetDataSize();
                imageResource->Build(desc, Core::Image::ImageDataUPtr(image.GetImageData()));

				//release the png image without deallocating the image data
				image.Release(false);

				imageResource->SetLoadState(Core::Resource::LoadState::k_loaded);
                if(in_delegate != nullptr)
                {
                	Core::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(std::bind(in_delegate, out_resource));
                }
			}
		}

		CS_DEFINE_NAMEDTYPE(ImageProvider);
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		bool ImageProvider::IsA(Core::InterfaceIDType in_interfaceId) const
		{
			return (in_interfaceId == Core::ResourceProvider::InterfaceID || in_interfaceId == Core::ImageProvider::InterfaceID || in_interfaceId == ImageProvider::InterfaceID);
		}
        //-------------------------------------------------------
        //-------------------------------------------------------
        Core::InterfaceIDType ImageProvider::GetResourceType() const
        {
            return Core::Image::InterfaceID;
        }
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		bool ImageProvider::CanCreateResourceWithFileExtension(const std::string& in_extension) const
		{
			return (in_extension == k_pngExtension);
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		void ImageProvider::CreateResourceFromFile(Core::StorageLocation in_storageLocation, const std::string& in_filepath, const Core::ResourceSPtr& out_resource)
		{
			CreatePNGImageFromFile(in_storageLocation, in_filepath, nullptr, out_resource);
		}
		//----------------------------------------------------
		//----------------------------------------------------
		void ImageProvider::CreateResourceFromFileAsync(Core::StorageLocation in_storageLocation, const std::string& in_filePath, const Core::ResourceProvider::AsyncLoadDelegate& in_delegate, const Core::ResourceSPtr& out_resource)
		{
            auto task = std::bind(CreatePNGImageFromFile, in_storageLocation, in_filePath, in_delegate, out_resource);
            Core::Application::Get()->GetTaskScheduler()->ScheduleTask(task);
		}
	}
}
