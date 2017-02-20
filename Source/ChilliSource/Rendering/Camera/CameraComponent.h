//
//  CameraComponent.h
//  ChilliSource
//  Created by Scott Downie on 12/10/2010.
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

#ifndef _CHILLISOURCE_RENDERING_CAMERA_CAMERACOMPONENT_H_
#define _CHILLISOURCE_RENDERING_CAMERA_CAMERACOMPONENT_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Entity/Component.h>
#include <ChilliSource/Core/Event/EventConnection.h>
#include <ChilliSource/Core/Math/Geometry/Shapes.h>

namespace ChilliSource
{
    //------------------------------------------------------------------------------
    /// Base class for camera components. The camera is added to the scene and
    /// provides the renderer with the view and projection matrices required to draw
    /// the scene correctly
    ///
    /// @author S Downie
    //------------------------------------------------------------------------------
    class CameraComponent : public Component
    {
    public:
        CS_DECLARE_NAMEDTYPE(CameraComponent);
        //------------------------------------------------------------------------------
        /// Governs how the viewport updates with screen resize
        ///
        /// @author S Downie
        //------------------------------------------------------------------------------
        enum class ViewportResizePolicy
        {
            k_none,
            k_scaleWithScreen
        };
        //------------------------------------------------------------------------------
        /// Constructor
        ///
        /// @author S Downie
        ///
        /// @param Near plane
        /// @param Far plane
        //------------------------------------------------------------------------------
        CameraComponent(f32 in_nearClip, f32 in_farClip);
        //------------------------------------------------------------------------------
        /// Virtual Destructor
        ///
        /// @author S Downie
        //------------------------------------------------------------------------------
        virtual ~CameraComponent(){}
        //------------------------------------------------------------------------------
        /// @author S Downie
        ///
        /// @param Near Z clipping distance in view space
        //------------------------------------------------------------------------------
        void SetNearClipping(f32 in_near);
        //------------------------------------------------------------------------------
        /// @author S Downie
        ///
        /// @param Far Z clipping distance in view space
        //------------------------------------------------------------------------------
        void SetFarClipping(f32 in_far);
        //------------------------------------------------------------------------------
        /// @author S Downie
        ///
        /// @return Projection matrix
        //------------------------------------------------------------------------------
        const Matrix4& GetProjection() const;
        //------------------------------------------------------------------------------
        /// @author S Downie
        ///
        /// @return View matrix
        //------------------------------------------------------------------------------
        const Matrix4& GetView();
        //------------------------------------------------------------------------------
        /// Unproject from a point in screen space to a ray in world space
        ///
        /// @param Point in screen space
        ///
        /// @return Ray in world space with camera view direction
        //------------------------------------------------------------------------------
        Ray Unproject(const Vector2& in_screenPos);
        //------------------------------------------------------------------------------
        /// Convert from a point in world space to a point in screen space
        ///
        /// @param World space pos
        ///
        /// @return Screen space pos
        //------------------------------------------------------------------------------
        Vector2 Project(const Vector3& in_worldPos);
        //------------------------------------------------------------------------------
        /// @author S Downie
        ///
        /// @return Camera frustum
        //------------------------------------------------------------------------------
        const Frustum& GetFrustum();
        //------------------------------------------------------------------------------
        /// Orientate the given matrix to face the cameras view vector
        ///
        /// @author S Downie
        ///
        /// @param Matrix to billboard
        ///
        /// @return Billboarded matrix
        //------------------------------------------------------------------------------
        Matrix4 Billboard(const Matrix4& in_toBillboard);
        
    private:
        //------------------------------------------------------------------------------
        /// Delegates the calculation of the projection matrix to the concrete camera
        /// types.
        ///
        /// @author S Downie
        ///
        /// @return Projection matrix
        //------------------------------------------------------------------------------
        virtual Matrix4 CalculateProjectionMatrix() = 0;
        //------------------------------------------------------------------------------
        /// @author S Downie
        ///
        /// Recalculate frustum planes
        //------------------------------------------------------------------------------
        virtual void UpdateFrustum() = 0;
        //------------------------------------------------------------------------------
        /// Called when the component is added to an entity.
        ///
        /// @author S Downie
        //------------------------------------------------------------------------------
        void OnAddedToEntity() override;
        //------------------------------------------------------------------------------
        /// Called when the component is added to the scene.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        void OnAddedToScene() override;
        //------------------------------------------------------------------------------
        /// Called when the component is added to the scene.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        void OnRemovedFromScene() override;
        //------------------------------------------------------------------------------
        /// Called when the component is removed from an entity.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        void OnRemovedFromEntity() override;
        
    protected:
        
        Frustum m_frustum;
        f32 m_nearClip;
        f32 m_farClip;
        bool m_isFrustumCacheValid = false;
        Screen* m_screen = nullptr;
        Matrix4 m_projMat;
        Matrix4 m_viewMat;

    private:
        
        EventConnectionUPtr m_transformChangedConnection;
        EventConnectionUPtr m_resolutionChangedConnection;
    };
}

#endif
