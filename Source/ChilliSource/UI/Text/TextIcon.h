//
//  TextIcon.h
//  Chilli Source
//  Created by Nicolas Tanda on 23/02/2015.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2015 Tag Games Limited
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

#ifndef _CHILLISOURCE_UI_TEXT_TEXTICON_H_
#define _CHILLISOURCE_UI_TEXT_TEXTICON_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Math/Vector2.h>

#include <unordered_map>

namespace ChilliSource
{
    namespace UI
    {
        //--------------------------------------------------------------------------
        /// A text specific image data
        ///
        /// @author Nicolas Tanda
        //--------------------------------------------------------------------------
        class TextIcon final
        {
        public:
            //-------------------------------------------------------------------
            /// Default constructor
            ///
            /// @author Nicolas Tanda
            //-------------------------------------------------------------------
            TextIcon() = default;
            //-------------------------------------------------------------------
            /// @author Nicolas Tanda
            ///
            /// @param Texture
            /// @param Scale of the image, relative to the text height
            //-------------------------------------------------------------------
            TextIcon(const Rendering::TextureCSPtr& in_texture, f32 in_scale = 1.0f);
            //-------------------------------------------------------------------
            /// @author Nicolas Tanda
            ///
            /// @param Texture
            /// @param Texture atlas
            /// @param Texture atlas ID
            /// @param Scale of the image, relative to the text height
            //-------------------------------------------------------------------
            TextIcon(const Rendering::TextureCSPtr& in_texture, const Rendering::TextureAtlasCSPtr& in_atlas, const std::string& in_atlasID, f32 in_scale = 1.0f);
            
            //-------------------------------------------------------------------
            /// @author Nicolas Tanda
            ///
            /// @return The texture of the image
            //-------------------------------------------------------------------
            const Rendering::TextureCSPtr& GetTexture() const;
            //-------------------------------------------------------------------
            /// @author Nicolas Tanda
            ///
            /// @return The texture atlas of the image
            //-------------------------------------------------------------------
            const Rendering::TextureAtlasCSPtr& GetTextureAtlas() const;
            //-------------------------------------------------------------------
            /// @author Nicolas Tanda
            ///
            /// @return The texture atlas ID of the image
            //-------------------------------------------------------------------
            const std::string& GetTextureAtlasID() const;
            //-------------------------------------------------------------------
            /// @author Nicolas Tanda
            ///
            /// @return The scale of the image
            //-------------------------------------------------------------------
            f32 GetScale() const;
            
        private:
            Rendering::TextureCSPtr m_texture;
            Rendering::TextureAtlasCSPtr m_atlas;
            std::string m_atlasID;
            
            f32 m_scale = 1.0f;
        };
    }
}

#endif