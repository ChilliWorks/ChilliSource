/*
 *  Texture.h
 *  MoFlow
 *
 *  Created by Tag Games on 01/10/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLO_RENDERING_ITEXTURE_H_
#define _MOFLO_RENDERING_ITEXTURE_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Resource/ResourceOld.h>

namespace ChilliSource
{
	namespace Rendering
	{
		class Texture : public Core::ResourceOld
		{
		public:
			
			virtual ~Texture(){}
			CS_DECLARE_NAMEDTYPE(Texture);
			
			enum class Filter
			{
                k_point,
                k_linear
			};
			
			enum class WrapMode
			{
                k_clamp,
                k_repeat
			};
			
			virtual void Bind(u32 inSlot = 0) = 0;
			virtual void Unbind() = 0;
			
			virtual void SetFilter(Filter ineSFilter, Filter ineTFilter) = 0;
			virtual void SetWrapMode(WrapMode inSWrapMode, WrapMode inTWrapMode) = 0;
			
			u32 GetWidth() const{return mudwWidth;}
			u32 GetHeight() const {return mudwHeight;}
			
			void SetWidth(u32 inudwWidth){mudwWidth = inudwWidth;}
			void SetHeight(u32 inudwHeight) {mudwHeight = inudwHeight;}
			
		protected:
			//Only texture loader can create this
			Texture():mudwWidth(0), mudwHeight(0){};
			
		protected:
			u32 mudwWidth;
			u32 mudwHeight;
		};
	}
}

#endif