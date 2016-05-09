//
//  ZippedCkBankLoader.h
//  ChilliSource
//  Created by Ian Copland on 16/06/2015.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2015 Tag Games Limited
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

#ifndef _CSBACKEND_PLATFORM_ANDROID_MAIN_JNI_AUDIO_CRICKETAUDIO_H_
#define _CSBACKEND_PLATFORM_ANDROID_MAIN_JNI_AUDIO_CRICKETAUDIO_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Resource/ResourceProvider.h>

#include <CSBackend/Platform/Android/Main/JNI/ForwardDeclarations.h>

#include <mutex>

namespace CSBackend
{
	namespace Android
	{
		//------------------------------------------------------------------------------
		/// Cricket Audio doesn't provide any means to load from within the OBB, only
		/// APK. To get around that, this can be used. The methods provided will
		/// load the entire file into memory and create a CkBank from memory.
		///
		/// @author Ian Copland
		//------------------------------------------------------------------------------
		class ZippedCkBankLoader final
		{
		public:
		    CS_DECLARE_NOCOPY(ZippedCkBankLoader);
            //------------------------------------------------------------------------------
            /// Constructor
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            ZippedCkBankLoader() = default;
            //------------------------------------------------------------------------------
            /// Loads the entire file from the zip into memory then creates the CkBank from
            /// this memory buffer. This can technically be used for any of the storage
            /// locations, but for the sake of performance should only be used for Package,
            /// ChilliSource and DLC if not in Cached DLC on Android.
            ///
            /// This must be called from the main thread.
            ///
            /// @author Ian Copland
            ///
            /// @param in_storageLocation - The storage location of the file which will
            /// be loaded into memory.
            /// @param in_filePath - The file which should be loaded into memory.
            /// @param out_resource - [Out] The output resource. If this was successful it
            /// will have its load state set to loaded, otherwise it will be set to failed.
            //------------------------------------------------------------------------------
            void Load(ChilliSource::StorageLocation in_storageLocation, const std::string& in_filePath, const ChilliSource::ResourceSPtr& out_resource) const;
            //------------------------------------------------------------------------------
            /// Asynchronously loads the entire file from the zip into memory then creates
            /// the CkBank from this memory buffer. This can technically be used for any of
            /// the storage locations, but for the sake of performance should only be used
            /// for Package, ChilliSource and DLC if not in Cached DLC on Android.
            ///
            /// This must be called from the main thread. The loaded delegate will be
            /// called on the main thread.
            ///
            /// @author Ian Copland
            ///
            /// @param in_storageLocation - The storage location of the file which will
            /// be loaded into memory.
            /// @param in_filePath - The file which should be loaded into memory.
            /// @param out_resource - [Out] The output resource. If this was successful it
            /// will have its load state set to loaded, otherwise it will be set to failed.
            //------------------------------------------------------------------------------
            void LoadAsync(ChilliSource::StorageLocation in_storageLocation, const std::string& in_filePath, const ChilliSource::ResourceProvider::AsyncLoadDelegate& in_delegate,
                const ChilliSource::ResourceSPtr& out_resource);

		private:
            //------------------------------------------------------------------------------
            /// Stores the given buffer for later retrieval. This is needed to get around
            /// the fact that we cannot capture the unique pointer, instead it's stored
            /// here and retrieved again after.
            ///
            /// This is thread safe.
            ///
            /// @author Ian Copland
            ///
            /// @param in_buffer - The buffer to store.
            //------------------------------------------------------------------------------
            void StoreBuffer(std::unique_ptr<u8[]> in_buffer);
            //------------------------------------------------------------------------------
            /// Retrieves previously stored buffers using the pointer as a handle. This will
            /// assert if the buffer couldn't be found.
            ///
            /// This is thread safe.
            ///
            /// @author Ian Copland
            ///
            /// @param in_bufferHandle - The buffer to retrieve.
            ///
            /// @return The unique pointer to the buffer.
            //------------------------------------------------------------------------------
            std::unique_ptr<u8[]> RetrieveBuffer(u8* in_bufferHandle);

            std::vector<std::unique_ptr<u8[]>> m_bankBuffers;
            mutable std::mutex m_mutex;
		};
	}
}

#endif

#endif
