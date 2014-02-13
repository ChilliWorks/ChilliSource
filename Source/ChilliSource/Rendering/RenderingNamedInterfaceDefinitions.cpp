/*
 *  NamedInterfaceDefinitions.cpp
 *  moFlo
 *
 *  Created by Tag Games on 28/09/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#include <moFlo/Rendering/Texture.h>
#include <moFlo/Rendering/Cubemap.h>
#include <moFlo/Rendering/Shader.h>

namespace moFlo 
{
	namespace Rendering
	{
		DEFINE_NAMED_INTERFACE(ITexture);
        DEFINE_NAMED_INTERFACE(ICubemap);
		DEFINE_NAMED_INTERFACE(IShader);
	}
}