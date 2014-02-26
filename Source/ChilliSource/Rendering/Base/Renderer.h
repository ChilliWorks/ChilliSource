/*
 *  Renderer.h
 *  moFlo
 *
 *  Created by Scott Downie on 30/09/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#ifndef _MO_FLO_RENDERING_RENDERER_H_
#define _MO_FLO_RENDERING_RENDERER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Entity/Component.h>
#include <ChilliSource/Rendering/Base/CanvasRenderer.h>

namespace ChilliSource
{
	namespace Rendering
	{
		//==================================================
		/// Description:
		///
		/// Does a pre-pass on the scene to convert moFlo
		/// components to plain data for the render system
		//==================================================
		class CRenderer 
		{
		public:
			CRenderer(IRenderSystem * inpRenderSystem);
			//----------------------------------------------------------
			/// Set Transparent Sort Predicate
			///
			/// Sets the sort predicate to use for sorting transparent objects
			/// @param Our CRendererSortPredicate functor. If this is nullptr the renderer will not sort transparent objects
			//----------------------------------------------------------
			void SetTransparentSortPredicate(const RendererSortPredicatePtr & inpFunctor);
            //----------------------------------------------------------
			/// Set Opaque Sort Predicate
			///
			/// Sets the sort predicate to use for sorting opaque objects
			/// @param Our CRendererSortPredicate functor. If this is nullptr the renderer will not sort opaque objects
			//----------------------------------------------------------
			void SetOpaqueSortPredicate(const RendererSortPredicatePtr & inpFunctor);
            
            //----------------------------------------------------------
			/// Set Perspective Cull Predicate
			///
            /// @param Predicate that is called to determine whether
            /// an object should be culled with an perspective camera
			//----------------------------------------------------------
			void SetPerspectiveCullPredicate(const CullingPredicatePtr & inpFunctor);
            //----------------------------------------------------------
			/// Set Ortho Cull Predicate
			///
            /// @param Predicate that is called to determine whether
            /// an object should be culled with an orthographic camera
			//----------------------------------------------------------
			void SetOrthoCullPredicate(const CullingPredicatePtr & inpFunctor);
            //----------------------------------------------------------
			/// Render To Screen
			///
			/// Render the scene via active the render system to the
			/// frame buffer. This function is called by the system
			///
			/// @param Scene to render
			//----------------------------------------------------------
			void RenderToScreen(Core::CScene* inpScene);
			//----------------------------------------------------------
			/// Render To Texture
			///
			/// Render the scene to texture via active the render system
            /// If no colour texture is provided only the depth will
            /// be rendered. If no depth target is provided a depth
            /// buffer will be created.
            ///
			/// @param Handle to the scene 
			/// @param Texture to render colour to.
            /// @param Texture to render depth to.
			//----------------------------------------------------------
			void RenderToTexture(Core::CScene* inpScene, const TexturePtr &inpColourTarget, const TexturePtr& inpDepthTarget = TexturePtr());
			//----------------------------------------------------------
			/// Get Active Camera Pointer
			///
			/// @return A weak pointer to the active scene camera
			//----------------------------------------------------------
			CCameraComponent* GetActiveCameraPtr();
            
            static Core::CMatrix4x4 matViewProjCache;
			
		private:
			
            //----------------------------------------------------------
            /// Render Shadow Map
            ///
            /// Generate a shadow map from the current scene
            ///
            /// @param Camera component
            /// @param Light components
            /// @param Render components
            //----------------------------------------------------------
            void RenderShadowMap(CCameraComponent* inpCameraComponent, DYNAMIC_ARRAY<CDirectionalLightComponent*>& inaLightComponents, DYNAMIC_ARRAY<IRenderComponent*>& inaRenderables);
            //----------------------------------------------------------
            /// Render Shadow Map
            ///
            /// Generate a shadow map from the current scene
            ///
            /// @param Camera component
            /// @param Light component
            /// @param Render components
            //----------------------------------------------------------
            void RenderShadowMap(CCameraComponent* inpCameraComponent, CDirectionalLightComponent* inpLightComponent, DYNAMIC_ARRAY<IRenderComponent*>& inaRenderables);
			//----------------------------------------------------------
			/// Render
			///
			/// This function renders the contents of the scene to the
			/// currently bound render target (i.e. the frame buffer or
			/// an offscreen buffer) for the given shader pass.
            ///
            /// @param Camera component
            /// @param The shader pass.
            /// @param Renderables
			//----------------------------------------------------------
			void Render(CCameraComponent* inpCameraComponent, ShaderPass ineShaderPass, DYNAMIC_ARRAY<IRenderComponent*>& inaRenderables);
            //----------------------------------------------------------
			/// Render UI
            ///
            /// Render the UI views to screen space
            ///
            /// @param Root window
			//----------------------------------------------------------
			void RenderUI(GUI::CWindow* inpWindow);
            //----------------------------------------------------------
            /// Render Scene To Target
            ///
            /// Render the contents of the scene to the target buffer
            ///
            /// @param Scene
            /// @param Target
            //----------------------------------------------------------
            void RenderSceneToTarget(Core::CScene* inpScene, IRenderTarget* inpRenderTarget);
			//----------------------------------------------------------
			/// Find Renderable Objects In Scene
			///
			/// Traverse the scene graph and get handles to
			/// all objects of render type. These objects will
			/// be contained and sorted.
			/// @param Handle to the scene graph
            /// @param Out: Renderables
            /// @param Out: Camera
            /// @param Out: Dir lights
            /// @param Out: Point lights
			//----------------------------------------------------------
			void FindRenderableObjectsInScene(Core::CScene* pScene, DYNAMIC_ARRAY<IRenderComponent*>& outaRenderCache, DYNAMIC_ARRAY<CCameraComponent*>& outaCameraCache,
                                              DYNAMIC_ARRAY<CDirectionalLightComponent*>& outaDirectionalLightComponentCache, DYNAMIC_ARRAY<CPointLightComponent*>& outaPointLightComponentCache, CAmbientLightComponent*& outpAmbientLight) const;
            //----------------------------------------------------------
            /// Cull Renderables
            ///
            /// @param Camera to cull against
            /// @param Renderables to cull
            /// @param [Out]: Visible renderables
            //----------------------------------------------------------
            void CullRenderables(CCameraComponent* inpCamera, const DYNAMIC_ARRAY<IRenderComponent*>& inaRenderCache, DYNAMIC_ARRAY<IRenderComponent*>& outaRenderCache) const;
            //----------------------------------------------------------
            /// Cull Renderables
            ///
            /// @param Light to cull against
            /// @param Renderables to cull
            /// @param [Out]: Visible renderables
            //----------------------------------------------------------
            void CullRenderables(CPointLightComponent* inpLightComponent, const DYNAMIC_ARRAY<IRenderComponent*>& inaRenderCache, DYNAMIC_ARRAY<IRenderComponent*>& outaRenderCache) const;
			//----------------------------------------------------------
			/// Filter Scene Renderables
			///
			/// Sort found renderable objects into opaque and transparent
            ///
            /// @param List of renderable objects
            /// @param Out: Opaque renderables
            /// @param Out: Transparent renderables
			//----------------------------------------------------------
			void FilterSceneRenderables(const DYNAMIC_ARRAY<IRenderComponent*>& inaRenderables, DYNAMIC_ARRAY<IRenderComponent*>& outaOpaque, DYNAMIC_ARRAY<IRenderComponent*>& outaTransparent) const;
            //----------------------------------------------------------
			/// Filter Shadow Map Renderables
			///
			/// Filter the renderables based on whether they cast
            /// shadows
            ///
            /// @param List of renderable objects
            /// @param Out: renderables
			//----------------------------------------------------------
			void FilterShadowMapRenderables(const DYNAMIC_ARRAY<IRenderComponent*>& inaRenderables, DYNAMIC_ARRAY<IRenderComponent*>& outaRenderables) const;
            //----------------------------------------------------------
            /// Get Cull Predicate
            ///
            /// @param Camera
            /// @return Camera cull predicate or nullptr
            //----------------------------------------------------------
            CullingPredicatePtr GetCullPredicate(CCameraComponent* inpActiveCamera) const;
            //----------------------------------------------------------
			/// Create Overlay Projection
            ///
            /// This function generates a projection matrix for overlay
            /// rendering
            /// @return Projection matrix for overlay rendering
            //----------------------------------------------------------
            Core::CMatrix4x4 CreateOverlayProjection(GUI::CWindow* inpWindow) const;
            //----------------------------------------------------------
            /// Sort Opaque
            ///
            /// Sort the opaque renderables by the current sort predicate
            /// on the camera
            ///
            /// @param Camera component
            /// @param Renderables
            //----------------------------------------------------------
            void SortOpaque(CCameraComponent* inpCameraComponent, DYNAMIC_ARRAY<IRenderComponent*>& inaRenderables) const;
            //----------------------------------------------------------
            /// Sort Transparent
            ///
            /// Sort the transparent renderables by the current sort predicate
            /// on the camera
            ///
            /// @param Camera component
            /// @param Renderables
            //----------------------------------------------------------
            void SortTransparent(CCameraComponent* inpCameraComponent, DYNAMIC_ARRAY<IRenderComponent*>& inaRenderables) const;
            
        private:
            
            CCanvasRenderer mCanvas;
            
			IRenderSystem* mpRenderSystem;
			CCameraComponent* mpActiveCamera;
            
			RendererSortPredicatePtr mpTransparentSortPredicate;
            RendererSortPredicatePtr mpOpaqueSortPredicate;
            
            CullingPredicatePtr mpPerspectiveCullPredicate;
            CullingPredicatePtr mpOrthoCullPredicate;
		};
	}
}

#endif