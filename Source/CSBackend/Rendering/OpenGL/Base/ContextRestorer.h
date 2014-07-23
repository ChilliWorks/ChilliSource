//
//  ContextRestorer.h
//  Chilli Source
//  Created by Scott Downie on 14/04/2014.
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

#ifndef _CSBACKEND_RENDERING_OPENGL_BASE_CONTEXTRESTORER_H_
#define _CSBACKEND_RENDERING_OPENGL_BASE_CONTEXTRESTORER_H_

#ifdef CS_TARGETPLATFORM_ANDROID

#include <ChilliSource/ChilliSource.h>
#include <CSBackend/Rendering/OpenGL/ForwardDeclarations.h>

#include <vector>

namespace CSBackend
{
    namespace OpenGL
    {
        //---------------------------------------------------------
        /// OpenGL on Android has its context killed whenever the
        /// app is suspended. This means all GL resources such
        /// as textures, shaders and buffers are destroyed and
        /// must be recreated on resume.
        ///
        /// @author S Downie
        //---------------------------------------------------------
        class ContextRestorer final
        {
        public:
            
            CS_DECLARE_NOCOPY(ContextRestorer);
            //-----------------------------------------------------
            /// Constructor
            ///
            /// @author S Downie
            //-----------------------------------------------------
            ContextRestorer() = default;
            //-----------------------------------------------------
            /// Take a snapshot of the GL context for resources
            /// that cannot be recreated from file and store them
            /// so they can be recreated
            ///
            /// @author S Downie
            //-----------------------------------------------------
            void Backup();
            //-----------------------------------------------------
            /// Recreate the GL context with the backed up resources
            ///
            /// @author S Downie
            //-----------------------------------------------------
            void Restore();
            //-----------------------------------------------------
            /// Add a mesh buffer to be backed up and restored
            ///
            /// @author S Downie
            ///
            /// @param Mesh buffer
            //-----------------------------------------------------
            void AddMeshBuffer(MeshBuffer* in_meshBuffer);
            //-----------------------------------------------------
            /// Remove a mesh buffer so it is no longer backed
            /// up or restored
            ///
            /// @author S Downie
            ///
            /// @param Mesh buffer
            //-----------------------------------------------------
            void RemoveMeshBuffer(MeshBuffer* in_meshBuffer);
        private:
            
            std::vector<MeshBuffer*> m_meshBuffers;
            bool m_hasContextBeenBackedUp = false;
        };
    }
}

#endif

#endif
