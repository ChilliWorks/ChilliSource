//
//  SpriteComponent.h
//  ChilliSource
//  Created by Scott Downie on 29/09/2010.
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

#ifndef _CHILLISOURCE_RENDERING_SPRITE_SPRITECOMPONENT_H_
#define _CHILLISOURCE_RENDERING_SPRITE_SPRITECOMPONENT_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/Colour.h>
#include <ChilliSource/Core/Math/Vector2.h>
#include <ChilliSource/Core/Math/Geometry/Shapes.h>
#include <ChilliSource/Core/Volume/VolumeComponent.h>
#include <ChilliSource/Rendering/Base/AlignmentAnchors.h>
#include <ChilliSource/Rendering/Texture/UVs.h>

#include <functional>

namespace ChilliSource
{
    //---------------------------------------------------------------
    /// A render component that draws a 2D textured quad. Sprites
    /// are rendered using a "Sprite" material and can be coloured,
    /// aligned and have thier UVs changed manually or via a texture
    /// atlas.
    ///
    /// @author S Downie
    //---------------------------------------------------------------
    class SpriteComponent final : public VolumeComponent
    {
    public: 
        
        CS_DECLARE_NAMEDTYPE(SpriteComponent);
        
        //----------------------------------------------------------------------------------------
        /// Delegate for size policy functions.
        ///
        /// @author S Downie
        ///
        /// @param Original size
        /// @param Preferred size
        ///
        /// @return New size with function applied
        //----------------------------------------------------------------------------------------
        using SizePolicyDelegate = std::function<Vector2(const Vector2&, const Vector2&)>;

        ///
        SpriteComponent();
        
        /// Creates a sprite with the given material and size
        ///
        /// @param material
        ///     Material the describes how the sprite should be rendered
        /// @param size
        ///    Size of the sprite in world units
        /// @param sizePolicy
        ///     How the sprite should maintain its aspect ratio when resized
        ///
        SpriteComponent(const MaterialCSPtr& material, const Vector2& size, SizePolicy sizePolicy);
        
        /// Creates a sprite with the given material and size that references
        /// a sprite on the given texture atlas.
        ///
        /// @param material
        ///     Material the describes how the sprite should be rendered
        /// @param atlas
        ///     Resource that describes where all the sprites are on a texture
        /// @param atlasId
        ///     Which of the sprites on the texture should this component use
        /// @param size
        ///    Size of the sprite in world units
        /// @param sizePolicy
        ///     How the sprite should maintain its aspect ratio when resized
        ///
        SpriteComponent(const MaterialCSPtr& material, const TextureAtlasCSPtr& atlas, const std::string& atlasId, const Vector2& size, SizePolicy sizePolicy);
        
