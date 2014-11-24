//
//  StandardDrawable.h
//  Chilli Source
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


#ifndef _CHILLISOURCE_UI_DRAWABLE_STANADARDDRAWABLE_H_
#define _CHILLISOURCE_UI_DRAWABLE_STANADARDDRAWABLE_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Math/Vector2.h>
#include <ChilliSource/Rendering/Texture/TextureAtlas.h>
#include <ChilliSource/Rendering/Texture/UVs.h>
#include <ChilliSource/UI/Base/PropertyMap.h>
#include <ChilliSource/UI/Drawable/IDrawable.h>

namespace ChilliSource
{
    namespace UI
    {
        //----------------------------------------------------------------------------------------
        /// Interface for rendering widget with a texture and UVs.
        ///
        /// @author S Downie
        //----------------------------------------------------------------------------------------
        class StandardDrawable final : public IDrawable
        {
        public:
            //----------------------------------------------------------------------------------------
            /// Constructor
            ///
            /// @author Ian Copland
            ///
            /// @param The texture.
            //----------------------------------------------------------------------------------------
            StandardDrawable(const Rendering::TextureCSPtr& in_texture);
            //----------------------------------------------------------------------------------------
            /// Constructor
            ///
            /// @author Ian Copland
            ///
            /// @param The texture.
            /// @param The texture atlas.
            /// @param The atlas id.
            //----------------------------------------------------------------------------------------
            StandardDrawable(const Rendering::TextureCSPtr& in_texture, const Rendering::TextureAtlasCSPtr& in_atlas, const std::string& in_atlasId);
            //---------------------------------------------------------------------------------------
            /// Constructor that builds the drawable from a drawable description. The description
            /// must be for a standard drawable type or this will assert.
            ///
            /// @author Ian Copland
            ///
            /// @param The drawable description.
            //----------------------------------------------------------------------------------------
            StandardDrawable(const DrawableDesc& in_desc);
            //----------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return The type of this drawable instance
            //----------------------------------------------------------------------------------------
            DrawableType GetType() const override;
            //----------------------------------------------------------------------------------------
            /// Set the texture that should be used in subsequent draws
            ///
            /// @author S Downie
            ///
            /// @param Texture
            //----------------------------------------------------------------------------------------
            void SetTexture(const Rendering::TextureCSPtr& in_texture) override;
            //----------------------------------------------------------------------------------------
            /// Set the texture atlas that should be used in subsequent draws.
            ///
            /// @author S Downie
            ///
            /// @param Texture atlas
            //----------------------------------------------------------------------------------------
            void SetTextureAtlas(const Rendering::TextureAtlasCSPtr& in_atlas) override;
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
            void SetUVs(const Rendering::UVs& in_UVs) override;
            //----------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return The preferred size that the drawable wishes to de drawn at based on the
            /// texture size
            //----------------------------------------------------------------------------------------
            Core::Vector2 GetPreferredSize() const override;
            //----------------------------------------------------------------------------------------
            /// Render the widget using the canvas renderer. The widget has is rendered using the
            /// set texture and UVs.
            ///
            /// @author S Downie
            ///
            /// @param Renderer
            /// @param Absolute screen transform
            /// @param Asbolute screen size
            /// @param Absolute colour
            //----------------------------------------------------------------------------------------
            void Draw(Rendering::CanvasRenderer* in_renderer, const Core::Matrix3& in_transform, const Core::Vector2& in_absSize, const Core::Colour& in_absColour) override;
            
        private:
            
            Rendering::TextureCSPtr m_texture;
            Rendering::TextureAtlasCSPtr m_atlas;
            Rendering::TextureAtlas::Frame m_atlasFrame;
            Rendering::UVs m_uvs;
            std::string m_atlasId;
        };
    }
}

#endif