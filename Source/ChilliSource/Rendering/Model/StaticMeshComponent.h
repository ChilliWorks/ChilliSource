//
//  StaticMeshComponent.h
//  Chilli Source
//  Created by Scott Downie on 07/10/2010.
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

#ifndef _CHILLISOURCE_RENDERING_STATIC_MESH_COMPONENT_H_
#define _CHILLISOURCE_RENDERING_STATIC_MESH_COMPONENT_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Volume/VolumeComponent.h>
#include <ChilliSource/Rendering/Model/Model.h>

namespace ChilliSource
{
    //===============================================================
    /// Description:
    ///
    /// A static mesh component. This defines a 3D mesh that can
    /// be manipulated, textured but not animated.
    //===============================================================
    class StaticMeshComponent : public VolumeComponent
    {
    public:
        CS_DECLARE_NAMEDTYPE(StaticMeshComponent);
        
        StaticMeshComponent();
        //----------------------------------------------------------
        /// Is A
        ///
        /// Returns if it is of the type given
        /// @param Comparison Type
        /// @return Whether the class matches the comparison type
        //----------------------------------------------------------
        bool IsA(InterfaceIDType inInterfaceID) const override;
        //----------------------------------------------------
        /// Get Axis Aligned Bounding Box
        ///
        /// All render components have a box used for culling
        /// and coarse intersections. This is cached and 
        /// recomputed when required.
        /// @return Axis aligned bounding box
        //----------------------------------------------------
        const AABB& GetAABB() override;
        //----------------------------------------------------
        /// Get Object Oriented Bounding Box
        ///
        /// All render objects have an OOBB for
        /// picking. This is cached and 
        /// recomputed when required.
        /// @return OOBB
        //----------------------------------------------------
        const OOBB& GetOOBB() override;
        //----------------------------------------------------
        /// Get Bounding Sphere
        ///
        /// All render objects have an bounding sphere for
        /// culling. This is cached and 
        /// recomputed when required.
        /// @return bounding sphere
        //----------------------------------------------------
        const Sphere& GetBoundingSphere() override;
        //----------------------------------------------------
        /// Is Visible
        ///
        /// @return Whether or not to render
        //----------------------------------------------------
        bool IsVisible() const override { return m_isVisible; }
        //----------------------------------------------------
        /// Is Visible
        ///
        /// @param in_isVisible - Whether or not to render
        //----------------------------------------------------
        void SetVisible(bool in_isVisible) { m_isVisible = in_isVisible; }
        //-----------------------------------------------------------
        /// Set Material
        ///
        /// Set the material that the mesh will use. Applies the material
        /// To all submeshes
        ///
        /// @param Handle to material
        //-----------------------------------------------------------
        void SetMaterial(const MaterialCSPtr& inpMaterial);
        //-----------------------------------------------------------
        /// Set Material For Sub Model
        ///
        /// Set the material that one sub mesh will use.
        ///
        /// @param Handle to material
        /// @Param Index to the submesh
        //-----------------------------------------------------------
        void SetMaterialForSubMesh(const MaterialCSPtr& inpMaterial, u32 indwSubMeshIndex);
        //-----------------------------------------------------------
        /// Set Material For Sub Model
        ///
        /// Set the material that one sub mesh will use.
        ///
        /// @param Handle to material
        /// @param The name of the submesh.
        //-----------------------------------------------------------
        void SetMaterialForSubMesh(const MaterialCSPtr& inpMaterial, const std::string& instrSubMeshName);
        //-----------------------------------------------------------
        /// Get Material Of Sub Model
        ///
        /// Get the material of a single sub mesh.
        ///
        /// @param Index to the sub mesh
        /// @return Handle to material
        //-----------------------------------------------------------
        MaterialCSPtr GetMaterialOfSubMesh(u32 indwSubMeshIndex) const;
        //-----------------------------------------------------------
        /// Get Material Of Sub Model
        ///
        /// Get the material of a single sub mesh.
        ///
        /// @param The name of the submesh.
        /// @return Handle to material
        //-----------------------------------------------------------
        MaterialCSPtr GetMaterialOfSubMesh(const std::string& instrSubMeshName) const;
        //----------------------------------------------------------
        /// Attach Model
        ///
        /// Attach a mesh to this component
        /// @param Model object
        //----------------------------------------------------------
        void AttachMesh(const ModelCSPtr& inpModel);
        //----------------------------------------------------------
        /// Attach Model
        ///
        /// Attach a mesh to this component but uses the given 
        /// material
        /// @param Model object
        //----------------------------------------------------------
        void AttachMesh(const ModelCSPtr& inpModel, const MaterialCSPtr& inpMaterial);
        //----------------------------------------------------------
        /// Get Model
        ///
        /// @return The components internal mesh
        //----------------------------------------------------------
        const ModelCSPtr& GetMesh() const;
        //-----------------------------------------------------
        /// Set Shadow Casting Enabled
        ///
        /// @param Whether the render component casts shadows
        //-----------------------------------------------------
        void SetShadowCastingEnabled(bool inbEnabled);
        //-----------------------------------------------------
        /// Is Shadow Casting Enabled
        ///
        /// @return Whether the render component casts shadows
        //-----------------------------------------------------
        bool IsShadowCastingEnabled() const;
        
    private:
        //----------------------------------------------------
        /// Triggered when the component is attached to
        /// an entity on the scene
        ///
        /// @author S Downie
        //----------------------------------------------------
        void OnAddedToScene() override;
        //----------------------------------------------------
        /// On Entity Transform Changed
        ///
        /// Delegate called when the owning entities transform
        /// changes. This is used to dirty the bounding volumes
        //----------------------------------------------------
        void OnEntityTransformChanged();
        //----------------------------------------------------
        /// Triggered when the component is removed from
        /// an entity on the scene
        ///
        /// @author S Downie
        //----------------------------------------------------
        void OnRemovedFromScene() override;
        
    private:
        
        ModelCSPtr mpModel;
        std::vector<MaterialCSPtr> mMaterials;
        
        EventConnectionUPtr m_transformChangedConnection;
        
        bool m_isBSValid;
        bool m_isAABBValid;
        bool m_isOOBBValid;
        
        AABB mBoundingBox;
        OOBB mOBBoundingBox;
        Sphere mBoundingSphere;
        bool m_shadowCastingEnabled = true;
        bool m_isVisible = true;
    };
}

#endif
