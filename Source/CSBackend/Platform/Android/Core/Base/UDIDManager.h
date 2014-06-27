//
//  UDIDManager.h
//  Chilli Source
//  Created by Ian Copland on 01/12/2011.
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

#ifndef _CHILLISOURCE_PLATFORM_ANDROID_UDID_MANAGER_H_
#define _CHILLISOURCE_PLATFORM_ANDROID_UDID_MANAGER_H_

#include <string>

namespace CSBackend
{
	namespace Android
	{
		//==================================================
		/// UDID Manager
		///
		/// Calculates, stores and manages a UDID for a
		/// device
		//==================================================
		class UDIDManager
		{
		public:
			UDIDManager();
			virtual ~UDIDManager();
			//-----------------------------------------
			/// Get UDID
			///
			/// @return The UDID
			//-----------------------------------------
			const std::string& GetUDID();

		private:
			//-----------------------------------------
			/// Initialise
			///
			/// Initialises and calculates the UDID
			//-----------------------------------------
			void Initialise();
			//-----------------------------------------
			/// Load UDID
			///
			/// Tries to load a previously calculated
			/// UDID.
			///
			/// @return whether or not loading the UDID
			///			was successful
			//-----------------------------------------
			bool LoadUDID();
			//-----------------------------------------
			/// Save UDID
			///
			/// Saves a newly calculated UDID
			//-----------------------------------------
			void SaveUDID();
			//-----------------------------------------
			/// Calculate UDID
			///
			/// Calculates the UDID
			//-----------------------------------------
			void CalculateUDID();

			std::string mstrUDID;
			bool mbInitialised;
		};
	}
}

#endif

#endif
