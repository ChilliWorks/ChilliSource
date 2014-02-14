/*
 *  ImageResourceProvider.h
 *  iOSTemplate
 *
 *  Created by Scott Downie on 01/08/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#ifndef _MO_FLO_CORE_IMAGE_RESOURCE_PROVIDER_H_
#define _MO_FLO_CORE_IMAGE_RESOURCE_PROVIDER_H_

#include <ChilliSource/Core/Resource/ResourceProvider.h>
#include <ChilliSource/Core/Image/Image.h>

namespace moFlo
{
	class IImageResourceProvider : public Core::IResourceProvider
	{
	public:

		//----------------------------------------------------------------
		/// Create Image From File
		///
        /// @param The storage location to load from
		/// @param File path to resource
		/// @return Instantiated resource
		//----------------------------------------------------------------
		virtual bool CreateImageFromFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string & inFilePath, Core::CImage::Format ineFormat, Core::ResourcePtr& outpResource) = 0;
		//--------------------------------------------------
		/// Set Default Format
		///
		/// @param Image format that will be used in lieu
		/// of specific
		//--------------------------------------------------
		static void SetDefaultFormat(Core::CImage::Format ineFormat);
		
	protected:
		
		static Core::CImage::Format meDefaultFormat;
	};
}

#endif