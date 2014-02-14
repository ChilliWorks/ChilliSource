/** 
 * File: SystemConcepts.h
 * Date: 27 Sept 2010
 * Description: Defines a number of abstract concepts systems can implement
 */

/** 
 *
 * Author Stuart McGaw
 * Version 1.0 - moFlo
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#ifndef _MOFLO_CORE_SYSTEMCONCEPTS_H_
#define _MOFLO_CORE_SYSTEMCONCEPTS_H_

#include <ChilliSource/Core/ForwardDeclarations.h>
#include <ChilliSource/Core/Main/QueryableInterface.h>

namespace moFlo
{
	namespace Core
	{
		///Something that can be updated
		class IUpdateable
		{
		public:
			IUpdateable(){}
			virtual ~IUpdateable(){}
			
			DECLARE_NAMED_INTERFACE(IUpdateable);
			//Update this thing given the passed time delta in seconds
			virtual void Update(f32 infDT) = 0;
		};
		
		
		///An IComponentProducer can provide a component factory to construct components
		class IComponentProducer
		{
		public:
			IComponentProducer(){}
			virtual ~IComponentProducer(){}
			
			DECLARE_NAMED_INTERFACE(IComponentProducer);
			//Returns a reference to an IComponentFactory
			virtual IComponentFactory & GetComponentFactory(u32 inudwIndex) = 0;
			
			//Returns a pointer to an IComponentFactory
			virtual IComponentFactory * GetComponentFactoryPtr(u32 inudwIndex) = 0;
	
			virtual u32 GetNumComponentFactories() const = 0;
		};
	}
}

#endif