        //----------------------------------------------------------
        /// @author S Downie
        ///
        /// @param Comparison Type
        ///
        /// @return Whether the class matches the comparison type
        //----------------------------------------------------------
        bool IsA(InterfaceIDType in_interfaceId) const override;
        //----------------------------------------------------
        /// The axis aligned bounding box is positioned in
        /// world space but the orientation is aligned to
        /// the x-y plane. This is cached and recomputed when
        /// tranform is changed.
        ///
        /// @author S Downie
        ///
        /// @return AABB in world space
        //----------------------------------------------------
        const AABB& GetAABB() override;
        //----------------------------------------------------
        /// The object oriented bounding box complete with transform to
        /// transform from local space to world space. This is cached
        /// and recomputed when tranform is changed.
        ///
        /// @author S Downie
        ///
        /// @return OOBB in local space with world transform
        //----------------------------------------------------
        const OOBB& GetOOBB() override;
        //----------------------------------------------------
        /// All render objects have an bounding sphere for
        /// culling. This is cached and recomputed when tranform
        /// is changed. The bounding sphere encompasses
        /// the diagonal axis of the sprite
        ///
        /// @author S Downie
        ///
        /// @return world space bounding sphere
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
        /// @author S Downie
        ///
        /// @param Vector containing width and height of sprite in
        /// local space
        //-----------------------------------------------------------
        void SetSize(const Vector2& in_size);
        //-----------------------------------------------------------
        /// @author S Downie
        ///
        /// @param Width in local space
        /// @param Height in local space
        //-----------------------------------------------------------
        void SetSize(f32 in_width, f32 in_height);
        //-----------------------------------------------------------
        /// @author S Downie
        ///
        /// @param Size policy that governs how the set size and
        /// the image size are accommodated. Usually by maintaining
        /// the aspect ratio of the image.
        //-----------------------------------------------------------
        void SetSizePolicy(SizePolicy in_sizePolicy);
        //-----------------------------------------------------------
        /// @author S Downie
        ///
        /// @return Size after the size policy has been applied
        //-----------------------------------------------------------
        Vector2 GetSize() const;
        //-----------------------------------------------------------
        /// Sets the material
        ///
        /// @author Ian Copland
        ///
        /// @param in_material - The material
        //-----------------------------------------------------------
        void SetMaterial(const MaterialCSPtr& in_material);
        //-----------------------------------------------------------
        /// @author Ian Copland
        ///
        /// @return The material
        //-----------------------------------------------------------
        const MaterialCSPtr& GetMaterial() const;
        //-----------------------------------------------------------
        /// @author S Downie
        ///
        /// @param Texture atlas
        //-----------------------------------------------------------
        void SetTextureAtlas(const TextureAtlasCSPtr& in_atlas);
        //-----------------------------------------------------------
        /// Set the id of the frame that will be used to texture
        /// the sprite. This will supercede and UVs that have been
        /// set on the sprite
        ///
        /// @author S Downie
        ///
        /// @param Texture atlas id
        //-----------------------------------------------------------
        void SetTextureAtlasId(const std::string& in_atlasId);
        //-----------------------------------------------------------
        /// Set the texture co-ordinates
        ///
        /// @author S Downie
        ///
        /// @param Rect containing uv, st
        //-----------------------------------------------------------
        void SetUVs(const UVs& in_uvs);
        //-----------------------------------------------------------
        /// Set the UV texture coordinates
        ///
        /// @author S Downie
        ///
        /// @param U
        /// @param V
        /// @param S
        /// @param T
        //-----------------------------------------------------------
        void SetUVs(f32 in_u, f32 in_v, f32 in_s, f32 in_t);
        //-----------------------------------------------------------
        /// Set the RGBA colour of the sprite. This is independent
        /// of the material colour and is applied on a per sprite
        /// basis
        ///
        /// @author S Downie
        ///
        /// @param Colour containing RGBA
        //-----------------------------------------------------------
        void SetColour(const Colour& in_colour);
        //-----------------------------------------------------------
        /// Set the RGBA colour of the sprite. This is independent
        /// of the material colour and is applied on a per sprite
        /// basis
        ///
        /// @author S Downie
        ///
        /// @param Red
        /// @param Green
        /// @param Blue
        /// @param Alpha
        //-----------------------------------------------------------
        void SetColour(f32 in_r, f32 in_g, f32 in_b, f32 in_a);
        //-----------------------------------------------------------
        /// Get Colour
        ///
        /// @return Sprite colour
        //-----------------------------------------------------------
        const Colour& GetColour() const;
        //-----------------------------------------------------------
        /// Flip the sprite UVs about it's local x-axis
        ///
        /// @author S Downie
        ///
        /// @param Whether to flip or not
        //-----------------------------------------------------------
        void SetFlippedHorizontally(bool in_flip);
        //-----------------------------------------------------------
        /// @author S Downie
        ///
        /// @return Whether sprite is flipped about it's local x-axis
        //-----------------------------------------------------------
        bool IsFlippedHorizontally() const;
        //-----------------------------------------------------------
        /// Flip the sprite UVs about it's local y-axis
        ///
        /// @author S Downie
        ///
        /// @param Whether to flip or not
        //-----------------------------------------------------------
        void SetFlippedVertically(bool in_flip);
        //-----------------------------------------------------------
        /// @author S Downie
        ///
        /// @return Whether sprite is flipped about it's local y-axis
        //-----------------------------------------------------------
        bool IsFlippedVertically() const;
        //-----------------------------------------------------------
        /// Controls the origin of the sprite relative to its position.
        /// An origin of middle centre will draw the sprite with its
        /// centre at its world position. An origin of bottom centre
        /// will draw the sprite with its base at its world position
        ///
        /// @author S Downie
        ///
        /// @param Alignment (middle-centre by default)
        //-----------------------------------------------------------
        void SetOriginAlignment(AlignmentAnchor in_alignment);
        //-----------------------------------------------------------
        /// @author S Downie
        ///
        /// @return Returns alignment of this sprite's origin
        //-----------------------------------------------------------
        AlignmentAnchor GetOriginAlignment() const;
        
