/*
 *  NamedInterfaceDefinitions.cpp
 *  moFlo
 *
 *  Created by Tag Games on 28/09/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Rendering/Texture/Texture.h>
#include <ChilliSource/Rendering/Texture/Cubemap.h>
#include <ChilliSource/Rendering/Shader/Shader.h>

namespace ChilliSource 
{
	namespace Rendering
	{
		DEFINE_NAMED_INTERFACE(Texture);
        DEFINE_NAMED_INTERFACE(Cubemap);
		DEFINE_NAMED_INTERFACE(Shader);
	}
}