//
//  ForwardDeclarations.h
//  Chilli Source
//
//  Created by Ian Copland on 03/03/2014
//	Copyright 2011 Tag Games Ltd. All rights reserved.
//

#ifndef _CHILLISOURCE_BACKEND_PLATFORM_WINDOWS_FORWARDDECLARATION_H_
#define _CHILLISOURCE_BACKEND_PLATFORM_WINDOWS_FORWARDDECLARATION_H_

#include <ChilliSource/ChilliSource.h>

namespace ChilliSource
{
	namespace Windows
	{
		//------------------------------------------------------
		/// Core
		//------------------------------------------------------
		CS_FORWARDDECLARE_CLASS(PlatformSystem);
		CS_FORWARDDECLARE_CLASS(FileSystem);
		CS_FORWARDDECLARE_CLASS(ImageProvider);
		CS_FORWARDDECLARE_CLASS(PngImage);
		//------------------------------------------------------
		/// Image
		//------------------------------------------------------
		CS_FORWARDDECLARE_CLASS(InputSystem);
		CS_FORWARDDECLARE_CLASS(Mouse);
		CS_FORWARDDECLARE_CLASS(TouchScreen);
		//------------------------------------------------------
		/// Networking
		//------------------------------------------------------
		CS_FORWARDDECLARE_CLASS(HttpConnectionSystem);
	}
}

#endif