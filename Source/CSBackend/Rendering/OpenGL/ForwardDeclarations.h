//
//  ForwardDeclarations.h
//  ChilliSource
//  Created by Scott Downie on 25/01/2012.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2012 Tag Games Limited
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

#ifndef _CSBACKEND_RENDERING_OPENGL_FORWARDDECLARATIONS_H_
#define _CSBACKEND_RENDERING_OPENGL_FORWARDDECLARATIONS_H_

#include <ChilliSource/Core/Base/StandardMacros.h>

#include <memory>

namespace CSBackend
{
    namespace OpenGL
    {
        //----------------------------------------------------
        /// Base
        //----------------------------------------------------
        CS_FORWARDDECLARE_CLASS(ContextState);
        CS_FORWARDDECLARE_CLASS(GLContextRestorer);

        CS_FORWARDDECLARE_CLASS(RenderCapabilities);
        CS_FORWARDDECLARE_CLASS(RenderCommandProcessor);
        //----------------------------------------------------
        /// Camera
        //----------------------------------------------------
        CS_FORWARDDECLARE_CLASS(GLCamera);
        //----------------------------------------------------
        /// Lighting
        //----------------------------------------------------
        CS_FORWARDDECLARE_CLASS(GLAmbientLight);
        CS_FORWARDDECLARE_CLASS(GLDirectionalLight);
        CS_FORWARDDECLARE_CLASS(GLLight);
        CS_FORWARDDECLARE_CLASS(GLPointLight);
        //----------------------------------------------------
        /// Model
        //----------------------------------------------------
        CS_FORWARDDECLARE_CLASS(GLMesh);
        CS_FORWARDDECLARE_CLASS(GLDynamicMesh);
        //----------------------------------------------------
        /// Shader
        //----------------------------------------------------
        CS_FORWARDDECLARE_CLASS(GLShader);
        //----------------------------------------------------
        /// Target
        //----------------------------------------------------
        CS_FORWARDDECLARE_CLASS(GLTargetGroup);
        //----------------------------------------------------
        /// Texture
        //----------------------------------------------------
        CS_FORWARDDECLARE_CLASS(GLCubemap);
        CS_FORWARDDECLARE_CLASS(GLTexture);
        CS_FORWARDDECLARE_CLASS(GLTextureUnitManager);
    }
}

#endif
