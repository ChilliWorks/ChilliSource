/*
 *  Renderer.cpp
 *  moFlo
 *
 *  Created by Scott Downie on 30/09/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Rendering/Base/Renderer.h>
#include <ChilliSource/Rendering/Base/RenderSystem.h>

#include <ChilliSource/Rendering/Texture/Texture.h>
#include <ChilliSource/Rendering/Base/RenderComponent.h>
#include <ChilliSource/Rendering/Camera/CameraComponent.h>
#include <ChilliSource/Rendering/Lighting/LightComponent.h>
#include <ChilliSource/Rendering/Lighting/DirectionalLightComponent.h>
#include <ChilliSource/Rendering/Lighting/PointLightComponent.h>
#include <ChilliSource/Rendering/Lighting/AmbientLightComponent.h>
#include <ChilliSource/Rendering/Base/RendererSortPredicates.h>
#include <ChilliSource/Rendering/Base/CullingPredicates.h>
#include <ChilliSource/Rendering/Material/MaterialFactory.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Math/Geometry/ShapeIntersection.h>
#include <ChilliSource/Rendering/GUI/Window.h>

namespace ChilliSource
{
	namespace Rendering
	{
        //---Matrix caches
        Core::CMatrix4x4 CRenderer::matViewProjCache;
		
		typedef fastdelegate::FastDelegate2<IRenderComponent*, IRenderComponent*, bool> RenderSortDelegate;
		
		//----------------------------------------------------------
		/// Constructor
		//----------------------------------------------------------
		CRenderer::CRenderer(IRenderSystem * inpRenderSystem) 
        : mpRenderSystem(inpRenderSystem), mCanvas(inpRenderSystem), mpActiveCamera(NULL)
		{
			mpTransparentSortPredicate = RendererSortPredicatePtr(new CBackToFrontSortPredicate());
            mpOpaqueSortPredicate = RendererSortPredicatePtr(new CMaterialSortPredicate());
            
            mpPerspectiveCullPredicate = CullingPredicatePtr(new CFrustumCullPredicate());
            mpOrthoCullPredicate = CullingPredicatePtr(new CViewportCullPredicate());
		}
		//----------------------------------------------------------
		/// Set Transparent Sort Predicate
		//----------------------------------------------------------
		void CRenderer::SetTransparentSortPredicate(const RendererSortPredicatePtr & inpFunctor)
        {
			mpTransparentSortPredicate = inpFunctor;
		}
        //----------------------------------------------------------
        /// Set Opaque Sort Predicate
        //----------------------------------------------------------
        void CRenderer::SetOpaqueSortPredicate(const RendererSortPredicatePtr & inpFunctor)
        {
            mpOpaqueSortPredicate = inpFunctor;
        }
        //----------------------------------------------------------
        /// Set Perspective Cull Predicate
        //----------------------------------------------------------
        void CRenderer::SetPerspectiveCullPredicate(const CullingPredicatePtr & inpFunctor)
        {
            mpPerspectiveCullPredicate = inpFunctor;
        }
        //----------------------------------------------------------
        /// Set Ortho Cull Predicate
        //----------------------------------------------------------
        void CRenderer::SetOrthoCullPredicate(const CullingPredicatePtr & inpFunctor)
        {
            mpOrthoCullPredicate = inpFunctor;
        }
		//----------------------------------------------------------
		/// Get Active Camera Pointer
		//----------------------------------------------------------
		CCameraComponent* CRenderer::GetActiveCameraPtr()
		{
			return mpActiveCamera;
		}
		//----------------------------------------------------------
		/// Render To Screen
		//----------------------------------------------------------
		void CRenderer::RenderToScreen(Core::CScene* inpScene)
		{
            RenderSceneToTarget(inpScene, mpRenderSystem->GetDefaultRenderTarget());
		}
        //----------------------------------------------------------
        /// Render To Texture
        //----------------------------------------------------------
        void CRenderer::RenderToTexture(Core::CScene* inpScene, const TexturePtr& inpColourTarget, const TexturePtr& inpDepthTarget)
		{
            //get the width and height
            u32 udwWidth = 1;
            u32 udwHeight = 1;
            if (inpColourTarget != NULL)
            {
                udwWidth = inpColourTarget->GetWidth();
                udwHeight = inpColourTarget->GetHeight();
            }
            else if (inpDepthTarget != NULL)
            {
                udwWidth = inpDepthTarget->GetWidth();
                udwHeight = inpDepthTarget->GetHeight();
            }
            
            IRenderTarget* pOffscreenTarget = mpRenderSystem->CreateRenderTarget(udwWidth, udwHeight);
            pOffscreenTarget->SetTargetTextures(inpColourTarget, inpDepthTarget);
            RenderSceneToTarget(inpScene, pOffscreenTarget);
            SAFE_DELETE(pOffscreenTarget);
		}
        //----------------------------------------------------------
		/// Render Scene To Target
		//----------------------------------------------------------
		void CRenderer::RenderSceneToTarget(Core::CScene* inpScene, IRenderTarget* inpRenderTarget)
        {
			//Traverse the scene graph and get all renderable objects
            DYNAMIC_ARRAY<IRenderComponent*> aPreFilteredRenderCache;
            DYNAMIC_ARRAY<CCameraComponent*> aCameraCache;
            DYNAMIC_ARRAY<CDirectionalLightComponent*> aDirLightCache;
            DYNAMIC_ARRAY<CPointLightComponent*> aPointLightCache;
            CAmbientLightComponent* pAmbientLight = NULL;
            
			FindRenderableObjectsInScene(inpScene, aPreFilteredRenderCache, aCameraCache, aDirLightCache, aPointLightCache, pAmbientLight);
            mpActiveCamera = (aCameraCache.empty() ? NULL : aCameraCache.back());
            
            if(mpActiveCamera)
            {
                //Apply the world view projection matrix
                mpRenderSystem->ApplyCamera(mpActiveCamera->GetEntityOwner()->Transform().GetWorldPosition(), mpActiveCamera->GetView(), mpActiveCamera->GetProjection(), mpActiveCamera->GetClearColour());
                //Calculate the view-projection matrix as we will need it for sorting
                Core::CMatrix4x4::Multiply(&mpActiveCamera->GetView(), &mpActiveCamera->GetProjection(), &matViewProjCache);
                
                //Render shadow maps
                RenderShadowMap(mpActiveCamera, aDirLightCache, aPreFilteredRenderCache);
                
                //Cull items based on camera
                DYNAMIC_ARRAY<IRenderComponent*> aCameraRenderCache;
                DYNAMIC_ARRAY<IRenderComponent*> aCameraOpaqueCache;
                DYNAMIC_ARRAY<IRenderComponent*> aCameraTransparentCache;
                CullRenderables(mpActiveCamera, aPreFilteredRenderCache, aCameraRenderCache);
                FilterSceneRenderables(aCameraRenderCache, aCameraOpaqueCache, aCameraTransparentCache);
                
                //Render scene
                mpRenderSystem->BeginFrame(inpRenderTarget);
                
                //Perform the ambient pass
                mpRenderSystem->SetLight(pAmbientLight);
                SortOpaque(mpActiveCamera, aCameraOpaqueCache);
                Render(mpActiveCamera, ShaderPass::k_ambient, aCameraOpaqueCache);
                
                //Perform the diffuse pass
                if(aDirLightCache.empty() == false || aPointLightCache.empty() == false)
                {
                    mpRenderSystem->SetBlendFunction(AlphaBlend::k_one, AlphaBlend::k_one);
                    mpRenderSystem->LockBlendFunction();
                    
                    mpRenderSystem->EnableDepthWriting(false);
                    mpRenderSystem->LockDepthWriting();
                    
                    mpRenderSystem->EnableAlphaBlending(true);
                    mpRenderSystem->LockAlphaBlending();
                    
                    for(u32 i=0; i<aDirLightCache.size(); ++i)
                    {
                        mpRenderSystem->SetLight(aDirLightCache[i]);
                        Render(mpActiveCamera, ShaderPass::k_directional, aCameraOpaqueCache);
                    }
                    
                    for(u32 i=0; i<aPointLightCache.size(); ++i)
                    {
                        mpRenderSystem->SetLight(aPointLightCache[i]);
                        DYNAMIC_ARRAY<IRenderComponent*> aPointLightOpaqueCache;
                        CullRenderables(aPointLightCache[i], aCameraOpaqueCache, aPointLightOpaqueCache);
                        Render(mpActiveCamera, ShaderPass::k_point, aPointLightOpaqueCache);
                    }
                    
                    mpRenderSystem->UnlockAlphaBlending();
                    mpRenderSystem->UnlockDepthWriting();
                    mpRenderSystem->UnlockBlendFunction();
                }

                SortTransparent(mpActiveCamera, aCameraTransparentCache);
                Render(mpActiveCamera, ShaderPass::k_ambient, aCameraTransparentCache);
                
                mpRenderSystem->SetLight(NULL);
                RenderUI(inpScene->GetWindowPtr());
                
                //Present contents of buffer to screen
                if (inpRenderTarget != NULL)
                {
                	inpRenderTarget->Discard();
                }
                mpRenderSystem->EndFrame(inpRenderTarget);
            }
            else
            {
                //Render the UI only
                //Clear the frame buffer ready for rendering
                mpRenderSystem->BeginFrame(inpRenderTarget);
                
                mpRenderSystem->SetLight(NULL);
                RenderUI(inpScene->GetWindowPtr());
                
                //Present contents of buffer to screen
                if (inpRenderTarget != NULL)
                {
                	inpRenderTarget->Discard();
                }
                mpRenderSystem->EndFrame(inpRenderTarget);
            }
        }
        //----------------------------------------------------------
		/// Find Renderable Objects In Scene
		//----------------------------------------------------------
        void CRenderer::FindRenderableObjectsInScene(Core::CScene* pScene, DYNAMIC_ARRAY<IRenderComponent*>& outaRenderCache, DYNAMIC_ARRAY<CCameraComponent*>& outaCameraCache,
                                          DYNAMIC_ARRAY<CDirectionalLightComponent*>& outaDirectionalLightComponentCache, DYNAMIC_ARRAY<CPointLightComponent*>& outaPointLightComponentCache, CAmbientLightComponent*& outpAmbientLight) const
		{
            static DYNAMIC_ARRAY<ILightComponent*> aLightComponentCache;
            aLightComponentCache.clear();
            
            pScene->QuerySceneForComponents<IRenderComponent, CCameraComponent, ILightComponent>(outaRenderCache, outaCameraCache, aLightComponentCache);
            
            //Split the lights
            for(u32 i=0; i<aLightComponentCache.size(); ++i)
            {
                if(aLightComponentCache[i]->GetEntityOwner()->IsVisible() == false)
                    continue;
                
                if(aLightComponentCache[i]->IsA(CDirectionalLightComponent::InterfaceID))
                {
                    outaDirectionalLightComponentCache.push_back((CDirectionalLightComponent*)aLightComponentCache[i]);
                }
                else if(aLightComponentCache[i]->IsA(CPointLightComponent::InterfaceID))
                {
                    outaPointLightComponentCache.push_back((CPointLightComponent*)aLightComponentCache[i]);
                }
                else if(aLightComponentCache[i]->IsA(CAmbientLightComponent::InterfaceID))
                {
                    outpAmbientLight = (CAmbientLightComponent*)aLightComponentCache[i];
                }
            }
		}
        //----------------------------------------------------------
        /// Get Cull Predicate
        //----------------------------------------------------------
        CullingPredicatePtr CRenderer::GetCullPredicate(CCameraComponent* inpActiveCamera) const
        {
            CullingPredicatePtr pCullPredicate = inpActiveCamera->GetCullingPredicate();
            
            if(pCullPredicate)
            {
                return pCullPredicate;
            }
            if(inpActiveCamera->IsOrthographicView())
            {
                return mpOrthoCullPredicate;
            }
            
            return mpPerspectiveCullPredicate;
        }
        //----------------------------------------------------------
        /// Sort Opaque
        //----------------------------------------------------------
        void CRenderer::SortOpaque(CCameraComponent* inpCameraComponent, DYNAMIC_ARRAY<IRenderComponent*>& inaRenderables) const
        {
            RendererSortPredicatePtr pOpaqueSort = inpCameraComponent->GetOpaqueSortPredicate();
            if(!pOpaqueSort)
            {
                pOpaqueSort = mpOpaqueSortPredicate;
            }
            
            if(pOpaqueSort)
            {
                pOpaqueSort->PrepareForSort(&inaRenderables);
				std::sort(inaRenderables.begin(), inaRenderables.end(), RenderSortDelegate(pOpaqueSort.get(), &CRendererSortPredicate::SortItem));
            }
        }
        //----------------------------------------------------------
        /// Sort Transparent
        //----------------------------------------------------------
        void CRenderer::SortTransparent(CCameraComponent* inpCameraComponent, DYNAMIC_ARRAY<IRenderComponent*>& inaRenderables) const
        {
            RendererSortPredicatePtr pTransparentSort = inpCameraComponent->GetTransparentSortPredicate();
            if(!pTransparentSort)
            {
                pTransparentSort = mpTransparentSortPredicate;
            }
            
			if(pTransparentSort)
            {
				pTransparentSort->PrepareForSort(&inaRenderables);
				std::sort(inaRenderables.begin(), inaRenderables.end(), RenderSortDelegate(pTransparentSort.get(), &CRendererSortPredicate::SortItem));
			}
        }
        //----------------------------------------------------------
        /// Render Shadow Map
        //----------------------------------------------------------
        void CRenderer::RenderShadowMap(CCameraComponent* inpCameraComponent, DYNAMIC_ARRAY<CDirectionalLightComponent*>& inaLightComponents, DYNAMIC_ARRAY<IRenderComponent*>& inaRenderables)
        {
            DYNAMIC_ARRAY<IRenderComponent*> aFilteredShadowMapRenderCache;
            
            if(inaLightComponents.size() > 0)
            {
                //Cull items based on whether they cast shadows
                FilterShadowMapRenderables(inaRenderables, aFilteredShadowMapRenderCache);
            }
            
            for(u32 i=0; i<inaLightComponents.size(); ++i)
            {
                if(inaLightComponents[i]->GetShadowMapPtr() != NULL)
                {
                    mpRenderSystem->SetLight(inaLightComponents[i]);
                    RenderShadowMap(mpActiveCamera, inaLightComponents[i], aFilteredShadowMapRenderCache);
                }
            }
        }
        //----------------------------------------------------------
		/// Render Shadow Map
		//----------------------------------------------------------
		void CRenderer::RenderShadowMap(CCameraComponent* inpCameraComponent, CDirectionalLightComponent* inpLightComponent, DYNAMIC_ARRAY<IRenderComponent*>& inaRenderables)
		{
			//Create a new offscreen render target using the given texture
			IRenderTarget* pRenderTarget = mpRenderSystem->CreateRenderTarget(inpLightComponent->GetShadowMapPtr()->GetWidth(), inpLightComponent->GetShadowMapPtr()->GetHeight());
			pRenderTarget->SetTargetTextures(inpLightComponent->GetShadowMapDebugPtr(), inpLightComponent->GetShadowMapPtr());
            
            mpRenderSystem->BeginFrame(pRenderTarget);
            
            //Only opaque objects cast and receive shadows
            for(DYNAMIC_ARRAY<IRenderComponent*>::const_iterator it = inaRenderables.begin(); it != inaRenderables.end(); ++it)
            {
                (*it)->RenderShadowMap(mpRenderSystem, inpCameraComponent);
            }
            
            mpRenderSystem->EndFrame(pRenderTarget);
            
            SAFE_DELETE(pRenderTarget);
		}
        //----------------------------------------------------------
		/// Render
		//----------------------------------------------------------
		void CRenderer::Render(CCameraComponent* inpCameraComponent, ShaderPass ineShaderPass, DYNAMIC_ARRAY<IRenderComponent*>& inaRenderables)
		{
            for(DYNAMIC_ARRAY<IRenderComponent*>::const_iterator it = inaRenderables.begin(); it != inaRenderables.end(); ++it)
            {
                (*it)->Render(mpRenderSystem, inpCameraComponent, ineShaderPass);
            }
			
            //The final dynamic sprite batch needs to be flushed
            mpRenderSystem->GetDynamicSpriteBatchPtr()->ForceRender(mpRenderSystem);
        }
        //----------------------------------------------------------
        /// Render UI
        //----------------------------------------------------------
        void CRenderer::RenderUI(CWindow* inpWindow)
        {
            mpRenderSystem->ApplyCamera(Core::CVector3::ZERO, Core::CMatrix4x4::IDENTITY, CreateOverlayProjection(inpWindow), ChilliSource::Core::CColour::CORNFLOWER_BLUE);
			mCanvas.Render(inpWindow, 1.0f);
        }
        //----------------------------------------------------------
        /// Cull Renderables
        //----------------------------------------------------------
		void CRenderer::CullRenderables(CCameraComponent* inpCamera, const DYNAMIC_ARRAY<IRenderComponent*>& inaRenderCache, DYNAMIC_ARRAY<IRenderComponent*>& outaRenderCache) const
		{
            ICullingPredicate * pCullingPredicate = GetCullPredicate(inpCamera).get();
            
            if(pCullingPredicate == NULL)
            {
                outaRenderCache = inaRenderCache;
                return;
            }
            
            outaRenderCache.reserve(inaRenderCache.size());
            
            inpCamera->UpdateFrustum();
            
			for(DYNAMIC_ARRAY<IRenderComponent*>::const_iterator it = inaRenderCache.begin(); it != inaRenderCache.end(); ++it)
			{
				IRenderComponent* pRenderable = (*it);
				
				if(pRenderable->IsVisible() == false)
                {
                    continue;
                }
                
                if(pRenderable->IsCullingEnabled() == false || pCullingPredicate->CullItem(inpCamera, pRenderable) == false)
                {
                    outaRenderCache.push_back(pRenderable);
                    continue;
                }
			}
		}
        //----------------------------------------------------------
        /// Cull Renderables
        //----------------------------------------------------------
		void CRenderer::CullRenderables(CPointLightComponent* inpLightComponent, const DYNAMIC_ARRAY<IRenderComponent*>& inaRenderCache, DYNAMIC_ARRAY<IRenderComponent*>& outaRenderCache) const
        {
            //Reserve estimated space
            outaRenderCache.reserve(inaRenderCache.size());
            
            Core::Sphere aLightSphere;
            aLightSphere.vOrigin = inpLightComponent->GetWorldPosition();
            aLightSphere.fRadius = inpLightComponent->GetRangeOfInfluence();
            
            for(DYNAMIC_ARRAY<IRenderComponent*>::const_iterator it = inaRenderCache.begin(); it != inaRenderCache.end(); ++it)
            {
                if(Core::CIntersection::Intersects(aLightSphere, (*it)->GetBoundingSphere()) == true)
                {
                    outaRenderCache.push_back(*it);
                }
            }
        }
        //----------------------------------------------------------
        /// Filter Scene Renderables
        //----------------------------------------------------------
		void CRenderer::FilterSceneRenderables(const DYNAMIC_ARRAY<IRenderComponent*>& inaRenderables, DYNAMIC_ARRAY<IRenderComponent*>& outaOpaque, DYNAMIC_ARRAY<IRenderComponent*>& outaTransparent) const
		{
            //Reserve estimated space
            outaOpaque.reserve(inaRenderables.size());
            outaTransparent.reserve(inaRenderables.size());
            
			for(DYNAMIC_ARRAY<IRenderComponent*>::const_iterator it = inaRenderables.begin(); it != inaRenderables.end(); ++it)
			{
				IRenderComponent* pRenderable = (*it);
                pRenderable->IsTransparent() ? outaTransparent.push_back(pRenderable) : outaOpaque.push_back(pRenderable);
			}
		}
        //----------------------------------------------------------
        /// Filter Shadow Map Renderables
        //----------------------------------------------------------
        void CRenderer::FilterShadowMapRenderables(const DYNAMIC_ARRAY<IRenderComponent*>& inaRenderables, DYNAMIC_ARRAY<IRenderComponent*>& outaRenderables) const
        {
            //Reserve estimated space
            outaRenderables.reserve(inaRenderables.size());
            
            for(DYNAMIC_ARRAY<IRenderComponent*>::const_iterator it = inaRenderables.begin(); it != inaRenderables.end(); ++it)
			{
				IRenderComponent* pRenderable = (*it);
                if(pRenderable->IsShadowCastingEnabled() == true && pRenderable->IsTransparent() == false)
                {
                    outaRenderables.push_back(pRenderable);
                }
			}
        }
        //----------------------------------------------------------
        /// Create Overlay Projection
        //----------------------------------------------------------
        Core::CMatrix4x4 CRenderer::CreateOverlayProjection(CWindow* inpWindow) const
        {
            const Core::CVector2 kvOverlayDimensions(inpWindow->GetAbsoluteSize());
            const f32 kfOverlayNear = 1.0f;
            const f32 kfOverlayFar = 100.0f;
            return Core::CMatrix4x4::CreateOrthoMatrixOffset(0, kvOverlayDimensions.x, 0, kvOverlayDimensions.y, kfOverlayNear, kfOverlayFar);
        }
	}
}
