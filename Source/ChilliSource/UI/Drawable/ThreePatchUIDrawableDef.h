//
//  ThreePatchUIDrawableDef.h
//  Chilli Source
//  Created by Ian Copland on 15/12/2014.
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

#ifndef _CHILLISOURCE_UI_DRAWABLE_THREEPATCHUIDRAWABLEDEF_H_
#define _CHILLISOURCE_UI_DRAWABLE_THREEPATCHUIDRAWABLEDEF_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/Colour.h>
#include <ChilliSource/Core/Math/Vector2.h>
#include <ChilliSource/Rendering/Texture/UVs.h>
#include <ChilliSource/UI/Drawable/UIDrawableDef.h>
#include <ChilliSource/UI/Drawable/ThreePatchUIDrawable.h>

namespace ChilliSource
{
    //---------------------------------------------------------------------
    /// A definition of a Three-Patch UIDrawable which can be built from a
    /// json object. This is effectively an immutable container for the
    /// properties of a Three-Patch UIDrawable.
    ///
    /// A Three-Patch UIDrawable Def contains the following properties in
    /// addition to those defined in the UIDrawable Def base class
    /// documentation:
    ///
    /// "Insets": The insets from the edges of the stretchable portion of
    /// the image. 
    ///
    /// "Direction": The direction the Three-Patch will stretch in:
    /// horizontal or vertical. Defaults to horizontal.
    ///
    /// @author Ian Copland
    //---------------------------------------------------------------------
    class ThreePatchUIDrawableDef final : public UIDrawableDef
    {
    public:
        CS_DECLARE_NAMEDTYPE(ThreePatchUIDrawableDef);
        //--------------------------------------------------------------
        /// Constructor. Creates an empty standard drawable definition.
        ///
        /// @author Ian Copland
        //--------------------------------------------------------------
        ThreePatchUIDrawableDef() = default;
        //--------------------------------------------------------------
        /// Constructor. Creates a standard drawable definition from
        /// json.
        ///
        /// @author Ian Copland
        ///
        /// @param The JSON object containing the drawable properties.
        /// @param [Optional] The relative storage location which will
        /// be used if there is no storage location specified in the
        /// json. Defaults to package.
        /// @param [Optional] The directory paths will be relative to if
        /// there is no storage location specified in the json. Defaults
        /// to empty.
        //--------------------------------------------------------------
        ThreePatchUIDrawableDef(const Json::Value& in_json, StorageLocation in_defaultLocation = StorageLocation::k_package, const std::string& in_defaultPath = "");
        //--------------------------------------------------------------
        /// Constructor. Creates a standard drawable definition from
        /// with just a texture.
        ///
        /// @author Ian Copland
        ///
        /// @param The texture.
        /// @param The insets.
        /// @param The stretch direction.
        /// @param [Optional] The colour. Defaults to white.
        /// @param [Optional] The UVs. Defaults to (0, 0, 1, 1)
        //--------------------------------------------------------------
        ThreePatchUIDrawableDef(const TextureCSPtr& in_texture, const Vector2& in_insets, ThreePatchUIDrawable::Direction in_direction, const Colour& in_colour = Colour::k_white,
                              const UVs& in_uvs = UVs());
        //--------------------------------------------------------------
        /// Constructor. Creates a standard drawable definition from
        /// with a texture atlas.
        ///
        /// @author Ian Copland
        ///
        /// @param The texture.
        /// @param The texture atlas.
        /// @param The texture atlas id.
        /// @param The insets.
        /// @param The stretch direction.
        /// @param [Optional] The colour. Defaults to white.
        /// @param [Optional] The UVs. Defaults to (0, 0, 1, 1)
        //--------------------------------------------------------------
        ThreePatchUIDrawableDef(const TextureCSPtr& in_texture, const TextureAtlasCSPtr& in_atlas, const std::string& in_atlasId, const Vector2& in_insets,
                              ThreePatchUIDrawable::Direction in_direction, const Colour& in_colour = Colour::k_white, const UVs& in_uvs = UVs());
        //--------------------------------------------------------------
        /// Allows querying of whether or not the component implements
        /// the interface associated with the given interface Id.
        /// Typically this won't be called directly, instead the
        /// templated version IsA<Interface>() should be used.
        ///
        /// @author Ian Copland
        ///
        /// @param The interface Id.
        ///
        /// @return Whether the object implements the given interface.
        //--------------------------------------------------------------
        bool IsA(InterfaceIDType in_interfaceId) const override;
        //--------------------------------------------------------------
        /// @author Ian Copland
        ///
        /// @param The texture that will be rendered by the drawable.
        //--------------------------------------------------------------
        const TextureCSPtr& GetTexture() const override;
        //--------------------------------------------------------------
        /// @author Ian Copland
        ///
        /// @param The texture atlas. If is supplied the drawable will
        /// render only the portion of the texture described by this and
        /// the atlas id.
        //--------------------------------------------------------------
        const TextureAtlasCSPtr& GetAtlas() const override;
        //--------------------------------------------------------------
        /// @author Ian Copland
        ///
        /// @param The Id of the image in the texture atlas. This will
        /// be empty if there is no texture altas.
        //--------------------------------------------------------------
        const std::string& GetAtlasId() const override;
        //--------------------------------------------------------------
        /// @author Ian Copland
        ///
        /// @param The UVs of the image within the texture. If a texture
        /// atlas is used the UVs are virtual and are relative to the
        /// atlas image.
        //--------------------------------------------------------------
        const UVs& GetUVs() const override;
        //--------------------------------------------------------------
        /// @author Ian Copland
        ///
        /// @param The colour of the drawable.
        //--------------------------------------------------------------
        const Colour& GetColour() const override;
        //--------------------------------------------------------------
        /// @author Ian Copland
        ///
        /// @param The insets from the edges of the stretchable portion
        /// of the image.
        //--------------------------------------------------------------
        const Vector2& GetInsets() const;
        //--------------------------------------------------------------
        /// @author Ian Copland
        ///
        /// @param The direction that a Three-Patch drawable will stretch.
        //--------------------------------------------------------------
        ThreePatchUIDrawable::Direction GetDirection() const;
        
    private:
        //--------------------------------------------------------------
        /// Creates a new instance of a standard drawable as described
        /// by this definition. This should typically only be called by
        /// a drawable component.
        ///
        /// @author Ian Copland
        ///
        /// @return a new drawable instance.
        //--------------------------------------------------------------
        UIDrawableUPtr CreateDrawable() const override;
        
        TextureCSPtr m_texture;
        TextureAtlasCSPtr m_atlas;
        std::string m_atlasId;
        UVs m_uvs;
        Colour m_colour;
        Vector2 m_insets;
        ThreePatchUIDrawable::Direction m_direction;
    };
}

#endif
