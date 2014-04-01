//
//  CSSubtitlesProvider.h
//  Chilli Source
//
//  Created by Ian Copland 21/02/2013.
//  Copyright 2013 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_VIDEO_BASE_CSSUBTITLESPROVIDER_H_
#define _CHILLISOURCE_VIDEO_BASE_CSSUBTITLESPROVIDER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/JSON/json.h>
#include <ChilliSource/Core/Resource/ResourceProvider.h>
#include <ChilliSource/Video/Base/Subtitles.h>

namespace ChilliSource
{
	namespace Video
	{
        //-------------------------------------------------------------------------
        /// Factory loader for creating subtitle resources from file.
        ///
        /// @param I Copland
        //-------------------------------------------------------------------------
		class CSSubtitlesProvider final : public Core::ResourceProvider
		{
		public:
            
            CS_DECLARE_NAMEDTYPE(CSSubtitlesProvider);
            
			//-------------------------------------------------------------------------
			/// @author I Copland
			///
			/// @param Interface to compare
            ///
			/// @return Whether the object implements the given interface
			//-------------------------------------------------------------------------
			bool IsA(Core::InterfaceIDType in_interfaceId) const override;
			//-------------------------------------------------------------------------
			/// @author I Copland
			///
			/// @param Type to compare
            ///
			/// @return Whether the object can create a resource of given type
			//-------------------------------------------------------------------------
			bool CanCreateResourceOfKind(Core::InterfaceIDType in_interfaceId) const override;
			//-------------------------------------------------------------------------
			/// @author I Copland
			///
			/// @param Type to compare
			/// @param Extension to compare
            ///
			/// @return Whether the object can create a resource with the given extension
			//-------------------------------------------------------------------------
			bool CanCreateResourceFromFileWithExtension(const std::string& in_extension) const override;
		private:
			//-------------------------------------------------------------------------
            /// Load the subtitles resource from the given file location
            ///
			/// @author I Copland
			///
            /// @param The storage location to load from
			/// @param Filename
			/// @param [Out] Resource object
            ///
			/// @return Whether the resource was loaded 
			//-------------------------------------------------------------------------
			bool CreateResourceFromFile(Core::StorageLocation in_storageLocation, const std::string& in_filePath, Core::ResourceSPtr& out_resource) override;
			//-------------------------------------------------------------------------
            /// Load the subtitles resource on a background thread from the given
            /// file location
            ///
			/// @author I Copland
			///
            /// @param The storage location to load from
			/// @param Filename
			/// @param [Out] Resource object
            ///
			/// @return Whether the resource was loaded
			//-------------------------------------------------------------------------
			bool AsyncCreateResourceFromFile(Core::StorageLocation in_storageLocation, const std::string& in_filePath, Core::ResourceSPtr& out_resource) override;
            //-------------------------------------------------------------------------
			/// @author I Copland
			///
            /// @param The storage location to load from
			/// @param Filename
			/// @param [Out] Resource object
			//-------------------------------------------------------------------------
			void LoadSubtitles(Core::StorageLocation in_storageLocation, const std::string& in_filePath, SubtitlesSPtr& out_resource) const;
            //-------------------------------------------------------------------------
			/// @author I Copland
			///
            /// @param The style JSON object.
            ///
			/// @return the style.
			//-------------------------------------------------------------------------
			Subtitles::StylePtr LoadStyle(const Json::Value& in_styleJSON) const;
            //-------------------------------------------------------------------------
			/// @author I Copland
			///
            /// @param The subtitle JSON object.
            ///
			/// @return the subtitle.
			//-------------------------------------------------------------------------
			Subtitles::SubtitlePtr LoadSubtitle(const Json::Value& in_subtitleJSON) const;
            //-------------------------------------------------------------------------
			/// @author I Copland
			///
            /// @param The bounds JSON object.
            ///
			/// @return the rectangle representing the bounding box that contains the
            /// subtitles.
			//-------------------------------------------------------------------------
            Core::Rectangle LoadBounds(const Json::Value& in_boundsJSON) const;
            //-------------------------------------------------------------------------
            /// Parses a time string in the format Hours:Minutes:Seconds:Milliseconds.
            /// For example 01:05:34:123.
            ///
            /// @author I Copland
            ///
            /// @param The string time.
            ///
            /// @return the integer time in milliseconds.
            //-------------------------------------------------------------------------
            TimeIntervalMs ParseTime(const std::string& in_time) const;
		};
	}
}

#endif
