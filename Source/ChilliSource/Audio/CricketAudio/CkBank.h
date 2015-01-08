//
//  CkBank.h
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

#ifndef _CHILLISOURCE_AUDIO_CRICKETAUDIO_CKAUDIOBANK_H_
#define _CHILLISOURCE_AUDIO_CRICKETAUDIO_CKAUDIOBANK_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Resource/Resource.h>
#include <ChilliSource/Audio/CricketAudio/CkForwardDeclarations.h>

namespace ChilliSource
{
	namespace Audio
	{
		//------------------------------------------------------------------------------
		/// A Cricket Audio bank resource. This contains a group of sounds which are
		/// loaded as a batch. Individual sounds can be played by creating a CkSound
		/// with a bank or through the CkAudioPlayer.
		///
		/// Cricket Technology has kindly allows us to include the Cricket Audio SDK
		/// in the engine under the free license. For more information see the
		/// documentation for CricketAudioSystem.
		///
		/// @author Ian Copland
		//------------------------------------------------------------------------------
		class CkBank final : public Core::Resource
		{
		public:
			CS_DECLARE_NAMEDTYPE(CkBank);
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
			/// Populates the resource with the Cricket Audio bank that it represents. This
			/// can only be called once, if a call to this is repeated the app is considered
			/// to be in an irrecoverable state and will terminate.
			///
			/// @author Ian Copland
			///
			/// @param The cricket audio sound bank that this resource represents. Cannot
			/// be null. This will take ownership of the bank and release it when it has
			/// finished with it.
			//------------------------------------------------------------------------------
			void Build(::CkBank* in_ckBank);
			//------------------------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @return A pointer to the underlying sound bank. This retains ownership of the
			/// bank so it must not be destroyed.
			//------------------------------------------------------------------------------
			::CkBank* GetBank() const;
			//------------------------------------------------------------------------------
			/// Destructor.
			///
			/// @author Ian Copland
			//------------------------------------------------------------------------------
			~CkBank();
		private:
			friend class Core::ResourcePool;
			//------------------------------------------------------------------------------
			/// Factory method for creating a new instance of the resource. 
			///
			/// @author Ian Copland
			///
			/// @return The new resource instance.
			//------------------------------------------------------------------------------
			static CkBankUPtr Create();
			//------------------------------------------------------------------------------
			/// Default constructor. This is declared private to ensure this is only ever
			/// created via the Resource Pool.
			///
			/// @author Ian Copland
			//------------------------------------------------------------------------------
			CkBank() = default;

			::CkBank* m_bank = nullptr;
		};
	}
}

#endif
