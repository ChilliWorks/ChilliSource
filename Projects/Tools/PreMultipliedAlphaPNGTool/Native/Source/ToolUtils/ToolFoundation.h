/*
 *  ToolFoundation.cpp
 *  ToolUtils
 *
 *  Created by Robert Henning on 05/06/2012.
 *  Copyright 2012 Tag Games Ltd. All rights reserved.
 *
 */
 
#ifndef _TOOL_FOUNDATION_H_
#define _TOOL_FOUNDATION_H_

#define SAFE_DELETE(x)						if(x) delete(x); x = NULL;
#define SAFE_DELETE_ARRAY(x)				if(x) delete[] (x); x = NULL;

typedef char         		s8;
typedef short				s16;
typedef int					s32;
typedef long				s64;

typedef unsigned char		u8;
typedef unsigned short		u16;
typedef unsigned int		u32;
typedef unsigned long		u64;

typedef float				f32;
typedef double				f64;

struct CVector2i
{
	CVector2i(u32 inX = 0, u32 inY = 0)
	: x(inX), y(inY) {}
	u32 x;
	u32 y;	
};

#endif
