/*
 *  ForwardDeclarations.h
 *  moFlow
 *
 *  Created by Ian Copland on 09/08/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

#ifndef MOFLOW_PLATFORM_ANDROID_FORWARDDECLARATIONS_H
#define MOFLOW_PLATFORM_ANDROID_FORWARDDECLARATIONS_H

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
		typedef SHARED_PTR<CJavaInterfaceManager> JavaInterfaceManagerPtr;
		typedef SHARED_PTR<IJavaInterface> JavaInterfacePtr;
		typedef SHARED_PTR<CCoreJavaInterface> CoreJavaInterfacePtr;
		typedef SHARED_PTR<CRemoteNotificationJavaInterface> RemoteNotificationJavaInterfacePtr;
		typedef SHARED_PTR<CVideoPlayerJavaInterface> VideoPlayerJavaInterfacePtr;
		typedef SHARED_PTR<CVideoPlayerActivity> VideoPlayerActivityPtr;
		typedef SHARED_PTR<CLocalNotificationJavaInterface> LocalNotificationJavaInterfacePtr;
		typedef SHARED_PTR<CRemoteNotificationScheduler> AndroidRemoteNotificationSchedulerPtr;
		typedef SHARED_PTR<CEmailCompositionActivity> EmailCompositionActivityPtr;
		typedef SHARED_PTR<CEmailCompositionJavaInterface> EmailCompositionJavaInterfacePtr;
		typedef SHARED_PTR<CAccelerometerJavaInterface> AccelerometerJavaInterfacePtr;
		typedef SHARED_PTR<CKeyboardJavaInterface> KeyboardJavaInterfacePtr;
	}
}

#endif
