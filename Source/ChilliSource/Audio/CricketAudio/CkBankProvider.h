//
//  CkBankProvider.h
//  Chilli Source
//  Created by Ian Copland on 30/12/2014.
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

#ifndef _CHILLISOURCE_AUDIO_CRICKETAUDIO_CKAUDIOBANKPROVIDER_H_
#define _CHILLISOURCE_AUDIO_CRICKETAUDIO_CKAUDIOBANKPROVIDER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Audio/CricketAudio/CkForwardDeclarations.h>
#include <ChilliSource/Core/Container/concurrent_vector.h>
#include <ChilliSource/Core/Resource/ResourceProvider.h>

namespace ChilliSource
{
	namespace Audio
	{
		//------------------------------------------------------------------------------
		/// The resource provider for Cricket Audio sound banks. An audio bank contains
		/// a number of audio effects that are loaded into memory as a batch. Specific 
		/// sounds within a bank can be played by creating a CkSound with the bank, or 
		/// through the CKAudioPlayer.
		///
        /// This is not added to Application by default. If you need to load bank
        /// resources you will have to add this during the Application::AddSystems()
        /// life cycle event.
        ///
		/// Cricket Technology has kindly allows us to include the Cricket Audio SDK
		/// in the engine under the free license. For more information see the
		/// documentation for CricketAudioSystem.
		///
		/// @author Ian Copland
		//------------------------------------------------------------------------------
		class CkBankProvider final : public Core::ResourceProvider
		{
		public:
			CS_DECLARE_NAMEDTYPE(CkBankProvider);
			//------------------------------------------------------------------------------
			/// Allows querying of whether or not this system implements the interface
			/// described by the given interface Id. Typically this is not called directly
			/// as the templated equivalent IsA<Interface>() is preferred.
			///
			/// @author Ian Copland
			///
			/// @param The interface Id.
			///
			/// @return Whether or not the interface is implemented.
			//------------------------------------------------------------------------------
			bool IsA(Core::InterfaceIDType in_interfaceId) const override;
			//------------------------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @return The interface Id for the resource type that this provider can 
			/// create.
			//------------------------------------------------------------------------------
			Core::InterfaceIDType GetResourceType() const override;
			//------------------------------------------------------------------------------
			/// Allows querying of whether or not this system create resources from files 
			/// with the given extension. 
			///
			/// @author Ian Copland
			///
			/// @param The extension of the resource file. This is case insensitive.
			///
			/// @return Whether or not the resource can be created.
			//------------------------------------------------------------------------------
			bool CanCreateResourceWithFileExtension(const std::string& in_extension) const override;
			//------------------------------------------------------------------------------
			/// Creates a new Cricket Audio bank from the described file. The load state of 
			/// the resource should be checked for success or failure.
			///
			/// @author Ian Copland
			///
			/// @param The storage location.
			/// @param The filepath.
			/// @param The options to customise the creation. This should always be null for
			/// an audio bank.
			/// @param [Out] The output audio bank resource.
			//------------------------------------------------------------------------------
			void CreateResourceFromFile(Core::StorageLocation in_storageLocation, const std::string& in_filePath, const Core::IResourceOptionsBaseCSPtr& in_options, const Core::ResourceSPtr& out_resource) override;
			//------------------------------------------------------------------------------
			/// Creates a new Cricket Audio bank from the described file asynchronously. The 
			/// load state of the resource should be checked for success or failure.
			///
			/// @author Ian Copland
			///
			/// @param The storage location.
			/// @param The filepath.
			/// @param The options to customise the creation. This should always be null for
			/// an audio bank.
			/// @param The completion delegate.
			/// @param [Out] The output audio bank resource.
			//------------------------------------------------------------------------------
			void CreateResourceFromFileAsync(Core::StorageLocation in_storageLocation, const std::string& in_filePath, const Core::IResourceOptionsBaseCSPtr& in_options, const AsyncLoadDelegate& in_delegate, const Core::ResourceSPtr& out_resource) override;

		private:
            friend class Core::Application;
            //------------------------------------------------------------------------------
            /// A container for information needed for asynchonous load requests.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            struct AsyncRequest
            {
                ::CkBank* m_bank = nullptr;
                CkBankSPtr m_bankResource;
                AsyncLoadDelegate m_delegate;
            };
			//------------------------------------------------------------------------------
			/// A factory method for creating a new instance of the system.
			///
			/// @author Ian Copland
			///
			/// @return The new instance.
			//------------------------------------------------------------------------------
			static CkBankProviderUPtr Create();
			//------------------------------------------------------------------------------
			/// Default Constructor. This is private to ensure this can only be created
			/// through Application::CreateSystem().
			///
			/// @author Ian Copland
			//------------------------------------------------------------------------------
			CkBankProvider() = default;
            //------------------------------------------------------------------------------
            /// Called when app systems are initialised. This simply confirms that the
            /// CricketAudioSystem exists.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            void OnInit() override;
            //------------------------------------------------------------------------------
            /// Polls to check if background loaded banks are finished loading.
            ///
            /// @author Ian Copland
            ///
            /// @param The delta time.
            //------------------------------------------------------------------------------
            void OnUpdate(f32 in_deltaTime) override;
            
            Core::concurrent_vector<AsyncRequest> m_asyncRequests;
		};
	}
}

#endif
