//
//  Drawble.h
//  ChilliSource
//  Created by Scott Downie on 17/04/2014.
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


#ifndef _CHILLISOURCE_UI_DRAWABLE_UIDRAWABLE_H_
#define _CHILLISOURCE_UI_DRAWABLE_UIDRAWABLE_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/QueryableInterface.h>

namespace ChilliSource
{
    //------------------------------------------------------------------------
    /// Interface for rendering widgets
    ///
    /// @author S Downie
    //------------------------------------------------------------------------
    class UIDrawable : public QueryableInterface
    {
    public:
        CS_DECLARE_NAMEDTYPE(UIDrawable);
        CS_DECLARE_NOCOPY(UIDrawable);
        //----------------------------------------------------------------------------------------
        /// Constructor
        ///
        /// @author S Downie
        //----------------------------------------------------------------------------------------
        UIDrawable() = default;
        //----------------------------------------------------------------------------------------
        /// @author S Downie
        ///
        /// @return The preferred size that the drawable wishes to de drawn at. This is usually
        /// based on an underlying image
        //----------------------------------------------------------------------------------------
        virtual Vector2 GetPreferredSize() const = 0;
        //----------------------------------------------------------------------------------------
        /// Get the texture that is currently used.
        ///
        /// @author HMcLaughlin
        ///
        /// @return Texture
        //----------------------------------------------------------------------------------------
        virtual const TextureCSPtr& GetTexture() const = 0;
        //----------------------------------------------------------------------------------------
        /// Get the texture atlas that is currently used.
        ///
        /// @author HMcLaughlin
        ///
        /// @return Texture atlas
        //----------------------------------------------------------------------------------------
        virtual const TextureAtlasCSPtr& GetTextureAtlas() const = 0;
        //----------------------------------------------------------------------------------------
        /// Get the texture atlas frame Id that is currently used.
        ///
        /// @author HMcLaughlin
        ///
        /// @return Frame Id
        //----------------------------------------------------------------------------------------
        virtual const std::string& GetTextureAtlasId() const = 0;
        //----------------------------------------------------------------------------------------
        /// Get the UVs that are currently used. UVs are relative to the
        /// frame and not the overall atlas
        ///
        /// @author HMcLaughlin
        ///
        /// @return Rectangle containing U, V, S, T
        //----------------------------------------------------------------------------------------
        virtual const UVs& GetUVs() const = 0;
        //----------------------------------------------------------------------------------------
        /// Gets the colour of the drawable.
        ///
        /// @author HMcLaughlin
        ///
        /// @param The colour.
        //----------------------------------------------------------------------------------------
        virtual const Colour& GetColour() const = 0;
        //----------------------------------------------------------------------------------------
        /// Set the texture that should be used in subsequent draws
        ///
        /// @author S Downie
        ///
        /// @param Texture
        //----------------------------------------------------------------------------------------
        virtual void SetTexture(const TextureCSPtr& in_texture) = 0;
        //----------------------------------------------------------------------------------------
        /// Set the texture atlas that should be used in subsequent draws. This will
        /// potentially affect the UVs of a drawable.
        ///
        /// @author S Downie
        ///
        /// @param Texture atlas
        //----------------------------------------------------------------------------------------
        virtual void SetTextureAtlas(const TextureAtlasCSPtr& in_atlas) = 0;
        //----------------------------------------------------------------------------------------
        /// Set the texture atlas frame Id that should be used in subsequent draws. This will
        /// potentially affect the UVs of a drawable.
        ///
        /// NOTE: An atlas must have been set prior to calling this
        ///
        /// @author S Downie
        ///
        /// @param Frame Id
        //----------------------------------------------------------------------------------------
        virtual void SetTextureAtlasId(const std::string& in_atlasId) = 0;
        //----------------------------------------------------------------------------------------
        /// Set the UVs that should be used in subsequent draws. UVs are relative to the
        /// frame and not the overall atlas
        ///
        /// @author S Downie
        ///
        /// @param Rectangle containing U, V, S, T
        //----------------------------------------------------------------------------------------
        virtual void SetUVs(const UVs& in_UVs) = 0;
        //----------------------------------------------------------------------------------------
        /// Sets the colour of the drawable. The final colour of the drawable takes into account
        /// the owning widgets colour and this colour.
        ///
        /// @author Ian Copland
        ///
        /// @param The colour.
        //----------------------------------------------------------------------------------------
        virtual void SetColour(const Colour& in_colour) = 0;
        
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
        ///     Whether or not to create a clipping mask from this drawable or just render it
        ///
        virtual void Draw(CanvasRenderer* renderer, const Matrix3& transform, const Vector2& absSize, const Colour& absColour, CanvasDrawMode drawMode) noexcept = 0;
        //----------------------------------------------------------------------------------------
        /// Virtual destructor
        ///
        /// @author S Downie
        //----------------------------------------------------------------------------------------
        virtual ~UIDrawable(){};
    };
}

#endif
