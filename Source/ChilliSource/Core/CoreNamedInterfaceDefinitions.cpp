/*
 *  NamedInterfaceDefinitions.cpp
 *  MoFlow
 *
 *  Created by Tag Games on 28/09/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */


#include <ChilliSource/Core/SystemConcepts.h>
#include <ChilliSource/Core/ResourceProvider.h>

namespace moFlo
{
	namespace Core
	{
		DEFINE_NAMED_INTERFACE(IUpdateable);
		DEFINE_NAMED_INTERFACE(IComponentProducer);
		DEFINE_NAMED_INTERFACE(IResourceProvider);
	}
}