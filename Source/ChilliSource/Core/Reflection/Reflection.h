//
//  Reflection.hpp
//  MoFlowSkeleton
//
//  Created by Scott Downie on 28/11/2011.
//  Copyright (c) 2011 Tag Games. All rights reserved.
//

#ifndef MO_FLOW_CORE_REFLECTION_REFLECTION_H_
#define MO_FLOW_CORE_REFLECTION_REFLECTION_H_

#include <ChilliSource/Core/Reflection/Property.h>
#include <ChilliSource/Core/Reflection/Instance.h>
#include <ChilliSource/Core/Reflection/MetaClass.h>
//#include <ChilliSource/Core/Reflection/Method.h>
#include <ChilliSource/Core/Reflection/Registry.h>

#ifdef ENABLE_MOFLOW_REFLECTION
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
			pProp = Reflect::CProperty::Create(#NAME, &ClassType::##NAME, #TYPE, ClassType::StaticMetaClass, moFlo::Reflect::PROP_READ_WRITE);\
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
			pProp = Reflect::CProperty::Create(#NAME, &ClassType::##NAME, #TYPE, ClassType::StaticMetaClass, moFlo::Reflect::PROP_READ_WRITE);\
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
