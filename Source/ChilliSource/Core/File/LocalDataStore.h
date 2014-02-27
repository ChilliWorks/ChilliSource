/*
 *  LocalDataStore.h
 *  moFlow
 *
 *  Created by Stuart McGaw on 24/05/2011.
 *  Modified by Robert Henning on 24/07/2013
 *   + Updated to use encryption
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 *  LocalDataStore provides a simple key-value data store
 *  useful for storing persistent data such as options
 *  or user log-in details.
 */

#ifndef _MOFLO_CORE_LOCALDATASTORE_H_
#define _MOFLO_CORE_LOCALDATASTORE_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Container/ParamDictionary.h>

namespace ChilliSource
{
	namespace Core
    {
		class CLocalDataStore
        {
            // Application is a friend as it is the only place
            // our data store is created and subscribes to the
            // suspend event
            friend class Application;
        
        public:
            //----------------------------------------------------------------
            /// Get Singleton
            ///
            /// Returns the singleton
            ///
            /// @return Reference to singleton
            //----------------------------------------------------------------
			static CLocalDataStore& GetSingleton();
            //----------------------------------------------------------------
            /// Get Singleton Pointer
            ///
            /// Returns the singleton
            ///
            /// @return Pointer to singleton
            //----------------------------------------------------------------
			static CLocalDataStore* GetSingletonPtr();
			//----------------------------------------------------------------
            /// Has Value For Key
            ///
            /// Returns if the given key exists in the current data store
            ///
            /// @return True if key exist, false otherwise
            //----------------------------------------------------------------
			bool HasValueForKey(const std::string& instrKey);
            //----------------------------------------------------------------
            /// Try Get Value
            ///
            /// Attempts to read the value for the given key from the data
            /// store as a string.
            ///
            /// @param Key to attempt to retrieve value for
            /// @param [Out] Value ready
            /// @return True if value has been read, false otherwise
            //----------------------------------------------------------------
			bool TryGetValue(const std::string& instrKey, std::string& outstrValue);
            //----------------------------------------------------------------
            /// Try Get Value
            ///
            /// Attempts to read the value for the given key from the data
            /// store as a bool.
            ///
            /// @param Key to attempt to retrieve value for
            /// @param [Out] Value ready
            /// @return True if value has been read, false otherwise
            //----------------------------------------------------------------
			bool TryGetValue(const std::string& instrKey, bool &outbValue);
            //----------------------------------------------------------------
            /// Try Get Value
            ///
            /// Attempts to read the value for the given key from the data
            /// store as a u16.
            ///
            /// @param Key to attempt to retrieve value for
            /// @param [Out] Value ready
            /// @return True if value has been read, false otherwise
            //----------------------------------------------------------------
			bool TryGetValue(const std::string& instrKey, u16 &outuwValue);
            //----------------------------------------------------------------
            /// Try Get Value
            ///
            /// Attempts to read the value for the given key from the data
            /// store as a s16.
            ///
            /// @param Key to attempt to retrieve value for
            /// @param [Out] Value ready
            /// @return True if value has been read, false otherwise
            //----------------------------------------------------------------
			bool TryGetValue(const std::string& instrKey, s16 &outwValue);
            //----------------------------------------------------------------
            /// Try Get Value
            ///
            /// Attempts to read the value for the given key from the data
            /// store as a u32.
            ///
            /// @param Key to attempt to retrieve value for
            /// @param [Out] Value ready
            /// @return True if value has been read, false otherwise
            //----------------------------------------------------------------
			bool TryGetValue(const std::string& instrKey, u32 &outudwValue);
            //----------------------------------------------------------------
            /// Try Get Value
            ///
            /// Attempts to read the value for the given key from the data
            /// store as a s32.
            ///
            /// @param Key to attempt to retrieve value for
            /// @param [Out] Value ready
            /// @return True if value has been read, false otherwise
            //----------------------------------------------------------------
			bool TryGetValue(const std::string& instrKey, s32 &outdwValue);
            //----------------------------------------------------------------
            /// Try Get Value
            ///
            /// Attempts to read the value for the given key from the data
            /// store as a u64.
            ///
            /// @param Key to attempt to retrieve value for
            /// @param [Out] Value ready
            /// @return True if value has been read, false otherwise
            //----------------------------------------------------------------
			bool TryGetValue(const std::string& instrKey, u64 &outuddwValue);
            //----------------------------------------------------------------
            /// Try Get Value
            ///
            /// Attempts to read the value for the given key from the data
            /// store as a s64.
            ///
            /// @param Key to attempt to retrieve value for
            /// @param [Out] Value ready
            /// @return True if value has been read, false otherwise
            //----------------------------------------------------------------
			bool TryGetValue(const std::string& instrKey, s64 &outddwValue);
            //----------------------------------------------------------------
            /// Try Get Value
            ///
            /// Attempts to read the value for the given key from the data
            /// store as a f32.
            ///
            /// @param Key to attempt to retrieve value for
            /// @param [Out] Value ready
            /// @return True if value has been read, false otherwise
            //----------------------------------------------------------------
			bool TryGetValue(const std::string& instrKey, f32 &outfValue);			
			//----------------------------------------------------------------
            /// Set Value For Key
            ///
            /// Sets a new value for the given key overwriting any previous data
            ///
            /// @param Key to use
            /// @param Value to set
            //----------------------------------------------------------------
			void SetValueForKey(const std::string& instrKey, const std::string& instrValue);
            //----------------------------------------------------------------
            /// Set Value For Key
            ///
            /// Sets a new value for the given key overwriting any previous data
            ///
            /// @param Key to use
            /// @param Value to set
            //----------------------------------------------------------------
			void SetValueForKey(const std::string& instrKey, bool inbValue);
            //----------------------------------------------------------------
            /// Set Value For Key
            ///
            /// Sets a new value for the given key overwriting any previous data
            ///
            /// @param Key to use
            /// @param Value to set
            //----------------------------------------------------------------
			void SetValueForKey(const std::string& instrKey, u16 inuwValue);
            //----------------------------------------------------------------
            /// Set Value For Key
            ///
            /// Sets a new value for the given key overwriting any previous data
            ///
            /// @param Key to use
            /// @param Value to set
            //----------------------------------------------------------------
			void SetValueForKey(const std::string& instrKey, s16 inwValue);
            //----------------------------------------------------------------
            /// Set Value For Key
            ///
            /// Sets a new value for the given key overwriting any previous data
            ///
            /// @param Key to use
            /// @param Value to set
            //----------------------------------------------------------------
			void SetValueForKey(const std::string& instrKey, u32 inudwValue);
            //----------------------------------------------------------------
            /// Set Value For Key
            ///
            /// Sets a new value for the given key overwriting any previous data
            ///
            /// @param Key to use
            /// @param Value to set
            //----------------------------------------------------------------
			void SetValueForKey(const std::string& instrKey, s32 indwValue);
            //----------------------------------------------------------------
            /// Set Value For Key
            ///
            /// Sets a new value for the given key overwriting any previous data
            ///
            /// @param Key to use
            /// @param Value to set
            //----------------------------------------------------------------
			void SetValueForKey(const std::string& instrKey, u64 inuddwValue);
            //----------------------------------------------------------------
            /// Set Value For Key
            ///
            /// Sets a new value for the given key overwriting any previous data
            ///
            /// @param Key to use
            /// @param Value to set
            //----------------------------------------------------------------
			void SetValueForKey(const std::string& instrKey, s64 inddwValue);
            //----------------------------------------------------------------
            /// Set Value For Key
            ///
            /// Sets a new value for the given key overwriting any previous data
            ///
            /// @param Key to use
            /// @param Value to set
            //----------------------------------------------------------------
			void SetValueForKey(const std::string& instrKey, f32 infValue);			
			//----------------------------------------------------------------
            /// Try Erase Key
            ///
            /// Attempts to erase the entry for the given key from the data
            /// store.
            ///
            /// @param Key of entry to remove
            /// @return True if entry removed, false otherwise
            //----------------------------------------------------------------
			bool TryEraseKey(const std::string& instrKey);
            //------------------------
            /// Clear
            ///
            /// Clears the local data store of all values
            ///
            //------------------------
            void Clear();
			//----------------------------------------------------------------
            /// Synchronise
            ///
            /// Writes the current state of the data store to disk.
            //----------------------------------------------------------------
			void Synchronise();
		
        private:
            //----------------------------------------------------------------
            /// Constructor
            //----------------------------------------------------------------
			CLocalDataStore();
            //----------------------------------------------------------------
            /// Refresh From File
            ///
            /// Loads the saved data from disk into the current data store
            //----------------------------------------------------------------
            void RefreshFromFile();
            //----------------------------------------------------------------
            /// Subscribe To Application Suspend Event
            ///
            /// Subscribes to the suspend event
            //----------------------------------------------------------------
            void SubscribeToApplicationSuspendEvent();
            //----------------------------------------------------------------
            /// On Application Suspended
            ///
            /// Delegate called with the application suspends
            //----------------------------------------------------------------
			void OnApplicationSuspended();
			
            // Member vars
			bool                        mbBackingValid;
			ParamDictionary             mBackingDictionary;
			static CLocalDataStore*     mpSingletonInstance;
            
            ConnectionUPtr m_appSuspendedConnection;
		};
	}
}

#endif
