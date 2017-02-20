//
//  CanvasMaterialPool.h
//  ChilliSource
//  Created by Ian Copland on 30/07/2015
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

#ifndef _CHILLISOURCE_RENDERING_BASE_CANVASMATERIALPOOL_H_
#define _CHILLISOURCE_RENDERING_BASE_CANVASMATERIALPOOL_H_

#include <ChilliSource/ChilliSource.h>

#include <functional>
#include <tuple>
#include <vector>
#include <unordered_map>

namespace ChilliSource
{
    /// Materials are not exposed to the user of the canvas renderer, but it still
    /// internally uses materials for rendering. This manages those materials,
    /// providing a means to get a shared material for the given texture and stencil mask,
    /// ensuring objects with the same values can be rendered as part of the same batch.
    /// Materials can be re-used for different textures in subsequent frames.
    ///
    class CanvasMaterialPool final
    {
    public:
        
        using MaterialSetupDelegate = std::function<void(Material*)>;
        
        /// @param materialFactory
        ///     The material factory which is used to generate new materials as required.
        /// @param materialNamePrefix
        ///     The base name for any created materials
        /// @param materialSetupDelegate
        ///     Called after creating a material allowing it to be customised
        ///
        CanvasMaterialPool(MaterialFactory* materialFactory, const std::string& materialNamePrefix, const MaterialSetupDelegate& materialSetupDelegate);
        //------------------------------------------------------------------------------
        /// @author Ian Copland
        ///
        /// @param in_texture - The texture for which to get a material
        ///
        /// @return The material associated with the given texture.
        //------------------------------------------------------------------------------
        MaterialCSPtr GetMaterial(const TextureCSPtr& texture, s32 stencilRef);
        //------------------------------------------------------------------------------
        /// Clears all associations between textures and materials, ensuring the
        /// the materials no longer reference the previously associated texture.
        /// After this is called the materials can be re-used for other textures.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        void Clear();
        
    private:
        
        // Unique identifier for canvas materials (texture and stencil mask)
        using Key = std::tuple<const Texture*, s32>;
        
        struct KeyHash : public std::unary_function<Key, std::size_t>
        {
            /// Custom hashing functor for the Key tuple.
            ///
            /// @param key
            ///     Tuple containing texture and stencil
            /// @return Hash of the given tuple
            ///
            std::size_t operator()(const Key& key) const
            {
                return std::size_t(std::get<0>(key)) ^ std::get<1>(key);
            }
        };
        
        MaterialSetupDelegate m_materialSetupDelegate;
        std::string m_materialNamePrefix;
        MaterialFactory* m_materialFactory = nullptr;
        u32 m_nextMaterial = 0;
        std::vector<MaterialSPtr> m_materials;
        std::unordered_map<Key, MaterialCSPtr, KeyHash> m_associations;
    };
}

#endif
