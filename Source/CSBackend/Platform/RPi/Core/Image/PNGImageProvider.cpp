//
//  The MIT License (MIT)
//
//  Copyright (c) 2017 Tag Games Limited
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

#ifdef CS_TARGETPLATFORM_RPI

#include <CSBackend/Platform/RPi/Core/Image/PNGImageProvider.h>

#include <CSBackend/Platform/RPi/Core/Image/PngImage.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Image/Image.h>
#include <ChilliSource/Core/Image/ImageCompression.h>
#include <ChilliSource/Core/Image/ImageFormat.h>
#include <ChilliSource/Core/String/StringUtils.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>

namespace CSBackend
{
	namespace RPi
	{
		namespace
		{
			const std::string k_pngExtension("png");

			///
			/// Performs the heavy lifting for the 2 create methods
			///
			/// @author Ian Copland
			///
			/// @param storageLocation
			///		The storage location.
			/// @param filePath
			///		The filepath.
			/// @param delegate
			///		Completion delegate
			/// @param [Out] out_resource
			///		The output resource
			///
			void CreatePNGImageFromFile(ChilliSource::StorageLocation storageLocation, const std::string& filepath, const ChilliSource::ResourceProvider::AsyncLoadDelegate& delegate, const ChilliSource::ResourceSPtr& out_resource)
			{
				ChilliSource::Image* imageResource = (ChilliSource::Image*)(out_resource.get());

				//load the png image
				PngImage image;
				image.Load(storageLocation, filepath);

				//check the image has loaded
				if (image.IsLoaded() == false)
				{
					image.Release();
					CS_LOG_ERROR("Failed to load image: " + filepath);
					imageResource->SetLoadState(ChilliSource::Resource::LoadState::k_failed);
					if (delegate != nullptr)
					{
						ChilliSource::Application::Get()->GetTaskScheduler()->ScheduleTask(ChilliSource::TaskType::k_mainThread, [=](const ChilliSource::TaskContext&)
						{
							delegate(out_resource);
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
				if (delegate != nullptr)
				{
					ChilliSource::Application::Get()->GetTaskScheduler()->ScheduleTask(ChilliSource::TaskType::k_mainThread, [=](const ChilliSource::TaskContext&)
					{
						delegate(out_resource);
					});
				}
			}
		}

		CS_DEFINE_NAMEDTYPE(PNGImageProvider);

		//---------------------------------------------------------------------------------
		bool PNGImageProvider::IsA(ChilliSource::InterfaceIDType interfaceId) const
		{
			return (interfaceId == ChilliSource::ResourceProvider::InterfaceID || interfaceId == ChilliSource::PNGImageProvider::InterfaceID || interfaceId == PNGImageProvider::InterfaceID);
		}

		//---------------------------------------------------------------------------------
		ChilliSource::InterfaceIDType PNGImageProvider::GetResourceType() const
		{
			return ChilliSource::Image::InterfaceID;
		}

		//---------------------------------------------------------------------------------
		bool PNGImageProvider::CanCreateResourceWithFileExtension(const std::string& extension) const
		{
			return (extension == k_pngExtension);
		}

		//---------------------------------------------------------------------------------
		void PNGImageProvider::CreateResourceFromFile(ChilliSource::StorageLocation storageLocation, const std::string& filepath, const ChilliSource::IResourceOptionsBaseCSPtr& options, const ChilliSource::ResourceSPtr& out_resource)
		{
			CreatePNGImageFromFile(storageLocation, filepath, nullptr, out_resource);
		}

		//---------------------------------------------------------------------------------
		void PNGImageProvider::CreateResourceFromFileAsync(ChilliSource::StorageLocation storageLocation, const std::string& filePath, const ChilliSource::IResourceOptionsBaseCSPtr& options, const ChilliSource::ResourceProvider::AsyncLoadDelegate& delegate, const ChilliSource::ResourceSPtr& out_resource)
		{
			ChilliSource::Application::Get()->GetTaskScheduler()->ScheduleTask(ChilliSource::TaskType::k_file, [=](const ChilliSource::TaskContext&)
			{
				CreatePNGImageFromFile(storageLocation, filePath, delegate, out_resource);
			});
		}
	}
}

#endif
