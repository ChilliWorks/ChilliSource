//
//  Resource.h
//  Chilli Source
//  Created by Scott Downie on 30/09/2010.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2010 Tag Games Limited
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


#ifndef _CHILLISOURCE_CORE_RESOURCE_RESOURCE_H_
#define _CHILLISOURCE_CORE_RESOURCE_RESOURCE_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/QueryableInterface.h>
#include <ChilliSource/Core/Container/ParamDictionary.h>

#include <atomic>

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
            
            using ResourceId = u32;
            
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
            //-------------------------------------------------------
			Resource();
			//-------------------------------------------------------
			/// @author S Downie
			///
			/// @return The file path of the resource. If the resource
            /// was not loaded from file then this is empty.
			//-------------------------------------------------------
			const std::string& GetFilePath() const;
            //-------------------------------------------------------
			/// @author Ian Copland
			///
			/// @return The name of the resource. If this was loaded
            /// from file this will be the file name. Otherwise this
            /// will be the unique string Id of the resource.
			//-------------------------------------------------------
			const std::string& GetName() const;
            //-------------------------------------------------------
			/// @author S Downie
			///
			/// @return The resource id as set by the resource pool
			//-------------------------------------------------------
			ResourceId GetId() const;
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
            /// should not be called by the general application unless
            /// they are manually constructing resources. The application
            /// can check this flag to determine if the resource was
            /// loaded correctly
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
            /// Virtual desctructor
            ///
            /// @author S Downie
            //-------------------------------------------------------
			virtual ~Resource(){}
            
        private:
            
            friend class ResourcePool;
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
            /// Sets the name of the resource. This is the name
            /// that can be used to look up the resource using
            /// ResourcePool::GetResource(). This should only be set
            /// by the resource pool.
            ///
            /// @author Ian Copland
            ///
            /// @param The name of the resource.
			//-------------------------------------------------------
			void SetName(const std::string& in_name);
            //-------------------------------------------------------
            /// NOTE: Not for general application used by the providers
            /// to set the id
            ///
			/// @author S Downie
			///
			/// @param The resource id
			//-------------------------------------------------------
			void SetId(ResourceId in_id);
            //-------------------------------------------------------
            /// NOTE: Not for general application used by the providers
            /// to set the load options as the resource may need to be
            /// reloaded on context loss
            ///
			/// @author S Downie
			///
			/// @param The load options of the resource. Used to load
            /// again if context is lost
			//-------------------------------------------------------
			void SetOptions(const IResourceOptionsBaseCSPtr& in_options);
			//-------------------------------------------------------
			/// @author S Downie
			///
			/// @return The options that were set on the resource on
            /// loading.
			//-------------------------------------------------------
			const IResourceOptionsBaseCSPtr& GetOptions() const;
			
		private:
			
            IResourceOptionsBaseCSPtr m_options;
            
			std::string m_filePath;
            std::string m_name;
			StorageLocation m_location;
            ResourceId m_id;
        
            std::atomic<LoadState> m_loadState;
		};
	}
}

#endif
