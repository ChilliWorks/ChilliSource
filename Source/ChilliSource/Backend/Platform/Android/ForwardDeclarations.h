/*
 *  ForwardDeclarations.h
 *  moFlow
 *
 *  Created by Ian Copland on 09/08/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

#ifndef _CHILLISOURCE_BACKEND_PLATFORM_ANDROID_FORWARDDECLARATIONS_H_
#define _CHILLISOURCE_BACKEND_PLATFORM_ANDROID_FORWARDDECLARATIONS_H_

#include <ChilliSource/Core/Base/StandardMacros.h>

#include <memory>

namespace ChilliSource
{
	namespace Android
	{
		//forward declarations
		class CJavaInterfaceManager;
		class IJavaInterface;
		class CCoreJavaInterface;
		class CVideoPlayerJavaInterface;
		class CVideoPlayerActivity;
		class CRemoteNotificationJavaInterface;
		class CLocalNotificationJavaInterface;
		class CRemoteNotificationScheduler;
		class CEmailCompositionActivity;
		class CEmailCompositionJavaInterface;
		class CAccelerometerJavaInterface;
		class CKeyboardJavaInterface;

		//shared pointer declarations
		typedef std::shared_ptr<CJavaInterfaceManager> JavaInterfaceManagerPtr;
		typedef std::shared_ptr<IJavaInterface> JavaInterfacePtr;
		typedef std::shared_ptr<CCoreJavaInterface> CoreJavaInterfacePtr;
		typedef std::shared_ptr<CRemoteNotificationJavaInterface> RemoteNotificationJavaInterfacePtr;
		typedef std::shared_ptr<CVideoPlayerJavaInterface> VideoPlayerJavaInterfacePtr;
		typedef std::shared_ptr<CVideoPlayerActivity> VideoPlayerActivityPtr;
		typedef std::shared_ptr<CLocalNotificationJavaInterface> LocalNotificationJavaInterfacePtr;
		typedef std::shared_ptr<CRemoteNotificationScheduler> AndroidRemoteNotificationSchedulerPtr;
		typedef std::shared_ptr<CEmailCompositionActivity> EmailCompositionActivityPtr;
		typedef std::shared_ptr<CEmailCompositionJavaInterface> EmailCompositionJavaInterfacePtr;
		typedef std::shared_ptr<CAccelerometerJavaInterface> AccelerometerJavaInterfacePtr;
		typedef std::shared_ptr<CKeyboardJavaInterface> KeyboardJavaInterfacePtr;
	}
}

#endif
