/*
 *  Static Constructor.cpp
 *  MoFlow
 *
 *  Created by Ian Copland on 09/01/2012.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLOW_CORE_STATICCONSTRUCTOR_H_
#define _MOFLOW_CORE_STATICCONSTRUCTOR_H_

//=============================================================
/// As C++ doesn't support static constructors, these macros can
/// be used as an equivalent.
//=============================================================
#define DECLARE_STATIC_CONSTRUCTOR()										\
	friend class _StaticConstructor_;										\
	class _StaticConstructor_ 												\
	{																		\
	public:																	\
		_StaticConstructor_();												\
	};																		\
	static _StaticConstructor_ _StaticConstructorInstance_;

#define DEFINE_STATIC_CONSTRUCTOR(inClass, inCode)							\
	inClass::_StaticConstructor_ inClass::_StaticConstructorInstance_;		\
	inClass::_StaticConstructor_::_StaticConstructor_()						\
	{																		\
		inCode;																\
	}

#endif
