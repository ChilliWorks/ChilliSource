/*
 *  ITexture.h
 *  MoFlow
 *
 *  Created by Tag Games on 01/10/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLO_RENDERING_ITEXTURE_H_
#define _MOFLO_RENDERING_ITEXTURE_H_

#include <moFlo/Core/Resource.h>

namespace moFlo
{
	namespace Rendering
	{
		const f32 kTexelOffset		= 1.0f;
		const f32 kHalfTexelOffset	= kTexelOffset * 0.5f;
		
		class ITexture : public Core::IResource
		{
		public:
			
			virtual ~ITexture(){}
			DECLARE_NAMED_INTERFACE(ITexture);
			
			enum Filter
			{
				TEX_FILTER_POINT, 
				TEX_FILTER_LINEAR
			};
			
			enum WrapMode
			{
				TEX_WRAP_CLAMP,
				TEX_WRAP_REPEAT
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
			ITexture():mudwWidth(0), mudwHeight(0){};
			
		protected:
			u32 mudwWidth;
			u32 mudwHeight;
		};
	}
}

#endif