    private:
        //------------------------------------------------------------
        /// Triggered when the component is attached to an entity on
        /// the scene
        ///
        /// @author S Downie
        //------------------------------------------------------------
        void OnAddedToScene() override;
        //------------------------------------------------------------
        /// Called during the Render Snapshot phase of the render
        /// pipeline to capture all render primitives in the scene.
        /// This builds a new dynamic mesh for the sprite described
        /// by this component and adds a new render object which uses
        /// it.
        ///
        /// @author Ian Copland
        ///
        /// @param renderSnapshot - The snapshot containing all
        /// data pertaining to a single frame.
        /// @param frameAllocator - Use this to allocate any memory
        /// for this render frame
        //------------------------------------------------------------
        void OnRenderSnapshot(RenderSnapshot& renderSnapshot, IAllocator* frameAllocator) noexcept override;
        //------------------------------------------------------------
        /// Triggered when the component is removed from an entity on
        /// the scene
        ///
        /// @author S Downie
        //------------------------------------------------------------
        void OnRemovedFromScene() override;
        //------------------------------------------------------------
        /// On Transform Changed
        ///
        /// Our local transform or our parent transform has changed
        /// We must rebuild our sprite data
        //------------------------------------------------------------
        void OnTransformChanged();
        //-----------------------------------------------------------
        /// @author S Downie
        ///
        /// @return Preferred size based on the current atlas, id or texture
        //-----------------------------------------------------------
        Vector2 GetPreferredSize() const;
        //-----------------------------------------------------------
        /// @author S Downie
        ///
        /// @return Whether the texture has changed in a manner to
        /// invalidate the size cache
        //-----------------------------------------------------------
        bool IsTextureSizeCacheValid() const;
        //-----------------------------------------------------------
        /// Once the sprite has adjusted to the new texture size
        /// it can flag it as no longer dirty
        ///
        /// @author S Downie
        //-----------------------------------------------------------
        void SetTextureSizeCacheValid();
        //------------------------------------------------------------------------------
        /// Calculates the frame centre and size for the current texture atlas. Vertical
        /// and horizontal flipping are taken into account when calculating the centre.
        /// This should only be called when there is a texture atlas, otherwise the
        /// app is considered to be in an irrecoverable state and will exit.
        ///
        /// @author Ian Copland
        ///
        /// @param out_centre - [Output] The frame centre from the centre of the sprite.
        /// @param out_size - [Output] The sprite frame size.
        //------------------------------------------------------------------------------
        void CalcFrameCentreAndSize(Vector2& out_centre, Vector2& out_size) const;

    private:
        EventConnectionUPtr m_transformChangedConnection;
    
        TextureAtlasCSPtr m_textureAtlas;
        u32 m_hashedTextureAtlasId = 0;
    
        SizePolicyDelegate m_sizePolicyDelegate;
        Vector2 m_cachedTextureSize;
        Vector2 m_originalSize;
        
        UVs m_uvs;
        
        Colour m_colour;
        
        AlignmentAnchor m_originAlignment = AlignmentAnchor::k_middleCentre;
        
        bool m_flippedHorizontally = false;
        bool m_flippedVertically = false;
        bool m_vertexPositionsValid = false;
        
        bool m_isBSValid = false;
        bool m_isAABBValid = false;
        bool m_isOOBBValid = false;
        
        AABB mBoundingBox;
        OOBB mOBBoundingBox;
        Sphere mBoundingSphere;
        MaterialCSPtr mpMaterial;
        bool m_isVisible = true;
    };
}

#endif
