/*
 *  CustomCreator.cpp
 *  MoFlow
 *
 *  Created by Ian Copland on 09/01/2012.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLOW_CORE_CUSTOMCREATOR_H_
#define _MOFLOW_CORE_CUSTOMCREATOR_H_

#include <ChilliSource/Core/Base/FastDelegate.h>
#include <ChilliSource/Core/Base/StaticConstructor.h>

//==================================================
/// This should be used by child classes to specify
/// that this is the child the parent should create.
//==================================================
#define DECLARE_CREATABLE(inOutputType, inInputType) 																			\
	static inOutputType* _Create_();																							\
	DECLARE_STATIC_CONSTRUCTOR();

#define DEFINE_CREATABLE(inOutputType, inInputType)																				\
	inOutputType* inInputType::_Create_()																						\
	{																															\
		return new inInputType;																									\
	};																															\
	DEFINE_STATIC_CONSTRUCTOR(inInputType,																						\
	{																															\
		moFlo::ICustomCreator<inOutputType>::SetCreator(fastdelegate::FastDelegate0<inOutputType*>(&inInputType::_Create_));	\
	});

namespace moFlo
{
	//================================================
	/// This is used to allocate a base class without
	/// needing to know of the base classes type. If a
	/// parent class inherits from this, a child can
	/// then call DECLARE_CREATABLE() and DEFINE_CREATABLE().
    /// Any calls to Create() on the parent will now 
    /// create an instance of the child without needing 
    /// to know about the child.
	//================================================
	template<class Type> class ICustomCreator
	{
	public:
		//-------------------------------------------------
		/// Create
		///
		/// Instantiates an instance of the desired base
		/// class, cast to that of the parent.
		///
		/// @return the new object.
		//-------------------------------------------------
		static Type* Create()
		{
			if (GetCreatorRef())
            {
				return GetCreatorRef()();
            }
			return NULL;
		}
		//-------------------------------------------------
		/// Set Creator
		///
		/// Sets the method that should be used to create
		/// the child class.
		///
		/// @param the function
		//-------------------------------------------------
		static void SetCreator(fastdelegate::FastDelegate0<Type*> inCreatorDelegate)
		{
			GetCreatorRef() = inCreatorDelegate;
		}
    private:
        //-------------------------------------------------
		/// Creator
		///
		/// @return a reference to the creator delegate
        /// object.
		//-------------------------------------------------
        static fastdelegate::FastDelegate0<Type*>& GetCreatorRef()
        {
            static fastdelegate::FastDelegate0<Type*> sCreator;
            return sCreator;
        }
	};
}

#endif
