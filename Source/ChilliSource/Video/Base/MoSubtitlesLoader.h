/*
 *  MoSubtitlesLoader.h
 *  moFlow
 *
 *  Created by Ian Copland 21/02/2013.
 *  Copyright 2013 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLOW_VIDEO_MOSUBTITLESLOADER_H_
#define _MOFLOW_VIDEO_MOSUBTITLESLOADER_H_

#include <ChilliSource/Video/ForwardDeclarations.h>
#include <ChilliSource/Core/Resource/ResourceProvider.h>
#include <ChilliSource/Core/JSON/json.h>
#include <ChilliSource/Video/Base/Subtitles.h>

namespace moFlo
{
	namespace Video
	{
        //=================================================================
        /// MoSubtitles Loader
        ///
        /// Creates a new Subtitles resource from a MoSubtitles file.
        //=================================================================
		class CMoSubtitlesLoader : public Core::IResourceProvider
		{
		public:
            //-------------------------------------------------------------------------
			/// Constructor
			//-------------------------------------------------------------------------
            CMoSubtitlesLoader();
			//-------------------------------------------------------------------------
			/// Is A
			///
			/// @param Interface to compare
			/// @return Whether the object implements the given interface
			//-------------------------------------------------------------------------
			bool IsA(Core::InterfaceIDType inInterfaceID) const;
			//-------------------------------------------------------------------------
			/// Can Create Resource of Kind
			///
			/// @param Type to compare
			/// @return Whether the object can create a resource of given type
			//-------------------------------------------------------------------------
			bool CanCreateResourceOfKind(Core::InterfaceIDType inInterfaceID) const;
			//-------------------------------------------------------------------------
			/// Can Create Resource From File With Extension
			///
			/// @param Type to compare
			/// @param Extension to compare
			/// @return Whether the object can create a resource with the given extension
			//-------------------------------------------------------------------------
			bool CanCreateResourceFromFileWithExtension(const std::string & inExtension) const;
            //-------------------------------------------------------------------------
			/// Destructor
			//-------------------------------------------------------------------------
			virtual ~CMoSubtitlesLoader();
		private:
			//-------------------------------------------------------------------------
			/// Create Resource From File
			///
            /// @param The storage location to load from
			/// @param Filename
			/// @param Out: Resource object
			/// @return Whether the resource was loaded 
			//-------------------------------------------------------------------------
			bool CreateResourceFromFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string & inFilePath, Core::ResourcePtr& outpResource);
			//-------------------------------------------------------------------------
			/// Async Create Resource From File
			///
            /// @param The storage location to load from
			/// @param Filename
			/// @param Out: Resource object
			/// @return Whether the resource was loaded
			//-------------------------------------------------------------------------
			bool AsyncCreateResourceFromFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string & inFilePath, Core::ResourcePtr& outpResource);
            //-------------------------------------------------------------------------
			/// Load MoSubtitles Task
            ///
            /// Performs the loading of a MoSubtitles file on a background thread.
			///
            /// @param The storage location to load from
			/// @param Filename
			/// @param Out: Resource object
			//-------------------------------------------------------------------------
			void LoadMoSubtitlesTask(Core::STORAGE_LOCATION ineStorageLocation, const std::string & inFilePath, SubtitlesPtr& outpResource);
            //-------------------------------------------------------------------------
			/// Load MoSubtitles
			///
            /// @param The storage location to load from
			/// @param Filename
			/// @param Out: Resource object
			//-------------------------------------------------------------------------
			bool LoadMoSubtitles(Core::STORAGE_LOCATION ineStorageLocation, const std::string & inFilePath, SubtitlesPtr& outpResource);
            //-------------------------------------------------------------------------
			/// Load Style
			///
            /// @param The style JSON object.
			/// @return the style.
			//-------------------------------------------------------------------------
			CSubtitles::StylePtr LoadStyle(const Json::Value& inStyleJson);
            //-------------------------------------------------------------------------
			/// Load Subtitle
			///
            /// @param The subtitle JSON object.
			/// @return the style.
			//-------------------------------------------------------------------------
			CSubtitles::SubtitlePtr LoadSubtitle(const Json::Value& inSubtitleJson);
            //-------------------------------------------------------------------------
			/// Load Bounds
			///
            /// @param The bounds JSON object.
			/// @return the bounds.
			//-------------------------------------------------------------------------
            Core::Rectangle LoadBounds(const Json::Value& inBoundsJson);
            //-------------------------------------------------------------------------
            /// Parse Time
            ///
            /// Parses a time string in the format Hours:Minutes:Seconds:Milliseconds.
            /// For example 01:05:34:123.
            ///
            /// @param The string time.
            /// @return the integer time in milliseconds.
            //-------------------------------------------------------------------------
            TimeIntervalMs ParseTime(const std::string& instrTime);
		};
	}
}

#endif
