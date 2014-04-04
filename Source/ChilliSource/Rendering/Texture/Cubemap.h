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

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Resource/ResourceOld.h>
#include <ChilliSource/Rendering/Texture/Texture.h>

namespace ChilliSource
{
	namespace Rendering
	{
		class Cubemap : public Core::ResourceOld
		{
		public:
			
			CS_DECLARE_NAMEDTYPE(Cubemap);
			
			virtual void Bind(u32 inSlot = 0) = 0;
			virtual void Unbind() = 0;
            
            virtual void SetFilter(Texture::Filter ineSFilter, Texture::Filter ineTFilter) = 0;
			virtual void SetWrapMode(Texture::WrapMode inSWrapMode, Texture::WrapMode inTWrapMode) = 0;
		};
	}
}

#endif