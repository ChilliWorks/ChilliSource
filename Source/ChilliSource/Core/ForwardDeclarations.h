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
        /// Animation
        //---------------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(Animation);
        template <typename TType> class ValueAnimation;
        template <typename TType> class LinearAnimation;
        template <typename TType> class BounceAnimation;
        template <typename TType> class KeyframeAnimation;
        enum class InterpolationType;
        //---------------------------------------------------------
        /// Base
        //---------------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(Activity);
        CS_FORWARD_DECLARE_CLASS(Application);
        CS_FORWARD_DECLARE_CLASS(Colour);
        CS_FORWARD_DECLARE_CLASS(IInformationProvider);
        CS_FORWARD_DECLARE_CLASS(PlatformSystem);
        CS_FORWARD_DECLARE_CLASS(QueryableInterface);
        //---------------------------------------------------------
        /// Container
        //---------------------------------------------------------
        template <typename TKey, typename TValue> class HashedArray;
        template <typename TType> class ObjectPool;
        CS_FORWARD_DECLARE_CLASS(Octree);
        CS_FORWARD_DECLARE_CLASS(ParamDictionary);
        CS_FORWARD_DECLARE_CLASS(StaticMemPool);
        template <typename T> class WorkerQueue;
        //---------------------------------------------------------
        /// Cryptographic
        //---------------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(HMAC_SHA1);
        CS_FORWARD_DECLARE_CLASS(OAuthSystem);
        CS_FORWARD_DECLARE_CLASS(SHA1);
        //---------------------------------------------------------
        /// Entity
        //---------------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(Component);
        CS_FORWARD_DECLARE_CLASS(ComponentFactory);
        CS_FORWARD_DECLARE_CLASS(ComponentFactoryDispenser);
        CS_FORWARD_DECLARE_CLASS(Entity);
        CS_FORWARD_DECLARE_CLASS(EntityAnimation);
        CS_FORWARD_DECLARE_CLASS(EntityAnimationController);
        CS_FORWARD_DECLARE_CLASS(Transform);
        //---------------------------------------------------------
        /// Event
        //---------------------------------------------------------
        template <typename TDelegateType> class Event;
        template <typename TDelegateType> class IConnectableEvent;
        CS_FORWARD_DECLARE_CLASS(IDisconnectableEvent);
        CS_FORWARD_DECLARE_CLASS(Connection);
        //---------------------------------------------------------
        /// File
        //---------------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(FileStream);
        CS_FORWARD_DECLARE_CLASS(FileSystem);
        CS_FORWARD_DECLARE_CLASS(LocalDataStore);
        CS_FORWARD_DECLARE_CLASS(TweakableConstants);
        enum class StorageLocation;
        //---------------------------------------------------------
        /// Image
        //---------------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(ETC1ImageProvider);
        CS_FORWARD_DECLARE_CLASS(Image);
        CS_FORWARD_DECLARE_CLASS(ImageResourceProvider);
        CS_FORWARD_DECLARE_CLASS(MoImageProvider);
        //---------------------------------------------------------
        /// Localisation
        //---------------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(Locale);
        //---------------------------------------------------------
        /// Math
        //---------------------------------------------------------
        template <typename T> class CatmullRomSpline;
        template <typename T> class MultiLineSegment;
        CS_FORWARD_DECLARE_CLASS(UVRect);
        CS_FORWARD_DECLARE_CLASS(Rectangle);
        CS_FORWARD_DECLARE_CLASS(Circle);
        CS_FORWARD_DECLARE_CLASS(Sphere);
        CS_FORWARD_DECLARE_CLASS(AABB);
        CS_FORWARD_DECLARE_CLASS(OOBB);
        CS_FORWARD_DECLARE_CLASS(Ray);
        CS_FORWARD_DECLARE_CLASS(Line);
        CS_FORWARD_DECLARE_CLASS(Plane);
        CS_FORWARD_DECLARE_CLASS(Frustum);
        CS_FORWARD_DECLARE_CLASS(Matrix3x3);
        CS_FORWARD_DECLARE_CLASS(Matrix4x4);
        CS_FORWARD_DECLARE_CLASS(Quaternion);
        CS_FORWARD_DECLARE_CLASS(UnifiedScalar);
        CS_FORWARD_DECLARE_CLASS(UnifiedVector2);
        CS_FORWARD_DECLARE_CLASS(UnifiedRectangle);
        CS_FORWARD_DECLARE_CLASS(Vector2);
        CS_FORWARD_DECLARE_CLASS(Vector3);
        CS_FORWARD_DECLARE_CLASS(Vector4);
        //---------------------------------------------------------
        /// Notifications
        //---------------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(LocalNotificationScheduler);
        CS_FORWARD_DECLARE_CLASS(NotificationScheduler);
        CS_FORWARD_DECLARE_CLASS(Notification);
        CS_FORWARD_DECLARE_CLASS(RemoteNotificationSystem);
        //---------------------------------------------------------
        /// Resource
        //---------------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(Resource);
        CS_FORWARD_DECLARE_CLASS(ResourceGroupManager);
        CS_FORWARD_DECLARE_CLASS(ResourceManager);
        CS_FORWARD_DECLARE_CLASS(ResourceManagerDispenser);
        CS_FORWARD_DECLARE_CLASS(ResourceProvider);
        //---------------------------------------------------------
        /// Scene
        //---------------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(Scene);
        CS_FORWARD_DECLARE_CLASS(SceneAnimation);
        CS_FORWARD_DECLARE_CLASS(SceneAnimationDesc);
        CS_FORWARD_DECLARE_CLASS(SceneAnimationProvider);
        CS_FORWARD_DECLARE_CLASS(SceneAnimDescManager);
        CS_FORWARD_DECLARE_CLASS(SceneDescManager);
        CS_FORWARD_DECLARE_CLASS(SceneDesc);
        CS_FORWARD_DECLARE_CLASS(SceneProvider);
        //---------------------------------------------------------
        /// State
        //---------------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(State);
        CS_FORWARD_DECLARE_CLASS(StateManager);
        //---------------------------------------------------------
        /// String
        //---------------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(UTF8String);
        //---------------------------------------------------------
        /// System
        //---------------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(System);
        CS_FORWARD_DECLARE_CLASS(IUpdateable);
        CS_FORWARD_DECLARE_CLASS(IComponentProducer);
        //---------------------------------------------------------
        /// Threading
        //---------------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(WaitCondition);
        template <typename... TArgTypes> class Task;
        CS_FORWARD_DECLARE_CLASS(ThreadPool);
        //---------------------------------------------------------
        /// Time
        //---------------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(PerformanceTimer);
        CS_FORWARD_DECLARE_CLASS(Timer);
        //---------------------------------------------------------
        /// Volume
        //---------------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(VolumeComponent);
	}
}

#endif
