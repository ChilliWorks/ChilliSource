//
//  Resource.h
//  Chilli Source
//
//  Created by Scott Downie on 30/09/2010.
//  Copyright (c) 2010 Tag Games Ltd. All rights reserved.
//


#ifndef _CHILLISOURCE_CORE_RESOURCE_RESOURCE_H_
#define _CHILLISOURCE_CORE_RESOURCE_RESOURCE_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/QueryableInterface.h>

#include <condition_variable>
#include <mutex>

namespace ChilliSource
{
	namespace Core
	{
        //-------------------------------------------------------
        /// Interface into a resource. A resource is typically
        /// an asset loaded from file (i.e. an image, a shader, a
        /// model, etc.). Resources are loaded via providers
        /// and the resource pool API. Resources are cached
        /// by the pool and will not be destroyed until the pool
        /// releases them. Resources can be loaded asynchronously
        /// and their current load state queried.
        ///
        /// @author S Downie
        //-------------------------------------------------------
		class Resource : public QueryableInterface
		{
		public:
            
            CS_DECLARE_NOCOPY(Resource);
            
            //-------------------------------------------------------
            /// Describes the current load state of the resource
            /// largely for use with async loading in order to
            /// determine when the resource is safe to use
            ///
            /// @author S Downie
            //-------------------------------------------------------
            enum class LoadState
            {
                k_loading,
                k_loaded,
                k_failed
            };
            
            //-------------------------------------------------------
            /// Constructor
            ///
            /// @author S Downie
            ///
            /// @param Owning resource pool
            //-------------------------------------------------------
			Resource(ResourcePool* in_resourcePool);
			//-------------------------------------------------------
            /// NOTE: Not for general application used by the providers
            /// to set the file path as the resource may need to be
            /// reloaded on context loss
            ///
			/// @author S Downie
			///
			/// @param The file path of the resource. Used to load
            /// again if context is lost
			//-------------------------------------------------------
			void SetFilePath(const std::string& in_filePath);
			//-------------------------------------------------------
			/// @author S Downie
			///
			/// @return The file path of the resource.
			//-------------------------------------------------------
			const std::string& GetFilePath() const;
			//-------------------------------------------------------
			/// @author S Downie
			///
			/// @param The storage location of the resource.
			//-------------------------------------------------------
			void SetStorageLocation(StorageLocation in_location);
			//-------------------------------------------------------
			/// @author S Downie
			///
			/// @return The storage location of the resource.
			//-------------------------------------------------------
            StorageLocation GetStorageLocation() const;
            //-------------------------------------------------------
            /// NOTE: This is used by the resource providers and
            /// should not be called by the general application.
            /// Used to flag the load state mostly from async load operations
            ///
            /// Will notify any waiting parties if load is complete
            ///
            /// @author S Downie
            ///
            /// @param Current load state
            //-------------------------------------------------------
            void SetLoadState(LoadState in_loadState);
            //-------------------------------------------------------
            /// Used to query the loading progress of a resource
            /// to determine when it is safe to use
            ///
            /// @author S Downie
            ///
            /// @return Current load state
            //-------------------------------------------------------
            LoadState GetLoadState() const;
			//-------------------------------------------------------
			/// Block the calling thread until the resource load state
            /// is changed from loading to loaded or failed
            ///
            /// @author S Downie
			//-------------------------------------------------------
			void WaitTilLoaded();
			//-------------------------------------------------------
            /// Remove the resource from the resource pool and cause it
            /// to be destroyed. This will assert if it has any references
            /// so must be called from a raw pointer i.e.
            ///
            ///
            ///  rawPtr = sharedPtr.get();
            ///  sharedPtr.reset();
            ///  rawPtr->Release();
            ///
            ///
            /// @author S Downie
			//-------------------------------------------------------
			void Release();
            //-------------------------------------------------------
            /// Virtual desctructor
            ///
            /// @author S Downie
            //-------------------------------------------------------
			virtual ~Resource(){}
			
		private:
			
            std::condition_variable m_loadWaitCondition;
			std::mutex m_mutex;
            
			std::string m_filePath;
			StorageLocation m_location;
        
            std::atomic<LoadState> m_loadState;
            
            ResourcePool* m_resourcePool;
		};
	}
}

#endif
