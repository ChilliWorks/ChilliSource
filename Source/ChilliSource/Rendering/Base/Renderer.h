//
//  Renderer.h
//  Chilli Source
//  Created by Scott Downie on 30/09/2010.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2010 Tag Games Limited
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

#ifndef _CHILLISOURCE_RENDERING_BASE_RENDERER_H_
#define _CHILLISOURCE_RENDERING_BASE_RENDERER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/System/AppSystem.h>
#include <ChilliSource/Rendering/Base/CanvasRenderer.h>

namespace ChilliSource
{
	namespace Rendering
	{
		//==================================================
		/// Description:
		///
		/// Does a pre-pass on the scene to convert Chilli Source
		/// components to plain data for the render system
		//==================================================
		class Renderer final : public Core::AppSystem
		{
		public:
            CS_DECLARE_NAMEDTYPE(Renderer);
            //----------------------------------------------------------
            /// Allows querying of whether or not this system implements
            /// a given interface.
            ///
			/// @author Ian Copland
			///
			/// @param Interface Id
			///
			/// @return Whether this object is of the given type.
			//----------------------------------------------------------
			bool IsA(Core::InterfaceIDType in_interfaceId) const override;
			//----------------------------------------------------------
			/// Set Transparent Sort Predicate
			///
			/// Sets the sort predicate to use for sorting transparent objects
			/// @param Our RendererSortPredicate functor. If this is nullptr the renderer will not sort transparent objects
			//----------------------------------------------------------
			void SetTransparentSortPredicate(const RendererSortPredicateSPtr & inpFunctor);
            //----------------------------------------------------------
			/// Set Opaque Sort Predicate
			///
			/// Sets the sort predicate to use for sorting opaque objects
			/// @param Our RendererSortPredicate functor. If this is nullptr the renderer will not sort opaque objects
			//----------------------------------------------------------
			void SetOpaqueSortPredicate(const RendererSortPredicateSPtr & inpFunctor);
            //----------------------------------------------------------
			/// Set Perspective Cull Predicate
			///
            /// @param Predicate that is called to determine whether
            /// an object should be culled with an perspective camera
			//----------------------------------------------------------
			void SetPerspectiveCullPredicate(const ICullingPredicateSPtr & inpFunctor);
            //----------------------------------------------------------
			/// Set Ortho Cull Predicate
			///
            /// @param Predicate that is called to determine whether
            /// an object should be culled with an orthographic camera
			//----------------------------------------------------------
			void SetOrthoCullPredicate(const ICullingPredicateSPtr & inpFunctor);
            //----------------------------------------------------------
			/// Render To Screen
			///
			/// Render the scene via active the render system to the
			/// frame buffer. This function is called by the system
			///
			/// @param Scene to render
            /// @param Canvas to render
			//----------------------------------------------------------
			void RenderToScreen(Core::Scene* inpScene, UI::Canvas* in_canvas);
			//----------------------------------------------------------
			/// Render To Texture
			///
			/// Render the scene to texture via active the render system
            /// If no colour texture is provided only the depth will
            /// be rendered. If no depth target is provided a depth
            /// buffer will be created.
            ///
			/// @param Scene to render
            /// @param Canvas to render. Can be null.
			/// @param Texture to render colour to.
            /// @param Texture to render depth to.
			//----------------------------------------------------------
			void RenderToTexture(Core::Scene* inpScene, UI::Canvas* in_canvas, const TextureSPtr &inpColourTarget, const TextureSPtr& inpDepthTarget = TextureSPtr());
			//----------------------------------------------------------
			/// Get Active Camera Pointer
			///
			/// @return A weak pointer to the active scene camera
			//----------------------------------------------------------
			CameraComponent* GetActiveCameraPtr();

            static Core::Matrix4 matViewProjCache;

