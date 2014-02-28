/*
 *  RenderSystem.cpp
 *  moFlo
 *
 *  Created by Scott Downie on 13/10/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Rendering/Sprite/DynamicSpriteBatcher.h>
#include <ChilliSource/Rendering/Base/RenderSystem.h>
#include <ChilliSource/Rendering/Base/RenderComponentFactory.h>

#include <ChilliSource/Input/Base/InputSystem.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Resource/ResourceManagerDispenser.h>

namespace ChilliSource
{
	namespace Rendering
	{
        DEFINE_NAMED_INTERFACE(RenderSystem);
		//-------------------------------------------------------
		/// Constructor
		///
		/// Default
		//-------------------------------------------------------
		RenderSystem::RenderSystem()
        : mpRenderFactory(nullptr), mpSpriteBatcher(nullptr)
		{
            Core::ResourceManagerDispenser::GetSingletonPtr()->RegisterResourceManager(&mMeshManager);
			Core::ResourceManagerDispenser::GetSingletonPtr()->RegisterResourceManager(&mSkinnedAnimationManager);
			Core::ResourceManagerDispenser::GetSingletonPtr()->RegisterResourceManager(&mFontManager);
			Core::ResourceManagerDispenser::GetSingletonPtr()->RegisterResourceManager(&mMaterialManager);
			Core::ResourceManagerDispenser::GetSingletonPtr()->RegisterResourceManager(&mSpriteManager);
		}
        //----------------------------------------------------
        /// Get Dynamic Sprite Batch Pointer
        ///
        /// @return Pointer to dynamic sprite batcher
        //----------------------------------------------------
        DynamicSpriteBatch* RenderSystem::GetDynamicSpriteBatchPtr()
        {
            if(mpSpriteBatcher == nullptr)
            {
                mpSpriteBatcher = new DynamicSpriteBatch(this);
            }
            
            return mpSpriteBatcher;
        }
		//----------------------------------------------------
		/// Get Number Of Component Factories
		///
		/// @return Number of factories in this system
		//----------------------------------------------------
		u32 RenderSystem::GetNumComponentFactories() const
		{
			return 1;
		}
		//-------------------------------------------------------
		/// Get Component Factory Pointer
		///
		/// 
		//-------------------------------------------------------
		Core::ComponentFactory* RenderSystem::GetComponentFactoryPtr(u32 inudwIndex)
		{
            if(mpRenderFactory == nullptr)
            {
                mpRenderFactory = new RenderComponentFactory(this);
            }
            
            return mpRenderFactory;
		}
		//-------------------------------------------------------
		/// Get Component Factory
		///
		/// 
		//-------------------------------------------------------
		Core::ComponentFactory& RenderSystem::GetComponentFactory(u32 inudwIndex)
		{
            if(mpRenderFactory == nullptr)
            {
                mpRenderFactory = new RenderComponentFactory(this);
            }
            
            return *mpRenderFactory;
		}
		//-------------------------------------------------------
		/// Destructor
		//-------------------------------------------------------
		RenderSystem::~RenderSystem()
		{
			CS_SAFEDELETE(mpRenderFactory);
            CS_SAFEDELETE(mpSpriteBatcher);
		}
	}
}
