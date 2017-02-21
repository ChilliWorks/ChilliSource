//
//  ThreePatchUIDrawable.h
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


#ifndef _CHILLISOURCE_UI_DRAWABLE_THREEPATCHUIDRAWABLE_H_
#define _CHILLISOURCE_UI_DRAWABLE_THREEPATCHUIDRAWABLE_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/Colour.h>
#include <ChilliSource/Core/Math/Vector2.h>
#include <ChilliSource/Rendering/Texture/TextureAtlas.h>
#include <ChilliSource/Rendering/Texture/UVs.h>
#include <ChilliSource/UI/Drawable/UIDrawable.h>

#include <array>
#include <functional>

namespace ChilliSource
{
    //----------------------------------------------------------------------------------------
    /// Interface for rendering widget with a texture and UVs as a 3 patch either horizontally
    /// or vertically. This allows the widget to be scaled in one direction without distorting the edges.
    /// The patches are specified by percentage insets from the edges of the widget
    ///
    /// The centre patch should be designed to stretch horizontally or vertically.
    ///
    /// @author S Downie
    //----------------------------------------------------------------------------------------
    class ThreePatchUIDrawable final : public UIDrawable
    {
    public:
        CS_DECLARE_NAMEDTYPE(ThreePatchUIDrawable);
        
        static const u32 k_numPatches = 3;
        //----------------------------------------------------------------------------------------
        /// The type of the 3-patch i.e. horizontal or vertical
        ///
        /// @author S Downie
        //----------------------------------------------------------------------------------------
        enum class Direction
        {
            k_horizontal,
            k_vertical
        };
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
        /// @param Left inset if horizontal 3-patch bottom inset if vertical 3-patch (as normalised fraction 0 - 1)
        /// @param Right inset if horizontal 3-patch top inset if vertical 3-patch (as normalised fraction 0 - 1)
        //----------------------------------------------------------------------------------------
        void SetInsets(f32 in_leftOrBottom, f32 in_rightOrTop);
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
        ///     Whether or not to create a clipping mask from this drawable or just render to screen
        ///
        void Draw(CanvasRenderer* renderer, const Matrix3& transform, const Vector2& absSize, const Colour& absColour, CanvasDrawMode drawMode) noexcept override;
        
    private:
        friend class ThreePatchUIDrawableDef;
        
        //----------------------------------------------------------------------------------------
        /// Calculates the UVs for each patch.
        ///
        /// @author S Downie
        ///
        /// @param Image frame
        /// @param Left inset if horizontal 3-patch bottom inset if vertical 3-patch (as normalised fraction 0 - 1)
        /// @param Right inset if horizontal 3-patch top inset if vertical 3-patch (as normalised fraction 0 - 1)
        ///
        /// @return UVs for each patch
        //----------------------------------------------------------------------------------------
        using CalculateUVsDelegate = std::function<std::array<UVs, k_numPatches>(const TextureAtlas::Frame&, f32, f32)>;
        //----------------------------------------------------------------------------------------
        /// Calculates the sizes for each patch.
        ///
        /// @author S Downie
        ///
        /// @param Widget absolute size
        /// @param Image frame
        /// @param Left inset if horizontal 3-patch bottom inset if vertical 3-patch (as normalised fraction 0 - 1)
        /// @param Right inset if horizontal 3-patch top inset if vertical 3-patch (as normalised fraction 0 - 1)
        ///
        /// @return Size for each patch
        //----------------------------------------------------------------------------------------
        using CalculateSizesDelegate = std::function<std::array<Vector2, k_numPatches>(const Vector2&, const TextureAtlas::Frame&, f32, f32)>;
        //----------------------------------------------------------------------------------------
        /// Calculates the local space positions for each patch.
        ///
        /// @author S Downie
        ///
        /// @param Widget absolute size
        /// @param Patch sizes
        ///
        /// @return Positions for each patch
        //----------------------------------------------------------------------------------------
        using CalculatePositionsDelegate = std::function<std::array<Vector2, k_numPatches>(const Vector2&, const std::array<Vector2, k_numPatches>&)>;
        //----------------------------------------------------------------------------------------
        /// Calculate the offset for the patches that will position them as if they still had
        /// their cropped space
        ///
        /// @author S Downie
        ///
        /// @param Widget absolute size
        /// @param Image frame
        /// @param Left inset if horizontal 3-patch bottom inset if vertical 3-patch (as normalised fraction 0 - 1)
        /// @param Right inset if horizontal 3-patch top inset if vertical 3-patch (as normalised fraction 0 - 1)
        ///
        /// @return Offset from top left
        //----------------------------------------------------------------------------------------
        using CalculateOffsetDelegate = std::function<Vector2(const Vector2&, const TextureAtlas::Frame&, f32, f32)>;
        //----------------------------------------------------------------------------------------
        /// Constructor
        ///
        /// @author Ian Copland
        ///
        /// @param The texture.
        /// @param The direction the drawable will stretch.
        /// @param The left inset if a horizontal 3-patch or the bottom inset if a vertical
        /// 3-patch. This should be provided as a normalised fraction, 0.0 - 1.0.
        /// @param The right inset if a horizontal 3-patch or the top inset if a vertical 3-patch.
        /// This should be provided as a normalised fraction, 0.0 - 1.0.
        //----------------------------------------------------------------------------------------
        ThreePatchUIDrawable(const TextureCSPtr& in_texture, Direction in_direction, f32 in_leftOrBottom, f32 in_rightOrTop);
        //----------------------------------------------------------------------------------------
        /// Constructor
        ///
        /// @author Ian Copland
        ///
        /// @param The texture.
        /// @param The texture atlas.
        /// @param The atlas id.
        /// @param The direction the drawable will stretch.
        /// @param The left inset if a horizontal 3-patch or the bottom inset if a vertical
        /// 3-patch. This should be provided as a normalised fraction, 0.0 - 1.0.
        /// @param The right inset if a horizontal 3-patch or the top inset if a vertical 3-patch.
        /// This should be provided as a normalised fraction, 0.0 - 1.0.
        //----------------------------------------------------------------------------------------
        ThreePatchUIDrawable(const TextureCSPtr& in_texture, const TextureAtlasCSPtr& in_atlas, const std::string& in_atlasId, Direction in_direction, f32 in_leftOrBottom, f32 in_rightOrTop);
        
        /// Patches (size, pos, etc) are calculated and cached. This function when called will
        /// update the cache if required, otherwise it will do nothing
        ///
        /// @param absSize
        ///     Canvas space size of the widget, if changed from last draw, update the cache
        ///
        void UpdatePatchCache(const Vector2& absSize);
        
        CalculateUVsDelegate m_uvCalculationDelegate;
        CalculateSizesDelegate m_sizeCalculationDelegate;
        CalculatePositionsDelegate m_positionCalculationDelegate;
        CalculateOffsetDelegate m_offsetCalculationDelegate;
        
        TextureCSPtr m_texture;
        TextureAtlasCSPtr m_atlas;
        TextureAtlas::Frame m_atlasFrame;
        UVs m_uvs;
        std::string m_atlasId;
        Colour m_colour;
        
        std::array<UVs, k_numPatches> m_cachedUvs;
        std::array<Vector2, k_numPatches> m_cachedSizes;
        std::array<Vector2, k_numPatches> m_cachedPositions;
        Vector2 m_cachedOffsetTL;
        Vector2 m_cachedWidgetSize;
        
        f32 m_leftOrBottomInset = 0.01f;
        f32 m_rightOrTopInset = 0.01f;
        
        bool m_isPatchCatchValid = false;
    };
}

#endif
