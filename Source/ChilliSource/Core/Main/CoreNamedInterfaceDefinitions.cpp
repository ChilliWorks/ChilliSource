/*
 *  NamedInterfaceDefinitions.cpp
 *  MoFlow
 *
 *  Created by Tag Games on 28/09/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */


#include <ChilliSource/Core/System/SystemConcepts.h>
#include <ChilliSource/Core/Resource/ResourceProvider.h>

namespace moFlo
{
	namespace Core
	{
		DEFINE_NAMED_INTERFACE(IUpdateable);
		DEFINE_NAMED_INTERFACE(IComponentProducer);
		DEFINE_NAMED_INTERFACE(IResourceProvider);
	}
}