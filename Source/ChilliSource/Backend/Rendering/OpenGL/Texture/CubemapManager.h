/*
 *  GLCubemapManager.h
 *  moFlo
 *
 *  Created by Tag Games on 01/10/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLOW_OPENGL_CUBEMAPMANAGER_H_
#define _MOFLOW_OPENGL_CUBEMAPMANAGER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Backend/Rendering/OpenGL/ForwardDeclarations.h>
#include <ChilliSource/Rendering/Texture/CubemapManager.h>

namespace ChilliSource
{
	namespace OpenGL
	{
		class CCubemapManager : public ChilliSource::Rendering::CubemapManager
		{
		public:
			//----------------------------------------------------------------
			/// Create Cubemap Resource
			///
			/// @return Concrete Cubemap resource based on the render system
			//----------------------------------------------------------------
			ChilliSource::Rendering::CubemapSPtr CreateCubemapResource();
			//----------------------------------------------------------------
			/// Create Cubemap From Image
			///
			/// @param Images
			/// @param Generate mip maps
			/// @param Out: Cubemap resource
			/// @return Success
			//----------------------------------------------------------------
			bool CreateCubemapFromImages(const std::vector<Core::ResourceSPtr>& inaImages, bool inbWithMipsMaps, ChilliSource::Rendering::CubemapSPtr& outpCubemap);
			//----------------------------------------------------------------
			/// Restore
			///
			/// Restore all Cubemaps either from file or from cached images.
			//----------------------------------------------------------------
			void Restore();
			//----------------------------------------------------------------
			/// Add Restorable Cubemap
			///
			/// Adds the Cubemap to the list of all created Cubemaps. This
			/// list is used to restore the Cubemaps when the context is lost.
			///
			/// @param The Cubemap pointer.
			//----------------------------------------------------------------
			void AddRestorableCubemap(const Rendering::CubemapSPtr& inpCubemap);
			//----------------------------------------------------------------
			/// Remove Restorable Cubemap
			///
			/// Removes the Cubemap from the list of all created Cubemaps. This
			/// list is used to restore the Cubemaps when the context is lost.
			///
			/// @param The Cubemap pointer.
			//----------------------------------------------------------------
			void RemoveRestorableCubemap(CCubemap* inpCubemap);
		private:
#ifdef TARGET_ANDROID
			std::vector<Rendering::CubemapWeakPtr> mapCubemapCache;
			std::unordered_map<CCubemap*, Core::ImageSPtr> mapBackedUpImages;
#endif
		};
	}
}

#endif
