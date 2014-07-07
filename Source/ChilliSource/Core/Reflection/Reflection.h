//
//  Reflection.h
//  Chilli Source
//  Created by Scott Downie on 28/11/2011.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2011 Tag Games Limited
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

#ifndef _CHILLISOURCE_CORE_REFLECTION_REFLECTION_H_
#define _CHILLISOURCE_CORE_REFLECTION_REFLECTION_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Reflection/Property.h>
#include <ChilliSource/Core/Reflection/Instance.h>
#include <ChilliSource/Core/Reflection/MetaClass.h>
#include <ChilliSource/Core/Reflection/Registry.h>

#ifdef CS_ENABLE_REFLECTION
#include <ChilliSource/Core/Reflection/Reflection.h>

//--------------------------------------------------------
/// Declare Meta Class
///
/// Convenience macro to declare a static meta-class 
/// instance for the given type
///
/// @param Class type
//--------------------------------------------------------
#define DECLARE_META_CLASS(T)\
	typedef T ClassType;\
	static Reflect::CMetaClass* StaticMetaClass;\
	Reflect::CInstance* mpMetaInstance;\
	virtual Reflect::CInstance* GetMetaInstance() const\
	{\
		return mpMetaInstance;\
	}\
	virtual void SetMetaInstance(Reflect::CInstance* inpInstance)\
	{\
		mpMetaInstance = inpInstance;\
	}\
	virtual Reflect::CMetaClass* GetMetaClass() const\
	{\
		return StaticMetaClass;\
	}
//--------------------------------------------------------
/// Define Meta Class
///
/// Convenience macro to initialise a static meta-class 
/// instance for the given type
///
/// @param Class type
//--------------------------------------------------------
#define DEFINE_META_CLASS(T)\
	typedef T MetaClassType;\
	Reflect::CMetaClass* T::StaticMetaClass = Reflect::CMetaClass::Create<T>(#T);
//--------------------------------------------------------
/// Declare Property
///
/// Convenience macro to declare a reflectable property
/// including the meta property and variable
///
/// @param Property type
/// @param Property name
//--------------------------------------------------------
#define DECLARE_PROPERTY(TYPE, NAME)\
	protected:\
	TYPE NAME;\
	public:\
struct PropContainer##NAME\
		{\
		Reflect::CProperty* pProp;\
		PropContainer##NAME()\
			{\
			pProp = Reflect::CProperty::Create(#NAME, &ClassType::##NAME, #TYPE, ClassType::StaticMetaClass, ChilliSource::Reflect::PROP_READ_WRITE);\
			}\
		};\
		static PropContainer##NAME sProp##NAME;\
	protected:
//--------------------------------------------------------
/// Define Property with Accessors
///
/// Convenience macro to init a reflectable property
/// and setter and getter
///
/// @param Property type
/// @param Property name
/// @param Setter method
/// @param Getter method
//--------------------------------------------------------
#define DECLARE_PROPERTY_A(TYPE, NAME, SETTER, GETTER)\
		protected:\
		TYPE NAME;\
	public:\
struct PropContainer##NAME\
		{\
		Reflect::CProperty* pProp;\
		PropContainer##NAME()\
			{\
			pProp = Reflect::CProperty::Create(#NAME, &ClassType::##NAME, #TYPE, ClassType::StaticMetaClass, ChilliSource::Reflect::PROP_READ_WRITE);\
			pProp->SetAccessor(&ClassType::##SETTER);\
			pProp->SetAccessor(&ClassType::##GETTER);\
			}\
		};\
		static PropContainer##NAME sProp##NAME;\
	protected:
//--------------------------------------------------------
/// Define Property
///
/// Convenience macro to init a reflectable property
///
/// @param Property type
/// @param Property name
//--------------------------------------------------------
#define DEFINE_PROPERTY(NAME)\
	MetaClassType::PropContainer##NAME MetaClassType::sProp##NAME;
#else
#define DECLARE_META_CLASS(T) 
#define DEFINE_META_CLASS(T) 
#define DECLARE_PROPERTY(TYPE, NAME) TYPE NAME;
#define DEFINE_PROPERTY(NAME)
#define DECLARE_PROPERTY_A(TYPE, NAME, SETTER, GETTER) TYPE NAME;
#endif

#endif
