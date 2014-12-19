//
//  DrawableDef.h
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

#ifndef _CHILLISOURCE_UI_DRAWABLE_DRAWABLEDEF_H_
#define _CHILLISOURCE_UI_DRAWABLE_DRAWABLEDEF_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/QueryableInterface.h>
#include <ChilliSource/Core/File/StorageLocation.h>

#include <json/forwards.h>

namespace ChilliSource
{
    namespace UI
    {
        //---------------------------------------------------------------------
        /// A definition of a drawable which can be built from a json object.
        /// This is effectively an immutable container for the properties of a
        /// specific type of drawable.
        ///
        /// Drawable Defs can be either created using the concrete def's
        /// constructor or using the Create() method. This creates the def
        /// from json which describes the type and properties of the def.
        ///
        /// All drawable defs contain the following properties. Specific def
        /// types can also contain further properties.
        ///
        /// "Type": Describes the type of drawable def.
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
        /// "Colour": The colour of the drawable.
        ///
        /// @author Ian Copland
        //---------------------------------------------------------------------
        class DrawableDef : public Core::QueryableInterface
        {
        public:
            CS_DECLARE_NAMEDTYPE(DrawableDef);
            //--------------------------------------------------------------
            /// Creates a new drawable def from json which describes the
            /// type of def and its properties.
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
            static DrawableDefCUPtr Create(const Json::Value& in_json, Core::StorageLocation in_defaultLocation = Core::StorageLocation::k_package, const std::string& in_defaultPath = "");
            //--------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param The texture that will be rendered by the drawable.
            //--------------------------------------------------------------
            virtual const Rendering::TextureCSPtr& GetTexture() const = 0;
            //--------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param The texture atlas. If is supplied the drawable will
            /// render only the portion of the texture described by this and
            /// the atlas id.
            //--------------------------------------------------------------
            virtual const Rendering::TextureAtlasCSPtr& GetAtlas() const = 0;
            //--------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param The Id of the image in the texture atlas. This will
            /// be empty if there is no texture altas.
            //--------------------------------------------------------------
            virtual const std::string& GetAtlasId() const = 0;
            //--------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param The UVs of the image within the texture. If a texture
            /// atlas is used the UVs are virtual and are relative to the
            /// atlas image.
            //--------------------------------------------------------------
            virtual const Rendering::UVs& GetUVs() const = 0;
            //--------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param The colour of the drawable.
            //--------------------------------------------------------------
            virtual const Core::Colour& GetColour() const = 0;
            //--------------------------------------------------------------
            /// Destructor
            ///
            /// @author Ian Copland
            //--------------------------------------------------------------
            virtual ~DrawableDef() {};
        protected:
            friend class DrawableComponent;
            //--------------------------------------------------------------
            /// Creates a new instance of the drawable described by this
            /// definition. This should typically only be called by a
            /// drawable component.
            ///
            /// @author Ian Copland
            ///
            /// @return a new drawable instance.
            //--------------------------------------------------------------
            virtual DrawableUPtr CreateDrawable() const = 0;
        };
    }
}

#endif
