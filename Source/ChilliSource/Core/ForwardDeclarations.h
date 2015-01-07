//
//  ForwardDeclarations.h
//  Chilli Source
//  Created by Ian Copland on 27/02/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
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
        CS_FORWARDDECLARE_CLASS(AppConfig);
        //---------------------------------------------------------
        /// Container
        //---------------------------------------------------------
        CS_FORWARDDECLARE_CLASS(ParamDictionary);
        CS_FORWARDDECLARE_CLASS(IProperty);
        CS_FORWARDDECLARE_CLASS(IPropertyType);
        CS_FORWARDDECLARE_CLASS(PropertyMap);
        template <typename TKey, typename TValue> class HashedArray;
        template <typename TType> class ObjectPool;
		template <typename TType> class concurrent_blocking_queue;
        template <typename TType> class concurrent_vector;
        template <typename TType> class dynamic_array;
        template <typename TType> class Property;
        template <typename TType> class PropertyType;
        template <typename TType> class random_access_iterator;
        template <typename TType> class ReferenceProperty;
        template <typename TType> class ValueProperty;
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
        CS_FORWARDDECLARE_CLASS(TaggedFilePathResolver);
        CS_FORWARDDECLARE_CLASS(CSBinaryInputStream);
        CS_FORWARDDECLARE_CLASS(CSBinaryChunk);
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
        CS_FORWARDDECLARE_CLASS(Rectangle);
        CS_FORWARDDECLARE_CLASS(Circle);
        CS_FORWARDDECLARE_CLASS(Sphere);
        CS_FORWARDDECLARE_CLASS(AABB);
        CS_FORWARDDECLARE_CLASS(OOBB);
        CS_FORWARDDECLARE_CLASS(Ray);
        CS_FORWARDDECLARE_CLASS(Line);
        CS_FORWARDDECLARE_CLASS(Plane);
        CS_FORWARDDECLARE_CLASS(Frustum);
        CS_FORWARDDECLARE_STRUCT(UnifiedScalar);
		CS_FORWARDDECLARE_STRUCT(UnifiedVector2);
		CS_FORWARDDECLARE_STRUCT(UnifiedRectangle);
		template <typename TType> class GenericVector2;
		template <typename TType> class GenericVector3;
		template <typename TType> class GenericVector4;
		template <typename TType> class GenericQuaternion;
		template <typename TType> class GenericMatrix3;
		template <typename TType> class GenericMatrix4;
		using Vector2 = GenericVector2<f32>;
		using Float2 = GenericVector2<f32>;
		using Integer2 = GenericVector2<s32>;
		using Vector3 = GenericVector3<f32>;
		using Float3 = GenericVector3<f32>;
		using Integer3 = GenericVector3<s32>;
		using Vector4 = GenericVector4<f32>;
		using Float4 = GenericVector4<f32>;
		using Integer4 = GenericVector4<s32>;
		using Quaternion = GenericQuaternion<f32>;
		using Matrix3 = GenericMatrix3<f32>;
		using Matrix4 = GenericMatrix4<f32>;
        //---------------------------------------------------------
        /// Notifications
        //---------------------------------------------------------
        CS_FORWARDDECLARE_STRUCT(Notification);
		CS_FORWARDDECLARE_CLASS(NotificationManager);
        CS_FORWARDDECLARE_CLASS(AppNotificationSystem);
        CS_FORWARDDECLARE_CLASS(LocalNotificationSystem);
        CS_FORWARDDECLARE_CLASS(RemoteNotificationSystem);
        //---------------------------------------------------------
        /// Reflection
        //---------------------------------------------------------
        namespace Reflect
        {
            CS_FORWARDDECLARE_CLASS(CInstance);
            CS_FORWARDDECLARE_CLASS(CMetaClass);
            CS_FORWARDDECLARE_CLASS(CProperty);
            enum class PropAccess;
        }
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
        //---------------------------------------------------------
        /// XML
        //---------------------------------------------------------
        CS_FORWARDDECLARE_CLASS(XML);
	}
}

#endif
