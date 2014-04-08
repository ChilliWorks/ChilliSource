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

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/QueryableInterface.h>

namespace ChilliSource
{
	namespace Core
	{
		///An IComponentProducer can provide a component factory to construct components
		class IComponentProducer
		{
		public:
			IComponentProducer(){}
			virtual ~IComponentProducer(){}
			
			CS_DECLARE_NAMEDTYPE(IComponentProducer);
			//Returns a reference to an ComponentFactory
			virtual ComponentFactory & GetComponentFactory(u32 inudwIndex) = 0;
			
			//Returns a pointer to an ComponentFactory
			virtual ComponentFactory * GetComponentFactoryPtr(u32 inudwIndex) = 0;
	
			virtual u32 GetNumComponentFactories() const = 0;
		};
	}
}

#endif