		private:
            friend class Core::Application;
            //-------------------------------------------------------
            /// Factory method
            ///
            /// @author S Downie
            ///
            /// @param Render system
            ///
            /// @return Ownership of new instance
            //-------------------------------------------------------
            static RendererUPtr Create(RenderSystem* in_renderSystem);
            //-------------------------------------------------------
            /// Private constructor to force use of factory method
            ///
            /// @author S Downie
            ///
            /// @param Render system
            //-------------------------------------------------------
			Renderer(RenderSystem* in_renderSystem);
            //----------------------------------------------------------
            /// Initialisation method called at a time when
            /// all App Systems have been created. System
            /// initialisation occurs in the order they were
            /// created.
            ///
            /// @author S Downie
            //----------------------------------------------------------
            void OnInit() override;
            //----------------------------------------------------------
            /// Render Shadow Map
            ///
            /// Generate a shadow map from the current scene
            ///
            /// @param Camera component
            /// @param Light components
            /// @param Render components
            //----------------------------------------------------------
            void RenderShadowMap(CameraComponent* inpCameraComponent, std::vector<DirectionalLightComponent*>& inaLightComponents, std::vector<RenderComponent*>& inaRenderables);
            //----------------------------------------------------------
            /// Render Shadow Map
            ///
            /// Generate a shadow map from the current scene
            ///
            /// @param Camera component
            /// @param Light component
            /// @param Render components
            //----------------------------------------------------------
            void RenderShadowMap(CameraComponent* inpCameraComponent, DirectionalLightComponent* inpLightComponent, std::vector<RenderComponent*>& inaRenderables);
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
			void Render(CameraComponent* inpCameraComponent, ShaderPass ineShaderPass, std::vector<RenderComponent*>& inaRenderables);
            //----------------------------------------------------------
            /// Render UI
            ///
            /// @param Canvas
            /// @param Screen clear colour
            //----------------------------------------------------------
            void RenderUI(UI::Canvas* in_canvas, const Core::Colour& in_clearColour);
            //----------------------------------------------------------
            /// Render Scene To Target
            ///
            /// Render the contents of the scene to the target buffer
            ///
            /// @param Scene
            /// @param Canvas to render
            /// @param Target
            //----------------------------------------------------------
            void RenderSceneToTarget(Core::Scene* inpScene, UI::Canvas* in_canvas, RenderTarget* inpRenderTarget);
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
			void FindRenderableObjectsInScene(Core::Scene* pScene, std::vector<RenderComponent*>& outaRenderCache, std::vector<CameraComponent*>& outaCameraCache,
                                              std::vector<DirectionalLightComponent*>& outaDirectionalLightComponentCache, std::vector<PointLightComponent*>& outaPointLightComponentCache, AmbientLightComponent*& outpAmbientLight) const;
            //----------------------------------------------------------
            /// Cull Renderables
            ///
            /// @param Camera to cull against
            /// @param Renderables to cull
            /// @param [Out]: Visible renderables
            //----------------------------------------------------------
            void CullRenderables(CameraComponent* inpCamera, const std::vector<RenderComponent*>& inaRenderCache, std::vector<RenderComponent*>& outaRenderCache) const;
            //----------------------------------------------------------
            /// Cull Renderables
            ///
            /// @param Light to cull against
            /// @param Renderables to cull
            /// @param [Out]: Visible renderables
            //----------------------------------------------------------
            void CullRenderables(PointLightComponent* inpLightComponent, const std::vector<RenderComponent*>& inaRenderCache, std::vector<RenderComponent*>& outaRenderCache) const;
			//----------------------------------------------------------
			/// Filter Scene Renderables
			///
			/// Sort found renderable objects into opaque and transparent
            ///
            /// @param List of renderable objects
            /// @param Out: Opaque renderables
            /// @param Out: Transparent renderables
			//----------------------------------------------------------
			void FilterSceneRenderables(const std::vector<RenderComponent*>& inaRenderables, std::vector<RenderComponent*>& outaOpaque, std::vector<RenderComponent*>& outaTransparent) const;
            //----------------------------------------------------------
			/// Filter Shadow Map Renderables
			///
			/// Filter the renderables based on whether they cast
            /// shadows
            ///
            /// @param List of renderable objects
            /// @param Out: renderables
			//----------------------------------------------------------
			void FilterShadowMapRenderables(const std::vector<RenderComponent*>& inaRenderables, std::vector<RenderComponent*>& outaRenderables) const;
            //----------------------------------------------------------
            /// Get Cull Predicate
            ///
            /// @param Camera
            /// @return Camera cull predicate or nullptr
            //----------------------------------------------------------
            ICullingPredicateSPtr GetCullPredicate(CameraComponent* inpActiveCamera) const;
            //----------------------------------------------------------
			/// Create Overlay Projection
            ///
            /// This function generates a projection matrix for overlay
            /// rendering
            /// @return Projection matrix for overlay rendering
            //----------------------------------------------------------
            Core::Matrix4 CreateOverlayProjection(const Core::Vector2& in_size) const;

            //----------------------------------------------------------
            /// Sort Opaque
            ///
            /// Sort the opaque renderables by the current sort predicate
            /// on the camera
            ///
            /// @param Camera component
            /// @param Renderables
            //----------------------------------------------------------
            void SortOpaque(CameraComponent* inpCameraComponent, std::vector<RenderComponent*>& inaRenderables) const;
            //----------------------------------------------------------
            /// Sort Transparent
            ///
            /// Sort the transparent renderables by the current sort predicate
            /// on the camera
            ///
            /// @param Camera component
            /// @param Renderables
            //----------------------------------------------------------
            void SortTransparent(CameraComponent* inpCameraComponent, std::vector<RenderComponent*>& inaRenderables) const;
            //------------------------------------------------
            /// Called when the application is being destroyed.
            /// This should be used to cleanup memory and
            /// references to other systems. System destruction
            /// occurs in the reverse order to which they
            /// were created
            ///
            /// @author Ian Copland
            //------------------------------------------------
            void OnDestroy() override;
        private:

            CanvasRenderer* m_canvas = nullptr;

			RenderSystem* mpRenderSystem;
			CameraComponent* mpActiveCamera;

			RendererSortPredicateSPtr mpTransparentSortPredicate;
            RendererSortPredicateSPtr mpOpaqueSortPredicate;

            ICullingPredicateSPtr mpPerspectiveCullPredicate;
            ICullingPredicateSPtr mpOrthoCullPredicate;

            MaterialCSPtr m_staticDirShadowMaterial;
            MaterialCSPtr m_animDirShadowMaterial;
		};
	}
}

#endif
