/*
 *  CubeMap.h
 *  MoFlow
 *
 *  Created by Scott Downie 15/07/2013.
 *  Copyright 2013 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLO_RENDERING_CUBEMAP_H_
#define _MOFLO_RENDERING_CUBEMAP_H_

#include <ChilliSource/Core/Resource/Resource.h>
#include <ChilliSource/Rendering/Texture/Texture.h>

namespace ChilliSource
{
	namespace Rendering
	{
		class ICubemap : public Core::IResource
		{
		public:
			
			DECLARE_NAMED_INTERFACE(ICubemap);
			
			virtual void Bind(u32 inSlot = 0) = 0;
			virtual void Unbind() = 0;
            
            virtual void SetFilter(ITexture::Filter ineSFilter, ITexture::Filter ineTFilter) = 0;
			virtual void SetWrapMode(ITexture::WrapMode inSWrapMode, ITexture::WrapMode inTWrapMode) = 0;
		};
	}
}

#endif