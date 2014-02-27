//
//  ForwardDeclarations.h
//  Chilli Source
//
//  Created by Ian Copland on 27/02/2014.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_CORE_FORWARDDECLARATIONS_H_
#define _CHILLISOURCE_CORE_FORWARDDECLARATIONS_H_

#include <ChilliSource/ChilliSource.h>

namespace ChilliSource 
{
	namespace Core
	{
        //---------------------------------------------------------
        /// Animation
        //---------------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(Animation);
        CS_FORWARD_DECLARE_CLASS(ValueAnimation);
        CS_FORWARD_DECLARE_CLASS(LinearAnimation);
        CS_FORWARD_DECLARE_CLASS(BounceAnimation);
        CS_FORWARD_DECLARE_CLASS(KeyframeAnimation);
        enum class InterpolationType;
        //---------------------------------------------------------
        /// Base
        //---------------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(Application);
        CS_FORWARD_DECLARE_CLASS(CColour);
        CS_FORWARD_DECLARE_CLASS(IInformationProvider);
        CS_FORWARD_DECLARE_CLASS(IPlatformSystem);
        CS_FORWARD_DECLARE_CLASS(IQueryableInterface);
        //---------------------------------------------------------
        /// Container
        //---------------------------------------------------------
        template <typename TKey, typename TValue> class CHashedArray;
        template <typename TType> class CObjectPool;
        CS_FORWARD_DECLARE_CLASS(COctree);
        CS_FORWARD_DECLARE_CLASS(ParamDictionary);
        CS_FORWARD_DECLARE_CLASS(CStaticMemPool);
        template <typename T> class WorkerQueue;
        //---------------------------------------------------------
        /// Cryptographic
        //---------------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(CHMAC_SHA1);
        CS_FORWARD_DECLARE_CLASS(COAuthSystem);
        CS_FORWARD_DECLARE_CLASS(CSHA1);
        //---------------------------------------------------------
        /// Entity
        //---------------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(IComponent);
        CS_FORWARD_DECLARE_CLASS(IComponentFactory);
        CS_FORWARD_DECLARE_CLASS(CComponentFactoryDispenser);
        CS_FORWARD_DECLARE_CLASS(CEntity);
        CS_FORWARD_DECLARE_CLASS(CEntityAnimation);
        CS_FORWARD_DECLARE_CLASS(CTransform);
        //---------------------------------------------------------
        /// Event
        //---------------------------------------------------------
        template <typename TDelegateType> class IEvent;
        template <typename TDelegateType> class CEvent0;
        template <typename TDelegateType> class CEvent1;
        template <typename TDelegateType> class CEvent2;
        template <typename TDelegateType> class CEvent3;
        template <typename TDelegateType> class CEvent4;
        //---------------------------------------------------------
        /// File
        //---------------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(IFileStream);
        CS_FORWARD_DECLARE_CLASS(IFileSystem);
        CS_FORWARD_DECLARE_CLASS(CTweakableConstants);
        enum class StorageLocation;
        //---------------------------------------------------------
        /// Image
        //---------------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(CETC1ImageProvider);
        CS_FORWARD_DECLARE_CLASS(CImage);
        CS_FORWARD_DECLARE_CLASS(IImageResourceProvider);
        CS_FORWARD_DECLARE_CLASS(CMoImageProvider);
        //---------------------------------------------------------
        /// Localisation
        //---------------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(CLocale);
        //---------------------------------------------------------
        /// Math
        //---------------------------------------------------------
        template <typename T> class CCatmullRomSpline;
        template <typename T> class CMultiLineSegment;
        CS_FORWARD_DECLARE_CLASS(UVRect);
        CS_FORWARD_DECLARE_CLASS(Rectangle);
        CS_FORWARD_DECLARE_CLASS(Circle);
        CS_FORWARD_DECLARE_CLASS(Sphere);
        CS_FORWARD_DECLARE_CLASS(AABB);
        CS_FORWARD_DECLARE_CLASS(OOBB);
        CS_FORWARD_DECLARE_CLASS(Ray);
        CS_FORWARD_DECLARE_CLASS(Line);
        CS_FORWARD_DECLARE_CLASS(Plane);
        CS_FORWARD_DECLARE_CLASS(CFrustum);
        CS_FORWARD_DECLARE_CLASS(CMatrix3x3);
        CS_FORWARD_DECLARE_CLASS(CMatrix4x4);
        CS_FORWARD_DECLARE_CLASS(CQuaternion);
        CS_FORWARD_DECLARE_CLASS(UnifiedScalar);
        CS_FORWARD_DECLARE_CLASS(UnifiedVector2);
        CS_FORWARD_DECLARE_CLASS(UnifiedRectangle);
        CS_FORWARD_DECLARE_CLASS(CVector2);
        CS_FORWARD_DECLARE_CLASS(CVector3);
        CS_FORWARD_DECLARE_CLASS(CVector4);
        //---------------------------------------------------------
        /// Notifications
        //---------------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(ILocalNotificationScheduler);
        CS_FORWARD_DECLARE_CLASS(CNotificationScheduler);
        CS_FORWARD_DECLARE_CLASS(Notification);
        CS_FORWARD_DECLARE_CLASS(IRemoteNotificationSystem);
        //---------------------------------------------------------
        /// Resource
        //---------------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(IResource);
        CS_FORWARD_DECLARE_CLASS(IResourceGroupManager);
        CS_FORWARD_DECLARE_CLASS(IResourceManager);
        CS_FORWARD_DECLARE_CLASS(CResourceManagerDispenser);
        CS_FORWARD_DECLARE_CLASS(IResourceProvider);
        //---------------------------------------------------------
        /// Scene
        //---------------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(CScene);
        CS_FORWARD_DECLARE_CLASS(CSceneAnimation);
        CS_FORWARD_DECLARE_CLASS(CSceneAnimationDesc);
        CS_FORWARD_DECLARE_CLASS(CSceneAnimationProvider);
        CS_FORWARD_DECLARE_CLASS(CSceneAnimDescManager);
        CS_FORWARD_DECLARE_CLASS(CSceneDescManager);
        CS_FORWARD_DECLARE_CLASS(CSceneDesc);
        CS_FORWARD_DECLARE_CLASS(CSceneProvider);
        //---------------------------------------------------------
        /// State
        //---------------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(IState);
        CS_FORWARD_DECLARE_CLASS(CStateManager);
        //---------------------------------------------------------
        /// String
        //---------------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(CUTF8String);
        //---------------------------------------------------------
        /// System
        //---------------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(ISystem);
        //---------------------------------------------------------
        /// Threading
        //---------------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(WaitCondition);
        template <typename... TArgTypes> class Task;
        CS_FORWARD_DECLARE_CLASS(ThreadPool);
        //---------------------------------------------------------
        /// Time
        //---------------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(CPerformanceTimer);
        CS_FORWARD_DECLARE_CLASS(CTimer);
        //---------------------------------------------------------
        /// Volume
        //---------------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(IVolumeComponent);
	}
}

#endif
