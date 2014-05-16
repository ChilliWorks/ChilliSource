//
//  ForwardDeclarations.h
//  Chilli Source
//
//  Created by Ian Copland on 27/02/2014.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_CORE_FORWARDDECLARATIONS_H_
#define _CHILLISOURCE_CORE_FORWARDDECLARATIONS_H_

#include <ChilliSource/Core/Base/StandardMacros.h>

#include <memory>

namespace ChilliSource 
{
	namespace Core
	{
        //---------------------------------------------------------
        /// Base
        //---------------------------------------------------------
        CS_FORWARDDECLARE_CLASS(Application);
        CS_FORWARDDECLARE_CLASS(Colour);
        CS_FORWARDDECLARE_CLASS(Device);
        CS_FORWARDDECLARE_CLASS(IInformationProvider);
        CS_FORWARDDECLARE_CLASS(Logging);
        CS_FORWARDDECLARE_CLASS(PlatformSystem);
        CS_FORWARDDECLARE_CLASS(QueryableInterface);
        CS_FORWARDDECLARE_CLASS(Screen);
        //---------------------------------------------------------
        /// Container
        //---------------------------------------------------------
        template <typename TKey, typename TValue> class HashedArray;
        template <typename TType> class ObjectPool;
        CS_FORWARDDECLARE_CLASS(ParamDictionary);
        template <typename T> class WorkerQueue;
        //---------------------------------------------------------
        /// Cryptographic
        //---------------------------------------------------------
        CS_FORWARDDECLARE_CLASS(HMAC_SHA1);
        CS_FORWARDDECLARE_CLASS(SHA1);
        //---------------------------------------------------------
        /// Delegate
        //---------------------------------------------------------
        template <typename TReturnType, typename... TArgTypes> class ConnectableDelegate;
        template <typename TReturnType, typename... TArgTypes> class DelegateConnection;
        //---------------------------------------------------------
        /// Dialogue
        //---------------------------------------------------------
        CS_FORWARDDECLARE_CLASS(DialogueBoxSystem);
        //---------------------------------------------------------
        /// Entity
        //---------------------------------------------------------
        CS_FORWARDDECLARE_CLASS(Component);
        CS_FORWARDDECLARE_CLASS(Entity);
        CS_FORWARDDECLARE_CLASS(Transform);
        //---------------------------------------------------------
        /// Event
        //---------------------------------------------------------
        template <typename TDelegateType> class Event;
        template <typename TDelegateType> class IConnectableEvent;
        CS_FORWARDDECLARE_CLASS(IDisconnectableEvent);
        CS_FORWARDDECLARE_CLASS(EventConnection);
        //---------------------------------------------------------
        /// File
        //---------------------------------------------------------
        CS_FORWARDDECLARE_CLASS(FileStream);
        CS_FORWARDDECLARE_CLASS(FileSystem);
        CS_FORWARDDECLARE_CLASS(AppDataStore);
        enum class StorageLocation;
        //---------------------------------------------------------
        /// Image
        //---------------------------------------------------------
        CS_FORWARDDECLARE_CLASS(CSImageProvider);
        CS_FORWARDDECLARE_CLASS(ETC1ImageProvider);
        CS_FORWARDDECLARE_CLASS(Image);
        CS_FORWARDDECLARE_CLASS(PNGImageProvider);
        CS_FORWARDDECLARE_CLASS(PVRImageProvider);
        enum class ImageFormat;
        enum class ImageCompression;
        //---------------------------------------------------------
        /// Localisation
        //---------------------------------------------------------
	CS_FORWARDDECLARE_CLASS(LocalisedText);
	CS_FORWARDDECLARE_CLASS(LocalisedTextProvider);
        //---------------------------------------------------------
        /// Math
        //---------------------------------------------------------
        template <typename T> class CatmullRomSpline;
        template <typename T> class MultiLineSegment;
        CS_FORWARDDECLARE_CLASS(UVRect);
        CS_FORWARDDECLARE_CLASS(Rectangle);
        CS_FORWARDDECLARE_CLASS(Circle);
        CS_FORWARDDECLARE_CLASS(Sphere);
        CS_FORWARDDECLARE_CLASS(AABB);
        CS_FORWARDDECLARE_CLASS(OOBB);
        CS_FORWARDDECLARE_CLASS(Ray);
        CS_FORWARDDECLARE_CLASS(Line);
        CS_FORWARDDECLARE_CLASS(Plane);
        CS_FORWARDDECLARE_CLASS(Frustum);
        CS_FORWARDDECLARE_CLASS(Matrix3x3);
        CS_FORWARDDECLARE_CLASS(Matrix4x4);
        CS_FORWARDDECLARE_CLASS(Quaternion);
        CS_FORWARDDECLARE_STRUCT(UnifiedScalar);
		CS_FORWARDDECLARE_STRUCT(UnifiedVector2);
		CS_FORWARDDECLARE_STRUCT(UnifiedRectangle);
        CS_FORWARDDECLARE_CLASS(Vector2);
        CS_FORWARDDECLARE_CLASS(Vector3);
        CS_FORWARDDECLARE_CLASS(Vector4);
        //---------------------------------------------------------
        /// Notifications
        //---------------------------------------------------------
        CS_FORWARDDECLARE_STRUCT(Notification);
		CS_FORWARDDECLARE_CLASS(NotificationManager);
        CS_FORWARDDECLARE_CLASS(AppNotificationSystem);
        CS_FORWARDDECLARE_CLASS(LocalNotificationSystem);
        CS_FORWARDDECLARE_CLASS(RemoteNotificationSystem);
        //---------------------------------------------------------
        /// Resource
        //---------------------------------------------------------
        CS_FORWARDDECLARE_CLASS(Resource);
        CS_FORWARDDECLARE_CLASS(ResourcePool);
        CS_FORWARDDECLARE_CLASS(ResourceProvider);
        template <typename TResourceType> class IResourceOptions;
        template <typename TResourceType> using IResourceOptionsSPtr = std::shared_ptr<IResourceOptions<TResourceType>>;
        template <typename TResourceType> using IResourceOptionsCSPtr = std::shared_ptr<const IResourceOptions<TResourceType>>;
        CS_FORWARDDECLARE_CLASS(IResourceOptionsBase);
        //---------------------------------------------------------
        /// Scene
        //---------------------------------------------------------
        CS_FORWARDDECLARE_CLASS(Scene);
        //---------------------------------------------------------
        /// State
        //---------------------------------------------------------
        CS_FORWARDDECLARE_CLASS(State);
        CS_FORWARDDECLARE_CLASS(StateManager);
        //---------------------------------------------------------
        /// String
        //---------------------------------------------------------
        CS_FORWARDDECLARE_CLASS(UTF8String);
        //---------------------------------------------------------
        /// System
        //---------------------------------------------------------
        CS_FORWARDDECLARE_CLASS(AppSystem);
        CS_FORWARDDECLARE_CLASS(StateSystem);
        CS_FORWARDDECLARE_CLASS(IComponentProducer);
        //---------------------------------------------------------
        /// Threading
        //---------------------------------------------------------
        CS_FORWARDDECLARE_CLASS(WaitCondition);
        CS_FORWARDDECLARE_CLASS(TaskScheduler);
        CS_FORWARDDECLARE_CLASS(ThreadPool);
        //---------------------------------------------------------
        /// Time
        //---------------------------------------------------------
        CS_FORWARDDECLARE_CLASS(CoreTimer);
        CS_FORWARDDECLARE_CLASS(PerformanceTimer);
        CS_FORWARDDECLARE_CLASS(Timer);
        //---------------------------------------------------------
        /// Tween
        //---------------------------------------------------------
        template <typename TInterpType> class Tween;
        //---------------------------------------------------------
        /// Volume
        //---------------------------------------------------------
        CS_FORWARDDECLARE_CLASS(VolumeComponent);
	}
}

#endif
