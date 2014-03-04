//
//  ForwardDeclarations.h
//  MoFlow
//
//  Created by Scott Downie on 25/01/2012.
//  Copyright (c) 2012 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_BACKEND_RENDERING_OPENGL_FORWARDDECLARATIONS_H_
#define _CHILLISOURCE_BACKEND_RENDERING_OPENGL_FORWARDDECLARATIONS_H_

namespace ChilliSource
{
    namespace OpenGL
    {
        //----------------------------------------------------
        /// Base
        //----------------------------------------------------
        CS_FORWARDDECLARE_CLASS(MeshBuffer);
        CS_FORWARDDECLARE_CLASS(RenderCapabilities);
        CS_FORWARDDECLARE_CLASS(RenderSystem);
        CS_FORWARDDECLARE_CLASS(RenderTarget);
        //----------------------------------------------------
        /// Shader
        //----------------------------------------------------
        CS_FORWARDDECLARE_CLASS(Shader);
        CS_FORWARDDECLARE_CLASS(ShaderManager);
        //----------------------------------------------------
        /// Texture
        //----------------------------------------------------
        CS_FORWARDDECLARE_CLASS(Cubemap);
        CS_FORWARDDECLARE_CLASS(CubemapManager);
        CS_FORWARDDECLARE_CLASS(Texture);
        CS_FORWARDDECLARE_CLASS(TextureManager);
    }
}

#endif
