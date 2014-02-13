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

#include <moFlo/Video/ForwardDeclarations.h>
#include <moFlo/Core/ResourceManager.h>


namespace moFlo
{
	namespace Video
	{
		class CSubtitlesManager : public Core::IResourceManager
		{
		public:
			DECLARE_NAMED_INTERFACE(CSubtitlesManager);
			//----------------------------------------------------------------
			/// Is A
			///
			/// Query the interface type
            ///
			/// @param The interface to compare
			/// @return Whether the object implements that interface
			//----------------------------------------------------------------
			bool IsA(Core::InterfaceIDType inInterfaceID) const;
			//----------------------------------------------------------------
			/// Get Resource Type
			///
			/// @return The type of resource this manager handles
			//----------------------------------------------------------------
			Core::InterfaceIDType GetResourceType() const;
			//----------------------------------------------------------------
			/// Get Provider Type
			///
			/// @return The type of resource it consumes from resource provider
			//----------------------------------------------------------------
			Core::InterfaceIDType GetProviderType() const; 
			//----------------------------------------------------------------
			/// Manages Resource Of Type
			///
			/// @param Type
			/// @return Whether this object manages the object of type
			//----------------------------------------------------------------
			bool ManagesResourceOfType(Core::InterfaceIDType inInterfaceID) const;
			//-----------------------------------------------------------------
			/// Get Resource From File
			///
			/// Generic call to get the managers resource
            ///
            /// @param The storage location to load from
			/// @param File path to resource
			/// @return Generic pointer to object type
			//-----------------------------------------------------------------
			Core::ResourcePtr GetResourceFromFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string &instrFilePath);
			//-----------------------------------------------------------------
			/// Async Get Resource From File
			///
			/// Generic call to get the managers resource
            ///
            /// @param The storage location to load from
			/// @param File path to resource
			/// @return Generic pointer to object type
			//-----------------------------------------------------------------
			Core::ResourcePtr AsyncGetResourceFromFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string &instrFilePath);
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
			SubtitlesPtr GetSubtitlesFromFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string & inFilePathMaterial);
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
			SubtitlesPtr AsyncGetSubtitlesFromFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string & inFilePathMaterial);
		};
	}
}

#endif