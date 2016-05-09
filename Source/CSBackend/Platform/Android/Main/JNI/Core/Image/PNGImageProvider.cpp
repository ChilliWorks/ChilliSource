//
//  PNGImageProvider.cpp
//  Chilli Source
//  Created by Ian Copland on 05/02/2011.
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

#ifdef CS_TARGETPLATFORM_ANDROID

#include <CSBackend/Platform/Android/Main/JNI/Core/Image/PNGImageProvider.h>

#include <CSBackend/Platform/Android/Main/JNI/Core/Image/PngImage.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Image/Image.h>
#include <ChilliSource/Core/Image/ImageCompression.h>
#include <ChilliSource/Core/Image/ImageFormat.h>
#include <ChilliSource/Core/String/StringUtils.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>

namespace CSBackend
{
	namespace Android
	{
		namespace
		{
			const std::string k_pngExtension("png");

            //-----------------------------------------------------------
            /// Performs the heavy lifting for the 2 create methods
            ///
            /// @author Ian Copland
            ///
            /// @param The storage location.
            /// @param The filepath.
            /// @param Completion delegate
            /// @param [Out] The output resource
            //-----------------------------------------------------------
			void CreatePNGImageFromFile(ChilliSource::StorageLocation in_storageLocation, const std::string& in_filepath, const ChilliSource::ResourceProvider::AsyncLoadDelegate& in_delegate, const ChilliSource::ResourceSPtr& out_resource)
			{
				ChilliSource::Image* imageResource = (ChilliSource::Image*)(out_resource.get());

				//load the png image
				PngImage image;
				image.Load(in_storageLocation, in_filepath);

				//check the image has loaded
				if(image.IsLoaded() == false)
				{
					image.Release();
					CS_LOG_ERROR("Failed to load image: " + in_filepath);
					imageResource->SetLoadState(ChilliSource::Resource::LoadState::k_failed);
	                if(in_delegate != nullptr)
	                {
	                    ChilliSource::Application::Get()->GetTaskScheduler()->ScheduleTask(ChilliSource::TaskType::k_mainThread, [=](const ChilliSource::TaskContext&)
	                    {
	                    	in_delegate(out_resource);
	                    });
	                }
					return;
				}

                ChilliSource::Image::Descriptor desc;
                desc.m_compression = ChilliSource::ImageCompression::k_none;
                desc.m_format = image.GetImageFormat();
                desc.m_width = image.GetWidth();
                desc.m_height = image.GetHeight();
                desc.m_dataSize = image.GetDataSize();
                imageResource->Build(desc, ChilliSource::Image::ImageDataUPtr(image.GetImageData()));

				//release the png image without deallocating the image data
				image.Release(false);

				imageResource->SetLoadState(ChilliSource::Resource::LoadState::k_loaded);
                if(in_delegate != nullptr)
                {
                    ChilliSource::Application::Get()->GetTaskScheduler()->ScheduleTask(ChilliSource::TaskType::k_mainThread, [=](const ChilliSource::TaskContext&)
                    {
                        in_delegate(out_resource);
                    });
                }
			}
		}

		CS_DEFINE_NAMEDTYPE(PNGImageProvider);
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		bool PNGImageProvider::IsA(ChilliSource::InterfaceIDType in_interfaceId) const
		{
			return (in_interfaceId == ChilliSource::ResourceProvider::InterfaceID || in_interfaceId == ChilliSource::PNGImageProvider::InterfaceID || in_interfaceId == PNGImageProvider::InterfaceID);
		}
        //-------------------------------------------------------
        //-------------------------------------------------------
        ChilliSource::InterfaceIDType PNGImageProvider::GetResourceType() const
        {
            return ChilliSource::Image::InterfaceID;
        }
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		bool PNGImageProvider::CanCreateResourceWithFileExtension(const std::string& in_extension) const
		{
			return (in_extension == k_pngExtension);
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		void PNGImageProvider::CreateResourceFromFile(ChilliSource::StorageLocation in_storageLocation, const std::string& in_filepath, const ChilliSource::IResourceOptionsBaseCSPtr& in_options, const ChilliSource::ResourceSPtr& out_resource)
		{
			CreatePNGImageFromFile(in_storageLocation, in_filepath, nullptr, out_resource);
		}
		//----------------------------------------------------
		//----------------------------------------------------
		void PNGImageProvider::CreateResourceFromFileAsync(ChilliSource::StorageLocation in_storageLocation, const std::string& in_filePath, const ChilliSource::IResourceOptionsBaseCSPtr& in_options, const ChilliSource::ResourceProvider::AsyncLoadDelegate& in_delegate, const ChilliSource::ResourceSPtr& out_resource)
		{
            ChilliSource::Application::Get()->GetTaskScheduler()->ScheduleTask(ChilliSource::TaskType::k_file, [=](const ChilliSource::TaskContext&)
            {
                CreatePNGImageFromFile(in_storageLocation, in_filePath, in_delegate, out_resource);
            });
		}
	}
}

#endif