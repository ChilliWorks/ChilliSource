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

#include <ChilliSource/Core/Base/Application.h>

#if defined CS_TARGETPLATFORM_IOS || defined CS_TARGETPLATFORM_ANDROID || defined CS_TARGETPLATFORM_WINDOWS
#include <ChilliSource/Backend/Rendering/OpenGL/Base/RenderSystem.h>
#endif

namespace ChilliSource
{
	namespace Rendering
	{
        CS_DEFINE_NAMEDTYPE(RenderSystem);
        
        //-------------------------------------------------------
        //-------------------------------------------------------
        RenderSystemUPtr RenderSystem::Create(RenderCapabilities* in_renderCapabilities)
        {
#if defined CS_TARGETPLATFORM_IOS || defined CS_TARGETPLATFORM_ANDROID || defined CS_TARGETPLATFORM_WINDOWS
            return RenderSystemUPtr(new OpenGL::RenderSystem(in_renderCapabilities));
#else
            return nullptr;
#endif
        }
		//-------------------------------------------------------
		/// Constructor
		///
		/// Default
		//-------------------------------------------------------
		RenderSystem::RenderSystem()
        : mpRenderFactory(nullptr), mpSpriteBatcher(nullptr)
		{

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
