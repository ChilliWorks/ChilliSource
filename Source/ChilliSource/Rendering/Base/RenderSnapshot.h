//
//  The MIT License (MIT)
//
//  Copyright (c) 2016 Tag Games Limited
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

#ifndef _CHILLISOURCE_RENDERING_BASE_RENDERSNAPSHOT_H_
#define _CHILLISOURCE_RENDERING_BASE_RENDERSNAPSHOT_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/Colour.h>
#include <ChilliSource/Core/Math/Vector2.h>
#include <ChilliSource/Core/Memory/PagedLinearAllocator.h>
#include <ChilliSource/Rendering/Base/RenderFrameData.h>
#include <ChilliSource/Rendering/Base/RenderObject.h>
#include <ChilliSource/Rendering/Camera/RenderCamera.h>
#include <ChilliSource/Rendering/Lighting/AmbientRenderLight.h>
#include <ChilliSource/Rendering/Lighting/DirectionalRenderLight.h>
#include <ChilliSource/Rendering/Lighting/PointRenderLight.h>
#include <ChilliSource/Rendering/RenderCommand/RenderCommandList.h>

#include <vector>

namespace ChilliSource
{
    /// A snapshot of the state of the scene, containing only information which the renderer
    /// requires. The render snapshot is passed to all active systems and all components in
    /// the scene allowing them to snapshot their state.
    ///
    /// Note that snapshotted data is moved rather than copied from the snapshot for the sake
    /// of performance, meaning that each peice of data can only be claimed once.
    ///
    /// This is not thread-safe and must only be accessed by one thread at a time.
    ///
    class RenderSnapshot final
    {
    public:
        CS_DECLARE_NOCOPY(RenderSnapshot);
        
        RenderSnapshot(RenderSnapshot&&) = default;
        RenderSnapshot& operator=(RenderSnapshot&&) = default;
        
        /// Creates a new instance with the given viewport resolution and clear colour.
        ///
        /// @param renderTarget
        ///     The render target to render into, if null renders to screen (frame buffer)
        /// @param resolution
        ///     The viewport resolution.
        /// @param clearColour
        ///     The clear colour
        /// @param renderCamera
        ///     The main camera that will be used to render the scene. Currently only one camera per
        ///     scene is supported.
        ///
        RenderSnapshot(const RenderTargetGroup* renderTarget, const Integer2& resolution, const Colour& clearColour, const RenderCamera& renderCamera) noexcept;
        
        /// @return The viewport resolution.
        ///
        const Integer2& GetResolution() const noexcept { return m_resolution; }
        
        /// @return The clear colour that should be used during rendering.
        ///
        const Colour& GetClearColour() const noexcept { return m_clearColour; }
        
        /// @return  The main camera that will be used to render the scene.
        ///
        RenderCamera GetRenderCamera() noexcept { return m_renderCamera; }
        
        /// Adds an ambient light to the render snapshot.
        ///
        /// @param renderAmbientLight
        ///     The ambient light which should be added.
        ///
        void AddAmbientRenderLight(const AmbientRenderLight& renderAmbientLight) noexcept;
        
        /// Adds a directional light to the render snapshot.
        ///
        /// @param renderDirectionalLight
        ///     The directional light which should be added.
        ///
        void AddDirectionalRenderLight(const DirectionalRenderLight& renderDirectionalLight) noexcept;
        
        /// Adds a point light to the render snapshot.
        ///
        /// @param renderPointLight
        ///     The point light which should be added.
        ///
        void AddPointRenderLight(const PointRenderLight& renderPointLight) noexcept;
        
        /// Adds an object to the render snapshot.
        ///
        /// @param renderObject
        ///     The object which should be added.
        ///
        void AddRenderObject(const RenderObject& renderObject) noexcept;
        
        /// Adds a RenderDynamicMesh to the snapshot. This will be deleted at the end of the frame.
        ///
        /// @param The render dynamic mesh.
        ///
        void AddRenderDynamicMesh(RenderDynamicMeshAUPtr renderDynamicMesh) noexcept;
        
        /// Adds a RenderSkinnedAnimation to the snapshot. This will be deleted at the end of the frame.
        ///
        /// @param The render skinned animation.
        ///
        void AddRenderSkinnedAnimation(RenderSkinnedAnimationAUPtr renderSkinnedAnimation) noexcept;
        
        /// @return A modifiable version of the pre render command list. This can be used to populate
        ///     The list with additional commands.
        ///
        RenderCommandList* GetPreRenderCommandList() noexcept;
        
        /// @return A modifiable version of the post render command list. This can be used to
        ///     populate the list with additional commands.
        ///
        RenderCommandList* GetPostRenderCommandList() noexcept;
        
        /// Moves the list of ambient lights to a new external owner.
        ///
        /// @return The moved list of ambient lights.
        ///
        std::vector<AmbientRenderLight> ClaimAmbientRenderLights() noexcept;
        
        /// Moves the list of directional lights to a new external owner.
        ///
        /// @return The moved list of directional lights.
        ///
        std::vector<DirectionalRenderLight> ClaimDirectionalRenderLights() noexcept;
        
        /// Moves the list of point lights to a new external owner.
        ///
        /// @return The moved list of point lights.
        ///
        std::vector<PointRenderLight> ClaimPointRenderLights() noexcept;
        
        /// Moves the list of objects to a new external owner.
        ///
        /// @return The moved list of objects.
        ///
        std::vector<RenderObject> ClaimRenderObjects() noexcept;
        
        /// Moves the pre render command list to a new external owner.
        ///
        /// @return The moved pre render command list.
        ///
        RenderCommandListUPtr ClaimPreRenderCommandList() noexcept;
    
        /// Moves the post render command list to a new external owner.
        ///
        /// @return The moved post render command list.
        ///
        RenderCommandListUPtr ClaimPostRenderCommandList() noexcept;
        
        /// Moves the render frame data to a new external owner.
        ///
        /// @return The moved container for data which needs to persist to the end of the frame.
        ///
        RenderFrameData ClaimRenderFrameData() noexcept;
        
        ///@return Render target for this frame, if null defaults to the frame buffer
        ///
        const RenderTargetGroup* GetOffscreenRenderTarget() const noexcept { return m_offscreenRenderTarget; }
        
    private:
        Integer2 m_resolution;
        Colour m_clearColour;
        RenderCamera m_renderCamera;
        std::vector<AmbientRenderLight> m_renderAmbientLights;
        std::vector<DirectionalRenderLight> m_renderDirectionalLights;
        std::vector<PointRenderLight> m_renderPointLights;
        std::vector<RenderObject> m_renderObjects;
        RenderCommandListUPtr m_preRenderCommandList;
        RenderCommandListUPtr m_postRenderCommandList;
        RenderFrameData m_renderFrameData;
        const RenderTargetGroup* m_offscreenRenderTarget;
        
        bool m_renderCameraClaimed = false;
        bool m_renderAmbientLightsClaimed = false;
        bool m_renderDirectionalLightsClaimed = false;
        bool m_renderPointLightsClaimed = false;
        bool m_renderObjectsClaimed = false;
        bool m_renderFrameDataClaimed = false;
    };
};

#endif
