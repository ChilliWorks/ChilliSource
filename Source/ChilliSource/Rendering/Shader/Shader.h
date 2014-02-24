/*
 * File: Shader.h
 * Date: 22/11/2010 2010 
 * Description: Abstract interface to concrete shader container such as GLShader.
 */

/*
 * Author: Scott Downie
 * Version: v 1.0
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#ifndef _MO_FLO_RENDERING_SHADER_H_
#define _MO_FLO_RENDERING_SHADER_H_

#include <ChilliSource/Core/Resource/Resource.h>

namespace ChilliSource
{
	namespace Rendering
	{
		enum class ShaderType
		{
            k_combined,
            k_vertex,
            k_fragment
		};
		
		class IShader : public Core::IResource
		{
		public:
			
			virtual ~IShader(){}
			DECLARE_NAMED_INTERFACE(IShader);
			
		protected:
			
			//Can only be created via the resource manager
			IShader(){}
		};
	}
}

#endif