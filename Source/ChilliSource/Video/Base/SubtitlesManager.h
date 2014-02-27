/*
 *  SubtitlesManager.h
 *  moFlow
 *
 *  Created by Ian Copland 21/02/2013.
 *  Copyright 2013 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLOW_VIDEO_SUBTITLESMANAGER_H_
#define _MOFLOW_VIDEO_SUBTITLESMANAGER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Resource/ResourceManager.h>

namespace ChilliSource
{
	namespace Video
	{
		class SubtitlesManager : public Core::ResourceManager
		{
		public:
			DECLARE_NAMED_INTERFACE(SubtitlesManager);
			//----------------------------------------------------------------
			/// Is A
			///
			/// Query the interface type
            ///
			/// @param The interface to compare
			/// @return Whether the object implements that interface
			//----------------------------------------------------------------
			bool IsA(Core::InterfaceIDType inInterfaceID) const override;
			//----------------------------------------------------------------
			/// Get Resource Type
			///
			/// @return The type of resource this manager handles
			//----------------------------------------------------------------
			Core::InterfaceIDType GetResourceType() const override;
			//----------------------------------------------------------------
			/// Get Provider Type
			///
			/// @return The type of resource it consumes from resource provider
			//----------------------------------------------------------------
			Core::InterfaceIDType GetProviderType() const override;
			//----------------------------------------------------------------
			/// Manages Resource Of Type
			///
			/// @param Type
			/// @return Whether this object manages the object of type
			//----------------------------------------------------------------
			bool ManagesResourceOfType(Core::InterfaceIDType inInterfaceID) const override;
			//-----------------------------------------------------------------
			/// Get Resource From File
			///
			/// Generic call to get the managers resource
            ///
            /// @param The storage location to load from
			/// @param File path to resource
			/// @return Generic pointer to object type
			//-----------------------------------------------------------------
			Core::ResourceSPtr GetResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string &instrFilePath) override;
			//-----------------------------------------------------------------
			/// Async Get Resource From File
			///
			/// Generic call to get the managers resource
            ///
            /// @param The storage location to load from
			/// @param File path to resource
			/// @return Generic pointer to object type
			//-----------------------------------------------------------------
			Core::ResourceSPtr AsyncGetResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string &instrFilePath) override;
            //----------------------------------------------------------------
			/// Get Subtitles From File
			///
			/// Loads the Subtitles from resource and returns a handle to it.
			/// Alternately if the Subtitles already exist it will return the
			/// handle without loading
			///
            /// @param The storage location to load from
			/// @param File name
			/// @return A handle to the Subtitles
			//----------------------------------------------------------------
			SubtitlesSPtr GetSubtitlesFromFile(Core::StorageLocation ineStorageLocation, const std::string & inFilePathMaterial);
			//----------------------------------------------------------------
			/// Async Get Subtitles From File
			///
			/// starts loading Subtitles from resource and returns a handle to it.
			/// Alternately if the Subtitles already exist it will return the
			/// handle without loading
			///
            /// @param The storage location to load from
			/// @param File name
			/// @return A handle to the Subtitles
			//----------------------------------------------------------------
			SubtitlesSPtr AsyncGetSubtitlesFromFile(Core::StorageLocation ineStorageLocation, const std::string & inFilePathMaterial);
		};
	}
}

#endif