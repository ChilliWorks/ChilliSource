//
//  Shader.h
//  Chilli Source
//  Created by Scott Downie on 22/11/2010.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2010 Tag Games Limited
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
            friend class Core::ResourcePool;
            //----------------------------------------------------------
            /// Factory method that creates a new empty shader resource.
            /// Only called by resource pool
            ///
            /// @author S Downie
            ///
            /// @return Shader resource
            //----------------------------------------------------------
			static ShaderUPtr Create();
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