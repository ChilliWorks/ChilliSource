/*
 *  UDIDManager.h
 *  MoFlow
 *
 *  Created by Ian Copland on 01/12/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

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