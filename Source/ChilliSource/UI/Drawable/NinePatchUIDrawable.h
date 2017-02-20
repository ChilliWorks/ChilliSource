//
//  NinePatchUIDrawable.h
//  ChilliSource
//  Created by Scott Downie on 24/07/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
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


#ifndef _CHILLISOURCE_UI_DRAWABLE_NINEPATCHUIDRAWABLE_H_
#define _CHILLISOURCE_UI_DRAWABLE_NINEPATCHUIDRAWABLE_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/Colour.h>
#include <ChilliSource/Rendering/Texture/TextureAtlas.h>
#include <ChilliSource/Rendering/Texture/UVs.h>
#include <ChilliSource/UI/Drawable/UIDrawable.h>

#include <array>

namespace ChilliSource
{
    //----------------------------------------------------------------------------------------
    /// Interface for rendering widget with a texture and UVs as a 9 patch. This allows the
    /// widget to be scaled without distorting the edges. The patches are specified by percentage
    /// insets from the edges of the widget.
    ///
    /// The right and left patches should be designed to stretch vertically.
    /// The top and bottom patches should be designed to stretch horizontally.
    /// The centre patch should be designed to stretch horizontally and vertically
    ///
    /// @author S Downie
    //----------------------------------------------------------------------------------------
    class NinePatchUIDrawable final : public UIDrawable
    {
    public:
        CS_DECLARE_NAMEDTYPE(NinePatchUIDrawable);
        
        static const u32 k_numPatches = 9;
        //----------------------------------------------------------------------------------------
        /// Allows querying of whether or not the component implements the interface associated
        /// with the given interface Id. Typically this won't be called directly, instead the
        /// templated version IsA<Interface>() should be used.
        ///
        /// @author Ian Copland
        ///
        /// @param The interface Id.
        ///
        /// @return Whether the object implements the given interface.
        //----------------------------------------------------------------------------------------
        bool IsA(InterfaceIDType in_interfaceId) const override;
        //----------------------------------------------------------------------------------------
        /// Get the texture that is currently used.
        ///
        /// @author HMcLaughlin
        ///
        /// @return Texture
        //----------------------------------------------------------------------------------------
        const TextureCSPtr& GetTexture() const override;
        //----------------------------------------------------------------------------------------
        /// Get the texture atlas that is currently used.
        ///
        /// @author HMcLaughlin
        ///
        /// @return Texture atlas
        //----------------------------------------------------------------------------------------
        const TextureAtlasCSPtr& GetTextureAtlas() const override;
        //----------------------------------------------------------------------------------------
        /// Get the texture atlas frame Id that is currently used.
        ///
        /// @author HMcLaughlin
        ///
        /// @return Frame Id
        //----------------------------------------------------------------------------------------
        const std::string& GetTextureAtlasId() const override;
        //----------------------------------------------------------------------------------------
        /// Get the UVs that are currently used. UVs are relative to the
        /// frame and not the overall atlas
        ///
        /// @author HMcLaughlin
        ///
        /// @return Rectangle containing U, V, S, T
        //----------------------------------------------------------------------------------------
        const UVs& GetUVs() const override;
        //----------------------------------------------------------------------------------------
        /// Gets the colour of the drawable.
        ///
        /// @author HMcLaughlin
        ///
        /// @param The colour.
        //----------------------------------------------------------------------------------------
        const Colour& GetColour() const override;
        //----------------------------------------------------------------------------------------
        /// Set the texture that should be used in subsequent draws
        ///
        /// @author S Downie
        ///
        /// @param Texture
        //----------------------------------------------------------------------------------------
        void SetTexture(const TextureCSPtr& in_texture) override;
        //----------------------------------------------------------------------------------------
        /// Set the texture atlas that should be used in subsequent draws.
        ///
        /// @author S Downie
        ///
        /// @param Texture atlas
        //----------------------------------------------------------------------------------------
        void SetTextureAtlas(const TextureAtlasCSPtr& in_atlas) override;
        //----------------------------------------------------------------------------------------
        /// Set the texture atlas frame Id that should be used in subsequent draws.
        ///
        /// NOTE: An atlas must have been set prior to calling this
        ///
        /// @author S Downie
        ///
        /// @param Frame Id
        //----------------------------------------------------------------------------------------
        void SetTextureAtlasId(const std::string& in_atlasId) override;
        //----------------------------------------------------------------------------------------
        /// Set the UVs that should be used in subsequent draws. UVs are relative to the
        /// frame and not the overall atlas
        ///
        /// @author S Downie
        ///
        /// @param Rectangle containing U, V, S, T
        //----------------------------------------------------------------------------------------
        void SetUVs(const UVs& in_UVs) override;
        //----------------------------------------------------------------------------------------
        /// Sets the colour of the drawable. The final colour of the drawable takes into account
        /// the owning widgets colour and this colour.
        ///
        /// @author Ian Copland
        ///
        /// @param The colour.
        //----------------------------------------------------------------------------------------
        void SetColour(const Colour& in_colour) override;
        //----------------------------------------------------------------------------------------
        /// Set the UV insets that should be used to create the patches. Insets are from the edge
        /// and therefore no negative numbers need to be specified for right and bottom insets.
        ///
        /// NOTE: Insets must compliment each other i.e. left and right cannot sum to more than 1.0
        /// as they would overlap and insets cannot be zero or less.
        ///
        /// @author S Downie
        ///
        /// @param Left inset as normalised fraction (0 - 1)
        /// @param Right inset as normalised fraction (0 - 1)
        /// @param Top inset as normalised fraction (0 - 1)
        /// @param Bottom inset as normalised fraction (0 - 1)
        //----------------------------------------------------------------------------------------
        void SetInsets(f32 in_left, f32 in_right, f32 in_top, f32 in_bottom);
        //----------------------------------------------------------------------------------------
        /// @author S Downie
        ///
        /// @return The preferred size that the drawable wishes to de drawn at based on the
        /// texture size
        //----------------------------------------------------------------------------------------
        Vector2 GetPreferredSize() const override;
        
