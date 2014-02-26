/*
 *  CoreForwardDeclarations.h
 *  moFlo
 *
 *  Created by Stuart McGaw on 23/09/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLO_CORE_FORWARDDECLARATIONS_H_
#define _MOFLO_CORE_FORWARDDECLARATIONS_H_

#include <ChilliSource/ChilliSource.h>

namespace ChilliSource 
{
	namespace Core
	{
		class CApplication;
        class CResourceManagerDispenser;
        class CComponentFactoryDispenser;
		
		class ISystem;
		
		//System concepts
		class IUpdateable;
		class IComponentProducer;
		
		class INotificationsSystem;
		class CNotificationComponent;

		class CStateManager;
		class IState;
		
		//ComponentSystem
		class CEntity;
        typedef std::shared_ptr<CEntity> EntityPtr;
        typedef std::weak_ptr<CEntity> EntityWeakPtr;
        
		class IComponent;
        typedef std::shared_ptr<IComponent> ComponentPtr;
		class IComponentFactory;
		
		//Scene
		class CScene;
		class CComponentSceneQuery;
		class CRaySceneQuery;
		
		class CEntityAnimationController;
		class CEntityAnimation;
		typedef std::shared_ptr<CEntityAnimation> EntityAnimationPtr;
		typedef std::shared_ptr<CEntityAnimationController> EntityAnimationControllerPtr;
		struct EntityAnimationData;
		
		//Resources
		class IResource;
		class IResourceProvider;
		class IResourceManager;
		class IResourceGroupManager;
		
		class CLocalisedText;

		class CScreen;
		struct ParamDictionary;
		class CLogging;
		class CTimer;
		class CImage;
		class CColour;

		
		//Math
		class CVector2;
		class CVector3;
		class CVector4;
		class CQuaternion;
        class CMatrix3x3;
		class CMatrix4x4;
        struct UnifiedRectangle;
		
		//Shapes
		struct Rectangle;
		class OOBB;
		struct AABB;
		struct Ray;
		struct Circle;
		struct Sphere;
        struct Plane;
		class CFrustum;

		//File io classes
		class IFileSystem;
		class IFileStream;
		class IWFileStream;
		class CFileSystemFactory;
        
        typedef std::shared_ptr<IFileStream> FileStreamPtr;
        
		typedef std::shared_ptr<IResource> ResourcePtr;
		typedef std::weak_ptr<IResource> ResourceWeakPtr;

        typedef std::shared_ptr<CImage> ImagePtr;
		typedef std::weak_ptr<CImage> ImageWeakPtr;

        class IActivity;
        class IInformationProvider;
        struct UnifiedVector2;
        class IVolumeComponent;
        class IPlatformSystem;
        class WebViewActivity;
        
        typedef std::shared_ptr<IVolumeComponent> VolumeComponentPtr;
        typedef std::weak_ptr<IVolumeComponent> VolumeComponentWeakPtr;
        
        enum class StorageLocation;
        
        enum class InterpolationType
        {
            k_step,
            k_linear
        };
	}
}

#endif
