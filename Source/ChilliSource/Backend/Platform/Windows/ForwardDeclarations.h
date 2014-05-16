//
//  ForwardDeclarations.h
//  Chilli Source
//
//  Created by Ian Copland on 03/03/2014
//	Copyright 2011 Tag Games Ltd. All rights reserved.
//

#ifndef _CHILLISOURCE_BACKEND_PLATFORM_WINDOWS_FORWARDDECLARATION_H_
#define _CHILLISOURCE_BACKEND_PLATFORM_WINDOWS_FORWARDDECLARATION_H_

#include <ChilliSource/Core/Base/StandardMacros.h>

#include <memory>

namespace ChilliSource
{
	namespace Windows
	{
		//------------------------------------------------------
		/// Core
		//------------------------------------------------------
		CS_FORWARDDECLARE_CLASS(PlatformSystem);
		CS_FORWARDDECLARE_CLASS(FileSystem);
		CS_FORWARDDECLARE_CLASS(PNGImageProvider);
		CS_FORWARDDECLARE_CLASS(PngImage);
        CS_FORWARDDECLARE_CLASS(Device);
        CS_FORWARDDECLARE_CLASS(Screen);
		//------------------------------------------------------
		/// Input
		//------------------------------------------------------
        CS_FORWARDDECLARE_CLASS(DeviceButtonSystem);
		CS_FORWARDDECLARE_CLASS(PointerSystem);
		//------------------------------------------------------
		/// Networking
		//------------------------------------------------------
		CS_FORWARDDECLARE_CLASS(HttpRequest);
		CS_FORWARDDECLARE_CLASS(HttpRequestSystem);
	}
}

#endif