        /// Render the widget using the canvas renderer.
        ///
        /// @param renderer
        ///     Performs the actual drawing to canvas
        /// @param transform
        ///     Absolute screen transform
        /// @param absSize
        ///     Asbolute screen size
        /// @param absColour
        ///     Absolute colour
        /// @param drawMode
        ///     Whether or not to create a clip mask for this drawable or just render to screen
        ///
        void Draw(CanvasRenderer* renderer, const Matrix3& transform, const Vector2& absSize, const Colour& absColour, CanvasDrawMode drawMode) noexcept override;
        
    private:
        friend class NinePatchUIDrawableDef;
        //----------------------------------------------------------------------------------------
        /// Constructor
        ///
        /// @author Ian Copland
        ///
        /// @param The texture.
        /// @param The left inset.
        /// @param The right inset.
        /// @param The top inset.
        /// @param The bottom inset.
        //----------------------------------------------------------------------------------------
        NinePatchUIDrawable(const TextureCSPtr& in_texture, f32 in_leftInset, f32 in_rightInset, f32 in_topInset, f32 in_bottomInset);
        //----------------------------------------------------------------------------------------
        /// Constructor
        ///
        /// @author Ian Copland
        ///
        /// @param The texture.
        /// @param The texture atlas.
        /// @param The atlas id.
        /// @param The left inset.
        /// @param The right inset.
        /// @param The top inset.
        /// @param The bottom inset.
        //----------------------------------------------------------------------------------------
        NinePatchUIDrawable(const TextureCSPtr& in_texture, const TextureAtlasCSPtr& in_atlas, const std::string& in_atlasId, f32 in_leftInset, f32 in_rightInset, f32 in_topInset, f32 in_bottomInset);
        
        /// Patches (size, pos, etc) are calculated and cached. This function when called will
        /// update the cache if required, otherwise it will do nothing
        ///
        /// @param absSize
        ///     Canvas space size of the widget, if changed from last draw, update the cache
        ///
        void UpdatePatchCache(const Vector2& absSize);
        
        TextureCSPtr m_texture;
        TextureAtlasCSPtr m_atlas;
        TextureAtlas::Frame m_atlasFrame;
        UVs m_uvs;
        std::string m_atlasId;
        
        std::array<UVs, k_numPatches> m_cachedUvs;
        std::array<Vector2, k_numPatches> m_cachedSizes;
        std::array<Vector2, k_numPatches> m_cachedPositions;
        Vector2 m_cachedOffsetTL;
        Vector2 m_cachedWidgetSize;
        Colour m_colour;
        
        f32 m_leftInset = 0.01f;
        f32 m_rightInset = 0.01f;
        f32 m_topInset = 0.01f;
        f32 m_bottomInset = 0.01f;
        
        bool m_isPatchCatchValid = false;
    };
}

#endif
