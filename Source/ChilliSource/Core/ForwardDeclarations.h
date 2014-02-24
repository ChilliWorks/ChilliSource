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

namespace ChilliSource 
{
	class IActivity;
	class IInformationProvider;
	struct UnifiedVector2;
	class IVolumeComponent;
	class IPlatformSystem;
	class IWebViewActivity;
    
    typedef SHARED_PTR<IVolumeComponent> VolumeComponentPtr;
    typedef WEAK_PTR<IVolumeComponent> VolumeComponentWeakPtr;
    
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
        typedef SHARED_PTR<CEntity> EntityPtr;
        typedef WEAK_PTR<CEntity> EntityWeakPtr;
        
		class IComponent;
        typedef SHARED_PTR<IComponent> ComponentPtr;
		class IComponentFactory;
		
		//Scene
		class CScene;
		class CComponentSceneQuery;
		class CRaySceneQuery;
		
		class CEntityAnimationController;
		class CEntityAnimation;
		typedef SHARED_PTR<CEntityAnimation> EntityAnimationPtr;
		typedef SHARED_PTR<CEntityAnimationController> EntityAnimationControllerPtr;
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
        
        typedef SHARED_PTR<IFileStream> FileStreamPtr;
        
		typedef SHARED_PTR<IResource> ResourcePtr;
		typedef WEAK_PTR<IResource> ResourceWeakPtr;

        typedef SHARED_PTR<CImage> ImagePtr;
		typedef WEAK_PTR<CImage> ImageWeakPtr;

        
        enum class InterpolationType
        {
            k_step,
            k_linear
        };
	}
}

#endif
