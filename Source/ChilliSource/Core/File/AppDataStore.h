//
//  AppDataStore.h
//  Chilli Source
//  Created by Stuart McGaw on 24/05/2011.
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

#ifndef _CHILLISOURCE_CORE_FILE_LOCALDATASTORE_H_
#define _CHILLISOURCE_CORE_FILE_LOCALDATASTORE_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Container/ParamDictionary.h>
#include <ChilliSource/Core/System/AppSystem.h>

#include <mutex>

namespace ChilliSource
{
	namespace Core
    {
        //--------------------------------------------------------------------
        /// Local data store provides a simple key-value data store useful for
        /// storing persistent data such as options and player preferences.
        ///
        /// The local data store is encrypted on disk however the private key
        /// used is generated from known information. This means the local
        /// data store should not be used to store sensitive information such
        /// as passwords.
        ///
        /// Note that keys beginning with an underscore '_' are reserved for
        /// internal engine use.
        ///
        /// The ADS is thread-safe and is initialised prior to the OnInit()
        /// lifecycle event.
        ///
        /// @author S McGaw
        //--------------------------------------------------------------------
		class AppDataStore final : public AppSystem
        {
        public:
            CS_DECLARE_NAMEDTYPE(AppDataStore);
            //------------------------------------------------------------
            /// Allows querying of whether or not this system implements
            /// a given interface.
            ///
			/// @author Ian Copland
			///
			/// @param Interface Id
			///
			/// @return Whether this object is of the given type.
			//------------------------------------------------------------
			bool IsA(InterfaceIDType in_interfaceId) const override;
			//------------------------------------------------------------
            /// Returns if the given key exists in the data store.
            ///
            /// @author R Henning
            ///
            /// @param The name of the key.
            ///
            /// @return Whether or not the key exists.
            //------------------------------------------------------------
			bool Contains(const std::string& in_key);
            //------------------------------------------------------------
            /// Attempts to get the value for the given key from the data
            /// store as a string.
            ///
            /// @author R Henning
            ///
            /// @param Key to attempt to retrieve the value for.
            /// @param [Out] The output value.
            ///
            /// @return Whether or not the value was successfully retreived.
            //------------------------------------------------------------
			bool TryGetValue(const std::string& in_key, std::string& out_value) const;
            //------------------------------------------------------------
            /// Attempts to get the value for the given key from the data
            /// store as a bool.
            ///
            /// @author R Henning
            ///
            /// @param Key to attempt to retrieve the value for.
            /// @param [Out] The output value.
            ///
            /// @return Whether or not the value was successfully retreived.
            //------------------------------------------------------------
			bool TryGetValue(const std::string& in_key, bool& out_value) const;
            //------------------------------------------------------------
            /// Attempts to get the value for the given key from the data
            /// store as a u16.
            ///
            /// @author R Henning
            ///
            /// @param Key to attempt to retrieve the value for.
            /// @param [Out] The output value.
            ///
            /// @return Whether or not the value was successfully retreived.
            //------------------------------------------------------------
			bool TryGetValue(const std::string& in_key, u16& out_value) const;
            //------------------------------------------------------------
            /// Attempts to get the value for the given key from the data
            /// store as a s16.
            ///
            /// @author R Henning
            ///
            /// @param Key to attempt to retrieve the value for.
            /// @param [Out] The output value.
            ///
            /// @return Whether or not the value was successfully retreived.
            //------------------------------------------------------------
			bool TryGetValue(const std::string& in_key, s16& out_value) const;
            //------------------------------------------------------------
            /// Attempts to get the value for the given key from the data
            /// store as a u32.
            ///
            /// @author R Henning
            ///
            /// @param Key to attempt to retrieve the value for.
            /// @param [Out] The output value.
            ///
            /// @return Whether or not the value was successfully retreived.
            //------------------------------------------------------------
			bool TryGetValue(const std::string& in_key, u32& out_value) const;
            //------------------------------------------------------------
            /// Attempts to get the value for the given key from the data
            /// store as a s32.
            ///
            /// @author R Henning
            ///
            /// @param Key to attempt to retrieve the value for.
            /// @param [Out] The output value.
            ///
            /// @return Whether or not the value was successfully retreived.
            //------------------------------------------------------------
			bool TryGetValue(const std::string& in_key, s32& out_value) const;
            //------------------------------------------------------------
            /// Attempts to get the value for the given key from the data
            /// store as a u64.
            ///
            /// @author R Henning
            ///
            /// @param Key to attempt to retrieve the value for.
            /// @param [Out] The output value.
            ///
            /// @return Whether or not the value was successfully retreived.
            //------------------------------------------------------------
			bool TryGetValue(const std::string& in_key, u64& out_value) const;
            //------------------------------------------------------------
            /// Attempts to get the value for the given key from the data
            /// store as a s64.
            ///
            /// @author R Henning
            ///
            /// @param Key to attempt to retrieve the value for.
            /// @param [Out] The output value.
            /// @return Whether or not the value was successfully retreived.
            //------------------------------------------------------------
			bool TryGetValue(const std::string& in_key, s64& out_value) const;
            //------------------------------------------------------------
            /// Attempts to get the value for the given key from the data
            /// store as a f32.
            ///
            /// @author R Henning
            ///
            /// @param Key to attempt to retrieve the value for.
            /// @param [Out] The output value.
            ///
            /// @return Whether or not the value was successfully retreived.
            //------------------------------------------------------------
			bool TryGetValue(const std::string& in_key, f32& out_value) const;
			//--------------------------------------------------------------
            /// Sets the value for the given key. If the key already existed
            /// its previous value will be overwritten. Note that keys
            /// beginning with an underscore '_' are reserved for internal
            /// engine use.
            ///
            /// @author R Henning
            ///
            /// @param The key.
            /// @param The value.
            //--------------------------------------------------------------
			void SetValue(const std::string& in_key, const std::string& in_value);
            //--------------------------------------------------------------
            /// Sets the value for the given key. If the key already existed
            /// its previous value will be overwritten. Note that keys
            /// beginning with an underscore '_' are reserved for internal
            /// engine use.
            ///
            /// @author R Henning
            ///
            /// @param The key.
            /// @param The value.
            //--------------------------------------------------------------
			void SetValue(const std::string& in_key, const s8* in_value);
			//--------------------------------------------------------------
            /// Sets the value for the given key. If the key already existed
            /// its previous value will be overwritten. Note that keys
            /// beginning with an underscore '_' are reserved for internal
            /// engine use.
            ///
            /// @author R Henning
            ///
            /// @param The key.
            /// @param The value.
            //--------------------------------------------------------------
			void SetValue(const std::string& in_key, bool in_value);
			//--------------------------------------------------------------
            /// Sets the value for the given key. If the key already existed
            /// its previous value will be overwritten. Note that keys
            /// beginning with an underscore '_' are reserved for internal
            /// engine use.
            ///
            /// @author R Henning
            ///
            /// @param The key.
            /// @param The value.
            //--------------------------------------------------------------
			void SetValue(const std::string& in_key, u16 in_value);
			//--------------------------------------------------------------
            /// Sets the value for the given key. If the key already existed
            /// its previous value will be overwritten. Note that keys
            /// beginning with an underscore '_' are reserved for internal
            /// engine use.
            ///
            /// @author R Henning
            ///
            /// @param The key.
            /// @param The value.
            //--------------------------------------------------------------
			void SetValue(const std::string& in_key, s16 in_value);
			//--------------------------------------------------------------
            /// Sets the value for the given key. If the key already existed
            /// its previous value will be overwritten. Note that keys
            /// beginning with an underscore '_' are reserved for internal
            /// engine use.
            ///
            /// @author R Henning
            ///
            /// @param The key.
            /// @param The value.
            //--------------------------------------------------------------
			void SetValue(const std::string& in_key, u32 in_value);
			//--------------------------------------------------------------
            /// Sets the value for the given key. If the key already existed
            /// its previous value will be overwritten. Note that keys
            /// beginning with an underscore '_' are reserved for internal
            /// engine use.
            ///
            /// @author R Henning
            ///
            /// @param The key.
            /// @param The value.
            //--------------------------------------------------------------
			void SetValue(const std::string& in_key, s32 in_value);
			//--------------------------------------------------------------
            /// Sets the value for the given key. If the key already existed
            /// its previous value will be overwritten. Note that keys
            /// beginning with an underscore '_' are reserved for internal
            /// engine use.
            ///
            /// @author R Henning
            ///
            /// @param The key.
            /// @param The value.
            //--------------------------------------------------------------
			void SetValue(const std::string& in_key, u64 in_value);
			//--------------------------------------------------------------
            /// Sets the value for the given key. If the key already existed
            /// its previous value will be overwritten. Note that keys
            /// beginning with an underscore '_' are reserved for internal
            /// engine use.
            ///
            /// @author R Henning
            ///
            /// @param The key.
            /// @param The value.
            //--------------------------------------------------------------
			void SetValue(const std::string& in_key, s64 in_value);
			//--------------------------------------------------------------
            /// Sets the value for the given key. If the key already existed
            /// its previous value will be overwritten. Note that keys
            /// beginning with an underscore '_' are reserved for internal
            /// engine use.
            ///
            /// @author R Henning
            ///
            /// @param The key.
            /// @param The value.
            //--------------------------------------------------------------
			void SetValue(const std::string& in_key, f32 in_value);
			//--------------------------------------------------------------
            /// Attempts to erase the entry for the given key from the data
            /// store.
            ///
            /// @author R Henning
            ///
            /// @param The key of the entry to remove.
            ///
            /// @return Whether or not the key was successfully removed.
            //--------------------------------------------------------------
			bool Erase(const std::string& in_key);
            //--------------------------------------------------------------
            /// Clears the Local Data Store, removing all contents.
            ///
            /// @author Ian Copland
            //--------------------------------------------------------------
            void Clear();
			//--------------------------------------------------------------
            /// Writes the current state of the data store to disk.
            ///
            /// @author R Henning
            //--------------------------------------------------------------
			void Save();
		
        private:
            friend class Application;
			//--------------------------------------------------------------
			/// Factory create method called by application to create a new
            /// instance of the system.
            ///
            /// @author Ian Copland
			///
			/// @return Creates a new instance of the system.
			//--------------------------------------------------------------
			static AppDataStoreUPtr Create();
			//--------------------------------------------------------------
			/// Private constructor to enforce use of the factory method.
			///
			/// @author S McGaw
			//--------------------------------------------------------------
			AppDataStore();
            //--------------------------------------------------------------
            /// Loads the saved data from disk into the current data store.
            ///
            /// @author R Henning
            //--------------------------------------------------------------
            void RefreshFromFile();
            //--------------------------------------------------------------
            /// Called when the application is suspended. This is also called
            /// when the application is exiting just prior to calling On
            /// Destroy. System suspend is called in the reverse order to
            /// which they were created.
            ///
            /// @author Ian Copland
            //--------------------------------------------------------------
            void OnSuspend() override;
			
            mutable std::mutex m_mutex;
			bool m_needsSynchonised;
			ParamDictionary m_dictionary;
		};
	}
}

#endif
