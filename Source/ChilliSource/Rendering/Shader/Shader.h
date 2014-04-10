//
//  Shader.h
//  Chilli Source
//
//  Created by Scott Downie on 22/11/2010.
//  Copyright (c) 2010 Tag Games Ltd. All rights reserved.
//

#ifndef _CHILLISOURCE_RENDERING_SHADER_SHADER_H_
#define _CHILLISOURCE_RENDERING_SHADER_SHADER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Resource/Resource.h>

namespace ChilliSource
{
	namespace Rendering
	{
        //------------------------------------------------
        /// Interface into a shader resource. Shaders
        /// are rendering platform dependent and are used
        /// by the renderer to decide how to pass data
        /// to the currently executing shader program
        ///
        /// @author S Downie
        //------------------------------------------------
		class Shader : public Core::Resource
		{
		public:
			
            CS_DECLARE_NAMEDTYPE(Shader);
            
            //------------------------------------------------
            /// Virtual destructor
            ///
            /// @author S Downie
            //------------------------------------------------
			virtual ~Shader(){}
			
		protected:
			
            //------------------------------------------------
            /// Private constructor to ensure that
            /// "abstract" shader cannot be created
            ///
            /// @author S Downie
            //------------------------------------------------
			Shader() = default;
		};
	}
}

#endif