//
//  DrawableDesc.h
//  Chilli Source
//  Created by Ian Copland on 21/11/2014.
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

#ifndef _CHILLISOURCE_UI_DRAWABLE_DRAWABLEDESC_H_
#define _CHILLISOURCE_UI_DRAWABLE_DRAWABLEDESC_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/File/StorageLocation.h>
#include <ChilliSource/UI/Drawable/DrawableType.h>
#include <ChilliSource/UI/Drawable/ThreePatchDrawable.h>
#include <ChilliSource/Rendering/Texture/UVs.h>

#include <json/forwards.h>

namespace ChilliSource
{
    namespace UI
    {
        //---------------------------------------------------------------------
        /// A description of a drawable which can be built from a json object.
        /// This contains all possible properties that a drawable can contain
        /// but can only be constructed using a valid set of properties.
        /// Invalid properties will remain as their default value.
        ///
        /// The properties contained in a drawable desc are as follows:
        ///
        /// "Type": Describes the type of layout that should be used. The layout
        /// type determines which of the other properties can be used.
        ///
        /// "TextureLocation": The storage location of the texture that is
        /// used. If this is not specified a relative path will be instead used,
        /// loading the drawable relative to the parent resource.
        ///
        /// "TexturePath": The path to the texture the drawable will render.
        /// A texture must always be supplied.
        ///
        /// "AtlasLocation": The storage location of the texture atlas. This
        /// defaults to Package.
        ///
        /// "AtlasPath": The file path of the texture atlas. A texture atlas
        /// is optional, if one is not supplied the whole texture will be used.
        ///
        /// "AtlasId": The Id of the image in the texture atlas. This should
        /// not be supplied unless a texture atlas is used.
        ///
        /// "UVs": The UVs of the image within the texture. If a texture atlas
        /// is used the UVs are virtual and are relative to the atlas image.
        /// UV wrapping does not work for texture atlased images.
        ///
        /// "NinePatchInsets": The insets from the edges of the stretchable
        /// portion of the image. This should only be supplied for Nine-Patch
        /// type drawables.
        ///
        /// "ThreePatchInsets": The insets from the edges of the stretchable
        /// portion of the image. This should only be supplied for Three-Patch
        /// type drawables.
        ///
        /// "ThreePatchDirection": The direction the Three-Patch will stretch
        /// in: horizontal or vertical. Defaults to horizontal.
        ///
        /// @author Ian Copland
        //---------------------------------------------------------------------
        class DrawableDesc final
        {
        public:
            //--------------------------------------------------------------
            /// Constructor. Creates an empty drawable description.
            ///
            /// @author Ian Copland
            //--------------------------------------------------------------
            DrawableDesc() = default;
            //--------------------------------------------------------------
            /// Constructor. Creates the drawable description from a JSON
            /// object.
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
            DrawableDesc(const Json::Value& in_json, Core::StorageLocation in_relStorageLocation = Core::StorageLocation::k_package, const std::string& in_relDirectoryPath = "");
            //--------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param The type of drawable.
            //--------------------------------------------------------------
            DrawableType GetType() const;
            //--------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param The storage location of the texture that should be
            /// loaded.
            //--------------------------------------------------------------
            Core::StorageLocation GetTextureLocation() const;
            //--------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param The file path of the texture.
            //--------------------------------------------------------------
            const std::string& GetTexturePath() const;
            //--------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param The storage location of the texture atlas that should
            /// be loaded.
            //--------------------------------------------------------------
            Core::StorageLocation GetAtlasLocation() const;
            //--------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param The file path of the texture atlas. This may be empty
            /// in which case no texture atlas should be used.
            //--------------------------------------------------------------
            const std::string& GetAtlasPath() const;
            //--------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param The Id of the image in the texture atlas. This will
            /// be empty if there is no texture altas.
            //--------------------------------------------------------------
            const std::string& GetAtlasId() const;
            //--------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param The UVs of the image within the texture. If a texture
            /// atlas is used the UVs are virtual and are relative to the
            /// atlas image.
            //--------------------------------------------------------------
            const Rendering::UVs& GetUVs() const;
            //--------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param The insets from the edges of the stretchable portion
            /// of the image. This will always be a zero sized vector unless
            /// the type is nine-patch.
            //--------------------------------------------------------------
            const Core::Vector4& GetNinePatchInsets() const;
            //--------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param The insets from the edges of the stretchable portion
            /// of the image. This will always be a zero sized vector unless
            /// the type is three-patch.
            //--------------------------------------------------------------
            const Core::Vector2& GetThreePatchInsets() const;
            //--------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param The direction that a Three-Patch drawable will stretch.
            /// This should only be used with a 3-patch drawable.
            //--------------------------------------------------------------
            ThreePatchDrawable::Direction GetThreePatchDirection() const;
            
        private:
            DrawableType m_type = DrawableType::k_standard;
            Core::StorageLocation m_textureLocation = Core::StorageLocation::k_package;
            std::string m_texturePath;
            Core::StorageLocation m_atlasLocation = Core::StorageLocation::k_package;
            std::string m_atlasPath;
            std::string m_atlasId;
            Rendering::UVs m_uvs;
            Core::Vector4 m_ninePatchInsets;
            Core::Vector2 m_threePatchInsets;
            ThreePatchDrawable::Direction m_threePatchDirection;
        };
    }
}

#endif
