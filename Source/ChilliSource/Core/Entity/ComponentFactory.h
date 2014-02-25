/** 
 * File: ComponentFactory.h
 * Date: 29 Sept 2010
 * Description: Defines the interface ComponentFactories in moFlo must implement
 */

/** 
 *
 * Author Stuart McGaw
 * Version 1.0 - moFlo
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#ifndef _MOFLO_CORE_COMPONENTFACTORY_H
#define _MOFLO_CORE_COMPONENTFACTORY_H

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Entity/Component.h>

namespace ChilliSource
{
	namespace Core
	{	
		class IComponentFactory : public IQueryableInterface
		{
		public:
            
			IComponentFactory(){}
			virtual ~IComponentFactory(){}
			virtual bool CanProduceComponentWithInterface(InterfaceIDType inInterfaceID) const = 0;
			virtual bool CanProduceComponentWithTypeName(const std::string & incName) const = 0;
			
			//This is the method used to create a component from a data-description of same.
			virtual ComponentPtr CreateComponent(const std::string & insTypeName, const ChilliSource::Core::ParamDictionary & insParamDictionary){
				WARNING_LOG("Calling default ComponentFactory::CreateComponent with typename: " + insTypeName + ". Derived factories should override this method for their component classes.");
				return ComponentPtr();
			};  
			
		};
	}
	
}

#endif