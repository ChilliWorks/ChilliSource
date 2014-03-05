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
		class JavaInterfaceManager;
		class IJavaInterface;
		class CoreJavaInterface;
		class VideoPlayerJavaInterface;
		class VideoPlayerActivity;
		class RemoteNotificationJavaInterface;
		class LocalNotificationJavaInterface;
		class RemoteNotificationScheduler;
		class EmailCompositionActivity;
		class EmailCompositionJavaInterface;
		class AccelerometerJavaInterface;
		class KeyboardJavaInterface;

		//shared pointer declarations
		typedef std::shared_ptr<JavaInterfaceManager> JavaInterfaceManagerPtr;
		typedef std::shared_ptr<IJavaInterface> JavaInterfacePtr;
		typedef std::shared_ptr<CoreJavaInterface> CoreJavaInterfacePtr;
		typedef std::shared_ptr<RemoteNotificationJavaInterface> RemoteNotificationJavaInterfacePtr;
		typedef std::shared_ptr<VideoPlayerJavaInterface> VideoPlayerJavaInterfacePtr;
		typedef std::shared_ptr<VideoPlayerActivity> VideoPlayerActivityPtr;
		typedef std::shared_ptr<LocalNotificationJavaInterface> LocalNotificationJavaInterfacePtr;
		typedef std::shared_ptr<RemoteNotificationScheduler> AndroidRemoteNotificationSchedulerPtr;
		typedef std::shared_ptr<EmailCompositionActivity> EmailCompositionActivityPtr;
		typedef std::shared_ptr<EmailCompositionJavaInterface> EmailCompositionJavaInterfacePtr;
		typedef std::shared_ptr<AccelerometerJavaInterface> AccelerometerJavaInterfacePtr;
		typedef std::shared_ptr<KeyboardJavaInterface> KeyboardJavaInterfacePtr;
	}
}

#endif
