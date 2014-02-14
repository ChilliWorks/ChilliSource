/*
 *  RenderSystem.cpp
 *  moFlo
 *
 *  Created by Scott Downie on 13/10/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Rendering/DynamicSpriteBatcher.h>
#include <ChilliSource/Rendering/Main/RenderSystem.h>
#include <ChilliSource/Rendering/Main/RenderComponentFactory.h>

#include <ChilliSource/Input/Main/InputSystem.h>

#include <ChilliSource/Core/Main/Application.h>
#include <ChilliSource/Core/Resource/ResourceManagerDispenser.h>

namespace moFlo
{
	namespace Rendering
	{
        DEFINE_NAMED_INTERFACE(IRenderSystem);
		//-------------------------------------------------------
		/// Constructor
		///
		/// Default
		//-------------------------------------------------------
		IRenderSystem::IRenderSystem()
        : mpRenderFactory(NULL), mpSpriteBatcher(NULL)
		{
            Core::CResourceManagerDispenser::GetSingletonPtr()->RegisterResourceManager(&mMeshManager);
			Core::CResourceManagerDispenser::GetSingletonPtr()->RegisterResourceManager(&mSkinnedAnimationManager);
			Core::CResourceManagerDispenser::GetSingletonPtr()->RegisterResourceManager(&mFontManager);
			Core::CResourceManagerDispenser::GetSingletonPtr()->RegisterResourceManager(&mMaterialManager);
			Core::CResourceManagerDispenser::GetSingletonPtr()->RegisterResourceManager(&mSpriteManager);
		}
        //----------------------------------------------------
        /// Get Dynamic Sprite Batch Pointer
        ///
        /// @return Pointer to dynamic sprite batcher
        //----------------------------------------------------
        CDynamicSpriteBatch* IRenderSystem::GetDynamicSpriteBatchPtr()
        {
            if(mpSpriteBatcher == NULL)
            {
                mpSpriteBatcher = new CDynamicSpriteBatch(this);
            }
            
            return mpSpriteBatcher;
        }
		//----------------------------------------------------
		/// Get Number Of Component Factories
		///
		/// @return Number of factories in this system
		//----------------------------------------------------
		u32 IRenderSystem::GetNumComponentFactories() const
		{
			return 1;
		}
		//-------------------------------------------------------
		/// Get Component Factory Pointer
		///
		/// 
		//-------------------------------------------------------
		Core::IComponentFactory* IRenderSystem::GetComponentFactoryPtr(u32 inudwIndex)
		{
            if(mpRenderFactory == NULL)
            {
                mpRenderFactory = new CRenderComponentFactory(this);
            }
            
            return mpRenderFactory;
		}
		//-------------------------------------------------------
		/// Get Component Factory
		///
		/// 
		//-------------------------------------------------------
		Core::IComponentFactory& IRenderSystem::GetComponentFactory(u32 inudwIndex)
		{
            if(mpRenderFactory == NULL)
            {
                mpRenderFactory = new CRenderComponentFactory(this);
            }
            
            return *mpRenderFactory;
		}
		//-------------------------------------------------------
		/// Destructor
		//-------------------------------------------------------
		IRenderSystem::~IRenderSystem()
		{
			SAFE_DELETE(mpRenderFactory);
            SAFE_DELETE(mpSpriteBatcher);
		}
	}
}
