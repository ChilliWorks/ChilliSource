//
//  ForwardDeclarations.h
//  Chilli Source
//  Created by Ian Copland on 09/08/2012.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2012 Tag Games Limited
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

#include <CSBackend/Platform/Android/Main/JNI/Audio/CricketAudio/ZippedCkBankLoader.h>

#include <ChilliSource/Audio/CricketAudio/CkBank.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>

#include <ck/ck.h>
#include <ck/bank.h>

namespace CSBackend
{
	namespace Android
	{
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		void ZippedCkBankLoader::Load(ChilliSource::StorageLocation in_storageLocation, const std::string& in_filePath, const ChilliSource::ResourceSPtr& out_resource) const
		{
			CS_ASSERT(ChilliSource::Application::Get()->GetTaskScheduler()->IsMainThread() == true, "Must be called from the main thread.");

			auto fileSystem = ChilliSource::Application::Get()->GetFileSystem();

			auto stream = fileSystem->CreateFileStream(in_storageLocation, in_filePath, ChilliSource::FileMode::k_readBinary);
			if (stream == nullptr)
			{
				out_resource->SetLoadState(ChilliSource::Resource::LoadState::k_failed);
				return;
			}

			stream->SeekG(0, ChilliSource::SeekDir::k_end);
			u32 length = stream->TellG();
			stream->SeekG(0, ChilliSource::SeekDir::k_beginning);

			std::unique_ptr<u8[]> bankBuffer(new u8[length]);
			stream->Read(reinterpret_cast<s8*>(bankBuffer.get()), length);
			stream.reset();

			::CkBank* bank = ::CkBank::newBankFromMemory(reinterpret_cast<void*>(bankBuffer.get()), length);
			if (bank == nullptr)
			{
				out_resource->SetLoadState(ChilliSource::Resource::LoadState::k_failed);
				return;
			}

			static_cast<ChilliSource::CkBank*>(out_resource.get())->Build(bank, std::move(bankBuffer));
			out_resource->SetLoadState(ChilliSource::Resource::LoadState::k_loaded);
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		void ZippedCkBankLoader::LoadAsync(ChilliSource::StorageLocation in_storageLocation, const std::string& in_filePath, const ChilliSource::ResourceProvider::AsyncLoadDelegate& in_delegate, const ChilliSource::ResourceSPtr& out_resource)
		{
			CS_ASSERT(ChilliSource::Application::Get()->GetTaskScheduler()->IsMainThread() == true, "Must be called from the main thread.");

			auto taskScheduler = ChilliSource::Application::Get()->GetTaskScheduler();
			taskScheduler->ScheduleTask(ChilliSource::TaskType::k_file, [=](const ChilliSource::TaskContext&)
			{
				auto fileSystem = ChilliSource::Application::Get()->GetFileSystem();
				auto stream = fileSystem->CreateFileStream(in_storageLocation, in_filePath, ChilliSource::FileMode::k_readBinary);
				if (stream == nullptr)
				{
					taskScheduler->ScheduleTask(ChilliSource::TaskType::k_mainThread, [=](const ChilliSource::TaskContext&)
					{
						out_resource->SetLoadState(ChilliSource::Resource::LoadState::k_failed);
						in_delegate(out_resource);
					});
				}

				stream->SeekG(0, ChilliSource::SeekDir::k_end);
				u32 length = stream->TellG();
				stream->SeekG(0, ChilliSource::SeekDir::k_beginning);

				std::unique_ptr<u8[]> bankBuffer(new u8[length]);
				stream->Read(reinterpret_cast<s8*>(bankBuffer.get()), length);
				stream.reset();

				//we cannot capture the unique pointer in the main thread task, so store it and
				//retain a handle to it so it can be retrieved later.
				u8* bufferHandle = bankBuffer.get();
				StoreBuffer(std::move(bankBuffer));

				taskScheduler->ScheduleTask(ChilliSource::TaskType::k_mainThread, [=](const ChilliSource::TaskContext&)
                {
					std::unique_ptr<u8[]> retrievedBuffer = RetrieveBuffer(bufferHandle);

					::CkBank* bank = ::CkBank::newBankFromMemory(reinterpret_cast<void*>(retrievedBuffer.get()), length);
					if (bank != nullptr)
					{
						static_cast<ChilliSource::CkBank*>(out_resource.get())->Build(bank, std::move(retrievedBuffer));
						out_resource->SetLoadState(ChilliSource::Resource::LoadState::k_loaded);
						in_delegate(out_resource);
					}
					else
					{
						out_resource->SetLoadState(ChilliSource::Resource::LoadState::k_failed);
						in_delegate(out_resource);
					}
                });
			});
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		void ZippedCkBankLoader::StoreBuffer(std::unique_ptr<u8[]> in_buffer)
		{
			std::unique_lock<std::mutex> lock(m_mutex);

			m_bankBuffers.push_back(std::move(in_buffer));
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		std::unique_ptr<u8[]> ZippedCkBankLoader::RetrieveBuffer(u8* in_bufferHandle)
		{
			std::unique_lock<std::mutex> lock(m_mutex);

			auto it = std::find_if(m_bankBuffers.begin(), m_bankBuffers.end(), [=](const std::unique_ptr<u8[]>& in_buffer)
			{
				return (in_buffer.get() == in_bufferHandle);
			});

			CS_ASSERT(it != m_bankBuffers.end(), "The buffer wasn't stored.");

			auto output = std::move(*it);
			m_bankBuffers.erase(it);
			return output;
		}
	}
}

#endif
